#include "ASM/ASM.hpp"

namespace assas {
    void ASM::rmUselessActivities() {
        Acts work, newWork, liveActs, acts;
        work.insert(mainActivity);
        liveActs.insert(mainActivity);
        while (work.size() > 0) {
            for (Activity* act : work) {
                acts.clear();
                act -> getLaunchActs(acts);
                for (Activity* newAct : acts) {
                    if (liveActs.insert(newAct).second) {
                        newWork.insert(newAct);
                    }
                }
            }
            work.clear();
            if (newWork.size() > 0) {
                work.insert(newWork.begin(), newWork.end());
            }
            newWork.clear();
        }
        if (liveActs.size() == activities.size()) return;
        Acts delActs;
        for (Activity* act : activities) {
            if (liveActs.count(act) == 0) {
                delActs.insert(act);
            }
        }
        for (Activity* act : delActs) {
            delActivity(act);
        }
    }

    ostream & operator<<( ostream & os, const ASM& a) {
        for (Activity* act : a.getActivities()) {
            for (auto& mapPair : act -> getLaunchMap()) {
                for (Action* action : mapPair.second) {
                    cout << *act << "-->";
                    cout << *(mapPair.first) << endl;
                    cout << *action << endl;
                }
            }
        }
        return os;
    }
}
