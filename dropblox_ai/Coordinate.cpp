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

// copy constructor
CCoordinate::CCoordinate(const CCoordinate& other)
{
    _setLocation(other._row, other._col, other._isRelative);
}

// constructs a new instance and initializes if from
// JSON input; isRelative is set to true for block centers and to
// false for offsets
CCoordinate::CCoordinate(const Json::Value& location, bool isRelative)
{
    // just assume the JSON is valid
    _setLocation(location["i"].asInt(), location["j"].asInt(), isRelative);
}

// constructs a new instance and initializes to the specified
// row and column
CCoordinate::CCoordinate(int row, int col, bool isRelative)
{
    _setLocation(row, col, isRelative);
}

// operators

// implements support for output stream to help debugging
std::ostream& operator<<(std::ostream &strm, const CCoordinate &o)
{
    return strm << "{CCoordinate:"
    << "[_row=" << o._row << "]"
    << "[_col=" << o._col << "]"
    << "[_isRelative" << o._isRelative << "]"
    << "}";
}

// implements support for output stream to help debugging
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

// instance methods

// helper method to *rotate* this coordinate
void CCoordinate::rotate()
{
    assert(_isRelative);
    swap(_row, _col);
    _col *= -1;
}

// shifts this coordinate by the specified offsets
void CCoordinate::move(int rowOffset, int colOffset)
{
    _setLocation(_row + rowOffset, _col + colOffset, _isRelative);
}

// changes this coordate to the specified row and column
void CCoordinate::_setLocation(int row, int col, bool isRelative)
{
    _row = row;
    _col = col;
    _isRelative = isRelative;
}
