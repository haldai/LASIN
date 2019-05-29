#ifndef _CORE_UTILS_HPP_
#define _CORE_UTILS_HPP_

#include <SWI-cpp.h>
#include <SWI-Prolog.h>
#include <armadillo>

#include <climits>

using namespace std;
using namespace arma;

/************** declaration ***************/

/* list2vec
 * translate a PlTerm (list) into vector
 * @Type: c-type of terms in the list (int, long, double, wchar*, char*),
 *   CANNOT handle string
 * @PlTerm: input term
 * @size: number of elements in list, -1 means all elements
 */
template <class Type>
vector<Type> list2vec(PlTerm term, int size = -1);
template <class Type>
PlTerm vec2list(vector<Type> list, int size = -1);
template <class Type>
vector<vector<Type>> list2vecvec(PlTerm term, int size_outer = -1,
                                 int size_inner = -1);
template <class Type>
PlTerm vecvec2list(vector<vector<Type>> vec, int size_outer = -1,
                                 int size_inner = -1);

/* Return an empty list */
PlTerm empty_list();

/* Parse PlTail selector to vector of indices */
vector<uword> parse_selector(PlTerm selectors);

/* Assert and retract a fact (PlCompoud as a PlTerm) in Prolog Engine */
void pl_assert(string type, PlTerm A);
void pl_retract(string type, PlTerm A);

/* replace all appearance in string*/
void str_replace_all(string &s1, const string &s2, const string &s3);

/* compute error rate of predictors */
template <class T = size_t>
double error_rate(arma::Row<T> labels, arma::Row<T> predictions);

/* compute auc-roc */
double calcAUC(double* labels, double * scores, int n, int posclass);

/************ implementation ************/

template <class Type>
vector<Type> list2vec(PlTerm term, int size) {
    static_assert((is_same<Type, int>::value)
                  || (is_same<Type, long>::value)
                  || (is_same<Type, double>::value)
                  || (is_same<Type, wchar_t*>::value)
                  || (is_same<Type, char*>::value),
                  "Wrong template type for list2vec!");
    assert(term.type() != PL_VARIABLE);
    vector<Type> re;
    
    try {
        PlTail tail(term);
        PlTerm e;
        if (size == -1) {
            while(tail.next(e))
                re.push_back((Type) e);
        } else {
            for (int i = 0; i < size; i++) {
                tail.next(e);
                re.push_back((Type) e);
            }
        }
        return re;
    } catch (...) {
        cerr << "Parsing list2vec error!" << endl;
        return re;
    }
}

template <class Type>
PlTerm vec2list(vector<Type> list, int size) {
    static_assert((is_same<Type, long>::value)
                  || (is_same<Type, double>::value)
                  || (is_same<Type, wchar_t*>::value)
                  || (is_same<Type, char*>::value)
                  || (is_same<Type, PlTerm>::value),
                  "Wrong template type for list2vec!");
        
    term_t term_ref = PL_new_term_ref();
    PlTerm term(term_ref);
    PlTail tail(term);

    try {
        if (size == -1) {
            for (auto it = list.begin(); it != list.end(); ++it)
                tail.append((Type) *it);
            tail.close();
        } else {
            for (int i = 0; i < size; i++)
                tail.append((Type) list[i]);
            tail.close();
        }
        return term;
    } catch (...) {
        cerr << "Saving list error!" << endl;
        return term;
    }
}

template <class Type>
vector<vector<Type>> list2vecvec(PlTerm term, int size_outer, int size_inner) {
    static_assert((is_same<Type, int>::value)
                  || (is_same<Type, long>::value)
                  || (is_same<Type, double>::value)
                  || (is_same<Type, wchar_t*>::value)
                  || (is_same<Type, char*>::value),
                  "Wrong template type for list2vec!");
    assert(term.type() != PL_VARIABLE);

    vector<vector<Type>> re;
    try {
        PlTail tail(term);
        PlTerm e;
        if (size_outer == -1) {
            while(tail.next(e)) {
                vector<Type> p = list2vec<Type>(e, size_inner);
                re.push_back(p);
            }
        } else {
            for (int i = 0; i < size_outer; i++) {
                tail.next(e);
                vector<Type> p = list2vec<Type>(e, size_inner);
                re.push_back(p);
            }
        }
    } catch (...) {
        cerr << "Parsing list2vecvec error!" << endl;
    }
    return re;
}

template <class Type>
PlTerm vecvec2list(vector<vector<Type>> vec, int size_outer, int size_inner) {
    static_assert((is_same<Type, long>::value)
                  || (is_same<Type, double>::value)
                  || (is_same<Type, wchar_t*>::value)
                  || (is_same<Type, char*>::value)
                  || (is_same<Type, PlTerm>::value),
                  "Wrong template type for list2vecvec!");
    term_t re_ref = PL_new_term_ref();
    PlTerm re_term(re_ref);
    PlTail re_tail(re_term);

    try {
        size_outer = (size_outer < 0) ? vec.size() : size_outer;
        for (int o_idx = 0; o_idx < size_outer; o_idx++) {
            size_inner = (size_inner < 0) ? vec[o_idx].size() : size_inner;
            term_t points_ref = PL_new_term_ref();
            PlTerm points_term(points_ref);
            PlTail points_tail(points_term);
            for (int i_idx = 0; i_idx < size_inner; i_idx++) {
                PlTerm point((Type) (vec[o_idx][i_idx]));
                points_tail.append(point);
            }
            points_tail.close();
            re_tail.append(points_term);
        }
        re_tail.close();
    } catch (...) {
        cerr << "Parsing list error!" << endl;
    }
    return re_term;
}

PlTerm empty_list() {
    PlTerm re;
    PlTail t(re);
    t.close();
    return re;
}

vector<uword> parse_selector(PlTerm selectors) {
    
    PlTail sel(selectors);
    PlTerm e;
    vector<uword> re;
    while(sel.next(e)) {
        term_t term_ref = e.ref;
        long val;
        if (PL_get_long(term_ref, &val)) {
            assert(val >= 0);
            re.push_back((uword) val);
        }
        else {
            cerr << "Reading selector error!" << endl;
            exit(0);
        }
    }
    return re;
}

void str_replace_all(string &s1, const string &s2, const string &s3) {
    string::size_type pos = 0;
    string::size_type a = s2.size();
    string::size_type b = s3.size();
	while((pos = s1.find(s2, pos)) != string::npos) {
		s1.replace(pos, a, s3);
		pos += b;
	}
}

template <class T>
double error_rate(arma::Row<T> labels, arma::Row<T> predictions) {
    size_t correct = 0;
    for (size_t i = 0; i < labels.n_elem; ++i)
        if (labels[i] == predictions[i])
            ++correct;
    return 1 - (double(correct) / double(labels.n_elem));
}

template <class T>
double aucroc(arma::Row<T> labels, arma::Row<T> predictions) {
    // convert to double
    int n = labels.n_elem;
    double l[n];
    double p[n];

    for (size_t i = 0; i < labels.n_elem; ++i) {
        l[i] = labels[i];
        p[i] = predictions[i];
    }

    return calcAUC(l, p, n, 1);
}

/* @TODO::use F1-score to evaluate
template <class T>
double f1score(arma::Row<T> labels, arma::Row<T> predictions) {
    size_t correct = 0;
    for (size_t i = 0; i < labels.n_elem; ++i)
        if (labels[i] == predictions[i])
            ++correct;
    return 1 - (double(correct) / double(labels.n_elem));
}
*/

/* Caculate the trapezoidal area bound by the quad (X1,X2,Y1,Y2) */
double trapezoidArea(double X1, double X2, double Y1, double Y2) {
	double base = std::abs(X1-X2);
	double height = (Y1 + Y2) / 2.0;
	return (base * height);
}

/* 
 * Initialize a pair list containing score/label pairs that need to be sorted to calculate the AUC 
 * Vector automatically sorts pair.first first and that is the desired behaviour.    
 *      scores      array of scores for each instance
 *      labels      array of labels for each instance
 *      n           amount of input instances
 *      posclass    label for the positive class
 *
 *      returns     the area under the ROC curve
 */
double calcAUC(double* labels, double * scores, int n, int posclass) {
	typedef std::pair<float, int> mypair;
	std::vector<mypair> L(n);
	for(int i = 0; i < n; i++) {
		L[i].first = scores[i];
		L[i].second = labels[i];
	}
	std::sort(L.begin(), L.end());
	std::reverse(L.begin(),L.end());

  	/* Count number of positive and negative examples first */
	int N = 0, P = 0;
	for (int i = 0; i < n ; i++) {
		if(labels[i] == posclass)
            P++;
		else N++;
	}
    if (N == 0 || P == 0)
        cerr << "I only found class 1 in the labels vector ...\n";

    /* Then calculate the actual are under the ROC curve */
	double A = 0;
	double fprev = INT_MIN; //-infinity
	unsigned long long FP = 0, TP = 0, FPprev = 0, TPprev = 0;
	for(int i = 0 ; i < n; i++) {
		double fi = L[i].first;
		double label= L[i].second;		
		if(fi != fprev) {
            /* Divide area here already : a bit slower, but gains in precision and avoids overflows */
			A = A + (trapezoidArea(FP*1.0/N, FPprev*1.0/N, TP*1.0/P, TPprev*1.0/P));
			fprev = fi;
			FPprev = FP;
			TPprev = TP;
		}
		if(label == posclass)
			TP = TP + 1;
		else
			FP = FP + 1;
	}
    /* Not the same as Fawcett's original pseudocode though I think his contains a typo */
	A = A + trapezoidArea(1.0, FPprev*1.0/N, 1.0, TPprev*1.0/P); 
	return A;
}


void pl_assert(string type, PlTerm A) {
    PlTermv pl_args(1);
    pl_args[0] = PlCompound(type.c_str(), A);
    PlQuery q("assert", pl_args);
    q.next_solution();
}

void pl_retract(string type, PlTerm A) {
    PlTermv pl_args(1);
    pl_args[0] = PlCompound(type.c_str(), A);
    PlQuery q("retract", pl_args);
    q.next_solution();
}


#endif
