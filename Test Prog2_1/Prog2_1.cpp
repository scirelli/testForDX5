// PROG2_1.CPP - A simple console based game to illustrate
// a generic game loop

// INCLUDES ///////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include <conio.h>
#include <windows.h>
#include <time.h>

// DEFINES ////////////////////////////////////////////////

#define MAX_X        77  // maximum x position for player
#define SCROLL_POS   24  // the point that scrolling occurs

// PROTOTYPES /////////////////////////////////////////////

void Init_Graphics(void);
inline void Set_Color(int fcolor, int bcolor);
inline void Draw_String(int x,int y, char *string);

// GLOBALS ////////////////////////////////////////////////

CONSOLE_SCREEN_BUFFER_INFO con_info;   // holds screen info

HANDLE hconsole;         // handle to console
int    game_running = 1; // state of game, 0=done, 1=run

// FUNCTIONS //////////////////////////////////////////////

void Init_Graphics(void)
{
// this function initializes the console graphics engine

COORD console_size = {80,25}; // size of console

// seed the random number generator with time
srand((unsigned)time(NULL));

// open i/o channel to console screen
hconsole=CreateFile("CONOUT$",GENERIC_WRITE | GENERIC_READ,
         FILE_SHARE_READ | FILE_SHARE_WRITE,
         0L, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0L);

// make sure we are in 80x25
SetConsoleScreenBufferSize(hconsole,console_size);

// get details for console screen                       
GetConsoleScreenBufferInfo(hconsole,&con_info);

} // end Init_Graphics

///////////////////////////////////////////////////////////

inline void Set_Color(int fcolor, int bcolor=0) 
{
// this function sets the color of the console output
SetConsoleTextAttribute(hconsole,(WORD)((bcolor << 4) | 
                        fcolor));

} // Set_Color

///////////////////////////////////////////////////////////

inline void Draw_String(int x,int y, char *string)
{
// this function draws a string at the given x,y

COORD cursor_pos; // used to pass coords

// set printing position
cursor_pos.X = x;
cursor_pos.Y = y;
SetConsoleCursorPosition(hconsole,cursor_pos);

// print the string in current color
printf("%s",string);

} // end Draw_String

///////////////////////////////////////////////////////////

inline void Clear_Screen(void)
{
// this function clears the screen

// set color to white on black
Set_Color(15,0);

// clear the screen
for (int index=0; index<=25; index++)
    Draw_String(0, SCROLL_POS,"\n");

} // end Clear_Screen

// MAIN GAME LOOP /////////////////////////////////////////

void main(void)
{
char key;            // player input data
int  player_x = 40;  // player's x position
// SECTION: initialization

// set up the console text graphics system
Init_Graphics();

// clear the screen
Clear_Screen();

// SECTION: main event loop, this is where all the action  
// takes place, the general loop is erase-move-draw

while(game_running)
     {
     // SECTION: erase all the objects or clear screen

     // nothing to erase in our case   

     // SECTION: get player input
     if (kbhit())
        {
        // get keyboard data, and filter it
        key = toupper(getch());

        // is player trying to exit, if so exit
        if (key=='Q' || key==27)
           game_running = 0; 

        // is player moving left        
        if (key=='A')
           player_x--;
           
        // is player moving right
        if (key=='S')
           player_x++;
	
		} // end if   

     // SECTION: game logic and further processing
     
     // make sure player stays on screen 
     if (++player_x > MAX_X)
        player_x=MAX_X;     

     if (--player_x < 0)
        player_x=0;     

     // SECTION: draw everything

     // draw next star at random position
     Set_Color(15,0);
     Draw_String(rand()%80, SCROLL_POS,".\n");
     // draw player 
     Set_Color(rand()%15,0);
     Draw_String(player_x,0,"<-*->");
	 Draw_String(0,0,"");//keeps cursor at the left of the screen

     // SECTION: synchronize to a constant frame rate
     Sleep(40);   

     } // end while

// SECTION: shutdown and bail
Clear_Screen();

printf("\nG A M E  O V E R \n\n");

} // end main
                