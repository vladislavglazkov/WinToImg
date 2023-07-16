#include <string>
#include <Windows.h>
#include <WinBase.h>
#include <iostream>
#include <iomanip>
#include <random>
#include "PipeOps.h"
#include <wtsapi32.h>
#pragma comment(lib,"wtsapi32.lib")
#include <fstream>
using namespace std;
LPCWSTR ConvertToLPCWSTR(const char* charString)
{
	int bufferSize = MultiByteToWideChar(CP_UTF8, 0, charString, -1, NULL, 0);
	wchar_t* wideString = new wchar_t[bufferSize];
	MultiByteToWideChar(CP_UTF8, 0, charString, -1, wideString, bufferSize);

	return wideString;
}

struct Up {
	wchar_t cmdline[500];
	
	int xpos;
	int ypos;
	int delay;
};
struct Down {
	wchar_t guid[50];
	

};
struct CallbackArgs {
	HANDLE token;
};



/*extern "C" __declspec(dllexport) void* OPS(HANDLE token, int thrid) {
	TOKEN_PRIVILEGES privs;
	privs.PrivilegeCount = 1;
	LUID luid;
	LookupPrivilegeValue(NULL, SE_CREATE_GLOBAL_NAME, &luid);
	privs.Privileges[0].Luid = luid;
	privs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(token, false, &privs, sizeof(privs), 0, 0);


	wstring name1 = L"aspodckasp";

	


	

	
	STARTUPINFO sinfo;
	PROCESS_INFORMATION pinfo;
	ZeroMemory(&sinfo, sizeof(sinfo));
	sinfo.cb = sizeof(sinfo);
	
	wstring param(L"C:\\WinCap.exe ");
	param += to_wstring(thrid);
	param += L" ";
	param += wstring(name1);
	auto res=CreateProcessAsUser(token,L"C:\\WinCap.exe", &param[0], 0, 0, 0, 0, 0, 0, &sinfo, &pinfo);
	auto err=GetLastError();
	WaitForSingleObject(pinfo.hProcess, INFINITE);


	BITMAPINFO binfo;
	auto ptr=MapViewOfFile(infomapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(binfo));
	CopyMemory(&binfo, ptr, sizeof(binfo));
	UnmapViewOfFile(ptr);
	void* temp;
	HBITMAP sharedhbmp = CreateDIBSection(0, &binfo, DIB_RGB_COLORS, &temp, mapping, 0);
	return sharedhbmp;

	
}*/


std::wstring GenerateGUIDString() {
	


	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 15);

	const wchar_t* hexDigits = L"0123456789ABCDEF";
	std::wstring guidString;

	for (int i = 0; i < 32; ++i) {
		guidString += hexDigits[dis(gen)];
	}
	return guidString;
}

struct Empty {
private: int _empty;
};
struct DeleteCallbackData {
	HANDLE map1, map2;
};
Empty DeleteCallback(Empty data, void* argsraw) {
	DeleteCallbackData* args = (DeleteCallbackData*)argsraw;
	CloseHandle(args->map1);
	CloseHandle(args->map2);
	delete args;
	return Empty();
}

DWORD WINAPI TEST(LPVOID arg) {
	globallog << "TEST PASSED" << endl;
	return 0;
}

void CleanUpNameCallback(void* arg) {
	delete[] arg;
	globallog << L"Released\n";
}

Down Callback(Up data, void* argsraw) {

	/*CallbackArgs* cbcargs = new CallbackArgs();
	cbcargs->token = NULL;
	AcceptViaPipe<Up, Down>(L"WTI-PIPE-DELETE-A12B662", 1, false, Callback, cbcargs);*/
	



	globallog << 123 << endl;
	globallog << data.cmdline << ' ' << data.xpos << ' ' << data.ypos << ' ' << data.delay << endl;
	CallbackArgs* args=(CallbackArgs*)argsraw;
	SECURITY_DESCRIPTOR desc;
	InitializeSecurityDescriptor(&desc, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&desc, true, NULL, false);

	SECURITY_ATTRIBUTES atrs;
	atrs.bInheritHandle = false;
	atrs.lpSecurityDescriptor = &desc;
	atrs.nLength = sizeof(SECURITY_ATTRIBUTES);

	wstring guid = GenerateGUIDString();
	auto infomapping = CreateFileMapping(INVALID_HANDLE_VALUE, &atrs, PAGE_READWRITE, 0, sizeof(BITMAPINFO), (L"Global\\" + guid + L"alpha").c_str());
	auto mapping = CreateFileMapping(INVALID_HANDLE_VALUE, &atrs, PAGE_READWRITE, 0, 10000000, (L"Global\\" + guid).c_str());

	STARTUPINFO sinfo;
	PROCESS_INFORMATION pinfo;
	ZeroMemory(&sinfo, sizeof(sinfo));
	ZeroMemory(&pinfo, sizeof(pinfo));
	sinfo.cb = sizeof(sinfo);
	globallog << "PRE-REACHED" << endl;

	
	globallog << "REACHED" << endl;
	
	
	auto res = CreateProcessAsUser(args->token,0, data.cmdline, 0, 0, 0, 0, 0, 0, &sinfo, &pinfo);
	int thrid = pinfo.dwProcessId;
	
	
	ZeroMemory(&sinfo, sizeof(sinfo));
	ZeroMemory(&pinfo, sizeof(pinfo));
	sinfo.cb = sizeof(sinfo);
	
	char* dirbuf=0;
	DWORD directorysz=0;
	RegGetValue(HKEY_LOCAL_MACHINE, L"SOFTWARE\\WTI", L"filesdir", RRF_RT_ANY, 0, dirbuf, &directorysz);
	dirbuf = new char[directorysz];
	RegGetValue(HKEY_LOCAL_MACHINE, L"SOFTWARE\\WTI", L"filesdir", RRF_RT_ANY, 0, dirbuf, &directorysz);

	wstring dirstr((wchar_t*)dirbuf);
	delete[] dirbuf;
	globallog << dirstr<<endl;
	wstring param((L"\""+ dirstr + L"WinCap.exe"+ L"\" "));
	param += to_wstring(thrid);
	param += L" ";
	param += wstring(guid);
	param += L" ";
		

	param += to_wstring(data.xpos);
	param += L" ";
	param += to_wstring(data.ypos);
	param += L" ";
	param += to_wstring(data.delay);

	wofstream woffer("C:\\superlog.txt");
	woffer << param;
	res = CreateProcessAsUser(args->token, (dirstr + L"WinCap.exe").c_str(), &param[0], 0, 0, 0, 0, 0, 0, &sinfo, &pinfo);
	auto err = GetLastError();
	WaitForSingleObject(pinfo.hProcess, INFINITE);
	
	Down ans;
	ZeroMemory(&ans, sizeof(ans));
	guid.copy(&(ans.guid[0]), guid.length());


	DeleteCallbackData* d3 = new DeleteCallbackData();
	d3->map1 = 0;
	d3->map2 = 0;
	//AcceptViaPipe<Up, Down>(L"WTI-PIPE-DELETE-A12B66234", 1, false, Callback, d3);


	DeleteCallbackData* d2 = new DeleteCallbackData();
	d2->map1 = mapping;
	d2->map2 = infomapping;
	wstring forcbc = L"WTI-PIPE-DELETE-" + guid;
	wchar_t* forcbcbuf = new wchar_t[forcbc.length() + 1];
	CopyMemory(forcbcbuf, &forcbc[0], (sizeof(wchar_t)) * (forcbc.length() + 1));
	AcceptViaPipe<Empty, Empty>(forcbcbuf, 1, false, DeleteCallback, d2, CleanUpNameCallback, forcbcbuf);
	
	return ans;


}


extern "C" __declspec(dllexport) void StartListen(HANDLE token) {
	CallbackArgs* cbcargs = new CallbackArgs();
	cbcargs->token = token;
	
	typedef void (*FunPtr)(void*);
	FunPtr nptr = 0;
	AcceptViaPipe<Up,Down>(L"WTI-PIPE-MAIN", 30,true, Callback,cbcargs,nptr,NULL);

}


extern "C" __declspec(dllexport) int GetSessionId(LPCWSTR username) {
	ofstream off("C:\\GSI.log");
	WTS_SESSION_INFO* infos;
	DWORD size = 0;
	int ans = -1;
	
	for (const wchar_t* idx = username; *idx != 0; idx++) {
		off << (int)(*idx)<<' ';
	}
	off << endl << endl;
	WTSEnumerateSessions(0, 0, 1, &infos, &size);
	for (int i = 0; i < size; i++) {
		LPWSTR buf;
		DWORD bufsize;
		WTSQuerySessionInformation(0, infos[i].SessionId, WTSUserName, &buf, &bufsize);
		for (const wchar_t* idx = buf; *idx != 0; idx++) {
			off << (int)(*idx) << ' ';
		}
		off << endl << endl;
		if (lstrcmpi(username, buf)==0) {
			ans = infos[i].SessionId;
		}
		WTSFreeMemory(buf);
	}
	WTSFreeMemory(infos);
	return ans;
}