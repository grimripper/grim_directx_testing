#include<windows.h> 
#include<d3d9.h> 
#include<d3dx9.h> 
#include<dinput.h> 
#include<fstream> 
int set_offset (void);
int set_height (void);
int set_width (void);

short unsigned const int offset = set_offset();
short unsigned const int WIDTH  = set_width();
short unsigned const int HEIGHT = set_height();

struct OURCUSTOMVERTEX
{
    float x,y,z;
    DWORD color;
};

static int int_AppRunning = 1;


static float flt_Angle = 0;

static int set_offset (void)
{
    std::ifstream f_DataFile;
    short unsigned int offset;
    short dummy;

    f_DataFile.open("heightmap.bmp", std::ios::binary);
    
    
    for (int i = 0; i < 10; i++)
    {
        dummy = f_DataFile.get();
    }

    offset = f_DataFile.get();
    offset += f_DataFile.get()*256;
    offset += f_DataFile.get()*256*256;
    offset += f_DataFile.get()*256*256*256;

    f_DataFile.close();
    
    return offset;
}

static int set_height (void)
{
    std::ifstream f_DataFile;
    short unsigned int HEIGHT;
    short dummy;

    f_DataFile.open("heightmap.bmp", std::ios::binary);

    if (f_DataFile.is_open())
    {
        for (int i = 0; i < 22; i++)
        {
            dummy = f_DataFile.get();
        }

        HEIGHT = f_DataFile.get();
        HEIGHT += f_DataFile.get()*256;
        HEIGHT += f_DataFile.get()*256*256;
        HEIGHT += f_DataFile.get()*256*256*256;

        f_DataFile.close();
    }
    return HEIGHT;
}

static int set_width (void)
{
    std::ifstream f_DataFile;
    short unsigned int WIDTH;
    short dummy;

    f_DataFile.open("heightmap.bmp", std::ios::binary);

    if (f_DataFile.is_open())
    {

        for (int i = 0; i < 18; i++)
        {
            dummy = f_DataFile.get();
        }

        WIDTH = f_DataFile.get();
        WIDTH += f_DataFile.get()*256;
        WIDTH += f_DataFile.get()*256*256;
        WIDTH += f_DataFile.get()*256*256*256;

        f_DataFile.close();
    }
    return WIDTH;
}

static LRESULT CALLBACK OurWindowProcedure(HWND han_Wind,UINT uint_Message,WPARAM parameter1,LPARAM parameter2)
{
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
    p_dx_Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
    p_dx_Device->BeginScene();

    p_dx_Device->SetStreamSource( 0, p_dx_VertexBuffer, 0, sizeof(OURCUSTOMVERTEX) );
  p_dx_Device->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
    p_dx_Device->SetIndices(p_dx_IndexBuffer);


    D3DXMATRIX m_Rotation;
    D3DXMatrixRotationZ(&m_Rotation, flt_Angle);
    D3DXMATRIX m_Translation;
    D3DXMatrixTranslation(&m_Translation,32,-32,0);

    D3DXMATRIX m_World;
    D3DXMatrixMultiply(&m_World, &m_Translation, &m_Rotation);
    p_dx_Device->SetTransform(D3DTS_WORLD, &m_World);


    p_dx_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,WIDTH*HEIGHT,0,(WIDTH-1)*(HEIGHT-1)*2);

    p_dx_Device->EndScene();
    p_dx_Device->Present(NULL, NULL, NULL, NULL);
}

static LPDIRECT3DVERTEXBUFFER9 FillVertices(HWND han_Window, LPDIRECT3DDEVICE9 p_dx_Device)
{
    

    LPDIRECT3DVERTEXBUFFER9 p_dx_VertexBuffer;
    OURCUSTOMVERTEX *cv_Vertices = new OURCUSTOMVERTEX[WIDTH*WIDTH];

    std::ifstream f_DataFile;
    short unsigned int dummy;
    f_DataFile.open("heightmap.bmp", std::ios::binary);
    if (f_DataFile.is_open())
    {

        for (int i = 0;i< (offset);i++)        {
            dummy = f_DataFile.get();
        }

        for (int x=0;x< WIDTH;x++)        {

            for (int y=0; y< HEIGHT;y++)            {
                int height = f_DataFile.get();
                height += f_DataFile.get();
                height += f_DataFile.get();
                height /= 8;
                cv_Vertices[y*WIDTH + x].x = -x;
                cv_Vertices[y*WIDTH + x].y = y;
                cv_Vertices[y*WIDTH + x].z = height;
                cv_Vertices[y*WIDTH + x].color = 0xffffffff;
            }
        }
    }else{
        MessageBox(han_Window,L"HeighData file not found!",L"FillVertices()",MB_OK);
    }

    f_DataFile.close();

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
    delete [] cv_Vertices;
    return p_dx_VertexBuffer;
}

static LPDIRECT3DINDEXBUFFER9 FillIndices(HWND han_Window, LPDIRECT3DDEVICE9 p_dx_Device)
{
    LPDIRECT3DINDEXBUFFER9 p_dx_IndexBuffer;
    short *s_Indices = new short[(WIDTH-1)*(HEIGHT-1)*6];



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
    delete [] s_Indices;
    return p_dx_IndexBuffer;
}

static void SetUpCamera(LPDIRECT3DDEVICE9 p_dx_Device)
{
    D3DXVECTOR3 m_EyePos(80, 0, 120);
    D3DXVECTOR3 m_TargetPos(-20, 0, 0);
    D3DXVECTOR3 m_UpVector(0, 0, 1);
    D3DXMATRIXA16 m_View;
    D3DXMatrixLookAtLH(&m_View, &m_EyePos, &m_TargetPos, &m_UpVector);
    p_dx_Device->SetTransform(D3DTS_VIEW, &m_View);

    D3DXMATRIX m_Projection;
    D3DXMatrixPerspectiveFovLH(&m_Projection, D3DX_PI/4, 500/500, 1, 250);
    p_dx_Device->SetTransform(D3DTS_PROJECTION, &m_Projection);
}



static LPDIRECTINPUTDEVICE8 InitializeKeyboard(HWND han_Window)
{
    LPDIRECTINPUT8 p_dx_KeybObject;
    LPDIRECTINPUTDEVICE8 p_dx_KeybDevice;

    DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&p_dx_KeybObject, NULL);
    p_dx_KeybObject->CreateDevice(GUID_SysKeyboard, &p_dx_KeybDevice, NULL);

    p_dx_KeybDevice->SetDataFormat(&c_dfDIKeyboard);
    p_dx_KeybDevice->SetCooperativeLevel(han_Window, DISCL_FOREGROUND|DISCL_NONEXCLUSIVE);
    p_dx_KeybDevice->Acquire();

    return p_dx_KeybDevice;
}

static void ReadKeyboard(LPDIRECTINPUTDEVICE8 p_Keyb)
{
    char chr_KeybState[256];
    p_Keyb->GetDeviceState(sizeof(chr_KeybState),(LPVOID)&chr_KeybState);

    if (chr_KeybState[DIK_ESCAPE]/128)
    {
        int_AppRunning = 0;
    }

    if (chr_KeybState[DIK_DELETE]/128)
    {
        flt_Angle += (float)0.03;
    }

    if (chr_KeybState[DIK_NEXT]/128)
    {
        flt_Angle -= (float)0.03;
    }
}


int WINAPI WinMain7(HINSTANCE hInstance,HINSTANCE hPreviousInstance,LPSTR lpcmdline,int nCmdShow)
{
    MSG msg_Message;

    HWND han_Window = NewWindow(L"DirectX C++ Tutorial",100,100,500,500);
    LPDIRECT3DDEVICE9 p_Device = InitializeDevice(han_Window);
    LPDIRECT3DVERTEXBUFFER9 p_dx_VB = FillVertices(han_Window, p_Device);
    LPDIRECT3DINDEXBUFFER9 p_dx_IB = FillIndices(han_Window, p_Device);
    SetUpCamera(p_Device);


    LPDIRECTINPUTDEVICE8 p_KeybDevice = InitializeKeyboard(han_Window);


    while(int_AppRunning)
    {
        if(PeekMessage(&msg_Message,han_Window,0,0,PM_REMOVE))
        {
            DispatchMessage(&msg_Message);
        }

        ReadKeyboard(p_KeybDevice);

        DrawScene(p_Device, p_dx_VB, p_dx_IB);
    }


    p_KeybDevice->Release();

    p_dx_VB->Release();
    p_dx_IB->Release();
    p_Device->Release();
    DestroyWindow(han_Window);

    return 0;
}