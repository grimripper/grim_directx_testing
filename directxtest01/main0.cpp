#include <Windows.h>

static int int_AppRunning = 1;
static LRESULT CALLBACK OurWindowProcedure(
	HWND han_Wind,
	UINT uint_Message,
	WPARAM param1,
	LPARAM param2)
{
	switch(uint_Message)
	{
		case WM_KEYDOWN:
		{
			int_AppRunning = 0;
			break;
		}
	}	

	return DefWindowProc(han_Wind, uint_Message, param1, param2);
}

static HWND NewWindow(
	LPCTSTR str_Title,
	int int_XPos,
	int int_YPos,
	int int_Width,
	int int_Height)
{
	WNDCLASSEX wnd_struct;
	wnd_struct.cbSize = sizeof(WNDCLASSEX);
	wnd_struct.style = CS_HREDRAW | CS_VREDRAW;
	wnd_struct.lpfnWndProc = OurWindowProcedure;
	wnd_struct.cbClsExtra = 0;
	wnd_struct.cbWndExtra = 0;
	wnd_struct.hInstance = GetModuleHandle(NULL);
	wnd_struct.hIcon = NULL;
	wnd_struct.hCursor = NULL;
	wnd_struct.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
	wnd_struct.lpszMenuName = NULL;
	wnd_struct.lpszClassName = L"WindowClassName";
	wnd_struct.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wnd_struct);

	return CreateWindowEx(
		WS_EX_CONTROLPARENT,
		L"WindowClassName",
		str_Title,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
		int_XPos,
		int_YPos,
		int_Width,
		int_Height,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL);
}

int WINAPI WinMain0(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND han_Window = NewWindow(L"DirectX C++ Tut", 100, 100, 500, 500);
	MSG msg_Message;

	while (int_AppRunning)
	{
		if (PeekMessage(&msg_Message, han_Window, 0, 0, PM_REMOVE))
		{
			if (!IsDialogMessage(han_Window, &msg_Message))
			{
				DispatchMessage(&msg_Message);
			}
		}
	}

	DestroyWindow(han_Window);
	return 0;
}