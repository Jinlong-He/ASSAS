//
//  Operation.hpp
//  DNSS
//
//  Created by Jinlong He on 2019/6/23.
//  Copyright © 2019年 Jinlong He. All rights reserved.
//

#ifndef Operation_hpp
#define Operation_hpp

#include "../ASM/ASM.hpp"

namespace assas {

    typedef size_t Symbol;
    enum class Beta {pop, start, finish, START, FINISH};
    enum class Type {PUSH, CTK, CTP, STP, RTF, LTK};

    /// \brief Operation in DNSS.
    class Operation {
    protected:
        Beta beta;          ///< how to start symbol.
        Type type;          ///< Operation type.
        Symbol symbol;      ///< target symbol.
    public:
        /// \brief Default construction function.
        Operation() : beta(Beta::start), type(Type::PUSH), symbol(0) {}

        /// \brief Construction function with params.
        Operation(Beta b, Type t, Symbol s) : beta(b), type(t), symbol(s) {}

        /// \brief Gets target symbol.
        /// \return Symbol.
        Symbol getSymbol() const {
            return symbol;
        }

        /// \brief Gets Beta.
        /// \return Beta.
        Beta getBeta() const {
            return beta;
        }

        /// \brief Gets Operation type.
        /// \return Type.
        Type getType() const {
            return type;
        }

        friend ostream & operator<<( ostream & os, const Operation& o);
    };
}

#endif /* Operation_hpp */
