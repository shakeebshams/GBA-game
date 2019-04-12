#include "gba.h"
#include "logic.h"
#include "draw.h"

//#include "images/garbage.h"
#include "images/hogwarts.h"
#include "images/begin0.h"
#include "images/apple.h"
#include "images/win.h"

#include <stdio.h>
#include <stdlib.h>

// AppState enum definition
typedef enum {
    // TA-TODO: Add any additional states you need for your app.
    START,
    START_NODRAW,
	APP_FEED,
	APP_WIN,
    APP_EXIT_NODRAW,
} GBAState;


int main(void) {
    // TA-TODO: Manipulate REG_DISPCNT here to set Mode 3.
    REG_DISPCNT = MODE3 | BG2_ENABLE;

    GBAState state = START;


    u32 previousButtons = BUTTONS;
    u32 currentButtons = BUTTONS;

	//parameters for fuel
	Coordinate fuelLit = {60, 150};
	int appleSize = 15;

    while(1) {
        // Load the current state of the buttons
        currentButtons = BUTTONS;	
		waitForVBlank();
		if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
			state = START;
		}

        // TA-TODO: Manipulate the state machine below as needed.
        switch(state) {
        case START:
            // Wait for VBlank
            waitForVBlank();

            // TA-TODO: Draw the start state here.
			drawFullScreenImageDMA(hogwarts);

            state = START_NODRAW;
            break;
        case START_NODRAW:
            // TA-TODO: Check for a button press here to start the app.
            // Start the app by switching the state to APP_INIT.

			if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
				//printf("what happened?\n");
				drawFullScreenImageDMA(begin0);
				drawImageDMA(fuelLit.row, fuelLit.col, appleSize, appleSize, apple);
				state = APP_FEED;
			}
			break;
		case APP_FEED:
			if (fuelLit.row <= 120 && fuelLit.row >= 60 && fuelLit.col <= 100 && fuelLit.col >= 70) {
				waitForVBlank();
				drawFullScreenImageDMA(win);
				char *letter = "Congrats!";
				drawString(20, 20, letter, WHITE);
				char *line2 = "You saved the day!";
				drawString(20, 30, line2, WHITE);
				char *line3 = "NASA can now colonize the moon!";
				drawString(20, 40, line3, WHITE);
				state = APP_EXIT_NODRAW;
				fuelLit.row = 40;
				fuelLit.col = 180;
				break;
			}
			if (KEY_JUST_PRESSED(BUTTON_RIGHT, currentButtons, previousButtons)) {
		   		fuelLit.col+=2; 
				//drawFullScreenImageDMA(begin0);
				drawImageDMA(fuelLit.row, fuelLit.col, appleSize, appleSize, apple);
				drawRectangle(fuelLit.row, fuelLit.col-2, 2, appleSize, begin0);
				break;
			}
			if (KEY_JUST_PRESSED(BUTTON_LEFT, currentButtons, previousButtons)) {
		   		fuelLit.col-=2;
				//drawFullScreenImageDMA(begin0);
				drawImageDMA(fuelLit.row, fuelLit.col, appleSize, appleSize, apple);
				drawRectangle(fuelLit.row, fuelLit.col+appleSize, 2, appleSize, begin0);
				break;
			}
			if (KEY_JUST_PRESSED(BUTTON_UP, currentButtons, previousButtons)) {
		    	fuelLit.row-=2;
				//drawFullScreenImageDMA(begin0);
				drawImageDMA(fuelLit.row, fuelLit.col, appleSize, appleSize, apple);
				drawRectangle(appleSize+fuelLit.row, fuelLit.col, appleSize, 2, begin0);
				break;
			}
			if (KEY_JUST_PRESSED(BUTTON_DOWN, currentButtons, previousButtons)) {
		    	fuelLit.row+=2; 
				//drawFullScreenImageDMA(begin0);
				drawImageDMA(fuelLit.row, fuelLit.col, appleSize, appleSize, apple);
				drawRectangle(fuelLit.row-2, fuelLit.col, appleSize, 2, begin0);
			}
			break;
		case APP_WIN:
        	waitForVBlank();
        	drawFullScreenImageDMA(win);
        	state = APP_EXIT_NODRAW;
        	break;

        case APP_EXIT_NODRAW:
            // TA-TODO: Check for a button press here to go back to the start screen
			if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
				state = START;
			}
            break;
        }

        // Store the current state of the buttons
        previousButtons = currentButtons;
    }

    return 0;
}
