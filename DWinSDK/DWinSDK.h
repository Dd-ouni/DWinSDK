#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <cstdio>


namespace D{
	_declspec(dllexport) void OutputDebugStringEx(const wchar_t* strOutputString, ...);
	_declspec(dllexport) void OutputDebugStringEx(const char* strOutputString, ...);

	class _declspec(dllexport) AbstractControl {
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
		AbstractControl(
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

	class _declspec(dllexport) ButtonControl : public AbstractControl {
	public:
		ButtonControl(
			HWND hWndParent,
			HINSTANCE hInstance,
			LPCTSTR lpWindowName = "OK",
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int nWidth = 200,
			int nHeight = 50,
			DWORD dwStyle = WS_OVERLAPPEDWINDOW,
			HMENU hMenu = NULL,
			LPVOID lpParam = NULL
			):AbstractControl(
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
}