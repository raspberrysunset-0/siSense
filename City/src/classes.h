#include "files.h"
#include "timer.h"

bool move_back = false;
////////////////////////////////////////////////////////////////////////////////////////////////
//The tile
class Tile
{
    public:
    //The attributes of the tile
    std::vector<SDL_Rect> box;
	SDL_Rect mbox[ 4 ]; 

	SDL_Rect* clip;

    //Initializes the variables
    Tile( int kx, int ky, int tileType );

	int x, y, type;

	//Get the tile type
    int get_type();

	void handle_input();

    //Shows the tile
    void show();

	std::vector<SDL_Rect> &get_rects();
};

Tile::Tile( int kx, int ky, int tileType  )
{
	box.resize( 31 );

	//Create Collision tiles
	for ( int n = 0, b = 2, z = 15; n < 15; n++, b += 2, z -= 1)
	{ 
		box[ n ].x = kx + z;
		box[ n ].y = ky + n;
		box[ n ].w = b;
		box[ n ].h = 1;
	}

		box[ 15 ].x = kx;
		box[ 15 ].y = ky + 15;
		box[ 15 ].w = 32;
		box[ 15 ].h = 2;

	for ( int n = 16, b = 30, z = 0; n < 31; n++, b -= 2, z += 1)
	{ 
		box[ n ].x = kx + z;
		box[ n ].y = ky + n;
		box[ n ].w = b;
		box[ n ].h = 1;
	}

	//Create tiles for mouse events 5 boxes
	//TOP
	mbox[ 0 ].x = kx + 12;
	mbox[ 0 ].y = ky + 3;
	mbox[ 0 ].w = 8;
	mbox[ 0 ].h = 4;

	//LEFT
	mbox[ 1 ].x = kx + 3;
	mbox[ 1 ].y = ky + 12;
	mbox[ 1 ].w = 4;
	mbox[ 1 ].h = 8;

	//CENTER
	mbox[ 2 ].x = kx + 8;
	mbox[ 2 ].y = ky + 7;
	mbox[ 2 ].w = 16;
	mbox[ 2 ].h = 18;

	//RIGHT
	mbox[ 3 ].x = kx + 24;
	mbox[ 3 ].y = ky + 12;
	mbox[ 3 ].w = 4;
	mbox[ 3 ].h = 8;

	//DOWN
	mbox[ 4 ].x = kx + 12;
	mbox[ 4 ].y = ky + 25;
	mbox[ 4 ].w = 8;
	mbox[ 4 ].h = 4;

	clip = &clips[ 0 ];

	x = kx;
	y = ky;
	type = tileType;
}


    //The tiles that will be used
    Tile *tiles[ TOTAL_TILES ];

Tile* get_tile( int TX, int TY )
{
    int id = 0;
    id = TX / ( SCREEN_WIDTH + TILE_WIDTH ); // you should know this, just the amount of tiles across multiplied by the tileWidth
    id = id + ( TY / ( SCREEN_HEIGHT + TILE_HEIGHT ) * TILE_WIDTH );

    return tiles[ id ];
}

int Tile::get_type()
{
    return type;
}

void Tile::handle_input()
{
	int mx = 0;
	int my = 0;

	 //If a mouse button was pressed
	if( event.type == SDL_MOUSEBUTTONDOWN )
	{
		//If the left mouse button was pressed
		if( event.button.button == SDL_BUTTON_LEFT )
		{
			//Get the mouse offsets
			mx = event.button.x;
			my = event.button.y;
        
			for ( int n = 0; n < 4; n++)
			{
				//If the mouse is over the button
				if( ( mx > mbox[ n ].x ) && ( mx < mbox[ n ].x + mbox[ n ].w ) && ( my > mbox[ n ].y ) && ( my < mbox[ n ].y + mbox[ n ].h ) )
				{
					type = 2;
				}
			}
		}
			//If the left mouse button was pressed
		if( event.button.button == SDL_BUTTON_RIGHT )
		{
			//Get the mouse offsets
			mx = event.button.x;
			my = event.button.y;
        
			for ( int n = 0; n < 4; n++)
			{
				//If the mouse is over the button
				if( ( mx > mbox[ n ].x ) && ( mx < mbox[ n ].x + mbox[ n ].w ) && ( my > mbox[ n ].y ) && ( my < mbox[ n ].y + mbox[ n ].h ) )
				{
					type = 1;
				}
			}
		}
	}
}

std::vector<SDL_Rect> &Tile::get_rects()
{
    //Retrieve the collision boxes
    return box;    
}

void Tile::show()
{
	if ( type == 1 )
    //Show the tile ( floor )
	{ apply_surface( x, y, tile, screen, clip ); }

	if ( type == 2 )
    //Show the tile ( floor )
	{ apply_surface( x, y - TILE_HEIGHT, tile, screen, &clips[ 2 ] ); }
}


void clean_up( Tile *tiles[] )
{
    //Free the surfaces
    SDL_FreeSurface( dot );
    SDL_FreeSurface( tile );

    //Free the tiles
    for( int t = 0; t < TOTAL_TILES; t++ )
    {
        delete tiles[ t ];
    }

    //Quit SDL
    SDL_Quit();
}

bool set_tiles( Tile *tiles[] )
{
    //The tile offsets
    int x = - TILE_WIDTH / 2, y = - TILE_HEIGHT / 2;

    //Open the map
    std::ifstream map( "lazy.map" );

    //If the map couldn't be loaded
    if( map == NULL )
    {
		map.close();
		return false;
    }

    //Initialize the tiles
    for( int t = 0; t < TOTAL_TILES; t++ )
    {
        //Determines what kind of tile will be made
        int tileType = -1;

        //Read tile from map file
        map >> tileType;

        //If the was a problem in reading the map
        if( map.fail() == true )
        {
            //Stop loading map
            map.close();
            return false;
        }

        //If the number is a valid tile number
        if( ( tileType >= 0 ) && ( tileType < TILE_SPRITES ) )
        {
            tiles[ t ] = new Tile( x, y, tileType  );
        }
        //If we don't recognize the tile type
        else
        {
            //Stop loading map
            map.close();
            return false;
        }

        //Move to next tile spot
        x += TILE_WIDTH;

        //If we've gone too far
        if( x >= SCREEN_WIDTH )
        {
			if ( floor_create == 0 )
            //Move back and + so will be 3d
			{ x = 0; floor_create++; }
			else 
			//Move back and - so will be 3d
			{ x = 0 - TILE_WIDTH / 2; floor_create = 0; }

            //Move to the next row and - Y
			y += TILE_HEIGHT / 2; 
        }
    }

    //Close the file
    map.close();

    //If the map was loaded fine
    return true;
}

void save_tiles( Tile *tiles[] )
{
    //Open the map
    std::ofstream map( "lazy.map" );
    
    //Go through the tiles
    for( int t = 0; t < TOTAL_TILES; t++ )
    {
		//Write tile type to file
		map << tiles[ t ]->get_type() << " ";
    }
    
    //Close the file
    map.close();
}

//////////////////////////////////////////////////////////////////////////////////////
//The dot
class Dot
{
    public:
    //The dot's collision box
    std::vector<SDL_Rect> box;

    //The velocity of the dot
    int xVel, yVel;

	int status;

    //Initializes the variables
    Dot();

    //Takes key presses and adjusts the dot's velocity
    void handle_input();

    //Moves the dot
     void move();

	 std::vector<SDL_Rect> &get_rects();

    //Shows the dot on the screen
    void show();
};


Dot::Dot()
{
	box.resize( 1 );
	status = 0;

    //Initialize the offsets
	box[ 0 ].x = 0;
	box[ 0 ].y = 0;
    box[ 0 ].w = DOT_WIDTH;
    box[ 0 ].h = DOT_HEIGHT;

    //Initialize the velocity
    xVel = 0;
    yVel = 0;
}

void Dot::handle_input()
{
    //If a key was pressed
    if( event.type == SDL_KEYDOWN  )
    {
        //Adjust the velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: yVel = -5; break;
            case SDLK_DOWN: yVel = 5; break;
            case SDLK_LEFT: xVel = -5; break;
            case SDLK_RIGHT: xVel = 5; break;
        }
    }
    //If a key was released
    else if( event.type == SDL_KEYUP )
    {
        //Adjust the velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: yVel = 0; break;
            case SDLK_DOWN: yVel = 0; break;
            case SDLK_LEFT: xVel = 0; break;
            case SDLK_RIGHT: xVel = 0; break;
        }
    }
}

void Dot::move()
{
	int Bx = box[ 0 ].x, By = box[ 0 ].y;    

	//if ( tile[ id ].type != 2 )
	{
		//Move the dot left or right
		box[ 0 ].x += xVel;
	}

    //If the dot went too far to the left or right or touched a wall
    if( ( box[ 0 ].x < 0 ) || ( box[ 0 ].x + MAN_WIDTH > SCREEN_WIDTH ) )
    {
        //move back
        box[ 0 ].x -= xVel;
    }

	//if ( tile[ id ].type != 2 )
	{
		//Move the dot left or right
		box[ 0 ].y += yVel;
	}

    //If the dot went too far up or down or touched a wall
    if( ( box[ 0 ].y < 0 ) || ( box[ 0 ].y + MAN_HEIGHT > SCREEN_HEIGHT ) )
	{
        //move back
        box[ 0 ].y -= yVel;
    }

	//Move frames when charaster moving
	if ( xVel != 0 || yVel != 0 )
	{ frame++; }
	//What frames to display and in what direction charaster mowing
	if ( frame >= 3 )
	{ frame = 0; }

	if ( yVel > 0 )
    //Show the man if he go down
	{ status = 0; }
	else if ( xVel < 0 && yVel == 0  )
    //Show the man if he go left
	{ status = 1; }
	else if ( xVel > 0 && yVel == 0 )
    //Show the man if he go right
	{ status = 2; }
	else if ( yVel < 0 )
    //Show the man if he go up
	{ status = 3; }
}

std::vector<SDL_Rect> &Dot::get_rects()
{
    //Retrieve the collision boxes
    return box;    
}

void Dot::show()
{
	if ( status == 0 )
    //Show the man if he go down
	{ apply_surface( box[ 0 ].x, box[ 0 ].y, dot, screen, &Dclip[ frame ] ); }
	if ( status == 1 )
    //Show the man if he go left
	{ apply_surface( box[ 0 ].x, box[ 0 ].y, dot, screen, &Lclip[ frame ] ); }
	if ( status == 2 )
    //Show the man if he go right
	{ apply_surface( box[ 0 ].x, box[ 0 ].y, dot, screen, &Rclip[ frame ] ); }
	if ( status == 3 )
    //Show the man if he go up
	{ apply_surface( box[ 0 ].x, box[ 0 ].y, dot, screen, &Uclip[ frame ] ); }
}