#include "stdafx.h"
#include "main.h"
#include "../Classes/AppDelegate.h"

USING_NS_CC;


int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef _WIN32
    const char* GWorkingDir = "..\\Resources\\";
    ::SetCurrentDirectoryA(GWorkingDir);
#endif

    // create the application instance
    AppDelegate app;
    return Application::getInstance()->run();
}
