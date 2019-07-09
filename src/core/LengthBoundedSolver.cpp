#include "Solver/LengthBoundedSolver.hpp"
namespace assas {
    void LengthBoundedSolver::mkAvailablePositions() {
        Add2SymbolsMap initialSymbolsMap, add2SymbolsMap;
        Add2OperationsMap add2OpsMap;
        getInitialSymbolsMap(initialSymbolsMap);
        getReachableSymbolsAndOpsMap(initialSymbolsMap, add2SymbolsMap, add2OpsMap);
        mkVirtualOpsMap(add2SymbolsMap, add2OpsMap);
        Symbols& checkSymbols = this -> dnss -> getCheckSymbols();
        for (auto& mapPair : add2OpsMap) {
            Address address = mapPair.first;
            OperationsMap& opsMap = mapPair.second;
            IDsMap& idsMap = availablePositions[address];
            Symbols& reachSymbols = add2SymbolsMap[address];
            Symbol2ID2PowerSymbolsMap visitied, work, newWork;
            for (Symbol symbol : initialSymbolsMap[address]) {
                idsMap[symbol].insert(0);
                Symbols symbols;
                if (checkSymbols.count(symbol) > 0) {
                    symbols.insert(symbol);
                }
                visitied[symbol][0].insert(symbols);
                work[symbol][0].insert(symbols);
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
                            if (visitied[targetSymbol][0].insert(symbols).second) {
                                newWork[targetSymbol][0].insert(symbols);
                                idsMap[targetSymbol].insert(0);
                            }
                        }
                        for (auto& mapPair : map) {
                            ID id = mapPair.first;
                            for (Symbols symbols : mapPair.second) {
                                if (type == Type::PUSH ||
                                    type == Type::STP && symbol != targetSymbol ||
                                    type == Type::CTP && symbols.count(targetSymbol) == 0 ||
                                    type == Type::RTF && symbols.count(targetSymbol) == 0) {
                                    Symbols newSymbols = symbols;
                                    if (checkSymbols.count(targetSymbol) > 0) {
                                        newSymbols.insert(targetSymbol);
                                    }
                                    ID newID = 0;
                                    if (beta == Beta::start || beta == Beta::START) {
                                        newID = id + 1;
                                        if (newID == maxHeight) continue;
                                    } else if (beta == Beta::finish || beta == Beta::FINISH) {
                                        newID = id;
                                    }
                                    if (visitied[targetSymbol][newID].insert(newSymbols).second) {
                                        newWork[targetSymbol][newID].insert(newSymbols);
                                        idsMap[targetSymbol].insert(newID);
                                    }
                                } else if (type == Type::RTF && symbols.count(symbol) > 0) {
                                    for (Symbol s : reachSymbols) {
                                        if (s == targetSymbol) continue;
                                        ID2PowerSymbolsMap& pSymbolsMap = visitied[targetSymbol];
                                        for (auto& visitiedPair : pSymbolsMap) {
                                            ID id = visitiedPair.first;
                                            if (id == 0) continue;
                                            ID newID = id - 1;
                                            for (Symbols vs : visitiedPair.second) {
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
                for (ID id : mapPair1.second) {
                    cout << id << " ";
                }
                cout << endl;
            }
        }
    }

    void LengthBoundedSolver::mkVirtualOpsMap(Add2SymbolsMap& add2SymbolsMap, Add2OperationsMap& add2OpsMap) {
        Add2EnPortsMap enPortsMap;
        Add2ExPortsMap exPortsMap;
        for (auto& mapPair : add2SymbolsMap) {
            Address address = mapPair.first;
            for (Symbol symbol : mapPair.second) {
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
            for (ExPort exPort : mapPair.second) {
                Symbol symbol = exPort.first;
                Beta beta = exPort.second;
                for (EnPort enPort : enPortsMap[address]) {
                    Symbol targetSymbol = enPort.first;
                    Type type = enPort.second;
                    Operation* op = mkOperation(beta, type, targetSymbol);
                    opsMap[symbol].insert(op);
                }
            }
        }
    }
}
