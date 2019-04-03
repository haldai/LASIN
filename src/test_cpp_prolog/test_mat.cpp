#include <iostream>
#include <boost/numeric/ublas/matrix.hpp>

#include "test_mat.h"
#include "utils.h"

using namespace boost::numeric::ublas;
using namespace std;

void mat_init(matrix<double> *first, matrix<double> *second) {
    cout << "Please input the first matrix:" << endl;
    for (unsigned i = 0; i < (*first).size1(); i++)
        for (unsigned j = 0; j < (*first).size2(); j++)
            cin >> (*first)(i, j);

    cout << "Please input the second matrix:" << endl;
    for (unsigned i = 0; i < (*second).size1(); i++)
        for (unsigned j = 0; j < (*second).size2(); j++)
            cin >> (*second)(i, j);
}

matrix<double> *mat_prod(matrix<double> *first, matrix<double> *second) {
    matrix<double> prd = prod(*first, *second);
    matrix<double> *re = new matrix<double>(prd);
    return re;
}

matrix<double> *mat_reload(string addr) {
    stringstream ss;
    memAdd uAddr;
    
    ss << addr;
    ss >> uAddr;
    cout << "Rld addr (sub):\t" << uAddr << endl;

    matrix<double> *p = (matrix<double> *) uAddr;
    return p;
}
