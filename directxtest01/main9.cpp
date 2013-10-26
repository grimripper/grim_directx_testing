#include<windows.h> 
#include<d3d9.h> 
#include<d3dx9.h> 

struct OURCUSTOMVERTEX
{
    float x,y,z;
    D3DVECTOR normal;
    DWORD color;
};

static int int_AppRunning = 1;

static float flt_Angle = 0;

LRESULT CALLBACK OurWindowProcedure(HWND han_Wind,UINT uint_Message,WPARAM parameter1,LPARAM parameter2)
{
     switch(uint_Message)
     {
         case WM_KEYDOWN:
         {
             int_AppRunning = 0;
             break;
        }
         break;
    }

     return DefWindowProc(han_Wind,uint_Message,parameter1,parameter2);
}

static HWND NewWindow(LPCTSTR str_Title,int int_XPos, int int_YPos, int int_Width, int int_Height)
{
     WNDCLASSEX wnd_Structure;

     wnd_Structure.cbSize = sizeof(WNDCLASSEX);
     wnd_Structure.style = CS_HREDRAW | CS_VREDRAW;
     wnd_Structure.lpfnWndProc = OurWindowProcedure;
     wnd_Structure.cbClsExtra = 0;
     wnd_Structure.cbWndExtra = 0;
     wnd_Structure.hInstance = GetModuleHandle(NULL);
     wnd_Structure.hIcon = NULL;
     wnd_Structure.hCursor = NULL;
     wnd_Structure.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
     wnd_Structure.lpszMenuName = NULL;
     wnd_Structure.lpszClassName = L"WindowClassName";
     wnd_Structure.hIconSm = LoadIcon(NULL,IDI_APPLICATION);

     RegisterClassEx(&wnd_Structure);

     return CreateWindowEx(WS_EX_CONTROLPARENT, L"WindowClassName", str_Title, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, int_XPos, int_YPos, int_Width, int_Height, NULL, NULL, GetModuleHandle(NULL), NULL);
}

static void Lights(LPDIRECT3DDEVICE9 p_dx_Device)
{
     D3DLIGHT9 light;

     ZeroMemory(&light, sizeof(light));
     light.Type = D3DLIGHT_DIRECTIONAL;
     light.Diffuse.r = 1.0f;
     light.Diffuse.g = 1.0f;
     light.Diffuse.b = 1.0f;
     light.Diffuse.a = 1.0f;
     light.Direction.x = 0.8f;
     light.Direction.y = 0.0f;
     light.Direction.z = -1.0f;

     p_dx_Device->SetLight(0, &light);
     p_dx_Device->LightEnable(0, TRUE);
    
     return;
}

static LPDIRECT3DDEVICE9 InitializeDevice(HWND han_WindowToBindTo)
{
     LPDIRECT3D9 p_dx_Object;
     LPDIRECT3DDEVICE9 p_dx_Device;

     p_dx_Object = Direct3DCreate9(D3D_SDK_VERSION);
     if (p_dx_Object == NULL)
     {
         MessageBox(han_WindowToBindTo,L"DirectX Runtime library not installed!",L"InitializeDevice()",MB_OK);
    }

     D3DPRESENT_PARAMETERS dx_PresParams;
     ZeroMemory( &dx_PresParams, sizeof(dx_PresParams) );
     dx_PresParams.Windowed = TRUE;
     dx_PresParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
     dx_PresParams.BackBufferFormat = D3DFMT_UNKNOWN;

     if (FAILED(p_dx_Object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, han_WindowToBindTo,
         D3DCREATE_HARDWARE_VERTEXPROCESSING, &dx_PresParams, &p_dx_Device)))
     {
         if (FAILED(p_dx_Object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, han_WindowToBindTo,
             D3DCREATE_SOFTWARE_VERTEXPROCESSING, &dx_PresParams, &p_dx_Device)))
         {
             MessageBox(han_WindowToBindTo,L"Failed to create even the reference device!",L"InitializeDevice()",MB_OK);
        }
    }
    
     Lights(p_dx_Device);
     p_dx_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
     p_dx_Device->SetRenderState(D3DRS_LIGHTING,true);
     p_dx_Device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(150, 150, 150));

     return p_dx_Device;
}

static void DrawScene(LPDIRECT3DDEVICE9 p_dx_Device, LPDIRECT3DVERTEXBUFFER9 p_dx_VertexBuffer)
{

     flt_Angle += (float)0.05;


     p_dx_Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(72,61,139), 1.0f, 0);
     p_dx_Device->BeginScene();

     D3DXMATRIX m_Translation;
     D3DXMatrixTranslation(&m_Translation,-5,0,0);

     D3DXMATRIX m_World;
     p_dx_Device->SetTransform(D3DTS_WORLD,&m_Translation );

     p_dx_Device->SetStreamSource(0, p_dx_VertexBuffer, 0, sizeof(OURCUSTOMVERTEX));
     p_dx_Device->SetFVF(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE);
     p_dx_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

     p_dx_Device->EndScene();
     p_dx_Device->Present(NULL, NULL, NULL, NULL);
}

static LPDIRECT3DVERTEXBUFFER9 FillVertices(HWND han_Window, LPDIRECT3DDEVICE9 p_dx_Device)
{
     OURCUSTOMVERTEX cv_Vertices[6];
     LPDIRECT3DVERTEXBUFFER9 p_dx_VertexBuffer;

    cv_Vertices[0].x = 0.0f;
    cv_Vertices[0].y = 0.0f;
    cv_Vertices[0].z = 50.0f;
    cv_Vertices[0].color = 0xff3333FF;
    cv_Vertices[0].normal.x = 0;
    cv_Vertices[0].normal.y = 0;
    cv_Vertices[0].normal.z = 1;

    cv_Vertices[1].x = 50.0f;
    cv_Vertices[1].y = 0.0f;
    cv_Vertices[1].z = 0.0f;
    cv_Vertices[1].color = 0xff3333FF;
    cv_Vertices[1].normal.x = 1;
    cv_Vertices[1].normal.y = 0;
    cv_Vertices[1].normal.z = 1;

    cv_Vertices[2].x = 0.0f;
    cv_Vertices[2].y = 50.0f;
    cv_Vertices[2].z = 50.0f;
    cv_Vertices[2].color = 0xff3333FF;
    cv_Vertices[2].normal.x = 0;
    cv_Vertices[2].normal.y = 0;
    cv_Vertices[2].normal.z = 1;

    cv_Vertices[3].x = -50.0f;
    cv_Vertices[3].y = 0.0f;
    cv_Vertices[3].z = 0.0f;
    cv_Vertices[3].color = 0xff3333FF;
    cv_Vertices[3].normal.x = -1;
    cv_Vertices[3].normal.y = 0;
    cv_Vertices[3].normal.z = 1;
    
    cv_Vertices[4].x = 0.0f;
    cv_Vertices[4].y = 0.0f;
    cv_Vertices[4].z = 50.0f;
    cv_Vertices[4].color = 0xff3333FF;
    cv_Vertices[4].normal.x = 0;
    cv_Vertices[4].normal.y = 0;
    cv_Vertices[4].normal.z = 1;

    cv_Vertices[5].x = 0.0f;
    cv_Vertices[5].y = 50.0f;
    cv_Vertices[5].z = 50.0f;
    cv_Vertices[5].color = 0xff3333FF;
    cv_Vertices[5].normal.x = 0;
    cv_Vertices[5].normal.y = 0;
    cv_Vertices[5].normal.z = 1;

     if (FAILED(p_dx_Device->CreateVertexBuffer(6*sizeof(OURCUSTOMVERTEX),
         0, D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &p_dx_VertexBuffer, NULL)))
     {
         MessageBox(han_Window,L"Error while creating VertexBuffer",L"FillVertices()",MB_OK);
    }

     VOID* p_Vertices;
     if (FAILED(p_dx_VertexBuffer->Lock(0, 6*sizeof(OURCUSTOMVERTEX), (void**)&p_Vertices, 0)))
     {
         MessageBox(han_Window,L"Error trying to lock",L"FillVertices()",MB_OK);
    }else{
         memcpy(p_Vertices, cv_Vertices, 6*sizeof(OURCUSTOMVERTEX));
         p_dx_VertexBuffer->Unlock();
    }

     return p_dx_VertexBuffer;
}

static void SetUpCamera(LPDIRECT3DDEVICE9 p_dx_Device)
{
     D3DXVECTOR3 m_EyePos(0, -40, 100);
     D3DXVECTOR3 m_TargetPos(0, 50, 0);
     D3DXVECTOR3 m_UpVector(0, 1, 0);
     D3DXMATRIXA16 m_View;
     D3DXMatrixLookAtLH(&m_View, &m_EyePos, &m_TargetPos, &m_UpVector);
     p_dx_Device->SetTransform(D3DTS_VIEW, &m_View);

     D3DXMATRIX m_Projection;
     D3DXMatrixPerspectiveFovLH(&m_Projection, D3DX_PI/4, 500/500, 1, 200);
     p_dx_Device->SetTransform(D3DTS_PROJECTION, &m_Projection);
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPreviousInstance,LPSTR lpcmdline,int nCmdShow)
{
     MSG msg_Message;

     HWND han_Window = NewWindow(L"DirectX C++ Tutorial",100,100,500,500);
     LPDIRECT3DDEVICE9 p_Device = InitializeDevice(han_Window);
     LPDIRECT3DVERTEXBUFFER9 p_dx_VB = FillVertices(han_Window, p_Device);
     SetUpCamera(p_Device);

     while(int_AppRunning)
     {
         if(PeekMessage(&msg_Message,han_Window,0,0,PM_REMOVE))
         {
             DispatchMessage(&msg_Message);
        }
         DrawScene(p_Device, p_dx_VB);
    }

     p_Device->Release();
     DestroyWindow(han_Window);

     return 0;
}