//
//  main.cpp
//  dropblox_ai
//
//  Created by Brenda Bell on 2/20/13.
//

#include <assert.h>
#include <cstdio>
#include <cstring>
#include <iostream>

#include "jsoncpp/json/json.h"

#include "Block.h"
#include "BoardSurface.h"
#include "Constants.h"
#include "Coordinate.h"

using namespace std;

int main(int argc, const char * argv[])
{
    
    if (argc != 3)
    {
        cerr << "Unrecognized input" << endl;
        return 1;
    }
    
    const char *input = argv[1];
    // float timeRemaining = atof(argv[2]);
    
    Json::Value root;
    Json::Reader reader;
    bool parsedSuccess = reader.parse(
            input,
            root,
            false /* collectComments */ );
    
    if(!parsedSuccess)
    {
        cerr << "Failed to parse JSON"
            << endl << reader.getFormatedErrorMessages()
            << endl;
        return 1;
    }
    
    CBoardSurface surface(root["bitmap"]);
    surface.render();
    
    CBlock block(root["block"]);
    cerr << "current block: " << block << endl;
    block.render();
    cerr << endl;
    
    vector<CBlock> permutations = block.getPermutations();
    
    int bestcc = -1;
    float bestScore = -1;
    int bestPermutation = -1;

    srand((unsigned int)time(NULL));
    
    for( int perm = 0; perm < permutations.size(); perm++)
    {
        int k = rand() % BOARD_WIDTH;
        
        const CBlock& permutation = permutations[perm];
        cerr << "trying permutation " << perm << ": " << permutation << endl;
        permutation.render();
        cerr << endl;
        
        for (int cc = 0; cc < BOARD_WIDTH; cc++)
        {
            int c3 = (cc + k) % BOARD_WIDTH;
            cerr << "trying col " << c3 << "..." << endl;
            CBlock tmp = permutation.moveTo(permutation.getCenter().getRow(), c3);
            if (surface.check(tmp))
            {
                CBoardSurface newSurface = surface.drop(tmp);
                // newSurface.render();
                float score = newSurface.computeScore();
                cerr << "score " << score << endl;
                if (bestPermutation == -1
                    || score < bestScore)
                {
                    cerr << "recording new best score..." << endl;
                    permutation.render();
                    bestPermutation = perm;
                    bestcc = c3;
                    bestScore = score;
                    cerr << "best score:" << bestScore << " permutation: " << bestPermutation << " cc:" << bestcc <<
                        " removed: " << newSurface.getNumRowsRemoved() << endl << endl;
                }
            }
            else
            {
                cerr << "not valid" << endl;
            }
        }
    }
    
    cerr << endl << "best score:" << bestScore << " permutation: " << bestPermutation << " cc:" << bestcc << endl;
    
    if (bestPermutation != -1
        && bestcc != -1)
    {
        for(int numRotations = permutations[bestPermutation].getNumRotations(); numRotations > 0; numRotations--)
        {
            cout << "rotate" << endl;
        }
        
        while(bestcc < permutations[bestPermutation].getCenter().getColumn())
        {
            cout << "left" << endl;
            cout.flush();
            bestcc++;
        }
        
        while(bestcc > permutations[bestPermutation].getCenter().getColumn())
        {
            cout << "right" << endl;
            cout.flush();
            bestcc--;
        }
    }
    else
    {
        // something's really wrong
        surface.render();
        cerr << endl << block << endl << endl;
        block.render();
        assert(false);
    }
    
    return 0;
}
                    
