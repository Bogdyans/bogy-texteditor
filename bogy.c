#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

struct termios originalTermios;

void disableRawMode()
{
    tcsetattr( STDIN_FILENO, TCSAFLUSH, &originalTermios );
}

void enableRawMode()
{
    tcgetattr( STDIN_FILENO, &originalTermios );
    struct termios raw = originalTermios;

    raw.c_lflag &= ~( ECHO | ICANON );

    tcsetattr( STDIN_FILENO, TCSAFLUSH, &raw );
}

int main()
{
    enableRawMode();
    atexit( disableRawMode );

    char character;

    while ( read( STDIN_FILENO, &character, 1 ) == 1 && character != 'q' );

    return 0;
}
