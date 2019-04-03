#include <stdexcept>
#include <stdio.h>

#include "arff_utils.h"

#define STR_LENGTH 2048
void throw_ex(const char* file, int64 line, const char* fmt, ...) {
    char msg[STR_LENGTH];
    va_list va;
    va_start(va, fmt);
    vsprintf(msg, fmt, va);
    va_end(va);
    std::string err(file);
    err += ":" + num2str<int64>(line) + " -- ";
    err += msg;
    std::runtime_error ex(err);
    throw ex;
}
#undef STR_LENGTH

char to_lower(char c) {
    if((c >= 'A') && (c <= 'Z')) {
        return ((c - 'A') + 'a');
    }
    return c;
}

bool icompare(const std::string& str, const std::string& ref) {
    size_t s1 = str.size();
    size_t s2 = ref.size();
    if(s1 != s2) {
        return false;
    }
    const char* ch1 = str.c_str();
    const char* ch2 = ref.c_str();
    for(size_t i=0;i<s1;++i) {
        if(to_lower(ch1[i]) != to_lower(ch2[i])) {
            return false;
        }
    }
    return true;
}


std::vector<std::string> split_str(const std::string& s, const std::string& delim) {
    std::size_t last = 0;
    std::size_t index = s.find_first_of(delim, last);
    std::vector<std::string> ret;
	while (index!=std::string::npos) {
		ret.push_back(s.substr(last, index-last));
		last = index + 1;
		index = s.find_first_of(delim, last);
	}
	if (index - last > 0) {
		ret.push_back(s.substr(last, index-last));
	}
    return ret;
}

std::vector<std::string> split_h_str(const std::string& s, const std::string& delim) {
    std::vector<std::string> units = split_str(s, delim);
    std::vector<std::string> re;
    for (auto itr = units.begin(); itr != units.end(); ++itr) {
        std::string s = "";
        for (auto sub_itr = units.begin(); sub_itr != itr; ++sub_itr) {
            s = s + (*sub_itr) + "/";
        }
        s = s + *itr;
        re.push_back(s);
    }
    return re;
}
