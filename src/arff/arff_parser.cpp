#include <iostream>

#include "arff_parser.h"
#include "arff_token.h"

ArffParser::ArffParser(const std::string& _file) : m_lexer(NULL),
                                                   m_parsed(false),
                                                   m_data(NULL),
                                                   m_h_closed(true),
                                                   _previous(false) {
    m_lexer = new ArffLexer(_file);
}

ArffParser::~ArffParser() {
    if(m_lexer != NULL) {
        delete m_lexer;
        m_lexer = NULL;
    }
    if(m_data != NULL) {
        delete m_data;
        m_data = NULL;
    }
}

ArffData* ArffParser::parse() {
    if((ArffData*)NULL != m_data) {
        return m_data;
    }
    m_data = new ArffData();
    _read_relation();
    _read_attrs();
    _read_instances();
    m_parsed = true;
    return m_data;
}

void ArffParser::_read_relation() {
    // @relation
    ArffToken tok1 = m_lexer->next_token(_previous);
    if(tok1.token_enum() != RELATION) {
        THROW("%s: First token must be of 'RELATION'! It is '%s'",
              "ArffParser::_read_relation",
              arff_token2str(tok1.token_enum()).c_str());
    }
    // name
    ArffToken tok2 = m_lexer->next_token(_previous);
    if(tok2.token_enum() != VALUE_TOKEN) {
        THROW("%s: RELATION token must be followed by %s! It is '%s'",
              "ArffParser::_read_relation", "VALUE_TOKEN",
              arff_token2str(tok2.token_enum()).c_str());
    }
    m_data->set_relation_name(tok2.token_str());
}

void ArffParser::_read_attrs() {
    while(true) {
        // @attribute  (or @data)
        ArffToken tok = m_lexer->next_token(_previous);
        _previous = false;
        ArffTokenEnum type = tok.token_enum();
        if((type == DATA_TOKEN) || (type == END_OF_FILE)) {
            break;
        }
        if(type != ATTRIBUTE) {
            THROW("%s: First token must be of 'ATTRIBUTE'! It is '%s'",
                  "ArffParser::_read_attrs", arff_token2str(type).c_str());
        }
        _read_attr();
    }
}

void ArffParser::_read_attr() {
    // name
    ArffToken name = m_lexer->next_token(_previous);
    if(name.token_enum() != VALUE_TOKEN) {
        THROW("%s: 'ATTRIBUTE' must be followed by a '%s'! It is '%s'",
              "ArffParser::_read_attr", "VALUE_TOKEN",
              arff_token2str(name.token_enum()).c_str());
    }
    // type
    ArffToken type = m_lexer->next_token(_previous);
    ArffTokenEnum ate = type.token_enum();
    ArffValueEnum ave = UNKNOWN_VAL;
    switch(ate) {
    case NUMERIC_TOKEN:
        ave = NUMERIC;
        break;
    case STRING_TOKEN:
        ave = STRING;
        break;
    case DATE_TOKEN:
        ave = DATE;
        break;
    case BRKT_OPEN:
        ave = NOMINAL;
        break;
    case HIERARCHICAL:
        ave = H_NOMINAL;
        m_h_closed = false;
        //std::cout << "hierarchical start" << "\n";
        break;
    default:
        THROW("%s: Bad attribute type for name=%s attr-type=%s!",
              "ArffParser::_read_attr", name.token_str().c_str(),
              arff_token2str(ate).c_str());
    }
    m_data->add_attr(new ArffAttr(name.token_str(), ave));
    ///@todo: get the date-format
    if(ave != NOMINAL && ave != H_NOMINAL) {
        return;
    } else if (ave == NOMINAL) {
    // nominal type
        while(true) {
            ArffToken tok = m_lexer->next_token(_previous);
            //std::cout << tok.token_str() << "\n";
            if(tok.token_enum() == VALUE_TOKEN) {
                m_data->add_nominal_val(name.token_str(), tok.token_str());
                _previous = false;
            }
            else if(tok.token_enum() == BRKT_CLOSE) {
                _previous = false;
                break;
            }
            // @todo: if  @attribute or @data when hierarchical is on, break
            //else if(!m_h_closed && tok.token_enum() != VALUE_TOKEN) {
            //    m_h_closed = true;
            //    _previous = true;
            //    std::cout << "hierarchical end" << "\n";
            //    break;
            //}
            else {
                THROW("%s: For nominal values expecting '%s' got '%s' token!",
                      "ArffParser::_read_attr", "VALUE_TOKEN",
                      arff_token2str(tok.token_enum()).c_str());
                _previous = false;
            }
        }
    } else if (ave == H_NOMINAL) {
        // @todo: read a hierarchical label
        std::vector<std::string> item_set_;
        while(true) {
            ArffToken tok = m_lexer->next_token(_previous);
            //std::cout << tok.token_str() << "\n";
            if(tok.token_enum() == VALUE_TOKEN) {
                // ("class", "01/01/03")
                item_set_.push_back(tok.token_str());
                _previous = false;
            }
            // @todo: if  @attribute or @data when hierarchical is on, break
            else if(!m_h_closed && tok.token_enum() != VALUE_TOKEN) {
                m_h_closed = true;
                _previous = true;
                //std::cout << "hierarchical end" << "\n";
                break;
            }
            else {
                THROW("%s: For nominal values expecting '%s' got '%s' token!",
                      "ArffParser::_read_attr", "VALUE_TOKEN",
                      arff_token2str(tok.token_enum()).c_str());
                _previous = false;
            }
        }
        // do parsing
        ArffHierarchical *h_attr = new ArffHierarchical();
        (*h_attr).set_items(item_set_);
        (*h_attr).parse_hierarchy();// is tree
        //for (auto itr = item_set_.begin(); itr != item_set_.end(); ++itr)
        //    std::cout << *itr << "\n";
        m_data->add_hierarchical_attr(name.token_str(), h_attr);
    }
}

void ArffParser::_read_instances() {
    bool end_of_file = false;
    int32 num = m_data->num_attributes();
    while(!end_of_file) {
        ArffInstance* inst = new ArffInstance();
        for(int32 i = 0; i < num; ++i) {
            ArffToken tok = m_lexer->next_token(_previous);
            ArffTokenEnum type = tok.token_enum();
            ArffValueEnum aType = m_data->get_attr(i)->type();
            if(type == END_OF_FILE) {
                end_of_file = true;
                break;
            }
            if((type != VALUE_TOKEN) && (type != MISSING_TOKEN)) {
                THROW("%s expects '%s' or '%s', it is '%s'!",
                      "ArffParser::_read_instances", "VALUE_TOKEN",
                      "MISSING_TOKEN", arff_token2str(type).c_str());
            }
            if(type == MISSING_TOKEN) {
                inst->add(new ArffValue(aType));
            }
            else if(aType == NUMERIC) {
                inst->add(new ArffValue(tok.token_str(), true));
            }
            else if((aType == STRING) || (aType == NOMINAL)) {
                inst->add(new ArffValue(tok.token_str(), false));
            }
            else if((aType == H_NOMINAL)) {
                // @todo parse hierarchical nominal multi label
                //std::string attr_name = m_data->get_attr(i)->name();
                //ArffHierarchical h_attr = m_data->get_hierarchical(attr_name);
                std::vector<std::string> item_list = split_str(tok.token_str(), "@");
                inst->add(new ArffValue(item_list));
                item_list.clear();
                /*
                for (auto itr = item_list.begin(); itr != item_list.end(); ++itr) {
                    std::size_t item_idx = h_attr.get_item_idx(*itr);
                    std::cout << *itr << "\t" << item_idx << "\n";
                }
                */
       
       
            }
            else { // has to be DATE type!
                inst->add(new ArffValue(tok.token_str(), false, true));
            }
        }
        if(!end_of_file) {
            m_data->add_instance(inst);
        }
    }
}
