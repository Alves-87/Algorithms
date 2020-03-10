// You need to complete this program for your second project.

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h> 

//Libraries specific to this project
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
#include <set>
#include <stack>

using namespace std;

struct Point{
    int x, y;
};

//Global variables
int count = 0;
Point p0;

//Specifics for Quick Hull
#define iPair std::pair<int, int>
std::set<iPair> quick_hull;
std::vector<Point> quick_hull_vector;
//Used for QuickHull
std::vector<Point> jarvis_quick;

//Jarvis March adapted from: https://www.geeksforgeeks.org/convex-hull-set-1-jarviss-algorithm-or-wrapping/
// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are colinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int orientation(Point p, Point q, Point r) 
{ 
	int val = (q.y - p.y) * (r.x - q.x) - 
			(q.x - p.x) * (r.y - q.y); 

	if (val == 0) return 0; // colinear 
	return (val > 0)? 1: 2; // clock or counterclock wise 
} 
//Jarvis March adapted from: https://www.geeksforgeeks.org/convex-hull-set-1-jarviss-algorithm-or-wrapping/
void jarvisMarch(vector<Point> points, int n, string outputFile) 
{ 
	// There must be at least 3 points 
	if (n < 3) return; 

	// Initialize Result 
	vector<Point> hull; 

	// Find the leftmost point 
	int l = 0; 
	for (int i = 1; i < n; i++) 
		if (points[i].x < points[l].x) 
			l = i; 

	// Start from leftmost point, keep moving counterclockwise 
	// until reach the start point again. This loop runs O(h) 
	// times where h is number of points in result or output. 
	int p = l, q; 
	do
	{ 
		// Add current point to result 
		hull.push_back(points[p]); 

		// Search for a point 'q' such that orientation(p, x, 
		// q) is counterclockwise for all points 'x'. The idea 
		// is to keep track of last visited most counterclock- 
		// wise point in q. If any point 'i' is more counterclock- 
		// wise than q, then update q. 
		q = (p+1)%n; 
		for (int i = 0; i < n; i++) 
		{ 
		// If i is more counterclockwise than current q, then 
		// update q 
		if (orientation(points[p], points[i], points[q]) == 2) 
			q = i; 
		} 

		// Now q is the most counterclockwise with respect to p 
		// Set p as q for next iteration, so that q is added to 
		// result 'hull' 
		p = q; 

	} while (p != l); // While we don't come to first point 

	// // Print Result 
	// for (int i = 0; i < hull.size(); i++) 
	// 	cout << "(" << hull[i].x << ", "
	// 		<< hull[i].y << ")\n"; 

   //Export hull to file
    std::ofstream file;
    file.open(outputFile.c_str(), std::ofstream::out | std::ofstream::trunc);
    for (int i = 0; i < hull.size(); i++)
    {
        file << hull[i].x << "\t" << hull[i].y << "\n";
    }
    file.close();
} 


// Returns side of a point that joins p1 and p2, from Quick Hull adaptation
int findSide(iPair p1, iPair p2, iPair p)
{
    int val = (p.second - p1.second) * (p2.first - p1.first) - (p2.second - p1.second) * (p.first - p1.first);
    if (val > 0)
        return 1;
    if (val < 0)
        return -1;
    return 0;
}

// Return value proportional to distance between p and the line
// connecting p1 and p2, from Quick Hull adaptation
int lineDist(iPair p1, iPair p2, iPair p)
{
    return abs((p.second - p1.second) * (p2.first - p1.first) - (p2.second - p1.second) * (p.first - p1.first));
}

//Quick Hull adapted from: https://www.geeksforgeeks.org/quickhull-algorithm-convex-hull/
// Recursive helper for Quickhull function
void hullHelper(std::vector<iPair> points, int n, iPair p1, iPair p2, int side)
{
    int ind = -1;
    int max_dist = 0;

    // finding the point with maximum distance
    // from L and also on the specified side of L.
    for (int i = 0; i < n; i++)
    {
        int temp = lineDist(p1, p2, points[i]);
        if (findSide(p1, p2, points[i]) == side && temp > max_dist)
        {
            ind = i;
            max_dist = temp;
        }
    }

    // If no point is found, add the end points
    // of L to the convex hull.
    if (ind == -1)
    {
        quick_hull.insert(p1);
        quick_hull.insert(p2);
        return;
    }

    // Recurse twice with smaller sets of points
    // Recurse for the two parts divided by a[ind]
    hullHelper(points, n, points[ind], p1, -findSide(points[ind], p1, p2));
    hullHelper(points, n, points[ind], p2, -findSide(points[ind], p2, p1));
}

// Find a convex hull using Quickhull
void quickHull(std::vector<iPair> points, int n, std::string outputFile)
{
    // a[i].second -> y-coordinate of the ith point
    if (n < 3)
    {
        std::cout << "Quick Hull is not possible" << std::endl;
        return;
    }

    // Find the minimum and maximum x-coordinate values
    int min_x = 0, max_x = 0;
    for (int i = 1; i < n; i++)
    {
        if (points[i].first < points[min_x].first)
            min_x = i;
        if (points[i].first > points[max_x].first)
            max_x = i;
    }

    // Recursively find convex hull points on
    // one side and other side of line joining a[min_x] and
    // a[max_x]
    hullHelper(points, n, points[min_x], points[max_x], 1);
    hullHelper(points, n, points[min_x], points[max_x], -1);

    cout <<  quick_hull_vector.size() << endl;
    while(!quick_hull.empty())
    {
       Point temp;
       temp.x = (*quick_hull.begin()).first;
       temp.y = (*quick_hull.begin()).second; 
       quick_hull_vector.push_back(temp);
       quick_hull.erase(quick_hull.begin()); 
    }


    // Export hull to file
    std::ofstream file;
    file.open(outputFile.c_str(), std::ofstream::out | std::ofstream::trunc);
    for (int i = 0; i < quick_hull_vector.size(); i++)
    {
        file << quick_hull_vector[i].x << "\t" << quick_hull_vector[i].y << "\n";
    }
    file.close();
}









int main(int argc, char *argv[])
{   
   if (argc < 3) 
      std::cout << "wrong format! should be \"a.exe algType dataFile\"";
   else {
      std::string algType = argv[1];
      std::string dataFilename = argv[2];

      std::string outputFile = "";
      //read your data points from dataFile (see class example for the format)
      std::ifstream file;
      file.open(dataFilename.c_str());
      std::vector<Point> points;
      std::vector<iPair> quick_points;
      
      if (file.is_open())
      {
          int x, y;
          while (file >> x >> y)
          {
              Point myPoint;
              myPoint.x = x;
              myPoint.y = y;
              points.push_back(myPoint);
              quick_points.push_back(*new iPair(x, y));
          }
      }

      if (algType[0]=='G') {
         //call your Graham Scan algorithm to solve the problem
         outputFile = "hull_G.txt";
      } 
      else if (algType[0]=='J') {
         //call your Javis March algorithm to solve the problem
         outputFile = "hull_J.txt";
         jarvisMarch(points,points.size() ,outputFile);
      }
      else { //default 
         //call your Quickhull algorithm to solve the problem
         outputFile = "hull_Q.txt";
         quickHull(quick_points,points.size(),outputFile);
      }
      
      //write your convex hull to the outputFile (see class example for the format)
      //you should be able to visulize your convex hull using the "ConvexHull_GUI" program.
	}
	return 0;
}