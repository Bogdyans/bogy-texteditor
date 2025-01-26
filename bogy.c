/*** includes ***/

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

/***defines***/

#define CTRL(k) ((k) & 0x1f)

/*** variables ***/

struct termios originalTermios;

/*** terminal funcs ***/

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

char editorReadKey()
{
    int nread;
    char character;
    while ( ( nread = read( STDIN_FILENO, &character, 1 ) ) != 1 )
    {
        if ( nread == -1 && errno != EAGAIN ) die( "read" );
    }
    return character;
}

void processKeyPresses()
{
    char character = editorReadKey();

    switch (character) 
    {
        case CTRL('q'):
            exit(0);
            break;
    }
}

/*** main ***/

int main()
{
    enableRawMode();
    atexit( disableRawMode );

    while ( 1 )
    {
        processKeyPresses();
    }

    return 0;
}
