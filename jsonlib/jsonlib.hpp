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

    struct Value {
        Type type_;
        Data data_;

        Value()
            : type_{Null} {}

        Value(bool value)
            : type_{value ? True : False} {}

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

            for (auto &it : *data_.array_) {
                out << '[';
                it.value_->serialize_to(out);
                if (++i < n) {
                    out << ", ";
                }
            }
            out << ']';
        }
    };

public:
    Json([[maybe_unused]] std::nullptr_t null = nullptr)
        : value_{std::make_unique<Value>()} {}

    Json(bool value)
        : value_{std::make_unique<Value>(value)} {}

    auto operator[](const std::string &key) -> Json & {
        if (!value_->is<Object>()) {
            value_->to<Object>();
        }
        auto value = value_->as<Object>()->find(key);
        if (value != value_->as<Object>()->end()) {
            return value->second;
        }

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

    static auto deserialize([[maybe_unused]] std::string_view input) -> Json {
        return Json{false};
    }

private:
    std::unique_ptr<Value> value_;
};
} // namespace jsonlib
