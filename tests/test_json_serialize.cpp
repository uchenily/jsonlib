#include "debug.hpp"
#include "jsonlib.hpp"

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

auto test_array() {
    Json obj;
    obj["array"] = {"R", "G", "B"};
    obj["empty"] = {};
    auto ret = obj.serialize();
    LOG_INFO("`{}`", ret);
    ASSERT_MSG(ret == R"({"array": ["R", "G", "B"], "empty": null})",
               "serialize error");
}

auto test_array2() {
    Json obj;
    obj["array"] = {"string", 100, true};
    auto ret = obj.serialize();
    LOG_INFO("`{}`", ret);
    ASSERT_MSG(ret == R"({"array": ["string", 100, true]})", "serialize error");
}

auto test_object() {
    Json obj;
    Json nested_obj1;
    Json nested_obj2;
    obj["level1"] = nested_obj1;
    nested_obj1["level2"] = nested_obj2;
    nested_obj2["level3"] = Json();
    auto ret = obj.serialize();
    LOG_INFO("`{}`", ret);
    ASSERT_MSG(ret == R"({"level1": {"level2": {"level3": null}}})",
               "serialize error");
}

auto main() -> int {
    SET_LOG_STYLE(log::LogStyle::BG);
    test_null();
    test_bool();
    test_number();
    test_string();
    test_array();
    test_array2();
    test_object();
}
