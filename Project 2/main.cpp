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
void jarvisMarch(Point points[], int n) 
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

     // Export hull to file
    // std::ofstream file;
    // file.open(outputFile.c_str(), std::ofstream::out | std::ofstream::trunc);
    // for (int i = 0; i < jarvis_quick.size(); i++)
    // {
    //     file << jarvis_quick[i].x << "\t" << jarvis_quick[i].y << "\n";
    // }
    // file.close();
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
      }
      else { //default 
         //call your Quickhull algorithm to solve the problem
         outputFile = "hull_Q.txt";
      }
      
      //write your convex hull to the outputFile (see class example for the format)
      //you should be able to visulize your convex hull using the "ConvexHull_GUI" program.
	}
	return 0;
}