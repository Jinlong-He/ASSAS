//
//  BoundedReachabilitySolver.hpp
//  Reach
//
//  Created by Jinlong He on 2019/6/23.
//  Copyright © 2019年 Jinlong He. All rights reserved.
//

#ifndef BoundedReachabilitySolver_hpp
#define BoundedReachabilitySolver_hpp

#include "ReachabilitySolver.hpp"
namespace assas {

    /// \brief BoundedReachabilityability Solver.
    class BoundedReachabilitySolver : public ReachabilitySolver {
    private:
        nuxmvfst::NuXmvFST* fst;            ///< solve FST reachability.
        ID stackHeight;                     ///< the height of stack.
        ID stackNumber;                     ///< the number of stack.
        Add2IDsMap availablePositions;      ///< the available positions for each address.
        Order2IDMap orderMap;               
        Orders orders;

        void mkVirtualOpsMap(Add2SymbolsMap& add2SymbolsMap, Add2OperationsMap& add2OpsMap);

        void mkAvailablePositions(Add2SymbolsMap& initialSymbolsMap, Add2SymbolsMap& add2SymbolsMap, Add2OperationsMap& add2OpsMap);

        ConjunctionExp* mkTopExp(ID sID, ID pos, Symbol symbol);

        ConjunctionExp* mkCheckExp(Address add, ID pos, ID topID, Symbol symbol, ConjunctionExp* expression);

        void mkTrans(Address add, Symbol symbol, Operation* operation);

        Var* getSymbolVar(ID sID, ID pos) {
            return fst -> getVars()[(sID - 1) * stackHeight + pos];
        }

        Var* getOrderVar() {
            return fst -> getVars()[0];
        }

        Value* getSymbolValue(Symbol symbol) {
            return fst -> getValues()[symbol];
        }

        Value* getNullValue() {
            return fst -> getValues()[0];
        }

        Value* getOrderValue(Order& o) {
            return fst -> getValues()[orderMap[o]];
        }

        void mkValues(Add2SymbolsMap& add2SymbolsMap, ID2ValuesMap& id2ValuesMap);
    public:

        void init();

        /// \brief Default construction function.
        BoundedReachabilitySolver() : ReachabilitySolver(), fst(nullptr) {}

        /// \brief Construction function with params.
        /// \param s DNSS.
        BoundedReachabilitySolver(DNSS* s, ID h = MAX_HEIGHT) : ReachabilitySolver(s), fst(nullptr), stackHeight(h), stackNumber(s -> getAddresses().size()) {}

        /// \brief Desconstruction function and deletes fst.
        ~BoundedReachabilitySolver() {
            delete fst;
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

#endif /* BoundedReachabilitySolver_hpp */
