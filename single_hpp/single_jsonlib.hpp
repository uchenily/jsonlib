// DO NOT EDIT !!! This file was generated with a script.
//
// Generator Version: 0.0.1
// Generated Time: 2024-07-18 15:10:39.909054

#pragma once

// C++
#include <array>
#include <chrono>
#include <format>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <source_location>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>
#include <vector>

namespace print_hpp::log {
enum class LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
};
enum class LogStyle {
    FG,
    BG,
};
namespace detail {
    constexpr auto to_string(LogLevel level) noexcept -> std::string_view {
        using enum LogLevel;
        switch (level) {
        case TRACE:
            return "TRACE";
        case DEBUG:
            return "DEBUG";
        case INFO:
            return "INFO ";
        case WARN:
            return "WARN ";
        case ERROR:
            return "ERROR";
        case FATAL:
            return "FATAL";
        default:
            return "UNKNOWN";
        }
    }
    constexpr auto to_color_fg(LogLevel level) noexcept -> std::string_view {
        using enum LogLevel;
        switch (level) {
        case TRACE:
            return "\033[36m";
        case DEBUG:
            return "\033[94m";
        case INFO:
            return "\033[32m";
        case WARN:
            return "\033[33m";
        case ERROR:
            return "\033[31m";
        case FATAL:
            return "\033[35m";
        default:
            return "";
        }
    }
    constexpr auto to_color_bg(LogLevel level) noexcept -> std::string_view {
        switch (level) {
            using enum LogLevel;
        case TRACE:
            return "\033[97;46m"; // cyan
        case DEBUG:
            return "\033[97;44m"; // blue
        case INFO:
            return "\033[97;42m"; // green
        case WARN:
            return "\033[90;43m"; // yellow
        case ERROR:
            return "\033[97;41m"; // red
        case FATAL:
            return "\033[97;45m"; // purple
        default:
            return "";
        }
    }
    constexpr auto to_color(LogLevel level, LogStyle style) noexcept
        -> std::string_view {
        using enum LogStyle;
        switch (style) {
        case FG:
            return to_color_fg(level);
        case BG:
            return to_color_bg(level);
        default:
            return "";
        }
    }
    template <LogLevel Level>
    auto function_name(std::source_location loc) noexcept -> std::string {
        if constexpr (Level <= LogLevel::DEBUG) {
            return std::format("|{}|", loc.function_name());
        } else {
            return {};
        }
    }
    constexpr auto source_color() noexcept -> std::string_view {
        return "\033[90m";
    }
    constexpr auto reset_color() noexcept -> std::string_view {
        return "\033[0m";
    }
    template <int N, char c>
    inline auto to_int(uint64_t num, std::span<char> p, int &size) {
        constexpr static std::array<char, 10> digits
            = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
        for (int i = 0; i < N; i++) {
            p[--size] = digits.at(num % 10);
            num = num / 10;
        }
        if constexpr (N != 4) {
            p[--size] = c;
        }
    }
    inline auto get_timestamp(const std::chrono::system_clock::time_point &now)
        -> char * {
        static thread_local std::array<char, 33> buf{};
        static thread_local std::chrono::seconds last_second{};
        std::chrono::system_clock::duration duration = now.time_since_epoch();
        std::chrono::seconds                seconds
            = std::chrono::duration_cast<std::chrono::seconds>(duration);
        auto milliseconds
            = std::chrono::duration_cast<std::chrono::milliseconds>(duration
                                                                    - seconds)
                  .count();
        int size = 23;
        if (last_second == seconds) {
            to_int<3, '.'>(milliseconds, buf, size);
            return buf.data();
        }
        last_second = seconds;
        auto tt = std::chrono::system_clock::to_time_t(now);
        auto tm = localtime(&tt);
        to_int<3, '.'>(milliseconds, buf, size);
        to_int<2, ':'>(tm->tm_sec, buf, size);
        to_int<2, ':'>(tm->tm_min, buf, size);
        to_int<2, ' '>(tm->tm_hour, buf, size);
        to_int<2, '-'>(tm->tm_mday, buf, size);
        to_int<2, '-'>(tm->tm_mon + 1, buf, size);
        to_int<4, ' '>(tm->tm_year + 1900, buf, size);
        return buf.data();
    }
    class FmtWithSourceLocation {
    public:
        template <typename T>
            requires std::constructible_from<std::string_view, T>
        FmtWithSourceLocation(T                  &&fmt,
                              std::source_location location
                              = std::source_location::current())
            : fmt_{std::forward<T>(fmt)}
            , source_location_{location} {}

    public:
        auto fmt() {
            return fmt_;
        }
        auto source_location() {
            return source_location_;
        }

    private:
        std::string_view     fmt_;
        std::source_location source_location_;
    };
    class ConsoleLogger {
    public:
        template <typename... Args>
        auto trace(FmtWithSourceLocation fwsl, Args &&...args) {
            log<LogLevel::TRACE>(fwsl, std::forward<Args>(args)...);
        }
        template <typename... Args>
        auto debug(FmtWithSourceLocation fwsl, Args &&...args) {
            log<LogLevel::DEBUG>(fwsl, std::forward<Args>(args)...);
        }
        template <typename... Args>
        auto info(FmtWithSourceLocation fwsl, Args &&...args) {
            log<LogLevel::INFO>(fwsl, std::forward<Args>(args)...);
        }
        template <typename... Args>
        auto warn(FmtWithSourceLocation fwsl, Args &&...args) {
            log<LogLevel::WARN>(fwsl, std::forward<Args>(args)...);
        }
        template <typename... Args>
        auto error(FmtWithSourceLocation fwsl, Args &&...args) {
            log<LogLevel::ERROR>(fwsl, std::forward<Args>(args)...);
        }
        template <typename... Args>
        auto fatal(FmtWithSourceLocation fwsl, Args &&...args) {
            log<LogLevel::FATAL>(fwsl, std::forward<Args>(args)...);
        }
        template <typename Fmt, typename... Args>
            requires std::constructible_from<std::string_view, Fmt>
        auto printf(Fmt fmt, Args... args) {
            std::clog << std::vformat(fmt, std::make_format_args(args...));
        }
        auto set_level(LogLevel level) {
            level_ = level;
        }
        auto set_style(LogStyle style) {
            style_ = style;
        }

    private:
        template <LogLevel Level, typename... Args>
        auto log(FmtWithSourceLocation fwsl, const Args &...args) {
            if (Level < level_) {
                return;
            }
            auto fmt = fwsl.fmt();
            auto source_location = fwsl.source_location();
            auto message = std::vformat(fmt, std::make_format_args(args...));
            auto now = std::chrono::system_clock::now();
            std::clog << std::format("{} |{}{:<5}{}| {}{}:{}{}{} {}\n",
                                     get_timestamp(now),
                                     to_color(Level, style_),
                                     to_string(Level),
                                     reset_color(),
                                     source_color(),
                                     source_location.file_name(),
                                     source_location.line(),
                                     function_name<Level>(source_location),
                                     reset_color(),
                                     message);
        }

    private:
        LogLevel level_{LogLevel::DEBUG};
        LogStyle style_{LogStyle::FG};
    };
} // namespace detail
inline auto console = detail::ConsoleLogger();
} // namespace print_hpp::log

#include <ostream>
#include <ranges>
#include <sstream>

namespace print_custom {
template <typename T>
struct printer {
    printer() = delete;
    printer(const printer &) = delete;
    auto operator=(const printer &) = delete;
};
template <typename T>
inline auto print_to(std::ostream &out, T &&t);
} // namespace print_custom
namespace print_detail {
template <typename T>
constexpr auto name_of() -> std::string_view {
    std::string_view sv;
#if __clang__
    sv = __PRETTY_FUNCTION__;
    sv = sv.substr(sv.find_last_of('=') + 2);
    sv = sv.substr(0, sv.size() - 1);
#elif __GNUC__
    sv = __PRETTY_FUNCTION__;
    sv = sv.substr(sv.find_first_of('=') + 2);
    sv = sv.substr(0, sv.find_first_of(';'));
#else
#error "name_of not implemented on this platform"
#endif
    return sv;
}
template <typename Container>
concept is_iterable_v = requires(Container &c) {
    std::ranges::begin(c);
    std::ranges::end(c);
};
template <typename Container>
concept is_mappable_v = requires(Container &c) {
    typename Container::key_type;
    typename Container::mapped_type;
    {
        c[std::declval<const typename Container::key_type &>()]
    } -> std::same_as<typename Container::mapped_type &>;
};
template <typename T>
concept is_variant_v = requires { std::variant_size<T>::value; };
template <typename T>
concept is_optional_v = requires(T &t) {
    { t.has_value() } -> std::same_as<bool>;
    { t.value() } -> std::convertible_to<T>;
};
template <typename Container>
concept is_tuple_v = requires { std::tuple_size<Container>::value; };
template <typename T>
concept has_printer_print_v = requires(print_custom::printer<T> &printer,
                                       const T                  &t,
                                       std::ostream             &out) {
    { printer.print(t, out) };
};
template <typename T>
auto print_to(std::ostream &out, const T &t) {
    if constexpr (std::is_convertible_v<T, std::string_view>) {
        out << '"' << t << '"';
    } else if constexpr ((std::is_integral_v<T> && !std::is_same_v<T, bool>)
                         || std::is_floating_point_v<T>) {
        out << t;
    } else if constexpr (std::is_same_v<T, bool>) {
        out << (t ? "true" : "false");
    } else if constexpr (is_mappable_v<T>) {
        out << '{';
        auto first = true;
        for (const auto &elem : t) {
            if (!first) {
                out << ", ";
            }
            first = false;
            print_to(out, elem.first);
            out << ": ";
            print_to(out, elem.second);
        }
        out << "}";
    } else if constexpr (is_iterable_v<T>) {
        out << '[';
        auto first = true;
        for (const auto &elem : t) {
            if (!first) {
                out << ", ";
            }
            first = false;
            print_to(out, elem);
        }
        out << "]";
    } else if constexpr (is_variant_v<T>) {
        std::visit(
            [&out](const auto &value) {
                print_to(out, value);
            },
            t);
    } else if constexpr (is_optional_v<T>) {
        if (t) {
            print_to(out, t.value());
        } else {
            out << "nullopt";
        }
    } else if constexpr (std::is_same_v<T, std::nullopt_t>) {
        out << "nullopt";
    } else if constexpr (is_tuple_v<T>) {
        out << '(';
        auto first = true;
        std::apply(
            [&out, &first](auto &&...args) {
                (([&out, &first, &args] {
                     if (!first) {
                         out << ", ";
                     }
                     print_to(out, std::forward<decltype(args)>(args));
                     first = false;
                 }()),
                 ...);
            },
            t);
        out << ')';
    } else if constexpr (has_printer_print_v<T>) {
        print_custom::printer<T>().print(t, out);
    } else {
        out << "unprintable type " << name_of<T>();
    }
}
} // namespace print_detail
template <typename T>
auto print_custom::print_to(std::ostream &out, T &&t) {
    print_detail::print_to(out, std::forward<T>(t));
}
namespace print_hpp {
template <typename T>
auto print(T &&t) {
    print_detail::print_to(std::cout, std::forward<T>(t));
    std::cout << '\n';
}
template <typename T, typename... Ts>
void print(T &&t, Ts &&...ts) {
    print_detail::print_to(std::cout, std::forward<T>(t));
    ((std::cout << ' ',
      print_detail::print_to(std::cout, std::forward<Ts>(ts))),
     ...);
    std::cout << '\n';
}
// pretty print
template <typename T>
auto P(T &&t) {
    std::ostringstream oss; // 千万别用 `std::ostrstream` !
    print_detail::print_to(oss, std::forward<T>(t));
    return oss.str();
}
} // namespace print_hpp

inline auto &debug_logger = print_hpp::log::console;
#define PRINT_FMT(...) debug_logger.printf(__VA_ARGS__)
#if !defined(NDEBUG)
#define SET_LOG_LEVEL(level) debug_logger.set_level(level)
#define SET_LOG_STYLE(style) debug_logger.set_style(style)
#define LOG_TRACE(...) debug_logger.trace(__VA_ARGS__)
#define LOG_INFO(...) debug_logger.info(__VA_ARGS__)
#define LOG_DEBUG(...) debug_logger.debug(__VA_ARGS__)
#define LOG_WARN(...) debug_logger.warn(__VA_ARGS__)
#define LOG_ERROR(...) debug_logger.error(__VA_ARGS__)
#define LOG_FATAL(...) debug_logger.fatal(__VA_ARGS__)
#else
#define SET_LOG_LEVEL(level)
#define SET_LOG_STYLE(style)
#define LOG_TRACE(...)
#define LOG_INFO(...)
#define LOG_DEBUG(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_FATAL(...)
#endif
#if !defined(NDEBUG)
#include <cassert>
#define ASSERT(x) assert(x)
#define ASSERT_MSG(x, msg) ASSERT((x) && (msg))
#else
#define ASSERT(x)
#define ASSERT_MSG(x, msg)
#endif

#include <initializer_list>

inline const std::array<std::string_view, 256> json_encode
    = {"\x00", "\x01", "\x02", "\x03", "\x04", "\x05", "\x06", "\x07", "\\b",
       "\\t",  "\\n",  "\x0b", "\\f",  "\\r",  "\x0e", "\x0f", "\x10", "\x11",
       "\x12", "\x13", "\x14", "\x15", "\x16", "\x17", "\x18", "\x19", "\x1a",
       "\x1b", "\x1c", "\x1d", "\x1e", "\x1f", "\x20", "\x21", "\\\"", "\x23",
       "\x24", "\x25", "\x26", "\x27", "\x28", "\x29", "\x2a", "\x2b", "\x2c",
       "\x2d", "\x2e", "\\/",  "\x30", "\x31", "\x32", "\x33", "\x34", "\x35",
       "\x36", "\x37", "\x38", "\x39", "\x3a", "\x3b", "\x3c", "\x3d", "\x3e",
       "\x3f", "\x40", "\x41", "\x42", "\x43", "\x44", "\x45", "\x46", "\x47",
       "\x48", "\x49", "\x4a", "\x4b", "\x4c", "\x4d", "\x4e", "\x4f", "\x50",
       "\x51", "\x52", "\x53", "\x54", "\x55", "\x56", "\x57", "\x58", "\x59",
       "\x5a", "\x5b", "\\\\", "\x5d", "\x5e", "\x5f", "\x60", "\x61", "\x62",
       "\x63", "\x64", "\x65", "\x66", "\x67", "\x68", "\x69", "\x6a", "\x6b",
       "\x6c", "\x6d", "\x6e", "\x6f", "\x70", "\x71", "\x72", "\x73", "\x74",
       "\x75", "\x76", "\x77", "\x78", "\x79", "\x7a", "\x7b", "\x7c", "\x7d",
       "\x7e", "\x7f", "\x80", "\x81", "\x82", "\x83", "\x84", "\x85", "\x86",
       "\x87", "\x88", "\x89", "\x8a", "\x8b", "\x8c", "\x8d", "\x8e", "\x8f",
       "\x90", "\x91", "\x92", "\x93", "\x94", "\x95", "\x96", "\x97", "\x98",
       "\x99", "\x9a", "\x9b", "\x9c", "\x9d", "\x9e", "\x9f", "\xa0", "\xa1",
       "\xa2", "\xa3", "\xa4", "\xa5", "\xa6", "\xa7", "\xa8", "\xa9", "\xaa",
       "\xab", "\xac", "\xad", "\xae", "\xaf", "\xb0", "\xb1", "\xb2", "\xb3",
       "\xb4", "\xb5", "\xb6", "\xb7", "\xb8", "\xb9", "\xba", "\xbb", "\xbc",
       "\xbd", "\xbe", "\xbf", "\xc0", "\xc1", "\xc2", "\xc3", "\xc4", "\xc5",
       "\xc6", "\xc7", "\xc8", "\xc9", "\xca", "\xcb", "\xcc", "\xcd", "\xce",
       "\xcf", "\xd0", "\xd1", "\xd2", "\xd3", "\xd4", "\xd5", "\xd6", "\xd7",
       "\xd8", "\xd9", "\xda", "\xdb", "\xdc", "\xdd", "\xde", "\xdf", "\xe0",
       "\xe1", "\xe2", "\xe3", "\xe4", "\xe5", "\xe6", "\xe7", "\xe8", "\xe9",
       "\xea", "\xeb", "\xec", "\xed", "\xee", "\xef", "\xf0", "\xf1", "\xf2",
       "\xf3", "\xf4", "\xf5", "\xf6", "\xf7", "\xf8", "\xf9", "\xfa", "\xfb",
       "\xfc", "\xfd", "\xfe", "\xff"};
inline auto json_decode(std::string_view input) -> std::string {
    std::ostringstream out;
    std::size_t        pos = 0;
    while (pos < input.length()) {
        if (input[pos] == '\\') {
            ++pos;
            switch (input[pos]) {
            // Simple translations.
            case '"':
                out << '"';
                break;
            case '\\':
                out << '\\';
                break;
            case '/':
                out << '/';
                break;
            case 'b':
                out << '\b';
                break;
            case 'f':
                out << '\f';
                break;
            case 'n':
                out << '\n';
                break;
            case 'r':
                out << '\r';
                break;
            case 't':
                out << '\t';
                break;
            // If it is an unrecognized sequence, do nothing.
            default:
                out << '\\';
                out << input[pos];
                break;
            }
            ++pos;
        } else {
            size_t next_backslash = input.find('\\', pos);
            if (next_backslash == std::string_view::npos) {
                next_backslash = input.length();
            }
            out << std::string_view{input.data() + pos, next_backslash - pos};
            pos = next_backslash;
        }
    }
    return out.str();
}

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
                serialize_string(out);
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
        // static auto deserialize_from(std::istringstream &in) -> Json {
        static auto deserialize_from(std::string_view in, std::size_t &pos)
            -> Json {
            eat_whitespace(in, pos);
            switch (in[pos]) {
            case 'n':
                return deserialize_null(in, pos);
            case 't':
                return deserialize_true(in, pos);
            case 'f':
                return deserialize_false(in, pos);
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
                return deserialize_number(in, pos);
            case '"':
                return deserialize_string(in, pos);
            case '[':
                return deserialize_array(in, pos);
            case '{':
                return deserialize_object(in, pos);
            default:
                break;
            }
            return {};
        }

    private:
        static auto eat_whitespace(std::string_view in, std::size_t &pos)
            -> void {
            auto length = in.length();
            while (pos < length) {
                switch (in[pos]) {
                case ' ':
                case '\r':
                case '\n':
                    pos++;
                    break;
                default:
                    return;
                }
            }
        }
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
        auto serialize_string(std::ostringstream &out) const noexcept -> void {
            out << '"';
            auto &input = data_.string_;
            auto  first = input.begin();
            for (auto last = input.begin(); last != input.end(); ++last) {
                switch (*last) {
                // Simple translations.
                case '"':
                case '\\':
                case '/':
                case '\b':
                case '\f':
                case '\n':
                case '\r':
                case '\t':
                    out << std::string_view{first, last};
                    out << json_encode.at(static_cast<unsigned char>(*last));
                    first = last + 1;
                default:;
                    // Default NOP.
                }
            }
            out << std::string_view{first, input.end()};
            out << '"';
            // case '\\':
            //     out << "\\\\";
            //     break;
            // case '"':
            //     out << "\\\"";
            //     break;
            // case '/':
            //     out << "\\/";
            //     break;
            // case '\b':
            //     out << "\\b";
            //     break;
            // case '\f':
            //     out << "\\f";
            //     break;
            // case '\n':
            //     out << "\\n";
            //     break;
            // case '\r':
            //     out << "\\r";
            //     break;
            // case '\t':
            //     out << "\\t";
            //     break;
            // default:
            //     out << *last;
            //     break;
        }
        static auto deserialize_null(std::string_view in, std::size_t &pos)
            -> Json {
            ASSERT(in.substr(pos, 4) == "null");
            pos += 4;
            return {};
        }
        static auto deserialize_false(std::string_view in, std::size_t &pos)
            -> Json {
            ASSERT(in.substr(pos, 5) == "false");
            pos += 5;
            return false;
        }
        static auto deserialize_true(std::string_view in, std::size_t &pos)
            -> Json {
            ASSERT(in.substr(pos, 4) == "true");
            pos += 4;
            return true;
        }
        static auto deserialize_string(std::string_view in, std::size_t &pos)
            -> Json {
            auto start = pos;
            auto length = in.length();
            ASSERT(in[pos] == '"');
            pos++;
            while (pos < length && in[pos] != '"') {
                pos++;
            }
            ASSERT(in[pos] == '"');
            pos++;
            return json_decode(
                std::string_view{in.data() + start + 1, in.data() + pos - 1});
        }
        static auto deserialize_number(std::string_view in, std::size_t &pos)
            -> Json {
            auto start = pos;
            auto length = in.length();
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
                return std::stod(std::string{in.substr(start, pos - start)});
            }
            return std::stoi(std::string{in.substr(start, pos - start)});
        }
        static auto deserialize_array(std::string_view in, std::size_t &pos)
            -> Json {
            Json ret;
            ret.value_->to<Array>(); // array type
            ASSERT(in[pos] == '[');
            pos++;
            while (in[pos] != ']') {
                // ret.value_->data_.array_->push_back(deserialize_from(in,
                // pos)); // array may be uninitialized
                ret.value_->as<Array>()->push_back(deserialize_from(in, pos));
                if (in[pos] == ',') {
                    pos++;
                }
                eat_whitespace(in, pos);
            }
            ASSERT(in[pos] == ']');
            pos++;
            return ret;
        }
        static auto deserialize_object(std::string_view in, std::size_t &pos)
            -> Json {
            Json ret;
            ret.value_->to<Object>();
            ASSERT(in[pos] == '{');
            pos++;
            while (in[pos] != '}') {
                eat_whitespace(in, pos);
                auto key = deserialize_string(in, pos);
                ASSERT(in[pos] == ':');
                pos++;
                eat_whitespace(in, pos);
                auto value = deserialize_from(in, pos);
                (*ret.value_
                      ->as<Object>())[std::string{key.value_->as<String>()}]
                    = value;
                if (in[pos] == ',') {
                    pos++;
                }
                eat_whitespace(in, pos);
            }
            // skip '}'
            ASSERT(in[pos] == '}');
            pos++;
            return ret;
        }
    };

public:
    Json([[maybe_unused]] std::nullptr_t null = nullptr)
        : value_{std::make_shared<Value>()} {}
    template <typename T>
        requires(std::is_same_v<T, bool> || std::is_floating_point_v<T>
                 || std::is_integral_v<T>
                 || std::convertible_to<T, std::string_view>
                 || std::is_same_v<T, Value>)
    Json(T value)
        : value_{std::make_shared<Value>(value)} {}
    Json(std::initializer_list<Json> values)
        : value_{std::make_shared<Value>(values)} {}
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
        std::size_t pos = 0;
        return Value::deserialize_from(str, pos);
    }
    auto number() const {
        return value_->as<Number>();
    }
    auto string() const {
        return value_->as<String>();
    }

private:
    std::shared_ptr<Value> value_;
};
} // namespace jsonlib
