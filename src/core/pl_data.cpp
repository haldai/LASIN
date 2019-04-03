#include "memread.hpp"
#include "data_set.hpp"
#include "data_patch.hpp"
#include "utils.hpp"
#include "errors.hpp"

#include <SWI-cpp.h>
#include <SWI-Prolog.h>
#include <mlpack/core.hpp>

#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <ctime>
#include <cmath>

/* load_data(Featre_path, Label_path, Data)
 * load a dataset into memory
 */
PREDICATE(load_data, 3) {
    char *p1 = (char*) A1;
    char *p2 = (char*) A2;
    string feature_path(p1);
    string label_path(p2);
    DataPatch *data = new DataPatch(feature_path, label_path);
    string add = ptr2str(data);
    return A3 = PlTerm(add.c_str());
}

/* load_data(Featre_path, Data)
 * load a dataset (no label) into memory
 */
PREDICATE(load_data, 2) {
    char *p1 = (char*) A1;
    string feature_path(p1);
    DataPatch *data = new DataPatch(feature_path);
    string add = ptr2str(data);
    return A2 = PlTerm(add.c_str());
}

/* print_data(Data)
 */
PREDICATE(print_data, 1) {
    char *p1 = (char*) A1;
    DataPatch *data = str2ptr<DataPatch>(p1);
    data->print_data();
    return TRUE;
}

/* print_labels(Data)
 */
PREDICATE(print_labels, 1) {
    char *p1 = (char*) A1;
    DataPatch *data = str2ptr<DataPatch>(p1);
    data->print_labels();
    return TRUE;
}

/* print_data(Data, Path)
 * save data to a csv file
 */
PREDICATE(print_data, 2) {
    char *p1 = (char*) A1;
    char *p2 = (char*) A2;
    DataPatch *data = str2ptr<DataPatch>(p1);
    data->save_data(string(p2));
    return TRUE;
}

/* print_labels(Data, Path)
 * save labels to a csv file
 */
PREDICATE(print_labels, 2) {
    char *p1 = (char*) A1;
    char *p2 = (char*) A2;
    DataPatch *data = str2ptr<DataPatch>(p1);
    data->save_labels(string(p2));
    return TRUE;
}

/* Prolog data selector
 * select_data(Data, Indices, Return)
 * select data with indices [1, 2, 5, ...]
 */
PREDICATE(select_data, 3) {
    char *p1 = (char*) A1;
    DataPatch *data = str2ptr<DataPatch>(p1);
    DataPatch *sub = new DataPatch(data->select_data(A2));
    
    // store the address of subset data in term_t
    string a3_s = ptr2str(sub);
    return A3 = PlTerm(a3_s.c_str());
    /*
    term_t a3_t = PL_new_term_ref();
    if (PL_put_atom_chars(a3_t, a3_s.c_str()))
        // return the term of the address of the subset data
        return A3 = PlTerm(a3_t);
    else
        return PUT_ERROR("filter_data", 3, "ADD2", "STRING");
    */
}

/* Prolog data selector
 * get_instance(Data, Idx, Return)
 * get an instance, return pointer of a colvec
 */
PREDICATE(get_instance, 3) {
    char *p1 = (char*) A1;
    DataPatch *data = str2ptr<DataPatch>(p1);
    int idx = (int) A2;
    vec *inst =  new vec(data->get_feature(idx));
    string a3_s = ptr2str(inst);
    return A3 = PlTerm(a3_s.c_str());
}

/* free_data(ADD)
 * free a pointer of DataPatch in memory
 */
PREDICATE(free_data, 1) {
    char *p1 = (char*) A1;
    DataPatch *ptr = str2ptr<DataPatch>(p1);
    free(ptr);
    ptr = NULL;
    return TRUE;
}

/* free_vec(ADD)
 * free a pointer of arma::vec in memory
 */
PREDICATE(free_vec, 1) {
    char *p1 = (char*) A1;
    vec *ptr = str2ptr<vec>(p1);
    free(ptr);
    ptr = NULL;
    return TRUE;
}


/* DataPatch size aquirer
 * @A1: DataPatch pointer
 * @A2: Data size
 */
PREDICATE(get_data_size, 2) {
    char *p1 = (char*) A1;
    DataPatch *data = str2ptr<DataPatch>(p1);
    size_t size = data->size();
    return A2 = PlTerm((long) size);
}

/* data_range_chk_positive(Data, Range)
 * if the Data in range contains positive instance, return true; else false;
 */
PREDICATE(data_range_chk_positive, 2) {
    term_t t1 = A1.ref;
    char *p1;
    if (PL_get_atom_chars(t1, &p1)) {
        DataPatch *data_patch = str2ptr<DataPatch>(p1);
        PlTail tail(A2);
        vector<long> range = list2vec<long>(tail);
        Row<size_t> *labels = data_patch->get_labels();
        // judge if any positive instance in range
        for (auto ptr = range.begin(); ptr != range.end(); ++ptr) 
            if (labels->at((size_t) *ptr) > 0)
                return TRUE;
        return FALSE;
    } else
        return LOAD_ERROR("data_range_chk_positive/2", 1, "DATA_ADD", "STRING");
}

/* eval_range(DataPatch, Range, Err)
 * @DataPatch: Data pointer
 * @Range: data coverage to be evaluated
 * @Err: returning error of the coverage
 */
PREDICATE(eval_range, 3) {
    term_t t1 = A1.ref;
    char *p1;
    if (PL_get_atom_chars(t1, &p1)) {
        DataPatch *data_patch = str2ptr<DataPatch>(p1);
        PlTail tail(A2);
        vector<long> range = list2vec<long>(tail);

        arma::Row<size_t> *labels = data_patch->get_labels_type<size_t>();
        arma::Row<size_t> predictions(data_patch->size(), fill::zeros);
        for (auto it = range.begin(); it != range.end(); ++it)
            predictions[(int) *it] = 1;

        //double err = error_rate(*labels, predictions);
        double auc = aucroc(*labels, predictions);
        //cout << "err: " << err << endl;
        term_t t3 = PL_new_term_ref();
        if (PL_put_float(t3, auc)) {
            return A3 = PlTerm(t3);
        } else {
            cerr << "[eval_range] Writing @Err error!" << endl;
            return FALSE;
        }
    } else {
        cerr << "[eval_range] Reading @DataPatch error!" << endl;
        return FALSE;
    }
}

/* centering_data(InDataPatch, OutDataPatch)
 * centering the contents in images
 */
PREDICATE(centering_data, 2) {
    char *p1;
    if (!(p1 = (char*) A1))
        return FALSE;
    const string add_data(p1);
    DataPatch *data_patch = str2ptr<DataPatch>(p1);
    arma::mat *imgs = data_patch->get_features();
    int l = data_patch->size();
    int dim = imgs->n_rows;
    int m = sqrt(dim);
    int n = m; // square img
    arma::mat centered_mat(dim, l);

    for (int idx = 0; idx < l; idx++) {
        arma::vec img_vec = imgs->col(idx);
        arma::mat img = arma::reshape(img_vec, n, m);
        int middle = round(n / 2);
        arma::uvec ind = find(img_vec);
        //arma::umat sub = arma::ind2sub(arma::size(n, m), ind);
        arma::umat sub(ind.n_rows, 2);
        for (int ii = 0; ii < (int) ind.n_rows; ii++) {
            sub(ii, 0) = (int) ind.at(ii) / n;
            sub(ii, 1) = (int) ind.at(ii) % n;
        }
        arma::uvec subi = sub.col(1);
        arma::uvec subj = sub.col(0);
        arma::mat newimg(n, m);
        arma::mat lastimg(n, m);
        
        double minj = (double) arma::min(subj);
        double maxj = (double) arma::max(subj);
        double mini = (double) arma::min(subi);
        double maxi = (double) arma::max(subi);
        int centj = round((maxj + minj) / 2);
        int centi = round((maxi + mini) / 2);
        
        if (centj <= middle) {
            arma::uword diffn = middle - centj;
            if (diffn == 0)
                newimg = img;
            else {
                arma::mat temp = img.cols(n - diffn, n - 1);
                newimg = arma::join_rows(temp, img.cols(0, n - diffn - 1));
            }
        } else {
            arma::uword diffn = centj - middle;
            if (diffn == 0)
                newimg = img;
            else {
                arma::mat temp = img.cols(0, diffn - 1);
                newimg = arma::join_rows(img.cols(diffn, n - 1), temp);
            }
        }
        
        if (centi <= middle) {
            arma::uword diffn = middle - centi;
            if (diffn == 0)
                lastimg = newimg;
            else {
                arma::mat temp = newimg.rows(n - diffn, n - 1);
                lastimg = arma::join_cols(temp, newimg.rows(0, n - diffn - 1));
            }
        } else {
            arma::uword diffn = centi - middle;
            if (diffn == 0)
                lastimg = newimg;
            else {
                arma::mat temp = newimg.rows(0, diffn - 1);
                lastimg = arma::join_cols(newimg.rows(diffn, n - 1), temp);
            }
        }
        centered_mat.col(idx) = arma::vectorise(lastimg);
    }
    DataPatch *p_data = new DataPatch(centered_mat);
    string addr = ptr2str(p_data);
    cout << endl;
    return A2 = PlTerm(addr.c_str());
}

