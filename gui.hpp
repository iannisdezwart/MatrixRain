#ifndef CLI_HEADER
#define CLI_HEADER

#include <bits/stdc++.h>
#include <sys/ioctl.h>

using namespace std;

namespace gui {
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

	struct winsize get_window_size()
	{
		// Request window size

		struct winsize window_size;
		ioctl(1, TIOCGWINSZ, &window_size);

		return window_size;
	}

	void clear()
	{
		// Clear entire screen

		printf("\x1b[2J");
	}

	void put(string str, uint16_t x, uint16_t y)
	{
		// Move to (x, y)

		printf("\x1b[%hd;%hdH", y, x);
		cout << str;
	}

	void set_graphic_rendition(uint8_t sgr_code)
	{
		printf("\x1b[%hhdm", sgr_code);
	}

	void set_colour(initializer_list<uint8_t> colour_codes)
	{
		// Reset

		printf("\x1b[0m");

		for (uint8_t colour_code : colour_codes) {
			printf("\x1b[%hhdm", colour_code);
		}
	}

	void set_foreground_colour_rgb(uint8_t r, uint8_t g, uint8_t b)
	{
		printf("\x1b[38;2;%hhd;%hhd;%hhdm", r, g, b);
	}

	void set_background_colour_rgb(uint8_t r, uint8_t g, uint8_t b)
	{
		printf("\x1b[48;2;%hhd;%hhd;%hhdm", r, g, b);
	}

	void move_cursor(uint16_t x, uint16_t y)
	{
		printf("\x1b[%hd;%hdH", y, x);
	}
};

#endif