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

    /// \breif Doubly Nested Stack System.
    class DNSS : public ASSASObject {
    private:
        Symbol initialSymbol;           ///< the initial symbol for this DNSS.
        Symbols alphabet;               ///< the alphabet for this DNSS.
        Symbols checkSymbols;           ///> the symbols which are started by CTP or RTF.
        Addresses addresses;            ///< the addresses for this DNSS.
        AddressMap addressMap;          ///< the address map for this DNSS.
        OperationsMap operationsMap;    ///< the operations map for this DNSS.
        OperationsMap superOpsMap;      ///< the operations map for this DNSS.
        bool stackBoundedness;          ///< true for stack boundedness.

    public:
        /// \brief Default construction function.
        DNSS() {}

        /// \brief Construction function with params.
        /// \param a ASM.
        DNSS(ASM* a) {
            load(a);
        }

        ~DNSS() {
        }

        void load(ASM* a);

        /// \brief Makes a Operation in this DNSS.
        /// \param symbol Source symbol.
        /// \param beta Beta.
        /// \param type Operation type.
        /// \param tSymbol Target symbol.
        /// \return Pointer of Operation.
        Operation* mkOperation(Symbol symbol, Beta beta, Type type, Symbol tSymbol);

        /// \brief Gets initial symbol.
        /// \return Symbol.
        Symbol getInitialSymbol() const {
            return initialSymbol;
        }

        /// \brief Gets alphabet.
        /// \return reference of Symbols.
        Symbols& getAlphabet() {
            return alphabet;
        }

        Symbols& getCheckSymbols() {
            return checkSymbols;
        }

        const Symbols& getAlphabet() const {
            return alphabet;
        }

        const Symbols& getCheckSymbols() const {
            return checkSymbols;
        }

        /// \brief Gets addresses.
        /// \return reference of Addresses.
        Addresses& getAddresses() {
            return addresses;
        }

        const Addresses& getAddresses() const {
            return addresses;
        }

        /// \brief Gets address map.
        /// \return reference of AddressMap
        AddressMap& getAddressMap() {
            return addressMap;
        }

        const AddressMap& getAddressMap() const {
            return addressMap;
        }

        /// \brief Gets operations map.
        /// \return reference of OperationsMap.
        OperationsMap& getOperationsMap() {
            return operationsMap;
        }

        OperationsMap& getSuperOperationsMap() {
            return superOpsMap;
        }

        const OperationsMap& getOperationsMap() const {
            return operationsMap;
        }

        const OperationsMap& getSuperOperationsMap() const {
            return superOpsMap;
        }

        /// \brief Gets stack boundedness.
        /// \return Boolean.
        bool isStackBounded() const {
            return stackBoundedness;
        }

        /// \brief Decides whether this DNSS is stack bounded.
        void decideStackBounded();

        friend ostream & operator<<( ostream & os, const DNSS& dnss);
    };
}

#endif /* DNSS_hpp */
