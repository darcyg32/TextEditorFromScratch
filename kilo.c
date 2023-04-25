/*** includes ***/

#include <stdlib.h> 
#include <termios.h> // For enabling raw mode
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>

/*** defines ***/

#define CTRL_KEY(k) ((k) & 0x1f) // Defines that CTRL_KEY(k) takes away bits 5&6 from k, to equal the correct ASCII code

/*** data ***/

struct termios orig_termios;

/*** terminal ***/

void die(const char *s) {
    perror(s); // Prints error message
    exit(1); // Exits program
}

void disableRawMode() {
    // Disables raw mode and also checks for error
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) {
        die("tcsetattr");
    }
}

void enableRawMode() {
    
    // Stores orginal terminal attributes into orig_termios, also checks for error
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
        die("tcgetattr");
    } 
    atexit(disableRawMode); // disabled Raw Mode when program exits

    struct termios raw = orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); // ICRNL allows ctrl-m, IXON allows ctrl-s/q
    raw.c_oflag &= ~(OPOST); // OPOST turns off output processing (means we require "\r\n" when we want a newline.)
    raw.c_oflag |= (CS8); // Sets character size to 8 bits per byte
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG ); // Allows us to read in byte by byte, instead of line by line. ISIG allows ctrl-c/z. IEXTEN allows ctrl-v
    raw.c_cc[VMIN] = 0; // Timeout function, sets min number of bytes of input needed before read() can return
    raw.c_cc[VTIME] = 1; // Timeout function, sets 10ths of a second that read() will wait

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        die("tcsetattr");
    };
}

/***init***/

int main() {
    // Enables raw mode
    enableRawMode();

    // Read one byte from std input at a time into c, until there are none left to read in. (read() returns no. of bytes read in)
    while (1) {
        char c = '\0';
        // Reads in character and checks for error
        // When read() times out, it returns -1 with errno of EAGAIN
        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) {
            die("read");
        }
        if (iscntrl(c)) { // if c is a control character, print it as a decimal number (it's ASCII code)
            printf("%d\r\n", c);
        } else { // Else, print out it's ASCII code and c itself
            printf("%d ('%c')\r\n", c, c);
        }
        if (c == CTRL_KEY('q')) { // If ctrl+q is pressed, exit
            break;
        }
    }

    return 0;
}