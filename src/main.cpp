#include "Parse.hpp"
#include "DNSS/DNSS.hpp"
using namespace assas;
int main(int argc, const char * argv[]) {
    ASM* a = new ASM();
    Parse parse(argv[1], argv[2], a);
    DNSS dnss(a);
    cout << dnss << endl;
    delete a;
}
