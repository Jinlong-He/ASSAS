#include "DNSS/DNSS.hpp"
namespace assas {
    Operation* DNSS::mkOperation(Symbol symbol, Beta beta, Type type, Symbol tSymbol) {
        Operation* operation = new Operation(beta, type, tSymbol);
        operationsMap[symbol].insert(operation);
        return operation;
    }

    void DNSS::load(ASM* a) {
        initialSymbol = a -> getMainActivity() -> getID();
        for (Activity* act : a -> getActivities()) {
            Symbol symbol = act -> getID();
            Lmd lmd = act -> getLmd();
            Address address = act -> getAft() -> getID();
            alphabet.insert(symbol);
            addressMap[symbol] = address;
            for (auto& mapPair : act -> getLaunchMap()) {
                Activity* tAct = mapPair.first;
                Symbol tSymbol = tAct -> getID();
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
        return os;
    }
}
