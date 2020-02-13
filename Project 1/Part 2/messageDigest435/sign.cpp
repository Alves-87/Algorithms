/*
   Alexis Alves
   Algorithms Project 1 Part 3
   Student ID: 4147004
*/

// /***
//    prepared for CS435 Project 1 part 2
// **/

#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include "sha256.h"
#include "BigIntegerLibrary.hh"

std::string convertToString(char* a) 
{ 
    std::string s(a); 
    return s; 
} 

 
int main(int argc, char *argv[])
{
   std::cout << "agrments " << argv[0][0] << " " << argv[1][0] 
             << " " << argv[2][0]  <<  " " <<  argc << std::endl;

   if (/*argc != 3 || */ (argv[1][0]!='s' && argv[1][0]!='v')) 
      std::cout << "wrong format! should be \"a.exe s filename\"";
   else {
      std::string filename = argv[2];
      
            
      //read the file
      std::streampos begin,end;
      std::ifstream myfile (filename.c_str(), std::ios::binary);
      begin = myfile.tellg();
      myfile.seekg (0, std::ios::end);
      end = myfile.tellg();
      std::streampos size = end-begin;
      //std::cout << "size of the file: " << size << " bytes.\n"; //size of the file
      
      myfile.seekg (0, std::ios::beg);
      char * memblock = new char[size];
      myfile.read (memblock, size); //read file; it's saved in the char array memblock
      myfile.close();
      
      std::string copyOFfile = filename+".Copy"; 
      std::ofstream myfile2 (copyOFfile.c_str(), std::ios::binary);
      myfile2.write (memblock, size); //write to a file
      myfile2.close();
      
      // Generate a SHA-256 hash of the contents of the file to be signed
      std::string string_Hash;

     // Turn hashed string into BigUnsigned data type
      BigUnsignedInABase hashedFileContentNum =
            BigUnsignedInABase(sha256(convertToString(memblock)), 16);
      BigUnsigned hash(hashedFileContentNum);
      
       std::cout << "Hashed contents from file.txt: " << hash << std::endl;

      //std::cout<<memblock;
        
      if (argv[1][0]=='s') {
         std::cout << "\n"<<"Need to sign the doc.\n";
         std::cout << "Signing File\n";

         //declaring string and BigUnsigned for key
         std::string string_d,string_n;
         BigUnsigned d(1),n(1);

         //Load  Key from d_n.txt 
         std::ifstream File("d_n.txt");

         while(File >> string_d >> string_n ){}
         File.close();
         
         //Convert string files to BigUnsigned
         d = stringToBigUnsigned(string_d);
         n = stringToBigUnsigned(string_n);
         std::cout << "Signing Hash\n";
         //Sign Hash
         BigUnsigned signed_Hash = modexp(hash, d, n);
         std::cout << signed_Hash << std::endl;
         //Save Signed Hash
         std::ofstream signature_File;
         signature_File.open("file.txt.signature", std::ofstream::out | std::ofstream::trunc);
         signature_File << signed_Hash;
         signature_File.close();


         std::cout << "File Signed \n";


         
      }
      else {
         std::string string_e,string_n;
         BigUnsigned e(1),n(1),signature(1),encryptedSignature(1);
        
         std::cout << "\n"<<"Need to verify the doc.\n";

         // read contents of signature file
         std::string signaturefilename = argv[3];

         std::ifstream sigfile(signaturefilename.c_str());
         std::string signatureString;
         while (sigfile >> signatureString) {}
         sigfile.close();
         
         signature = stringToBigUnsigned(signatureString);
         std::cout << "Signature from file.txt.signature: " << signature << std::endl;

         //Load  Key from e_n.txt 
         std::ifstream File("e_n.txt");

         while(File >> string_e >> string_n ){}
         File.close();

         //Convert string files to BigUnsigned
         e = stringToBigUnsigned(string_e);
         n = stringToBigUnsigned(string_n);
       

         //Encrypt signature 
         encryptedSignature = modexp(signature, e, n);
         std::cout << "Encrypted signature: " << encryptedSignature << std::endl;

         //Compare
         if (encryptedSignature == hash)
         std::cout << "File authentic " << std::endl;
         else
         std::cout << "File Modified" << std::endl;
            
      

         
      }
      delete[] memblock;
    }
    return 0;
}