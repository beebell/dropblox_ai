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

bool CBlock::operator==(const CBlock& other) const
{
    return _type == other._type
        && _center == other._center
        && _offsets == other._offsets
        && _minRowOffset == other._minRowOffset
        && _maxRowOffset == other._maxRowOffset
        && _minColOffset == other._minColOffset
        && _maxColOffset == other._maxColOffset;
    
    // _numRotations intentionally ignored
}

// instance methods

vector<CBlock> CBlock::getPermutations() const
{
    vector<CBlock> permutations;

    // add the unrotated block
    permutations.push_back(*this);

    // iterate through the rotations -- 90, 180 and 270 degrees
    for( int i = 0; i < 3; i++)
    {
        CBlock block = permutations.back()._rotate();
        
        // todo: need to test this
        // some blocks are mirror images when flipped vert or horz so
        // don't include the dups
        if(std::find(permutations.begin(), permutations.end(), block) == permutations.end())
        {
            block._numRotations = i + 1;
            permutations.push_back(block);
        }
    }
    
    return permutations;
}

// moves a block by manipulating the center coordinates
// by the specified row and column offsets
CBlock CBlock::move(int rowOffset, int colOffset) const
{
    CBlock block = *this;
    block._center.move(rowOffset, colOffset);
    
    return block;
}

// moves a block by setting the center coordinates
// to the specified row and column
CBlock CBlock::moveTo(int row, int col) const
{
    CBlock block = *this;
    block._center = CCoordinate(row, col, false);
    
    return block;
}

// renders the block to cerr to help visualization during debug sessions
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
}

// computes the min and max row/col offsets used to
// compute width and height and figure out where the
// center is in relation to the block
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
