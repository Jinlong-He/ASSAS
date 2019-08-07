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

    /// \brief Operation in DNSS.
    class Operation : public Object{
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

        /// \brief Gets whether this Operation is Pop Operation.
        /// \return Boolean.
        bool isPop() const {
            return beta == Beta::pop;
        }

        /// \brief Gets whether this Operation is Super Operation.
        /// \return Boolean.
        bool isSuper() const {
            return (beta == Beta::START || beta == Beta::FINISH);
        }

        /// \brief Gets whether this Operation is Start Operation.
        /// \return Boolean.
        bool isFinish() const {
            return (beta == Beta::FINISH|| beta == Beta::finish);
        }

        friend ostream & operator<<( ostream & os, const Operation& o);
    };
}

#endif /* Operation_hpp */
