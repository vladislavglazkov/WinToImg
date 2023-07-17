#include "PipeOps.h"

struct Up {
	wchar_t cmdline[500];
	int xpos;
	int ypos;
	int delay;

};
struct Down {
	wchar_t guid[50];

};

struct Empty {
private: int _empty;
};

extern "C" __declspec(dllexport) void* OPS(LPWSTR cmdline,int xsize,int ysize,int delay) {



	Up data;
	ZeroMemory(&data, sizeof(data));
	wstring line(cmdline);
	line.copy(data.cmdline, line.length());
	data.xpos = xsize;
	data.ypos = ysize;
	data.delay = delay;

	Down downresult = SendViaPipe<Up, Down>(L"WTI-PIPE-MAIN", data);


	BITMAPINFO binfo;
	wstring wstr = downresult.guid;
	wstring guid = wstr;
	wstr = L"Global\\" + wstr;
	auto mapping = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, 10000000, wstr.c_str());

	auto infomapping = CreateFileMappingW(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, sizeof(BITMAPINFO), ((wstr + L"alpha").c_str()));
	auto ptr = MapViewOfFile(infomapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(binfo));
	CopyMemory(&binfo, ptr, sizeof(binfo));
	UnmapViewOfFile(ptr);
	void* temp;
	Sleep(50);

	Empty deleteresult = SendViaPipe<Empty, Empty>((L"WTI-PIPE-DELETE-" + guid).c_str(), Empty());

	auto mappingckc = OpenFileMappingW(PAGE_READWRITE, 0, wstr.c_str());



	HBITMAP sharedhbmp = CreateDIBSection(0, &binfo, DIB_RGB_COLORS, &temp, mapping, 0);


	return sharedhbmp;
}
