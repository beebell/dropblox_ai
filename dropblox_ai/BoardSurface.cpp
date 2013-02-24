//
//  CBoardSurface.cpp
//  dropblox_ai
//
//  Created by Brenda Bell on 2/21/13.
//

#include "BoardSurface.h"

// lifetime management

// constructs a new instance and initializes it from
// the JSON input
CBoardSurface::CBoardSurface(const Json::Value& bitmap)
{
    
    for(int r = 0; r < BOARD_HEIGHT; r++)
    {
        const Json::Value& row = bitmap[r];
        for( int c = 0; c < BOARD_WIDTH; c++)
        {
            int val = row[c].asInt();
            _surface[r][c] = val;
            /* if (val != 0
                && _topRow == -1)
            {
                _topRow = r;
            } */
        }
    }
}

// operators

// implements support for an output stream to help debugging
std::ostream& operator<<(std::ostream &strm, const CBoardSurface &o)
{
    strm << "{CBoardSurface:"
    // << "[_topRow=" << o._topRow << "]"
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

// returns true if the current block's position is
// valid for this surface
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

// computes a score for this surface
//
// * lower scores are better
// * a cell is unreachable if the cells to the left, right and above are
//   occupied
// * unreachable cells raise the score by a factor of UNREACHABLE_PENALTY
// * a cell is obstructed if the cell above it is occupied, but the cell
//   to the left or right is open and not obstructed
// * obstructed cells raise the score by a factor of OBSTRUCTED_PENALTY
// * a mostly empty grid is better so scores are adjusted by a
//   HEIGHT_PENALTY on higher rows
// * dropping pieces away from the center has advantages, so the other
//   scores are adjusted by a COLUMN_PENALTY to create a bias away from
//   the center
float CBoardSurface::computeScore() const
{
    float score = 0;
    
    for(int r = 0; r < BOARD_HEIGHT; r++)
    {
        // the same height penalty is applied to all scenarios so
        // just compute it up front
        float heightPenalty = (BOARD_HEIGHT - r) * HEIGHT_PENALTY;
        
        for( int c = 0; c < BOARD_WIDTH; c++)
        {
            if (_surface[r][c] != 0)
            {
                score += OCCUPIED_PENALTY * heightPenalty * COLUMN_PENALTY[c];
                continue;
            }
            
            if (r > 0 && _isUnreachable(r, c))
            {
                score += UNREACHABLE_PENALTY * heightPenalty;
            }
            else if (r > 0 && _isObstructed(r, c))
            {
                score += OBSTRUCTED_PENALTY * heightPenalty;
            }
        }
    }
    
    return score;
}

// drops the specified block on the surface, returning the number of down commands
// and final score
CBoardSurface CBoardSurface::drop(const CBlock& block, int& numDown, float& score) const
{
    CBoardSurface board = *this;
    numDown = 0;
    
    CBlock tmp = block;
    while(board.check(tmp))
    {
        tmp = tmp.move(1, 0);
        numDown++;
    }
    
    tmp = tmp.move(-1, 0);
    numDown--;
    
    board._merge(tmp, score);

    return board;
}

// renders the surface to cerr to help visualization during debug sessions
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
    cerr << endl;
}

// incomplete function intended to improve a solution by seeking right and left
void CBoardSurface::seekBetterSolution(const CBlock& block, int numDown, int& numLeft, int& numRight, float& score) const
{
    numLeft = 0;
    numRight = 0;
    cerr << "current score " << score << endl;
    
    int leftShifts = 0;
    CBlock leftBlock = block;
    float leftScore = score;
    
    while(check(leftBlock))
    {
        CBoardSurface leftBoard = *this;
        float newScore;
        leftBoard._merge(leftBlock, newScore);
        if (leftScore == 0.0
            || newScore < leftScore)
        {
            leftScore = newScore;
            cerr << "found a better solution by shifting left " << leftShifts << " score=" << leftScore << endl;
        }
        
        leftBlock = leftBlock.move(0, -1);
        leftShifts++;
    }
    leftShifts--;
    
    int rightShifts = 0;
    CBlock rightBlock = block;
    float rightScore = score;
    
    while(check(rightBlock))
    {
        CBoardSurface rightBoard = *this;
        float newScore;
        rightBoard._merge(rightBlock, newScore);
        if (rightScore == 0.0
            || newScore < rightScore)
        {
            rightScore = newScore;
            cerr << "found a better solution by shifting right " << rightShifts << " score=" << rightScore << endl;
        }
        
        rightBlock = rightBlock.move(0, 1);
        rightShifts++;
    }
    rightShifts--;
    
    if (leftShifts != 0
        && leftScore < rightScore)
    {
        numLeft = leftShifts;
        score = leftScore;
    }
    else if (rightShifts != 0)
    {
        numRight = rightShifts;
        score = rightScore;
    }
    
}

// returns true if the cell at the specified row and column
// is reachable from the left or right, but not directly from
// up above
bool CBoardSurface::_isObstructed(int row, int col) const
{
    int up = row - 1;
    
    return (up < 0 || _surface[up][col] != 0);
}

// returns true if the cells to the left, right and above
// are occupied
bool CBoardSurface::_isUnreachable(int row, int col) const
{
    // todo: need to find a better way to do this -- it's too
    // rudimentary to produce a reliable result
    int left = col - 1;
    int right = col + 1;
    int up = row - 1;
    
    return (up < 0 || _surface[up][col] != 0 || _isUnreachable(up, col))
        && (left < 0 || _surface[row][left] != 0 || _isUnreachableFromLeft(row, left))
        && (right >= BOARD_WIDTH || _surface[row][right] != 0 || _isUnreachableFromRight(row, right));
}

// returns true if the specified cell is unreachable from the left
bool CBoardSurface::_isUnreachableFromLeft(int row, int col) const
{
    int left = col - 1;
    int up = row - 1;
    
    return (up < 0 || _isUnreachableFromAbove(up, col))
        && (left < 0 || _surface[left][row] != 0 || _isUnreachableFromLeft(left, row));
}

// returns true if the specified cell is unreachable from the right
bool CBoardSurface::_isUnreachableFromRight(int row, int col) const
{
    int right = col + 1;
    int up = row - 1;
    
    return (up < 0 || _isUnreachableFromAbove(up, col))
        && (right >= BOARD_WIDTH || _surface[row][right] != 0 || _isUnreachableFromRight(row, right));
}

// returns true if the specified cell is unreachable from above
bool CBoardSurface::_isUnreachableFromAbove(int row, int col) const
{
    // cout << "checking row " << row << " col " << col << endl;
    int left = col - 1;
    int right = col + 1;
    int up = row - 1;
    
    bool result = (left < 0 || _surface[row][left] != 0)
    && (right >= BOARD_WIDTH || _surface[row][right] != 0)
    && (up < 0 || _isUnreachableFromAbove(up, col));
    // cout << "result=" << result << endl;
    
    return result;
}

// merges the specified block in its current position into
// this surface and returns the resulting score
void CBoardSurface::_merge(const CBlock& block, float& score)
{
    CCoordinate center = block.getCenter();
    score = 0.0;
    
    for(int i = 0; i < block.getOffsets().size(); i++)
    {
        CCoordinate offset = block.getOffsets()[i];
        _surface[center.getRow() + offset.getRow()][center.getColumn() + offset.getColumn()] = 1;
    }
    
    _removeFullRows();
    score = computeScore();
}

// removes full rows from this surface
void CBoardSurface::_removeFullRows()
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
                memcpy(&_surface[1][0], &_surface[0][0], row * BOARD_WIDTH * sizeof(_surface[0][0]));
            }
            memset(&_surface[0][0], 0, BOARD_WIDTH * sizeof(_surface[0][0]));
            _numRowsRemoved++;
        }
    }
}
