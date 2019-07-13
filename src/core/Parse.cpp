//
//  Parse.cpp
//  ASM
//
//  Created by 何锦龙 on 2018/8/22.
//  Copyright © 2018年 何锦龙. All rights reserved.
//

#include "Parse.hpp"

namespace assas {
    void readFile(const string& fileName, strings& strs) {
        string str;
        ifstream file(fileName);
        if (!file) return;
        while (!file.eof()) {
            getline(file, str);
            if(!str.empty())
                strs.push_back(str);
        }
        file.close();
    }

    void Parse::readInfoFile(const string& fileName) {
        vector<string> strs;
        readFile(fileName, strs);
        bool flag = false;
        string name = "";
        string aftName = "";
        Lmd lmd;
        for (ID i = 0; i < strs.size(); i++) {
            if (strs[i] == "#MAIN") {
                flag = true;
                continue;
            } else if (strs[i] == "NIAM#") {
                flag = false;
                continue;
            } else {
                name = Utility::split(strs[i++], ":")[1];
                string lmdStr = Utility::split(strs[i], ":")[1];
                if (lmdStr == "std") {
                    lmd = Lmd::STD;
                } else if (lmdStr == "stk") {
                    lmd = Lmd::STK;
                } else if (lmdStr == "stp") {
                    lmd = Lmd::STP;
                } else if (lmdStr == "sit") {
                    lmd = Lmd::SIT;
                } else {
                    writeErr(lmdError);
                }
            }
            Affinity* aft = nullptr;
            vector<string> strVec = Utility::split(strs[++i], ":");
            if (strVec.size() > 1) {
                aftName = strVec[1];
            } else {
                aftName = " ";
            }
            if (aftName == " ") {
                aft = a -> mkAffinity(aftName);
            } else {
                aft = aftMap[aftName];
                if (aft == nullptr) {
                    aft = a -> mkAffinity(aftName);
                    aftMap[aftName] = aft;
                }
            }
            Activity* act = nullptr;
            if (flag) {
                act = a -> mkMainActivity(name, aft, lmd);
            } else {
                act = a -> mkActivity(name, aft, lmd);
            }
            actMap[name] = act;
        }
    }

    void Parse::writeErr(Error error) {
        delete a;
        switch (error) {
            case lmdError :
                cout << "lmdError" << endl;
                exit(1);
            case aftError :
                cout << "aftError" << endl;
                exit(1);
            case unmatchError :
                cout << "unmatchError" << endl;
                exit(1);
            case noMainError :
                cout << "noMainError" << endl;
                exit(1);
        }
    }

    void Parse::readTransitionGragh(const string& fileName) {
        vector<string> strs;
        readFile(fileName, strs);
        for (ID i = 0; i < strs.size(); i++) {
            vector<string> strVec = Utility::split(strs[i++], "-->");
            Activity* sAct = actMap[strVec[0]];
            Activity* tAct = actMap[strVec[1]];
            if (!(sAct && tAct)) {
                writeErr(unmatchError);
            }
            strVec.clear();
            if (strs[i].find(':') != strs[i].size() - 1)
                strVec = Utility::split(Utility::split(strs[i], ":")[1], " ");
            FLAGs flags;
            Alpha alpha = start;
            for (auto& str : strVec) {
                if(str.find("FLAG_ACTIVITY_NEW_TASK") != -1) {
                    flags.push_back(NTK);
                } else if(str.find("FLAG_ACTIVITY_CLEAR_TASK") != -1) {
                    flags.push_back(CTK);
                } else if(str.find("FLAG_ACTIVITY_CLEAR_TOP") != -1) {
                    flags.push_back(CTP);
                } else if(str.find("FLAG_ACTIVITY_REORDER_TO_FRONT") != -1) {
                    flags.push_back(RTF);
                } else if(str.find("FLAG_ACTIVITY_SINGLE_TOP") != -1) {
                    flags.push_back(STP);
                } else if(str.find("FLAG_ACTIVITY_MULTIPLE_TASK") != -1) {
                    flags.push_back(MTK);
                } else if(str.find("FLAG_ACTIVITY_TASK_ON_HOME") != -1) {
                    flags.push_back(TOH);
                } else if(str.find("finish") != -1) {
                    alpha = finish;
                } else {
                    continue;
                }
            }
            sAct -> addLaunchActivity(tAct, flags, alpha);
        }
        if (a -> getMainActivity() -> getLaunchMap().size() == 0) {
            writeErr(noMainError);
        }
    }
}




//
//void Parse::getClosure(Activity* act, Acts& visited) {
//    for (Action* action : act -> getActions()) {
//        Activity* newAct = action -> getTargetAct();
//        if (visited.insert(newAct).second)
//            getClosure(newAct, visited);
//    }
//}
//
//void Parse::RmvUselessTrans() {
//    Acts visited;
//    visited.insert(mainActivity);
//    getClosure(mainActivity, visited);
//    Acts delActs;
//    for (Activity* act : activities) {
//        if (visited.count(act) == 0)
//            delActs.insert(act);
//    }
//    for (Activity* act : delActs) {
//        for (Action* action : act -> getActions()) {
//            Activity* tAct = action -> getTargetAct();
//            if (tAct) tAct -> delInDegree();
//            actions.erase(action);
//            delete action;
//        }
//        activities.erase(act);
//        delete act;
//    }
//    Afts liveAfts;
//    for (Aft aft : afts) {
//        for (Activity* act : activities) {
//            if (aft == act -> getAft())
//                liveAfts.insert(aft);
//        }
//    }
//    afts.clear();
//    afts.insert(liveAfts.begin(), liveAfts.end());
//}
//

