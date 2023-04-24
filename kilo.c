#include <stdlib.h> 
#include <termios.h> // For enabling raw mode
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>

struct termios orig_termios;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    
    tcgetattr(STDIN_FILENO, &orig_termios); // Stores orginal terminal attributes into orig_termios
    atexit(disableRawMode); // disabled Raw Mode when program exits

    struct termios raw = orig_termios;
    raw.c_iflag &= ~(ICRNL | IXON); // ICRNL allows ctrl-M, IXON allows ctrl-s/q
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG ); // Allows us to read in byte by byte, instead of line by line. ISIG allows ctrl-c/z. IEXTEN allows ctrl-v

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


int main() {
    // Enables raw mode
    enableRawMode();

    // Read one byte from std input at a time into c, until there are none left to read in. (read() returns no. of bytes read in)
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        if (iscntrl(c)) { // if c is a control character, print it as a decimal number (it's ASCII code)
            printf("%d\n", c);
        } else { // Else, print out it's ASCII code and c itself
            printf("%d ('%c')\n", c, c);
        }
    }

    return 0;
}