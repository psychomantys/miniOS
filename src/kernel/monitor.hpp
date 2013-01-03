/*
 * =====================================================================================
 *
 *       Filename:  monitor.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/13/2012 08:48:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef MONITOR_HPP
#define MONITOR_HPP

#include	<stdint.h>

/*
// Write a single character out to the screen.
void monitor_put(const char c);

// Clear the screen to all black.
void monitor_clear();

// Output a null-terminated ASCII string to the monitor.
void monitor_write(const char *c);
*/

class VGA {
	public:
		typedef enum{
			black=0,
			blue,
			green,
			cyan,
			red,
			magenta,
			brown,
			light_grey,
			dark_grey,
			light_blue,
			light_green,
			light_cyan,
			light_red,
			light_magenta,
			light_brown,
			white
		} Color;

		VGA();

		// Write a single character out to the screen.
		void put(const char &c);

		// Clear the screen to all black.
		void clear();

		// Output a null-terminated ASCII string to the monitor.
		void write(const char *c);

		void settextcolor(const uint8_t &forecolor, const uint8_t &backcolor);
	private:
		uint16_t * const video_memory;
		uint8_t cursor_x,cursor_y;
		uint16_t attribute;
		void move_cursor();
		void scroll();
};

#endif // MONITOR_HPP

