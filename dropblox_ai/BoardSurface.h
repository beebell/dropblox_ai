//
//  CBoardSurface.h
//  dropblox_ai
//
//  Created by Brenda Bell on 2/21/13.
//

#ifndef __dropblox_ai__CBoardSurface__
#define __dropblox_ai__CBoardSurface__

#include <iostream>

#include "jsoncpp/json/json.h"

#include "Block.h"

using namespace std;

const int BOARD_HEIGHT = 33;
const int BOARD_WIDTH = 12;

class CBoardSurface
{
    
// fields
    
private:

    // board state
    int _surface[BOARD_HEIGHT][BOARD_WIDTH];
    
    // the number of rows removed from this surface during a drop
    int _numRowsRemoved = 0;
    
// properties
    
public:
    
    int getNumRowsRemoved() { return _numRowsRemoved; }
    
// lifetime management
    
public:
    
    CBoardSurface(const Json::Value& bitmap);
    
// operators
    
private:
    
    friend std::ostream& operator<<(std::ostream&, const CBoardSurface&);
    
// instance methods
    
private:
    
    bool _isObstructed(int row, int col) const;
    
    bool _isUnreachable(int row, int col) const;
    
public:

    bool check(const CBlock& block) const;
    
    float computeScore() const;
    
    CBoardSurface drop(const CBlock& block) const;
    
    void removeFullRows();
    
    void render() const;
    
};

#endif /* defined(__dropblox_ai__CBoardSurface__) */
