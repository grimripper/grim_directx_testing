#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	m_D3D = new D3DClass;
	if (!m_D3D)
		return false;

	result = m_D3D->Initialize(
		screenWidth, 
		screenHeight, 
		VSYNC_ENABLED,
		hwnd,
		FULL_SCREEN,
		SCREEN_DEPTH,
		SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize D3D", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
	return;
}


bool GraphicsClass::Frame()
{
	return Render();
}

bool GraphicsClass::Render()
{
	//Clears buffers
	m_D3D->BeginScene(1.0f, 0.f, 0.f, 1.0f);

	// Present rendered scene to the screen
	m_D3D->EndScene();
	return true;
}
