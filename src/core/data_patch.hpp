#ifndef _DATA_PATCH_HPP_
#define _DATA_PATCH_HPP_

#include <iostream>
#include <cassert>
#include <mlpack/core.hpp>

#include "utils.hpp"

using namespace std;
using namespace arma;
using namespace mlpack::data;

using PolicyType = mlpack::data::IncrementPolicy;

/* Single-labeled Dataset */
class DataPatch {
private:
    mat *m_data; // stored column-wise, i.e. num_dim by num_insts
    Row<size_t> *m_labels; // labels are stored in a row
    DatasetMapper<PolicyType> *m_mapper; // auxilliary information of data
public:
    /* Constructor */
    DataPatch();
    DataPatch(string data_path, string label_path = "");
    DataPatch(mat features, Row<size_t> labels, DatasetMapper<PolicyType> info);
    DataPatch(mat features);
    DataPatch(mat *features, Row<size_t> *labels, DatasetMapper<PolicyType> info);
    DataPatch(mat features, uvec labels, DatasetMapper<PolicyType> info);
    DataPatch(mat *features, uvec *labels, DatasetMapper<PolicyType> info);
    DataPatch(mat features, vector<uword> labels, DatasetMapper<PolicyType> info);
    DataPatch(const DataPatch& data_patch); // copy
    /* Destructor */
    ~DataPatch();
    /* Modifier */
    void load_data(string file_path);
    void load_label(string file_path);
    void set_info(mlpack::data::DatasetMapper<PolicyType> info);
    /* Accessor */
    mat *get_features() const;
    vec get_feature(int idx) const;
    Row<size_t> *get_labels() const;
    template<class T> arma::Row<T> *get_labels_type();
    void print_data();
    void print_labels();
    void save_data(string file_path);
    void save_labels(string file_path);
    // select instance
    DataPatch select_data(PlTerm selectors);
    DataPatch select_data(vector<uword> selectors);
    // select feature dimension
    DataPatch select_dimension(PlTerm selectors);
    DataPatch select_dimension(vector<uword> selectors);
    mlpack::data::DatasetMapper<PolicyType> *get_mapper() const;
    /* Capacity */
    size_t size();
};

DataPatch::DataPatch() {
    m_data = NULL;
    m_labels = NULL;
    m_mapper = NULL;
}

DataPatch::DataPatch(string data_path, string label_path) {
    load_data(data_path);
    if (label_path.size() > 0)
        load_label(label_path);
    else
        m_labels = NULL;
}

DataPatch::DataPatch(mat features, uvec labels, DatasetMapper<PolicyType> info) {
    assert(features.n_rows == labels.n_rows);
    m_labels = new Row<size_t>(labels.n_rows);
    for (size_t i = 0; i < labels.n_rows; i++)
        (*m_labels)[i]  = (size_t) labels[i];
    m_data = new mat(features);
    m_mapper = new DatasetMapper<PolicyType>(info.Policy(), info.Dimensionality());
}

DataPatch::DataPatch(mat features) {
    m_data = new mat(features);
    m_labels = NULL;
    int dim = m_data->col(0).n_rows;
    m_mapper = new DatasetMapper<PolicyType>(dim);
}

DataPatch::DataPatch(mat *features, uvec *labels, DatasetMapper<PolicyType> info) {
    if (labels != NULL) {
        assert(features->n_rows == labels->n_rows);
        m_labels = new Row<size_t>(labels->n_rows);
        for (size_t i = 0; i < labels->n_rows; i++)
            (*m_labels)[i]  = (size_t) (*labels)[i];
    } else
        m_labels = NULL;
    if (features != NULL)
        m_data = new mat(*features);
    else
        m_data = NULL;
    m_mapper = new DatasetMapper<PolicyType>(info.Policy(), info.Dimensionality());
}

DataPatch::DataPatch(mat features, Row<size_t> labels, DatasetMapper<PolicyType> info) {
    assert(features.n_rows == labels.n_cols);
    m_labels = new Row<size_t>(labels);
    m_data = new mat(features);
    m_mapper = new DatasetMapper<PolicyType>(info.Policy(), info.Dimensionality());
}

DataPatch::DataPatch(mat *features, Row<size_t> *labels, DatasetMapper<PolicyType> info) {
    if (labels != NULL) {
        assert(features->n_rows == labels->n_cols);
        m_labels = new Row<size_t>(*labels);
    } else
        m_labels = NULL;
    if (features != NULL)
        m_data = new mat(*features);
    else
        m_data = NULL;
    m_mapper = new DatasetMapper<PolicyType>(info.Policy(), info.Dimensionality());
}

DataPatch::DataPatch(mat features, vector<uword> labels, DatasetMapper<PolicyType> info) {
    assert(features.n_rows == labels.size());
    m_data = new mat(features);
    m_labels = new Row<size_t>(labels.size());
    for (size_t i = 0; i < labels.size(); i++)
        (*m_labels)[i]  = (size_t) labels[i];
    m_mapper = new DatasetMapper<PolicyType>(info.Policy(), info.Dimensionality());
}

DataPatch::DataPatch(const DataPatch& data_patch) {
    if (data_patch.get_features() != NULL)
        m_data = new mat(*(data_patch.get_features()));
    else
        m_data = NULL;
    if (data_patch.get_labels() != NULL)
        m_labels = new Row<size_t>(*(data_patch.get_labels()));
    else
        m_labels = NULL;
    if (data_patch.get_mapper() != NULL) {
        DatasetMapper<PolicyType> info = *(data_patch.get_mapper());
        m_mapper = new DatasetMapper<PolicyType>(info.Policy(), info.Dimensionality());
    }
    else
        m_mapper = NULL;
}

DataPatch::~DataPatch() {
    if (m_data)
        delete m_data;
    m_data = NULL;
    if (m_labels)
        delete m_labels;
    m_labels = NULL;
    if (m_mapper)
        delete m_mapper;
    m_mapper = NULL;
}

void DataPatch::load_data(string file_path) {
    mat temp_data;
    DatasetMapper<PolicyType> temp_info;
    Load(file_path, temp_data, temp_info, true);
    m_data = new mat(temp_data);
    m_mapper = new DatasetMapper<PolicyType>(temp_info.Policy(),
                                 temp_info.Dimensionality());
}

void DataPatch::load_label(string file_path) {
    Col<size_t> temp_label;
    Load(file_path, temp_label, true, false);
    m_labels = new Row<size_t>(temp_label.t()); // transpose
}

void DataPatch::set_info(mlpack::data::DatasetMapper<PolicyType> info) {
    delete m_mapper;
    m_mapper = new DatasetMapper<PolicyType>(info.Policy(), info.Dimensionality());
}

mat *DataPatch::get_features() const {
    return m_data;
}

vec DataPatch::get_feature(int idx) const {
    return m_data->col(idx);
}

Row<size_t> *DataPatch::get_labels() const {
    return m_labels;
}

template<class T>
arma::Row<T> *DataPatch::get_labels_type() {
    arma::Row<T> *re = new arma::Row<T>(m_data->n_cols);
    for (int i = 0; i < (int) m_data->n_cols; i++)
        re->at(i) = (T) m_labels->at(i);
    return re;
}

DataPatch DataPatch::select_data(PlTerm selectors) {
    vector<uword> sel_vec = parse_selector(selectors);
    return select_data(sel_vec);
}

DataPatch DataPatch::select_data(vector<uword> selectors) {
    uvec v_sel(selectors);
    mat *temp_feat;
    if (m_data)
        temp_feat = new mat(m_data->cols(v_sel));
    else
        temp_feat = NULL;
    Row<size_t> *temp_lab;
    if (m_labels)
        temp_lab = new Row<size_t>(m_labels->cols(v_sel));
    else
        temp_lab = NULL;
    DataPatch re = DataPatch(temp_feat, temp_lab, *m_mapper);
    delete temp_feat;
    delete temp_lab;
    return re;
}

mlpack::data::DatasetMapper<PolicyType> *DataPatch::get_mapper() const {
    return m_mapper;
}

size_t DataPatch::size() {
    return (size_t) m_data->n_cols;
}

void DataPatch::print_data() {
    (*m_data).t().print("Data: ");
}

void DataPatch::print_labels() {
    m_labels->print("Label: ");
}

void DataPatch::save_data(string file_path) {
    Save(file_path, *m_data, true, true);
}

void DataPatch::save_labels(string file_path) {
    Save(file_path, *m_labels, true, true);
}


#endif
