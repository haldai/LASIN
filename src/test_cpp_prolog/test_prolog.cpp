#include <iostream>
#include <sstream>
#include <cstring>
#include <SWI-cpp.h>
#include <SWI-Prolog.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "test_mat.h"
#include "../core/memread.hpp"

using namespace std;
using namespace boost::numeric::ublas;

PREDICATE(hello, 2) {
    char *name = (char *) A1;
    cout << "Hello " << name << endl;
    // use ostring to include all spaces
    ostringstream a2;
    a2 << "Hello " << name;
    // use string's .c_str() to transform sstream to char* for PlString return
    const char *a2_s = a2.str().c_str();
    return A2 = (char *) a2_s;
}

PREDICATE(make_mat, 1) {
    unsigned int m, n;
    cout << "Please input the size (m by n) of this matrix:" << endl;
    cin >> m >> n;
    matrix<double> *mat = new matrix<double>(m, n);
    cout << "Please input the matrix:" << endl;
    for (unsigned i = 0; i < m; i++)
        for (unsigned j = 0; j < n; j++)
            cin >> (*mat)(i, j);
    // create output ref
    term_t a1 = PL_new_term_ref();
    cout << mat << endl;
    ostringstream oss;
    oss << mat;
    const char* a1_s = oss.str().c_str();
    if (PL_put_atom_chars(a1, a1_s))
        return A1 = PlTerm(a1);
    //if (PL_put_atom_charsu(a1, mat))
    //    return A1 = PlTerm(a1);
    else
	PL_fail;
}

PREDICATE(mat_prod, 3) {
    term_t t1 = A1.ref;
    term_t t2 = A2.ref;
    char *p1, *p2;
    cout << "get args:" << PL_get_atom_chars(t1, &p1) << "," <<  PL_get_atom_chars(t2, &p2) << endl;
    cout << "prod args: " << p1 << " & " << p2 << endl;
    
    //matrix<double> *m1 = (matrix<double> *) ((void *) A1);
    //matrix<double> *m2 = (matrix<double> *) ((void *) A2);

    matrix<double> *m1 = mat_reload(string (p1));
    matrix<double> *m2 = mat_reload(string (p2));
    
    cout << "first: " << *m1 << endl;
    cout << "second: " << *m2 << endl;

    if (m1->size2() != m2->size1()) {
	cout << "Size don't match!" << endl;
	return FALSE;
    } else {
	matrix<double> *prd = new matrix<double>(m1->size1(), m2->size2());
	*prd = prod(*m1, *m2);
	cout << "product: " << *prd << endl;
	ostringstream oss;
	oss << prd;
	const char *a3_s = oss.str().c_str();
	term_t a3_t = PL_new_term_ref();

	if (PL_put_atom_chars(a3_t, a3_s)) {
	    cout << a3_s << endl;
	    return A3 = PlTerm(a3_t); // do not use "new" operator
	}
	else
	    return FALSE;
    }
}

PREDICATE(mat_valid, 1) {
    char *a1_s;
    //void *a1_s;
    term_t t1 = A1.ref;
    if (PL_get_atom_chars(t1, &a1_s)) {
    //if (PL_get_pointer(t1, &a1_s)) {

    	//string addr(a1_s);
	//cout << "arg: " << addr << endl;
	cout << "arg: " << a1_s << endl;
	matrix<double> *mat = mat_reload((string) a1_s);
	//matrix<double> *mat = (matrix<double> *) a1_s;
    
	//matrix<double> *mat = (matrix<double> *) ((void *) A1);
	cout << *mat << endl;
	/*
	  if ((int) (*mat)(0, 0) % 3 == 2)
	  return TRUE;
	  else 
	  return FALSE;
	*/
	return TRUE;
    } else
	return FALSE;
}

// test for make a list, and unify with a given variable/list
PREDICATE(make_list, 2) {
    term_t t1 = A1.ref;
    char *a1_s;
    if (PL_get_atom_chars(t1, &a1_s)) {
        std::vector<double> *l = str2ptr<std::vector<double>>(a1_s);
        /*
        for (size_t i = 0; i < (*l).size(); ++i)
            cout << (*l)[i] << " ";
        cout << endl;
        */
        
        // make a list
        
        // ******list reference is a PlTerm******
        term_t tt = PL_new_term_ref();
        PlTerm av(tt);
        PlTail ll(av);
        
        for(size_t i = 0; i < (*l).size(); ++i) {
            //term_t t = PL_new_term_ref();
            //PL_put_integer(t, (long) (*l)[i]);
            //list.append(PlTerm(t));
            cout << (double) (*l)[i] << " ";
            ll.append((*l)[i]);
        }
        ll.close();
        return A2 = av;
    } else
        return FALSE;
}
