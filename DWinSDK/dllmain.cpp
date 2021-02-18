// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "DWinSDK.h"
#define MAX_LOADSTRING 100

HMODULE HookDll;
TCHAR szWindowClass[MAX_LOADSTRING]{"CHEATWIN"};
D::MainControl* pCheatWin = NULL;



LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (code == HC_ACTION) {
        if (wParam == VK_HOME) {
            if (lParam >> 0x1F & 1) {
                // D::OutputDebugStringEx("\n ========= HOOK info =========");
                
                if (!pCheatWin) {
                    CreateThread(
                        NULL,
                        0,
                        ThreadProc,
                        NULL,
                        0,
                        NULL
                    );
                
                }
                MessageBox(NULL, "OKXXdsffsdfds", NULL, MB_OK);
                

            }
        }
    }
    return CallNextHookEx(0, code, wParam, lParam);
}

DWORD WINAPI ThreadProc(_In_ LPVOID lpParameter)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = HookWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = NULL;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = NULL;

    RegisterClassEx(&wcex);

    pCheatWin = new D::MainControl(szWindowClass, (LPTSTR)TEXT("作弊引擎"), NULL);

    HWND hWnd = NULL;
    pCheatWin->getHwnd(hWnd);
    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK HookWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 分析菜单选择:
        switch (wmId)
        {
        case BN_CLICKED:
            MessageBox(NULL, "OKXXdsffsdfds", NULL, MB_OK);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

_declspec(dllexport) bool D::SetHookEx(int idHook, DWORD dwPid)
{
    /* 使用
     HWND FindHwnd = FindWindowA(TEXT("Sword2 Class"), TEXT("Sword2 Window"));
                    DWORD dwPid;
                    DWORD dwTid = GetWindowThreadProcessId(FindHwnd, &dwPid);
     D::SetHookEx(WH_KEYBOARD, dwTid);
    */
#ifdef _DEBUG

#ifdef _WIN64
    HookDll = LoadLibrary(TEXT("..\\x64\\Debug\\DWinSDK.dll"));
#else
    HookDll = LoadLibrary(TEXT("..\\Debug\\DWinSDK.dll"));
#endif // _WIN64

#else

#ifdef _WIN64
    HookDll = LoadLibrary(TEXT("..\\x64\\Release\\DWinSDK.dll"));
#else
    HookDll = LoadLibrary(TEXT("..\\Release\\DWinSDK.dll"));
#endif // _WIN64

#endif // _DEBUG

    pFuncGetHookInfo pGetHookInfo = (pFuncGetHookInfo)GetProcAddress(HookDll, "?GetHookInfo@@YAXPAUHookInfo@@@Z");
    HookInfo info;
    pGetHookInfo(&info);
    SetWindowsHookEx(idHook, info.proc, HookDll, dwPid);
    return true;
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

_declspec(dllexport) void GetHookInfo(HookInfo* pHookInfo)
{
    pHookInfo->proc = KeyboardProc;
}

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



int D::AbstractWindow::GetText(PSTR& pcText)
{   
    int cTxtLen = GetWindowTextLength(this->wHwnd);

    if (cTxtLen) {
        pcText = (PSTR)VirtualAlloc(
            (LPVOID)NULL,
            (DWORD)(cTxtLen + 1),
            MEM_COMMIT,
            PAGE_READWRITE);

        GetWindowText(
            this->wHwnd,
            pcText,
            cTxtLen + 1);
    }
    // Allocate memory for the string and copy 
    // the string into the memory. 

    // Free the memory and return. 
    //VirtualFree(pcText, 0, MEM_RELEASE);
    return cTxtLen;
}
bool D::AbstractWindow::SetText(AbstractWindow& aWin)
{
    PSTR pcText;
    int textLen = aWin.GetText(pcText);
    if (textLen) {
        SetWindowText(this->wHwnd, pcText);
    }
    VirtualFree(pcText, 0, MEM_RELEASE);
    return true;
}
bool D::AbstractWindow::SetText(PSTR pcText)
{
    int textLen = sizeof(pcText);
    if (textLen) {
        SetWindowText(this->wHwnd, pcText);
    }
    return true;
}
bool D::AbstractWindow::isSelf(PVOID& pHwnd) {

    return this->wHwnd == (HWND)pHwnd;
}
bool D::AbstractWindow::isSelf(LPARAM& pHwnd)
{
    return this->wHwnd == (HWND)pHwnd;
}

bool D::AbstractWindow::getHwnd(HWND& pHwnd)
{
    if (!this->wHwnd) {
        return false;
    }
    pHwnd = this->wHwnd;
    return true;
}

void D::AbstractWindow::Create() {
    D::OutputDebugStringEx("================================= \n lpClassName:[%s] lpWindowName:[%s]", this->lpClassName, this->lpWindowName);
        
    this->wHwnd = CreateWindow(
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
  
}

