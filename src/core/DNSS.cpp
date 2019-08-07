#include "DNSS/DNSS.hpp"
namespace assas {
    Operation* DNSS::mkOperation(Symbol symbol, Beta beta, Type type, Symbol tSymbol) {
        Operation* operation = new Operation(beta, type, tSymbol);
        if (operation -> isSuper()) {
            superOpsMap[symbol].insert(operation);
        } else {
            operationsMap[symbol].insert(operation);
        }
        if (type == Type::CTP || type == Type::RTF) {
            checkSymbols.insert(tSymbol);
        }
        Manage::manage(operation);
        return operation;
    }

    void DNSS::load(ASM* a) {
        unordered_map<Activity*, Symbol> actMap;
        unordered_map<Affinity*, Symbol> aftMap;
        Address address = 1;
        for (Affinity* aft : a -> getAffinities()) {
            aftMap[aft] = address;
            addresses.insert(address++);
        }
        Symbol symbol = 1;
        for (Activity* act : a -> getActivities()) {
            actMap[act] = symbol;
            addressMap[symbol] = aftMap[act -> getAft()];
            alphabet.insert(symbol++);
        }
        for (Activity* act : a -> getActivities()) {
            Symbol symbol = actMap[act];
            if (act == a -> getMainActivity()) {
                initialSymbol = symbol;
            }
            Lmd lmd = act -> getLmd();
            for (auto& mapPair : act -> getLaunchMap()) {
                Activity* tAct = mapPair.first;
                Symbol tSymbol = actMap[tAct];
                Lmd tLmd = tAct -> getLmd();
                for (Action* action : mapPair.second) {
                    Alpha alpha = action -> getAlpha();
                    Beta beta;
                    Type type;
                    if (tLmd == Lmd::STK) {
                        beta = alpha == start ? Beta::START : Beta::FINISH;
                        type = action -> hasCTK() ? Type::CTK : Type::CTP;
                    } else if (tLmd == Lmd::SIT){
                    } else {
                        if (lmd == Lmd::SIT || action -> hasNTK()) {
                            beta = alpha == start ? Beta::START : Beta::FINISH;
                            if (action -> hasMTK()) {
                                type = Type::LTK;
                                continue;
                            }
                        } else {
                            beta = alpha == start ? Beta::start : Beta::finish;
                        }
                        if (action -> hasCTK()) {
                            type = Type::CTK;
                        } else if (action -> hasCTP()) {
                            type = Type::CTP;
                        } else if (action -> hasRTF()) {
                            type = Type::RTF;
                        } else if (action -> hasSTP() || tLmd == Lmd::STP) {
                            type = Type::STP;
                        } else {
                            type = Type::PUSH;
                        }
                    }
                    mkOperation(symbol, beta, type, tSymbol);
                }
            }
        }
    }

    ostream & operator<<( ostream & os, const DNSS& dnss) {
        for (auto& mapPair : dnss.operationsMap) {
            for (Operation* op : mapPair.second) {
                cout << mapPair.first << "->" << *op << endl;
            }
        }
        for (auto& mapPair : dnss.superOpsMap) {
            for (Operation* op : mapPair.second) {
                cout << mapPair.first << "->" << *op << endl;
            }
        }
        return os;
    }
}
