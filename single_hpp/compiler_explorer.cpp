#include "https://raw.githubusercontent.com/uchenily/jsonlib/main/single_hpp/single_jsonlib.hpp"

using namespace jsonlib;

auto main() -> int {
    std::string json_string = R"({
      "Table": "Farm",
      "Columns": {
        "farm_name": {
          "type": "str",
          "block_stats": {
            "0": {
              "num": 6,
              "min": "Breakfast Lunch Dinner",
              "max": "Incubator",
              "min_len": 9,
              "max_len": 22
            }
          },
          "num_blocks": 1,
          "start_offset": 0
        },
        "min_age_weeks": {
          "type": "float",
          "block_stats": {
            "0": {
              "num": 6,
              "min": 0,
              "max": 52
            }
          },
          "num_blocks": 1,
          "start_offset": 192
        },
        "max_age_weeks": {
          "type": "float",
          "block_stats": {
            "0": {
              "num": 6,
              "min": 2,
              "max": 156
            }
          },
          "num_blocks": 1,
          "start_offset": 216
        }
      },
      "Max Values Per Block": 50000
    })";

    auto obj = Json::deserialize(json_string);
    LOG_INFO("obj.serialize(): {}", obj.serialize());
    ASSERT(obj["Table"].string() == "Farm");
    ASSERT(obj["Columns"]["farm_name"]["type"].string() == "str");
    ASSERT((int) obj["Columns"]["farm_name"]["block_stats"]["0"]["num"].number()
           == 6);
    ASSERT((int) obj["Max Values Per Block"].number() == 50000);
}
