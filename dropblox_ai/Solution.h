//
//  Solution.h
//  dropblox_ai
//
//  Created by Brenda Bell on 2/23/13.
//

#ifndef __dropblox_ai__Solution__
#define __dropblox_ai__Solution__

#include <iostream>

// this class captures the details for one solution for a specific block and board layout

class CSolution
{
    
// fields
    
public:
    
    // todo: make the fields private and implement accessors
    
    int _startColumn = 0;
    
    int _nbrDownCommands = 0;
    
    int _numberOfRotations = 0;
    
    float _score = 0.0;
    
    int _nbrLeftCommands = 0;
    
    int _nbrRightCommands = 0;
    
// lifetime management
    
public:
    
    CSolution() { }
    
// operators
    
private:
    
    friend std::ostream& operator << (std::ostream&, const CSolution&);
    
};

#endif /* defined(__dropblox_ai__Solution__) */
