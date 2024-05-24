#include "jsonlib/debug.hpp"
#include "jsonlib/jsonlib.hpp"

using namespace jsonlib;

auto main() -> int {
    std::string json_string = "null";

    auto obj = Json::deserialize(json_string);
    LOG_INFO("obj.serialize(): {}", obj.serialize());
    ASSERT(json_string == obj.serialize());
}
