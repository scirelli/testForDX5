//-----------------------------------------------------------------------------
// File: WinMain.cpp
//
// Desc: This is a template for createing DirectX 16bit programs
//
//-----------------------------------------------------------------------------

////INCLUDES////////////
#include "General.h"

//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
HWND                main_window_handle = NULL; // save the window handle
HINSTANCE           main_instance      = NULL; // save the instance
bool                iKeyStatus[256];           //array to hold key presses
HRESULT             hr;
char                cpErrorBuf[1024];
LPDIRECTDRAW        lpdd               = NULL; //pointer to direct draw object
LPDIRECTDRAWSURFACE lpddsprimary       = NULL; //where the interface pointer will be placed when the surface is created
DDSURFACEDESC       ddsd;             //used to hold the directDraw surface decription

//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The main window procedure
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{// this is the main message handler of the system
		
	//find out what the message is
	switch(uMsg)
	{	
	
	case WM_CREATE: //called when window is created
		{// do initialization stuff here
						
			return(0);
		} break;

	case WM_KEYDOWN:
		{
			iKeyStatus[wParam] = true;
			break;
		}
	case WM_KEYUP:
		{
			iKeyStatus[wParam] = false;
			break;
		}
	case WM_DESTROY: 
		{// kill the application			
			
			//close the program
			PostQuitMessage(0);
			return(0);
		} break;

	default:break;

    } // end main switch

	// process any messages that we didn't take care of 
	return (DefWindowProc(hWnd, uMsg, wParam, lParam));

} // end WinProc


//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point to the program. Initializes everything and calls
//-----------------------------------------------------------------------------
int WINAPI WinMain(	HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdline, int nCmdShow)
{

	HWND	 hWnd;		// generic window handle
	MSG		 msg;		// generic message
	HDC      hDC;       // generic dc
	WNDCLASS winclass;	// this will hold the class created
	bool bDone = false;
	
	//fill in the window class stucture
	winclass.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hInstance;
	winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground	= (HBRUSH) GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= NULL; //MAKEINTRESOURCE(IDR_MENU1);
	winclass.lpszClassName	= WINDOW_CLASS_NAME;

	// register the window class
	if (!RegisterClass(&winclass))
		return(0);
	
	// create the window
	if (!(hWnd = CreateWindow(WINDOW_CLASS_NAME, // class
							  "Bouncing on the desktop",	 // title
							  WS_POPUP | WS_VISIBLE,
						 	  100,100,	   // x,y
							  WINDOW_WIDTH,  // width
						      WINDOW_HEIGHT, // height
							  NULL,	   // handle to parent 
							  NULL,	   // handle to menu
							  hInstance,// instance
							  NULL)))	// creation parms
	{
		MessageBox(hWnd, "Window Could not be Created", NULL, MB_OK); //NULL is default for Error
		return(0);
	}
	
	// save the window handle and instance in a global
	main_window_handle = hWnd;
	main_instance      = hInstance;

	//Init everything
	if( !Initiate() )
		{
			MessageBox(hWnd, cpErrorBuf, "Initiate() Failed", NULL); //NULL is default for Error box
			return(0);
		}

	// enter main event loop
	while(!bDone)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
			// test if this is a quit message
			if (msg.message == WM_QUIT)
				break;
	
			// translate any accelerator keys
			TranslateMessage(&msg);

			// send the message to the window proc
			DispatchMessage(&msg);
		} // end if
    
    	bDone = Main(ddsd, lpddsprimary);

	} // end while

	if( !Shutdown() )
		{
			MessageBox(hWnd, cpErrorBuf, "Shutdown() Failed" , NULL); //NULL is default for Error box
			return(0);
		}		

	// return to Windows like this
	return(msg.wParam);

} // end WinMain

//------------------------------------------------------------
// Name: Initiate
// Desc: Init everything
//------------------------------------------------------------
int Initiate()
{
	// create dd object and test for an error
	hr = DirectDrawCreate(NULL,&lpdd,NULL);
	if (DDFailedCheck(hr, "DirectDrawCreate failed", cpErrorBuf ))
		return 0;
	
	// set cooperation level to windowed mode normal
	hr = lpdd->SetCooperativeLevel(main_window_handle,  DDSCL_ALLOWMODEX | DDSCL_FULLSCREEN | 
														DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT   );
	if (DDFailedCheck(hr, "SetCooperativeLevel", cpErrorBuf ))
		return 0;

	// set the display mode. use this for full screen programs
	hr = lpdd->SetDisplayMode(WINDOW_WIDTH ,WINDOW_HEIGHT,16);
	if (DDFailedCheck(hr, "SetDisplayMode", cpErrorBuf ))
		return 0;

	// Create the primary surface
	memset(&ddsd,0,sizeof(ddsd));//clear ddsd structure
	ddsd.dwSize         = sizeof(ddsd);
	ddsd.dwFlags        = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	hr = lpdd->CreateSurface(&ddsd,&lpddsprimary,NULL);
	if (DDFailedCheck(hr, "CreateSurface", cpErrorBuf ))
		return 0;

	// return success
	return(1);
}//End Initiate

//------------------------------------------------------------
// Name: ShutDown
// Desc: Clean up everything.
//------------------------------------------------------------
int Shutdown()
{
	// this function is where you shutdown your game and
	// release all resources that you allocated

	// first release the primary surface
	if (lpddsprimary != NULL)
		lpddsprimary->Release();
       
	// release the directdraw object
	if (lpdd != NULL)
		lpdd->Release();

	// return success
	return(1);
} // end Shutdown

//-----------------------------------------------------------------
// Name: GetKeyStatus
// Desc: Returns the status of a key, wether it is pressed or not
//-----------------------------------------------------------------
bool GetKeyStatus(int key)
{
	return iKeyStatus[key];
}