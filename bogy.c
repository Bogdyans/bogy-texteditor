/*** includes ***/

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

/*** variables ***/

struct termios originalTermios;

/*** functions ***/

void die( const char* s )
{
    perror( s );
    exit( 1 );
}

void disableRawMode()
{
    if ( tcsetattr( STDIN_FILENO, TCSAFLUSH, &originalTermios ) < 0 )
    {
        die( "tcsetattr" );
    }
}

void enableRawMode()
{
    if ( tcgetattr( STDIN_FILENO, &originalTermios ) < 0 )
    {
        die( "tcgetattr" );
    }
    struct termios raw = originalTermios;

    raw.c_iflag &= ~( BRKINT | ICRNL | INPCK | ISTRIP | IXON ); 
    raw.c_oflag &= ~( OPOST );
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~( ECHO | ICANON | ISIG | IEXTEN );

    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if ( tcsetattr( STDIN_FILENO, TCSAFLUSH, &raw ) < 0 )
    {
        die( "tcsetattr" );
    }
}

/*** main ***/

int main()
{
    enableRawMode();
    atexit( disableRawMode );

    while ( 1 )
    {
        char character = '\0';
        if ( read( STDIN_FILENO, &character, 1 ) < 0 && errno != EAGAIN ) die( "read" );
        if ( iscntrl( character ) )
        {
            printf( "%d\r\n", character );
        }
        else
        {
            printf( "%d ('%c')\r\n", character, character );
        }

        if ( character == 'q' ) break;
    }

    return 0;
}
