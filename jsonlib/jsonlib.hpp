#pragma once

#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace jsonlib {

class Json {
    struct Data {
        std::shared_ptr<std::map<std::string, Json>> object_;
        std::shared_ptr<std::vector<Json>>           array_;
        double                                       number_{};
        std::string                                  string_;
    };
    enum class Type {
        OBJECT,
        ARRAY,
        STRING,
        NUMBER,
        TRUE,
        FALSE,
        NULL_,
    };

    using enum Type;

public:
    Json()
        : type_{NULL_} {}

    Json(bool value)
        : type_{((value) ? TRUE : FALSE)} {}

    auto operator[](const std::string &key) -> Json & {
        if (!is_object()) {
            create_object();
        }
        auto value = data_.object_->find(key);
        if (value != data_.object_->end()) {
            return value->second;
        }

        return (*data_.object_)[key];
    }

    auto operator==(bool value) -> bool {
        if (type_ == TRUE) {
            return value;
        } else if (type_ == FALSE) {
            return !value;
        }
        return false;
    }

public:
    auto serialize() const -> std::string {
        std::ostringstream out;
        serialize_to(out);
        return out.str();
    }

    static auto deserialize([[maybe_unused]] std::string_view input) -> Json {
        return Json{false};
    }

private:
    auto is_object() const noexcept -> bool {
        return type_ == OBJECT;
    }

    auto create_object() noexcept -> void {
        type_ = OBJECT;
        data_.object_ = std::make_shared<std::map<std::string, Json>>();
    }

    auto serialize_to(std::ostringstream &out) const -> void {
        switch (type_) {
        case OBJECT:
            serialize_object(out);
            break;
        case ARRAY:
            serialize_array(out);
            break;
        case STRING:
            out << '"';
            out << data_.string_;
            out << '"';
            break;
        case NUMBER:
        case TRUE:
            out << "true";
            break;
        case FALSE:
            out << "false";
            break;
        case NULL_:
            out << "null";
            break;
        default:
            out << "?";
        }
    }

    auto serialize_object(std::ostringstream &out) const noexcept -> void {
        auto        n = data_.object_->size();
        std::size_t i{0};

        out << '{';
        for (auto &it : *data_.object_) {
            out << '"' << it.first << '"';
            out << ": ";
            it.second.serialize_to(out);
            if (++i < n) {
                out << ',';
            }
        }
        out << '}';
    }

    auto serialize_array(std::ostringstream &out) const noexcept -> void {
        auto        n = data_.array_->size();
        std::size_t i{0};

        for (auto &it : *data_.array_) {
            out << '[';
            it.serialize_to(out);
            if (++i < n) {
                out << ',';
            }
        }
        out << ']';
    }

private:
    Data data_;
    Type type_;
};
} // namespace jsonlib
