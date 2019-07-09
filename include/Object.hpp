#ifndef Object_hpp
#define Object_hpp

#include "ASSASAlias.hpp"
namespace assas {
    class Object {
    public:
        virtual ~Object() {}
    };

    class Manage {
    public:
        static unordered_set<Object*> buffer;
        static void manage(Object* object) {
            buffer.insert(object);
        }
        static void del(Object* object) {
            buffer.erase(object);
            delete object;
        }
        ~Manage() {
            for (Object* object : buffer) {
                if (object) {
                    delete object;
                    object = nullptr;
                }
            }
        }
    };
}

#endif /* Object_hpp */
