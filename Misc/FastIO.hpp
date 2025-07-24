/**
 * @brief 快速输入输出库
 * @details 提供高效的输入输出操作，支持多种数据类型的快速读写
 *          包含自定义的输入输出流类，性能优于标准库的cin/cout
 * @author huxint
 */
#pragma once
#include <ranges>
#include <string>
#include <bitset>
#include <cstdio>
#include <cstdint>
#include <complex>
#include <cstring>
#include <iomanip>
#include <charconv>
#include <string_view>

namespace fio {
    static constexpr std::size_t bufsize = 1 << 20;
    static constexpr std::string_view hexdigits = "0123456789abcdef";
    class Istream {
    public:
        Istream(FILE *file) {
            input(file);
        }

        explicit operator bool() const {
            return _status;
        }

        void set(bool status = true) {
            _status = status;
        }

        void input(FILE *file) {
            _file = file;
            bufptr = bufend = buf;
            set();
        }

        void input(std::string_view file) {
            input(std::fopen(file.data(), "rb"));
        }

        int getchar() {
            return (bufptr == bufend ? (bufend = (bufptr = buf) + std::fread(buf, 1, bufsize, _file)) : nullptr), bufptr == bufend ? EOF : *bufptr++;
        }

        Istream &read(char *str, std::size_t size) {
            if (std::size_t r = bufend - bufptr; size > r) [[unlikely]] {
                std::memcpy(str, bufptr, r);
                size -= r;
                str += r;
                bufptr = bufend;
                std::fread(str, 1, size, _file);
            } else {
                std::memcpy(str, bufptr, size);
                bufptr += size;
            }
            return *this;
        }

        Istream &operator>>(Istream &self) {
            return *this;
        }

        Istream &operator>>(char &ch) {
            skip();
            ch = getchar();
            if (ch == EOF) {
                set(false);
            }
            return *this;
        }

        Istream &operator>>(char *str) {
            skip();
            int ch = peek();
            if (ch == EOF) {
                set(false);
                return *this;
            }
            for (; not blank(ch); ch = peek()) {
                *str++ = ch;
                getcharuncheck();
            }
            *str = 0;
            return *this;
        }

        Istream &operator>>(std::string &str) {
            skip();
            int ch = peek();
            if (ch == EOF) {
                set(false);
                return *this;
            }
            for (str.erase(); not blank(ch); ch = peek()) {
                str.append(1, ch);
                getcharuncheck();
            }
            return *this;
        }

        template <std::signed_integral T>
        Istream &operator>>(T &value) {
            bool sign = false;
            int ch = getchar();
            std::make_unsigned_t<T> uvalue = 0;
            for (; not isdigit(ch) and ch != EOF; ch = getchar()) {
                sign = ch == '-';
            }
            if (ch == EOF) {
                set(false);
                return *this;
            }
            for (; isdigit(ch); ch = getchar()) {
                uvalue = (uvalue << 3) + (uvalue << 1) + (ch & 0x0F);
            }
            ungetchar();
            value = sign ? -uvalue : uvalue;
            return *this;
        }

        Istream &operator>>(std::unsigned_integral auto &value) {
            int ch = getchar();
            for (value = 0; not isdigit(ch) and ch != EOF;) {
                ch = getchar();
            }
            if (ch == EOF) {
                set(false);
                return *this;
            }
            for (; isdigit(ch); ch = getchar()) {
                value = (value << 3) + (value << 1) + (ch & 0x0F);
            }
            ungetchar();
            return *this;
        }

        Istream &operator>>(std::floating_point auto &value) {
            static char _buf[4940];
            if (*this >> _buf) {
                std::from_chars(_buf, static_cast<char *>(std::memchr(_buf, 0, 4940)), value);
            }
            return *this;
        }

        template <std::size_t N>
        Istream &operator>>(std::bitset<N> &value) {
            static char _buf[N];
            *this >> _buf;
            value = std::move(std::bitset<N>(_buf));
            return *this;
        }

        template <typename T>
        Istream &operator>>(std::complex<T> &value) {
            return *this >> value.real() >> value.imag();
        }

    private:
        FILE *_file;
        bool _status;
        char *bufptr;
        char *bufend;
        char buf[bufsize];
        static constexpr bool isdigit(int c) {
            return c >= '0' and c <= '9';
        }

        static constexpr bool blank(int c) {
            return c <= ' ';
        }

        int peek() {
            return (bufptr == bufend ? (bufend = (bufptr = buf) + std::fread(buf, 1, bufsize, _file)) : nullptr), bufptr == bufend ? EOF : *bufptr;
        }

        void skip() {
            int ch = getchar();
            while (blank(ch) and ch != EOF) {
                ch = getchar();
            }
            if (ch == EOF) {
                set(false);
                return;
            }
            ungetchar();
        }

        int ungetchar() {
            return *bufptr--;
        }

        int getcharuncheck() {
            return *bufptr++;
        }
    } cin(stdin);

    class Ostream {
    public:
        Ostream(FILE *file) {
            output(file);
            setprecision(6);
        }

        ~Ostream() {
            flush();
        }

        void flush() {
            std::fwrite(buf, 1, bufptr - buf, _file);
            bufptr = buf;
        }

        void output(FILE *file) {
            _file = file;
        }

        void output(std::string_view file) {
            output(std::fopen(file.data(), "wb"));
        }

        void putchar(int c) {
            bufptr == std::ranges::end(buf) ? flush() : void();
            putcharuncheck(c);
        }

        void setprecision(std::size_t precision) {
            _precision = precision;
        }

        Ostream &write(const char *str, std::size_t size) {
            if (size >= std::size_t(std::ranges::end(buf) - bufptr)) [[unlikely]] {
                flush();
                std::fwrite(str, 1, size, _file);
            } else {
                std::memcpy(bufptr, str, size);
                bufptr += size;
            }
            return *this;
        }

        Ostream &operator<<(Ostream &self) {
            return *this;
        }

        Ostream &operator<<(char ch) {
            putchar(ch);
            return *this;
        }

        Ostream &operator<<(std::string_view str) {
            return write(str.data(), str.size());
        }

        Ostream &operator<<(std::integral auto value) {
            static char _buf[20];
            return write(_buf, std::to_chars(_buf, _buf + 20, value).ptr - _buf);
        }

        Ostream &operator<<(std::floating_point auto value) {
            static char _buf[4940];
            return write(_buf, std::to_chars(_buf, _buf + 4940, value, std::chars_format::fixed, _precision).ptr - _buf);
        }

        template <std::size_t N>
        Ostream &operator<<(const std::bitset<N> &value) {
            return *this << value.to_string();
        }

        template <typename T>
        Ostream &operator<<(const std::complex<T> &value) {
            return *this << '(' << value.real() << ',' << value.imag() << ')';
        }

        Ostream &operator<<(std::nullptr_t value) {
            return *this << "nullptr";
        }

        template <typename T>
            requires (std::is_pointer_v<T> and (std::is_object_v<std::remove_pointer_t<T>> or std::is_function_v<std::remove_pointer_t<T>> or std::is_void_v<std::remove_pointer_t<T>>) and not std::is_same_v<std::remove_cv_t<std::remove_pointer_t<T>>, char>)
        Ostream &operator<<(T ptr) {
            auto hex = [&](auto &&self, std::uintptr_t value) -> void {
                if (value == 0) {
                    *this << "0x";
                    return;
                } else {
                    self(self, value >> 4);
                    putchar(hexdigits[value & 0x0F]);
                }
            };
            hex(hex, reinterpret_cast<std::uintptr_t>(ptr));
            return *this;
        }
    private:
        FILE *_file;
        char buf[bufsize];
        char *bufptr = buf;
        std::size_t _precision;
        void putcharuncheck(int c) {
            *bufptr++ = c;
        }
    } cout(stdout), cerr(stderr);

    Ostream &operator<<(Ostream &ostream, std::_Setprecision __f) {
        ostream.setprecision(__f._M_n);
        return ostream;
    }

    Istream &getline(Istream &istream, char *str) {
        int ch;
        char *first = str;
        for (ch = istream.getchar(); ch != '\n' and ch != '\r' and ch != '\t' and ch != EOF; ch = istream.getchar()) {
            *str++ = ch;
        }
        *str = 0;
        if (str == first and ch == EOF) {
            istream.set(false);
        }
        return istream;
    }

    Istream &getline(Istream &istream, std::string &str) {
        int ch;
        str.erase();
        for (ch = istream.getchar(); ch != '\n' and ch != '\r' and ch != '\t' and ch != EOF; ch = istream.getchar()) {
            str.append(1, ch);
        }
        if (str.empty() and ch == EOF) {
            istream.set(false);
        }
        return istream;
    }
}