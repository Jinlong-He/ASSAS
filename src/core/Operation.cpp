#include"DNSS/Operation.hpp"
namespace assas {
    ostream & operator<<( ostream & os, const Operation& o) {
        switch (o.beta) {
            case Beta::pop :
                cout << "pop";
                return os;
            case Beta::start :
                cout << "start_";
                break;
            case Beta::finish :
                cout << "finish_";
                break;
            case Beta::START :
                cout << "START_";
                break;
            case Beta::FINISH :
                cout << "FINISH_";
                break;
        }
        switch (o.type) {
            case Type::CTK :
                cout << "CTK(" << o.symbol << ")";
                break;
            case Type::CTP :
                cout << "CTP(" << o.symbol << ")";
                break;
            case Type::STP :
                cout << "STP(" << o.symbol << ")";
                break;
            case Type::RTF :
                cout << "RTF(" << o.symbol << ")";
                break;
            case Type::PUSH :
                cout << "PUSH(" << o.symbol << ")";
                break;
            case Type::LTK :
                cout << "LTK(" << o.symbol << ")";
                break;
        }
        return os;
    }
}

