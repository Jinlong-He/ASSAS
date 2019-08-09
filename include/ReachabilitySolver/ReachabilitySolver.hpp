//
//  ReachabilitySolver.hpp
//  Reachability 
//
//  Created by Jinlong He on 2019/6/23.
//  Copyright © 2019年 Jinlong He. All rights reserved.
//

#ifndef ReachabilitySolver_hpp
#define ReachabilitySolver_hpp

#include "../DNSS/DNSS.hpp"
namespace assas {

    /// \brief Interface of Reachabilityability Solver.
    class ReachabilitySolver : public ASSASObject{
    protected:
        DNSS* dnss;             ///< the DNSS to be solved.
    public:
        /// \brief Default construction function.
        ReachabilitySolver() : dnss(nullptr) {}

        /// \brief Construction function with params.
        /// \param s DNSS.
        ReachabilitySolver(DNSS* s) : dnss(s) {}

        virtual ~ReachabilitySolver() {
        }

        /// \brief Gets reachable symbols and operations for each address.
        void getReachableSymbolsAndOpsMap(Add2SymbolsMap& initialSymbolsMap, Add2SymbolsMap& add2SymbolsMap, Add2OperationsMap& add2OpsMap);

        /// \brief Gets initial symbols for each address.
        /// \param initialSymbolsMap initial symbols map.
        void getInitialSymbolsMap(Add2SymbolsMap& initialSymbolMap);

        /// \brief Decides whether param regEx is reachable from initial config.
        /// \param regEx Given configuration.
        /// \return Boolean.
        virtual bool isReachable(const RegEx& regEx) = 0;
        virtual bool isReachable(const RegExMap& regExMap) = 0;

        /// \brief Decides whether param regEx which is a sub configuration that is reachable from initial configuration.
        /// \param regEx Given configuration.
        /// \return Boolean.
        virtual bool contains(const RegEx& regEx) = 0;

        /// \brief Gets Address of param symbol.
        /// \param symbol
        /// \return Address.
        Address getAddress(Symbol symbol) {
            return dnss -> getAddressMap()[symbol];
        }

    };
}

#endif /* ReachabilitySolver_hpp */
