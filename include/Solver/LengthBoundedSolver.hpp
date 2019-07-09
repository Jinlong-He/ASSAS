//
//  LengthBoundedSolver.hpp
//  Reach
//
//  Created by Jinlong He on 2019/6/23.
//  Copyright © 2019年 Jinlong He. All rights reserved.
//

#ifndef LengthBoundedSolver_hpp
#define LengthBoundedSolver_hpp

#include "ReachabilitySolver.hpp"

namespace assas {

    /// \brief LengthBoundedability Solver.
    class LengthBoundedSolver : public ReachabilitySolver {
    private:
        nuxmv::NuXmv nuxvm;                 ///< solve FST reachability.
        ID maxLength;                       ///< the maximal length of stack.
        Add2IDsMap availablePositions;      ///< the available positions for each address.

        void mkVirtualOpsMap(Add2SymbolsMap& add2SymbolsMap, Add2OperationsMap& add2OpsMap);
    public:
        void mkAvailablePositions();

        /// \brief Default construction function.
        LengthBoundedSolver() : ReachabilitySolver() {}

        /// \brief Construction function with params.
        /// \param s DNSS.
        LengthBoundedSolver(DNSS* s) : ReachabilitySolver(s) {}

        ~LengthBoundedSolver() {
        }

        Operation* mkOperation(Beta beta, Type type, Symbol symbol) {
            Operation* operation = new Operation(beta, type, symbol);
            Manage::manage(operation);
            return operation;
        }

        bool isReachable(const RegEx& regEx) {
            return true;
        }

    };
}

#endif /* LengthBoundedSolver_hpp */
