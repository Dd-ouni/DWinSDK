// dllmain.cpp : 定义 DLL 应用程序的入口点。

#include "DWinSDK.h"
#define MAX_LOADSTRING 100

HMODULE HookDll;
TCHAR szWindowClass[MAX_LOADSTRING]{"CHEATWIN"};
D::MainControl* pCheatWin = NULL;
wkeWebView m_web;
HWND m_wkeHwnd;

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
                //MessageBox(NULL, "OKXXdsffsdfds", NULL, MB_OK);
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

    //绑定ui
/*
    wkeInitialize();
    RECT rtClient;
    GetClientRect(hWnd, &rtClient);
    rtClient.top += 0;
    m_web = wkeCreateWebWindow(WKE_WINDOW_TYPE_CONTROL, hWnd, rtClient.left, rtClient.top, rtClient.right - rtClient.left, rtClient.bottom - rtClient.top);
    wkeLoadFile(m_web, "html/index.html");
    //获取hwnd
    m_wkeHwnd = wkeGetHostHWND(m_web);
    //jsBindFunction("inj", js_inj, 1);//JS调用C++
    wkeShowWindow(m_web, TRUE);

*/

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

_declspec(dllexport) bool D::GetOpenFileNameEx(LPSTR& pFilePath)
{
    OPENFILENAME opfn;
   // WCHAR strFilename[MAX_PATH];//存放文件名
    //初始化
    ZeroMemory(&opfn, sizeof(OPENFILENAME));
    opfn.lStructSize = sizeof(OPENFILENAME);//结构体大小
    //设置过滤 
    opfn.lpstrFilter = "所有文件\0*.*\0文本文件\0*.txt\0MP3文件\0*.mp3\0";
    //默认过滤器索引设为1
    opfn.nFilterIndex = 1;
    //文件名的字段必须先把第一个字符设为 \0
    opfn.lpstrFile = pFilePath;
    opfn.lpstrFile[0] = '\0';
    opfn.nMaxFile = MAX_PATH;
    //设置标志位，检查目录或文件是否存在 | OFN_NODEREFERENCELINKS
    opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    //opfn.lpstrInitialDir = NULL;
    // 显示对话框让用户选择文件
    if (GetOpenFileName(&opfn))
    {
        D::OutputDebugStringEx("DEBUG_INFO | GetOpenFileName %s", pFilePath);
        return true;
    }
    return false;
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

_declspec(dllexport) void EntryPointInjFunc()
{
    // 注入到入口点的代码，主要是加载DLL获得DLL CODE
    // 动态检索0xCCCCCCCC 修改为远程INJ_DATA_STRUCT地址
    INJ_DATA_STRUCT* injData = (INJ_DATA_STRUCT*)0xCCCCCCCC;

    HMODULE hInjDllMd = injData->_loadLibrary(injData->clDllPath);
    // 入口点数据修复
    char* pcEntryPonint = (char*)injData->unEntryPointAddress;
    pcEntryPonint[0] = injData->clEntryPointRepair[0];
    pcEntryPonint[1] = injData->clEntryPointRepair[1];
    pcEntryPonint[2] = injData->clEntryPointRepair[2];
    pcEntryPonint[3] = injData->clEntryPointRepair[3];
    pcEntryPonint[4] = injData->clEntryPointRepair[4];
    // 跳入口点
    unsigned unEntryPonint = injData->unEntryPointAddress;
    _asm {
        mov eax,unEntryPonint
        jmp eax
    }
    return;
}


_declspec(dllexport) DWORD __stdcall InjRemoteThreadProc(INJ_DATA_STRUCT* injData)
{
    // 获取入口点地址
    injData->unEntryPointAddress += (unsigned)injData->_GetModuleHandle(0);
    // 修改入口点权限
    DWORD lpflOldProtect{0};
    injData->_VirtualProtect(
        (LPVOID)injData->unEntryPointAddress,
        0x5,
        PAGE_EXECUTE_READWRITE,
        &lpflOldProtect
    );
    char* pcEntryPonint = (char*)injData->unEntryPointAddress;
    // 备份入口点代码
    injData->clEntryPointRepair[0] = pcEntryPonint[0];
    injData->clEntryPointRepair[1] = pcEntryPonint[1];
    injData->clEntryPointRepair[2] = pcEntryPonint[2];
    injData->clEntryPointRepair[3] = pcEntryPonint[3];
    injData->clEntryPointRepair[4] = pcEntryPonint[4];
    // 修改入口点跳转代码
    pcEntryPonint[0] = 0xE9;
    int iJmpAddress = injData->unEntryPointInjFunAddress - injData->unEntryPointAddress - 5;
    int* pJmpAddress = (int*)(pcEntryPonint + 1);
    *pJmpAddress = iJmpAddress;
    return 1;
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

D::Inj::Inj()
{
    ZeroMemory(&this->si, sizeof(this->si));
    this->si.cb = sizeof(this->si);
    //ZeroMemory(&this->si, sizeof(this->si));
}

bool D::Inj::CreateInjData(INJ_DATA_STRUCT* pInjDataStruct, char* pcInjDllPath)
{   
    
    int iDllLen{ 0 };
    for (; pcInjDllPath[iDllLen] != 0; iDllLen++);

    memcpy(pInjDataStruct->clDllPath, pcInjDllPath, iDllLen);
    D::OutputDebugStringEx("DEBUG_INFO | dllPath %s", pInjDataStruct->clDllPath);

    HMODULE hKernel32Md = LoadLibrary("kernel32.dll");
    if (!hKernel32Md) {
        D::OutputDebugStringEx("ERROR_INFO | LoadLibrary %d", GetLastError());
        return false;
    }
   
    pInjDataStruct->_loadLibrary = (pLoadLibrary)GetProcAddress(hKernel32Md, "LoadLibraryA");
    if (!pInjDataStruct->_loadLibrary) {
        D::OutputDebugStringEx("ERROR_INFO | GetProcAddress %d", GetLastError());
        return false;
    }
    
    pInjDataStruct->_GetModuleHandle = (pGetModuleHandle)GetProcAddress(hKernel32Md, "GetModuleHandleA");
    if (!pInjDataStruct->_GetModuleHandle) {
        D::OutputDebugStringEx("ERROR_INFO | GetProcAddress %d", GetLastError());
        return false;
    }

    pInjDataStruct->_VirtualProtect = (pVirtualProtect)GetProcAddress(hKernel32Md, "VirtualProtect");
    if (!pInjDataStruct->_VirtualProtect) {
        D::OutputDebugStringEx("ERROR_INFO | GetProcAddress %d", GetLastError());
        return false;
    }

    return true;
}

unsigned D::Inj::GetAddressOfEntryPoint(TCHAR* path)
{
    // IMAGE_DOS_HEADER  //DOS 头结构体
    // IMAGE_NT_HEADERS  //PE 结构体 
    FILE* fp;
    if ((fp = fopen(path, "rb")) == NULL) {
        return 0;
    }
    // 读取入口点不需要那么大的空间
    char* buff = new char[1024];
    fread(buff, 1024, 1, fp);
    IMAGE_DOS_HEADER* dos_header = (IMAGE_DOS_HEADER*)buff;
    IMAGE_NT_HEADERS32* pe_file_header = (IMAGE_NT_HEADERS32*)(buff + dos_header->e_lfanew);
    unsigned AddressOfEntryPoint = pe_file_header->OptionalHeader.AddressOfEntryPoint;
    fclose(fp);
    fp = NULL;
    delete[] buff;
    return AddressOfEntryPoint;
}

bool D::Inj::StartProcess(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPCSTR lpCurrentDirectory, bool bIsStop)
{
    DWORD dwCreationFlags{ 0 };
    if (bIsStop) dwCreationFlags = CREATE_SUSPENDED;
    return CreateProcess(
        lpApplicationName, //app_path,   // No module name (use command line) "E:\\software\\JX2\\Sword2.exe"
        lpCommandLine, //start_arg,        // Command line (LPTSTR)"E:\\software\\JX2\\Sword2.exe -window"
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        dwCreationFlags,              // 创建之后暂停
        NULL,           // Use parent's environment block
        lpCurrentDirectory,           // Use parent's starting directory  "E:\\software\\JX2\\"
        &this->si,            // Pointer to STARTUPINFO structure
        &this->pi);           // Pointer to PROCESS_INFORMATION structure;
}

DWORD D::Inj::RunProcess()
{
    return ResumeThread(this->pi.hThread);
}

bool D::Inj::MemAlloc(SIZE_T dwSize)
{   
    this->lpMemAddress = VirtualAllocEx(this->pi.hProcess, NULL, dwSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    return this->lpMemAddress == 0 ? false : true;
}

bool D::Inj::EntryPointInj(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPCSTR lpCurrentDirectory, char* pcInjDllPath, bool bIsDebug)
{

    // 入口注入
    // 1.启动暂停程序
    // 2.创建远程进程内存
    // 3.写入远程数据结构体包含：注入dll地址
    // 4.写入加载dll func到远程程序，该函数需要通过函数指针修改其
    // 5.调用注入dll获取远程进程的PE程序起始点+入口地址
    // 6.写入代码到入口点

    // 启动暂停程序
    if (!this->StartProcess(lpApplicationName, lpCommandLine, lpCurrentDirectory, true))
    {
        D::OutputDebugStringEx("ERROR_INFO | CreateProcess %d", GetLastError());
        return false;
    }

    // 申请远程内存
    this->MemAlloc(0x3000);


    // 0x0 入口点函数位置
    LPVOID lpEntryPointFunAddress = this->lpMemAddress;
    // 0x1000 远程数据位置
    LPVOID lpInjDataAddress = (LPVOID)((unsigned)this->lpMemAddress + 0x1000);
    // 0x2000 远程线程获取入口点位置,修改入口点代码
    LPVOID lpRemoteThreadFunAddress = (LPVOID)((unsigned)this->lpMemAddress + 0x2000);



    INJ_DATA_STRUCT InjData{};
    TCHAR* pcAppPath = (TCHAR*)lpApplicationName;
    InjData.unEntryPointAddress = this->GetAddressOfEntryPoint(pcAppPath);
    InjData.unEntryPointInjFunAddress = (unsigned)lpEntryPointFunAddress;
    // 创建注入数据
    if (!this->CreateInjData(&InjData, pcInjDllPath)) {
        return false;
    }

    // 写入注入数据
    if (!WriteProcessMemory(this->pi.hProcess, lpInjDataAddress, &InjData, sizeof(INJ_DATA_STRUCT), NULL)) {
        D::OutputDebugStringEx("ERROR_INFO | WriteProcessMemory %d", GetLastError());
        return false;
    }

    // 修改入口点方法硬编码并写入方法
    CHAR clEntryPointFunCode[300]{ 0 };
    memcpy(clEntryPointFunCode, &EntryPointInjFunc, sizeof(clEntryPointFunCode));
    CHAR* pcEntryPointFunCode = &clEntryPointFunCode[0];
    // D::OutputDebugStringEx("\n %x", *(unsigned*)pcEntryPointFunCode);
    for (; (*(unsigned*)pcEntryPointFunCode) != 0xCCCCCCCC; pcEntryPointFunCode++);
    unsigned unInjDataAddress = (unsigned)lpInjDataAddress;
    // D::OutputDebugStringEx("\n %x ", unInjDataAddress);
    memcpy(pcEntryPointFunCode, &unInjDataAddress, sizeof(unsigned));
    if (!WriteProcessMemory(this->pi.hProcess, lpEntryPointFunAddress, &clEntryPointFunCode, 500, NULL)) {
        D::OutputDebugStringEx("ERROR_INFO | WriteProcessMemory %d", GetLastError());
        return false;
    }

    // 远程线程方法写入并启动（主要是完成获取入口）
    if (!WriteProcessMemory(this->pi.hProcess, lpRemoteThreadFunAddress, &InjRemoteThreadProc, 0x500, NULL)) {
        D::OutputDebugStringEx("ERROR_INFO | WriteProcessMemory %d", GetLastError());
        return false;
    }

    if (bIsDebug) {
        char lpCommandLine2[MAX_PATH];
        sprintf(lpCommandLine2, "E:\\software\\OllyDbg\\Ollydbg.exe -p %d", this->pi.dwProcessId);

        D::OutputDebugStringEx("\n %x %x %s", lpRemoteThreadFunAddress, &InjRemoteThreadProc, lpCommandLine2);
        //this->StartProcess("E:\\software\\OllyDbg\\Ollydbg.exe", lpCommandLine2, "E:\\software\\OllyDbg\\");
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        //ZeroMemory(&si, sizeof(si));
        CreateProcess(
            "E:\\software\\OllyDbg\\Ollydbg.exe", //app_path,   // No module name (use command line) "E:\\software\\JX2\\Sword2.exe"
            lpCommandLine2, //start_arg,        // Command line (LPTSTR)"E:\\software\\JX2\\Sword2.exe -window"
            NULL,           // Process handle not inheritable
            NULL,           // Thread handle not inheritable
            FALSE,          // Set handle inheritance to FALSE
            0,
            NULL,           // Use parent's environment block
            "E:\\software\\OllyDbg\\",           // Use parent's starting directory  "E:\\software\\JX2\\"
            &si,            // Pointer to STARTUPINFO structure
            &pi);


        MessageBoxA(NULL, lpCommandLine2, "123", MB_OK);
    }

    HANDLE hThread = CreateRemoteThread(
        this->pi.hProcess,
        NULL,
        0,
        (LPTHREAD_START_ROUTINE)lpRemoteThreadFunAddress,
        lpInjDataAddress,
        0,
        NULL);

    if (hThread)
    {
        WaitForSingleObject(hThread, INFINITE);
        //GetExitCodeThread(hThread, &h);
        CloseHandle(hThread);
    }
    else {
        D::OutputDebugStringEx("ERROR_INFO | CreateRemoteThread %d", GetLastError());
        return false;
    }

    this->RunProcess();

    return false;
}


