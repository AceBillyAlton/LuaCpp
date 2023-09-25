#include <iostream>
#include <string>
#include <cassert>

extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}
namespace {

    int CallLuaAdd(lua_State* InLuaState, int InLo, int InRo)
    {
        // Fetch it from the global table and push it to the top of the stack
        lua_getglobal(InLuaState, "Add");

        // push arguments to stack
        lua_pushnumber(InLuaState, InLo);
        lua_pushnumber(InLuaState, InRo);

        lua_call(InLuaState, 2, 1);

        const int Result = lua_tointeger(InLuaState, -1);

        // pop stack
        lua_pop(InLuaState, 1);

        return Result;
    }

    static int LuaCallCxxAdd(lua_State* InLuaState) 
    {
        int Result{0};
        int ArgumentsCount = lua_gettop(InLuaState);
        for (size_t Index = 1; Index < ArgumentsCount; ++Index)
        {
            Result += lua_tointeger(InLuaState, lua_upvalueindex(Index));
        }
             
        lua_pushnumber(InLuaState, Result);

        // The number of return values obtained by lua
        return 1;
    }
}

int main() 
{
    // create lua instance 
    auto *LuaState = luaL_newstate();
    
    // load lua standard lib to LuaState;
    luaL_openlibs(LuaState);

    // const luaL_Reg Funcs[1] { {.name = "LuaCallCxxAdd", .func=LuaCallCxxAdd}};
    // luaL_newlibtable(LuaState, Funcs);
    // luaL_setfuncs(LuaState, Funcs, 3);

    // load script file
    if(luaL_loadfile(LuaState, "src/lua/Test.lua") != LUA_OK) 
    {
        return -1;
    }

    // run script -- test call cxx function
    lua_pcall(LuaState, 0, 0, 0);

    // get lua global variable
    lua_getglobal(LuaState, "LuaGlobalVariable");
    if(lua_isstring(LuaState, -1) != 0) 
    {
        const auto LuaGlobalVariable = std::string(lua_tostring(LuaState, -1));
        assert(LuaGlobalVariable == "This is string variable");
    }
    else return -1;

    // get lua table
    lua_getglobal(LuaState, "LuaTable");
    lua_pushstring(LuaState, "Name");
    lua_gettable(LuaState, -2);
    lua_pushstring(LuaState, "NameId");
    lua_gettable(LuaState, -3);

    const auto Name = std::string(lua_tostring(LuaState, -2));
    const int NameId = lua_tointeger(LuaState, -1);
    assert(Name == "LuaTable" && NameId == 10);

    // --call lua function
    assert(CallLuaAdd(LuaState, 1, 2)==3);
    
    // -- modifier lua variable
      // -- modifier lua global variable
    lua_pushstring(LuaState, "Modifier in C++");
    lua_setglobal(LuaState, "LuaModifiesVariable");

    lua_getglobal(LuaState, "LuaModifiesVariable");
    const auto LuaModifiesVariable = std::string(lua_tostring(LuaState, -1));
    assert(LuaModifiesVariable == "Modifier in C++");
      // -- set lua table variable

    lua_getglobal(LuaState, "LuaModifiesTable");
    
    lua_pushstring(LuaState, "Modifier in C++ table");
    lua_setfield(LuaState, -2, "Name");

    lua_getglobal(LuaState, "LuaModifiesTable");
    lua_getfield(LuaState, -1, "Name");
    const auto LuaModifiesTableName = std::string(lua_tostring(LuaState, -1));
    assert(LuaModifiesTableName == "Modifier in C++ table");

    lua_close(LuaState);

    return 0;
}