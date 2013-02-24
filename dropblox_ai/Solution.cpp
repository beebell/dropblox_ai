//
//  Solution.cpp
//  dropblox_ai
//
//  Created by Brenda Bell on 2/23/13.
//

#include "Solution.h"

// operators

// implement support for output stream for debugging
std::ostream& operator<<(std::ostream &strm, const CSolution &o) {
    return strm << "{CSolution:"
    << "[_startColumn=" << o._startColumn << "]"
    << "[_nbrDownCommands=" << o._nbrDownCommands << "]"
    << "[_numberOfRotations=" << o._numberOfRotations << "]"
    << "[_score=" << o._score << "]"
    << "[_nbrLeftCommands=" << o._nbrLeftCommands << "]"
    << "[_nbrRightCommands=" << o._nbrRightCommands << "]"
    << "}";
}

