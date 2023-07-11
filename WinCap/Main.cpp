#include <iostream>
#include <fstream>
#include <Windows.h>
#include <wtsapi32.h>
#include <string>
#include <WinBase.h>
#include <cwchar>

#pragma comment(lib,"wtsapi32.lib")
using namespace std;

struct IDS {

    HWND output = 0;
};
BOOL CALLBACK EnumWindowsCallback(HWND _hwnd, LPARAM lParam) {
    cout << lParam << endl;
    IDS* ops = (IDS*)lParam;
    cout << ops << endl;

    if (!ops->output) {
        RECT temp;
        GetClientRect(_hwnd, &temp);
        if (temp.right > temp.left && temp.bottom > temp.top)
            (ops->output) = _hwnd;

    }

    return TRUE;
}
void WaitTillWindowReady(int id, IDS* ids) {


    EnumThreadWindows(id, EnumWindowsCallback, (LPARAM)ids);
    while (ids->output == 0) {
        Sleep(200);

        EnumThreadWindows(id, EnumWindowsCallback, (LPARAM)ids);
    }
    //hwnd = ids->output;


}
ofstream off("C:\\temp\\log.txt");

void GetHandle(int thrid,void*mapping,void*infomapping) {
    IDS ids;
    WaitTillWindowReady(thrid, &ids);
    Sleep(1000);
    HWND hwnd = ids.output;
    auto windc = GetDC(hwnd);
    LPRECT winrect = new RECT();
    ZeroMemory(winrect, sizeof(winrect));

    auto rect = GetClientRect(hwnd, winrect);
    int cy = winrect->bottom - winrect->top;
    int cx = winrect->right - winrect->left;
    off << endl << cy << endl << cx;

    auto hbmp = CreateCompatibleBitmap(windc, cx, cy);
    BITMAP bmp;
    GetObject(hbmp, sizeof(BITMAP), &bmp);
    BITMAPINFO bmi;
    ZeroMemory(&bmi, sizeof(BITMAPINFO));
      
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = bmp.bmWidth;
    bmi.bmiHeader.biHeight = bmp.bmHeight;
    bmi.bmiHeader.biPlanes = bmp.bmPlanes;
    bmi.bmiHeader.biBitCount = bmp.bmBitsPixel;
    bmi.bmiHeader.biCompression = BI_RGB;
    void* temp;
    off << "BEFOREDIBSECTOIN" << endl;
    auto sharedhbmp = CreateDIBSection(0, &bmi, DIB_RGB_COLORS, &temp, mapping, 0);
    off << "AFTERDIBSECTION" << endl;


    void* ptr = MapViewOfFile(infomapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(bmi));
    if (ptr == 0) {
        off << "ERRR" << endl << GetLastError() << endl;
    }
    off << "AFTERMAPPING2" << endl;

    CopyMemory(ptr, &bmi, sizeof(bmi));
    off << "ONCOPY" << endl;
    UnmapViewOfFile(ptr);
    off << "Precreate" << endl;


    auto bmpdc = CreateCompatibleDC(windc);
    off << "Preselect" << endl;

    auto oldbmp = SelectObject(bmpdc, sharedhbmp);
    off << "PreCopied" << endl;

    BitBlt(bmpdc, 0, 0, cx, cy, windc, 0, 0, SRCCOPY);
    off << "COPIED" << endl;

    SelectObject(bmpdc, oldbmp);
    DeleteObject(hbmp);
    DeleteDC(bmpdc);
    ReleaseDC(hwnd, windc);

    

    return;
}

LPCWSTR ConvertToLPCWSTR(const char* charString)
{
    int bufferSize = MultiByteToWideChar(CP_UTF8, 0, charString, -1, NULL, 0);
    wchar_t* wideString = new wchar_t[bufferSize];
    MultiByteToWideChar(CP_UTF8, 0, charString, -1, wideString, bufferSize);

    return wideString;
}




int main(int argc, char* argv[]) {
    off << "Hi" << endl;

    cout << "Welcome\n";
    string ss;
    //cin >> ss;
    cout << argv[1] << endl;
    int res=stoi(argv[1]);
    

    string name1 = "Global\\" + string(argv[2]);
    auto mapping = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, 10000000, ConvertToLPCWSTR(name1.c_str()));
    off << mapping << endl;
   
    auto infomapping = CreateFileMappingW(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, sizeof(BITMAPINFO), ConvertToLPCWSTR((name1+ "alpha").c_str()));
    off << "Mappings good" << endl;

    cout << "GO IN\n";
    //void* mem = MapViewOfFile(mapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    GetHandle(res,mapping,infomapping);
    off << "Got Finals" << endl;
    CloseHandle(mapping);
    CloseHandle(infomapping);


    
    
    return 0;
}
