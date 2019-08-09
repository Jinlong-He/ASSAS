//
//  Action.hpp
//  ASM
//
//  Created by Jinlong He on 2019/6/23.
//  Copyright © 2019年 Jinlong He. All rights reserved.
//

#ifndef Action_hpp
#define Action_hpp

#include "../ASSASObject.hpp"

using namespace std;

namespace assas {

    /// \breif Action in the Android system.
    class Action : public ASSASObject {
    private:
        ID key;                 ///< the key for Action's flags and alpha.
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
        Alpha getAlpha() const;

        /// \brief Gets the FLAGs for this Action.
        /// \return FLAGs.
        const FLAGs getFLAGs();
        const FLAGs getFLAGs() const;
        void getFLAGs(FLAGs& flags) const;

        /// \brief Decides there exists given FALG in this Action.
        /// return Boolean.
        bool hasNTK() const;
        bool hasCTK() const;
        bool hasCTP() const;
        bool hasSTP() const;
        bool hasRTF() const;
        bool hasMTK() const;
        bool hasTOH() const;

        /// \brief Overwrites <<.
        friend ostream & operator<<( ostream & os, const Action& action);
        
    };
}

#endif /* Action_hpp */
