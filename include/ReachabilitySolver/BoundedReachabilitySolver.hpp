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
        Transition2IDMap transMap;
        IDPair2IDMap varMap;
        Orders orders;
        Symbols alphabet;

        void mkVirtualOpsMap(Add2SymbolsMap& add2SymbolsMap, Add2OperationsMap& add2OpsMap);

        void mkAvailablePositions(Add2SymbolsMap& initialSymbolsMap, Add2SymbolsMap& add2SymbolsMap, Add2OperationsMap& add2OpsMap);

        ConjunctionExp* mkTopStackExp(Address add, ID pos, Symbol symbol);
        DisjunctionExp* mkTopStackExp(Address add, Symbol symbol);
        ConjunctionExp* mkTargetTopStackExp(Address add, ID pos);
        ConjunctionExp* mkTargetTopStackExp(Address add, ID pos, Symbol symbol);
        DisjunctionExp* mkTopOrderExp(Address add);
        DisjunctionExp* mkTopOrderExp(Address sAdd, Address tAdd);
        ConjunctionExp* mkTopExp(Address add, ID pos, Symbol symbol, Expression* expression);

        ConjunctionExp* mkCheckSymbolExp(Address add, ID pos, ID topID, Symbol symbol, Expression* expression);

        ConjunctionExp* mkCheckNegSymbolExp(Address add, Symbol symbol, ID topID, Expression* expression);


        void mkLchTsk(Address add, Symbol symbol, Address targetAdd, Symbol targetSymbol, Expression* expression);
        void mkPush(Address add, ID pos, Symbol symbol, Expression* exp, bool finish = false);
        void mkMv2Top(Address add, ID pos, Symbol symbol, Expression* exp);
        void mkClrTop(Address add, ID pos, Symbol symbol, Expression* exp);
        void mkClrTsk(Address add, ID pos, Symbol symbol, Expression* exp);

        void mkTrans(Address add, Symbol symbol, Operation* operation);
        void mkSuperTrans(Address add, Symbol symbol, Operation* operation);
        void mkInitialTrans();

        Var* getSymbolVar(Address add, ID pos) {
            return fst -> getVars()[varMap[IDPair(add, pos)]];
        }

        Var* getOrderVar() {
            return fst -> getVars()[1];
        }

        Var* getVar() {
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

        Value* getTransValue(Transition& trans) {
            return fst -> getValues()[transMap[trans]];
        }

        void mkValues(Add2SymbolsMap& add2SymbolsMap, ID2ValuesMap& id2ValuesMap, Values& orderValues, Values& transValues);

        Operation* mkOperation(Beta beta, Type type, Symbol symbol) {
            Operation* operation = new Operation(beta, type, symbol);
            ASSASManage::manage(operation);
            return operation;
        }

        bool checkWord(Address add, Word& word);
        Expression* mkContainsExp(DFA<Symbol>& dfa);
        Expression* mkStackExp(Address add, Word& word);
        void getWords(DFA<Symbol>& dfa, Words& words);

        Expression* mkReachExp(DFAMap& dfaMap);

    public:
        void init();

        /// \brief Default construction function.
        BoundedReachabilitySolver() : ReachabilitySolver(), fst(nullptr) {}

        /// \brief Construction function with params.
        /// \param s DNSS.
        BoundedReachabilitySolver(DNSS* s, ID h = MAX_HEIGHT) : ReachabilitySolver(s), fst(nullptr), stackHeight(h), stackNumber(s -> getAddresses().size()), alphabet(s -> getAlphabet()) {
            //Symbol symbol = alphabet.size();
            //alphabet.insert(symbol);
            //alphabet.insert(symbol);
        }

        /// \brief Desconstruction function and deletes fst.
        ~BoundedReachabilitySolver() {
            delete fst;
        }

        bool isReachable(const RegEx& regEx) {
            return true;
        }

        bool isReachable(const RegExMap& regExMap);

        bool contains(const RegEx& regEx);

        void getPath(const string& terminal);

        void verify();

    };
}

#endif /* BoundedReachabilitySolver_hpp */
