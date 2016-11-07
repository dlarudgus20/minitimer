#define _UNICODE
#define UNICODE

#include <windows.h>
#include <sstream>

const LPCWSTR lpszClass = L"minitimer";
const LPCWSTR lpszTitle = L"minitimer";

DWORD time_set = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE, LPSTR lpCmdParam, int)
{
	WNDCLASS wc;
	HWND hWnd;
	MSG msg;
	
	{
		std::stringstream strm(lpCmdParam);
		strm >> time_set;
	}
	
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hInstance = hInst;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = lpszClass;
	wc.lpszMenuName = nullptr;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	
	hWnd = CreateWindow(lpszClass, lpszTitle,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		100, 100, 250, 75,
		nullptr, nullptr, hInst, nullptr);
	ShowWindow(hWnd, SW_NORMAL);
	
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static DWORD timeout = 0;

	PAINTSTRUCT ps;
	wchar_t buf[512];
	DWORD t;

	switch (iMsg)
	{
		case WM_CREATE:
			timeout = GetTickCount() + time_set * 1000;
			SetTimer(hWnd, 0, 500, nullptr);
			SendMessage(hWnd, WM_TIMER, 0, 0);
			return 0;
		case WM_PAINT:
			BeginPaint(hWnd, &ps);
			if (timeout <= GetTickCount())
			{
				TextOut(ps.hdc, 10, 10, L"Time Out!!!", 11);
			}
			EndPaint(hWnd, &ps);
			return 0;
		case WM_TIMER:
			if (timeout <= GetTickCount())
			{
				SetWindowText(hWnd, L"00:00:00");
				KillTimer(hWnd, 0);
				SetForegroundWindow(hWnd);
			}
			else
			{
				t = (timeout - GetTickCount()) / 1000;
				wsprintf(buf, L"%02d:%02d:%02d",
					t / 3600,
					t % 3600 / 60,
					t % 60);
				SetWindowText(hWnd, buf);
			}
			InvalidateRect(hWnd, nullptr, TRUE);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}
