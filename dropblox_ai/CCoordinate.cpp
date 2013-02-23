//
//  CCoordinate.cpp
//  dropblox_ai
//
//  Created by Brenda Bell on 2/21/13.
//

#include <algorithm>

#include "CCoordinate.h"

using namespace std;

// lifetime management

CCoordinate::CCoordinate(const CCoordinate& other)
{
    _row = other._row;
    _col = other._col;
}

CCoordinate::CCoordinate(const Json::Value& location)
{
    _setLocation(location["i"].asInt(), location["j"].asInt());
}

CCoordinate::CCoordinate(int row, int col)
{
    _setLocation(row, col);
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

// instance methods

void CCoordinate::rotate()
{
    swap(_row, _col);
    _col *= -1;
}

void CCoordinate::move(int rowOffset, int colOffset)
{
    _row += rowOffset;
    _col += colOffset;
}

void CCoordinate::_setLocation(int row, int col)
{
    _row = row;
    _col = col;
}
