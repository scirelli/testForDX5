//-----------------------------------------------------------------------------
// File: CreateRGB.h
//
// Desc: Header file for createRGB.cpp
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
#define _RGB16BIT(r,g,b) ((b%32)+((g%32)<<5)+((r%32)<<10))
#define _RGB16BIT555(r,g,b) ((b&31)+((g&31)<<5)+((r&31)<<10))
// 99% of all video cards use 5-6-5 formatt
#define _RGB16BIT565(r,g,b) ((b&31)+((g&63)<<5)+((r&31)<<11))

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
unsigned int CreateRGB( int r, int g, int b, int iBpp );//5-6-5 formatt