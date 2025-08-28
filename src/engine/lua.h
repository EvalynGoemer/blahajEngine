#pragma once

#include <lua.hpp>
#include <memory>
#include <string>

namespace blahajEngine {
    class lua {
    public:
        static void bindAllEngineFuncs(lua_State *L);
    };

    template<typename T>
    void push_shared_ptr_to_lua(lua_State* L, const std::shared_ptr<T>& ptr) {
        void* ptr_mem = lua_newuserdata(L, sizeof(std::shared_ptr<T>));
        new(ptr_mem) std::shared_ptr<T>(ptr);

        lua_newtable(L);

        lua_pushstring(L, "__gc");
        lua_pushcfunction(L, [](lua_State* L) -> int {
            std::shared_ptr<T>* ptr = (std::shared_ptr<T>*)lua_touserdata(L, 1);
            ptr->~shared_ptr();
            return 0;
        });
        lua_settable(L, -3);

        lua_setmetatable(L, -2);
    }

    template<typename T>
    std::shared_ptr<T> pop_shared_ptr_from_lua(lua_State* L, int index) {
        std::shared_ptr<T>* ptr = (std::shared_ptr<T>*)lua_touserdata(L, index);
        if (ptr) {
            return *ptr;
        }
        return nullptr;
    }

    template<typename T>
    void push_raw_ptr_to_lua(lua_State* L, T* ptr) {
        T** ptr_mem = (T**)lua_newuserdata(L, sizeof(T*));
        *ptr_mem = ptr;
    }

    template<typename T>
    T* pop_raw_ptr_from_lua(lua_State* L, int index) {
        T** ptr = (T**)lua_touserdata(L, index);
        if (ptr) {
            return *ptr;
        }
        return nullptr;
    }
}
