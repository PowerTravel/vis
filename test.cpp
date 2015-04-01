#include <iostream>
#include <vector>
#include "external/eigen-3.2.4/Eigen/Dense"
#include "external/eigen-3.2.4/Eigen/SparseCore"
#include <cstdarg>
void foo(double* d, int a){
	for(int i = 0; i<a; i++ )
	{
		std::cout << d[i]<< " "; 
	}
	std::cout << std::endl;
};

void bar(double* d, int a){
	for(int i = 0; i<a; i++ )
	{
		d[i] = i; 
	}
	std::cout << std::endl;
};

int main(int argc, char* argv[])
{
/*
	std::vector<Eigen::Triplet<double> > v = std::vector<Eigen::Triplet<double> >();
	for(int i =0; i<1000; i=i+10){
		v.push_back( Eigen::Triplet<double>(i,i,i) );
	}

	Eigen::SparseMatrix<double> m = Eigen::SparseMatrix<double>(1000,1000);
	m.setFromTriplets(v.begin(), v.end());


	m=m*m;
	int i = 0;
	for(int k=0; k<m.outerSize(); ++k)
	{
		for(Eigen::SparseMatrix<double>::InnerIterator it(m,k); it; ++it){
			i++;
			std::cout << "("<< it.row() <<","; // Row index 
			std::cout << it.col() << ")\t"<< k << ","<< i  << "\t" << std::endl;
		}
		i=0;
	}
	
*/
	Eigen::VectorXd v1 =  Eigen::ArrayXd::Constant(5,2);
	Eigen::MatrixXd m1 = 2*Eigen::MatrixXd::Identity(5,5);
	std::cout << v1.transpose()*m1*v1 << std::endl;



	return 0;
};
