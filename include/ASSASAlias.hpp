#ifndef ASSASAlias_hpp
#define ASSASAlias_hpp

#define MAX_HEIGHT 100
#define NUXMV_PATH "./nuxmv/bin/nuxmv "
#define lb -1
#define rb -2
#define qm -3
#define pm -4
#define sm -5
#define cm -6
#define mm -7
#define dm -8

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
#include "../../NuXmvFST/NuXmvFST.hpp"
#include "../../Utility/Utility.hpp"
#include "../../CGH/include/CGH.hpp"
using namespace std;
using namespace nuxmvfst;
using namespace utility;
using namespace cgh;

namespace assas {
    class Action;
    class Activity;
    class Affinity;
    class Operation;
    class DNSS;
    class NuXmvFST;
    class Object;
    class Manage;

    enum Alpha {start, finish};
    enum FLAG {CTK, CTP, NTK, RTF, STP, MTK, TOH};
    enum Error {lmdError, aftError, unmatchError, noMainError};

    enum class Lmd {STD, STK, STP, SIT};
    enum class Type {PUSH, CTK, CTP, STP, RTF, LTK};
    enum class Beta {pop, start, finish, START, FINISH};

    typedef size_t ID, Address;
    typedef long Symbol;

    typedef pair<Symbol, Beta> ExPort;
    typedef pair<Symbol, Type> EnPort;
    typedef pair<Symbol, Operation*> Transition;
    typedef pair<Address, ID> IDPair;

    typedef vector<ID> Order;
    typedef vector<FLAG> FLAGs;
    typedef vector<Symbol> RegEx;
    typedef vector<Order> Orders;
    typedef vector<string> strings;
    typedef vector<ExPort> ExPorts;
    typedef vector<EnPort> EnPorts;
    typedef vector<Activity*> Activities;
    typedef vector<Affinity*> Affinities;

    typedef unordered_set<ID> IDs;
    typedef unordered_set<Activity*> Acts;
    typedef unordered_set<Symbol> Symbols;
    typedef unordered_set<Action*> Actions;
    typedef unordered_set<Address> Addresses;
    typedef unordered_set<Operation*> Operations;
    typedef unordered_set<Symbols, SetHash<Symbol> > PowerSymbols;
    typedef vector<Symbols> Word;
    typedef vector<Word> Words;

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
    typedef unordered_map<ID, Values> ID2ValuesMap;
    typedef unordered_map<Order, ID, VectorHash<ID> > Order2IDMap;
    typedef unordered_map<Transition, ID, PairHash<ID, Operation*> > Transition2IDMap;
    typedef unordered_map<IDPair, ID, PairHash<ID, ID> > IDPair2IDMap;
    typedef unordered_map<DFAState<Symbol>*, Symbols> State2SymbolsMap;
    typedef unordered_map<DFAState<Symbol>*, State2SymbolsMap> TransMap;
    typedef unordered_map<Address, DFA<Symbol>*> DFAMap;
    typedef unordered_map<Address, RegEx> RegExMap;
}

#endif /* ASSASAlias_hpp */
