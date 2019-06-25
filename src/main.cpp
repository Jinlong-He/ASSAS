#include "Parse.hpp"
using namespace assas;
int main(int argc, const char * argv[]) {
    ASM* a = new ASM();
    Parse parse(argv[1], argv[2], a);
    delete a;
}
