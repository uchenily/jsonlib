#include "jsonlib/debug.hpp"
#include "jsonlib/jsonlib.hpp"

using namespace jsonlib;

auto test_null() {
    Json obj;
    obj["null"] = nullptr;
    auto ret = obj.serialize();
    LOG_INFO("`{}`", ret);
    ASSERT_MSG(ret == R"({"null": null})", "serialize error");
}

auto test_bool() {
    Json obj;
    obj["bool1"] = true;
    obj["bool2"] = false;
    auto ret = obj.serialize();
    LOG_INFO("`{}`", ret);
    ASSERT_MSG(ret == R"({"bool1": true, "bool2": false})", "serialize error");
}

auto test_number() {
    Json obj;
    obj["number1"] = 1;
    obj["number2"] = 3.14;
    auto ret = obj.serialize();
    LOG_INFO("`{}`", ret);
    ASSERT_MSG(ret == R"({"number1": 1, "number2": 3.14})", "serialize error");
}

auto test_string() {
    Json obj;
    obj["hello"] = "world";
    obj["empty"] = "";
    auto ret = obj.serialize();
    LOG_INFO("`{}`", ret);
    // NOTE: std::map sort by key
    ASSERT_MSG(ret == R"({"empty": "", "hello": "world"})", "serialize error");
}

auto main() -> int {
    SET_LOG_STYLE(log::LogStyle::BG);
    test_null();
    test_bool();
    test_number();
    test_string();
}
