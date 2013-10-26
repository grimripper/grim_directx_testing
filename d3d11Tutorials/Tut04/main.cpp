#include <D3D11.h>
#include "SystemClass.h"
//#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PSTR pCmdLine, int iCmdShow)
{
	SystemClass* system;
	bool result;

	system = new SystemClass;
	if (!system)
	{
		return -1;
	}

	result = system->Initialize();
	if (result)
	{
		system->Run();
	}

	system->Shutdown();
	delete system;
	system = NULL;
	return 0;
}
