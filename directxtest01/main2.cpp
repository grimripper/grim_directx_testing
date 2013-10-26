#include <Windows.h>
#include <D3D9.h>
#include <D3DX9.h>

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

struct OURCUSTOMVERTEX
{
	float x,y,z;
	DWORD color;
};

static LPDIRECT3DVERTEXBUFFER9 FillVertices(HWND han_Window, LPDIRECT3DDEVICE9 p_dx_Device)
{
	OURCUSTOMVERTEX cv_Vertices[3];

	cv_Vertices[0].x = 5.0f;
	cv_Vertices[0].y = 10.0f;
	cv_Vertices[0].z = 0;
	cv_Vertices[0].color = 0xffff0000; // ARGB (RED)

	cv_Vertices[1].x = 10.f;
	cv_Vertices[1].y = 0.f;
	cv_Vertices[1].z = 0.f;
	cv_Vertices[1].color = 0xff00ff00; // GREEN

	cv_Vertices[2].x = 0.f;
	cv_Vertices[2].y = 0.f;
	cv_Vertices[2].z = 0.f;
	cv_Vertices[2].color = 0xff00ffff; // BLUE

	LPDIRECT3DVERTEXBUFFER9 p_dx_VertexBuffer;
	if (FAILED(p_dx_Device->CreateVertexBuffer(
		3*sizeof(OURCUSTOMVERTEX),
		0,
		D3DFVF_XYZ | D3DFVF_DIFFUSE, //(world coordinates + color)
		D3DPOOL_DEFAULT,
		&p_dx_VertexBuffer,
		NULL)))
	{
		MessageBox(han_Window,
			L"Error while creating VB",
			L"FillVertices()",
			MB_OK);
	}

	VOID* p_Vertices;
	if (FAILED(p_dx_VertexBuffer->Lock(
		0,
		3*sizeof(OURCUSTOMVERTEX),
		(void**)&p_Vertices,
		0)))
	{
		MessageBox(han_Window,
			L"FAiled to lock VB",
			L"FillVertices()",
			MB_OK);
	}
	else
	{
		memcpy(p_Vertices, cv_Vertices, 3*sizeof(OURCUSTOMVERTEX));
		p_dx_VertexBuffer->Unlock();
	}

	return p_dx_VertexBuffer;
}

static void SetupCamera(LPDIRECT3DDEVICE9 p_dx_Device)
{
	D3DXVECTOR3 m_EyePos(0, 0, -30);
	D3DXVECTOR3 m_TargetPos(0, 0, 0);
	D3DXVECTOR3 m_UpVec(0, 1, 0);

	D3DXMATRIXA16 m_View;
	D3DXMatrixLookAtLH(&m_View, &m_EyePos, &m_TargetPos, &m_UpVec);

	p_dx_Device->SetTransform(D3DTS_VIEW, &m_View);

	D3DXMATRIX m_Projection;
	D3DXMatrixPerspectiveFovLH(&m_Projection, D3DX_PI/4, 500/500, 1, 50);

	p_dx_Device->SetTransform(D3DTS_PROJECTION, &m_Projection);
}

static LPDIRECT3DDEVICE9 InitializeDevice(HWND han_WindowToBindTo)
{
	// Check if DirectX is installed
	LPDIRECT3D9 p_dx_Object;
	p_dx_Object = ::Direct3DCreate9(D3D_SDK_VERSION);
	if (p_dx_Object == NULL)
	{
		MessageBox(han_WindowToBindTo, L"DX RT not installed!", L"InitializeDevice()", MB_OK);
	}

	D3DPRESENT_PARAMETERS dx_PresParams;
	ZeroMemory(&dx_PresParams, sizeof(dx_PresParams));
	// not full screen
	dx_PresParams.Windowed = TRUE;

	// write to the device immediately, not to an extra back buffer that will
	// be presented (= swapped) at runtime.
	dx_PresParams.SwapEffect = D3DSWAPEFFECT_DISCARD; 
	dx_PresParams.BackBufferFormat = D3DFMT_UNKNOWN;

	LPDIRECT3DDEVICE9 p_dx_Device;
	if (FAILED(p_dx_Object->CreateDevice(
		D3DADAPTER_DEFAULT, // if you have multiple graphics adapters
		D3DDEVTYPE_HAL, // you have a true graphics card
		han_WindowToBindTo, 
		D3DCREATE_HARDWARE_VERTEXPROCESSING, //fast enough to do processing in HW
		&dx_PresParams,
		&p_dx_Device)))
	{
		if (FAILED(p_dx_Object->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_REF,
			han_WindowToBindTo,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&dx_PresParams,
			&p_dx_Device)))
		{
			MessageBox(han_WindowToBindTo, L"DX RT not installed!", L"InitializeDevice()", MB_OK);
		}
	}

	// good for debugging
	//p_dx_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// assume no light
	p_dx_Device->SetRenderState(::D3DRS_LIGHTING, false);
	return p_dx_Device;
}

static void DrawScene(LPDIRECT3DDEVICE9 p_dx_Device, LPDIRECT3DVERTEXBUFFER9 p_dx_vb)
{
	p_dx_Device->Clear(
		0, // to clear specific rectangles
		NULL,  // actual rects
		D3DCLEAR_TARGET, 
		D3DCOLOR_XRGB(0,0,0),
		1.0f, // infinity
		0);
	p_dx_Device->BeginScene();

	p_dx_Device->SetStreamSource(0, p_dx_vb, 0, sizeof(OURCUSTOMVERTEX));
	p_dx_Device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	p_dx_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	p_dx_Device->EndScene();
	p_dx_Device->Present(NULL, NULL, NULL, NULL);
}

int WINAPI WinMain2(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND han_Window = NewWindow(L"DirectX C++ Tut", 100, 100, 500, 500);
	LPDIRECT3DDEVICE9 p_Device = InitializeDevice(han_Window);
	SetupCamera(p_Device);
	LPDIRECT3DVERTEXBUFFER9 p_DX_VB = FillVertices(han_Window, p_Device);

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
		DrawScene(p_Device, p_DX_VB);
	}

	p_DX_VB->Release();
	p_Device->Release();
	DestroyWindow(han_Window);
	return 0;
}