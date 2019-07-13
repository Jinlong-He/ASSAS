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
                        Address targetAddress = getAddress(targetSymbol);
                        if (targetAddress != address) {
                            exPortsMap[address].push_back(ExPort(symbol, beta));
                            enPortsMap[targetAddress].push_back(EnPort(targetSymbol, type));
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

    void BoundedReachabilitySolver::mkValues(Add2SymbolsMap& add2SymbolsMap, ID2ValuesMap& id2ValuesMap) {
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
        Values& values = id2ValuesMap[0];
        for (auto& o : orders) {
            string name = "o";
            for (auto& i : o) {
                name += "_" + to_string(i);
            }
            values.push_back(fst -> mkEnumValue(name));
            orderMap[o] = id++;
        }
    }

    ConjunctionExp* BoundedReachabilitySolver::mkTopExp(ID sID, ID pos, Symbol symbol) {
        DisjunctionExp* exp = fst -> mkDisjunctionExp();
        DisjunctionExp* orderExp = fst -> mkDisjunctionExp(exp);
        for (auto& o : orders) {
            if (o[sID] == 1) {
                EquAtomic* atomic = fst -> mkEquAtomic(getOrderVar(), getOrderValue(o));
                DisjunctionExp* leftExp = fst -> mkDisjunctionExp();
                exp -> mkLeftExp(leftExp);
                exp -> mkRightExp(atomic);
                exp = leftExp;
            }
        }
        Var* var = getSymbolVar(sID, pos);
        Value* value = getSymbolValue(symbol);
        EquAtomic* atomic = fst -> mkEquAtomic(var, value);
        ConjunctionExp* stackExp = fst -> mkConjunctionExp(atomic);
        if (pos < stackHeight) {
            Var* var = getSymbolVar(sID, pos + 1);
            EquAtomic* atomic = fst -> mkEquAtomic(var, getNullValue());
            stackExp -> mkRightExp(atomic);
        }
        ConjunctionExp* res = fst -> mkConjunctionExp(stackExp, orderExp);
        return fst -> mkConjunctionExp(res);
    }

    ConjunctionExp* BoundedReachabilitySolver::mkCheckExp(Address add, ID pos, ID topID, Symbol symbol, ConjunctionExp* expression) {
        ConjunctionExp* exp = fst -> mkConjunctionExp();
        Var* var = getSymbolVar(add, pos);
        Value* value = getSymbolValue(symbol);
        EquAtomic* atomic = fst -> mkEquAtomic(var, value);
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

    void BoundedReachabilitySolver::mkTrans(Address add, Symbol symbol, Operation* operation) {
        Beta beta = operation -> getBeta();
        Type type = operation -> getType();
        Symbol targetSymbol = operation -> getSymbol();
        Address targetAddress = getAddress(targetSymbol);
        if (beta == Beta::pop) {
            for (auto& pos : availablePositions[add][symbol]) {
                ConjunctionExp* exp = mkTopExp(add, pos, symbol);
                Var* var = getSymbolVar(add, pos);
                var -> mkTransition(exp, getNullValue());
            }
        } else if (beta == Beta::start) {
            if (type == Type::PUSH || (type == Type::STP && symbol != targetSymbol)) {
                for (auto& pos : availablePositions[add][symbol]) {
                    if (availablePositions[add][targetSymbol].count(pos + 1) == 0) continue;
                    ConjunctionExp* exp = mkTopExp(add, pos, symbol);
                    Var* var = getSymbolVar(add, pos + 1);
                    Value* value = getSymbolValue(targetSymbol);
                    var -> mkTransition(exp, value);
                }
            } else if (type == Type::CTP) {
                for (auto& pos : availablePositions[add][symbol]) {
                    ConjunctionExp* exp = mkTopExp(add, pos, symbol);
                    for (ID tID = 1; tID < pos; tID++) {
                        if (availablePositions[add][targetSymbol].count(tID) == 0) continue;
                        ConjunctionExp* checkExp = mkCheckExp(add, tID, pos, targetSymbol, exp);
                        for (ID id = tID + 1; id <= pos; id++) {
                            Var* var = getSymbolVar(add, id);
                            var -> mkTransition(checkExp, getNullValue());
                        }
                    }
                }
            } else if (type == Type::RTF) {
                for (auto& pos : availablePositions[add][symbol]) {
                    ConjunctionExp* exp = mkTopExp(add, pos, symbol);
                    for (ID tID = 1; tID < pos; tID++) {
                        if (availablePositions[add][targetSymbol].count(tID) == 0) continue;
                        ConjunctionExp* checkExp = mkCheckExp(add, tID, pos, targetSymbol, exp);
                        Var* var = getSymbolVar(add, pos);
                        Var* targetVar = getSymbolVar(add, tID);
                        var -> mkTransition(checkExp, targetVar);
                        for (ID id = tID; id < pos; id++) {
                            var = targetVar;
                            targetVar = getSymbolVar(add, id + 1);
                            var -> mkTransition(checkExp, targetVar);
                        }
                    }
                }
            } else if (type == Type::CTK) {
                for (auto& pos : availablePositions[add][symbol]) {
                    ConjunctionExp* exp = mkTopExp(add, pos, symbol);
                    for (ID id = 2; id <= pos; id++) {
                        Var* var = getSymbolVar(add, id);
                        var -> mkTransition(exp, getNullValue());
                    }
                    Var* var = getSymbolVar(add, 1);
                    var -> mkTransition(exp, getSymbolValue(targetSymbol));
                }
            }
        } else if (beta == Beta::start || beta == Beta::finish) {
        }
    }

    void BoundedReachabilitySolver::init() {
        fst = new nuxmvfst::NuXmvFST();
        Add2SymbolsMap initialSymbolsMap, add2SymbolsMap;
        Add2OperationsMap add2OpsMap;
        ID2ValuesMap id2ValuesMap;
        getInitialSymbolsMap(initialSymbolsMap);
        getReachableSymbolsAndOpsMap(initialSymbolsMap, add2SymbolsMap, add2OpsMap);
        mkVirtualOpsMap(add2SymbolsMap, add2OpsMap);
        mkAvailablePositions(initialSymbolsMap, add2SymbolsMap, add2OpsMap);
        mkValues(add2SymbolsMap, id2ValuesMap);
        for (auto& mapPair : id2ValuesMap) {
            ID id = mapPair.first;
            if (id == 0) {
                fst -> mkEnumVar("o", mapPair.second, getNullValue());
            } else {
                for (ID j = 1; j <= stackHeight; j++) {
                    string name = "s" + to_string(id) + "_" + to_string(j);
                    fst -> mkEnumVar(name, mapPair.second, getNullValue());
                }
            }
        }
        for (auto& mapPair : add2SymbolsMap) {
            Address add = mapPair.first;
            for (Symbol symbol : mapPair.second) {
                if (dnss -> getOperationsMap().count(symbol) == 0) continue;
                for (Operation* op : dnss -> getOperationsMap()[symbol]) {
                    mkTrans(add, symbol, op);
                }
            }
        }
        cout << fst -> getASSIGN_NEXT() << endl;
    }
}
