#include "ReachabilitySolver/BoundedReachabilitySolver.hpp"
ID nuxmvfst::Var::counter = 0;
ID nuxmvfst::Value::counter = 0;
namespace assas {
    void BoundedReachabilitySolver::mkAvailablePositions(Add2SymbolsMap& initialSymbolsMap, Add2SymbolsMap& add2SymbolsMap, Add2OperationsMap& add2OpsMap) {
        Symbols& checkSymbols = this -> dnss -> getCheckSymbols();
        for (auto& mapPair : add2OpsMap) {
            Address address = mapPair.first;
            OperationsMap& opsMap = mapPair.second;
            IDsMap& idsMap = availablePositions[address];
            Symbols& reachSymbols = add2SymbolsMap[address];
            Symbol2ID2PowerSymbolsMap visitied, work, newWork;
            for (auto& symbol : initialSymbolsMap[address]) {
                idsMap[symbol].insert(1);
                Symbols symbols;
                if (checkSymbols.count(symbol) > 0) {
                    symbols.insert(symbol);
                }
                visitied[symbol][1].insert(symbols);
                work[symbol][1].insert(symbols);
            }
            while (work.size() > 0) {
                for (auto& workPair : work) {
                    Symbol symbol = workPair.first;
                    ID2PowerSymbolsMap& map = workPair.second;
                    if (opsMap.count(symbol) == 0) continue;
                    for (Operation* op : opsMap[symbol]) {
                        Beta beta = op -> getBeta();
                        Type type = op -> getType();
                        Symbol targetSymbol = op -> getSymbol();
                        if (type == Type::CTK) {
                            Symbols symbols;
                            if (checkSymbols.count(symbol) > 0) {
                                symbols.insert(symbol);
                            }
                            if (visitied[targetSymbol][1].insert(symbols).second) {
                                newWork[targetSymbol][1].insert(symbols);
                                idsMap[targetSymbol].insert(1);
                            }
                        }
                        for (auto& mapPair : map) {
                            ID id = mapPair.first;
                            for (auto& symbols : mapPair.second) {
                                if ((type == Type::PUSH) ||
                                    (type == Type::STP && symbol != targetSymbol) ||
                                    (type == Type::CTP && symbols.count(targetSymbol) == 0) ||
                                    (type == Type::RTF && symbols.count(targetSymbol) == 0)) {
                                    Symbols newSymbols = symbols;
                                    if (checkSymbols.count(targetSymbol) > 0) {
                                        newSymbols.insert(targetSymbol);
                                    }
                                    ID newID = 0;
                                    if (beta == Beta::start || beta == Beta::START) {
                                        newID = id + 1;
                                        if (newID > stackHeight) continue;
                                    } else if (beta == Beta::finish || beta == Beta::FINISH) {
                                        newID = id;
                                    }
                                    if (visitied[targetSymbol][newID].insert(newSymbols).second) {
                                        newWork[targetSymbol][newID].insert(newSymbols);
                                        idsMap[targetSymbol].insert(newID);
                                    }
                                } else if (type == Type::RTF && symbols.count(symbol) > 0) {
                                    for (auto& s : reachSymbols) {
                                        if (s == targetSymbol) continue;
                                        ID2PowerSymbolsMap& pSymbolsMap = visitied[targetSymbol];
                                        for (auto& visitiedPair : pSymbolsMap) {
                                            ID id = visitiedPair.first;
                                            if (id == 0) continue;
                                            ID newID = id - 1;
                                            for (auto& vs : visitiedPair.second) {
                                                if (vs.count(targetSymbol) == 0) continue;
                                                if (pSymbolsMap[newID].insert(vs).second) {
                                                    newWork[targetSymbol][newID].insert(vs);
                                                    idsMap[targetSymbol].insert(newID);
                                                }
                                            }
                                        }
                                    }
                                    ID newID = 0;
                                    if (beta == Beta::start || beta == Beta::START) {
                                        newID = id;
                                    } else if (beta == Beta::finish || beta == Beta::FINISH) {
                                        newID = id - 1;
                                    }
                                    if (visitied[targetSymbol][newID].insert(symbols).second) {
                                        newWork[targetSymbol][newID].insert(symbols);
                                        idsMap[targetSymbol].insert(newID);
                                    }
                                }
                            }
                        }
                    }
                }
                work.clear();
                if (newWork.size() > 0) {
                    work.insert(newWork.begin(), newWork.end());
                    newWork.clear();
                }
            }
        }

        for (auto& mapPair : availablePositions) {
            cout << mapPair.first << endl;
            for (auto& mapPair1 : mapPair.second) {
                cout << mapPair1.first << " : ";
                for (auto& id : mapPair1.second) {
                    cout << id << " ";
                }
                cout << endl;
            }
        }
        cout << "endl" << endl;
    }

    void BoundedReachabilitySolver::mkVirtualOpsMap(Add2SymbolsMap& add2SymbolsMap, Add2OperationsMap& add2OpsMap) {
        Add2EnPortsMap enPortsMap;
        Add2ExPortsMap exPortsMap;
        for (auto& mapPair : add2SymbolsMap) {
            Address address = mapPair.first;
            for (auto& symbol : mapPair.second) {
                OperationsMap& superOpsMap = this -> dnss -> getSuperOperationsMap();
                if (superOpsMap.count(symbol) > 0) {
                    for (Operation* op : superOpsMap[symbol]) {
                        Symbol targetSymbol = op -> getSymbol();
                        Beta beta = op -> getBeta();
                        Type type = op -> getType();
                        Address targetAdd = getAddress(targetSymbol);
                        if (targetAdd != address) {
                            exPortsMap[address].push_back(ExPort(symbol, beta));
                            enPortsMap[targetAdd].push_back(EnPort(targetSymbol, type));
                        }
                    }
                }
            }
        }
        for (auto& mapPair : exPortsMap) {
            Address address = mapPair.first;
            if (exPortsMap.count(address) == 0) continue;
            OperationsMap& opsMap = add2OpsMap[address];
            for (ExPort& exPort : mapPair.second) {
                Symbol symbol = exPort.first;
                Beta beta = exPort.second;
                for (EnPort& enPort : enPortsMap[address]) {
                    Symbol targetSymbol = enPort.first;
                    Type type = enPort.second;
                    Operation* op = mkOperation(beta, type, targetSymbol);
                    opsMap[symbol].insert(op);
                }
            }
        }
    }

    void BoundedReachabilitySolver::mkValues(Add2SymbolsMap& add2SymbolsMap, ID2ValuesMap& id2ValuesMap, Values& orderValues, Values& transValues) {
        fst -> mkEnumValue("null");
        for (ID i = 0; i < dnss -> getAlphabet().size(); i++) {
            fst -> mkEnumValue();
        }
        for (auto& mapPair : add2SymbolsMap) {
            ID id = mapPair.first;
            Values& values = id2ValuesMap[id];
            values.push_back(getNullValue());
            for (auto& symbol : mapPair.second) {
                values.push_back(fst -> getValues()[symbol]);
            }
        }
        Order o;
        for (ID i = 1; i < stackNumber + 1; i++) {
            o.push_back(i);
        }
        Utility::Permutation_Order_Null(o, (ID)0, orders);
        ID id = fst -> getValues().size();
        for (auto& o : orders) {
            string name = "o";
            for (auto& i : o) {
                name += "_" + to_string(i);
            }
            orderValues.push_back(fst -> mkEnumValue(name));
            orderMap[o] = id++;
        }

        ID num = 1;
        OperationsMap opsMap = dnss -> getOperationsMap();
        opsMap.insert(dnss -> getSuperOperationsMap().begin(), dnss -> getSuperOperationsMap().end());
        for (auto& mapPair : opsMap) {
            Symbol symbol = mapPair.first;
            for (Operation* op : mapPair.second) {
                string name = "c" + to_string(num++);
                transValues.push_back(fst -> mkEnumValue(name));
                transMap[Transition(symbol, op)] = id++;
            }
        }
    }

    ConjunctionExp* BoundedReachabilitySolver::mkTopStackExp(Address add, ID pos, Symbol symbol) {
        Var* var = getSymbolVar(add, pos);
        Value* value = getSymbolValue(symbol);
        EquAtomic* atomic = fst -> mkEquAtomic(var, value);
        ConjunctionExp* stackExp = fst -> mkConjunctionExp(atomic);
        if (pos < stackHeight) {
            Var* var = getSymbolVar(add, pos + 1);
            EquAtomic* atomic = fst -> mkEquAtomic(var, getNullValue());
            stackExp -> mkRightExp(atomic);
        }
        return stackExp;
    }

    DisjunctionExp* BoundedReachabilitySolver::mkTopStackExp(Address add, Symbol symbol) {
        Value* value = getSymbolValue(symbol);
        DisjunctionExp* exp = fst -> mkDisjunctionExp();
        DisjunctionExp* stackExp = fst -> mkDisjunctionExp(exp);
        for (const auto& pos : availablePositions[add][symbol]) {
            DisjunctionExp* leftExp = fst -> mkDisjunctionExp();
            exp -> mkLeftExp(leftExp);
            exp -> mkRightExp(mkTopStackExp(add, pos, symbol));
            exp = leftExp;
        }
        return stackExp;
    }

    ConjunctionExp* BoundedReachabilitySolver::mkTargetTopStackExp(Address add, ID pos) {
        Var* var = getSymbolVar(add, pos);
        EquAtomic* rightAtomic = fst -> mkEquAtomic(var, getNullValue());
        var = getSymbolVar(add, pos - 1);
        EquAtomic* leftAtomic = fst -> mkEquAtomic(var, getNullValue(), 0);
        return fst -> mkConjunctionExp(leftAtomic, rightAtomic);
    }

    ConjunctionExp* BoundedReachabilitySolver::mkTargetTopStackExp(Address add, ID pos, Symbol symbol) {
        Var* var = getSymbolVar(add, pos);
        EquAtomic* rightAtomic = fst -> mkEquAtomic(var, getNullValue());
        var = getSymbolVar(add, pos - 1);
        EquAtomic* leftAtomic1 = fst -> mkEquAtomic(var, getSymbolValue(symbol), 0);
        EquAtomic* leftAtomic2 = fst -> mkEquAtomic(var, getNullValue(), 0);
        ConjunctionExp* leftExp = fst -> mkConjunctionExp(leftAtomic1, leftAtomic2);
        return fst -> mkConjunctionExp(leftExp, rightAtomic);
    }

    DisjunctionExp* BoundedReachabilitySolver::mkTopOrderExp(Address add) {
        DisjunctionExp* exp = fst -> mkDisjunctionExp();
        DisjunctionExp* orderExp = fst -> mkDisjunctionExp(exp);
        for (auto& o : orders) {
            if (o[add - 1] == 1) {
                EquAtomic* atomic = fst -> mkEquAtomic(getOrderVar(), getOrderValue(o));
                DisjunctionExp* leftExp = fst -> mkDisjunctionExp();
                exp -> mkLeftExp(leftExp);
                exp -> mkRightExp(atomic);
                exp = leftExp;
            }
        }
        return orderExp;
    }

    DisjunctionExp* BoundedReachabilitySolver::mkTopOrderExp(Address sAdd, Address tAdd) {
        DisjunctionExp* exp = fst -> mkDisjunctionExp();
        DisjunctionExp* orderExp = fst -> mkDisjunctionExp(exp);
        for (auto& o : orders) {
            if (o[sAdd - 1] == 1 && o[tAdd - 1] > 0) {
                EquAtomic* atomic = fst -> mkEquAtomic(getOrderVar(), getOrderValue(o));
                DisjunctionExp* leftExp = fst -> mkDisjunctionExp();
                exp -> mkLeftExp(leftExp);
                exp -> mkRightExp(atomic);
                exp = leftExp;
            }
        }
        return orderExp;
    }

    ConjunctionExp* BoundedReachabilitySolver::mkTopExp(Address add, ID pos, Symbol symbol, Expression* expression) {
        DisjunctionExp* orderExp = mkTopOrderExp(add);
        ConjunctionExp* stackExp = mkTopStackExp(add, pos, symbol);
        ConjunctionExp* res = fst -> mkConjunctionExp(stackExp, orderExp);
        return fst -> mkConjunctionExp(expression, res);
    }

    ConjunctionExp* BoundedReachabilitySolver::mkCheckSymbolExp(Address add, ID pos, ID topID, Symbol symbol, Expression* expression) {
        Var* var = getSymbolVar(add, pos);
        Value* value = getSymbolValue(symbol);
        EquAtomic* atomic = fst -> mkEquAtomic(var, value);
        ConjunctionExp* exp = fst -> mkConjunctionExp();
        ConjunctionExp* checkExp = fst -> mkConjunctionExp(exp, atomic);
        for (ID id = pos + 1; id <= topID; id++) {
            if (availablePositions[add][symbol].count(id) == 0) continue;
            Var* var = getSymbolVar(add, id);
            EquAtomic* atomic = fst -> mkEquAtomic(var, value, 0);
            ConjunctionExp* leftExp= fst -> mkConjunctionExp();
            exp -> mkLeftExp(leftExp);
            exp -> mkRightExp(atomic);
            exp = leftExp;
        }
        ConjunctionExp* res = fst -> mkConjunctionExp(expression, checkExp);
        return fst -> mkConjunctionExp(res);
    }

    ConjunctionExp* BoundedReachabilitySolver::mkCheckNegSymbolExp(Address add, Symbol symbol, ID topID, Expression* expression) {
        ConjunctionExp* exp = fst -> mkConjunctionExp();
        ConjunctionExp* checkExp = fst -> mkConjunctionExp(exp);
        Value* value = getSymbolValue(symbol);
        for (ID id = 1; id <= topID; id++) {
            if (availablePositions[add][symbol].count(id) == 0) continue;
            Var* var = getSymbolVar(add, id);
            EquAtomic* atomic = fst -> mkEquAtomic(var, value, 0);
            ConjunctionExp* leftExp= fst -> mkConjunctionExp();
            exp -> mkLeftExp(leftExp);
            exp -> mkRightExp(atomic);
            exp = leftExp;
        }
        ConjunctionExp* res = fst -> mkConjunctionExp(expression, checkExp);
        return fst -> mkConjunctionExp(res);
    }

    void getNewOrder4LchTsk(Order& o, Order& newOrder, Address add) {
        for (auto& id : o) {
            if (id > 0) {
                newOrder.push_back(id + 1);
            } else {
                newOrder.push_back(1);
            }
        }
        newOrder[add - 1] = 1;
    }

    void getNewOrder4Mv2Top(Order& o, Order& newOrder, Address add) {
        for (auto& id : o) {
            if (id > 0) {
                if (id > o[add - 1]) {
                    newOrder.push_back(id);
                } else if (id < o[add - 1]){
                    newOrder.push_back(id + 1);
                } else {
                    newOrder.push_back(1);
                }
            } else {
                newOrder.push_back(0);
            }
        }
    }

    void BoundedReachabilitySolver::mkLchTsk(Address add, Symbol symbol, Address targetAdd, Symbol targetSymbol, Expression* expression) {
        Var* var = getSymbolVar(targetAdd, 1);
        Var* orderVar = getOrderVar();
        Value* value = getSymbolValue(targetSymbol);
        for (auto& o : orders) {
            if (o[add - 1] == 1 && o[targetAdd - 1] == 0) {
                DisjunctionExp* stackExp = mkTopStackExp(add, symbol);
                EquAtomic* orderExp = fst -> mkEquAtomic(orderVar, getOrderValue(o));
                ConjunctionExp* exp = fst -> mkConjunctionExp(expression, fst -> mkConjunctionExp(stackExp, orderExp));
                Order newOrder;
                getNewOrder4LchTsk(o, newOrder, targetAdd);
                Value* orderValue = getOrderValue(newOrder);
                orderVar -> mkTransition(exp, orderValue);
                var -> mkTransition(exp, value);
            }
        }
    }

    void BoundedReachabilitySolver::mkPush(Address add, ID pos, Symbol symbol, Expression* expression, bool finish) {
        Var* var = nullptr;
        Value* value = getSymbolValue(symbol);
        if (finish) {
            var = getSymbolVar(add, pos);
        } else {
            var = getSymbolVar(add, pos + 1);
        }
        var -> mkTransition(expression, value);
    }

    void BoundedReachabilitySolver::mkClrTop(Address add, ID top, Symbol symbol, Expression* exp) {
        for (ID pos = 1; pos < top; pos++) {
            if (availablePositions[add][symbol].count(pos) == 0) continue;
            ConjunctionExp* checkExp = mkCheckSymbolExp(add, pos, top, symbol, exp);
            for (ID id = pos + 1; id <= top; id++) {
                Var* var = getSymbolVar(add, id);
                var -> mkTransition(checkExp, getNullValue());
            }
        }
    }

    void BoundedReachabilitySolver::mkClrTsk(Address add, ID pos, Symbol symbol, Expression* exp) {
        for (ID id = 2; id <= pos; id++) {
            Var* var = getSymbolVar(add, id);
            var -> mkTransition(exp, getNullValue());
        }
        Var* var = getSymbolVar(add, 1);
        var -> mkTransition(exp, getSymbolValue(symbol));
    }

    void BoundedReachabilitySolver::mkMv2Top(Address add, ID top, Symbol symbol, Expression* exp) {
        for (ID pos = 1; pos < top; pos++) {
            if (availablePositions[add][symbol].count(pos) == 0) continue;
            ConjunctionExp* checkExp = mkCheckSymbolExp(add, pos, top, symbol, exp);
            Var* var = getSymbolVar(add, top);
            Var* targetVar = getSymbolVar(add, pos);
            var -> mkTransition(checkExp, targetVar);
            for (ID id = pos; id < top; id++) {
                var = targetVar;
                targetVar = getSymbolVar(add, id + 1);
                var -> mkTransition(checkExp, targetVar);
            }
        }
    }

    void BoundedReachabilitySolver::mkSuperTrans(Address add, Symbol symbol, Operation* operation) {
        Type type = operation -> getType();
        Symbol targetSymbol = operation -> getSymbol();
        Address targetAdd = getAddress(targetSymbol);
        Transition t(symbol, operation);
        EquAtomic* transExp = fst -> mkEquAtomic(getVar(), getTransValue(t));
        mkLchTsk(add, symbol, targetAdd, targetSymbol, transExp);
        if (operation -> isFinish()) {
            for (const auto& pos : availablePositions[add][symbol]) {
                ConjunctionExp* topExp = mkTopExp(add, pos, symbol, transExp);
                Var* var = getSymbolVar(add, pos);
                var -> mkTransition(topExp, getNullValue());
            }
        }

        DisjunctionExp* stackExp = mkTopStackExp(add, symbol);
        for (auto& o : orders) {
            if (o[add - 1] == 1 && o[targetAdd - 1] > 0) {
                EquAtomic* orderExp = fst -> mkEquAtomic(getOrderVar(), getOrderValue(o));
                ConjunctionExp* topExp = fst -> mkConjunctionExp(stackExp, orderExp);
                topExp = fst -> mkConjunctionExp(transExp, topExp);
                Order newOrder;
                getNewOrder4Mv2Top(o, newOrder, targetAdd);
                Var* orderVar = getOrderVar();
                Value* orderValue = getOrderValue(newOrder);
                Value* value = getSymbolValue(targetSymbol);
                orderVar -> mkTransition(topExp, orderValue);
                if (type == Type::PUSH) {
                    for (auto& pos : availablePositions[targetAdd][targetSymbol]) {
                        if (pos == 1) continue;
                        ConjunctionExp* targetStackExp = mkTargetTopStackExp(targetAdd, pos);
                        ConjunctionExp* exp = fst -> mkConjunctionExp(topExp, targetStackExp);
                        Var* var = getSymbolVar(targetAdd, pos);
                        var -> mkTransition(exp, value);
                    }
                } else if (type == Type::CTK) {
                    mkClrTsk(targetAdd, stackHeight, targetSymbol, topExp);
                } else if (type == Type::STP) {
                    for (auto& pos : availablePositions[targetAdd][targetSymbol]) {
                        if (pos == 1) continue;
                        ConjunctionExp* targetStackExp = mkTargetTopStackExp(targetAdd, pos, targetSymbol);
                        ConjunctionExp* exp = fst -> mkConjunctionExp(topExp, targetStackExp);
                        Var* var = getSymbolVar(targetAdd, pos);
                        var -> mkTransition(exp, value);
                    }
                } else if (type == Type::CTP) {
                    for (ID top = 2; top <= stackHeight; top++) {
                        ConjunctionExp* targetStackExp = mkTargetTopStackExp(targetAdd, top, targetSymbol);
                        ConjunctionExp* exp = fst -> mkConjunctionExp(topExp, targetStackExp);
                        mkClrTop(targetAdd, top, targetSymbol, exp);
                    }
                } else if (type == Type::RTF) {
                    for (ID top = 2; top <= stackHeight; top++) {
                        ConjunctionExp* targetStackExp = mkTargetTopStackExp(targetAdd, top, targetSymbol);
                        ConjunctionExp* exp = fst -> mkConjunctionExp(topExp, targetStackExp);
                        mkMv2Top(targetAdd, top, targetSymbol, exp);
                    }
                }
            }
        }
    }

    void BoundedReachabilitySolver::mkTrans(Address add, Symbol symbol, Operation* operation) {
        Type type = operation -> getType();
        Symbol targetSymbol = operation -> getSymbol();
        Address targetAdd = getAddress(targetSymbol);
        Transition t(symbol, operation);
        EquAtomic* transExp = fst -> mkEquAtomic(getVar(), getTransValue(t));
        if (operation -> isPop()) {
            for (auto& pos : availablePositions[add][symbol]) {
                ConjunctionExp* exp = mkTopExp(add, pos, symbol, transExp);
                Var* var = getSymbolVar(add, pos);
                var -> mkTransition(exp, getNullValue());
            }
        } else {
            if (type == Type::PUSH || (type == Type::STP && symbol != targetSymbol)) {
                for (auto& pos : availablePositions[add][symbol]) {
                    if (pos < stackHeight || operation -> isFinish()) {
                        ConjunctionExp* exp = mkTopExp(add, pos, symbol, transExp);
                        mkPush(add, pos, targetSymbol, exp, operation -> isFinish());
                    }
                }
            } else if (type == Type::CTP) {
                if (symbol == targetSymbol) return;
                for (auto& pos : availablePositions[add][symbol]) {
                    ConjunctionExp* exp = mkTopExp(add, pos, symbol, transExp);
                    mkClrTop(add, pos, targetSymbol, exp);
                    if (pos < stackHeight || operation -> isFinish()) {
                        ConjunctionExp* checkNegExp = mkCheckNegSymbolExp(add, targetSymbol, pos, exp);
                        mkPush(add, pos, targetSymbol, checkNegExp, operation -> isFinish());
                    }
                }
            } else if (type == Type::RTF) {
                if (symbol == targetSymbol) return;
                for (auto& pos : availablePositions[add][symbol]) {
                    ConjunctionExp* exp = mkTopExp(add, pos, symbol, transExp);
                    mkMv2Top(add, pos, targetSymbol, exp);
                    if (pos < stackHeight || operation -> isFinish()) {
                        ConjunctionExp* checkNegExp = mkCheckNegSymbolExp(add, targetSymbol, pos, exp);
                        mkPush(add, pos, targetSymbol, checkNegExp, operation -> isFinish());
                    }
                }
            } else if (type == Type::CTK) {
                for (auto& pos : availablePositions[add][symbol]) {
                    ConjunctionExp* exp = mkTopExp(add, pos, symbol, transExp);
                    mkClrTsk(add, pos, targetSymbol, exp);
                }
            }
        }
    }

    void BoundedReachabilitySolver::init() {
        fst = new nuxmvfst::NuXmvFST();
        Add2SymbolsMap initialSymbolsMap, add2SymbolsMap;
        Add2OperationsMap add2OpsMap;
        ID2ValuesMap id2ValuesMap;
        Values orderValues, transValues;
        getInitialSymbolsMap(initialSymbolsMap);
        getReachableSymbolsAndOpsMap(initialSymbolsMap, add2SymbolsMap, add2OpsMap);
        mkVirtualOpsMap(add2SymbolsMap, add2OpsMap);
        mkAvailablePositions(initialSymbolsMap, add2SymbolsMap, add2OpsMap);
        mkValues(add2SymbolsMap, id2ValuesMap, orderValues, transValues);
        fst -> mkEnumVar("o", orderValues, getNullValue());
        fst -> mkEnumVar("c", transValues, getNullValue());
        ID id = 2;
        for (auto& mapPair : id2ValuesMap) {
            ID i = mapPair.first;
            for (ID j = 1; j <= stackHeight; j++) {
                string name = "s" + to_string(i) + "_" + to_string(j);
                fst -> mkEnumVar(name, mapPair.second, getNullValue());
                varMap[IDPair(i, j)] = id++;
            }
        }
        for (auto& mapPair : add2SymbolsMap) {
            Address add = mapPair.first;
            for (Symbol symbol : mapPair.second) {
                if (dnss -> getOperationsMap().count(symbol) > 0) {
                    for (Operation* op : dnss -> getOperationsMap()[symbol]) {
                        mkTrans(add, symbol, op);
                    }
                }
                if (dnss -> getSuperOperationsMap().count(symbol) > 0) {
                    for (Operation* op : dnss -> getSuperOperationsMap()[symbol]) {
                        if (add == getAddress(op -> getSymbol())) {
                            mkTrans(add, symbol, op);
                        } else {
                            mkSuperTrans(add, symbol, op);
                        }
                    }
                }
            }
        }
        cout << fst -> getVAR() << endl;
        cout << fst -> getASSIGN_NEXT() << endl;
    }
}
