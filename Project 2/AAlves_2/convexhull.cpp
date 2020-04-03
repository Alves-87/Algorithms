// You need to complete this program for your second project.

/*
    Alexis Alves
    Algorithms Project 2
    4147004
*/

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h> 
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <set>
#include <stack>

using namespace std;

struct Point{
    int x;
    int y;
    float z;
    //bool operator<(const Point& rhs) const { z < rhs.z; }
};

//Global variables
int count = 0;
Point p0;

//Set and vector used for Quickhull implentation
#define iPair std::pair<int, int>
std::set<iPair> quick_hull;
std::vector<Point> QuickHull_Vec;


std::vector<Point> jarvis_quick;

//
// Following are Utility Function used Jarvis March and Graham Scan implemntation
//
// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are colinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int orientation(Point p, Point q, Point r) 
{ 
    int val = (q.y - p.y) * (r.x - q.x) - 
              (q.x - p.x) * (r.y - q.y); 
  
    if (val == 0) return 0;  // colinear 
    return (val > 0)? 1: 2; // clock or counterclock wise 
} 

bool acom(const Point &a, const Point &b)
{
    return a.z < b.z;
}

vector<Point> hull(vector<Point> &x)
{
    float angle;
    Point p1,p2;
    p1.x = 0;
    p1.y = 0;
    int index = 0;
    Point min_x;
    min_x = x[0];
    for (int i = 0; i < x.size(); i++)
    {
        if (x[i].x < min_x.x)
        {
            min_x = x[i] ;
        }
        
    }

    p1 = min_x;

    for( int i = 0; i < x.size(); i++)
    {
        x[i].z = atan2(p1.y - x[i].y, p1.x - x[i].x);
    }
    sort(x.begin(),x.end(), acom);
    for(int i = 0 ; i < x.size() ; i++)
    {
        if(x[i].x == p1.x && x[i].y == p1.y )
            index = i;
    }

    vector<Point> tmp;

    for(int i = index; i < x.size() ; i++)
    {
        tmp.push_back(x[i]);
    }
    for(int i = 0; i < index  ; i++)
    {
        tmp.push_back(x[i]);
    }

    return tmp;
}
//
// Following are Utility Function used Graham Scan implemntation
//
// A utility function to find next to top in a stack 
Point nextToTop(stack<Point> &S) 
{ 
    Point p = S.top(); 
    S.pop(); 
    Point res = S.top(); 
    S.push(p); 
    return res; 
} 
  
// A utility function to swap two points 
int swap(Point &p1, Point &p2) 
{ 
    Point temp = p1; 
    p1 = p2; 
    p2 = temp; 
} 
  
// A utility function to return square of distance 
// between p1 and p2 
int distSq(Point p1, Point p2) 
{ 
    return (p1.x - p2.x)*(p1.x - p2.x) + 
          (p1.y - p2.y)*(p1.y - p2.y); 
} 
// A utility function to sort an array of 
// points with respect to the first point 
int compare(const void *vp1, const void *vp2) 
{ 
   Point *p1 = (Point *)vp1; 
   Point *p2 = (Point *)vp2; 
  
   // Find orientation 
   int o = orientation(p0, *p1, *p2); 
   if (o == 0) 
     return (distSq(p0, *p2) >= distSq(p0, *p1))? -1 : 1; 
  
   return (o == 2)? -1: 1; 
} 

  

//Graham Scan adapted from: https://www.geeksforgeeks.org/convex-hull-set-2-graham-scan/
// Prints convex hull of a set of n points. 
void GrahamScan(std::vector<Point> points, int n,string outputFile ) 
{ 
   // Find the bottommost point 
   int ymin = points[0].y, min = 0; 
   for (int i = 1; i < n; i++) 
   { 
     int y = points[i].y; 
  
     // Pick the bottom-most or chose the left 
     // most point in case of tie 
     if ((y < ymin) || (ymin == y && 
         points[i].x < points[min].x)) 
        ymin = points[i].y, min = i; 
   } 
  
   // Place the bottom-most point at first position 
   swap(points[0], points[min]); 
  
   // Sort n-1 points with respect to the first point. 
   // A point p1 comes before p2 in sorted output if p2 
   // has larger polar angle (in counterclockwise 
   // direction) than p1 
   p0 = points[0]; 
   qsort(&points[1], n-1, sizeof(Point), compare); 
  
   // If two or more points make same angle with p0, 
   // Remove all but the one that is farthest from p0 
   // Remember that, in above sorting, our criteria was 
   // to keep the farthest point at the end when more than 
   // one points have same angle. 
   int m = 1; // Initialize size of modified array 
   for (int i=1; i<n; i++) 
   { 
       // Keep removing i while angle of i and i+1 is same 
       // with respect to p0 
       while (i < n-1 && orientation(p0, points[i],points[i+1]) == 0) 
          i++; 
  
  
       points[m] = points[i]; 
       m++;  // Update size of modified array 
   } 
  
   // If modified array of points has less than 3 points, 
   // convex hull is not possible 
   if (m < 3) return; 
  
   // Create an empty stack and push first three points 
   // to it. 
   stack<Point> S; 
   S.push(points[0]); 
   S.push(points[1]); 
   S.push(points[2]); 
  
   // Process remaining n-3 points 
   for (int i = 3; i < m; i++) 
   { 
      // Keep removing top while the angle formed by 
      // points next-to-top, top, and points[i] makes 
      // a non-left turn 
      while (orientation(nextToTop(S), S.top(), points[i]) != 2) 
         S.pop(); 
      S.push(points[i]); 
   } 
  
   // Now stack has the output points, print contents of stack 
   // Export hull to file
    std::ofstream file;
    file.open(outputFile.c_str(), std::ofstream::out | std::ofstream::trunc);
    while (!S.empty()) 
   { 
        Point p = S.top();
        file << p.x << "\t" <<p.y << "\n";
        S.pop(); 
    }
    file.close();
 
}

//

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

   
    while(!quick_hull.empty())
    {
       Point temp;
       temp.x = (*quick_hull.begin()).first;
       temp.y = (*quick_hull.begin()).second; 
       QuickHull_Vec.push_back(temp);
       quick_hull.erase(quick_hull.begin()); 
    }
     // cout <<  QuickHull_Vec.size() << endl;   

     QuickHull_Vec = hull(QuickHull_Vec);

    // Export hull to file
    std::ofstream file;
    file.open(outputFile.c_str(), std::ofstream::out | std::ofstream::trunc);
    for (int i = 0; i < QuickHull_Vec.size(); i++)
    {
        file << QuickHull_Vec[i].x << "\t" << QuickHull_Vec[i].y << "\n";
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

    //Inport Data from Respective File
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
         std::clock_t start = std::clock();
         outputFile = "hull_G.txt";
         GrahamScan(points,points.size(),outputFile);
         double duration = (std::clock() - start);
         duration = (duration/CLOCKS_PER_SEC)*1000;
         //std::cout << "Duration " << duration << " ms" << std::endl;
      } 
      else if (algType[0]=='J') {
         //call your Javis March algorithm to solve the problem
         std::clock_t start = std::clock();
         outputFile = "hull_J.txt";
         jarvisMarch(points,points.size() ,outputFile);
         double duration = (std::clock() - start);
         duration = (duration/CLOCKS_PER_SEC)*1000;
         //std::cout << "Duration " << duration << " ms" << std::endl;
      }
      else { //default 
         //call your Quickhull algorithm to solve the problem
         std::clock_t start = std::clock();
         outputFile = "hull_Q.txt";
         quickHull(quick_points,points.size(),outputFile);
         double duration = (std::clock() - start);
         duration = (duration/CLOCKS_PER_SEC)*1000;
         //std::cout << "Duration " << duration << " ms" << std::endl;
      }
      
      //write your convex hull to the outputFile (see class example for the format)
      //you should be able to visulize your convex hull using the "ConvexHull_GUI" program.
	}
	return 0;
}