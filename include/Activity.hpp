//
//  Activity.hpp
//  ASM
//
//  Created by Jinlong He on 2019/6/23.
//  Copyright © 2019年 Jinlong He. All rights reserved.
//

#ifndef Activity_hpp
#define Activity_hpp

#include "Action.hpp"

namespace assas {
    class Activity;
    class Action;
    enum Lmd {lmd_std, lmd_stk, lmd_stp, lmd_sit};
    typedef string Aft;
    typedef unordered_set<Activity*> Acts;
    typedef unordered_map<Activity*, Actions> LaunchMap;


    /// \breif Activity in the Android system.
    class Activity {
    private:
        ID id;                      ///< the uniqe identity of this Activity.
        Aft affinity;               ///< the afinity attribute for this Activity.
        Lmd launchMode;             ///< the launch mode for this Activity.
        string activityName;        ///< the name for this Activity.
        LaunchMap launchMap;        ///< the launch map for this Activity.

    public:
        /// \brief Default construction function.
        Activity() {}
        
        /// \brief Default desconstruction function.
        ~Activity() {
            for (auto& mapPair : launchMap) {
                for (Action* action : mapPair.second) {
                    delete action;
                }
            }
        }

        /// \brief Construction function with params.
        /// \param name The name for this Activity.
        /// \param i The identity for this Activity.
        /// \param lmd The lunch mode for this Activity. default is lmd_std.
        /// \param aft The affinity for this Activity. default is "".
        Activity(const string& name, ID i,  const Aft& aft = "", Lmd lmd = lmd_std) : id(i),  affinity(aft), launchMode(lmd), activityName(name) {}
        
        /// \brief Gets the id for this Activity.
        /// \return ID.
        ID getID() {
            return id;
        }

        /// \brief Gets the affinity for this Activity.
        /// \return Aft.
        Aft& getAft() {
            return affinity;
        }

        /// \brief Gets the launch mode for this Activity.
        /// \return Lmd.
        Lmd getLmd() {
            return launchMode;
        }

        /// \brief Gets the name for this Activity.
        /// \return string.
        string& getName() {
            return activityName;
        }

        /// \brief Gets the launch map fot this Activity.
        /// \return reference of LaunchMap.
        LaunchMap& getLaunchMap() {
            return launchMap;
        }

        /// \brief Gets the launch activities for this Activity.
        /// \return const reference of Acts.
        const Acts getLaunchActs() {
            Acts acts;
            for (auto& mapPair : launchMap) {
                acts.insert(mapPair.first);
            }
            return acts;
        }

        /// \brief Adds the Launch Activity for this Activity.
        /// \param act Activity to be lauched from this Activity.
        void AddLaunchActivity(Activity* act) {
            Action* action = new Action();
            launchMap[act].insert(action);
        }

        void AddLaunchActivity(Activity* act, const FLAGs& fs, Alpha a = start) {
            Action* action = new Action(fs, a);
            launchMap[act].insert(action);
        }

        void output() {
            cout << activityName << " " << launchMode << " " << affinity << endl;
        }

    };
}

#endif /* Activity_hpp */
