// Fill out your copyright notice in the Description page of Project Settings.

#include "Tools.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>

using namespace std;


void Tools::saveDataToFile(FString filePath, TArray<uint8> data)
{
	ofstream file(*filePath, ios::binary);

	char *cp = new char[data.Num()];
	for (int32 i = 0; i < data.Num(); ++i)
	{
		cp[i] = data[i];
	}
	file.write(cp, data.Num());
	file.close();
}

bool Tools::system_hide(TCHAR* CommandLine)
{
	SECURITY_ATTRIBUTES   sa;
	HANDLE   hRead, hWrite;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = true;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		return false;
	}
	STARTUPINFO   si;
	PROCESS_INFORMATION   pi;
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	//�ؼ����裬CreateProcess�����������������MSDN     
	if (!CreateProcess(NULL, CommandLine, NULL, NULL, true, NULL, NULL, NULL, &si, &pi))
	{
		return false;
	}
	CloseHandle(hWrite);
	char   buffer[4096] = { 0 };
	LPDWORD bytesRead = 0;
	while (true)
	{
		memset(buffer, 0, strlen(buffer));
		if (ReadFile(hRead, buffer, 4095, bytesRead, NULL) == NULL)
		{
			break;
		}
		//NOffer�о���ִ�еĽ�������Ա��浽�ı���Ҳ����ֱ�����     

		//printf(buffer);//����ע�͵��Ϳ�����    

		Sleep(100);
	}
	return true;
}


bool Tools::system_hide_nowait(TCHAR* CommandLine)
{
	SECURITY_ATTRIBUTES   sa;
	HANDLE   hRead, hWrite;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = true;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		return false;
	}
	STARTUPINFO   si;
	PROCESS_INFORMATION   pi;
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	if (!CreateProcess(NULL, CommandLine, NULL, NULL, true, NULL, NULL, NULL, &si, &pi))
	{
		return false;
	}
	CloseHandle(hWrite);
	char   buffer[4096] = { 0 };
	LPDWORD bytesRead = 0;
	return true;
}