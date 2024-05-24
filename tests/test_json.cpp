#include "jsonlib/debug.hpp"
#include "jsonlib/jsonlib.hpp"

using namespace jsonlib;

auto main() -> int {
    SET_LOG_STYLE(log::LogStyle::BG);
    Json obj;
    obj["bool"] = true;
    auto ret = obj.serialize();
    LOG_INFO("`{}`", ret);
    ASSERT_MSG(ret == R"({"bool": true})", "serialize error");

    // auto obj2 = Json::deserialize(ret);
    // ASSERT_MSG(obj2["bool"] == true, "deserialize error");
}
