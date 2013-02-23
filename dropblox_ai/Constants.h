//
//  Constants.h
//  dropblox_ai
//
//  Created by Brenda Bell on 2/23/13.
//

#ifndef dropblox_ai_Constants_h
#define dropblox_ai_Constants_h

const int BOARD_HEIGHT = 33;
const int BOARD_WIDTH = 12;

const int UNREACHABLE_PENALTY = 12;

// change this to 4 after left and right are working
const int OBSTRUCTED_PENALTY = 12;
const int OCCUPIED_PENALTY = 1;
const int HEIGHT_PENALTY = 1.1;

const float COLUMN_PENALTY[BOARD_WIDTH] = {
    1.0,1.1,1.2,1.3,1.4,1.5,1.5,1.4,1.3,1.2,1.1,1.0
};

#endif
