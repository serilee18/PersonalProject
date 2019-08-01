#include <windows.h>

char value;


void RS232_Comm(unsigned char data)
{
 LPCSTR lpszCommName = "COM5";  
 DCB          dcb;    
 COMMPROP     cp;     
 HANDLE       hComm;  
 COMSTAT      cs;     
 DWORD        dwErrorMask;  
 int          Status; 
 //---variables for Comm. Transmit -WriteFile()
 unsigned char    OutBuff[100];  
 unsigned long    nByteToWrite;  
 unsigned long    nByteWritten;  


 if((hComm = CreateFile(lpszCommName, GENERIC_READ|GENERIC_WRITE, 0, NULL,
                        OPEN_EXISTING, 0, NULL)) == INVALID_HANDLE_VALUE) { 
  printf("Error opening COM port\n");
  //return FALSE;
 }//if

 
 cp.wPacketLength = sizeof(COMMPROP); 
 if(!GetCommProperties(hComm, &cp)) 
  printf("error GetCommProperties() !\n");

 //       ...Cautiion : position of this routine = different from BOOK(= at WM_COMMAND = Error! -I don't know WHY?).
 if(!GetCommState(hComm, &dcb)) //get DCB data (COM port data)
  printf("error GetCommState() !\n");

 dcb.BaudRate = CBR_38400;   //COM data : others=default value!
 dcb.ByteSize = 8;
 dcb.Parity   = 0;
 dcb.StopBits = ONESTOPBIT;

 if(!SetCommState(hComm, &dcb)) //set COM port
  printf("error SetCommState() : COM port init error!\n");

 //////// Send data to COM port : =
  printf("====Send Data to RS232 Comm. port !!\n");

// strcpy(OutBuff, "SUCC !!"); 
 OutBuff[0] = data;
 nByteToWrite = 1; 
 Status = WriteFile(hComm, OutBuff, nByteToWrite, &nByteWritten, NULL); 
    
 if(Status == 0) { //WriteFile() 
  GetLastError();   //get last-error code
  ClearCommError(hComm, &dwErrorMask, &cs); //
 }                     
 PurgeComm(hComm, PURGE_RXCLEAR);
 //===Comm END
 CloseHandle(hComm);
 //--
}//RS232_Comm()
