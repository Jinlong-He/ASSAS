#include "Parse.hpp"
#include "DNSS/DNSS.hpp"
#include "ReachabilitySolver/BoundedReachabilitySolver.hpp"
using namespace assas;
unordered_set<Object*> Manage::buffer;
int main(int argc, const char * argv[]) {
    ASM* a = new ASM();
    Parse parse(argv[1], argv[2], a);
    DNSS dnss(a);
    cout << dnss << endl;
    BoundedReachabilitySolver solver(&dnss, 5);
    solver.init();
    delete a;
}
