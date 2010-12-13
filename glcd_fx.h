/* A collection of [arduino] GLCDv2-compatible effects.

   Author: Kristian Gunstone

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.
  
   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <ks0108_Panel.h>
#include <ks0108_Arduino.h>
#include <SystemFont5x7.h>
//#include <Arial14.h>
#include <ks0108_Mega.h>
#include <ks0108.h>
#include <stdlib.h>
#include <stdint.h>

#include "WProgram.h"

#ifndef GLCD_FX_H
#define GLCD_FX_H

#ifndef KS0108_H
#warn Must link against ks0108
#endif

#define dline(a, b, c, d, e, f)	{GLCD.FillRect((a), (b), (c), (d), (e));\
				 delay((f));}

#define DISPLAY_CX	(DISPLAY_WIDTH / 2)
#define DISPLAY_CY	(DISPLAY_HEIGHT / 2)

class GLCDFX {
	
	public:
		GLCDFX(void);
		void Wipe(uint8_t effect, uint8_t ms, uint8_t color);
		void Button(uint8_t x, uint8_t y, char *text, uint8_t color);
		void NButton(uint8_t x, uint8_t y, int value, uint8_t color);
	
	private:

};

extern GLCDFX GLCD_FX;

#endif
