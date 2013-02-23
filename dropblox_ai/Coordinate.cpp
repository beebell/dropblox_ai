//
//  CCoordinate.cpp
//  dropblox_ai
//
//  Created by Brenda Bell on 2/21/13.
//

#include <algorithm>
#include <assert.h>

#include "Constants.h"
#include "Coordinate.h"

using namespace std;

// lifetime management

CCoordinate::CCoordinate(const CCoordinate& other)
{
    _setLocation(other._row, other._col, other._isRelative);
}

CCoordinate::CCoordinate(const Json::Value& location, bool isRelative)
{
    // just assume the JSON is valid
    _setLocation(location["i"].asInt(), location["j"].asInt(), isRelative);
}

CCoordinate::CCoordinate(int row, int col, bool isAbsolute)
{
    _setLocation(row, col, isAbsolute);
}

// operators

std::ostream& operator<<(std::ostream &strm, const CCoordinate &o)
{
    return strm << "{CCoordinate:"
    << "[_row=" << o._row << "]"
    << "[_col=" << o._col << "]"
    << "}";
}

std::ostream& operator<<(std::ostream &strm, const vector<CCoordinate>& a)
{
    strm << "[";
    
    vector<const CCoordinate>::iterator itr;
    for(itr = a.begin();
        itr != a.end();
        itr++)
    {
        if (itr != a.begin())
        {
            strm << ",";
        }
        strm << *itr;
    }
    
    return strm << "]";
}

bool CCoordinate::operator == (const CCoordinate& other) const
{
    return _row == other._row
            && _col == other._col
            && _isRelative == other._isRelative;
}

// instance methods

void CCoordinate::rotate()
{
    assert(_isRelative);
    swap(_row, _col);
    _col *= -1;
}

void CCoordinate::move(int rowOffset, int colOffset)
{
    _setLocation(_row + rowOffset, _col + colOffset, _isRelative);
}

void CCoordinate::_setLocation(int row, int col, bool isRelative)
{
    _row = row;
    _col = col;
    _isRelative = isRelative;
    assert(_isRelative
           || (_row >= 0
               && _row < BOARD_HEIGHT
               && _col >= 0
               && _col < BOARD_WIDTH));
}
