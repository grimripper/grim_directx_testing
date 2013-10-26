#include<windows.h> 
#include<d3d9.h> 
#include<d3dx9.h> 

 #define WIDTH    4
 #define HEIGHT    3
 
 struct OURCUSTOMVERTEX
 {
     float x,y,z;
     DWORD color;
 };
 
static int int_AppRunning = 1;
 
static LRESULT CALLBACK OurWindowProcedure(HWND han_Wind,UINT uint_Message,WPARAM parameter1,LPARAM parameter2)
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
 
     if (FAILED(p_dx_Object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, han_WindowToBindTo, D3DCREATE_HARDWARE_VERTEXPROCESSING, &dx_PresParams, &p_dx_Device)))
     {
         if (FAILED(p_dx_Object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, han_WindowToBindTo, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &dx_PresParams, &p_dx_Device)))
         {
             MessageBox(han_WindowToBindTo,L"Failed to create even the reference device!",L"InitializeDevice()",MB_OK);
         }
  }
 
     p_dx_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
     p_dx_Device->SetRenderState(D3DRS_LIGHTING,false);
     p_dx_Device->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
 
     return p_dx_Device;
 }
 
static void DrawScene(LPDIRECT3DDEVICE9 p_dx_Device, LPDIRECT3DVERTEXBUFFER9 p_dx_VertexBuffer, LPDIRECT3DINDEXBUFFER9 p_dx_IndexBuffer)
 {
     p_dx_Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(72,61,139), 1.0f, 0);
     p_dx_Device->BeginScene();
 
     p_dx_Device->SetStreamSource( 0, p_dx_VertexBuffer, 0, sizeof(OURCUSTOMVERTEX) );
  p_dx_Device->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
     p_dx_Device->SetIndices(p_dx_IndexBuffer);
     p_dx_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,WIDTH*HEIGHT,0,(WIDTH-1)*(HEIGHT-1)*2);
 
     p_dx_Device->EndScene();
     p_dx_Device->Present(NULL, NULL, NULL, NULL);
 }
 
static LPDIRECT3DVERTEXBUFFER9 FillVertices(HWND han_Window, LPDIRECT3DDEVICE9 p_dx_Device)
 {
     LPDIRECT3DVERTEXBUFFER9 p_dx_VertexBuffer;
     OURCUSTOMVERTEX cv_Vertices[WIDTH*HEIGHT];
     float flt_HeightData[WIDTH][HEIGHT];
 
     flt_HeightData[0][0]=0;
     flt_HeightData[1][0]=0;
     flt_HeightData[2][0]=0;
     flt_HeightData[3][0]=0;
 
     flt_HeightData[0][1]=1;
     flt_HeightData[1][1]=0;
     flt_HeightData[2][1]=2;
     flt_HeightData[3][1]=2;
 
     flt_HeightData[0][2]=2;
     flt_HeightData[1][2]=2;
     flt_HeightData[2][2]=4;
     flt_HeightData[3][2]=2;
 

    for (int x=0;x< WIDTH;x++)    {

        for (int y=0; y< HEIGHT;y++)        {
            cv_Vertices[y*WIDTH + x].x = -x;
            cv_Vertices[y*WIDTH + x].y = y;
            cv_Vertices[y*WIDTH + x].z = flt_HeightData[x][y];
            cv_Vertices[y*WIDTH + x].color = 0xffffffff;
        }
    }

    if (FAILED(p_dx_Device->CreateVertexBuffer(WIDTH*HEIGHT*sizeof(OURCUSTOMVERTEX), 0, D3DFVF_XYZ|D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &p_dx_VertexBuffer, NULL)))
    {
        MessageBox(han_Window,L"Error while creating VertexBuffer",L"FillVertices()",MB_OK);
    }

    VOID* p_Vertices;
    if (FAILED(p_dx_VertexBuffer->Lock(0, WIDTH*HEIGHT*sizeof(OURCUSTOMVERTEX), (void**)&p_Vertices, 0)))
    {
        MessageBox(han_Window,L"Error trying to lock",L"FillVertices()",MB_OK);
    }else{
        memcpy(p_Vertices, cv_Vertices, WIDTH*HEIGHT*sizeof(OURCUSTOMVERTEX));
        p_dx_VertexBuffer->Unlock();
    }


     return p_dx_VertexBuffer;
 }
 
static LPDIRECT3DINDEXBUFFER9 FillIndices(HWND han_Window, LPDIRECT3DDEVICE9 p_dx_Device)
 {
     LPDIRECT3DINDEXBUFFER9 p_dx_IndexBuffer;
     short s_Indices[(WIDTH-1)*(HEIGHT-1)*6];
 

    for (int x=0;x< WIDTH-1;x++)    {

        for (int y=0; y< HEIGHT-1;y++)        {
            s_Indices[(x+y*(WIDTH-1))*6+2] = x+y*WIDTH;
            s_Indices[(x+y*(WIDTH-1))*6+1] = (x+1)+y*WIDTH;
            s_Indices[(x+y*(WIDTH-1))*6] = (x+1)+(y+1)*WIDTH;

            s_Indices[(x+y*(WIDTH-1))*6+3] = (x+1)+(y+1)*WIDTH;
            s_Indices[(x+y*(WIDTH-1))*6+4] = x+y*WIDTH;
            s_Indices[(x+y*(WIDTH-1))*6+5] = x+(y+1)*WIDTH;

        }
    }

    if (FAILED(p_dx_Device->CreateIndexBuffer((WIDTH-1)*(HEIGHT-1)*6*sizeof(short),D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&p_dx_IndexBuffer,NULL)))
    {
        MessageBox(han_Window,L"Error while creating IndexBuffer",L"FillIndices()",MB_OK);
    }

    VOID* p_Indices;
    if (FAILED(p_dx_IndexBuffer->Lock(0, (WIDTH-1)*(HEIGHT-1)*6*sizeof(short), (void**)&p_Indices, 0)))
    {
        MessageBox(han_Window,L"Error trying to lock",L"FillIndices()",MB_OK);
    }else{
        memcpy(p_Indices, s_Indices, (WIDTH-1)*(HEIGHT-1)*6*sizeof(short));
        p_dx_IndexBuffer->Unlock();
    }

 
     return p_dx_IndexBuffer;
 }
 
static void SetUpCamera(LPDIRECT3DDEVICE9 p_dx_Device)
 {
     D3DXVECTOR3 m_EyePos(0, 0, 15);
     D3DXVECTOR3 m_TargetPos(0, 0, 0);
     D3DXVECTOR3 m_UpVector(0, 1, 0);
     D3DXMATRIXA16 m_View;
     D3DXMatrixLookAtLH(&m_View, &m_EyePos, &m_TargetPos, &m_UpVector);
     p_dx_Device->SetTransform(D3DTS_VIEW, &m_View);
 
     D3DXMATRIX m_Projection;
     D3DXMatrixPerspectiveFovLH(&m_Projection, D3DX_PI/4, 500/500, 1, 50);
     p_dx_Device->SetTransform(D3DTS_PROJECTION, &m_Projection);
 }
 
 int WINAPI WinMain5(HINSTANCE hInstance,HINSTANCE hPreviousInstance,LPSTR lpcmdline,int nCmdShow)
 {
     MSG msg_Message;
 
     HWND han_Window = NewWindow(L"DirectX C++ Tutorial",100,100,500,500);
     LPDIRECT3DDEVICE9 p_Device = InitializeDevice(han_Window);
     LPDIRECT3DVERTEXBUFFER9 p_dx_VB = FillVertices(han_Window, p_Device);
     LPDIRECT3DINDEXBUFFER9 p_dx_IB = FillIndices(han_Window, p_Device);
     SetUpCamera(p_Device);
 
     while(int_AppRunning)
     {
         if(PeekMessage(&msg_Message,han_Window,0,0,PM_REMOVE))
         {
             DispatchMessage(&msg_Message);
         }
         DrawScene(p_Device, p_dx_VB, p_dx_IB);
     }
 
     p_dx_VB->Release();
     p_dx_IB->Release();
     p_Device->Release();
     DestroyWindow(han_Window);
 
     return 0;
 }