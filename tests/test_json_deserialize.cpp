#include "debug.hpp"
#include "jsonlib.hpp"

using namespace jsonlib;

auto test_null() {
    std::string json_string = "null";

    auto obj = Json::deserialize(json_string);
    LOG_INFO("obj.serialize(): {}", obj.serialize());
    ASSERT(json_string == obj.serialize());
}

auto test_boolean() {
    std::string json_string = "true";

    auto obj = Json::deserialize(json_string);
    LOG_INFO("obj.serialize(): {}", obj.serialize());
    ASSERT(json_string == obj.serialize());

    std::string json_string2 = "false";

    auto obj2 = Json::deserialize(json_string2);
    LOG_INFO("obj2.serialize(): {}", obj2.serialize());
    ASSERT(json_string2 == obj2.serialize());
}

auto test_number() {
    std::string json_string = "-123";

    auto obj = Json::deserialize(json_string);
    LOG_INFO("obj.serialize(): {}", obj.serialize());
    ASSERT(json_string == obj.serialize());

    std::string json_string2 = "3.14";

    auto obj2 = Json::deserialize(json_string2);
    LOG_INFO("obj2.serialize(): {}", obj2.serialize());
    ASSERT(json_string2 == obj2.serialize());
}

auto test_string() {
    std::string json_string = R"("hello world")";

    auto obj = Json::deserialize(json_string);
    LOG_INFO("obj.serialize(): {}", obj.serialize());
    ASSERT(json_string == obj.serialize());
}

auto test_string_with_escaped_char() {
    std::string json_string = R"("hello\\r\\n world\\r\\n")";

    auto obj = Json::deserialize(json_string);
    LOG_INFO("obj.serialize(): {}", obj.serialize());
    ASSERT(json_string == obj.serialize());
    ASSERT(obj.string() == R"(hello\r\n world\r\n)");
}

auto test_array() {
    std::string json_string = R"(["R", "G", "B"])";

    auto obj = Json::deserialize(json_string);
    LOG_INFO("obj.serialize(): {}", obj.serialize());
    ASSERT(json_string == obj.serialize());
}

auto test_object() {
    std::string json_string = R"({"pi": 3.14, "rule": {"work": 996}})";

    auto obj = Json::deserialize(json_string);
    LOG_INFO("obj.serialize(): {}", obj.serialize());
    ASSERT(json_string == obj.serialize());
}

auto main() -> int {
    test_null();
    test_boolean();
    test_number();
    test_string();
    test_string_with_escaped_char();
    test_array();
    test_object();
}
