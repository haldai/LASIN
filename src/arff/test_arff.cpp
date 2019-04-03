#include "arff_parser.h"
#include "arff_data.h"

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/median.hpp>

#include <string>
#include <iostream>

typedef long int _size;

int main() {
    ArffParser parser("../../data/test/case5.arff");
    ArffData *data = parser.parse();
    /*
    _size dim = 10;
    std::vector<int> a = std::vector<int>(dim, (int) 0);
    for (auto itr = a.begin(); itr != a.end(); ++itr)
        std::cout << *itr << std::endl;
    ArffValue *val = data->get_instance(0)->get(4);
    std::vector<std::string> strs = *val;
    for (auto itr = strs.begin(); itr != strs.end(); ++itr)
        std::cout << *itr << std::endl;

    ArffHierarchical *h_attr = data->get_hierarchical("class");
    boost::adjacency_list<> *g = h_attr->get_adjacency_list();
    
    using namespace boost;
    graph_traits < adjacency_list <> >::vertex_iterator i, end;
    graph_traits < adjacency_list <> >::adjacency_iterator ai, a_end;
    property_map < adjacency_list <>, vertex_index_t >::type
        index_map = get(vertex_index, *g);

    for (boost::tie(i, end) = vertices(*g); i != end; ++i) {
        std::cout << h_attr->get_item(get(index_map, *i));
        boost::tie(ai, a_end) = adjacent_vertices(*i, *g);
        if (ai == a_end)
            std::cout << " has no children";
        else
            std::cout << " is the parent of ";
        for (; ai != a_end; ++ai) {
            std::cout << h_attr->get_item(get(index_map, *ai));
            if (boost::next(ai) != a_end)
                std::cout << ", ";
        }
        std::cout << std::endl;
    }

    std::vector<std::string> aa;
    std::cout << aa.size() << std::endl;
    std::cout << aa.empty() << std::endl;
    */
    
    using namespace boost::accumulators;
    accumulator_set< double, stats<tag::mean, tag::median(with_p_square_quantile)> > acc;
    acc(1.);
    acc(2.);
    acc(2.);
    acc(2.);
    acc(2.);
    acc(3.);
    acc(3.);
    std::cout << mean(acc) << "\n";
    std::cout << median(acc) << "\n";
    return 0;
}
