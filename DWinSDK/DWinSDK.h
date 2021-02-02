#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <cstdio>
#include <cstdlib>


namespace D{
	_declspec(dllexport) void OutputDebugStringEx(const wchar_t* strOutputString, ...);
	_declspec(dllexport) void OutputDebugStringEx(const char* strOutputString, ...);

	_declspec(dllexport) int GetWindowsText(HWND, PSTR&);
	


/*!
	@brief Use end need CloseHandle(hProcess),
必须开启权限  链接器 -> 清单文件 -> UAC执行级别 -> requireAdmin 


	@param IN_OUT HANDLE& hProcess		// Process HANDLE
	@param IN LPCVOID lpBaseAddress		// Read process address
	@param IN_OUT LPVOID& lpBuffer		// Read buffer
	@param IN SIZE_T nSize				// Read size
	@param IN_ DWORD dwDesiredAccess	// Process PID

	@return bool						// 
*/
	_declspec(dllexport) bool ReadProcessMemoryEx(
		HANDLE&,
		LPCVOID,
		LPVOID&,
		SIZE_T,
		DWORD);

	class _declspec(dllexport) AbstractWindow {
	public:
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
			lpParam(lpParam) {};

		HWND Create();
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


}