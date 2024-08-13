

//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>
#include <fstream>
#include <vector>

int floor_create = 0;
int frame;

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The frame rate
const int FRAMES_PER_SECOND = 20;

//The dot dimensions
const int DOT_WIDTH = 25;
const int DOT_HEIGHT = 31;

//Tile constants
const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;
const int TOTAL_TILES = 636;
const int TILE_SPRITES = 12;

//The different tile sprites
const int TILE_RED = 0;
const int TILE_GREEN = 1;
const int TILE_BLUE = 2;
const int TILE_CENTER = 3;
const int TILE_TOP = 4;
const int TILE_TOPRIGHT = 5;
const int TILE_RIGHT = 6;
const int TILE_BOTTOMRIGHT = 7;
const int TILE_BOTTOM = 8;
const int TILE_BOTTOMLEFT = 9;
const int TILE_LEFT = 10;
const int TILE_TOPLEFT = 11;

const int MAN_HEIGHT = 31;
const int MAN_WIDTH = 25;

//The surfaces
SDL_Surface *dot = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *tile = NULL;

//The event structure
SDL_Event event;

SDL_Rect Dclip[ 3 ];
SDL_Rect Lclip[ 3 ];
SDL_Rect Rclip[ 3 ];
SDL_Rect Uclip[ 3 ];
SDL_Rect clips[ 3 ];

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    //Return the optimized surface
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

   SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Move the Dot", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load the dot image
	dot = load_image( "man.png" );

    //If there was a problem in loading the dot
    if( dot == NULL )
    {
        return false;
    }

    //Load the tile sheet
    tile = load_image( "tile.png" );

    //If there was a problem in loading the tiles
    if( tile == NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

void set_clips()
{
    //Clip range for the top left
    Dclip[ 0 ].x = 5;
    Dclip[ 0 ].y = 361;
    Dclip[ 0 ].w = MAN_WIDTH;
    Dclip[ 0 ].h = MAN_HEIGHT;
    
    //Clip range for the top right
    Dclip[ 1 ].x = 37;
    Dclip[ 1 ].y = 361;
    Dclip[ 1 ].w = MAN_WIDTH;
    Dclip[ 1 ].h = MAN_HEIGHT;
    
    //Clip range for the bottom left
    Dclip[ 2 ].x = 69;
    Dclip[ 2 ].y = 361;
    Dclip[ 2 ].w = MAN_WIDTH;
    Dclip[ 2 ].h = MAN_HEIGHT;
    
    //Clip range for the bottom right
    Lclip[ 0 ].x = 5;
    Lclip[ 0 ].y = 394;
    Lclip[ 0 ].w = MAN_WIDTH;
    Lclip[ 0 ].h = MAN_HEIGHT;

	    //Clip range for the top left
    Lclip[ 1 ].x = 37;
    Lclip[ 1 ].y = 394;
    Lclip[ 1 ].w = MAN_WIDTH;
    Lclip[ 1 ].h = MAN_HEIGHT;
    
    //Clip range for the top right
    Lclip[ 2 ].x = 69;
    Lclip[ 2 ].y = 394;
    Lclip[ 2 ].w = MAN_WIDTH;
    Lclip[ 2 ].h = MAN_HEIGHT;
    
    //Clip range for the bottom left
    Rclip[ 0 ].x = 5;
    Rclip[ 0 ].y = 426;
    Rclip[ 0 ].w = MAN_WIDTH;
    Rclip[ 0 ].h = MAN_HEIGHT;
    
    //Clip range for the bottom right
    Rclip[ 1 ].x = 37;
    Rclip[ 1 ].y = 426;
    Rclip[ 1 ].w = MAN_WIDTH;
    Rclip[ 1 ].h = MAN_HEIGHT;

	//Clip range for the top left
    Rclip[ 2 ].x = 69;
    Rclip[ 2 ].y = 426;
    Rclip[ 2 ].w = MAN_WIDTH;
    Rclip[ 2 ].h = MAN_HEIGHT;

	    //Clip range for the bottom left
    Uclip[ 0 ].x = 5;
    Uclip[ 0 ].y = 458;
    Uclip[ 0 ].w = MAN_WIDTH;
    Uclip[ 0 ].h = MAN_HEIGHT;
    
    //Clip range for the bottom right
    Uclip[ 1 ].x = 37;
    Uclip[ 1 ].y = 458;
    Uclip[ 1 ].w = MAN_WIDTH;
    Uclip[ 1 ].h = MAN_HEIGHT;

	//Clip range for the top left
    Uclip[ 2 ].x = 69;
    Uclip[ 2 ].y = 458;
    Uclip[ 2 ].w = MAN_WIDTH;
    Uclip[ 2 ].h = MAN_HEIGHT;

	clips[ 0 ].x = 0;
	clips[ 0 ].y = 0;
	clips[ 0 ].w = 32;
	clips[ 0 ].h = 32;

	clips[ 1 ].x = 32;
	clips[ 1 ].y = 0;
	clips[ 1 ].w = 32;
	clips[ 1 ].h = 32;

	clips[ 2 ].x = 0;
	clips[ 2 ].y = 32;
	clips[ 2 ].w = 32;
	clips[ 2 ].h = 64;
}

bool check_collision( std::vector<SDL_Rect> &A, std::vector<SDL_Rect> &B )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Go through the A boxes
    for( unsigned int Abox = 0; Abox < A.size(); Abox++ )
    {
        //Calculate the sides of rect A
        leftA = A[ Abox ].x;
        rightA = A[ Abox ].x + A[ Abox ].w;
        topA = A[ Abox ].y;
        bottomA = A[ Abox ].y + A[ Abox ].h;
        
        //Go through the B boxes    
        for( unsigned int Bbox = 0; Bbox < B.size(); Bbox++ )
        {
            //Calculate the sides of rect B
            leftB = B[ Bbox ].x;
            rightB = B[ Bbox ].x + B[ Bbox ].w;
            topB = B[ Bbox ].y;
            bottomB = B[ Bbox ].y + B[ Bbox ].h;
            
            //If no sides from A are outside of B
            if( ( ( bottomB < topA ) || ( topB > bottomA ) || ( rightB < leftA ) || ( leftB > rightA ) ) == false )
            {
                //A collision is detected
                return true;
            }
        }
    }
    
    //If neither set of collision boxes touched
    return false;
}