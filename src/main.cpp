#include "Parse.hpp"
#include "DNSS/DNSS.hpp"
#include "Solver/LengthBoundedSolver.hpp"
using namespace assas;
unordered_set<Object*> Manage::buffer;
int main(int argc, const char * argv[]) {
    ASM* a = new ASM();
    Parse parse(argv[1], argv[2], a);
    DNSS dnss(a);
    cout << dnss << endl;
    LengthBoundedSolver solver(&dnss);
    solver.mkAvailablePositions();
    delete a;
}
