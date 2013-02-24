//
//  CBlock.cpp
//  dropblox_ai
//
//  Created by Brenda Bell on 2/21/13.
//

#include "Block.h"

// lifetime management

// copy constructor
CBlock::CBlock(const CBlock& other)
{
    // copy the base fields, then recompute the variables
    // used to compute size
    _type = other._type;
    _center = other._center;
    _offsets = other._offsets;
    _numRotations = other._numRotations;
    _computeBoundingOffsets();
}

// construct a new instance and initialize it from
// the JSON input
CBlock::CBlock(const Json::Value& block)
{
    _type = block["type"].asInt();
    _center = CCoordinate(block["center"], false /* isRelative */);
    
    Json::Value offsets = block["offsets"];
    int numOffsets = offsets.size();
    
    for( int i = 0; i < numOffsets; i++)
    {
        _offsets.push_back(CCoordinate(offsets[i], true /* isRelative */));
    }
    
    _computeBoundingOffsets();
}

// operators

// implement support for output stream for debugging
std::ostream& operator<<(std::ostream &strm, const CBlock &o) {
    return strm << "{CBlock:"
    << "[_type=" << o._type << "]"
    << "[_center=" << o._center << "]"
    << "[_numRotations=" << o._numRotations << "]"
    << "[_offsets=" << o._offsets << "]"
    << "[height=" << o.getHeight() << "]"
    << "[width=" << o.getWidth() << "]"
    << "}";
}

// instance methods

// returns a collection containing all of the rotated
// permutations for this block
vector<CBlock> CBlock::getPermutations() const
{
    vector<CBlock> permutations;

    // start with the original unrotated block
    permutations.push_back(*this);
    
    // iterate through the rotations -- 90, 180 and 270 degrees
    for( int i = 0; i < 3; i++)
    {
        CBlock block = permutations.back()._rotate();
        
        // todo: only add this permutation to the collection
        // if it isn't visually the same as one that already
        // exists
        
        block._numRotations = i + 1;
        permutations.push_back(block);
    }
    
    return permutations;
}

// moves a block by manipulating its center coordinates
// by the specified row and column offsets
CBlock CBlock::move(int rowOffset, int colOffset) const
{
    CBlock block = *this;
    block._center.move(rowOffset, colOffset);
    
    return block;
}

// moves a block by setting its center coordinates
// to the specified row and column
CBlock CBlock::moveTo(int row, int col) const
{
    CBlock block = *this;
    block._center = CCoordinate(row, col, false /* isRelative */);
    
    return block;
}

// renders a visual representation of the block to help debugging
void CBlock::render() const
{
    int width = getWidth();
    int height = getHeight();
    
    int arr[height][width];
    memset(&arr, 0, height * width * sizeof(int));
    for(int i = 0; i < _offsets.size(); i++)
    {
        arr[_offsets[i].getRow() - _minRowOffset][_offsets[i].getColumn() - _minColOffset] = 1;
    }
    
    for(int r = 0; r < height; r++)
    {
        for( int c = 0; c < width; c++)
        {
            if (r == 0 - _minRowOffset
                && c == 0 - _minColOffset)
            {
                cerr << "c";
                continue;
            }
            if (arr[r][c] != 0)
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

// computes the min and max row/col offsets used to
// compute width and height of the block and figure
// out where the center is relative to the edges of
// the rectangle
void CBlock::_computeBoundingOffsets()
{
    vector<CCoordinate>::iterator itr;
    for(itr = _offsets.begin();
        itr != _offsets.end();
        itr++)
    {
        int row = itr->getRow();
        _minRowOffset = min(_minRowOffset, row);
        _maxRowOffset = max(_maxRowOffset, row);
        
        int col = itr->getColumn();
        _minColOffset = min(_minColOffset, col);
        _maxColOffset = max(_maxColOffset, col);
    }
}

// returns true if the shape of the block is the same
// when rotated 180 degrees
bool CBlock::_isReciprocal(const CBlock &orig, const CBlock &rotated) const
{
    // todo: implement this method
    return false;
}

// rotates a block 90 degrees clockwise
CBlock CBlock::_rotate() const
{
    CBlock block = *this;
    
    for(int i = 0; i < _offsets.size(); i++)
    {
        block._offsets[i].rotate();
    }
    block._computeBoundingOffsets();
    
    return block;
}
