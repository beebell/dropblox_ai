//
//  CBlock.h
//  dropblox_ai
//
//  Created by Brenda Bell on 2/21/13.
//

#ifndef __dropblox_ai__CBlock__
#define __dropblox_ai__CBlock__

#include <iostream>

#include "jsoncpp/json/json.h"

#include "Coordinate.h"

using namespace std;

class CBlock
{

// fields
    
private:
    
    // fields to track what we received in the JSON payload
    
    int _type = 0;
    
    CCoordinate _center;
    
    vector<CCoordinate> _offsets;
    
    // fields to track the minimum and maximum offsets required to compute height and width
    // and figure out where the center is in relation to the rest of the block
    
    int _minRowOffset = 0;
    
    int _maxRowOffset = 0;
    
    int _minColOffset = 0;
    
    int _maxColOffset = 0;
    
    // if this is a rotated block, indicates the number of 90-degree clockwise rotations
    // to arrive at this representation
    int _numRotations = 0;
    
// properties
    
public:
    
    const CCoordinate& getCenter() const { return _center; }
    
    const vector<CCoordinate>& getOffsets() const { return _offsets; }
    
    int getHeight() const { return _maxRowOffset - _minRowOffset + 1; }
    
    int getWidth() const { return _maxColOffset - _minColOffset + 1; }
    
    int getNumRotations() const { return _numRotations; }
    
// lifetime management
    
public:
    
    CBlock(const CBlock& other);
    
    CBlock(const Json::Value& block);
    
// operators
    
private:
    
    friend std::ostream& operator << (std::ostream&, const CBlock&);
    
public:
    
    // bool operator == (const CBlock&) const;
    
// instance methods
    
private:
    
    void _computeBoundingOffsets();
    
    bool _isReciprocal(const CBlock& orig, const CBlock& rotated) const;
    
    CBlock _rotate() const;
    
public:
    
    vector<CBlock> getPermutations() const;
    
    CBlock move(int rowOffset, int colOffset) const;
    
    CBlock moveTo(int row, int col) const;
    
    void render() const;
    
};

#endif /* defined(__dropblox_ai__CBlock__) */
