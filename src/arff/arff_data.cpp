#include <string>
#include <set>
#include <map>

#include "arff_data.h"

ArffHierarchical::ArffHierarchical() : idx_map(),
                                       is_tree_(false) {
    items = (std::vector<std::string> *) NULL;
    adj_list = (boost::adjacency_list<> *) NULL;
}

ArffHierarchical::~ArffHierarchical() {
    if (this != NULL) {
        delete items;
        items = NULL;
        delete adj_list;
        adj_list = NULL;
    }
}

void ArffHierarchical::set_items(std::vector<std::string> nominal_list) {
    for (auto itr = nominal_list.begin(); itr != nominal_list.end(); ++itr) {
        int found_f = itr->find_first_of("/\\");
        int found_l = itr->find_last_of("/\\");
        if (found_f != found_l) {
            is_tree_ = true;
            break;
        }
}
    if (is_tree_)
        set_items_tree(nominal_list);
    else
        set_items_DAG(nominal_list);
}

void ArffHierarchical::set_items_tree(std::vector<std::string> nominal_list) {
    std::size_t idx = 0;
    items = new std::vector<std::string>(nominal_list.size());
    for (auto itr = nominal_list.begin(); itr != nominal_list.end(); ++itr) {
        (*items)[idx] = *itr;
        idx_map.insert(std::pair<std::string, std::size_t>(*itr, idx));
        idx++;
    }
    num_items = nominal_list.size();
}

void ArffHierarchical::set_items_DAG(std::vector<std::string> nominal_list) {
    // Because the DAG nominals are inputed by edges, 
    // use a set to prevent redundant
    std::set<std::string> item_set;
    for (auto itr = nominal_list.begin(); itr != nominal_list.end(); ++itr) {
        std::size_t found = itr->find_last_of("/\\");
        std::string father = itr->substr(0, found);
        std::string child = itr->substr(found);
        item_set.insert(father);
        item_set.insert(child);
    }
    // set items
    items = new std::vector<std::string>(item_set.size());
    std::size_t idx = 0;
    for (auto itr = item_set.begin(); itr != item_set.end(); ++itr) {
        (*items)[idx] = *itr;
        idx_map.insert(std::pair<std::string, std::size_t>(*itr, idx));
        idx++;
    }
    num_items = item_set.size();
}

void ArffHierarchical::parse_hierarchy() {
    adj_list = new boost::adjacency_list<>(items->size());
    if (is_tree_)
        parse_tree();
    else
        parse_DAG();
}

// tree style hierarchy
void ArffHierarchical::parse_tree() {
    // for label in "items" are father = "xxx/ooo", child = "xxx"
    for (auto itr = items->begin(); itr != items->end(); ++itr) {
        // split this string
        std::size_t found = itr->find_last_of("/\\");
        if (found != std::string::npos) {
            std::size_t idx = idx_map[*itr];
            std::string father = itr->substr(0, found);
            std::size_t f_idx = idx_map[father];
            //boost::add_edge(f_idx, idx, *adj_list);
            boost::add_edge(f_idx, idx, *adj_list);
        }
    }
}

// DAG style hierarchy
void ArffHierarchical::parse_DAG() {
    // for label in "items" are "father/children"
    for (auto itr = items->begin(); itr != items->end(); ++itr) {
        // split this string
        std::size_t found = itr->find_last_of("/\\");
        if (found != std::string::npos) {
            std::string father = itr->substr(0, found);
            std::string child = itr->substr(found);
            std::size_t f_idx = idx_map[father];
            std::size_t idx = idx_map[child];
            boost::add_edge(f_idx, idx, *adj_list);
        }
    }
}

int32 ArffHierarchical::number_of_items() {
    return num_items;
}

boost::adjacency_list<> *ArffHierarchical::get_adjacency_list() {
    return adj_list;
}

std::map<std::string, std::size_t> ArffHierarchical::get_idx_map() {
    return idx_map;
}

std::size_t ArffHierarchical::get_item_idx(std::string name) {
    return idx_map[name];
}

std::string ArffHierarchical::get_item(std::size_t idx) {
    return (*items)[idx];
}

std::vector<std::string> *ArffHierarchical::get_item_list() {
    return items;
}

bool ArffHierarchical::is_tree() {
    return is_tree_;
}

ArffData::ArffData(): m_rel(""),
                      m_nominals(),
                      m_hierarchicals(),
                      m_formats(),
                      m_num_attrs(0),
                      m_attrs(),
                      m_num_instances(0),
                      m_instances() {
}

ArffData::~ArffData() {
    if (this == NULL)
        return;
    for (auto itr = m_hierarchicals.begin(); itr != m_hierarchicals.end(); ++itr) {
        delete itr->second;
        itr->second = NULL;
    }
    m_hierarchicals.clear();
    for (auto itr = m_attrs.begin(); itr != m_attrs.end(); ++itr) {
        delete *itr;
        *itr = NULL;
    }
    for (auto itr = m_instances.begin(); itr != m_instances.end(); ++itr) {
        delete *itr;
        *itr = NULL;
    }
}

void ArffData::set_relation_name(const std::string& name) {
    m_rel = name;
}

std::string ArffData::get_relation_name() const {
    return m_rel;
}

int32 ArffData::num_attributes() const {
    return m_num_attrs;
}

void ArffData::add_attr(ArffAttr* attr) {
    m_attrs.push_back(attr);
    ++m_num_attrs;
}

ArffAttr* ArffData::get_attr(int32 idx) const {
    if((idx < 0) || (idx >= m_num_attrs)) {
        THROW("%s index out of bounds! idx=%d size=%d",
              "ArffData::get_attr", idx, m_num_attrs);
    }
    return m_attrs[idx];
}

int32 ArffData::num_instances() const {
    return m_num_instances;
}

void ArffData::add_instance(ArffInstance* inst) {
    _cross_check_instance(inst);
    m_instances.push_back(inst);
    ++m_num_instances;
}

ArffInstance* ArffData::get_instance(int32 idx) const {
    if((idx < 0) || (idx >= m_num_instances)) {
        THROW("%s index out of bounds! idx=%d size=%d",
              "ArffData::get_instance", idx, m_num_instances);
    }
    return m_instances[idx];
}

void ArffData::add_nominal_val(const std::string& name,
                               const std::string& val) {
    m_nominals[name].push_back(val);
}

ArffNominal ArffData::get_nominal(const std::string& name) {
    if(m_nominals.find(name) == m_nominals.end()) {
        THROW("ArffData::get_nominal list named '%s' does not exist!",
              name.c_str());
    }
    return m_nominals[name];
}

void ArffData::add_date_format(const std::string& name,
                               const std::string& val) {
    m_formats[name] = val;
}

std::string ArffData::get_date_format(const std::string& name) {
    if(m_formats.find(name) == m_formats.end()) {
        THROW("ArffData::get_date_format date named '%s' does not exist!",
              name.c_str());
    }
    return m_formats[name];
}

void ArffData::add_hierarchical_attr(const std::string& name, ArffHierarchical *attr) {
    m_hierarchicals[name] = attr;
}

ArffHierarchical *ArffData::get_hierarchical(const std::string& name) {
    if(m_hierarchicals.find(name) == m_hierarchicals.end()) {
        THROW("ArffData::get_date_format date named '%s' does not exist!",
              name.c_str());
    }
    return m_hierarchicals[name];
}

void ArffData::_cross_check_instance(ArffInstance* inst) {
    if(inst == NULL) {
        THROW("ArffData: input instance pointer is null!", 0);
    }
    if(inst->size() != m_num_attrs) {
        THROW("%s: instance size and num-attrs mismatch inst=%d attrs=%d",
              "ArffData", inst->size(), m_num_attrs);
    }
    for(int32 i = 0; i < m_num_attrs; ++i) {
        ArffValue *val = inst->get(i);
        ArffAttr *attr = m_attrs[i];
        ArffValueEnum valType = val->type();
        ArffValueEnum attType = attr->type();
        bool a_is_num = (attr->type() == NUMERIC);
        bool a_is_nom = (attr->type() == NOMINAL);
        bool a_is_hier = (attr->type() == H_NOMINAL);
        bool v_nan = ((valType != INTEGER) && (valType != FLOAT) &&
                      (valType != NUMERIC));
        bool v_nas = (valType != STRING);
        // bad numeric/nominal
        if ((a_is_num && v_nan) || (a_is_nom && v_nas)) {
            THROW("%s: attr-name=%s attr-type=%s, but inst-type=%s!",
                  "ArffData", attr->name().c_str(),
                  arff_value2str(attType).c_str(),
                  arff_value2str(valType).c_str());
        }
        // bad nominal value
        if (a_is_nom) {
            ArffNominal nom = get_nominal(attr->name());
            ArffNominal::iterator itr;
            std::string str = (std::string) *val;
            for (itr = nom.begin(); itr != nom.end(); ++itr) {
                if (str == *itr) {
                    break;
                }
            }
            if(itr == nom.end()) {
                THROW("%s: attr:(name=%s type=%s) inst-val=%s not found!",
                      "ArffData", attr->name().c_str(),
                      arff_value2str(attType).c_str(), str.c_str());
            }
        }
        if (a_is_hier) {
            ArffHierarchical *hier = get_hierarchical(attr->name());
            std::vector<std::string> vec = *val;
            auto h_idx_map = hier->get_idx_map();
            for (auto itr = vec.begin(); itr != vec.end(); ++itr) {
                if (h_idx_map.find(*itr) != h_idx_map.end())
                    continue;
                else {
                    THROW("%s: attr:(name=%s type=%s) inst-val=%s not found!",
                      "ArffData", attr->name().c_str(),
                          arff_value2str(attType).c_str(), "vector");
                }
            }
        }
        if (a_is_num || a_is_nom) {
            continue;
        }
        // data mismatch
        if(attType != valType) {
            THROW("%s: attr-name=%s attr-type=%s, but inst-type=%s!",
                  "ArffData", attr->name().c_str(),
                  arff_value2str(attType).c_str(),
                  arff_value2str(valType).c_str());
        }
        ///@todo: Check for date-format mismatch
    }
}

///@todo: implement the method write_arff
