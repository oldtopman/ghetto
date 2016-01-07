// Copyright 2013 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://sourceforge.net/projects/jsoncons/files/ for latest version
// See https://sourceforge.net/p/jsoncons/wiki/Home/ for documentation.

#ifndef JSONCONS_JSONCONS_CONFIG_HPP
#define JSONCONS_JSONCONS_CONFIG_HPP

#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <istream>
#include <ostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <cstdarg>
#include <limits> // std::numeric_limits

#define JSONCONS_NO_MACRO_EXP

namespace jsoncons
{

// Follow boost

#if defined (__clang__)
#if defined(_GLIBCXX_USE_NOEXCEPT)
#define JSONCONS_NOEXCEPT _GLIBCXX_USE_NOEXCEPT
#else
#define JSONCONS_NOEXCEPT noexcept
#endif
#elif defined(__GNUC__)
#define JSONCONS_NOEXCEPT _GLIBCXX_USE_NOEXCEPT
#elif defined(_MSC_VER)
#if _MSC_VER >= 1900
#define JSONCONS_NOEXCEPT noexcept
#else
#define JSONCONS_NOEXCEPT
#endif
#else
#define JSONCONS_NOEXCEPT
#endif

#if defined(_MSC_VER)
#if _MSC_VER >= 1900
#define JSONCONS_ALIGNOF alignof
#else
#define JSONCONS_ALIGNOF __alignof
#endif
#else
#define JSONCONS_ALIGNOF alignof
#endif

#ifdef _MSC_VER
#pragma warning( disable : 4290 )
inline bool is_nan(double x) { return _isnan(x) != 0; }
inline bool is_inf(double x)
{
    return !_finite(x) && !_isnan(x);
}
inline bool is_pos_inf(double x)
{
    return is_inf(x) && x > 0;
}
inline bool is_neg_inf(double x)
{
    return is_inf(x) && x < 0;
}

inline
int c99_vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
    int count = -1;

    if (size != 0) count = _vsnprintf_s(str, size, _TRUNCATE, format, ap);
    if (count == -1) count = _vscprintf(format, ap);

    return count;
}

inline
int c99_snprintf(char *str, size_t size, const char *format, ...)
{
    int count;
    va_list ap;

    va_start(ap, format);
    count = c99_vsnprintf(str, size, format, ap);
    va_end(ap);

    return count;
}
#else
inline bool is_nan(double x)
{ return std::isnan( x ); }
inline bool is_pos_inf(double x)
{return std::isinf(x) && x > 0;}
inline bool is_neg_inf(double x)
{return  std::isinf(x) && x > 0;}

#if __cplusplus >= 201103L
#define c99_snprintf snprintf
#else
#define c99_snprintf std::snprintf
#endif

#endif

template<typename Char>
std::basic_string<Char> float_to_string(double val, int precision)
{
	std::basic_ostringstream<Char> ss;
    ss.imbue(std::locale::classic());
    {
        buffered_ostream<Char> os(ss);
        print_float(val, precision, os);
    }
	return ss.str();
}

template <typename Char>
class buffered_ostream;

template <typename Char>
class float_printer
{
    int precision_;
public:
    float_printer(int precision)
        : precision_(precision)
    {
    }

    void print(double val, buffered_ostream<Char>& os)
    {
    	print_float(val, precision_, os);
    }
};

#ifdef _MSC_VER

template<typename Char>
void print_float(double val, int precision, buffered_ostream<Char>& os)
{
    char buf[_CVTBUFSIZE];
    int decimal_point = 0;
    int sign = 0;

    if (precision >= _CVTBUFSIZE)
    {
        precision = _CVTBUFSIZE - 1;
    }

    int err = _ecvt_s(buf, _CVTBUFSIZE, val, precision, &decimal_point, &sign);
    if (err != 0)
    {
        throw std::runtime_error("Failed attempting double to string conversion");
    }
    char* s = buf;
    char* s0 = s;
	char* se = s + precision;

    int i, k;
    int j;

    if (sign)
    {
        os.put('-');
    }
    if (decimal_point <= -4 || decimal_point > se - s + 5) 
    {
        os.put(*s++);
        if (s < se) 
        {
            os.put('.');
            while ((se-1) > s && *(se-1) == '0')
            {
                --se;
            }
			
            while(s < se)
            {
                os.put(*s++);
            }
        }
        os.put('e');
        /* sprintf(b, "%+.2d", decimal_point - 1); */
        if (--decimal_point < 0) {
            os.put('-');
            decimal_point = -decimal_point;
            }
        else
            os.put('+');
        for(j = 2, k = 10; 10*k <= decimal_point; j++, k *= 10);
        for(;;) 
		{
            i = decimal_point / k;
            os.put(i + '0');
            if (--j <= 0)
                break;
            decimal_point -= i*k;
            decimal_point *= 10;
        }
    }
    else if (decimal_point <= 0) 
    {
        os.put('0');
        os.put('.');
        while ((se-1) > s && *(se-1) == '0')
        {
            --se;
        }
        for(; decimal_point < 0; decimal_point++)
        {
            os.put('0');
        }
		while(s < se)
        {
            os.put(*s++);
        }
	}
    else {
        while(s < se) 
        {
            os.put(*s++);
			if ((--decimal_point == 0) && s < se)
			{
                os.put('.');
				while ((se-1) > s && *(se-1) == '0')
				{
					--se;
				}
			}
        }
        for(; decimal_point > 0; decimal_point--)
        {
            os.put('0');
        }
	}
}

#else
template <typename Char>
void print_float(double val, int precision, buffered_ostream<Char>& os)
{
    std::basic_ostringstream<Char> ss;
    ss.imbue(std::locale::classic());
    ss << std::showpoint << std::setprecision(precision) << val;
    std::basic_string<Char> s(ss.str());

    typename std::basic_string<Char>::size_type exp_pos= s.find('e');
    std::basic_string<Char> exp;
    if (exp_pos != std::basic_string<Char>::npos)
    {
        exp = s.substr(exp_pos);
        s.erase(exp_pos);
    }

    int len = (int)s.size();
    while (len >= 2 && s[len - 1] == '0' && s[len - 2] != '.')
    {
        --len;
    }
    s.erase(len);
    if (exp_pos != std::basic_string<Char>::npos)
    {
        s.append(exp);
    }

    os.write(s.c_str(),s.length());
}
#endif

// string_to_float only requires narrow char
#ifdef _MSC_VER
class float_reader
{
private:
    _locale_t locale;
public:
    float_reader()
    {
        locale = _create_locale(LC_NUMERIC, "C");
    }

	double read(const char* s, size_t length)
	{
        const char *begin = s;
        char *end = nullptr;
        double val = _strtod_l(begin, &end, locale);
        if (begin == end)
        {
            throw std::invalid_argument("Invalid float value");
        }
        return val;
	}
};

#else
class float_reader
{
private:
    std::vector<char> buffer_;
    std::string decimal_point_;
	bool is_dot_;
public:
    float_reader()
        : buffer_()
    {
        struct lconv * lc = localeconv();
        if (lc != nullptr)
        {
            decimal_point_ = std::string(lc->decimal_point);	
        }
        else
        {
            decimal_point_ = std::string("."); 
        }
		buffer_.reserve(100);
		is_dot_ = decimal_point_ == ".";
    }

	double read(const char* s, size_t length)
	{
        double val;
        if (is_dot_)
        {
            const char *begin = s;
            char *end = nullptr;
            val = strtod(begin, &end);
            if (begin == end)
            {
                throw std::invalid_argument("Invalid float value");
            }
        }
        else
        {
            buffer_.clear();
            size_t j = 0;
            const char* pe = s + length;
            for (const char* p = s; p < pe; ++p)
            {
                if (*p == '.')
                {
                    buffer_.insert(buffer_.begin() + j, decimal_point_.begin(), decimal_point_.end());
                    j += decimal_point_.length();
                }
                else
                {
                    buffer_.push_back(*p);
                    ++j;
                }
            }
            const char *begin = &buffer_[0];
            char *end = nullptr;
            val = strtod(begin, &end);
            if (begin == end)
            {
                throw std::invalid_argument("Invalid float value");
            }
        }
		return val;
	}
};
#endif

}

#endif
