#pragma once

void ltrim(std::string& s);
void rtrim(std::string& s);
void trim(std::string& s);

spdlog::sink_ptr CreateCustomSinkST(std::function<void(const std::string&)> aSinkItHandler, std::function<void()> aFlushHandler = nullptr);
spdlog::sink_ptr CreateCustomSinkMT(std::function<void(const std::string&)> aSinkItHandler, std::function<void()> aFlushHandler = nullptr);
std::shared_ptr<spdlog::logger> CreateLogger(const std::filesystem::path& aPath, const std::string& aID, spdlog::sink_ptr aExtraSink = nullptr, const std::string& aPattern = "[%Y-%m-%d %H:%M:%S UTC%z] [%l] [%!] %v");

// deep copies sol object (doesnt take into account potential duplicates)
sol::object DeepCopySolObject(sol::object aObj, const sol::state_view& aStateView);

// makes sol usertype or userdata immutable when accessed from lua
void MakeSolUsertypeImmutable(sol::object aObj, const sol::state_view& aStateView);

// Add unnamed function to the Lua registry
template<typename F>
sol::function MakeSolFunction(sol::state& aState, F aFunc)
{
    // This is slightly better than wrapping lambdas in sol::object:
    // 1. When the lambda is wrapped in an object sol registers additional usertype for the lambda type.
    // 2. Calling a lambda as an object has a tiny overhead of dealing with metatables.
    // 3. In Lua `type(f)` for a lambda as an object will return "userdata" instead of the expected "function".

    static constexpr const char* s_cTempFuncName = "___func_temp_holder_";

    aState[s_cTempFuncName] = aFunc;
    sol::function luaFunc = aState[s_cTempFuncName];
    aState[s_cTempFuncName] = sol::nil;

    return luaFunc;
}

// Check if Lua object is of cdata type
bool IsLuaCData(sol::object aObject);