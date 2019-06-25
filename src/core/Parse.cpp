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
        ID id = 2;
        ID aftId = 1;
        bool flag = false;
        string name = "";
        Lmd lmd;
        for (ID i = 0; i < strs.size(); i++) {
            if (strs[i] == "#MAIN") {
                flag = true;
                continue;
            } else if (strs[i] == "#NIAM") {
                flag = false;
                continue;
            } else {
                name = Utility::split(strs[i++], ":")[1];
                string lmdStr = Utility::split(strs[i], ":")[1];
                if (lmdStr == "std") {
                    lmd = lmd_std;
                } else if (lmdStr == "stk") {
                    lmd = lmd_stk;
                } else if (lmdStr == "stp") {
                    lmd = lmd_stp;
                } else if (lmdStr == "sit") {
                        lmd = lmd_sit;
                } else {
                    //writeErr(lmdError);
                }
            }
            Aft aft;
            string aftStr = "";
            vector<string> strVec = Utility::split(strs[++i], ":");
            if (strVec.size() > 1) {
                if (aftMap.count(strVec[1]) == 0)
                    aftMap[strVec[1]] = "b" + to_string(aftId++);
                aftStr = aftMap[strVec[1]]; 
            }
            if (aftStr.length() == 0 || aftStr == " ") {
                aft = "e" + to_string(aftId++);
            } else {
                aft = aftStr;
            }
            Activity* act = nullptr;
            if (flag) {
                act = a -> mkMainActivity(name, id++, aft, lmd);
            } else {
                act = a -> mkActivity(name, id++, aft, lmd);
            }
            actMap[name] = act;
        }
    }
}



//void Parse::readTransitionGragh(const string& fileName)
//{
//    bool main = false;
//    vector<string> strs;
//    string str;
//    ifstream file(fileName);
//    if(!file) return;
//    while(!file.eof())
//    {
//        getline(file, str);
//        if(!str.empty())
//            strs.push_back(str);
//    }
//    ID id = 1;
//    for(ID i = 0; i < strs.size(); i++)
//    {
//        vector<string> strVec = Utility::split(strs[i++], "-->");
//        Activity* sAct = nullptr;
//        Activity* tAct = nullptr;
//        for (auto& mapPair : str2ActMap) {
//            if (mapPair.first.find(strVec[0]) != string::npos)
//                sAct = mapPair.second;
//            if (mapPair.first.find(strVec[1]) != string::npos)
//                tAct = mapPair.second;
//        }
//        if (!(sAct && tAct)) {
//            continue;
//            writeErr(unmatchError);
//        }
//        if (sAct == mainActivity)
//            main = true;
//        strVec.clear();
//        if(strs[i].find(':') != strs[i].size() - 1)
//            strVec = Utility::split(Utility::split(strs[i], ":")[1], " ");
//        Flags flags;
//        bool finish = false;
//        for(string str : strVec)
//        {
//            if(str.find("FLAG_ACTIVITY_NEW_TASK") != -1)
//                flags.push_back(NTK);
//            else if(str.find("FLAG_ACTIVITY_CLEAR_TASK") != -1)
//                flags.push_back(CTK);
//            else if(str.find("FLAG_ACTIVITY_CLEAR_TOP") != -1)
//                flags.push_back(CTP);
//            else if(str.find("FLAG_ACTIVITY_REORDER_TO_FRONT") != -1)
//                flags.push_back(RTF);
//            else if(str.find("FLAG_ACTIVITY_SINGLE_TOP") != -1)
//                flags.push_back(STP);
//            else if(str.find("FLAG_ACTIVITY_MULTIPLE_TASK") != -1)
//                flags.push_back(MTK);
//            else if(str.find("FLAG_ACTIVITY_TASK_ON_HOME") != -1)
//                flags.push_back(TOH);
//            else if(str.find("finish") != -1)
//                finish = true;
//            else continue;
//        }
//        tAct -> addDegree();
//        tAct -> addInDegree();
//        Action* action = new Action(sAct, tAct, flags, finish, id++);
//        actions.insert(action);
//        sAct -> addAction(action);
//    }
//    if (!main) writeErr(noMainError);
//    
//    file.close();
//    RmvUselessTrans();
//}
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
//void Parse::writeErr(int error) {
//    ofstream errorfile;
//    errorfile.open("error.txt", ios::app);
//    errorfile << infoFileName << endl;
//    if (error == lmError) {
//        errorfile << "lunchMode error" << endl;
//        errorfile.close();
//        exit(1);
//    } else if (error == unmatchError) {
//        errorfile << "unmatch error" << endl;
//        errorfile.close();
//        exit(2);
//    } else if (error == noMainError) {
//        errorfile << "noMain error" << endl;
//        errorfile.close();
//        exit(3);
//    }
//}
