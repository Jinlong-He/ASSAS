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
    typedef unordered_set<Activity*> Acts;

    /// \breif Android Stak Model.
    class ASM {
    private:
        Acts activities;            ///< the Activity set in ASM.
        Activity* mainActivity;     ///< the main Activity in ASM.

    public:
        /// \brief Default construction function.
        ASM() : mainActivity(nullptr) {}

        /// \brief Default desconstruction function.
        ~ASM() {
            for (Activity* act : activities) {
                delete act;
            }
            mainActivity = nullptr;
        }

        Activity* mkActivity(const string& name, ID i,  const Aft& aft = "", Lmd lmd = lmd_std) {
            Activity* activity = new Activity(name, i, aft, lmd);
            activities.insert(activity);
            return activity;
        }

        Activity* mkMainActivity(const string& name, ID i,  const Aft& aft = "", Lmd lmd = lmd_std) {
            mainActivity = mkActivity(name, i, aft, lmd);
            return mainActivity;
        }
    };
}
#endif /* ASM_hpp */

