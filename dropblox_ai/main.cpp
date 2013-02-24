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
#include "Solution.h"

using namespace std;

// variables for tracking the best known solution
int bestColumnToDrop = -1;
float bestScore = -1;
int bestPermutation = -1;

// a map to hold all of the investigated solutions
multimap<float, CSolution> solutionMap;

// when looking for possible solutions, work from the outer columns
// towards the center
int colOrder[] = {0,11,1,10,2,9,3,8,4,7,5,6};

void findBestPositionForPermutation(const CBoardSurface&, const CBlock&);

void seekBetterSolution(const CBoardSurface&, const CBlock&, int);

int main(int argc, const char * argv[])
{
    
    if (argc != 3)
    {
        cerr << "Unrecognized input" << endl;
        return 1;
    }
    
    const char *input = argv[1];
    
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
    surface.render(); // renders the game board for debug
    
    CBlock block(root["block"]);
    block.render(); // renders the current piece for debug
    
    vector<CBlock> permutations = block.getPermutations();
    
    // iterate through the permutations for the current block to find the solution
    // with the lowest score
    for( int permutationIdx = 0; permutationIdx < permutations.size(); permutationIdx++)
    {
        const CBlock& permutation = permutations[permutationIdx];
        permutation.render(); // debug
        
        findBestPositionForPermutation(surface, permutation);
    }

    // dump some debug stuff
    cerr << endl << "number of solutions evaluated " << solutionMap.size() << endl;
    const CSolution& best = solutionMap.begin()->second;
    cerr << endl << "best solution: " << best << endl;

    // see comment in seekBetterSolution(): we'd generally want to take the best solution from the map
    // but it's not working yet so this is commented out
    // bestPermutation = best._permutation;
    
    if (bestPermutation == -1)
    {
        // something's really wrong
        surface.render();
        cerr << endl << block << endl << endl;
        block.render();
        assert(false);
    }

    // rotate the block
    for(int numRotations = permutations[bestPermutation].getNumRotations(); numRotations > 0; numRotations--)
    {
        cout << "rotate" << endl;
    }
    cout.flush();

    // move it left or right
    while(bestColumnToDrop < permutations[bestPermutation].getCenter().getColumn())
    {
        cout << "left" << endl;
        bestColumnToDrop++;
    }
    cout.flush();
    while(bestColumnToDrop > permutations[bestPermutation].getCenter().getColumn())
    {
        cout << "right" << endl;
        bestColumnToDrop--;
    }
    cout.flush();
    
    // todo: if and when the seek is working, we'd look at the best
    // solution and determine whether we need to manually move down so we can
    // then move right or left to occupy a currently obstructed cell
    /* if (best._numLeftShifts != 0
        || best._numRightShifts != 0)
    {
        for(int i = 0; i < best._numberDropRows; i++)
        {
            cout << "down" << endl;
        }
        cout.flush();
        for(int i = 0; i < best._numLeftShifts; i++)
        {
            cout << "left" << endl;
        }
        cout.flush();
        for(int i = 0; i < best._numRightShifts; i++)
        {
            cout << "right" << endl;
        }
        cout.flush();
    } */
    
    return 0;
}

void findBestPositionForPermutation(const CBoardSurface& surface, const CBlock& permutation)
{
    for (int colOrderIdx = 0; colOrderIdx < BOARD_WIDTH; colOrderIdx++)
    {
        int colIdx = colOrder[colOrderIdx];
        CBlock tmp = permutation.moveTo(permutation.getCenter().getRow(), colIdx);
        if (surface.check(tmp))
        {
            int numDown = 0;
            float score = 0.0;
            CBoardSurface newSurface = surface.drop(tmp, numDown, score);
            
            // track this solution in the map
            CSolution s;
            s._startColumn = colIdx;
            s._nbrDownCommands = numDown;
            s._numberOfRotations = permutation.getNumRotations();
            s._score = score;
            solutionMap.insert(pair<float, CSolution>(score, s));
            
            // is the best we've found so far?
            if (bestPermutation == -1
                || score < bestScore)
            {
                newSurface.render(); // debug
                
                // remember the details for this solution
                bestPermutation = permutation.getNumRotations();
                bestColumnToDrop = colIdx;
                bestScore = score;
                
                seekBetterSolution(surface, tmp, numDown);
            }
            
            // todo: take this solution as a base and apply the next piece in the queue
            // since it's possible that a higher scoring move for this piece will result
            // in a better overall score when the next piece is manipulated and dropped
        }
    }
}

void seekBetterSolution(const CBoardSurface& surface, const CBlock& permutation, int numDown)
{
    // ideally, we'd take the current solution and do some manual shifting right/left to
    // see whether we can occupy some obstructed cells and improve the score, but running out of
    // time and not hitting many live play scenarios that trigger this code -- keeping the code
    // so I can keep playing with it, but it's not doing much
    int numLeft;
    int numRight;
    float newScore = bestScore;
    surface.seekBetterSolution(permutation, numDown, numLeft, numRight, newScore);
    
    if (numLeft != 0
        || numRight != 0)
    {
        CSolution alternate;
        alternate._startColumn = permutation.getCenter().getColumn();
        alternate._nbrDownCommands = numDown;
        alternate._numberOfRotations = permutation.getNumRotations();
        alternate._score = newScore;
        alternate._nbrLeftCommands = numLeft;
        alternate._nbrRightCommands = numRight;
        solutionMap.insert(pair<float, CSolution>(newScore, alternate));
    }
}
