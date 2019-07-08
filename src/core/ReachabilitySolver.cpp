#include "Solver/ReachabilitySolver.hpp"
namespace assas {

    void ReachabilitySolver::getInitialSymbolsMap(Add2SymbolsMap& initialSymbolMap) {
        Symbol initialSymbol = dnss -> getInitialSymbol();
        initialSymbolMap[dnss -> getAddressMap()[initialSymbol]].insert(initialSymbol);
        for (auto& mapPair : dnss -> getSuperOperationsMap()) {
            for (Operation* op : mapPair.second) {
                Symbol symbol = op -> getSymbol();
                initialSymbolMap[symbol].insert(symbol);
            }
        }
    }

    void ReachabilitySolver::getReachableSymbolsAndOpsMap(Add2SymbolsMap& initialSymbolMap, Add2SymbolsMap& add2SymbolsMap, Add2OperationsMap& add2OpsMap) {
        Symbols work, newWork;
        OperationsMap& operationsMap = dnss -> getOperationsMap();
        OperationsMap& superOpsMap = dnss -> getSuperOperationsMap();
        for (auto& mapPair : initialSymbolMap) {
            work.clear();
            newWork.clear();
            work.insert(mapPair.second.begin(), mapPair.second.end());
            Address address = mapPair.first;
            Symbols& symbols = add2SymbolsMap[address];
            OperationsMap& opsMap = add2OpsMap[address];
            while (work.size() > 0) {
                for (Symbol symbol : work) {
                    if (operationsMap.count(symbol) == 0) continue;
                    Operations& operations = operationsMap[symbol];
                    opsMap[symbol] = operations;
                    for (Operation* op : operations) {
                        Symbol targetSymbol = op -> getSymbol();
                        if (symbols.insert(targetSymbol).second) {
                            newWork.insert(targetSymbol);
                        }
                    }
                    if (superOpsMap.count(symbol) == 0) continue;
                    for (Operation* op : superOpsMap[symbol]) {
                        Symbol targetSymbol = op -> getSymbol();
                        if (dnss -> getAddressMap()[targetSymbol] != address) continue;
                        opsMap[symbol].insert(op);
                        if (symbols.insert(targetSymbol).second) {
                            newWork.insert(targetSymbol);
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
