//
//  DNSS.hpp
//  DNSS
//
//  Created by Jinlong He on 2019/6/23.
//  Copyright © 2019年 Jinlong He. All rights reserved.
//

#ifndef DNSS_hpp
#define DNSS_hpp

#include "Operation.hpp"

namespace assas {
    class Operation;
    typedef size_t Address;
    typedef unordered_set<Symbol> Symbols;
    typedef unordered_set<Operation*> Operations;
    typedef unordered_map<Symbol, Address> AddressMap;
    typedef unordered_map<Symbol, Operations> OperationsMap;

    /// \breif Doubly Nested Stack System.
    class DNSS {
    private:
        Symbol initialSymbol;           ///< the initial symbol for this DNSS.
        Symbols alphabet;               ///< the alphabet for this DNSS.
        AddressMap addressMap;          ///< the address map for this DNSS.
        OperationsMap operationsMap;    ///< the operations map for this DNSS.
    public:
        /// \brief Default construction function.
        DNSS() {}

        /// \brief Construction function with params.
        /// \param a ASM.
        DNSS(ASM* a) {
            load(a);
        }

        void load(ASM* a);

        /// \brief Makes a Operation in this DNSS.
        /// \param symbol Source symbol.
        /// \param beta Beta.
        /// \param type Operation type.
        /// \param tSymbol Target symbol.
        /// \return Pointer of Operation.
        Operation* mkOperation(Symbol symbol, Beta beta, Type type, Symbol tSymbol);

        friend ostream & operator<<( ostream & os, const DNSS& dnss);
    };
}

#endif /* DNSS_hpp */
