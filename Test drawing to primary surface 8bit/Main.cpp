//-----------------------------------------------------------------------------
// File: Main.cpp
//
// Desc: This is part of a template for createing DirectX programs
//
//-----------------------------------------------------------------------------

#include "General.h"


//------------------------------------------------------------
// Name: Main
// Desc: Where drawing is done
//------------------------------------------------------------
bool Main(DDSURFACEDESC &ddsd, LPDIRECTDRAWSURFACE lpddsprimary)
{
	UCHAR *video_buffer = NULL;

	//if hit esc then exit
	if(GetKeyStatus(VK_ESCAPE) == true)
		return(1);
	
	// set up the surface description to lock the surface
	memset(&ddsd,0,sizeof(ddsd)); 
	ddsd.dwSize = sizeof(ddsd);

	//Lock the primary surface
	lpddsprimary->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
	
	//get a pointer to the surface
	video_buffer = (UCHAR *)ddsd.lpSurface;
	
    video_buffer[200 + 200*ddsd.lPitch] = rand()%256;
   	
	//unlock the surface
	lpddsprimary->Unlock(ddsd.lpSurface); //NULL will unlock the whole surface

	return(0);
}