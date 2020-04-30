Alexis Alves
4147004
Algorithms Project 3


Run the program: 
g++ -std=c++11 main.cpp -o project3

./project3 image_name.pgm x y 

Where X is vertical seams  and Y is horizontal seams 

Output will be

"imageName_processed.pgm"

Program overview:

	Imports a .pgm into a vector Matrix.

	Calculates energy matrix from imported matrix

	Calculates cumulative energy matrix from energy matrix

	Finds path of smallest cumulative enegry (least effect on image)

	Follows smallest cumulative energy path removing each index along the way(this step is repeated as needed)

	Export Carved Matrix to output .pgm file
  