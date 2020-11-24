#ifndef CLI_HEADER
#define CLI_HEADER

#include <bits/stdc++.h>
#include <sys/ioctl.h>

using namespace std;

// Select Graphic Rendition codes

#define RESET 0
#define BOLD 1
#define FAINT 2
#define ITALIC 3
#define UNDERLINE 4
#define SLOW_BLINK 5
#define RAPID_BLINK 6
#define BLINK RAPID_BLINK
#define REVERSE_VIDEO 7
#define CONCEAL 8
#define CROSSED_OUT 9

// Foreground codes

#define BLACK 30
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define MAGENTA 35
#define CYAN 36
#define WHITE 37

#define BRIGHT_BLACK 90
#define GREY BRIGHT_BLACK
#define BRIGHT_RED 91
#define BRIGHT_GREEN 92
#define BRIGHT_YELLOW 93
#define BRIGHT_BLUE 94
#define BRIGHT_MAGENTA 95
#define BRIGHT_CYAN 96
#define BRIGHT_WHITE 97

// Background codes

#define BLACK_BG 40
#define RED_BG 41
#define GREEN_BG 42
#define YELLOW_BG 43
#define BLUE_BG 44
#define MAGENTA_BG 45
#define CYAN_BG 46
#define WHITE_BG 47

#define BRIGHT_BLACK_BG 100
#define GREY_BG BRIGHT_BLACK_BG
#define BRIGHT_RED_BG 101
#define BRIGHT_GREEN_BG 102
#define BRIGHT_YELLOW_BG 103
#define BRIGHT_BLUE_BG 104
#define BRIGHT_MAGENTA_BG 105
#define BRIGHT_CYAN_BG 106
#define BRIGHT_WHITE_BG 107

namespace gui {
	struct winsize get_window_size()
	{
		// Request window size

		struct winsize window_size;
		ioctl(1, TIOCGWINSZ, &window_size);

		return window_size;
	}

	class Frame {
		public:
			string buffer;
			uint16_t width, height;

			Frame(uint16_t width, uint16_t height)
			{
				this->width = width;
				this->height = height;

				for (int y = 0; y < height; y++) {
					for (int x = 0; x < height; x++) {
						buffer.push_back(0);
					}
				}
			}

			void put(string str)
			{
				buffer += str;
			}

			void put(string str, uint16_t x, uint16_t y)
			{
				move_cursor(x, y);
				put(str);
			}

			void display()
			{
				// Clear entire screen

				printf("\x1b[H");

				// Write the buffer to stdout

				cout << buffer;
			}

			void set_graphic_rendition(initializer_list<uint8_t> graphic_rendition_codes)
			{
				for (uint8_t graphic_rendition_code : graphic_rendition_codes) {
					buffer += "\x1b["
						+ to_string(graphic_rendition_code) + "m";
				};
			}

			void set_foreground_colour_rgb(uint8_t r, uint8_t g, uint8_t b)
			{
				buffer += "\x1b[38;2;"
					+ to_string(r) + ";"
					+ to_string(g) + ";"
					+ to_string(b) + "m";
			}

			void set_background_colour_rgb(uint8_t r, uint8_t g, uint8_t b)
			{
				buffer += "\x1b[48;2;"
					+ to_string(r) + ";"
					+ to_string(g) + ";"
					+ to_string(b) + "m";
			}

			void set_foreground_colour_rgb(uint32_t rgb)
			{
				uint8_t r = (rgb & 0x00ff0000) >> 16;
				uint8_t g = (rgb & 0x0000ff00) >> 8;
				uint8_t b = (rgb & 0x000000ff);
				set_foreground_colour_rgb(r, g, b);
			}

			void set_background_colour_rgb(uint32_t rgb)
			{
				uint8_t r = (rgb & 0x00ff0000) >> 16;
				uint8_t g = (rgb & 0x0000ff00) >> 8;
				uint8_t b = (rgb & 0x000000ff);
				set_background_colour_rgb(r, g, b);
			}

			void move_cursor(uint16_t x, uint16_t y)
			{
				buffer += "\x1b[" + to_string(y) + ";" + to_string(x) + "H";
			}

			void erase_current_line()
			{
				buffer += "\x1b[K";
			}
	};
};

#endif
