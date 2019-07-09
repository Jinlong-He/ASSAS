#include "Solver/LengthBoundedSolver.hpp"
namespace assas {
    void LengthBoundedSolver::mkAvailablePositions() {
        Add2SymbolsMap initialSymbolMap, add2SymbolsMap;
        Add2OperationsMap add2OpsMap;
        getInitialSymbolsMap(initialSymbolMap);
        getReachableSymbolsAndOpsMap(initialSymbolMap, add2SymbolsMap, add2OpsMap);
        mkVirtualOpsMap(add2SymbolsMap, add2OpsMap);
        for (auto& mapPair : add2OpsMap) {
            cout << mapPair.first << endl;
            for (auto& mapPair1 : mapPair.second) {
                for (Operation* op : mapPair1.second) {
                    cout << mapPair1.first << "->" << *op << endl;
                }
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
