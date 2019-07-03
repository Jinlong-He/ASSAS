//
//  Parse.hpp
//  ASM
//
//  Created by Jinlong He on 2019/6/23.
//  Copyright © 2019年 Jinlong He. All rights reserved.
//

#ifndef Parse_hpp
#define Parse_hpp

#include <stdio.h>
#include <string>
#include <ostream>
#include <fstream>
#include "ASM/ASM.hpp"
#include "../../Utility/Utility.hpp"
using namespace std;
namespace assas {
    enum Error {lmdError, aftError, unmatchError, noMainError};
    typedef vector<string> strings;
    typedef unordered_map<string, Activity*> ActMap;
    typedef unordered_map<string, Affinity*> AftMap;
    /// \brief This class is to get information via reading files.
    class Parse {
    private:
        ASM* a;
        ActMap actMap;
        AftMap aftMap;

    public:
        /// \brief Default construction function.
        Parse() : a(nullptr) {}

        /// \brief Construction function with params.
        /// \param infoFileName The file name of ActivityInfo.
        /// \param transFileName The file name of ActivityTrans.
        /// \param a The ASM.
        Parse(const string& infoFileName, const string& transFileName, ASM* a) : a(a) {
            readInfoFile(infoFileName);
            readTransitionGragh(transFileName);
            a -> rmUselessActivities();
            cout << *a << endl;
        }

        /// \brief Default desconstruction function.
        ~Parse() {}

        /// \brief Reads ActivityInfo file.
        /// \param fileName The name of file.
        void readInfoFile(const string& fileName);

        /// \brief Reads ActivityTrans file.
        /// \param fileName The name of file.
        void readTransitionGragh(const string& fileName);

        /// \brief Writes errors.
        /// \param error The type of error.
        void writeErr(Error error);
    };
}


#endif /* Parse_hpp */
