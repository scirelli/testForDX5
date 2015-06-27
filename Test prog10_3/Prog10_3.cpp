// PROG10_3.CPP - blitter filling demo
//NOTE to self: For some reason i can't make a normal window
//look into that.

// INCLUDES ///////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN  

#include <windows.h>   // include important windows stuff
#include <windowsx.h> 
#include <mmsystem.h>
#include <iostream.h> // include important C/C++ stuff
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>

#include <ddraw.h>  // directX includes
					//also need to add C:\DXSDK\lib\ddraw.lib to the project

// DEFINES ////////////////////////////////////////////////

// defines for windows 
#define WINDOW_CLASS_NAME "WINXCLASS"  // class name

#define WINDOW_WIDTH  1024              // size of window
#define WINDOW_HEIGHT 768
#define SCREEN_WIDTH  1024              // size of screen
#define SCREEN_HEIGHT 768
#define SCREEN_BPP    16                // bits per pixel

// MACROS /////////////////////////////////////////////////

// these read the keyboard asynchronously
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// this builds a 16 bit color value
#define _RGB16BIT(r,g,b) ((b%32) + ((g%32) << 5) + ((r%32) << 10))

// TYPES //////////////////////////////////////////////////

typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;

// PROTOTYPES /////////////////////////////////////////////

int Game_Init(void *parms=NULL);
int Game_Shutdown(void *parms=NULL);
int Game_Main(void *parms=NULL);

// GLOBALS ////////////////////////////////////////////////

HWND main_window_handle = NULL; // save the window handle
HINSTANCE main_instance = NULL; // save the instance
char buffer[80];                // used to print text

LPDIRECTDRAW         lpdd         = NULL;  // dd object
LPDIRECTDRAWSURFACE  lpddsprimary = NULL;  // dd primary surface
LPDIRECTDRAWSURFACE  lpddsback    = NULL;  // dd back surface
LPDIRECTDRAWPALETTE  lpddpal      = NULL;  // a pointer to the created dd palette
DDSURFACEDESC        ddsd;                 // a direct draw surface description struct
DDSCAPS              ddscaps;              // a direct draw surface capabilities struct
HRESULT              ddrval;               // result back from dd calls

UCHAR                *double_buffer = NULL; // the double buffer              
// FUNCTIONS //////////////////////////////////////////////

LRESULT CALLBACK WindowProc(HWND hwnd, 
						    UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
// this is the main message handler of the system
PAINTSTRUCT	ps;		   // used in WM_PAINT
HDC			hdc;	   // handle to a device context

// what is the message 
switch(msg)
	{	
	case WM_CREATE: 
        {
		// do initialization stuff here
		return(0);
		} break;

    case WM_PAINT:
         {
         // start painting
         hdc = BeginPaint(hwnd,&ps);

         // end painting
         EndPaint(hwnd,&ps);
         return(0);
        } break;

	case WM_DESTROY: 
		{
		// kill the application			
		PostQuitMessage(0);
		return(0);
		} break;

	default:break;

    } // end switch

// process any messages that we didn't take care of 
return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc

// WINMAIN ////////////////////////////////////////////////

int WINAPI WinMain(	HINSTANCE hinstance,
					HINSTANCE hprevinstance,
					LPSTR lpcmdline,
					int ncmdshow)
{

WNDCLASS winclass;	// this will hold the class we create
HWND	 hwnd;		// generic window handle
MSG		 msg;		// generic message
HDC      hdc;       // generic dc
PAINTSTRUCT ps;     // generic paintstruct

// first fill in the window class stucture
winclass.style			= CS_DBLCLKS | CS_OWNDC | 
                          CS_HREDRAW | CS_VREDRAW;
winclass.lpfnWndProc	= WindowProc;
winclass.cbClsExtra		= 0;
winclass.cbWndExtra		= 0;
winclass.hInstance		= hinstance;
winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
winclass.hbrBackground	= (HBRUSH) GetStockObject(BLACK_BRUSH);
winclass.lpszMenuName	= NULL; 
winclass.lpszClassName	= WINDOW_CLASS_NAME;

// register the window class
if (!RegisterClass(&winclass))
	return(0);

// create the window, note the use of WS_POPUP
if (!(hwnd = CreateWindow(WINDOW_CLASS_NAME, // class
						  "WinX Game Console",	 // title
						  WS_POPUP | WS_VISIBLE,
					 	  0,0,	   // x,y
						  WINDOW_WIDTH,  // width
                          WINDOW_HEIGHT, // height
						  NULL,	   // handle to parent 
						  NULL,	   // handle to menu
						  hinstance,// instance
						  NULL)))	// creation parms
return(0);

// hide the mouse
ShowCursor(FALSE);

// save the window handle and instance in a global
main_window_handle = hwnd;
main_instance      = hinstance;

// perform all game console specific initialization
Game_Init();

// enter main event loop
while(1)
	{
	if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
		// test if this is a quit
        if (msg.message == WM_QUIT)
           break;
	
		// translate any accelerator keys
		TranslateMessage(&msg);

		// send the message to the window proc
		DispatchMessage(&msg);
		} // end if
    
    // main game processing goes here
    Game_Main();

	} // end while

// shutdown game and release all resources
Game_Shutdown();

// return to Windows like this
return(msg.wParam);

} // end WinMain

// WINX GAME PROGRAMMING CONSOLE FUNCTIONS ////////////////

int Game_Init(void *parms)
{
// this function is where you do all the initialization 
// for your game

// create object and test for error
if (DirectDrawCreate(NULL,&lpdd,NULL)!=DD_OK)
   return(0);

// set cooperation level to windowed mode normal
if (lpdd->SetCooperativeLevel(main_window_handle, DDSCL_ALLOWMODEX | DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT) != DD_OK ) //creates a normal window
    return(0);

// set the display mode
if (lpdd->SetDisplayMode(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP)!=DD_OK)
   return(0);

// Create the primary surface
memset(&ddsd,0,sizeof(ddsd));
ddsd.dwSize         = sizeof(ddsd);
ddsd.dwFlags        = DDSD_CAPS;
ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

if (lpdd->CreateSurface(&ddsd,&lpddsprimary,NULL)!=DD_OK)
   return(0);

// allocate memory for the double buffer
if ((double_buffer = (UCHAR *)malloc(SCREEN_WIDTH*SCREEN_HEIGHT))==NULL)
   return(0);

// return success
return(1);

} // end Game_Init

///////////////////////////////////////////////////////////

int Game_Shutdown(void *parms)
{
// this function is where you shutdown your game and
// release all resources that you allocated

// first release the primary surface
if (lpddsprimary!=NULL)
   lpddsprimary->Release();
       
// release the directdraw object
if (lpdd!=NULL)
   lpdd->Release();

// free double buffer
if (double_buffer!=NULL)
   free(double_buffer);

// return success
return(1);
} // end Game_Shutdown

///////////////////////////////////////////////////////////

int Game_Main(void *parms)
{
// this is the workhorse of your game it will be called
// continuously in real-time this is like main() in C
// all the calls for you game go here!

DDBLTFX ddbltfx; // this contains the DDBLTFX structure
RECT fill_area;  // this contains the destination rectangle
USHORT color;    // 16 bit color to fill with

// check of user is trying to exit
if (KEY_DOWN(VK_ESCAPE) || KEY_DOWN(VK_SPACE))
    PostMessage(main_window_handle, WM_DESTROY,0,0);

// clear out the structure and set the size field 
memset(&ddbltfx,0,sizeof(DDBLTFX));
ddbltfx.dwSize = sizeof(DDBLTFX);

// build up color
color = _RGB16BIT((rand()%256),(rand()%256),(rand()%256)); 

// set the dwfillcolor field to the desired color
ddbltfx.dwFillColor = color; 

// fill in the destination rectangle data (your data)
fill_area.top    = rand()%SCREEN_HEIGHT;
fill_area.left   = rand()%SCREEN_WIDTH;
fill_area.bottom = rand()%SCREEN_HEIGHT;
fill_area.right  = rand()%SCREEN_WIDTH;

// ready to blt to surface, in this case blt to primary
lpddsprimary->Blt(&fill_area, // ptr to dest rectangle
                  NULL,       // ptr to source surface, NA            
                  NULL,       // ptr to source rectangle, NA
                  DDBLT_COLORFILL | DDBLT_WAIT,   // fill and wait                   
                  &ddbltfx);  // ptr to DDBLTFX structure

// wait a sec
Sleep(1);

// return success
return(1);
} // end Game_Main

///////////////////////////////////////////////////////////

