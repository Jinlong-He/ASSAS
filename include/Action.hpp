//
//  Action.hpp
//  ASM
//
//  Created by Jinlong He on 2019/6/23.
//  Copyright © 2019年 Jinlong He. All rights reserved.
//

#ifndef Action_hpp
#define Action_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <math.h>

using namespace std;

namespace assas {
    class Action;
    enum FLAG {CTK, CTP, NTK, RTF, STP, MTK, TOH};
    enum Alpha {start, finish};
    typedef size_t ID;
    typedef vector<FLAG> FLAGs;
    typedef unordered_set<Action*> Actions;

    /// \breif Action in the Android system.
    class Action {
    private:
        ID key;
    public:
        /// \brief Default construction function.
        Action() : key(0) {}
        
        /// \brief Default desconstruction function.
        ~Action() {}

        /// \brief Construction function with params.
        /// \param fs The FLAGs for this Action.
        /// \param a The method to launch activity for this Action.
        Action(const FLAGs& fs, Alpha a = start) : key(0) {
            setAlpha(a);
            for (FLAG f : fs) {
                addFLAG(f);
            }
        }

        /// \brief Sets the Alpha in key.
        /// \param a The method to launch activity for this Action.
        void setAlpha(Alpha a);

        /// \brief Sets the FLAG in key.
        /// \param f The FLAG for this Action.
        void addFLAG(FLAG f);
        
        /// \brief Gets the Alpha for this Action.
        /// \return Alpha.
        Alpha getAlpha();

        /// \brief Gets the FLAGs for this Action.
        /// \return FLAGs.
        const FLAGs getFLAGs();

        /// \brief Decides there exists given FALG in this Action.
        /// return Boolean.
        bool hasNTK();
        bool hasCTK();
        bool hasCTP();
        bool hasSTP();
        bool hasRTF();
        bool hasMTK();
        bool hasTOH();

        void output() {
        }

    };
}

#endif /* Action_hpp */
