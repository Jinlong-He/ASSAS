#include "Parse.hpp"
#include "DNSS/DNSS.hpp"
#include "Solver/LengthBoundedSolver.hpp"
using namespace assas;
int main(int argc, const char * argv[]) {
    ASM* a = new ASM();
    Parse parse(argv[1], argv[2], a);
    DNSS dnss(a);
    cout << dnss << endl;
    LengthBoundedSolver solver(&dnss);
    Add2SymbolsMap initialSymbolMap, add2SymbolsMap;
    Add2OperationsMap add2OpsMap;
    solver.getInitialSymbolsMap(initialSymbolMap);
    solver.getReachableSymbolsAndOpsMap(initialSymbolMap, add2SymbolsMap, add2OpsMap);
    for (auto& mapPair : add2OpsMap) {
        cout << mapPair.first << endl;
        for (auto& mapPair1 : mapPair.second) {
            for (Operation* op : mapPair1.second) {
                cout << mapPair1.first << "->" << *op << endl;
            }
        }
    }
    delete a;
}
