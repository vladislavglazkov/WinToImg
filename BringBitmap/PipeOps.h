#ifndef PIPEOPS_H
#define PIPEOPS_H



#include <iostream>
#include <Windows.h>
#include <WinBase.h>
#include <sddl.h>
#include <stdexcept>
#include <string>
#include <fstream>
using namespace std;
wofstream globallog("C:\\logging.txt");

struct AcceptViaPipeCallbackData {
	LPCWSTR pipename;
	void (*func)(char);
	void* arg;
};
template <typename UpType, typename DownType>
DWORD WINAPI AcceptViaPipeCallback(LPVOID arg) {
	//globallog << "IN CALLBACK"<<endl;
	SECURITY_DESCRIPTOR desc;
	InitializeSecurityDescriptor(&desc, SECURITY_DESCRIPTOR_REVISION);
	ACL* acl = (ACL*)new char[1000];
	InitializeAcl(acl, 1000, ACL_REVISION);
	PSID sid;
	ConvertStringSidToSid(L"S-1-1-0", &sid);
	AddAccessAllowedAce(acl, ACL_REVISION, GENERIC_READ | GENERIC_WRITE, sid);
	SetSecurityDescriptorDacl(&desc, true, acl, false);
	SECURITY_ATTRIBUTES atts;
	atts.bInheritHandle = false;
	atts.lpSecurityDescriptor = &desc;
	atts.nLength = sizeof(atts);

	HANDLE pipe = CreateNamedPipeW((wstring(L"\\\\.\\pipe\\") + ((AcceptViaPipeCallbackData*)arg)->pipename).c_str(), PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT | PIPE_REJECT_REMOTE_CLIENTS, PIPE_UNLIMITED_INSTANCES, 1000000, 1000000, 0, &atts);
	if (pipe == 0) {
		
		throw std::runtime_error("Couldn't create pipe. Win32 code: "+to_string(GetLastError()));
	}
	LocalFree(sid);
	delete[] acl;

	if (pipe == 0)
		globallog << "ERROR!"<<endl;
	auto succ=ConnectNamedPipe(pipe, 0);
	if (!succ) {
		throw std::runtime_error("Couldn't connect to pipe as server. Win32 code: " + to_string(GetLastError()));

	}
	CreateThread(0, 0, AcceptViaPipeCallback<UpType, DownType>, arg, 0, 0);

	globallog << "Connected"<<endl;
	int bufsize = sizeof(UpType);
	UpType buf;
	DWORD act;
	ReadFile(pipe, &buf, bufsize, &act, 0);
	globallog << "LET THE TRUTH OUT" << endl;
	globallog << *((wchar_t*)(&buf)) << endl;
	string ss = "OPS";
	//Sleep(0);


	typedef DownType(*funtype)(UpType,void*arg);
	auto func = reinterpret_cast<funtype> (((AcceptViaPipeCallbackData*)arg)->func);

	DownType res = func(buf, ((AcceptViaPipeCallbackData*)arg)->arg);

	WriteFile(pipe, &res, sizeof(DownType), &act, 0);
	CloseHandle(pipe);

	return 0;
}

template <typename UpType, typename DownType>
void AcceptViaPipe(LPCWSTR pipename, int simul, DownType(*func)(UpType,void*arg),void*arg) {
	AcceptViaPipeCallbackData* data=new AcceptViaPipeCallbackData();
	typedef void(*stantype)(char);

	data->pipename = pipename;
	data->func = reinterpret_cast<stantype>(func);
	data->arg = arg;
	for (int i = 0; i < simul; i++) {

		CreateThread(0, 0, AcceptViaPipeCallback<UpType, DownType>, data, 0, 0);

	}


}


template <typename UpType, typename DownType>
DownType SendViaPipe(LPCWSTR pipename, UpType data) {
	int upbufsize = sizeof(UpType);
	int downbufsize = sizeof(DownType);

	DownType res;
	DWORD act;
	auto succ=CallNamedPipeW((wstring(L"\\\\.\\pipe\\") + pipename).c_str(), &data, upbufsize, &res, downbufsize, &act, NMPWAIT_WAIT_FOREVER);
	if (!succ) {
		throw std::runtime_error("Couldn't connect to pipe as client. Win32 code: " + to_string(GetLastError()));

	}
	return res;

}
#endif