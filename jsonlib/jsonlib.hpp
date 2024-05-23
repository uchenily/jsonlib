#pragma once

#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace jsonlib {

class Json {
    using array_t = std::vector<Json>;
    using object_t = std::map<std::string, Json>;
    struct Data {
        std::shared_ptr<object_t> object_;
        std::shared_ptr<array_t>  array_;
        double                    number_{};
        std::string               string_;
    };
    enum class Type {
        Object,
        Array,
        String,
        Number,
        True,
        False,
        Null,
    };

    using enum Type;

public:
    Json()
        : type_{Null} {}

    Json(bool value)
        : type_{((value) ? True : False)} {}

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

    auto operator==(const Json &another) const noexcept -> bool {
        if (this->type_ != another.type_) {
            return false;
        }

        switch (this->type_) {
        case Null:
        case True:
        case False:
            return true;
        case String:
            return this->data_.string_ == another.data_.string_;
        case Number:
            return this->data_.number_ == another.data_.number_; // ?
        case Array:
            return *this->data_.array_ == *another.data_.array_;
        case Object:
            return *this->data_.object_ == *another.data_.object_;
        }
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
        return type_ == Object;
    }

    auto is_array() const noexcept -> bool {
        return type_ == Array;
    }

    auto is_string() const noexcept -> bool {
        return type_ == String;
    }
    auto is_boolean() const noexcept -> bool {
        return type_ == False || type_ == True;
    }

    auto is_null() const noexcept -> bool {
        return type_ == Null;
    };

    auto is_number() const noexcept -> bool {
        return type_ == Number;
    }

    // auto to_number() const noexcept -> double {
    //     return data_.number_;
    // }

    auto create_object() noexcept -> void {
        type_ = Object;
        data_.object_ = std::make_shared<std::map<std::string, Json>>();
    }

    auto serialize_to(std::ostringstream &out) const -> void {
        switch (type_) {
        case Object:
            serialize_object(out);
            break;
        case Array:
            serialize_array(out);
            break;
        case String:
            out << '"';
            out << data_.string_;
            out << '"';
            break;
        case Number:
        case True:
            out << "true";
            break;
        case False:
            out << "false";
            break;
        case Null:
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
