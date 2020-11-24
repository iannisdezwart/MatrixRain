#ifndef HUE_TO_RGB_HEADER
#define HUE_TO_RGB_HEADER

#include <bits/stdc++.h>

using namespace std;

uint32_t hue_to_rgb(double hue_ratio)
{
	// We multiply the `hue_ratio` by 6 and cast it to an int.
	// This will initialise `int angle_sector` to an integer between [0, 6].
	// The edge case `angle_sector == 6` will be handled by the switch below.

	int angle_sector = hue_ratio * 6; // [0, 6] (int)

	// We calculate how far the angle is from the `angle_sector` base.
	// We store this value into `int remaining_value` and multiply it by 255
	// to make it a pixel value.

	int remaining_value = ((hue_ratio * 6) - (double) angle_sector) * 255.0;

	// The switch will return the correct rgb value

	switch (angle_sector) {
		case 0:
		default: // If the `angle_sector` is 6 (equivalent to 6 mod 6 == 0)
			return 255 << 16 | remaining_value << 8 | 0;
		case 1:
			return (255 - remaining_value) << 16 | 255 << 8 | 0;
		case 2:
			return 0 << 16 | 255 << 8 | remaining_value;
		case 3:
			return 0 << 16 | (255 - remaining_value) << 8 | 255;
		case 4:
			return remaining_value << 16 | 0 << 8 | 255;
		case 5:
			return 255 << 16 | 0 << 8 | (255 - remaining_value);
	}
}

#endif