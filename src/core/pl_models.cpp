#include "memread.hpp"
#include "data_patch.hpp"
#include "utils.hpp"

#include <SWI-cpp.h>
#include <SWI-Prolog.h>
#include <mlpack/core.hpp>
#include <mlpack/methods/sparse_coding/sparse_coding.hpp>

#include <stdlib.h>
#include <time.h>

using namespace std;
using namespace arma;
using namespace mlpack;

/* sparse_coder(DataPatch, Model, [Params], Coded_DataPatch)
 * Params = [Atoms, Lambda1, Lambda2, MaxIterations].
 * Atoms: Number of atoms in dictionary.
 * Lambda1: Regularization parameter for l1-norm penalty.
 * Lambda2: Regularization parameter for l2-norm penalty.
 * MaxIterations: Maximum number of iterations to run algorithm. If 0, the
 *                algorithm will run until convergence (or forever).
 */
PREDICATE(sparse_coder, 4) {
    char *p1 = (char*) A1;
    DataPatch *data = str2ptr<DataPatch>(p1);
    // if Model is a variable train a new one, other wise evaluate
    bool train = (A2.type() == PL_VARIABLE);
    sparse_coding::SparseCoding *sc;
    if (train) {
        cout << "Training a new model...\n";
        sc = new sparse_coding::SparseCoding(0, 0.0);

        // read parameters, [Atoms, Lambda1, Lambda2, MaxIterations]
        vector<double> params = list2vec<double>(A3, 4);
        size_t atoms = (size_t) params[0];
        size_t max_iter = (size_t) params[3];        
        double lambda1 = params[1];
        double lambda2 = params[2];
        sc->Atoms() = atoms;        
        sc->MaxIterations() = max_iter;
        sc->Lambda1() = lambda1;
        sc->Lambda2() = lambda2;

        // train
        cout << "Start training sparse coder...\n";
        sc->Train(*(data->get_features()));
        cout << "Training ended.\n";
        string model_addr = ptr2str(sc);
        A2 = PlTerm(model_addr.c_str());
    } else {
        char *p2 = (char*) A2;
        sc = str2ptr<sparse_coding::SparseCoding>(p2);
    }
    // eval and return encode data
    mat codes;
    cout << "Start encoding...\n";
    sc->Encode(*(data->get_features()), codes);
    cout << "Encoding ended.\n";
    data::DatasetInfo info(codes.n_rows);
    DataPatch *coded_data = new DataPatch(&codes, data->get_labels(), info);
    string new_data_addr = ptr2str(coded_data);
    return A4 = PlTerm(new_data_addr.c_str());
}

/* sc_load_model(Param, DictPath, Model) */
PREDICATE(sc_load_model, 3) {
    // load parameters
    vector<double> params = list2vec<double>(A1, 4);
    size_t atoms = (size_t) params[0];
    size_t max_iter = (size_t) params[3];        
    double lambda1 = params[1];
    double lambda2 = params[2];
    // make a new model
    sparse_coding::SparseCoding *sc =
        new sparse_coding::SparseCoding(atoms, lambda1);
    sc->Lambda2() = lambda2;
    sc->MaxIterations() = max_iter;
    // load dictionary
    char *p2 = (char*) A2;
    string file_path(p2);
    mat dict;
    data::Load(file_path, dict, true, true);
    sc->Dictionary() = dict;
    // return model
    string addr = ptr2str(sc);
    return A3 = PlTerm(addr.c_str());
}

/* sc_encod_file(Model, FileIn, FileOut) */
PREDICATE(sc_encode_file, 3) {
    char *p1 = (char*) A1;
    sparse_coding::SparseCoding *sc =
        str2ptr<sparse_coding::SparseCoding>(p1);
    char *p2 = (char*) A2;
    char *p3 = (char*) A3;
    string file_in(p2);
    string file_out(p3);
    arma::mat data_in;
    arma::mat data_out;
    data::Load(file_in, data_in, true);
    cout << "start encoding..." << endl;
    sc->Encode(data_in, data_out);
    cout << "encoding ended..." << endl;
    data::Save(file_out, data_out, true);
    return TRUE;
}

/* sc_sample_mask_center(Data, Model, Thresh, Centers)
 * Given a data and model and difference threshold, return the center of
 * new data patches that should be sampled.   
 * @Data: original dataset
 * @Model: sparse coding model
 * @Thresh: threshold of large difference between encoded and original data
 * @Num_samples: number of sampled points
 * @Centers: list of indices in feature dimension, as mask centers
 */
PREDICATE(sc_sample_mask_center, 5) {
    char *p1 = (char*) A1;
    DataPatch *data = str2ptr<DataPatch>(p1);
    mat org_data = *(data->get_features());    
    mat decoded;
    if (A2.type() == PL_VARIABLE)
        // if no model yet, use all zeros
        decoded = zeros(org_data.n_rows, org_data.n_cols);
    else {
        char *p2 = (char*) A2;
        sparse_coding::SparseCoding *sc = str2ptr<sparse_coding::SparseCoding>(p2);
        // Encode
        cout << "Starting reconstruction...\n";
        mat codes;
        sc->Encode(org_data, codes);
        const mat dict = sc->Dictionary();
        decoded = dict * codes;
        cout << "Reconstruction ended.\n";
    }
    // data::Save("/home/daiwz/MATLAB/abduce/decoded.csv", decoded, true, true);
    double thresh = (double) A3;
    vector<PlTerm> centers(org_data.n_cols);
    // Sampling
    srand(time(NULL));
    for (size_t i = 0; i < org_data.n_cols; i++) {
        // Check diff of each feature and find large diff positions
        Col<double> diff(arma::abs(decoded.col(i) - org_data.col(i)));
        // Randomly sample a point which have large diff
        uvec ex = find(diff > thresh);
        if (ex.n_rows > 0) {
            int num_samples = (int) A4;
            if (num_samples < 0)
                num_samples = ex.n_rows;
            vector<long> c_vec(num_samples);
            for (int s = 0; s < num_samples; s++) {
                int idx = ((int) rand()) % ex.n_rows;
                long sampled_dim = ex(idx);
                c_vec[s] = sampled_dim;
            }
            centers[i] = vec2list<long>(c_vec);
        } else {
            centers[i] = empty_list();
        }
    }
    // Make list and return
    PlTerm list = vec2list<PlTerm>(centers);
    return A5 = list;
}

/* sc_mask_patches(Data, Masks, Patches).
 * generate data patches for sparse coding training.
 * @Data: original dataset
 * @Masks: masks for filtering data, list of list [[Masks of inst 1], ...]
 * @Patches: new dataset composed of all the patches
 */
PREDICATE(sc_mask_patches, 3) {
    char *p1 = (char*) A1;
    DataPatch *data = str2ptr<DataPatch>(p1);
    int dim = data->get_feature(0).n_rows;
    vector<vector<char*>> pts = list2vecvec<char*>(A2);
    int num = data->size();
    if ((int) pts.size() != num) {
        cout << "Mask num does not equal to Data num!" << endl;
        return FALSE;
    }
    vector<Col<double>> patches;
    //int total = 0;
    for (int i = 0; i < num; i++) {
        vector<char*> ms = pts[i];
        int num_mask = ms.size();
        //total += num_mask;
        //cout << i << ' ' << num_mask << ' ' << total << endl;
        Col<double> inst = data->get_feature(i);
        // apply masks
        for (int j = 0; j < num_mask; j++) {
            Col<double> *mask = str2ptr<Col<double>>(ms[j]);
            Col<double> masked_vector = inst % *mask;
            patches.push_back(masked_vector);
            delete mask; // only use mask for one time, recollect memory
        }
    }
    // make new DataPatch of masked patches
    mat patch_mat(dim, patches.size());
    for (int i = 0; i < (int) patches.size(); i++)
        patch_mat.col(i) = patches[i];
    DataPatch *p_data = new DataPatch(patch_mat);
    string addr = ptr2str(p_data);
    return A3 = PlTerm(addr.c_str());
}

/* sc_print_dict(Model).
 * Print the encoding dictionary to screen
 */
PREDICATE(sc_print_dict, 1) {
    char *p1 = (char*) A1;
    sparse_coding::SparseCoding *sc = str2ptr<sparse_coding::SparseCoding>(p1);
    mat dict = sc->Dictionary();
    cout << "Dictionary of model " << p1 << ":\n";
    dict.t().print();
    return TRUE;
}

/* sc_save_dict(Model, Path).
 * Save the encoding dictionary to a file.
 */
PREDICATE(sc_save_dict, 2) {
    char *p1 = (char*) A1;
    char *p2 = (char*) A2;
    sparse_coding::SparseCoding *sc = str2ptr<sparse_coding::SparseCoding>(p1);
    mat dict = sc->Dictionary();
    string path(p2);
    data::Save(path, sc->Dictionary());
    cout << "Dictionary saved at " << path << ".\n";
    return TRUE;
}

/* free_sc(ADD)
 * free a sparse coding model
 */
PREDICATE(free_sc, 1) {
    char *p1 = (char*) A1;
    sparse_coding::SparseCoding *sc = str2ptr<sparse_coding::SparseCoding>(p1);
    free(sc);
    sc = NULL;
    return TRUE;
}
