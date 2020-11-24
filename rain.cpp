#include <bits/stdc++.h>

#include "random.hpp"
#include "gui.hpp"

// Change these settings to your liking

#define DESIRED_FPS 15
#define SHOW_FPS_COUNTER true

#define MIN_RAIN_STREAM_LENGTH 10
#define MAX_RAIN_STREAM_LENGTH 35
#define RAIN_STREAM_SPAWN_CHANCE 0.015

#define BACKGROUND_COLOUR 0, 0, 0

#define FIRST_RAINDROP_COLOUR 255, 255, 255
#define RAINDROP_COLOUR(intensity) 0, 255 * intensity, 0

// Uncomment the below code for a green to blue gradient

// #define RAINDROP_COLOUR(intensity) 0, 128 + 127 * intensity, 255 - 255 * intensity

// Uncomment the below code for an RGB effect

// #include "hue-to-rgb.hpp"
// #define RAINDROP_COLOUR(intensity) hue_to_rgb(intensity)

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
			if (random_float() < RAIN_STREAM_SPAWN_CHANCE) {
				// Start this rain stream

				int length = random_int(MIN_RAIN_STREAM_LENGTH, MAX_RAIN_STREAM_LENGTH);

				rain_drop.char_index = random_int(0, chars.size() - 1);
				rain_drop.remaining_time = length;
				rain_drop.length = length;
			}
		} else {
			rain_drop.remaining_time--;
		}
	}
}

chrono::nanoseconds render_frame()
{
	// Keep track of the starting time

	auto starting_time = chrono::high_resolution_clock::now();

	// Render the frame

	int width = rain_streams.size();
	int height = rain_streams[0].size();

	gui::Frame frame(width, height);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			RainDrop& rain_drop = rain_streams[x][y];

			frame.set_background_colour_rgb(BACKGROUND_COLOUR);
			frame.set_graphic_rendition({ BOLD });

			if (rain_drop.remaining_time > 0) {
				// Render character

				if (rain_drop.remaining_time == rain_drop.length) {
					frame.set_foreground_colour_rgb(FIRST_RAINDROP_COLOUR);
				} else {
					double intensity = (double) rain_drop.remaining_time / (double) rain_drop.length;

					frame.set_foreground_colour_rgb(RAINDROP_COLOUR(intensity));
				}

				frame.put(chars[rain_drop.char_index]);
			} else {
				// Put a space

				frame.put(" ");
			}

			frame.set_graphic_rendition({ RESET });
		}

		// Move to the next line

		frame.put("\n");
	}

	// FPS counter

	if (SHOW_FPS_COUNTER) {
		auto elapsed_time = chrono::high_resolution_clock::now() - starting_time;
		auto duration = chrono::duration_cast<chrono::nanoseconds>(elapsed_time);
		double fps = 1E9 / (double) duration.count();

		// Remove the previous FPS counter

		frame.erase_current_line();

		// Put the new FPS counter

		frame.set_background_colour_rgb(23, 106, 173);
		frame.set_foreground_colour_rgb(255, 255, 255);
		frame.set_graphic_rendition({ BOLD });

		frame.put(" Potential FPS = " + to_string(fps).substr(0, 5) + " ");
		frame.put(
			" Actual FPS = "
			+ to_string(min(fps, (double) DESIRED_FPS)).substr(0, 5) + " "
		);

		frame.set_graphic_rendition({ RESET });

		frame.put("\n");

		// Fixes visual artefacts when sizing down

		frame.erase_current_line();
	}

	// Display the frame

	frame.display();

	// Return the total elapsed time of this whole function call

	return chrono::high_resolution_clock::now() - starting_time;
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
		uint16_t vertical_resize = rows - last_known_rows;

		if (vertical_resize > 0) {
			// Scroll down

			printf("\x1b[%dT\n", vertical_resize);
		} else {
			// Scroll up

			printf("\x1b[%dS\n", vertical_resize);
		}

		// Reinitialise the rain streams
		// Make space for the FPS counter if it is enabled

		initialise_rain_streams(cols, rows - 1 - SHOW_FPS_COUNTER);

		// Update the last_known_cols and last_known_rows

		last_known_cols = cols;
		last_known_rows = rows;
	}
}

int main()
{
	// Rendering loop

	while (true) {
		handle_window_size_change();
		update_rain();
		chrono::nanoseconds duration = render_frame();

		auto desired_refresh_time = chrono::nanoseconds((int) 1E9 / DESIRED_FPS);
		auto sleep = max(desired_refresh_time - duration, (chrono::nanoseconds) 0);

		this_thread::sleep_for(sleep);
	}
}