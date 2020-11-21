#include <bits/stdc++.h>

#include "random.hpp"
#include "gui.hpp"

using namespace std;

struct Rainlet {
	uint8_t remaining_time, length, char_index;
};

vector<string> chars = {
	"\uff66", "\uff67", "\uff68", "\uff69", "\uff6a", "\uff6b", "\uff6c",
	"\uff6d", "\uff6e", "\uff6f", "\uff70", "\uff71", "\uff72", "\uff73",
	"\uff74", "\uff75", "\uff76", "\uff77", "\uff78", "\uff79", "\uff7a",
	"\uff7b", "\uff7c", "\uff7d", "\uff7e", "\uff7f", "\uff80", "\uff81",
	"\uff82", "\uff83", "\uff84", "\uff85", "\uff86", "\uff87", "\uff88",
	"\uff89", "\uff8a", "\uff8b", "\uff8c", "\uff8d", "\uff8e", "\uff8f",
	"\uff90", "\uff91", "\uff92", "\uff93", "\uff94", "\uff95", "\uff96",
	"\uff97", "\uff98", "\uff99", "\uff9a", "\uff9b", "\uff9c", "\uff9d"
};

vector<vector<Rainlet>> rain_streams;

void initialise_rain(uint16_t width, uint16_t height)
{
	for (int x = 0; x < width; x++) {
		vector<Rainlet> rainlets;
		rain_streams.push_back(rainlets);

		for (int y = 0; y < height; y++) {
			Rainlet init_rainlet = {
				.remaining_time = 0,
				.length = 0,
				.char_index = 0
			};

			rain_streams[x].push_back(init_rainlet);
		}
	}
}

void start_rain_stream(uint16_t x)
{
	uint8_t length = random_int(5, 25);
	uint8_t char_index = random_int(0, chars.size());

	Rainlet rainlet = {
		.remaining_time = length,
		.length = length,
		.char_index = char_index
	};

	rain_streams[x][0] = rainlet;
}

void tick_rain_streams()
{
	for (int x = 0; x < rain_streams.size(); x++) {
		for (int y = rain_streams[x].size() - 1; y >= 0; y--) {
			Rainlet& rainlet = rain_streams[x][y];

			// Fade

			if (rainlet.remaining_time) rainlet.remaining_time--;

			// Fall

			if (y != 0) {
				Rainlet& prev_rainlet = rain_streams[x][y - 1];

				if (prev_rainlet.remaining_time) {
					Rainlet new_rainlet = {
						.remaining_time = prev_rainlet.length,
						.length = prev_rainlet.length,
					};

					// Create new rainlet

					rain_streams[y][x] = new_rainlet;
				}
			}
		}
	}
}

void render_rain_streams(uint16_t width, uint16_t height)
{
	// Render frame black

	for (int y = 0; y < width; y++) {
		gui::set_background_colour_rgb(0, 0, 0);
		gui::put(string(width, ' '), 1, y + 1);
		gui::set_colour({ RESET });
	}

	// Render rain streams

	for (int x = 0; x < rain_streams.size(); x++) {
		for (int y = 0; y < rain_streams[x].size(); y++) {
			Rainlet& rainlet = rain_streams[x][y];

			if (rainlet.remaining_time) {
				string character = chars[rainlet.char_index];

				gui::set_foreground_colour_rgb(0, 255, 0);
				// gui::put(character, x, y);
				// gui::put(character, x, y);
				gui::set_colour({ RESET });
			}
		}
	}

	// Tick rain streams

	tick_rain_streams();
}

uint16_t last_known_cols = 0;
uint16_t last_known_rows = 0;

void render_frame()
{
	struct winsize size = gui::get_window_size();
	uint16_t cols = size.ws_col;
	uint16_t rows = size.ws_row;

	// Restart if the window side has changed

	if (last_known_cols != cols || last_known_rows != rows) {
		last_known_cols = cols;
		last_known_rows = rows;

		initialise_rain(cols, rows);
	}

	// Render the rain streams

	render_rain_streams(cols, rows);
}

int main()
{
	render_frame();

	this_thread::sleep_for(1s);

	// start_rain_stream(1);
	// start_rain_stream(3);
	// start_rain_stream(17);
	// this_thread::sleep_for(1s);

	// render_frame();
	// this_thread::sleep_for(1s);

	// render_frame();
	// this_thread::sleep_for(1s);

	// render_frame();

	gui::set_colour({ RESET });
	gui::move_cursor(last_known_cols, last_known_rows);
	printf("\n");
}