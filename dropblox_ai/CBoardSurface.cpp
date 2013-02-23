//
//  CBoardSurface.cpp
//  dropblox_ai
//
//  Created by Brenda Bell on 2/21/13.
//

#include "CBoardSurface.h"

// lifetime management

CBoardSurface::CBoardSurface(const Json::Value& bitmap)
{
    
    for(int r = 0; r < BOARD_HEIGHT; r++)
    {
        const Json::Value& row = bitmap[r];
        for( int c = 0; c < BOARD_WIDTH; c++)
        {
            int val = row[c].asInt();
            _surface[r][c] = val;
            if (val != 0
                && _topRow == -1)
            {
                _topRow = r;
            }
        }
    }
}

// operators

std::ostream& operator<<(std::ostream &strm, const CBoardSurface &o)
{
    strm << "{CBoardSurface:"
    << "[_topRow=" << o._topRow << "]"
    << "[_surface=";
    
    for(int r = 0; r < BOARD_HEIGHT; r++)
    {
        strm << "[";
        for(int c = 0; c < BOARD_WIDTH; c++)
        {
            if (c > 0)
            {
                strm << ",";
            }
            strm << o._surface[r][c];
        }
        strm << "]";
    }
    return strm << "]"
        << "[score=" << o.computeScore() << "]"
        << "}";
}

// instance methods

bool CBoardSurface::check(const CBlock& block) const
{
    vector<CCoordinate> offsets = block.getOffsets();
    for(int i = 0; i < offsets.size(); i++)
    {
        int r = block.getCenter().getRow() + offsets[i].getRow();
        int c = block.getCenter().getColumn() + offsets[i].getColumn();
        
        if (r < 0
            || r >= BOARD_HEIGHT
            || c < 0
            || c >= BOARD_WIDTH
            || _surface[r][c])
            return false;
    }
    
    return true;
}

float CBoardSurface::computeScore() const
{
    float score = 0;
    
    for(int r = 0; r < BOARD_HEIGHT; r++)
    {
        float factor = (BOARD_HEIGHT - r) * .1;
        for( int c = 0; c < BOARD_WIDTH; c++)
        {
            if (_surface[r][c] != 0)
            {
                score += 1 * factor;
                continue;
            }
            
            if (r > 0 && _isUnreachable(r, c))
            {
                score += 4 * factor;
            }
            else if (r > 0 && _isObstructed(r, c))
            {
                score += 2 * factor;
            }
        }
    }
    
    return score;
}

CBoardSurface CBoardSurface::drop(const CBlock& block) const
{
    CBoardSurface board = *this;
    
    CBlock tmp = block;
    while(board.check(tmp))
    {
        tmp = tmp.move(1, 0);
    }
    tmp = tmp.move(-1, 0);
    
    CCoordinate center = tmp.getCenter();

    for(int i = 0; i < tmp.getOffsets().size(); i++)
    {
        CCoordinate offset = tmp.getOffsets()[i];
        board._surface[center.getRow() + offset.getRow()][center.getColumn() + offset.getColumn()] = 1;
    }
    
    board.removeFullRows();
    
    return board;
}

void CBoardSurface::removeFullRows()
{
    for(int row = BOARD_HEIGHT - 1; row >= 0; row--)
    {
        int numOccupied = 0;
        for( int col = 0; col < BOARD_WIDTH; col++)
        {
            if (_surface[row][col] != 0)
            {
                numOccupied++;
            }
        }
        if (numOccupied == 0)
        {
            break;
        }
        if (numOccupied == BOARD_WIDTH)
        {
            if (row > 0)
            {
                // todo: verify that overlapping memcpy is safe
                memcpy(&_surface[1][0], &_surface[0][0], row * BOARD_WIDTH * sizeof(_surface[0][0]));
            }
            memset(&_surface[0][0], 0, BOARD_WIDTH * sizeof(_surface[0][0]));
            _numRowsRemoved++;
        }
    }
}

void CBoardSurface::render() const
{
    for(int r = 0; r < BOARD_HEIGHT; r++)
    {
        for( int c = 0; c < BOARD_WIDTH; c++)
        {
            if (_surface[r][c] != 0)
            {
                cerr << "x";
                continue;
            }
            cerr << ".";
        }
        cerr << endl;
    }
}

bool CBoardSurface::_isObstructed(int row, int col) const
{
    int up = row - 1;
    
    return (up < 0 || _surface[up][col] != 0);
}

bool CBoardSurface::_isUnreachable(int row, int col) const
{
    int left = col - 1;
    int right = col + 1;
    int up = row - 1;
    
    return (left < 0 || _surface[left][row] != 0)
        && (right >= BOARD_WIDTH || _surface[right][row] != 0)
        && (up < 0 || _surface[up][col] != 0);
}
