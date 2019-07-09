#ifndef ASSASAlias_hpp
#define ASSASAlias_hpp

#define MAX_HEIGHT 100

#include <stdio.h>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <math.h>
#include <ostream>
#include <fstream>
#include "../../WriteNuXmv/NuXmv.hpp"
#include "../../Utility/Utility.hpp"
using namespace std;
using namespace nuxmv;
namespace std {
    template<typename T, typename U>
    struct hash<pair<T, U> > {
    public:
        size_t operator()(const pair<T, U> &p) const {
            return std::hash<T>()(p.first) ^ std::hash<U>()(p.second);
        }
    };

    template<typename T>
    struct hash<unordered_set<T> > {
    public:
        size_t operator()(const unordered_set<T> &hashSet) const {
            size_t size = 0;
            for (auto item : hashSet) {
                size = size ^ std::hash<T>()(item);
            }
            return size;
        }
    };
}
namespace assas {
    class Action;
    class Activity;
    class Affinity;
    class Operation;
    class DNSS;
    class NuXmv;
    class Object;
    class Manage;

    enum Alpha {start, finish};
    enum FLAG {CTK, CTP, NTK, RTF, STP, MTK, TOH};
    enum Error {lmdError, aftError, unmatchError, noMainError};

    enum class Lmd {STD, STK, STP, SIT};
    enum class Type {PUSH, CTK, CTP, STP, RTF, LTK};
    enum class Beta {pop, start, finish, START, FINISH};

    typedef size_t ID, Symbol, Address;

    typedef pair<Symbol, Beta> ExPort;
    typedef pair<Symbol, Type> EnPort;

    typedef vector<FLAG> FLAGs;
    typedef vector<Symbol> RegEx;
    typedef vector<string> strings;
    typedef vector<ExPort> ExPorts;
    typedef vector<EnPort> EnPorts;
    typedef vector<Activity*> ActVec;

    typedef unordered_set<ID> IDs;
    typedef unordered_set<Activity*> Acts;
    typedef unordered_set<Affinity*> Afts;
    typedef unordered_set<Symbol> Symbols;
    typedef unordered_set<Action*> Actions;
    typedef unordered_set<Address> Addresses;
    typedef unordered_set<Operation*> Operations;
    typedef unordered_set<Symbols> PowerSymbols;

    typedef unordered_map<Symbol, IDs> IDsMap;
    typedef unordered_map<string, Activity*> ActMap;
    typedef unordered_map<string, Affinity*> AftMap;
    typedef unordered_map<Symbol, Address> AddressMap;
    typedef unordered_map<Address, IDsMap> Add2IDsMap;
    typedef unordered_map<Address, EnPorts> Add2EnPortsMap;
    typedef unordered_map<Address, ExPorts> Add2ExPortsMap;
    typedef unordered_map<Address, Symbols> Add2SymbolsMap;
    typedef unordered_map<Activity*, Actions> LaunchMap;
    typedef unordered_map<Symbol, Operations> OperationsMap;
    typedef unordered_map<Address, OperationsMap> Add2OperationsMap;
    typedef unordered_map<ID, PowerSymbols> ID2PowerSymbolsMap;
    typedef unordered_map<Symbol, ID2PowerSymbolsMap> Symbol2ID2PowerSymbolsMap;
}

#endif /* ASSASAlias_hpp */
