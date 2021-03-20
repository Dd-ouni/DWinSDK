#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include "wke.h"
#include <commdlg.h>

/* include DWinSDK 

#include "DWinSDK.h"

#ifdef _DEBUG

#ifdef _WIN64
#pragma comment(lib, "../x64/Debug/DWinSDK.lib")
#else
#pragma comment(lib, "../Debug/DWinSDK.lib")
#endif // _WIN64

#else

#ifdef _WIN64
#pragma comment(lib, "../x64/Debug/DWinSDK.lib")
#else
#pragma comment(lib, "../Release/DWinSDK.lib")
#endif // _WIN64

#endif // _DEBUG

*/

struct HookInfo
{
	HOOKPROC proc;
};
// _declspec(dllexport) 
LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);
DWORD WINAPI ThreadProc(_In_ LPVOID lpParameter);
LRESULT CALLBACK HookWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
_declspec(dllexport) void GetHookInfo(HookInfo* pHookInfo);
using pFuncGetHookInfo = void (*)(HookInfo* pHookInfo);

using pLoadLibrary = HMODULE(__stdcall *)(LPCSTR lpLibFileName);
using pGetModuleHandle = HMODULE(__stdcall*)(LPCSTR lpModuleName);
using pVirtualProtect = BOOL(__stdcall*)(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);
struct INJ_DATA_STRUCT {
	pLoadLibrary _loadLibrary;
	pGetModuleHandle _GetModuleHandle;
	pVirtualProtect _VirtualProtect;
	unsigned unEntryPointAddress;
	unsigned unEntryPointInjFunAddress;
	CHAR clEntryPointRepair[5];
	CHAR clDllPath[260];
};
_declspec(dllexport) void EntryPointInjFunc();
_declspec(dllexport) DWORD __stdcall InjRemoteThreadProc(INJ_DATA_STRUCT* injData);

namespace D{
	_declspec(dllexport) void OutputDebugStringEx(const wchar_t* strOutputString, ...);
	_declspec(dllexport) void OutputDebugStringEx(const char* strOutputString, ...);

	_declspec(dllexport) bool SetHookEx(int idHook, DWORD dwPid);

	_declspec(dllexport) bool GetOpenFileNameEx(LPSTR& pFilePath);
	/* https://www.kancloud.cn/digest/win32-dev/165907 */

	class _declspec(dllexport) AbstractWindow {
	public:

		AbstractWindow(
			LPCTSTR lpClassName,
			LPCTSTR lpWindowName,
			DWORD dwStyle,
			int x,
			int y,
			int nWidth,
			int nHeight,
			HWND hWndParent,
			HMENU hMenu,
			HINSTANCE hInstance,
			LPVOID lpParam
		) :lpClassName(lpClassName),
			lpWindowName(lpWindowName),
			dwStyle(dwStyle),
			x(x),
			y(y),
			nWidth(nWidth),
			nHeight(nHeight),
			hWndParent(hWndParent),
			hMenu(hMenu),
			hInstance(hInstance),
			lpParam(lpParam) {
			this->Create();
		}; 
		int GetText(PSTR& pcText);
		bool SetText(AbstractWindow& aWin);
		bool SetText(PSTR pcText);
		bool isSelf(PVOID& pHwnd);
		bool isSelf(LPARAM& pHwnd);
		bool getHwnd(HWND& pHwnd);
	protected:
		LPCTSTR lpClassName;
		LPCTSTR lpWindowName;
		DWORD dwStyle;
		int x;
		int y;
		int nWidth;
		int nHeight;
		HWND hWndParent;
		HMENU hMenu;
		HINSTANCE hInstance;
		LPVOID lpParam;
		HWND wHwnd;
		void Create();
	};
	
	class _declspec(dllexport) MainControl : public AbstractWindow {
	public:
		MainControl(
			LPTSTR lpClassName,
			LPTSTR lpWindowName,
			HINSTANCE hInstance,
			int nWidth = 800,
			int nHeight = 600,
			int x = CW_USEDEFAULT,
			int y = 0,
			DWORD dwStyle = WS_POPUP,
			HMENU hMenu = NULL,
			LPVOID lpParam = NULL,
			HWND hWndParent = NULL
		) :AbstractWindow(
			lpClassName,
			lpWindowName,
			dwStyle,
			x,
			y,
			nWidth,
			nHeight,
			hWndParent,
			hMenu,
			hInstance,
			lpParam) {

			int scrWidth, scrHeight;
			scrWidth = GetSystemMetrics(SM_CXSCREEN);
			scrHeight = GetSystemMetrics(SM_CYSCREEN);
			SetWindowPos(this->wHwnd, HWND_TOPMOST, (scrWidth - this->nWidth) / 2, (scrHeight - this->nHeight) / 2, this->nWidth, this->nHeight, SWP_SHOWWINDOW);
			SetWindowLong(this->wHwnd, GWL_EXSTYLE, GetWindowLong(this->wHwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
			SetLayeredWindowAttributes(this->wHwnd, RGB(255, 0, 0), 0, LWA_COLORKEY);
			ShowScrollBar(this->wHwnd, SB_BOTH | SB_CTL | SB_HORZ | SB_VERT, FALSE);
		
		}
	};

	class _declspec(dllexport) ButtonControl : public AbstractWindow {
	public:
		ButtonControl(
			HWND hWndParent,
			HINSTANCE hInstance,
			LPCTSTR lpWindowName = "OK",
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int nWidth = 160,
			int nHeight = 30,
			DWORD dwStyle = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			HMENU hMenu = NULL,
			LPVOID lpParam = NULL
			):AbstractWindow(
			"BUTTON", 
			lpWindowName,
			dwStyle,
			x,
			y,
			nWidth,
			nHeight,
			hWndParent,
			hMenu,
			hInstance,
			lpParam){

		}
	};

	class _declspec(dllexport) EditControl : public AbstractWindow {
	public:
		EditControl(
			HWND hWndParent,
			HINSTANCE hInstance,
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int nWidth = 160,
			int nHeight = 30,
			DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
			HMENU hMenu = NULL,
			LPVOID lpParam = NULL
		) :AbstractWindow(
			"EDIT",
			NULL,
			dwStyle,
			x,
			y,
			nWidth,
			nHeight,
			hWndParent,
			hMenu,
			hInstance,
			lpParam) {
		}
	};

	class _declspec(dllexport) Inj {
	public:
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		LPVOID lpMemAddress;
		Inj();
		bool CreateInjData(INJ_DATA_STRUCT* pInjDataStruct, char* dllPath);
		unsigned GetAddressOfEntryPoint(TCHAR* path);
		bool StartProcess(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPCSTR lpCurrentDirectory, bool bIsStop = false);
		DWORD RunProcess();
		bool MemAlloc(SIZE_T dwSize);
		bool EntryPointInj(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPCSTR lpCurrentDirectory, char* pcInjDllPath, bool bIsDebug = false);
	};
}


