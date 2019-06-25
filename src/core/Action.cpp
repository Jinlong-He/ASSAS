#include"Action.hpp"
namespace assas {

    void Action::setAlpha(Alpha a) {
        switch(a) {
            case finish:
                key = (key | 1);
            case start:
                key = (key & ~1);
        }
    }

    void Action::addFLAG(FLAG f) {
        switch(f) {
            case NTK :
                key = (key | 1 << 1);
            case CTK :
                key = (key | 1 << 2);
            case CTP :
                key = (key | 1 << 3);
            case STP :
                key = (key | 1 << 4);
            case RTF :
                key = (key | 1 << 5);
            case MTK :
                key = (key | 1 << 6);
            case TOH :
                key = (key | 1 << 7);
        }
    }

    Alpha Action::getAlpha() {
        if ((key & 1) == 1) {
            return finish;
        } else {
            return start;
        }
    }

    bool Action::hasNTK() {
        if ((key & 1 << 1) == (1 << 1)) {
            return true;
        } else {
            return false;
        }
    }

    bool Action::hasCTK() {
        if ((key & 1 << 2) == (1 << 2)) {
            return true;
        } else {
            return false;
        }
    }

    bool Action::hasCTP() {
        if ((key & 1 << 3) == (1 << 3)) {
            return true;
        } else {
            return false;
        }
    }

    bool Action::hasSTP() {
        if ((key & 1 << 4) == (1 << 4)) {
            return true;
        } else {
            return false;
        }
    }

    bool Action::hasRTF() {
        if ((key & 1 << 5) == (1 << 5)) {
            return true;
        } else {
            return false;
        }
    }

    bool Action::hasMTK() {
        if ((key & 1 << 6) == (1 << 6)) {
            return true;
        } else {
            return false;
        }
    }

    bool Action::hasTOH() {
        if ((key & 1 << 7) == (1 << 7)) {
            return true;
        } else {
            return false;
        }
    }

    const FLAGs Action::getFLAGs() {
        FLAGs fs;
        if (hasNTK()) {
            fs.push_back(NTK);
        }
        if (hasCTK()) {
            fs.push_back(CTK);
        }
        if (hasCTP()) {
            fs.push_back(CTP);
        }
        if (hasSTP()) {
            fs.push_back(STP);
        }
        if (hasRTF()) {
            fs.push_back(RTF);
        }
        if (hasMTK()) {
            fs.push_back(MTK);
        }
        if (hasTOH()) {
            fs.push_back(TOH);
        }
        return fs;
    }
}
