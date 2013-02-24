//
//  Constants.h
//  dropblox_ai
//
//  Created by Brenda Bell on 2/23/13.
//

#ifndef dropblox_ai_Constants_h
#define dropblox_ai_Constants_h

// fixed width and height of the playing surface
const int BOARD_HEIGHT = 33;
const int BOARD_WIDTH = 12;

// various penalties to impose when computing the score for a particular solution

// prefer columns furthest away from the center
const float COLUMN_PENALTY[BOARD_WIDTH] = {
    1.0,1.02,1.04,1.06,1.08,1.10,1.10,1.08,1.06,1.04,1.02,1.0
};

// prefer keeping the height as low as possible
const int HEIGHT_PENALTY = 1.1;

// obstructed blocks are expensive
const int OBSTRUCTED_PENALTY = 4; // todo: make this lower than unreachable after left and right commands are working

// occupied blocks are normal
const int OCCUPIED_PENALTY = 1;

// unreachable blocks are expensive
const int UNREACHABLE_PENALTY = 4;

#endif
