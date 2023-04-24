#include <termios.h> // For enabling raw mode
#include <unistd.h>

void enableRawMode() {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


int main() {

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q'); // Read one byte from std input at a time into c, until there are none left to read in. (read() returns no. of bytes read in)

    return 0;
}