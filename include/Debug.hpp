#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <string>
#include <iostream>
#include <fstream>
void printToFile(const char* fileName,  const double* x ){

	std::ofstream of;
	if(x != NULL)
	{
		of.open(fileName, std::ios::out |std::ios::app);
		of << x[0]<< " " << x[1] << " " << x[2] << std::endl;
		of.close();
	}else{
		of.open(fileName, std::ios::out |std::ios::trunc);
		//of << std::endl;
		of.close();
	}
}

#endif
