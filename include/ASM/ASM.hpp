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
    class ASM {
    private:
        Acts activities;            ///< the Activity set in ASM.
        Afts affinities;            ///< the Affinity set in ASM.
        Activity* mainActivity;     ///< the main Activity in ASM.

    public:
        /// \brief Default construction function.
        ASM() : mainActivity(nullptr) {}

        /// \brief Default desconstruction function.
        ~ASM() {
            for (Activity* act : activities) {
                delete act;
            }
            for (Affinity* aft : affinities) {
                delete aft;
            }
            mainActivity = nullptr;
        }

        /// \brief Makes Activity in ASM.
        /// \param name The Activity name.
        /// \param aft The Activity affinity.
        /// \param lmd The Activity launch mode.
        /// \return Pointer of Activity.
        Activity* mkActivity(const string& name, Affinity* aft, Lmd lmd =Lmd::STD) {
            Activity* activity = new Activity(name, aft, lmd);
            activities.insert(activity);
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
            affinities.insert(affinity);
            return affinity;
        }

        /// \brief Deletes param activity in ASM.
        /// \param activity To be deleted.
        void delActivity(Activity* activity) {
            activities.erase(activity);
            delete activity;
        }

        /// \brief Gets the activities.
        /// \return Reference of Acts.
        Acts& getActivities() {
            return activities;
        }
        const Acts& getActivities() const {
            return activities;
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

