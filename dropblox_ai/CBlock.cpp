//
//  CBlock.cpp
//  dropblox_ai
//
//  Created by Brenda Bell on 2/21/13.
//

#include "CBlock.h"

// lifetime management

CBlock::CBlock(const CBlock& other)
{
    _type = other._type;
    _center = other._center;
    _offsets = other._offsets;
    _numRotations = other._numRotations;
    _computeSize();
}

CBlock::CBlock(const Json::Value& block)
{
    _type = block["type"].asInt();
    _center = CCoordinate(block["center"]);
    
    Json::Value offsets = block["offsets"];
    int numOffsets = offsets.size();
    
    for( int i = 0; i < numOffsets; i++)
    {
        _offsets.push_back(CCoordinate(offsets[i]));
    }
    
    _computeSize();
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

// instance methods

vector<CBlock> CBlock::getPermutations() const
{
    vector<CBlock> permutations;
    
    permutations.push_back(*this);
    
    for( int i = 0; i < 3; i++)
    {
        // todo: eliminate dups
        CBlock block = permutations.back()._rotate();
        block._numRotations = i + 1;
        permutations.push_back(block);
    }
    
    return permutations;
}

CBlock CBlock::move(int rowOffset, int colOffset) const
{
    CBlock block = *this;
    block._center.move(rowOffset, colOffset);
    
    return block;
}

CBlock CBlock::moveTo(int row, int col) const
{
    CBlock block = *this;
    block._center = CCoordinate(row, col);
    
    return block;
}

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

void CBlock::_computeSize()
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

CBlock CBlock::_rotate() const
{
    CBlock block = *this;
    
    for(int i = 0; i < _offsets.size(); i++)
    {
        block._offsets[i].rotate();
    }
    block._computeSize();
    
    return block;
}
