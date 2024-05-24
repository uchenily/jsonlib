#pragma once

#include <initializer_list>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "jsonlib/debug.hpp"

namespace jsonlib {

class Json {
    using array_t = std::vector<Json>;
    using object_t = std::map<std::string, Json>;
    struct Data {
        std::shared_ptr<object_t> object_;
        std::shared_ptr<array_t>  array_;
        double                    number_{};
        std::string_view          string_;
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

    struct Value {
        Type type_;
        Data data_;

        Value()
            : type_{Null} {}

        Value(bool value)
            : type_{value ? True : False} {}

        template <typename T>
            requires(std::is_floating_point_v<T> || std::is_integral_v<T>)
        Value(T value)
            : type_{Number} {
            data_.number_ = value;
        }

        template <typename T>
            requires(std::is_convertible_v<T, std::string_view>)
        Value(T value)
            : type_{String} {
            data_.string_ = value;
        }

        Value(std::initializer_list<Json> values)
            : type_{Array} {
            data_.array_ = std::make_shared<array_t>(values);
        }

        auto operator==(const Value &another) const noexcept -> bool {
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

        template <Type T>
        auto is() -> bool {
            return type_ == T;
        }

        template <Type T>
        auto to() -> void {
            type_ = T;
        }

        template <Type T>
        auto as() {
            if constexpr (T == Null) {
                return nullptr;
            } else if constexpr (T == False) {
                return false;
            } else if constexpr (T == True) {
                return true;
            } else if constexpr (T == Number) {
                return data_.number_;
            } else if constexpr (T == String) {
                return data_.string_;
            } else if constexpr (T == Array) {
                if (!data_.array_) {
                    data_.array_ = std::make_shared<array_t>();
                }
                return data_.array_;
            } else if constexpr (T == Object) {
                if (!data_.object_) {
                    data_.object_ = std::make_shared<object_t>();
                }
                return data_.object_;
            }
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
                // FIXME(x): string may contains special characters, eg. '{',
                // '"', '[' ...
                out << '"';
                out << data_.string_;
                out << '"';
                break;
            case Number:
                out << data_.number_;
                break;
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

        // static auto deserialize_from(std::istringstream &in) -> Value {
        static auto deserialize_from(std::string_view in) -> Value {
            switch (in[0]) {
            case 'n':
                return deserialize_null(in);
            case 't':
                return deserialize_true(in);
            case 'f':
                return deserialize_false(in);
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                return deserialize_number(in);
            case '"':
                return deserialize_string(in);
            case '[':
                return deserialize_array(in);
            case '{':
                return deserialize_object(in);
            default:
                break;
            }
            return {};
        }

    private:
        auto serialize_object(std::ostringstream &out) const noexcept -> void {
            auto        n = data_.object_->size();
            std::size_t i{0};

            out << '{';
            for (auto &it : *data_.object_) {
                out << '"' << it.first << '"';
                out << ": ";
                it.second.value_->serialize_to(out);
                if (++i < n) {
                    out << ", ";
                }
            }
            out << '}';
        }

        auto serialize_array(std::ostringstream &out) const noexcept -> void {
            auto        n = data_.array_->size();
            std::size_t i{0};

            out << '[';
            for (auto &it : *data_.array_) {
                it.value_->serialize_to(out);
                if (++i < n) {
                    out << ", ";
                }
            }
            out << ']';
        }

        static auto deserialize_null(std::string_view in) -> Value {
            ASSERT(in.substr(0, 4) == "null");
            return {};
        }
        static auto deserialize_false(std::string_view in) -> Value {
            ASSERT(in.substr(0, 5) == "false");
            return false;
        }
        static auto deserialize_true(std::string_view in) -> Value {
            ASSERT(in.substr(0, 4) == "true");
            return true;
        }
        static auto deserialize_string(std::string_view in) -> Value {
            auto length = in.length();
            ASSERT(in[0] == '"');
            auto pos = 0u;
            pos++;
            while (pos < length && in[pos] != '"') {
                pos++;
            }
            ASSERT(in[pos] == '"');
            pos++;
            return in.substr(1, pos - 2);
        }
        static auto deserialize_number(std::string_view in) -> Value {
            auto length = in.length();
            // LOG_DEBUG("in `{}` length = {}", in, in.length());
            auto pos = 0u;
            if (pos < length && in[pos] == '-') {
                pos++;
            }
            while (pos < length && '0' <= in[pos] && in[pos] <= '9') {
                pos++;
            }
            if (pos < length && in[pos] == '.') {
                pos++;
                while (pos < length && '0' <= in[pos] && in[pos] <= '9') {
                    pos++;
                }
                return std::stod(std::string{in.substr(0, pos)});
            }
            return std::stoi(std::string{in.substr(0, pos)});
        }
        static auto deserialize_array(std::string_view in) -> Value {
            return {};
        }
        static auto deserialize_object(std::string_view in) -> Value {
            return {};
        }
    };

public:
    Json([[maybe_unused]] std::nullptr_t null = nullptr)
        : value_{std::make_shared<Value>()} {}

    template <typename T>
        requires(std::is_same_v<T, bool> || std::is_floating_point_v<T>
                 || std::is_integral_v<T>
                 || std::convertible_to<T, std::string_view>)
    Json(T value)
        : value_{std::make_shared<Value>(value)} {}

    Json(std::initializer_list<Json> values)
        : value_{std::make_shared<Value>(values)} {}

    Json(const Value &value)
        : value_{std::make_shared<Value>(value)} {}

    Json(Json &&other) noexcept
        : value_{std::move(other.value_)} {
        other.value_ = nullptr;
    }

    auto operator=(Json &&other) noexcept -> Json & {
        if (std::addressof(other) != this) {
            value_ = std::move(other.value_);
            other.value_ = nullptr;
        }
        return *this;
    }

    Json(const Json &other) = default;
    auto operator=(const Json &other) -> Json & = default;

    auto operator[](const std::string &key) -> Json & {
        if (!value_->is<Object>()) {
            value_->to<Object>();
        }
        // auto value = value_->as<Object>()->find(key);
        // if (value != value_->as<Object>()->end()) {
        //     return value->second;
        // }

        return (*value_->as<Object>())[key];
    }

    auto operator==(const Json &another) const noexcept -> bool {
        return this->value_ == another.value_;
    }

public:
    auto serialize() const -> std::string {
        std::ostringstream out;
        value_->serialize_to(out);
        return out.str();
    }

    static auto deserialize(const std::string &str) -> Json {
        if (str.empty()) {
            return {nullptr};
        }
        // std::istringstream in{str};
        return Value::deserialize_from(str);
    }

    auto number() const {
        return value_->as<Number>();
    }

private:
private:
    std::shared_ptr<Value> value_;
};
} // namespace jsonlib
