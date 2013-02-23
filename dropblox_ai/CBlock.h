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

#include "CCoordinate.h"

using namespace std;

class CBlock
{

// fields
    
private:
    
    int _type = 0;
    
    CCoordinate _center;
    
    vector<CCoordinate> _offsets;
    
    int _minRowOffset = 0;
    
    int _maxRowOffset = 0;
    
    int _minColOffset = 0;
    
    int _maxColOffset = 0;
    
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
    
// instance methods
    
private:
    
    void _computeSize();
    
    CBlock _rotate() const;
    
public:
    
    vector<CBlock> getPermutations() const;
    
    CBlock move(int rowOffset, int colOffset) const;
    
    CBlock moveTo(int row, int col) const;
    
    void render() const;
    
};

#endif /* defined(__dropblox_ai__CBlock__) */
