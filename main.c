#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

struct termios orig_termios;

void disable_raw_mode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disable_raw_mode);

  struct termios raw = orig_termios;

  raw.c_lflag &= ~(ECHO | ICANON);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
int main() {
  enable_raw_mode();
  printf("Spotify Vim Controller\n");
  printf("Press j (next), k (prev), p (play/pause), q (quit)\n");

  char c;
  while (read(STDIN_FILENO, &c, 1) == 1) {
    if (c == 'q') break;
    else printf("You pressed: %c\n", c);
  }

  return 0;
}
