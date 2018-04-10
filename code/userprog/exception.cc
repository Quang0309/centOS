// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------





void ExceptionHandler(ExceptionType which)
{
	int type = machine->ReadRegister(2);
	if (which == SyscallException)
	{
		switch (type)
		{
			case SC_Halt:
			{
				DEBUG('a',"shutdown, initiated by user program");
			interrupt->Halt();
			break;
			}
			case SC_Compare:
				{
				int op1;
				op1 = machine->ReadRegister(4);
				int op2;
				op2 = machine->ReadRegister(5);
				int compare;
				compare = op1 - op2;
				if (compare > 0)
					compare = 1;
				else if (compare < 0)
					compare = -1;
				else 
					compare = 0;
				machine->WriteRegister(2,compare);
				
				//interrupt->Halt();
				break;
				}
				
			case SC_Readchar:
				{
				
				char c[2];		//The first element is the actual character that the user input, the second element is the ‘enter’
				synchConsole->Read(c,2);	
				machine->WriteRegister(2,c[0]); // so we just ignore it. Pass the c[0] to the register r2.
				//interrupt->Halt();
				break;
				}
				
			
				
			case SC_Printchar:
				{
				char d;
				d = (char) machine->ReadRegister(4);
				synchConsole->Write(&d,1);	//-	synchConsole will then read from the address of d with the offset of 1 (to ensure the writing of one character) and write to console.
				//machine->WriteRegister(2,int(0));
				//interrupt->Halt();
				break;	
				}
				
			case SC_Readint:	
				{	
				int num = 0;
				int length;
				char * ch = new char[11];	//-	The ‘ch’ has the length 11 because MAX_INT has 10 digit and we must take the minus into consideration
				length = synchConsole->Read(ch,11);
				for(int i = length -1,  j = 1 ;i>=0;i--,j = j*10)	//.  So we let i go from length-1 to 0, calculate the actual number from each element of ‘ch’ 
				{
					if(ch[i]=='-')
						num = num * -1;
					else
					{
						num = num + (ch[i] - '0')*j;
					}
				}
				machine->WriteRegister(2,num);
				delete ch;
				break;
				}
				
			case SC_Printint:
				{
					char s[9];
					int n;
					n = machine->ReadRegister(4);
					if (n < 0){
						n = -n;
						synchConsole->Write("-",1);
					}
					int i =0;
					do {						//-	The do-while-loop extracts each individual digit from the integer and put them into the string s, in reverse order, then a ‘\n’ signaling the ending of the string is added to s.
						s[i++] = n%10 + '0';
					} while ((n /= 10) > 0);
					
					s[i] = '\n';
	
					for ( int j = 0; j < i/2; ++j){	// The for-loop is used to reverse the string back to the correct order.
						char tmp = s[i-j-1];
						s[i-j-1] = s[j];
						s[j] = tmp;
					}
					synchConsole->Write(s,i);
					break;
				}
				
			case SC_Readstring:
					{
						char * buffer = new char[1000];
						int addr = machine->ReadRegister(4);
						int length = machine->ReadRegister(5);
						int size = synchConsole->Read(buffer,length);	
						machine->System2User(addr,size,buffer);	//. Remember that the address in the register 4 is in the user space and buffer is in the kernel space
						delete buffer;
						break;
					}
					
			case SC_Printstring: 
					{
						
						int addrStr = machine->ReadRegister(4);
						char *buf = new char [1000];
						
						buf = machine->User2System(addrStr, 1000);
						int i = 0;
						while(buf[i] != 0 && buf[i] != '\n'){	//the loops only stops when there is a 0 (in ASCII) or a ‘\n’ which signal the ending of the string.
							synchConsole->Write(buf+i,1);
							++i;
						}
						buf[i] = '\n';
						synchConsole->Write(buf+i, 1);

						delete []buf;
						break;
				}
				case SC_CreateFile:
					{
						int addStr = machine->ReadRegister(4);
						char *buf =  new char[1000];
						buf = machine->User2System(addStr,1000);
						bool b = fileSystem->Create(buf,0);
						delete []buf;
						if (b==true)
							machine->WriteRegister(2,0);
						else
							machine->WriteRegister(2,-1);
						break;
					}
				case SC_OpenFile:
				{
					int addr = machine->ReadRegister(4);
					char *buf = new char[1000];
					int type = machine->ReadRegister(5);
					if(fileSystem->cur > 10)
					{
						machine->WriteRegister(2,-1);
						delete []buf;
						break;
					}
					buf = machine->User2System(addr,1000);
					if (strcmp(buf,"stdin")==0)
					{
						machine->WriteRegister(2, 0);
						delete []buf;
						break;
					}
					if (strcmp(buf,"stdout")==0)
					{
						machine->WriteRegister(2, 1);
						delete []buf;
						break;
					}
					OpenFile* temp = fileSystem->Open(buf,type);
					if (temp != NULL)
					{
						fileSystem->data[fileSystem->cur-1] = temp;
						machine->WriteRegister(2,fileSystem->cur-1);
						delete []buf;
						break;
					}
					machine->WriteRegister(2,-1);
					delete []buf;
					break;
				}
		}
		 machine->registers[PrevPCReg] = machine->registers[PCReg];	// for debugging, in case we
						// are jumping into lala-land
    		machine->registers[PCReg] = machine->registers[NextPCReg];
   		 machine->registers[NextPCReg] += 4;
		
	}
}
