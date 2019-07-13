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
    class Affinity;
    class Action;
    class ASM;

    /// \brief Affinity in the Android system.
    class Affinity : public Object {
    private:
        string affinityName;        ///< the name of this Aft.
    public:
        /// \brief Default Construction function.
        Affinity() {}

        /// \brief Construction function with param.
        /// \param name Affinity name.
        Affinity(const string& name) : affinityName(name) {}

        /// \brief Desconstruction function.
        ~Affinity() {}

        /// \brief Gets affinity name.
        /// \return String.
        const string& getName() const {
            return affinityName;
        }
        string& getName() {
            return affinityName;
        }

        friend ostream & operator<<( ostream & os, const Affinity& affinity) {
            cout << affinity.affinityName;
            return os;
        }
    };

    /// \brief Activity in the Android system.
    class Activity : public Object {
    private:
        Affinity* affinity;         ///< the afinity attribute for this Activity.
        Lmd launchMode;             ///< the launch mode for this Activity.
        string activityName;        ///< the name for this Activity.
        LaunchMap launchMap;        ///< the launch map for this Activity.

        void getLaunchActs(Acts& acts) {
            for (auto& mapPair : launchMap) {
                acts.insert(mapPair.first);
            }
        }

    public:
        /// \brief Default construction function.
        Activity() : affinity(nullptr) {}
        
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
        /// \param lmd The lunch mode for this Activity. default is STD.
        /// \param aft The affinity for this Activity. default is "".
        Activity(const string& name,  Affinity* aft, Lmd lmd = Lmd::STD) : affinity(aft), launchMode(lmd), activityName(name) {}
        
        /// \brief Gets the affinity for this Activity.
        /// \return Pointer of Affinity.
        Affinity* getAft() const {
            return affinity;
        }

        /// \brief Gets the launch mode for this Activity.
        /// \return Lmd.
        Lmd getLmd() const {
            return launchMode;
        }

        /// \brief Gets the name for this Activity.
        /// \return string.
        const string& getName() const {
            return activityName;
        }

        string& getName() {
            return activityName;
        }

        /// \brief Gets the launch map fot this Activity.
        /// \return reference of LaunchMap.
        const LaunchMap& getLaunchMap() const {
            return launchMap;
        }

        LaunchMap& getLaunchMap() {
            return launchMap;
        }

        /// \brief Gets the launch activities for this Activity.
        /// \return const reference of Acts.
        const Acts getLaunchActs() {
            Acts acts;
            getLaunchActs(acts);
            return acts;
        }

        /// \brief Adds the Launch Activity for this Activity.
        /// \param act Activity to be lauched from this Activity.
        void addLaunchActivity(Activity* act) {
            Action* action = new Action();
            launchMap[act].insert(action);
        }
        void addLaunchActivity(Activity* act, const FLAGs& fs, Alpha a = start) {
            Action* action = new Action(fs, a);
            launchMap[act].insert(action);
        }

        /// \brief Overwrites <<.
        friend ostream & operator<<( ostream & os, const Activity& activity);

        friend ASM;

    };
}

#endif /* Activity_hpp */
