#include <windows.h>
#include <WinBase.h>
#include <sddl.h>

LPCWSTR ConvertToLPCWSTR(const char* charString)
{
	int bufferSize = MultiByteToWideChar(CP_UTF8, 0, charString, -1, NULL, 0);
	wchar_t* wideString = new wchar_t[bufferSize];
	MultiByteToWideChar(CP_UTF8, 0, charString, -1, wideString, bufferSize);

	return wideString;
}

int main(int argc, char* argv[]) {
	PSID sid;
	ConvertStringSidToSid(L"S-1-5-32-544", &sid);

	PSID sid2;

	ConvertStringSidToSidW(ConvertToLPCWSTR(argv[1]), &sid2);


	PACL acl = (PACL)new char[1000];
	InitializeAcl(acl, 1000, ACL_REVISION);
	AddAccessAllowedAce(acl, ACL_REVISION, KEY_ALL_ACCESS, sid);
	AddAccessAllowedAce(acl, ACL_REVISION, KEY_ALL_ACCESS, sid2);


	SECURITY_DESCRIPTOR desc;
	InitializeSecurityDescriptor(&desc, SECURITY_DESCRIPTOR_REVISION);
	DWORD resu = SetSecurityDescriptorDacl(&desc, true, acl, false);
	DWORD rev;
	SECURITY_DESCRIPTOR_CONTROL control;
	/*	GetSecurityDescriptorControl(&desc, &control, &rev);
		control |= SE_DACL_PROTECTED;
		SetSecurityDescriptorControl(&desc, SE_DACL_PROTECTED, control);*/



	SECURITY_INFORMATION info;
	HKEY key;
	RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\WTI\\PRIVILEGED", &key);

	resu = RegSetKeySecurity(key, DACL_SECURITY_INFORMATION, &desc);
	RegCloseKey(key);
	delete[] acl;
	LocalFree(sid);
	LocalFree(sid2);
}