#ifndef ASSASObject_hpp
#define ASSASObject_hpp

#include "ASSASAlias.hpp"
namespace assas {
    class ASSASObject {
    public:
        virtual ~ASSASObject() {}
    };

    class ASSASManage {
    public:
        static unordered_set<ASSASObject*> buffer;
        static void manage(ASSASObject* object) {
            buffer.insert(object);
        }
        static void del(ASSASObject* object) {
            buffer.erase(object);
            delete object;
        }
        ~ASSASManage() {
            for (ASSASObject* object : buffer) {
                if (object) {
                    delete object;
                    object = nullptr;
                }
            }
        }
    };
}

#endif /* ASSASObject_hpp */
