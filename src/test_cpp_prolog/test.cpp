#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
#include <SWI-cpp.h>
#include <SWI-Prolog.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "test_mat.h"
#include "../cpp/memread.hpp"

using namespace std;
using namespace boost::numeric::ublas;

int main(int argc, char *argv[]) {
    // start an prolog engine
    PlEngine PE(argv[0]);

    std::vector<double> *l = new std::vector<double>(3);
    for (size_t i = 0; i < (*l).size(); ++i) {
        (*l)[i] = (double) i*i;
    }

    // consult the file
    PlCall("consult('../prolog/test.pl').");

    string addr = ptr2str(l);

    //cout << ptr2str(l) << endl;

    term_t t = PL_new_term_ref();
    PL_put_atom_chars(t, addr.c_str());

    PlTermv addrs(1);
    addrs[0] = PlTerm(t);

    //PlQuery q("make_list", addrs);
    PlQuery q("tl", addrs);

    if(q.next_solution())
        cout << "Success!" << endl;
    else
        cout << "Failed!" << endl;
    
    delete (l);
    l = NULL;

    /*
    // init the two matrices

    unsigned int f_s1, f_s2, s_s1, s_s2;
    cout << "Please input the size (m by n) of 1st matrix:" << endl;
    cin >> f_s1 >> f_s2;
    cout << "Please input the size (m by n) of 2nd matrix:" << endl;
    cin >> s_s1 >> s_s2;
    matrix<double> *first = new matrix<double>(f_s1, f_s2);
    matrix<double> *second = new matrix<double>(s_s1, s_s2);
    mat_init(first, second);

    term_t t1 = PL_new_term_ref();
    term_t t2 = PL_new_term_ref();
    
    ostringstream oss;
    oss << first << ends;
    const char *addr1 = oss.str().c_str();
    oss.str("");
    oss.clear();
    oss << second << ends;
    const char *addr2 = oss.str().c_str();
    PL_put_atom_chars(t1, addr1);
    PL_put_atom_chars(t2, addr2);

    cout << "Address 1 & 2:" << endl;
    cout << first << " & " << second << endl;
    cout << "translated:" << endl;
    cout << addr1 << " & " << addr2 << endl;

    // pass it to query
    PlTermv addrs(2);
    //addrs[0] = new PlTerm((void *) &first);
    //addrs[1] = new PlTerm((void *) &second);
    addrs[0] = PlTerm(t1); // do not use "new" operator
    addrs[1] = PlTerm(t2);

    PlQuery q_valid("valid", addrs);
    if (q_valid.next_solution())
	cout << "valid product!" << endl;
    else
	cout << "NOT valid!" << endl;
    // call
    */
    return 0;
}


