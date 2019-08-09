#include "Parse.hpp"
#include "DNSS/DNSS.hpp"
#include "ReachabilitySolver/BoundedReachabilitySolver.hpp"
using namespace assas;
unordered_set<ASSASObject*> ASSASManage::buffer;
int main(int argc, const char * argv[]) {
    ASM* a = new ASM();
    Parse parse(argv[1], argv[2], a);
    DNSS dnss(a);
    cout << dnss << endl;
    BoundedReachabilitySolver solver(&dnss, 5);
    solver.init();

    RegEx regEx1;
    regEx1.push_back(1);
    regEx1.push_back(2);
    regEx1.push_back(1);
    regEx1.push_back(2);
    RegEx regEx2;
    regEx2.push_back(3);
    regEx2.push_back(4);
    RegExMap map;
    map[1] = regEx1;
    map[2] = regEx2;
    cout << solver.isReachable(map);
    delete a;
}
