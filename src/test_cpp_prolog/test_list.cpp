#include <string>
#include <sstream>
#include <SWI-cpp.h>
#include <SWI-Prolog.h>

int main(int argc, char **argv) {
    PlEngine e(argv[0]);
    PlTermv av(1);
    PlTail l(av[0]);

    PlCall("consult('../prolog/test.pl').");

    for(int i=1; i<argc; i++) {
        double num;
        std::stringstream ss;
        ss << argv[i];
        ss >> num;
        l.append(num);
    }
    l.close();

    PlQuery q("un", av);
    return q.next_solution() ? 0 : 1;
}
