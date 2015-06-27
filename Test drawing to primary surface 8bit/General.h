#ifndef general_h
#define general_h

////INCLUDES////////////////////////////////
#include <windows.h>
#include <windowsx.h> 
#include <math.h>
#include <stdio.h>
#include "G:\\DXSDK\\include\\ddraw.h" //G:\DXSDK\include

////DEFINES/////////////////////////////////
//defines for windows
#define WINDOW_CLASS_NAME "WINCLASS1"  // class name
#define WINDOW_WIDTH  640              // size of window
#define WINDOW_HEIGHT 480


//-----------------------------------------------------------------------------
// Function-prototypes
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
int WINAPI WinMain(	HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow);
bool GetKeyStatus(int);
int Initiate();
bool Main(DDSURFACEDESC &ddsd, LPDIRECTDRAWSURFACE lpddsprimary);
int Shutdown();
bool DDFailedCheck(HRESULT hr, char *szMessage, char * cpErrorBuf);

#endif