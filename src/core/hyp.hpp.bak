#ifndef _HYP_HPP_
#define _HYP_HPP_

#include "memread.hpp"
#include "models.hpp"
#include "utils.hpp"

#include <SWI-cpp.h>
#include <SWI-Prolog.h>
#include <mlpack/core.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/serialization/version.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

#define MODELDIR "../../models"

using namespace std;
//using namespace boost;
using namespace mlpack;

template <typename Type>
class MyHyp {
private:
    string m_sig_char;
    string m_sizebound_char;
    string m_metarules_char;
    size_t num_stat_models; // number of statistical models
    size_t num_metasubs;
    vector<string> m_metasubs;
    vector<StatModel<Type>> m_stat_models;
    //vector<HOEFF_TREE> m_stat_models;
    DatasetInfo m_info; // only for hoeffding tree
    
public:
    MyHyp();
    /* Constructor for Adaboost hypothesis*/
    //MyHyp(double tolerance = 1e-6);
    /* Constructors for Hoeffding tree hypothesis */
    MyHyp(DatasetInfo *info);
    ~MyHyp();
    /* parse prolog hypothesis */
    void parse_pl_hyp(PlTerm hyp);
    /* accessor */
    //vector<HOEFF_TREE> get_stat_models();
    vector<StatModel<Type>> get_stat_models();
    /* Return the stored hypothesis as a PlTerm (ps(_,_,_,_)) */
    PlTerm to_prolog_hyp();
    /*
    template<class Archive>
    void Serialize(Archive& ar, const unsigned int version);
    */
};

template<>
class MyHype<HOEFF_TREE> {
public:
    template<class Archive>
    void Serialize(Archive& ar, const unsigned int version);
};

template<>
class MyHype<ADABOOST_STUMP> {
public:
    template<class Archive>
    void Serialize(Archive& ar, const unsigned int version);
};

template<typename Type>
MyHyp<Type>::MyHyp() : m_sig_char(),
                       m_sizebound_char(),
                       m_metarules_char(),
                       m_metasubs(),
                       m_stat_models() {
    num_stat_models = 0;
    num_metasubs = 0;
    m_info = DatasetInfo();
}

template<typename Type>
MyHyp<Type>::MyHyp(DatasetInfo *info) : m_sig_char(),
                                  m_sizebound_char(),
                                  m_metarules_char(),
                                  m_metasubs(),
                                  m_stat_models() {
    num_stat_models = 0;
    num_metasubs = 0;
    m_info = *info;
}

/* Hyp is a hypothesis (ps([MetaSub1, MetaSub2, ...], Signature, SizeBound,
 * MetaRules)
 * MetaSub is term like:
 *    metasub(stat_cls,[l01_01__1/3,stat_classifier/4,'0x35b13c0'])
 * this constructor analyses this compound
 */
template<typename Type>
void MyHyp<Type>::parse_pl_hyp(PlTerm Hyp) {
    PlTail clause_list(Hyp[1]);
    PlTerm sig = Hyp[2];
    PlTerm sizebound = Hyp[3];
    PlTerm metarules = Hyp[4];
    
    m_sig_char = (char *) sig;
    m_sizebound_char = (char *) sizebound;
    m_metarules_char = (char *) metarules;

    map<string, size_t> name_idx_map;
    
    PlTerm clause;
    vector<PlTerm> clauses;
    // for each clause
    while (clause_list.next(clause)) {
        string clause_str((char *)clause);
        clauses.push_back(clause);
        PlTail pred_list(clause[2]); // metasub itmes
        PlTerm arg;
        while (pred_list.next(arg)) {
            char *chrs = (char*) arg;
            if (chrs[0] == '0' && chrs[1] == 'x') { // found model address
                string model_name(chrs);
                //HOEFF_TREE *model = str2ptr<HOEFF_TREE>(model_name);
                StatModel<Type> *model = str2ptr<StatModel<Type>>(model_name);
                m_stat_models.push_back(*model);
                size_t model_idx = -1;
                if (name_idx_map.find(model_name) == name_idx_map.end()) {
                    name_idx_map.insert(std::pair<string, size_t>(
                                            model_name,
                                            m_stat_models.size()));
                    model_idx = m_stat_models.size() - 1;
                } else
                    model_idx = name_idx_map[model_name];
                // replace clause_str's model addr with model idx
                string model_idx_str = "STATMODEL_" + to_string(model_idx);
                str_replace_all(clause_str, model_name, model_idx_str);
            }
        }
        m_metasubs.push_back(clause_str);
    }
    num_metasubs = m_metasubs.size();
    num_stat_models = m_stat_models.size();
}

template<typename Type>
MyHyp<Type>::~MyHyp() {}
/*
vector<string> MyHyp<Type>get_metasubs() {
    return m_metasubs;
}
*/
template<typename Type>
vector<StatModel<Type>> MyHyp<Type>::get_stat_models() {
    return m_stat_models;
}

/*
vector<string> MyHyp<Type>get_model_addrs() {
    return m_model_addrs;
}
*/
template<typename Type>
PlTerm MyHyp<Type>::to_prolog_hyp() {
    long int i = -1;
    // update model address
    vector<string> model_addrs(num_stat_models);
    for (auto it = m_stat_models.begin(); it != m_stat_models.end(); ++it) {
        i++;
        void *p = m_stat_models.data() + (it - m_stat_models.begin());
        string m_add = ptr2str(p);
        model_addrs[i] = m_add;
    }

    term_t list_term_ref = PL_new_term_ref();
    PlTerm list_term(list_term_ref);
    PlTail pl_list(list_term);
    // substitute and output
    for (size_t idx = 0; idx < num_metasubs; idx++) {
        string metasub_str = m_metasubs[idx];
        for (size_t j = 0; j < num_stat_models; j++) {
            string model_idx_str = "STATMODEL_" + to_string(j);
            str_replace_all(metasub_str, model_idx_str, model_addrs[j]);
        }
        PlCompound temp_compound(metasub_str.c_str());
        pl_list.append((PlTerm) temp_compound);
    }
    pl_list.close();
    // construct prolog term as output
    PlTermv args(list_term,
                 PlCompound(m_sig_char.c_str()),
                 PlCompound(m_sizebound_char.c_str()),
                 PlCompound(m_metarules_char.c_str()));
    PlCompound ps_term("ps", args);
    
    return (PlTerm) ps_term;
}

template<>
template<class Archive>  
void MyHyp<HOEFF_TREE>::Serialize(Archive& ar, const unsigned int version) {
    ar & mlpack::data::CreateNVP(m_sig_char, "m_sig_char");
    ar & mlpack::data::CreateNVP(m_sizebound_char, "m_sizebound_char");
    ar & mlpack::data::CreateNVP(m_metarules_char, "m_metarules_char");
    ar & mlpack::data::CreateNVP(num_stat_models, "num_stat_models");
    ar & mlpack::data::CreateNVP(num_metasubs, "num_metasubs");
    ar & mlpack::data::CreateNVP(m_info, "m_info");
    
    if (Archive::is_loading::value) {
        m_metasubs.clear();
        m_metasubs.resize(num_metasubs);
    }
    
    for (size_t i = 0; i < num_metasubs; ++i) {
        std::ostringstream oss;
        oss << "m_metasubs" << i;
        ar & mlpack::data::CreateNVP(m_metasubs[i], oss.str());
    }
    
    //ar & mlpack::data::CreateNVP(m_name_model_map, "m_name_model_map");
    
    if (Archive::is_loading::value) {
        m_stat_models.clear();
        if (is_same<Type, HOEFF_TREE>::value)
            m_stat_models.resize(num_stat_models, StatModel<Type>(m_info, 2)); // each tree is a binary classifier
        else if (is_same<Type, ADABOOST_STUMP>::value)
            // TODO:: debug
            m_stat_models.resize(num_stat_models, StatModel<Type>());
    }
    
    for (size_t i = 0; i < num_stat_models; ++i) {
        std::ostringstream oss;
        oss << "m_stat_models" << i;
        ar & mlpack::data::CreateNVP(m_stat_models[i], oss.str());
    }
}

template<>
template<class Archive>  
void MyHyp<ADABOOST_STUMP>::Serialize(Archive& ar, const unsigned int version) {
    ar & mlpack::data::CreateNVP(m_sig_char, "m_sig_char");
    ar & mlpack::data::CreateNVP(m_sizebound_char, "m_sizebound_char");
    ar & mlpack::data::CreateNVP(m_metarules_char, "m_metarules_char");
    ar & mlpack::data::CreateNVP(num_stat_models, "num_stat_models");
    ar & mlpack::data::CreateNVP(num_metasubs, "num_metasubs");
    ar & mlpack::data::CreateNVP(m_info, "m_info");
    
    if (Archive::is_loading::value) {
        m_metasubs.clear();
        m_metasubs.resize(num_metasubs);
    }
    
    for (size_t i = 0; i < num_metasubs; ++i) {
        std::ostringstream oss;
        oss << "m_metasubs" << i;
        ar & mlpack::data::CreateNVP(m_metasubs[i], oss.str());
    }
    
    //ar & mlpack::data::CreateNVP(m_name_model_map, "m_name_model_map");
    
    if (Archive::is_loading::value) {
        m_stat_models.clear();
        if (is_same<Type, HOEFF_TREE>::value)
            m_stat_models.resize(num_stat_models, StatModel<Type>(m_info, 2)); // each tree is a binary classifier
        else if (is_same<Type, ADABOOST_STUMP>::value)
            // TODO:: debug
            m_stat_models.resize(num_stat_models, StatModel<Type>());
    }
    
    for (size_t i = 0; i < num_stat_models; ++i) {
        std::ostringstream oss;
        oss << "m_stat_models" << i;
        ar & mlpack::data::CreateNVP(m_stat_models[i], oss.str());
    }
}

#endif
