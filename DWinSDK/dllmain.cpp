// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "DWinSDK.h"

_declspec(dllexport) void D::OutputDebugStringEx(const wchar_t* strOutputString, ...)
{
    va_list vlArgs = NULL;
    va_start(vlArgs, strOutputString);
    size_t nLen = _vscwprintf(strOutputString, vlArgs) + 1;
    wchar_t* strBuffer = new wchar_t[nLen + 8];
    _vsnwprintf_s(strBuffer, nLen, nLen, strOutputString, vlArgs);
    va_end(vlArgs);
    OutputDebugStringW(strBuffer);
    delete[] strBuffer;
}

_declspec(dllexport) void D::OutputDebugStringEx(const char* strOutputString, ...)
{
    va_list vlArgs = NULL;
    va_start(vlArgs, strOutputString);
    size_t nLen = _vscprintf(strOutputString, vlArgs) + 1;
    char* strBuffer = new char[nLen + 8];
    _vsnprintf_s(strBuffer, nLen, nLen, strOutputString, vlArgs);
    va_end(vlArgs);
    OutputDebugStringA(strBuffer);
    delete[] strBuffer;
}


HWND D::AbstractControl::Create() {
    HWND hwnd = CreateWindow(
        (LPCSTR) "BUTTON",  // Predefined class; Unicode assumed 
        (LPCSTR)"OK",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        10,         // x position 
        10,         // y position 
        100,        // Button width
        100,        // Button height
        this->hWndParent,     // Parent window
        NULL,       // No menu.
        this->hInstance,
        NULL);      // Pointer not needed.
    return hwnd;
}