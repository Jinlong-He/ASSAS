//
//  ASM.hpp
//  ASM
//
//  Created by 何锦龙 on 2018/8/22.
//  Copyright © 2018年 何锦龙. All rights reserved.
//

#ifndef ASM_hpp
#define ASM_hpp

#include "Activity.hpp"

namespace assas {

    /// \breif Android Stack Machine.
    class ASM : public ASSASObject {
    private:
        Activities activities;            ///< the Activity set in ASM.
        Affinities affinities;            ///< the Affinity set in ASM.
        Activity* mainActivity;           ///< the main Activity in ASM.

    public:
        /// \brief Default construction function.
        ASM() : mainActivity(nullptr) {}

        /// \brief Default desconstruction function.
        ~ASM() {
        }

        /// \brief Makes Activity in ASM.
        /// \param name The Activity name.
        /// \param aft The Activity affinity.
        /// \param lmd The Activity launch mode.
        /// \return Pointer of Activity.
        Activity* mkActivity(const string& name, Affinity* aft, Lmd lmd =Lmd::STD) {
            Activity* activity = new Activity(name, aft, lmd);
            activities.push_back(activity);
            ASSASManage::manage(activity);
            return activity;
        }

        /// \brief Makes Main Activity in ASM.
        /// \param name The Activity name.
        /// \param aft The Activity affinity.
        /// \param lmd The Activity launch mode.
        /// \return Pointer of Activity.
        Activity* mkMainActivity(const string& name, Affinity* aft, Lmd lmd = Lmd::STD) {
            mainActivity = mkActivity(name, aft, lmd);
            return mainActivity;
        }

        /// \brief Makes Affinity in ASM.
        /// \param name The name of Affinity.
        Affinity* mkAffinity(const string& name) {
            Affinity* affinity = new Affinity(name);
            affinities.push_back(affinity);
            ASSASManage::manage(affinity);
            return affinity;
        }

        /// \brief Gets the activities.
        /// \return Reference of Acts.
        Activities& getActivities() {
            return activities;
        }

        const Activities& getActivities() const {
            return activities;
        }

        /// \brief Gets the affinities.
        /// \return Reference of Acts.
        Affinities& getAffinities() {
            return affinities;
        }

        const Affinities& getAffinities() const {
            return affinities;
        }

        /// \brief Gets the Main Activity.
        /// \return Pointer of Activity.
        Activity* getMainActivity() {
            return mainActivity;
        }

        /// \brief Removes the useless Activities.
        void rmUselessActivities();

        /// \brief Overwrites <<.
        friend ostream & operator<<( ostream & os, const ASM& a);
    };
}
#endif /* ASM_hpp */

