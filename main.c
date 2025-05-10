#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <curl/curl.h>  // To handle HTTP requests

struct termios orig_termios;

// Spotify access token and API URL
const char *ACCESS_TOKEN = "BQDkePsHsnw2DLWi44iGxiAowxFlfKxCxm9WRmqaOXzQ4JhWjoLzV2cDxQ9IjtaggjO516wIpexxLBnzF6harq2yzdjrkbuscmyrgFxJiE6TYKvQvG3_1jOySYDixTuuwdfbdvWQss-NBP-S8NkZp3O1H8w7o6wCf3DGS0v_KgRgV0-VbP2wkeIbHHPWGOEgAU0Be0uCHlr7WmPd9fxkoEGgfLrJPPBomcP0c3ssUjuL9seKSXkiQZw"; // Replace with your actual access token
const char *BASE_URL = "https://api.spotify.com/v1/me/player";

// Function to disable raw mode
void disable_raw_mode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

// Function to enable raw mode
void enable_raw_mode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disable_raw_mode);

  struct termios raw = orig_termios;

  raw.c_lflag &= ~(ECHO | ICANON);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// Function to make HTTP requests to Spotify API
void make_spotify_request(const char *url, const char *method) {
  CURL *curl = curl_easy_init();
  if(curl) {
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    char auth_header[256];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", ACCESS_TOKEN);
    headers = curl_slist_append(headers, auth_header);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    if (strcmp(method, "POST") == 0) {
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    } else if (strcmp(method, "PUT") == 0) {
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    }

    CURLcode res = curl_easy_perform(curl);

    if(res != CURLE_OK) {
      fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(res));
    } else {
      printf("Successfully made %s request to %s\n", method, url);
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
  }
}

// Function to display interactive hints for Vimium-like behavior
void display_hints() {
  printf("\n--- Hint Mode Activated ---\n");
  printf("Press:\n");
  printf("j - Next track\n");
  printf("k - Previous track\n");
  printf("p - Play/Pause\n");
  printf("q - Quit\n");
  printf("\n--- End of Hints ---\n");
}

// Function to handle key presses and trigger Spotify actions
void handle_key_press(char c) {
  if (c == 'q') {
    printf("Exiting...\n");
    exit(0);
  }
  else if (c == 'j') {
    printf("Next track\n");
    make_spotify_request("https://api.spotify.com/v1/me/player/next", "POST");
  }
  else if (c == 'k') {
    printf("Previous track\n");
    make_spotify_request("https://api.spotify.com/v1/me/player/previous", "POST");
  }
  else if (c == 'p') {
    printf("Play/Pause\n");
    make_spotify_request("https://api.spotify.com/v1/me/player/play", "PUT");
  }
  else if (c == 'f') {
    // Enter hint mode when 'f' is pressed
    display_hints();
  }
  else {
    printf("Unknown key: %c\n", c);
  }
}

int main() {
  enable_raw_mode();
  printf("Spotify Vim Controller\n");
  printf("Press 'f' for interactive hints, j (next), k (prev), p (play/pause), q (quit)\n");

  char c;
  while (read(STDIN_FILENO, &c, 1) == 1) {
    handle_key_press(c);
  }

  return 0;
}

