-- 00 
print("-----LuaCppSimpleTest-----")

-- cxx call lua function
function Add(LeftOperand, RightOperand)
    local Result = LeftOperand + RightOperand;
    return Result;
end

-- call cxx function
-- local AddResult = LuaCallCxxAdd(1, 2, 3);
-- print("AddResult", AddResult)
-- assert(AddResult == 6);

-- lua global variable
LuaGlobalVariable = "This is string variable";

-- table
LuaTable = {Name="LuaTable", NameId = 10};

-- c++ modifies variables in lua
LuaModifiesVariable = "Native Lua";
LuaModifiesTable = {Name="Native lua table member", S = "None"};