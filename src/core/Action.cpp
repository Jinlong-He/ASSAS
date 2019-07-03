#include"ASM/Action.hpp"
namespace assas {
    ID Action::counter = 0;
    void Action::setAlpha(Alpha a) {
        switch(a) {
            case finish:
                key = (key | 1);
                break;
            case start:
                key = (key & ~1);
                break;
        }
    }

    void Action::addFLAG(FLAG f) {
        switch(f) {
            case NTK :
                key = (key | 1 << 1);
                break;
            case CTK :
                key = (key | 1 << 2);
                break;
            case CTP :
                key = (key | 1 << 3);
                break;
            case STP :
                key = (key | 1 << 4);
                break;
            case RTF :
                key = (key | 1 << 5);
                break;
            case MTK :
                key = (key | 1 << 6);
                break;
            case TOH :
                key = (key | 1 << 7);
                break;
        }
    }

    Alpha Action::getAlpha() {
        if ((key & 1) == 1) {
            return finish;
        } else {
            return start;
        }
    }

    Alpha Action::getAlpha() const {
        if ((key & 1) == 1) {
            return finish;
        } else {
            return start;
        }
    }

    bool Action::hasNTK() const {
        if ((key & 1 << 1) == (1 << 1)) {
            return true;
        } else {
            return false;
        }
    }

    bool Action::hasCTK() const {
        if ((key & 1 << 2) == (1 << 2)) {
            return true;
        } else {
            return false;
        }
    }

    bool Action::hasCTP() const {
        if ((key & 1 << 3) == (1 << 3)) {
            return true;
        } else {
            return false;
        }
    }

    bool Action::hasSTP() const {
        if ((key & 1 << 4) == (1 << 4)) {
            return true;
        } else {
            return false;
        }
    }

    bool Action::hasRTF() const {
        if ((key & 1 << 5) == (1 << 5)) {
            return true;
        } else {
            return false;
        }
    }

    bool Action::hasMTK() const {
        if ((key & 1 << 6) == (1 << 6)) {
            return true;
        } else {
            return false;
        }
    }

    bool Action::hasTOH() const {
        if ((key & 1 << 7) == (1 << 7)) {
            return true;
        } else {
            return false;
        }
    }

    const FLAGs Action::getFLAGs() {
        FLAGs flags;
        getFLAGs(flags);
        return flags;
    }

    const FLAGs Action::getFLAGs() const {
        FLAGs flags;
        getFLAGs(flags);
        return flags;
    }

    void Action::getFLAGs(FLAGs& flags) const {
        if (hasNTK()) {
            flags.push_back(NTK);
        }
        if (hasCTK()) {
            flags.push_back(CTK);
        }
        if (hasCTP()) {
            flags.push_back(CTP);
        }
        if (hasSTP()) {
            flags.push_back(STP);
        }
        if (hasRTF()) {
            flags.push_back(RTF);
        }
        if (hasMTK()) {
            flags.push_back(MTK);
        }
        if (hasTOH()) {
            flags.push_back(TOH);
        }
    }

    ostream & operator<<( ostream & os, const Action& action) {
        if (action.getAlpha() == start) {
            cout << "start: ";
        } else {
            cout << "finishStart: ";
        }
        FLAGs flags;
        action.getFLAGs(flags);
        for (FLAG flag : flags) {
            switch (flag) {
                case NTK :
                    cout << "NTK ";
                    break;
                case CTK :
                    cout << "CTK ";
                    break;
                case CTP :
                    cout << "CTP ";
                    break;
                case STP:
                    cout << "STP ";
                    break;
                case RTF:
                    cout << "RTF ";
                    break;
                case MTK:
                    cout << "MTK ";
                    break;
                case TOH:
                    cout << "TOH ";
                    break;
            }
        }
        return os;
    }
}
