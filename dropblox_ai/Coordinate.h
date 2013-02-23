//
//  CCoordinate.h
//  dropblox_ai
//
//  Created by Brenda Bell on 2/21/13.
//

#ifndef __dropblox_ai__CCoordinate__
#define __dropblox_ai__CCoordinate__

#include <iostream>

#include "jsoncpp/json/json.h"

using namespace std;

class CCoordinate
{
    
// fields
    
private:
    
    int _isRelative = false;
    
    int _row = 0;
    
    int _col = 0;
    
// instance properties
    
public:
    
    int getRow() const { return _row; }
    
    int getColumn() const { return _col; }
    
// lifetime management
    
public:
    
    CCoordinate() {};
    
    CCoordinate(const CCoordinate& other);
    
    CCoordinate(const Json::Value& location, bool isRelative);
    
    CCoordinate(int row, int col, bool isRelative);
    
// operators
    
private:
    
    friend std::ostream& operator<<(std::ostream&, const CCoordinate&);
    
    friend std::ostream& operator<<(std::ostream&, const vector<CCoordinate>&);
    
public:
    
    bool operator == (const CCoordinate&) const;
    
// instance methods
    
private:
    
    void _setLocation(int row, int col, bool isRelative);

public:
    
    void rotate();
    
    void move(int rowOffset, int colOffset);
    
};

#endif /* defined(__dropblox_ai__CCoordinate__) */
