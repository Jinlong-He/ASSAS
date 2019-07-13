#include "ReachabilitySolver/ReachabilitySolver.hpp"
namespace assas {

    void ReachabilitySolver::getInitialSymbolsMap(Add2SymbolsMap& initialSymbolsMap) {
        Symbol initialSymbol = dnss -> getInitialSymbol();
        initialSymbolsMap[getAddress(initialSymbol)].insert(initialSymbol);
        for (auto& mapPair : dnss -> getSuperOperationsMap()) {
            for (Operation* op : mapPair.second) {
                Symbol symbol = op -> getSymbol();
                initialSymbolsMap[getAddress(symbol)].insert(symbol);
            }
        }
    }

    void ReachabilitySolver::getReachableSymbolsAndOpsMap(Add2SymbolsMap& initialSymbolsMap, Add2SymbolsMap& add2SymbolsMap, Add2OperationsMap& add2OpsMap) {
        Symbols work, newWork;
        OperationsMap& operationsMap = dnss -> getOperationsMap();
        OperationsMap& superOpsMap = dnss -> getSuperOperationsMap();
        for (auto& mapPair : initialSymbolsMap) {
            work.clear();
            newWork.clear();
            work.insert(mapPair.second.begin(), mapPair.second.end());
            Address address = mapPair.first;
            Symbols& symbols = add2SymbolsMap[address];
            OperationsMap& opsMap = add2OpsMap[address];
            while (work.size() > 0) {
                for (auto& symbol : work) {
                    if (operationsMap.count(symbol) > 0) {
                        Operations& operations = operationsMap[symbol];
                        opsMap[symbol] = operations;
                        for (Operation* op : operations) {
                            Symbol targetSymbol = op -> getSymbol();
                            if (symbols.insert(targetSymbol).second) {
                                newWork.insert(targetSymbol);
                            }
                        }
                    }
                    if (superOpsMap.count(symbol) > 0) {
                        for (Operation* op : superOpsMap[symbol]) {
                            Symbol targetSymbol = op -> getSymbol();
                            if (getAddress(targetSymbol) != address) continue;
                            opsMap[symbol].insert(op);
                            if (symbols.insert(targetSymbol).second) {
                                newWork.insert(targetSymbol);
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
    }
}
