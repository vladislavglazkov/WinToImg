#include <string>
#include <Windows.h>
#include <WinBase.h>
using namespace std;
LPCWSTR ConvertToLPCWSTR(const char* charString)
{
	int bufferSize = MultiByteToWideChar(CP_UTF8, 0, charString, -1, NULL, 0);
	wchar_t* wideString = new wchar_t[bufferSize];
	MultiByteToWideChar(CP_UTF8, 0, charString, -1, wideString, bufferSize);

	return wideString;
}


extern "C" __declspec(dllexport) void* OPS(HANDLE token,int thrid) {
	TOKEN_PRIVILEGES privs;
	privs.PrivilegeCount = 1;
	LUID luid;
	LookupPrivilegeValue(NULL, SE_CREATE_GLOBAL_NAME, &luid);
	privs.Privileges[0].Luid = luid;
	privs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(token, false, &privs, sizeof(privs), 0, 0);


	wstring name1 = L"aspodckasp";

	SECURITY_DESCRIPTOR desc;
	InitializeSecurityDescriptor(&desc, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&desc, true, NULL, false);

	SECURITY_ATTRIBUTES atrs;
	atrs.bInheritHandle = false;
	atrs.lpSecurityDescriptor = &desc;
	atrs.nLength = sizeof(SECURITY_ATTRIBUTES);


	auto infomapping = CreateFileMapping(INVALID_HANDLE_VALUE, &atrs, PAGE_READWRITE, 0, sizeof(BITMAPINFO), (L"Global\\"+name1 + L"alpha").c_str());
	auto mapping = CreateFileMapping(INVALID_HANDLE_VALUE, &atrs, PAGE_READWRITE, 0, 10000000, (L"Global\\"+name1).c_str());

	
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

	
}