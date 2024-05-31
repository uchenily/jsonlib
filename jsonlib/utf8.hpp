#pragma once
/// https://github.com/GothenburgBitFactory/libshared/blob/master/src/utf8.cpp
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2016 - 2017, 2019 - 2021, 2023, Gothenburg Bit Factory.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// http://www.opensource.org/licenses/mit-license.php
//
////////////////////////////////////////////////////////////////////////////////

#include <cstdint>
#include <string_view>

#include "widechar_width.hpp"
////////////////////////////////////////////////////////////////////////////////
// Converts '0'     -> 0
//          '9'     -> 9
//          'a'/'A' -> 10
//          'f'/'F' -> 15
#define XDIGIT(x)                                                              \
    ((x) >= '0' && (x) <= '9'   ? ((x) - '0')                                  \
     : (x) >= 'a' && (x) <= 'f' ? ((x) + 10 - 'a')                             \
     : (x) >= 'A' && (x) <= 'F' ? ((x) + 10 - 'A')                             \
                                : 0)

inline auto utf8_sequence(unsigned int character) -> int {
    if ((character & 0xE0) == 0xC0) {
        return 2;
    }

    if ((character & 0xF0) == 0xE0) {
        return 3;
    }

    if ((character & 0xF8) == 0xF0) {
        return 4;
    }

    return 1;
}

inline auto mk_wcwidth(wchar_t ucs) -> int {
    int width = widechar_wcwidth(ucs);

    if (width >= 0) {
        return width;
    }

    // Interpret widened characters as 2 chars
    if (width == widechar_widened_in_9) {
        return 2;
    }

    // Interpret ambiguous east-asian characters as 1 char
    // This includes accented characters like á or é
    if (width == widechar_ambiguous) {
        return 1;
    }

    // All other negative values
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Note: Assumes 4-digit hex codepoints:
//         xxxx
//         \uxxxx
//         U+xxxx
inline auto utf8_codepoint(std::string_view input) -> unsigned int {
    unsigned int codepoint = 0;
    auto         length = input.length();

    // U+xxxx, \uxxxx
    if (length >= 6
        && ((input[0] == 'U' && input[1] == '+')
            || (input[0] == '\\' && input[1] == 'u'))) {
        codepoint = XDIGIT(input[2]) << 12 | XDIGIT(input[3]) << 8
                    | XDIGIT(input[4]) << 4 | XDIGIT(input[5]);
    } else if (length >= 4) {
        codepoint = XDIGIT(input[0]) << 12 | XDIGIT(input[1]) << 8
                    | XDIGIT(input[2]) << 4 | XDIGIT(input[3]);
    }

    return codepoint;
}

////////////////////////////////////////////////////////////////////////////////
// Iterates along a UTF8 string.
//   - argument i counts bytes advanced through the string
//   - returns the next character
inline auto utf8_next_char(std::string_view input, std::size_t &i)
    -> unsigned int {
    if (input[i] == '\0') {
        return 0;
    }

    // How many bytes in the sequence?
    int length = utf8_sequence(input[i]);
    i += length;

    // 0xxxxxxx -> 0xxxxxxx
    if (length == 1) {
        return input[i - 1];
    }

    // 110yyyyy 10xxxxxx -> 00000yyy yyxxxxxx
    if (length == 2) {
        return ((input[i - 2] & 0x1F) << 6) + (input[i - 1] & 0x3F);
    }

    // 1110zzzz 10yyyyyy 10xxxxxx -> zzzzyyyy yyxxxxxx
    if (length == 3) {
        return ((input[i - 3] & 0xF) << 12) + ((input[i - 2] & 0x3F) << 6)
               + (input[i - 1] & 0x3F);
    }

    // 11110www 10zzzzzz 10yyyyyy 10xxxxxx -> 000wwwzz zzzzyyyy yyxxxxxx
    if (length == 4) {
        return ((input[i - 4] & 0x7) << 18) + ((input[i - 3] & 0x3F) << 12)
               + ((input[i - 2] & 0x3F) << 6) + (input[i - 1] & 0x3F);
    }

    // Default: pretend as though it's a single character.
    // TODO Or should this throw?
    return input[i - 1];
}

////////////////////////////////////////////////////////////////////////////////
// http://en.wikipedia.org/wiki/UTF-8
inline auto utf8_character(unsigned int codepoint) -> std::string {
    uint8_t sequence[5]{};

    // 0xxxxxxx -> 0xxxxxxx
    if (codepoint < 0x80) {
        sequence[0] = codepoint;
    }

    // 00000yyy yyxxxxxx -> 110yyyyy 10xxxxxx
    else if (codepoint < 0x800) {
        sequence[0] = 0xC0 | (codepoint & 0x7C0) >> 6;
        sequence[1] = 0x80 | (codepoint & 0x3F);
    }

    // zzzzyyyy yyxxxxxx -> 1110zzzz 10yyyyyy 10xxxxxx
    else if (codepoint < 0x10000) {
        sequence[0] = 0xE0 | (codepoint & 0xF000) >> 12;
        sequence[1] = 0x80 | (codepoint & 0xFC0) >> 6;
        sequence[2] = 0x80 | (codepoint & 0x3F);
    }

    // 000wwwzz zzzzyyyy yyxxxxxx -> 11110www 10zzzzzz 10yyyyyy 10xxxxxx
    else if (codepoint < 0x110000) {
        sequence[0] = 0xF0 | (codepoint & 0x1C0000) >> 18;
        sequence[1] = 0x80 | (codepoint & 0x03F000) >> 12;
        sequence[2] = 0x80 | (codepoint & 0x0FC0) >> 6;
        sequence[3] = 0x80 | (codepoint & 0x3F);
    }

    return {reinterpret_cast<char *>(sequence)};
}

////////////////////////////////////////////////////////////////////////////////
// Length of a string in characters.
inline auto utf8_length(std::string_view str) -> unsigned int {
    auto byteLength = str.length();
    auto charLength = byteLength;
    auto data = str.data();

    // Decrement the number of bytes for each byte that matches 0b10??????
    // this way only the first byte of any utf8 sequence is counted.
    for (auto i = 0u; i < byteLength; i++) {
        // Extract the first two bits and check whether they are 10
        if ((data[i] & 0xC0) == 0x80) {
            charLength--;
        }
    }

    return charLength;
}

////////////////////////////////////////////////////////////////////////////////
// Width of a string in character cells.
inline auto utf8_width(std::string_view str) -> unsigned int {
    unsigned int length = 0;
    std::size_t  i = 0;
    unsigned int c;
    while ((c = utf8_next_char(str, i)) != 0) {
        // Control characters, and more especially newline characters, make
        // mk_wcwidth() return -1.  Ignore that, thereby "adding zero" to
        // length. Since control characters are not displayed in reports, this
        // is a valid choice.
        int l = mk_wcwidth(c);
        if (l != -1) {
            length += l;
        }
    }

    return length;
}

////////////////////////////////////////////////////////////////////////////////
inline auto utf8_text_length(std::string_view str) -> unsigned int {
    auto byteLength = str.length();
    auto charLength = byteLength;
    auto data = str.data();
    bool in_color = false;

    // Decrement the number of bytes for each byte that matches 0b10??????
    // this way only the first byte of any utf8 sequence is counted.
    for (auto i = 0u; i < byteLength; i++) {
        if (in_color) {
            if (data[i] == 'm') {
                in_color = false;
            }

            --charLength;
        } else {
            if (data[i] == 033) {
                in_color = true;
                --charLength;
            } else {
                // Extract the first two bits and check whether they are 10
                if ((data[i] & 0xC0) == 0x80) {
                    --charLength;
                }
            }
        }
    }

    return charLength;
}

////////////////////////////////////////////////////////////////////////////////
inline auto utf8_text_width(std::string_view str) -> unsigned int {
    bool in_color = false;

    unsigned int length = 0;
    std::size_t  i = 0;
    unsigned int c;
    while ((c = utf8_next_char(str, i)) != 0) {
        if (in_color) {
            if (c == 'm') {
                in_color = false;
            }
        } else if (c == 033) {
            in_color = true;
        } else {
            length += mk_wcwidth(c);
        }
    }

    return length;
}

////////////////////////////////////////////////////////////////////////////////
inline auto utf8_substr(std::string_view input,
                        unsigned int     start,
                        unsigned int     length /* = 0 */) -> std::string {
    // Find the starting index.
    std::size_t index_start = 0;
    for (unsigned int i = 0; i < start; i++) {
        utf8_next_char(input, index_start);
    }

    std::string result;
    if (length) {
        std::size_t index_end = index_start;
        for (unsigned int i = 0; i < length; i++) {
            utf8_next_char(input, index_end);
        }

        result = input.substr(index_start, index_end - index_start);
    } else {
        result = input.substr(index_start);
    }

    return result;
}
