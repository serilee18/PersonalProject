
void getFingerCount(IplImage *image)
{
	CvMemStorage* contour_storage = cvCreateMemStorage(0);
	CvSeq* contour = 0;
	
	double max_arccont = 0; 
	
	double min_arccont = 400.0; 
	double area = 0;
	double arcCont = 0;
	CvSeq *max_contour = 0;
	
	int temp_finger_count = 0;
	
	cvFindContours( image, contour_storage, &contour, sizeof(CvContour), CV_RETR_CCOMP , CV_CHAIN_APPROX_SIMPLE);
	
	CvScalar inColor = CV_RGB( 255, 255, 255 ); 
	
	for(;contour!=0; contour=contour->h_next)
	{
		area = fabs(cvContourArea(contour, CV_WHOLE_SEQ)); 
		cvDrawContours( image, contour, inColor, inColor, -1, CV_FILLED, 8 );
		temp_finger_count++;
	}
	
	finger_count = temp_finger_count;

	cvReleaseMemStorage(&contour_storage);
	
}

CvPoint getCenter(IplImage* oriImg)
{
	unsigned char* image = (unsigned char*)(oriImg->imageData);
	int widthStep = oriImg->widthStep;
	int width = oriImg->width;
	int height = oriImg->height;
	int x;//, i;
	int y;

	const int definedValue = 255;
	const int backValue = 0;
	
	int sumX = 0;
	int sumY = 0;
	cntDefPx = 0;
	
	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			if(image[IMGARR1(x, y, widthStep)] == definedValue)
			{
				sumX += x;
				sumY += y;
				cntDefPx++;
			}
		}
	}
	
	CvPoint result;
	
	if(cntDefPx == 0)
	{
		result.x = 0;
		result.y = 0;
	}
	else
	{
		result.x = sumX / cntDefPx;
		result.y = sumY / cntDefPx;
	}
	return result;
}


void deleteEdge(IplImage* sourceImage, IplImage* edgeImage)
{
	int sWS = sourceImage->widthStep;
	int eWS = edgeImage->widthStep;
	
	unsigned char* sImage = (unsigned char*)sourceImage->imageData;
	unsigned char* eImage = (unsigned char*)edgeImage->imageData;
	
	int width = sourceImage->width;
	int height = sourceImage->height;
	
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			if(eImage[IMGARR1(x, y, eWS)] == 255)
			{
				sImage[IMGARR1(x, y, sWS)] = 0;
			}
		}
	}
}


void NoiseReduction(IplImage* img,int range=5,float rate=0.5f)
{
	
	
	int i,j,k,index;
	int x1,y1,x2,y2;
	unsigned int tempSum,threshold=static_cast<unsigned int>((range*2+1)*(range*2+1)*rate);
	CvRect rect=cvGetImageROI(img);
	//static unsigned int *countMap = new unsigned int [rect];
	static unsigned int *countMap=new unsigned int[rect.height*rect.width];
	

	memset(countMap,0,rect.height*rect.width*sizeof(unsigned int));
	for(i=rect.y,k=0;i<rect.y+rect.height;i++)
		for(j=rect.x;j<rect.x+rect.width;j++,k++)
		{
			index=i*img->widthStep+j*img->nChannels;
			if(img->imageData[index] != 0 && img->imageData[index+1] == 0 && img->imageData[index+2] == 0)
				countMap[k]=1;
		}
		
		// dynamic table 구성
		for(j=1;j<rect.width;j++)
			countMap[j]+=countMap[j-1];
		for(i=1;i<rect.height;i++)
			countMap[i*rect.width]+=countMap[(i-1)*rect.width];
		for(i=1;i<rect.height;i++)
			for(j=1;j<rect.width;j++)
				countMap[i*rect.width+j]+=countMap[(i-1)*rect.width+j]+countMap[i*rect.width+j-1]
			-countMap[(i-1)*rect.width+j-1];
			// -range by +range 크기의 합 구하기
			for(i=0;i<rect.height;i++)
			{
				y1=(i-range<0) ? 0 : i-range;
				y2=(i+range>=rect.height) ? rect.height-1 : i+range;
				for(j=0;j<rect.width;j++)
				{
					x1=(j-range<0) ? 0 : j-range;
					x2=(j+range>=rect.width) ? rect.width-1 : j+range;
					
					tempSum=countMap[y2*rect.width+x2];
					if(x1-1>=0)
						tempSum-=countMap[y2*rect.width+x1-1];
					if(y1-1>=0)
						tempSum-=countMap[(y1-1)*rect.width+x2];
					if(x1-1>=0 && y1-1>=0)
						tempSum+=countMap[(y1-1)*rect.width+x1-1];
					
					index=(rect.y+i)*img->widthStep+(rect.x+j)*img->nChannels;
					if(tempSum>threshold)
					{
						img->imageData[index+2]=0;
						img->imageData[index+1]=0;
						img->imageData[index]=definedValue;
					}
					else
					{
						img->imageData[index+2]=0;
						img->imageData[index+1]=0;
						img->imageData[index]=0;
					}
				}
			}
		//cvReleaseImage(&img);

		}

		void ch3To1ch(IplImage *image, IplImage *dst_image)
		{
			unsigned char* img = (unsigned char*)(image->imageData);
			int widthStep = image->widthStep;
			int width = image->width;
			int height = image->height;

			for(int y = 0; y < height; y++)
			{
				for(int x = 0; x < width; x++)
				{
					if(img[IMGARR(x, y, widthStep)] == 255)
						dst_image->imageData[(y*(dst_image->widthStep)) + x] = definedValue;

				}
			}

		}

		void separateFinger(IplImage *img, CvPoint center)
		{
			unsigned char* image = (unsigned char*)(img->imageData);
			unsigned char* part_image = (unsigned char*)(finger_part->imageData);

			int widthStep = img->widthStep;
			int width = img->width;
			int height = img->height;
			int ceta; 
			int dist = 125;
			int x, y;
			int match_count = 0;
			int temp_count = 0;
			
			
			temp_func_flag = false;
			
			double radian;
			
			for(ceta = 0; ceta <= 360; ceta++)
			{
				radian = ceta * TRANS2RAD;
				
				if(ceta >= 45 && ceta < 200) 
				{
					continue;
					
				}
				
				else if((ceta >= 0 && ceta <= 45) || (ceta >= 285 && ceta <= 360))
				{
					for(dist = 125; dist <= 135; dist++)			
					{
						x = (int)(dist*cos(radian)) + center.x;
						y = (int)(dist*sin(radian)) + center.y;
						
						if(x <= 0 || x >= width)
							continue;
						if(y <= 0 || y >= height)
							continue;
						
						if(image[IMGARR1(x, y, widthStep)] == definedValue) 
						{
							match_count++;
							part_image[IMGARR1(x, y, widthStep)] = definedValue;
						}
						
						if(ceta == 360 && match_count > 30) 
						{	
							match_count = 0;
							temp_func_flag = true;
							func_count++;
						}
					}
					
				}
				else 
				{
					for(dist = 125; dist <= 135; dist++)
					{
						x = (int)(dist*cos(radian)) + center.x;
						y = (int)(dist*sin(radian)) + center.y;
						
						if(x <= 0 || x >= width)
							continue;
						if(y <= 0 || y >= height)
							continue;
						
						if(image[IMGARR1(x, y, widthStep)] == definedValue) 
						{
							match_count++;
							part_image[IMGARR1(x, y, widthStep)] = definedValue;
						}
						else 
						{
							if(match_count > 10)
							{
								temp_count++;
								match_count = 0;
								dist = 125;
							}
						}
						
					}
					
					
				}
		
		
		
		
	}
	
}


void copy1chTo3ch(IplImage *image, IplImage *dst_image2)
{
	int widthStep = image->widthStep;
	int widthStep2 = dst_image2->widthStep;
	
	unsigned char* img = (unsigned char*)image->imageData;
	unsigned char* dst_img = (unsigned char*)dst_image2->imageData;
	
	int width = image->width;
	int height = image->height;
	
	int width2 = dst_image2->width;
	int height2 = dst_image2->height;
	
	

	
	for(int y = 0; y < height2; y++)
	{
		for(int x = 0; x < width2; x++)
		{
			if(dst_img[IMGARR1(x, y, widthStep2)] == definedValue)
			{
				img[IMGARR(x, y, widthStep)] = 255;
				img[IMGARR(x, y, widthStep)+1] = 255;
				img[IMGARR(x, y, widthStep)+2] = 255;
			}
			
		}
	}
	
	
}
