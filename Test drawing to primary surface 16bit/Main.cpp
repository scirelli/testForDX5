//-----------------------------------------------------------------------------
// File: Main.cpp
//
// Desc: This is part of a template for createing DirectX programs
//
//-----------------------------------------------------------------------------

#include "General.h"

//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
//it seems that the 16-bit words for colors are 5 6 5 bits
//#define _RGB16BIT(r,g,b) ( b<<1 | ((WORD)g<<6) | ((WORD)r<<11) )//so this is wrong


//------------------------------------------------------------
// Name: Main
// Desc: Where drawing is done
//------------------------------------------------------------
bool Main(DDSURFACEDESC &ddsd, LPDIRECTDRAWSURFACE lpddsprimary)
{
	WORD *video_buffer = NULL;//USHORT so its 16-bit

	//if hit esc then exit
	if(GetKeyStatus(VK_ESCAPE) == true)
		return(1);
	
	// set up the surface description to lock the surface
	memset(&ddsd,0,sizeof(ddsd)); 
	ddsd.dwSize = sizeof(ddsd);

	//Lock the primary surface
	lpddsprimary->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
	
	//get a pointer to the surface
	video_buffer = (WORD *)ddsd.lpSurface;
	
	for (int index=0; index<100; index++)
    {
    // get random x,y
    int x = rand()%640;
    int y = rand()%480;
    
    // get random color
    UCHAR red   = rand()%256;
    UCHAR green = rand()%256;
    UCHAR blue  = rand()%256;

    // plot the pixel with a random color
    
    video_buffer[x + (y*ddsd.lPitch>>1)] = CreateRGB(red,green,blue, 16);

    } // end for index
	
	//unlock the surface
	lpddsprimary->Unlock(ddsd.lpSurface); //NULL will unlock the whole surface

	return(0);
}