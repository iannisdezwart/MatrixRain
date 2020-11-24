#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>

#include "random.hpp"
#include "gui.hpp"

// vector<wchar_t> chars = {
// 	0xff66, 0xff67, 0xff68, 0xff69, 0xff6a, 0xff6b, 0xff6c,
// 	0xff6d, 0xff6e, 0xff6f, 0xff70, 0xff71, 0xff72, 0xff73,
// 	0xff74, 0xff75, 0xff76, 0xff77, 0xff78, 0xff79, 0xff7a,
// 	0xff7b, 0xff7c, 0xff7d, 0xff7e, 0xff7f, 0xff80, 0xff81,
// 	0xff82, 0xff83, 0xff84, 0xff85, 0xff86, 0xff87, 0xff88,
// 	0xff89, 0xff8a, 0xff8b, 0xff8c, 0xff8d, 0xff8e, 0xff8f,
// 	0xff90, 0xff91, 0xff92, 0xff93, 0xff94, 0xff95, 0xff96,
// 	0xff97, 0xff98, 0xff99, 0xff9a, 0xff9b, 0xff9c, 0xff9d
// };

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

struct RainDrop {
	uint8_t char_index, length, remaining_time;
};

vector<vector<RainDrop>> rain_streams;

void update_rain()
{
	// Update the non-top rows

	for (int x = 0; x < rain_streams.size(); x++) {
		for (int y = rain_streams[x].size() - 1; y >= 1; y--) {
			RainDrop& rain_drop = rain_streams[x][y];
			RainDrop& prev_rain_drop = rain_streams[x][y - 1];

			if (rain_drop.remaining_time == 0) {
				if (prev_rain_drop.remaining_time > 0) {
					// Fall down

					rain_drop.char_index = random_int(0, chars.size() - 1);
					rain_drop.length = prev_rain_drop.length;
					rain_drop.remaining_time = prev_rain_drop.length;
				}
			} else {
				// Fade

				rain_drop.remaining_time--;
			}
		}
	}

	// Update the top row

	for (int x = 0; x < rain_streams.size(); x++) {
		RainDrop& rain_drop = rain_streams[x][0];

		if (rain_drop.remaining_time == 0) {
			if (random_float() < 0.015) {
				// Start this rain stream

				int length = random_int(5, 20);

				rain_drop.char_index = random_int(0, chars.size() - 1);
				rain_drop.remaining_time = length;
				rain_drop.length = length;
			}
		} else {
			rain_drop.remaining_time--;
		}
	}
}

void render_frame()
{
	// Clear screen

	printf("\033[H\033[2J");

	// Render frame

	int width = rain_streams.size();
	int height = rain_streams[0].size();

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			RainDrop& rain_drop = rain_streams[x][y];

			gui::set_background_colour_rgb(0, 0, 0);
			gui::set_graphic_rendition({ BOLD });

			if (rain_drop.remaining_time > 0) {
				// Render character

				if (rain_drop.remaining_time == rain_drop.length) {
					gui::set_foreground_colour_rgb(255, 255, 255);
				} else {
					double intensity = (double) rain_drop.remaining_time / (double) rain_drop.length;

					gui::set_foreground_colour_rgb(0, 50 + 205 * intensity, 0);
				}

				printf("%s", chars[rain_drop.char_index].c_str());
			} else {
				// Put a space

				printf(" ");
			}

			gui::set_graphic_rendition({ RESET });
		}

		// Move to the next line

		printf("\n");
	}
}

void initialise_rain_streams(uint16_t cols, uint16_t rows)
{
	// Delete the old rain streams

	rain_streams.clear();

	// Zero initialise the rain streams with the correct size

	for (uint16_t x = 0; x < cols; x++) {
		vector<RainDrop> rain_stream;

		for (uint16_t y = 0; y < rows; y++) {
			RainDrop rain_drop = {
				.char_index = 0,
				.length = 0,
				.remaining_time = 0
			};

			rain_stream.push_back(rain_drop);
		}

		rain_streams.push_back(rain_stream);
	}
}

uint16_t last_known_cols = 0;
uint16_t last_known_rows = 0;

void handle_window_size_change()
{
	struct winsize window_size = gui::get_window_size();
	uint16_t cols = window_size.ws_col;
	uint16_t rows = window_size.ws_row;

	if (last_known_cols != cols || last_known_rows != rows) {
		last_known_cols = cols;
		last_known_rows = rows;

		// Reinitialise the rain streams

		initialise_rain_streams(cols, rows - 1);
	}
}

int main()
{
	// Rendering loop

	while (true) {
		handle_window_size_change();
		update_rain();
		render_frame();
		this_thread::sleep_for(100ms);
	}
}