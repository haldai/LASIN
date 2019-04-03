#include <iostream>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "test_mat.h"
#include "utils.h"

using namespace boost::numeric::ublas;
using namespace std;

int main(){
    unsigned int f_s1, f_s2, s_s1, s_s2;
    cout << "Please input the size (m by n) of 1st matrix:" << endl;
    cin >> f_s1 >> f_s2;
    cout << "Please input the size (m by n) of 2nd matrix:" << endl;
    cin >> s_s1 >> s_s2;
    
    if (f_s2 != s_s1)
	cout << "Matrices sizes do not match!" << endl;
    else {
	matrix<double> first(f_s1, f_s2), second(s_s1, s_s2);
	mat_init(&first, &second);
	matrix<double> *prd = mat_prod(&first, &second);
	cout << "The product is: " << endl;
	cout << *prd << endl;
	
	cout << "Pointer Reload Test:" << endl;
	cout << "Orgn mat:\t" << prd << endl;
	cout << "Orgn addr:\t" << &prd << endl;
	stringstream ss;
	ss << prd;
	string addr;
	ss >> addr;

	cout << "Str addr:\t" << addr << endl;
	/*
	// clear stream and its buff
	ss.clear();
	ss.str("");
    
	// reload memory address from a string through stream
	memAdd uAddr;
    
	ss << addr;
	ss >> uAddr;
	cout << "Rld addr:\t" << uAddr << endl;
    
	// use this address to reload the original data
	matrix<double>* p = (matrix<double>*) uAddr;
	cout << "Rld mat:\t" << *p << endl;
	*/
	cout << "Rld mat:\t" << mat_reload(addr) << endl;
    }
    return 0;
}
