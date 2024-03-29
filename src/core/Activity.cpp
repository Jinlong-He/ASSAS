#include "ASM/Activity.hpp"
namespace assas {
    ostream & operator<<( ostream & os, const Activity& activity) {
        string lmdStr = "";
        switch (activity.launchMode) {
            case Lmd::STD:
                lmdStr = "std";
                break;
            case Lmd::STK:
                lmdStr = "stk";
                break;
            case Lmd::STP:
                lmdStr = "stp";
                break;
            case Lmd::SIT:
                lmdStr = "sit";
                break;
        }
        cout << activity.activityName << "(" << lmdStr << ", " << *(activity.affinity) << ")";
        return os;
    }
}
