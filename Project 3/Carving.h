#ifndef Carving_h_
#define Carving_h_

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h>

// Libraries for this project
#include <fstream>
#include <limits>
#include <sstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <ctime>
#include <bits/stdc++.h>

using namespace std;

//Transpose Matrix
vector<vector<int>> transpose(vector<vector<int>> &x)
{
    int W = x[0].size();
    int H = x.size();
    vector<vector<int>> transposed(W, vector<int>(H));
  
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            transposed[j][i] = x[i][j];

    return transposed;
}

//Print Matrix for testing purpose
void printMatrix(vector<vector<int>> &x)
{
    for( int i = 0; i < x.size() ; i++)
    {   
        for( int j = 0; j < x[0].size() ; j++)
        {
            cout << x[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

//Calculate Energy Matrix
vector<vector<int>> calculateEnergyMatrix(vector<vector<int>> &x)
{
    int L,R,U,D;
    int H = x.size(), W = x[0].size();
    //cout << H << W << endl;
    vector<vector<int>> energymatrix;
    energymatrix.resize(H);
    //printMatrix(x);
    for( int i = 0; i < H ; i++)
    {   
        for( int j = 0; j < W ; j++)
        {
            //handling corners

            i == 0 ? U = 0 : U = abs(x[i][j] - x[i - 1][j]);
            i == H - 1 ? D = 0 : D = abs(x[i][j] - x[i + 1][j]);
            j == 0 ? L = 0 : L = abs(x[i][j] - x[i][j - 1]);
            j == W - 1 ? R = 0 : R = abs(x[i][j] - x[i][j + 1]);
            
            // cout << "(" << i << ","<< j << ") \n"
            //      << "U: " << U << "\n"
            //      << "D: " << D << "\n"
            //      << "R: " << R << "\n"
            //      << "L: " << L << "\n\n";
            energymatrix[i].push_back(U + D + L + R); 
        }
    }
    
  return energymatrix;
}

//Find min of Three values
int smallest(int x, int y, int z){
    return std::min(std::min(x, y), z);
}

// Returns min value form above
int minFromAbove(vector<vector<int>> &cumulative, int i, int j)
{
    int minum = INT_MAX;

    
    if(j == 0)
        minum = min(cumulative[i-1][j],cumulative[i-1][j+1]);
    //handle Right Edge
    else if(j == cumulative[i].size()-1)
        minum = min(cumulative[i-1][j],cumulative[i-1][j-1]);
    else
        minum = smallest(cumulative[i-1][j-1],cumulative[i-1][j],cumulative[i-1][j+1]);

    return minum;
}

//Calculate Cumulative Energy Matrix
vector<vector<int>> calculateCumulativeMatrix(vector<vector<int>> &x)
{
    int minimum;
    int H = x.size(), W = x[0].size();
    vector<vector<int>> cumulativeEnergyMatrix(H, vector<int>(W));
    //printMatrix(x)

    //Copy first Line 
    for( int j = 0; j < W ; j++)
        cumulativeEnergyMatrix[0][j] = x[0][j] ; 
    for (int i = 1; i < H; i++)
    {
        for (int j = 0; j < W; j++)
            cumulativeEnergyMatrix[i][j] = x[i][j] + minFromAbove(cumulativeEnergyMatrix, i, j);
    }

  return cumulativeEnergyMatrix;
}

// Returns min coordinates from above
pair<int, int> minCoodsFromAbove(vector<vector<int>> &x, int i, int j)
{
    pair<int, int> m;
    m.first = i - 1;
    m.second = j;
  
    j != 0 ? m.second = j-1 : m.second +=0;
    x[i - 1][j] < x[m.first][m.second] ? m.second = j : m.second +=0;
    j != x[i].size() - 1 ? (x[i - 1][j + 1] < x[m.first][m.second] ? m.second = j + 1 : m.second +=0) : m.second +=0;

    return m;
}

//Traces and removes seams
vector<vector<int>> traceAndRemoval(vector<vector<int>> &x,vector<vector<int>> &y)
{
    vector<pair<int, int>> minPath;
    pair<int, int> start ((x.size() - 1),0);

    //Start of Path
    for (int j = 1; j < x[(x.size() - 1)].size(); j++)
        if (x[(x.size() - 1)][j] < x[(x.size() - 1)][start.second])
            start.second = j;

    //Find smallest path
    minPath.push_back(start);
    for (int i = x.size() - 1; i > 0; i--)
        minPath.push_back(minCoodsFromAbove(x, minPath.back().first, minPath.back().second));
    
    //Follow minpath and remove seam
    for (int i = 0; i < minPath.size(); i++)
        y[minPath[i].first].erase(y[minPath[i].first].begin() + minPath[i].second);

    return y;
}



#endif /*Carving_h_*/
