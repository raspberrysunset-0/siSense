
#include "classes.h"

int main( int argc, char* args[] )
{
    //Quit flag
    bool quit = false;

	set_clips();

    //The dot
    Dot myDot;

    //The frame rate regulator
    Timer fps;

    //Initialize
    if( init() == false )
    {
        return 1;
    }

    //Load the files
    if( load_files() == false )
    {
        return 1;
    }

    //Set the tiles
    if( set_tiles( tiles ) == false )
    {
        return 1;
    }

    //While the user hasn't quit
    while( quit == false )
    {
        //Start the frame timer
        fps.start();

        //While there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            //Handle events for the dot
			myDot.handle_input();

			for( int t = 0; t < TOTAL_TILES; t++ )
			{
				tiles[ t ]->handle_input();
			}

            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
        }

		//Move the dot
		myDot.move();  

        //Show the tiles
        for( int t = 0; t < TOTAL_TILES; t++ )
        {
            tiles[ t ]->show();
        }

        //Show the dot on the screen
        myDot.show();

        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }

        //Cap the frame rate
        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
    }

	 //Save the tile map
    save_tiles( tiles );

    //Clean up
    clean_up( tiles );

    return 0;
}

