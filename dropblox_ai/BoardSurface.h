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
#include "Constants.h"

using namespace std;

// class to represent the board
class CBoardSurface
{
    
// fields
    
private:

    // board state
    int _surface[BOARD_HEIGHT][BOARD_WIDTH];
    
    // the number of rows removed from this surface on a drop
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
    
    bool _isUnreachableFromLeft(int row, int col) const;
    
    bool _isUnreachableFromRight(int row, int col) const;
    
    bool _isUnreachableFromAbove(int row, int col) const;
    
    void _merge(const CBlock& block, float& score);
    
    void _removeFullRows();
    
public:

    bool check(const CBlock& block) const;
    
    float computeScore() const;
    
    CBoardSurface drop(const CBlock& block, int& numDown, float& score) const;
    
    void render() const;
    
    void seekBetterSolution(const CBlock& block, int numDown, int& numLeft, int& numRight, float& score) const;
    
};

#endif /* defined(__dropblox_ai__CBoardSurface__) */
