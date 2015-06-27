#ifndef general_h
#define general_h

////INCLUDES////////////////////////////////
#include <windows.h>
#include <windowsx.h> 
#include <math.h>
#include <stdio.h>
#include "C:\\Program Files\\SDKs\\DXSDK\\Include\\ddraw.h" //C:\Program Files\SDKs\DXSDK\Include

////DEFINES/////////////////////////////////
//defines for windows
#define WINDOW_CLASS_NAME "WINCLASS1"  // class name
#define WINDOW_WIDTH  640             // size of window
#define WINDOW_HEIGHT 480
#define BITS_PER_PIXEL 16

//-----------------------------------------------------------------------------
// Function-prototypes
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
int WINAPI WinMain(	HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow);
bool GetKeyStatus(int);
int Initiate();
int Main(char *cpErrorBuf, DDSURFACEDESC &ddsd, LPDIRECTDRAWSURFACE lpddsprimary, LPDIRECTDRAWSURFACE lpddsback);
int Shutdown();
unsigned int CreateRGB( int r, int g, int b, int iBpp );
bool DDFailedCheck(HRESULT hr, char *szMessage, char *cpErrorBuf);
char *DDErrorString(HRESULT hr);

#endif