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
        activities.clear();
        affinities.clear();
        activities.insert(activities.end(), liveActs.begin(), liveActs.end());
        for (Activity* act : activities) {
            affinities.push_back(act -> getAft());
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
