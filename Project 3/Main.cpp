/*
    Alexis Alves
    Algorithms Project 3
    4147004
*/

//Standard Libraries 
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


#include "Carving.h"

using namespace std;

int main(int argc,char *argv[])
{
    if( argc > 4)
        cout << "Invalid format" << endl;
    else
    {   

        int Width = 0, height = 0 , scaleVal = 0;
        vector<vector<int>> image;
        vector<vector<int>> energyMatrix;
        vector<vector<int>> cumulativeEnergyMatrix;

        //Argument values
        std::string filename = argv[1];
        std::string verticalRemove = argv[2];
        std::string horizontalRemove = argv[3];

        //decalre pgm header
        string pVal = "";
        int orgHeight,orgWidth;
        vector<string> comments;  
        int verticalRemoveInt = stoi(verticalRemove);
        int horzontalRemoveInt = stoi(horizontalRemove);

        cout << "Carving: \n" 
             << " From: "                      << filename           << "\n"
             << " Desired Removal Vertical: "  << verticalRemoveInt  << "\n"
             << " Desired Removal Horizontal: "<< horzontalRemoveInt << "\n\n";
       
        //Import File to vector image
        std::ifstream file;
        file.open(filename);
        if(file.is_open())
        {
            cout << "file open \n" << endl;
            string line;
            bool endofHeaderFlag = false;
            while(endofHeaderFlag == false)
            {
                file >> line;

                //input literal   
                if(pVal == "")
                {
                    pVal = line;
                    continue;
                }
                // skip comments
                if (line[0] == '#')
                {
                    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }

                if (Width == 0)
                    Width = stoi(line);
                else if (height == 0)
                    height = stoi(line);
                else 
                    scaleVal = stoi(line);

                if(scaleVal != 0)
                    endofHeaderFlag = true;
            }

            //set vec image for population
            image.resize(height);

            //Populate image vector
            int val;
            for( int i = 0; i < height ; i++)
            {   
                for( int j = 0; j < Width ; j++)
                {
                    file >> val;
                    image[i].push_back(val);
                }
            }

            //Save original size
            orgHeight = height;
            orgWidth  = Width; 

            cout << "Input File \"" << filename <<"\" Header Information: \n"
                 << "H: "      << orgHeight << "\n"
                 << "W: "      << orgWidth  << "\n"
                 << "P VAl: "  << pVal  << "\n"
                 << "Scale: "  << scaleVal<< "\n\n"; 
        }
        else
        {
            cout << "error opening file" << endl;
            return 0;
        }

        //Remove Vertical Seams
        for(int i = 0 ; i < verticalRemoveInt ; i++ )
        {
            energyMatrix = calculateEnergyMatrix(image);
            cumulativeEnergyMatrix = calculateCumulativeMatrix(energyMatrix);
            image = traceAndRemoval(cumulativeEnergyMatrix,image);
        }

        //Transpose for Horizontal
        image = transpose(image);

        //Remove Horzontal Seams
        for(int i = 0 ; i < horzontalRemoveInt ; i++ )
        {
            energyMatrix = calculateEnergyMatrix(image);
            cumulativeEnergyMatrix = calculateCumulativeMatrix(energyMatrix);
            image = traceAndRemoval(cumulativeEnergyMatrix,image);
        }

        //Transpose to normal oriantation
        image = transpose(image);

    
        cout << "Carving Complete: \n" 
             << " From: "              << filename           << "\n"
             << " Removed Vertical: "  << verticalRemoveInt  << "\n"
             << " Removed Horizontal: "<< horzontalRemoveInt << "\n"
             << " New Vertical: "      << image.size()       << "\n"
             << " New Horizontal: "    << image[0].size()    << "\n\n";
        

        // Export pgm
        string outputFilename = filename.substr(0, filename.size() - 4) + "_processed.pgm";
        std::ofstream outfile;
        outfile.open(outputFilename.c_str(), std::ofstream::out | std::ofstream::trunc);

        // export 4 initial entries
        outfile << pVal << "\n";
        outfile << "# Output after removing " << verticalRemoveInt << " vertical and " << horzontalRemoveInt << " seams from " << filename << "\n";
        outfile << image[0].size() << " " << image.size() << "\n";
        outfile << scaleVal << "\n";

        // export the matrix
        for (int i = 0; i < image.size(); i++)
        {
            for (int j = 0; j < image[0].size(); j++)
            {
                outfile << image[i][j] << " ";
            }
            outfile << "\n";
        }
        outfile.close();
     


        
    }
}
