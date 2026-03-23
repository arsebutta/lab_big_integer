// =============================================================
//  big_integer.cpp -- BigInteger class implementation
//
//  TASK: Implement all methods declared in big_integer.h
//  This stub file exists only so the project structure is clear.
//  Replace its contents with your implementation.
// =============================================================

#include "big_integer.h"
#include <algorithm>
#include <stdexcept>
#include <utility>
bool BigInteger::operator<(const BigInteger& rhs) const {
    if (is_negative() && !rhs.is_negative()) {
        return 1;
    }
    if (!is_negative() && rhs.is_negative()) {
        return 0;
    }
    if (digits_.size() < rhs.digits_.size()) {
        return !is_negative();
    }
    if (digits_.size() > rhs.digits_.size()) {
        return is_negative();
    }
    for (int i = digits_.size() - 1; i > -1; --i) {
        if (digits_[i] < rhs.digits_[i]) {
            return !is_negative();
        }
        if (digits_[i] > rhs.digits_[i]) {
            return is_negative();
        }
    }
    return 0;
}
bool BigInteger::operator<=(const BigInteger& rhs) const {
    return !(rhs < *this);
}
bool BigInteger::operator>(const BigInteger& rhs) const {
    return (rhs < *this);
}
bool BigInteger::operator>=(const BigInteger& rhs) const {
    return !(*this < rhs);
}
bool BigInteger::operator==(const BigInteger& rhs) const {
    return !(*this < rhs || rhs < *this);
}
bool BigInteger::operator!=(const BigInteger& rhs) const {
    return !(*this == rhs);
}

void BigInteger::Sub(const BigInteger& other) {
    int kostil = std::abs((int)digits_.size() - (int)other.digits_.size());
    int maxi = std::max(digits_.size(), other.digits_.size());
    for (int i = 0; i < maxi; i++) {
        if (i < maxi - kostil && abs() >= other.abs()) {
            digits_[i] -= other.digits_[i];
        } else if (i < maxi - kostil && other.abs() > abs()) {
            digits_[i] = other.digits_[i] - digits_[i];
        } else if ((int)other.digits_.size() == maxi) {
            digits_.push_back(other.digits_[i]);
        }
    }
    for (int i = 0; i < (int)digits_.size() - 1; i++)
        if (digits_[i] < 0) {
            digits_[i] += 10;
            --digits_[i + 1];
        }
    for (int i = (int)digits_.size() - 1; i > 0; --i) {
        if (digits_[i] == 0) {
            digits_.pop_back();
        } else {
            break;
        }
    }
}
void BigInteger::Mul(const BigInteger& other) {
    int ds = digits_.size();
    int ods = other.digits_.size();
    std::vector<int> result(ds + ods, 0);
    for (int i = 0; i < ods; i++) {
        for (int j = 0; j < ds; j++) {
            result[i + j] += digits_[j] * other.digits_[i];
        }
    }
    for (int i = 0; i < (int)result.size() - 1; i++) {
        result[i + 1] += result[i] / base;
        result[i] %= base;
    }
    for (int i = (int)result.size() - 1; i > 0; --i) {
        if (result[i] == 0) {
            result.pop_back();
        } else {
            break;
        }
    }
    digits_ = result;
}

void BigInteger::Div(const BigInteger& other) {
    int n = digits_.size();
    std::vector<int> result(n, 0);
    BigInteger remain{"0"};
    for (int i = n - 1; i >= 0; --i) {
        remain.digits_.insert(remain.digits_.begin(), digits_[i]);
        while (remain.digits_.size() > 1 && remain.digits_.back() == 0) {
            remain.digits_.pop_back();
        }
        int q = 0;
        BigInteger temp = remain;
        while (!(temp.abs() < other.abs())) {
            temp.Sub(other);
            ++q;
        }
        remain = temp;
        result[i] = q;
    }
    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }
    digits_ = result;
}

BigInteger& BigInteger::operator+=(const BigInteger& rhs) {
    if (negative_ == rhs.negative_) {
        Sum(rhs);
    } else if (abs() < rhs.abs()) {
        Sub(rhs);
        negative_ = rhs.negative_;
    } else {
        Sub(rhs);
    }
    return *this;
}
BigInteger& BigInteger::operator-=(const BigInteger& rhs) {
    *this += (-rhs);
    return *this;
}
BigInteger& BigInteger::operator*=(const BigInteger& rhs) {
    Mul(rhs);
    negative_ = (negative_ != rhs.negative_);
    return *this;
}
BigInteger& BigInteger::operator/=(const BigInteger& rhs) {
    Div(rhs);
    negative_ = (negative_ != rhs.negative_);
    return *this;
}
BigInteger& BigInteger::operator%=(const BigInteger& rhs) {
    *this = *this - (*this / rhs) * rhs;
    return *this;
}
BigInteger BigInteger::operator+(const BigInteger& rhs) const {
    BigInteger x = *this;
    x += rhs;
    return x;
}
BigInteger BigInteger::operator-(const BigInteger& rhs) const {
    BigInteger x = *this;
    x -= rhs;
    return x;
}
BigInteger BigInteger::operator*(const BigInteger& rhs) const {
    BigInteger x = *this;
    x *= rhs;
    return x;
}
BigInteger BigInteger::operator/(const BigInteger& rhs) const {
    BigInteger x = *this;
    x /= rhs;
    return x;
}
BigInteger BigInteger::operator%(const BigInteger& rhs) const {
    BigInteger x = *this;
    x %= rhs;
    return x;
}
BigInteger& BigInteger::operator++() {
    *this += BigInteger(1);
    return *this;
}
BigInteger BigInteger::operator++(int) {
    BigInteger x = *this;
    *this += BigInteger(1);
    return x;
}
BigInteger& BigInteger::operator--() {
    *this -= BigInteger(1);
    return *this;
}
BigInteger BigInteger::operator--(int) {
    BigInteger x = *this;
    *this -= BigInteger(1);
    return x;
}
std::string BigInteger::to_string() const {
    std::string ans = "";
    if (negative_ && !is_zero()) ans += "-";
    for (int i = (int)digits_.size() - 1; i > -1; --i) {
        ans += (digits_[i] + '0');
    }
    return ans;
}
BigInteger::operator bool() const {
    return (*this != BigInteger("0"));
}

/*int main() {
    BigInteger z1 = 200;
    BigInteger z2 = 200;
    z1 = z1 / z2;
    std::cout << z1<<std::endl;
}*/

// TODO: your code here
long long BigInteger::mod(long long a, long long b) const {
    return ((a % b) + b) % b;
}

BigInteger::BigInteger() : negative_(false) {
    digits_.push_back(0);
}

BigInteger::BigInteger(int value) : negative_(value < 0) {
    long long val = std::abs((long long)value);
    do {
        digits_.push_back(mod(val, 10));
        val /= 10;
    } while (val > 0);
}

BigInteger::BigInteger(long long value) : negative_(value < 0) {
    long long val = std::abs(value);
    do {
        digits_.push_back(mod(val, 10));
        val /= 10;
    } while (val > 0);
}

BigInteger::BigInteger(const std::string& str) : negative_(false) {
    size_t start = 0;
    if (str[0] == '-' || str[0] == '+') {
        negative_ = (str[0] == '-');
        start = 1;
    }
    while (start + 1 < str.size() && str[start] == '0') {
        ++start;
    }
    for (size_t i = str.size(); i > start; --i) {
        digits_.push_back(str[i - 1] - '0');
    }
}

bool BigInteger::is_negative() const {
    return negative_;
}

std::ostream& operator<<(std::ostream& os, const BigInteger& value) {
    if (value.negative_ && !value.is_zero()) {
        os << '-';
    }
    for (int i = (int)value.digits_.size() - 1; i > -1; --i) {
        os << value.digits_[i];
    }
    return os;
}

std::istream& operator>>(std::istream& is, BigInteger& value) {
    value.digits_.clear();
    std::string res;
    size_t start = 0;
    if (!(is >> res)) return is;
    if (res[0] == '-') {
        value.negative_ = true;
        ++start;
    } else if (res[0] == '+') {
        ++start;
    }
    while (start < res.size() - 1 && res[start] == '0') {
        ++start;
    }
    for (size_t i = res.size(); i > start; --i) {
        value.digits_.push_back(res[i - 1] - '0');
    }
    return is;
}

BigInteger BigInteger::operator-() const {
    BigInteger a = *this;
    if (!a.is_zero()) a.negative_ = !a.negative_;
    return a;
}

void BigInteger::Sum(const BigInteger& other) {
    size_t end = std::max(digits_.size(), other.digits_.size());
    size_t a_s = digits_.size();
    size_t b_s = other.digits_.size();
    int carry = 0;
    for (size_t i = 0; i < end; ++i) {
        int a = (i >= a_s ? 0 : digits_[i]);
        int b = (i >= b_s ? 0 : other.digits_[i]);
        if (i < digits_.size()) {
            digits_[i] = mod(a + b + carry, 10);
        } else {
            digits_.push_back(mod(a + b + carry, 10));
        }
        carry = (a + b + carry) / 10;
    }
    if (carry) digits_.push_back(1);
}

BigInteger BigInteger::abs() const {
    BigInteger a = *this;
    a.negative_ = false;
    return a;
}

bool BigInteger::is_zero() const {
    return digits_.size() == 1 && digits_[0] == 0;
}


