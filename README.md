# spotify-vim

A terminal-based Spotify controller inspired by Vim keybindings.

## Overview

**spotify-vim** is a small utility that allows you to control Spotify playback using Vim-like hotkeys directly from your terminal. It leverages the Spotify Web API, `libcurl` for HTTP requests in C, and a Python helper script for authentication.

- Written in C (controller) and Python (authentication)
- Vim-inspired keybindings: `j` for next, `k` for previous, `p` for play/pause, and `q` to quit
- Displays hints for available controls
- Uses Spotify API with OAuth authentication

## Features

- **Hotkey Control:**  
  - `j`: Next track  
  - `k`: Previous track  
  - `p`: Play/Pause  
  - `f`: Display interactive hint overlay  
  - `q`: Quit the application

- **Terminal UI:** Minimal and fast, runs in your terminal with raw input handling.

- **Spotify API Integration:** Control playback on your account via the official Spotify Web API.

## Installation

### Prerequisites

- A Spotify Premium account
- Developer credentials from the [Spotify Developer Dashboard](https://developer.spotify.com/dashboard/applications)
- Python 3 (for token management)
- C compiler (e.g., gcc)
- `libcurl` development headers (`libcurl4-openssl-dev` on Ubuntu/Debian)

### Clone the repository

```sh
git clone https://github.com/yakitoritrash/spotify-vim.git
cd spotify-vim
```

### Authentication Setup

1. Open `auth.py` and fill in your Spotify `CLIENT_ID`, `CLIENT_SECRET`, and `REFRESH_TOKEN`.
   - You can obtain a refresh token by following the [Spotify Authorization Guide](https://developer.spotify.com/documentation/general/guides/authorization-guide/).
2. Run the script to generate and refresh your access token as needed.

```sh
python3 auth.py
```

The script will print your access token. Copy this token and paste it into `main.c` as the value for `ACCESS_TOKEN`.

### Build the Controller

```sh
gcc -o spotify-vim main.c -lcurl
```

### Run

```sh
./spotify-vim
```

## Usage

- Start the program in your terminal.
- Use the keybindings (`j`, `k`, `p`, `q`, `f`) to control your Spotify playback.
- Press `f` to show keybinding hints at any time.

## Project Structure

- `main.c` - The main Spotify controller in C.
- `auth.py` - Python script to manage and refresh your Spotify access token.
- `spotify-vim` - Compiled binary (after build).

## Security Notice

**Do not share your `CLIENT_ID`, `CLIENT_SECRET`, or tokens publicly!**  
Ensure you keep your credentials safe. Consider using environment variables or a `.env` file for sensitive information.

## License

MIT License. See [LICENSE](LICENSE) for details.

## Credits

- Inspired by Vim and Vimium keybinding philosophy.
- Uses [Spotify Web API](https://developer.spotify.com/documentation/web-api/).

---
*Happy hacking and enjoy controlling Spotify from your terminal!*
