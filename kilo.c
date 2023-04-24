#include <stdlib.h> 
#include <termios.h> // For enabling raw mode
#include <unistd.h>

struct termios orig_termios;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode); // disabled Raw Mode when program exits

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


int main() {
    enableRawMode();

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q'); // Read one byte from std input at a time into c, until there are none left to read in. (read() returns no. of bytes read in)

    return 0;
}