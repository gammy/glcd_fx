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

#include "glcd_fx.h"
#define HALF_WIDTH	DISPLAY_WIDTH / 2 // DISPLAY_WIDTH is defined in ks0108_Panel.h

GLCDFX::GLCDFX(void) {
}

void GLCDFX::Wipe(uint8_t effect, uint8_t ms, uint8_t color) {

	int8_t x, y;

	// Note: 'width'/'height' in FillRect are 0-indexed!

	switch(effect) {
	default:
	case 0: // vertical wipe, begins from left
		for(x = 0; x < HALF_WIDTH; x++) 
			dline(x * 2, 0, 0, 63, color, ms);
		for(x = 0; x < HALF_WIDTH; x++)
			dline(DISPLAY_WIDTH - 1 - (x * 2), 0, 0, 63, color, ms);
		break;

	case 1: // horizontal wipe, begins from top
		for(y = 0; y < 32; y++)
			dline(0, y * 2, DISPLAY_WIDTH - 1, 0, color, ms); 
		for(y = 0; y < 32; y++)
			dline(0, 63 - (y * 2), DISPLAY_WIDTH - 1, 0, color, ms);
		break;

	case 2: // vertical wipe, begins from right
		for(x = 0; x < HALF_WIDTH; x++)
			dline(DISPLAY_WIDTH - 1 - (x * 2), 0, 0, 63, color, ms);
		for(x = 0; x < HALF_WIDTH; x++) 
			dline(x * 2, 0, 0, 63, color, ms);
		break;

	case 3: // horizontal wipe, begins, ms from bottom
		for(y = 0; y < 32; y++)
			dline(0, 63 - (y * 2), DISPLAY_WIDTH - 1, 0, color, ms);
		for(y = 0; y < 32; y++)
			dline(0, y * 2, DISPLAY_WIDTH - 1, 0, color, ms); 
		break;

	case 4: // vertical bidirectional wipe
		for(x = 0; x < HALF_WIDTH; x++) {
			GLCD.DrawRect(x * 2, 0, 0, 63, color);
			dline(DISPLAY_WIDTH - 1 - (x * 2), 0, 0, 63, color, ms);
		}
		break;

	case 5: // horizontal bidirectional wipe
		for(y = 0; y < 64; y++) {
			GLCD.DrawRect(0, y * 2, DISPLAY_WIDTH - 1, 0, color);
			dline(0, 63 - (y * 2), DISPLAY_WIDTH - 1, 0, color, ms);
		}
		break;

	case 6: // horizontal bidirectional half-screen wipe?
		for(x = 0; x < HALF_WIDTH; x++) {
			GLCD.DrawRect(x * 2, 0, 0, 31, color);
			dline(DISPLAY_WIDTH - 1 - (x * 2), 32, 0, 31, color, ms);
		}
		for(x = 0; x < HALF_WIDTH; x++) {
			GLCD.DrawRect(DISPLAY_WIDTH - 1 - (x * 2), 0, 0, 31, color);
			dline(x * 2, 32, 0, 31, color, ms);
		}
		break;

	case 7: // vertical bidirectional half-screen wipe?
		for(y = 0; y < 64; y++) {
			GLCD.DrawRect(0, y * 2, HALF_WIDTH - 1, 0, color);
			dline(HALF_WIDTH, 63 - (y * 2), HALF_WIDTH - 1, 0, color, ms);
		}
		for(y = 0; y < 64; y++) {
			GLCD.DrawRect(0, 63 - (y * 2), HALF_WIDTH - 1, 0, color);
			dline(HALF_WIDTH, (y * 2), HALF_WIDTH - 1, 0, color, ms);
		}
		break;

	case 8: // Combined wipe
		for(x = 0; x < HALF_WIDTH; x++) {
			GLCD.DrawRect(x * 2, 0, 0, 31, color);
			dline(DISPLAY_WIDTH - 1 - (x * 2), 32, 0, 31, color, ms);
		}
		for(y = 0; y < 64; y++) {
			GLCD.DrawRect(0, 63 - (y * 2), HALF_WIDTH - 1, 0, color);
			dline(HALF_WIDTH, (y * 2), HALF_WIDTH - 1, 0, color, ms);
		}
		for(x = 0; x < HALF_WIDTH; x++) {
			GLCD.DrawRect(DISPLAY_WIDTH - 1 - (x * 2), 0, 0, 31, color);
			dline(x * 2, 32, 0, 31, color, ms);
		}
		break;

	case 9: // Bank wipe, top to bottom
		for(x = 0; x < 8; x++) {
			GLCD.ClearPage(x, color);
			delay(ms);
		}
		break;

	case 10: // Bank wipe, bottom to top
		for(x = 0; x < 8; x++) {
			GLCD.ClearPage(7 - x, color);
			delay(ms);
		}
		break;

	case 11: // Bank wipe, edges to center
		for(x = 0; x < 4; x++) {
			GLCD.ClearPage(x, color);
			GLCD.ClearPage(7 - x, color);
			delay(ms);
		}
	case 12: // Bank wipe, center to edges
		for(x = 3; x >= 0; x--) {
			GLCD.ClearPage(x, color);
			GLCD.ClearPage(7 - x, color);
			delay(ms);
		}
		break;

	}

}

void GLCDFX::Button(uint8_t x, uint8_t y, char *text, uint8_t color) {

	uint16_t width = (5 + 1) * strlen(text) + 3; // (font width + font spacing) * strlen + spacing
	uint8_t height = 7 + 2 + 2; // Font height + 1 pixel spacing + 1 pixel border

	uint8_t tmp_color = GLCD.FontColor;
	uint8_t inv_color = color == WHITE ? BLACK : WHITE;
	
	// FIXME Retareded clearing. This shouldn't be required :[
	GLCD.FillRect(x, y, width - 1, height - 1, 
		      color == BLACK ? inv_color : color);
	
	// Draw exterior "round" border
	GLCD.DrawRoundRect(x, y, width - 1, height - 1, 1, inv_color);

	// Draw a rectangle border inside the exterior (1px padding)
	GLCD.DrawRect(x + 1, y + 1, width - 3, height - 3, inv_color);
	
	// Draw black border if the font color is black
	if(color == BLACK)
		GLCD.DrawRoundRect(x, y, width - 1, height - 1, 1, BLACK);

	// Draw the text
	GLCD.SelectFont(System5x7, color);
	GLCD.GotoXY(x + 2, y + 2);
	GLCD.Puts(text);

	// Restore font color (requires modified ks0108.h)
	GLCD.SelectFont(System5x7, tmp_color);

}

void GLCDFX::NButton(uint8_t x, uint8_t y, int value, uint8_t color) {

	char text[32]; // FIXME
	memset(text, 0, 32);

	itoa(value, (char *) &text, 10);

	Button(x, y, text, color);
}

// Instantiate
GLCDFX GLCD_FX = GLCDFX();
