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

_declspec(dllexport) int D::GetWindowsText(HWND pWinHwnd, PSTR& pcText) {
  
    int cTxtLen = GetWindowTextLength(pWinHwnd);

    if (cTxtLen) {
        pcText = (PSTR)VirtualAlloc(
            (LPVOID)NULL,
            (DWORD)(cTxtLen + 1),
            MEM_COMMIT,
            PAGE_READWRITE);

        GetWindowText(
            pWinHwnd,
            pcText,
            cTxtLen + 1);
    }

    // Allocate memory for the string and copy 
    // the string into the memory. 
    
    // Free the memory and return. 
    //VirtualFree(pcText, 0, MEM_RELEASE);
    return cTxtLen;
}



_declspec(dllexport) bool D::ReadProcessMemoryEx(
    HANDLE& hProcess,
    LPCVOID lpBaseAddress,
    LPVOID& lpBuffer,
    SIZE_T nSize,   
    DWORD dwDesiredAccess = NULL
) {
    bool bIsClearProcess = FALSE;
    if (hProcess == NULL && dwDesiredAccess != NULL) {
        bIsClearProcess = TRUE;
        hProcess = OpenProcess(
            PROCESS_ALL_ACCESS,     // 所有权限
            FALSE,                  // 不继承
            dwDesiredAccess
        );
    }

    if (hProcess == NULL) {
        D::OutputDebugStringEx("\n ERROR_INFO OpenProcess | %d", GetLastError());
        return FALSE;
    }

    if (!ReadProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, NULL)) {
        return FALSE;
    }

    return TRUE;
}

HWND D::AbstractWindow::Create() {
    D::OutputDebugStringEx("=================================");
    D::OutputDebugStringEx("\n lpClassName:[%s] lpWindowName:[%s]", this->lpClassName, this->lpWindowName);
    HWND hwnd = CreateWindow(
        (LPCSTR)this->lpClassName,  // Predefined class; Unicode assumed 
        (LPCSTR)this->lpWindowName,      // Button text 
        this->dwStyle, // WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        this->x,         // x position 
        this->y,         // y position 
        this->nWidth,        // Button width
        this->nHeight,        // Button height
        this->hWndParent,     // Parent window
        this->hMenu,       // No menu.
        this->hInstance,
        this->lpParam);      // Pointer not needed.
    return hwnd;
}