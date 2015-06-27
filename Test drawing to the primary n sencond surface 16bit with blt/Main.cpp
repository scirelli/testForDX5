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
int Main(char *cpErrorBuf, DDSURFACEDESC &ddsd, LPDIRECTDRAWSURFACE lpddsprimary, LPDIRECTDRAWSURFACE lpddsback)
{
	HRESULT hr = NULL;
	DDBLTFX ddbltfx; // this contains the DDBLTFX structure
	RECT fill_area={100,100,99,99};  // this contains the destination rectangle
	UCHAR *ucBackBuffer = NULL;//could use WORD because both are 16bit
	
	//if hit esc then exit
	if(GetKeyStatus(VK_ESCAPE) == true)
		return 1;
	
	// clear out the blt structure and set the size field 
	memset(&ddbltfx,0,sizeof(DDBLTFX));
	ddbltfx.dwSize = sizeof(DDBLTFX);
	
	// set the dwfillcolor field to the desired color
	ddbltfx.dwFillColor = RGB(0,0,0); 
	
	//clear the backbuffer
	hr = lpddsback->Blt(NULL,       // ptr to dest rectangle
                        NULL,       // ptr to source surface, NA            
                        NULL,       // ptr to source rectangle, NA
                        DDBLT_COLORFILL | DDBLT_WAIT,   // fill and wait                   
                        &ddbltfx);  // ptr to DDBLTFX structure
	if (DDFailedCheck(hr, "Blt on backbuff failed", cpErrorBuf ))
		return -1;

	// set the dwfillcolor field to the desired color
	ddbltfx.dwFillColor = RGB(rand()%255,rand()%255,rand()%255); 
	
	// fill in the destination rectangle data (your data)
	//for some reason blt wont fill a line or a point and top > bottom and left > right
	while( (fill_area.bottom <= fill_area.top) || (fill_area.right <= fill_area.left) )
	{
		fill_area.top    = rand()%WINDOW_HEIGHT;
		fill_area.left   = rand()%WINDOW_WIDTH;
		fill_area.bottom = rand()%WINDOW_HEIGHT;
		fill_area.right  = rand()%WINDOW_WIDTH;
	}
	
	//fill_area.top    = 100;
	//fill_area.left   = 100;
	//fill_area.bottom = 101;
	//fill_area.right  = 101;
	
	// ready to blt to surface, in this case blt to backsurface
	//NOTE: dont lock the surface for bliting
	hr = lpddsback->Blt(&fill_area,       // ptr to dest rectangle
                        NULL,       // ptr to source surface, NA            
                        NULL,       // ptr to source rectangle, NA
                        DDBLT_COLORFILL | DDBLT_WAIT,   // fill and wait                   
                        &ddbltfx);  // ptr to DDBLTFX structure
	if (DDFailedCheck(hr, "Blt on backbuff failed", cpErrorBuf ))
		return -1;
	
	// set up the surface description to lock the surface
	memset(&ddsd,0,sizeof(ddsd)); 
	ddsd.dwSize = sizeof(ddsd);

	// lock the secondary surface
	hr = lpddsback->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
	if (DDFailedCheck(hr, "DirectDrawCreate failed", cpErrorBuf ))
		return -1;
	
	//get a pointer to the surface could just use ddsd.lpSurface
	ucBackBuffer = (UCHAR*)ddsd.lpSurface;

	//something is really wrong here, colors change if u put them in diffrent locations and the size of the screen is not
	//640x480
	ucBackBuffer[0+0*ddsd.lPitch]= CreateRGB(255,255,255,16);
	ucBackBuffer[1024+400*ddsd.lPitch]= CreateRGB(255,255,255,16);
	ucBackBuffer[102+100*ddsd.lPitch]= CreateRGB(255,255,255,16);
	ucBackBuffer[103+100*ddsd.lPitch]= CreateRGB(255,255,255,16);

	// unlock secondary buffer
	hr = lpddsback->Unlock(ddsd.lpSurface);
	if (DDFailedCheck(hr, "DirectDrawCreate failed", cpErrorBuf ))
		return -1;
	
	// flip pages. actually flips the pages, backbuf dosn't over rigth primarysurface
	do
	{
		hr = lpddsprimary->Flip(NULL, DDFLIP_WAIT);
		if (DDFailedCheck(hr, "Flip failed", cpErrorBuf ))
			return -1;
	}while(hr != DD_OK);

	// wait a sec
	//Sleep(1500);

	return(0);
}