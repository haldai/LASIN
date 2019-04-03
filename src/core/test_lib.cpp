#include "memread.hpp"
#include "data_set.hpp"
#include "data_patch.hpp"
#include "../arff/arff_data.h"
#include "../arff/arff_parser.h"

#include <SWI-cpp.h>
#include <SWI-Prolog.h>
#include <mlpack/core.hpp>
#include <mlpack/methods/hoeffding_trees/hoeffding_tree.hpp>
#include <mlpack/methods/hoeffding_trees/binary_numeric_split.hpp>
#include <mlpack/methods/hoeffding_trees/information_gain.hpp>

#include <iostream>
#include <sstream>
#include <cstring>

typedef HoeffdingTree<GiniImpurity, HoeffdingDoubleNumericSplit> HOEFF_TREE;

using namespace std;
using namespace arma;
using namespace mlpack;
using namespace mlpack::tree;
using namespace mlpack::data;

void assert_classifier(PlTerm A) {
    PlTermv pl_args(1);
    pl_args[0] = PlCompound("classifier", A);
    PlQuery q("assertz", pl_args);
    q.next_solution();
}

void retract_classifier(PlTerm A) {
    PlTermv pl_args(1);
    pl_args[0] = PlCompound("classifier", A);
    PlQuery q("retract", pl_args);
    q.next_solution();
}

// testing
PREDICATE(test_learn, 2) {
    term_t t1 = A1.ref;
    char *p1;
    if (PL_get_atom_chars(t1, &p1)) {
        cout << "PL args: " << p1 << endl;
        DataPatch *dataTrain = str2ptr<DataPatch>(p1);
        
        RFTrainer trainer;
        RFClassifier *model = new RFClassifier();
        trainer.setMTry(1);
        trainer.setNodeSize(dataTrain->numberOfElements() * 0.2);
        trainer.setNTrees(200);
        trainer.train(*model, *(dataTrain->get_data()));

        //CARTTrainer trainer;
        //trainer.setNumberOfFolds(10); // limit the complexity of tree
        //CARTClassifier<RealVector> *model = new CARTClassifier<RealVector>();
        //trainer.train(*model, *(dataTrain->get_data()));

        term_t t2 = PL_new_term_ref();
        string model_addr = ptr2str(model);
        if (PL_put_atom_chars(t2, model_addr.c_str())) {
            // assert the classifier
            A2 = PlTerm(t2);
            assert_classifier(A2);
            return TRUE;
        } else {
            cerr << "Writing address " << model_addr << " error!" << endl;
            return FALSE;
        }
    } else {
        cerr << "Reading pointer error!" << endl;
        return FALSE;
    }
}

PREDICATE(test_test, 2) {
    term_t t1 = A1.ref;
    term_t t2 = A2.ref;
    char *p1;
    char *p2;
    if (PL_get_atom_chars(t1, &p1) && (PL_get_atom_chars(t2, &p2))) {
        cout << "PL args: " << p1 << ", " << p2 << endl;
        DataPatch *dataTrain = str2ptr<DataPatch>(p1);
        //RFClassifier *model = str2ptr<RFClassifier>(p2);
        AbstractModel<RealVector, RealVector> *model = str2ptr<AbstractModel<RealVector, RealVector>>(p2);
        
        // test
        ZeroOneLoss<unsigned int, RealVector> loss;
        Data<RealVector> prediction = (*model)(dataTrain->get_data()->inputs());
        auto pp = (dataTrain->get_data()->labels()).elements().begin();
        int idx = -1;
        for (auto pos = prediction.elements().begin(); pos != prediction.elements().end(); ++pos) {
            idx++;
            cout << "#" << idx << ">\t" << blas::arg_max(*pos) << " : " << *pp << " :: " << dataTrain->get_data()->labels().element(idx);
            if (blas::arg_max(*pos) == *pp)
                cout << endl;
            else
                cout << "*****" << endl;
            ++pp;
        }
        double err = loss.eval(dataTrain->get_data()->labels(), prediction);
        cout << "Error: " << err << endl;
        return TRUE;
    } else {
        cerr << "Reading pointer error!" << endl;
        return FALSE;
    }
}

}
}
