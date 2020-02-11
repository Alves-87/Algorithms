// You need to complete this program for a part of your first project.

// Standard libraries
#include <string>
#include <iostream>
#include <ctime>
#include <fstream>
#include <stdlib.h> 

// 'BigIntegerLibrary.hh' includes all of the library headers.
#include "BigIntegerLibrary.hh"


BigUnsigned RandomBig(int p){
   int y = rand()%10;
   int i = 0;
   BigUnsigned x = BigUnsigned(y);
   while( i < p)
   {
      i++;
      x = x * 10 + ( rand() % 10);
   }
   return x;
}
bool FermatTest(BigUnsigned x){
   BigUnsigned a1 = BigUnsigned(1);
   BigUnsigned a2 = BigUnsigned(2);
   if( ( modexp(a1, x -1, x) == 1 ) && ( modexp(a2, x -1, x) == 1 ) )
      return 1;
   else 
      return 0;

}
BigUnsigned GetPrime(int p){
   BigUnsigned x = BigUnsigned(1);
   x = RandomBig(p);
   bool PrimeFlag = false;
   PrimeFlag = FermatTest(x); 
   while(!PrimeFlag)
   {
      x += 1;
      PrimeFlag = FermatTest(x);
   }
   return x;
}
bool IsRelPrime(BigUnsigned x, BigUnsigned y){
   return gcd(x,y) == 1;
}
BigUnsigned Find_e(BigUnsigned x,BigUnsigned y){
   bool RelativePrimeFlag = false;
   x = RandomBig(101);
   RelativePrimeFlag = IsRelPrime(x,y);
   while(!RelativePrimeFlag)
   {
      x = RandomBig(101);
      RelativePrimeFlag = IsRelPrime(x,y);
   }  
   return x;
}
BigUnsigned Find_Phi(BigUnsigned x, BigUnsigned y){
   BigUnsigned phi(1);
   phi =(x - 1) * (y - 1);
   if (phi % 2 == 0)
      phi++;
   return phi;
}
BigUnsigned Find_d(BigUnsigned x, BigUnsigned y){
   return modinv(x, y);
}
BigUnsigned Find_n(BigUnsigned x, BigUnsigned y){
   return x * y;
}
void PrintToFile(BigUnsigned x, BigUnsigned y, std::string name){
   std::cout << "Outputting to file " << name << std::endl;
   std::ofstream File(name);
   File << x << "\n" << y;
   File.close();
}
void LineBoy(int x){
   while( x != 0)
   {
      std::cout << "------------------------------------------------------------------------"
               << std::endl;
      x--;
   }
}

int main(){
	/* The library throws 'const char *' error messages when things go
	 * wrong.  It's a good idea to catch them using a 'try' block like this
	 * one.  Your C++ compiler might need a command-line option to compile
	 * code that uses exceptions. */
	try {
		int PrimeLength = 155;
      BigUnsigned p, q, n, e, d, phi;
      srand(time(NULL));

      //Generate Two Prime of 512bytes
      std::cout << "Generating two Prime numbers...\n";
      p = GetPrime(PrimeLength);
      q = GetPrime(PrimeLength);
      std::cout << "The two Prime numbers are:\n";
      LineBoy(2);
      std::cout << "p: " << p << std::endl;
      LineBoy(1);
      std::cout << "q: " << q << std::endl;
      //Save p and q in a file named p_q.txt, one integer per line and making sure no white space saved;
      PrintToFile(p,q,"p_q.txt");
      LineBoy(2);
      //Compute the value for n 
      n = Find_n(p,q);
      std::cout << "n: " << n << std::endl;
      LineBoy(1);
      //Compute the value for phi(n)
      phi = Find_Phi(p,q);
      //Find the value for e (secret key)
      e = Find_e(e,phi);
      std::cout << "e: " << e << std::endl;
      LineBoy(1);
      //Find the value for d (public key)
      d = Find_d(e,phi);
      std::cout << "d: " << d << std::endl;
      LineBoy(1);

      //Save the two pairs of keys in two separate files: e_n.txt and d_n.txt, one integer per line and no white space
      PrintToFile(e,n,"e_n.txt");
      PrintToFile(d,n,"d_n.txt");
      LineBoy(2);

      std::cout << "RSA Key Generation Complete" << std::endl;
     
	} catch(char const* err) {
		std::cout << "The library threw an exception:\n"
			<< err << std::endl;
	}

	return 0;
}



