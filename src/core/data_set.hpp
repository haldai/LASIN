#ifndef _DATA_SET_HPP_
#define _DATA_SET_HPP_

#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/median.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <mlpack/core.hpp>

#include "../arff/arff_data.h"
#include "../arff/arff_value.h"
#include "../arff/arff_utils.h"
#include "data_patch.hpp"
#include "utils.hpp"

#include <string>
#include <fstream>

using namespace std;
using namespace arma;

using PolicyType = mlpack::data::IncrementPolicy;

/* Multi-label dataset with label structure, loaded from arff data */
class MLDataSet {

private:
    /* data of inistance feature vectors */
    mat m_data;
    /* median and mean for missing label */
    vector<double> m_data_mean;
    vector<double> m_data_median;
    /* labels */
    umat m_labels; // multi label container: labels[instance][label]
    /* number of instances */
    uword m_size; // number of instances
    /* number of labels */
    uword m_num_labels; // number of labels
    /* dimension of feature */
    uword m_dimension; // dimention of feature space
    /* feature names */
    vector<string> m_name_features; // feature names
    /* label names */
    vector<string> m_name_labels; // name of labels
    map<string, uword> m_label_idx_map; // index mapping of labels
    /* label relation */
    boost::adjacency_list<> *m_label_adj_list; // adjacent list of labels
    /* DataInfo */
    mlpack::data::DatasetMapper<PolicyType> *m_mapper;

    /* private functions */
    
    /* read labels: only attributes with prefix "class", return index list */
    vector<uword> parse_labels(ArffData *arff_data);
    /* read features: the attributes are not labels, return an index list */
    vector<uword> parse_features(ArffData *arff_data);
    /* read instances (only features) */
    void read_instance_features(ArffData *arff_data, int missing = 0);
    /* read instances (only labels) as vector of unsigned int */
    void read_instance_labels(ArffData *arff_data);
    // add an 'l' in front of label name, and replace slashes to dashes
    string slash_to_dash(string in_str);
    // reverse operator of the previous function
    string dash_to_slash(string in_str);
    
/* public functions */
    
public:
    /* Constructor and Destructor */
    MLDataSet(ArffData *arff_data); // constructor
    ~MLDataSet(); // destructor
    /* Modifiers */
    /* Element acess*/
    rowvec get_instance(uword inst_idx);
    uword get_inst_label(uword inst_idx, uword label_idx);
    /* Labels acess */
    uvec labels_column(uword label_idx);
    urowvec labels_row(uword inst_idx);
    uword label_index(string label_name);
    /* Capacity */
    uword num_instances();
    uword num_labels();
    /* Name acess */
    string feature_name(uword feat_idx);
    string label_name(uword label_idx);
    vector<string> feature_names();
    vector<string> label_names();
    /* Dataset acess, return pointers. !!!!REMEMBER DELETION!!!!! */
    DataPatch data_with_label(uword label_idx);
    // instances belong to class label_idx, value as label_value
    DataPatch data_of_label(uword label_idx, uword label_value = 1);
    DataPatch data_of_label(string name, uword label_value = 1);
    // indices of label_idx == label_values
    PlTerm range_of_label(uword label_idx, uword label_value = 1);
    PlTerm range_of_label(string name, uword label_value = 1);
    /* Data info access */
    mlpack::data::DatasetMapper<PolicyType> *get_mapper();
    /* Label relation acess*/
    boost::adjacency_list<> *label_relation();
    /* get label relation */
    vector<string> pl_label_relation();
    /* print label relation to file_path*/
    void print_label_relation(string file_path);
};

/* Constructor */
MLDataSet::MLDataSet(ArffData *arff_data) : m_data_mean(),
                                            m_data_median(),
                                            m_name_features(),
                                            m_name_labels(),
                                            m_label_idx_map() {
    // new a adjacency list
    m_label_adj_list = new boost::adjacency_list<>();
    // set the size of data
    m_size = arff_data->num_instances();
    // translate data attributes into m_data and m_data_view
    read_instance_features(arff_data);
    // translate labels into m_label and m_data_view
    read_instance_labels(arff_data);
}

MLDataSet::~MLDataSet() {
    delete m_label_adj_list;
    m_label_adj_list = NULL;
}

rowvec MLDataSet::get_instance(uword inst_idx) {
    return m_data.row(inst_idx);
}

uword MLDataSet::get_inst_label(uword inst_idx, uword label_idx) {
    return m_labels(inst_idx, label_idx);
}

uvec MLDataSet::labels_column(uword label_idx) {
    return m_labels.col(label_idx);
}

urowvec MLDataSet::labels_row(uword inst_idx) {
    return m_labels.row(inst_idx);
}

uword MLDataSet::label_index(string label_name) {
    return m_label_idx_map[label_name];
}

uword MLDataSet::num_instances() {
    return m_size;
}

uword MLDataSet::num_labels() {
    return m_num_labels;
}

string MLDataSet::feature_name(uword feat_idx) {
    return m_name_features[feat_idx];
}

string MLDataSet::label_name(uword label_idx) {
    return m_name_labels[label_idx];
}

vector<string> MLDataSet::feature_names() {
    return m_name_features;
}

vector<string> MLDataSet::label_names() {
    return m_name_labels;
}

boost::adjacency_list<> *MLDataSet::label_relation() {
    return m_label_adj_list;
}

mlpack::data::DatasetMapper<PolicyType> *MLDataSet::get_mapper() {
    return m_mapper;
}

DataPatch MLDataSet::data_with_label(uword label_idx) {
    return DataPatch(m_data, uvec(m_labels.col(label_idx)), *m_mapper);
}

DataPatch MLDataSet::data_of_label(uword label_idx, uword label_value) {
    vector<uword> selection;
    for (uword i = 0; i < m_size; i++) {
        if (get_inst_label(i, label_idx) == label_value)
            selection.push_back((uword) i);
    }
    uvec sel = uvec(selection);
    mat features = m_data.rows(sel);
    uvec label_total = m_labels.col(label_idx);
    uvec label_sel = label_total.rows(sel);
    return DataPatch(features, label_sel, *m_mapper);
}

DataPatch MLDataSet::data_of_label(string name, uword label_value) {
    uword idx = label_index(name);
    return data_of_label(idx, label_value);
}

PlTerm MLDataSet::range_of_label(uword label_idx, uword label_value) {
        
    vector<long> selection;
    for (int i = 0; i < long(m_size); i++) {
        if (get_inst_label(i, label_idx) == label_value)
            selection.push_back(i);
    }
    return vec2list<long>(selection);
}

PlTerm MLDataSet::range_of_label(string name, uword label_value) {
    uword idx = label_index(name);
    return range_of_label(idx, label_value);
}

vector<string> MLDataSet::pl_label_relation() {
    using namespace boost;
    vector<string> re;
    graph_traits <adjacency_list<> >::vertex_iterator vi, v_end;
    graph_traits <adjacency_list<> >::adjacency_iterator ai, a_end;
    //property_map <adjacency_list<>, vertex_index_t>::type
    //    index_map = get(vertex_index, *m_label_adj_list);

    for (tie(vi, v_end) = vertices(*m_label_adj_list); vi != v_end; ++vi) {
        string father = slash_to_dash(m_name_labels[*vi]);
        tie(ai, a_end) = adjacent_vertices(*vi, *m_label_adj_list);
        /*
        if (ai == a_end)
            cout << " has no children";
        else 
            cout << " is the parent of ";
        */
        for (; ai != a_end; ++ai) {
            //cout << m_name_labels[*ai];
            string child = slash_to_dash(m_name_labels[*ai]);
            string atom_s = "father('" + father + "','" + child + "').";
            re.push_back(atom_s);
            cout << "father('" << father << "','"
                      << child << "')." << endl;
        }
    }
    return re;
}

void MLDataSet::print_label_relation(string file_path) {
    vector<string> pl_atoms = pl_label_relation();
    ofstream pl_file;
    pl_file.open(file_path);
    for (auto it = pl_atoms.begin(); it != pl_atoms.end(); ++it) {
        pl_file << *it << endl;
    }
    pl_file.close();
}

vector<uword> MLDataSet::parse_labels(ArffData *arff_data) {
    vector<uword> re;
    uword num_attr = arff_data->num_attributes();
    for (uword idx = 0; idx < num_attr; idx++) {
        string attr_name = (string) (arff_data->get_attr(idx))->name();
        if (attr_name.find("class") == 0) {
            re.push_back(idx);
            // read label names
            ArffHierarchical *h_attr = arff_data->get_hierarchical(attr_name);
            vector<string> *l_names =  h_attr->get_item_list();
            for (auto itr = l_names->begin(); itr != l_names->end(); ++itr) {    
                m_label_idx_map[*itr] = (uword) m_name_labels.size();
                m_name_labels.push_back(*itr);                 
            }
            // insert label hierarchy (because label indices are disturbed by reading label names)
            // get new father_idx and son_idx
            boost::adjacency_list<> *adj_list = h_attr->get_adjacency_list();
            boost::graph_traits <boost::adjacency_list <> >::vertex_iterator vi, v_end;
            boost::graph_traits <boost::adjacency_list <> >::adjacency_iterator ai, a_end;
            boost::property_map <boost::adjacency_list <>, boost::vertex_index_t >::type
                index_map = boost::get(boost::vertex_index, *adj_list); // local index_map
            // for each vertex
            for (boost::tie(vi, v_end) = vertices(*adj_list); vi != v_end; ++vi) {
                string f_name = h_attr->get_item(boost::get(index_map, *vi));
                boost::tie(ai, a_end) = boost::adjacent_vertices(*vi, *adj_list);
                if (ai != a_end) { // if exists an edge
                    uword f_idx = m_label_idx_map[f_name];
                    for (; ai != a_end; ++ai) {
                        string s_name = h_attr->get_item(boost::get(index_map, *ai));
                        uword s_idx = m_label_idx_map[s_name];
                        if (boost::next(ai) != a_end)
                            boost::add_edge(f_idx, s_idx, *m_label_adj_list);
                    }
                }
            }
        }
    }
    return re;
};

vector<uword> MLDataSet::parse_features(ArffData *arff_data) {
    vector<uword> re;
    uword num_attr = arff_data->num_attributes();
    uword num_inst = arff_data->num_instances();
    for (uword idx = 0; idx < num_attr; idx++) {
        string attr_name = (string) (arff_data->get_attr(idx))->name();
        if (attr_name.find("class") != 0) {
            // push index into return vector
            re.push_back(idx);
            // add feature names
            m_name_features.push_back(attr_name);
            /* compute mean, median
            using namespace boost::accumulators;
            accumulator_set<double, stats<boost::accumulators::tag::mean, boost::accumulators::tag::median(with_p_square_quantile)>> acc;
            // for each instance, apply accumulation
            for (uword i = 0; i < num_inst; i++) {
                ArffValue *val = arff_data->get_instance(i)->get(idx);
                if (!val->missing()) {
                    acc((double) *val);
                }
            }
            m_data_mean.push_back(boost::accumulators::mean(acc));
            m_data_median.push_back(boost::accumulators::median(acc));
            */
        }
    }
    m_dimension = re.size();
    // record data info
    m_mapper = new mlpack::data::DatasetMapper<PolicyType>(m_dimension);


    for (uword dim = 0; dim < re.size(); dim++) {
        uword f_idx = re[dim];
        ArffAttr *attr = arff_data->get_attr(f_idx);

        // compute mean, median and transform nominal to numerical
        using namespace boost::accumulators;
        accumulator_set<double, stats<boost::accumulators::tag::mean, boost::accumulators::tag::median(with_p_square_quantile)>> acc;
        // for each instance, apply accumulation
        for (uword i = 0; i < num_inst; i++) {
            ArffValue *val = arff_data->get_instance(i)->get(f_idx);

            if (!val->missing()) {
                double val_e;
                if (attr->type() == NOMINAL || attr->type() == H_NOMINAL) {
                    string trimmedToken((string) *val);
                    val_e = m_mapper->MapString<uword>(trimmedToken, dim);
                } else {
                    val_e = (double) *val;
                }
                acc(val_e);
            }
        }
        m_data_mean.push_back(boost::accumulators::mean(acc));
        m_data_median.push_back(boost::accumulators::median(acc));
    }
    return re;
}

void MLDataSet::read_instance_features(ArffData *arff_data, int missing) {
    // read feature indices and get dimension
    vector<uword> feat_idx_list = parse_features(arff_data);

    mat t_m_data(m_size, m_dimension, fill::zeros);
    // read each instance
    for (uword idx = 0; idx < (uword) arff_data->num_instances(); idx++) {
        for (uword f = 0; f < m_dimension; f++) {
            uword f_idx = feat_idx_list[f];
            ArffAttr *attr = arff_data->get_attr(f_idx);
            ArffValue *val = arff_data->get_instance(idx)->get(f_idx);
            if (!val->missing()) {
                double val_e;
                if (attr->type() == NOMINAL || attr->type() == H_NOMINAL) {
                    string trimmedToken((string) *val);
                    val_e = m_mapper->MapString<uword>(trimmedToken, f);
                } else {
                    val_e = (double) *val;
                }
                t_m_data(idx, f) = val_e;
            } else {
                // if missing = 1, use mean value to replace missing value
                // else use median value
                switch (missing) {
                case 1:
                    t_m_data(idx, f) = m_data_mean[f_idx];
                    break;
                default:
                    t_m_data(idx, f) = m_data_median[f_idx];
                    break;
                }
            }
        }
    }
    m_data = mat(t_m_data);
};

void MLDataSet::read_instance_labels(ArffData *arff_data) {
    // parse features
    vector<uword> label_idx_list = parse_labels(arff_data);
    // read each instance
    vector<vector<uword>> temp_labels;
    for (uword idx = 0; idx < (uword) arff_data->num_instances(); idx++) {
        vector<uword> rv;
        for (uword l = 0; l < (uword) label_idx_list.size(); l++) {
            uword l_idx = label_idx_list[l];
            ArffValue *val = arff_data->get_instance(idx)->get(l_idx);
            switch (val->type()) {
            case H_NOMINAL: {
                // if is hierarchical treat as multi-label
                // this attribute
                const string attr_name = arff_data->get_attr(l_idx)->name();
                ArffHierarchical *h_attr = arff_data->get_hierarchical(attr_name);
                uword dim = (uword) h_attr->number_of_items(); // number of labels in this hierarchy
                vector<uword> h_rv = vector<uword>(dim, (uword) 0);
                vector<string> l_vals = *val;
                // for each nominal value, map its index and set it to 1, then concatenate with rv;
                for (auto itr = l_vals.begin(); itr != l_vals.end(); ++itr) {
                    // if the hierarchy is tree, label all fathers as well
                    if (h_attr->is_tree()) {
                        vector<string> all_labels = split_h_str(*itr, "/\\");
                        for (auto sub_itr = all_labels.begin(); sub_itr != all_labels.end();
                             ++sub_itr)
                            h_rv[h_attr->get_item_idx(*sub_itr)] = 1;
                    } else
                        h_rv[h_attr->get_item_idx(*itr)] = 1;
                }
                rv.insert(rv.end(), h_rv.begin(), h_rv.end());
                break;
            }
            case NOMINAL: {
                // if label is nominal (multiclass) treat as single label
                const string attr_name = arff_data->get_attr(l_idx)->name();
                vector<string> n_attr = arff_data->get_nominal(attr_name);
                string n_val = (string) *val;
                vector<string>::iterator result = find(n_attr.begin(), n_attr.end(), n_val);
                if (result != n_attr.end()) {
                    uword n_idx = result - n_attr.begin();
                    rv.push_back(n_idx);
                } else {
                    cerr << "Unknown label: " << n_val << " in " << attr_name << "!\n";
                    exit(0);
                }
                break;
            }
            default: // has to be numeric
                uword num_val = 0;
                if ((*val).type() != NUMERIC) {
                    cerr << "Unknown class type: " << val->type() << "\n";
                    exit(0);
                }
                try {
                    num_val = (uword) ((int32) *val);
                } catch (...) {
                    cerr << "Unknown class value: " << (int32) *val << "\n";
                }
                rv.push_back(num_val);
                break;
            }
        }
        temp_labels.push_back(rv);
    }
    m_num_labels = (uword) temp_labels[0].size();
    m_labels = umat(m_size, m_num_labels, fill::zeros);
    for (uword i = 0; i < m_size; i++)
        m_labels.row(i) = urowvec(temp_labels[i]);
};

string MLDataSet::slash_to_dash(string in_str) {
    string re = "l" + boost::replace_all_copy(in_str, "/", "_");
    return re;
}

string MLDataSet::dash_to_slash(string in_str) {
    string re = in_str.substr(1);
    re = boost::replace_all_copy(re, "_", "/");
    return re;
}

#endif
