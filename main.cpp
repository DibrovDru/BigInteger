#include <cstring>
#include <math.h>
#include <cstdio>
#include <iostream>
#include <iomanip>

class BigIntegerOverflow {
};

class BigInteger {
private:
    typedef uint32_t digit_type;
    typedef uint64_t double_digit_type;

    enum SignType {MINUS = 0, PLUS = 1};

    const static int maxLength = 3000;
    const static uint8_t baseLog = 8;
    const static uint32_t base = static_cast<digit_type>(pow(10, baseLog));
    const static size_t maxReadSize = 20010;

    SignType sign = PLUS;
    int lastNumberIndex = 0;
    double_digit_type digits[maxLength];

    void MakeBigInt(unsigned long long value);
    void IsPositive(long long value);
    void Copy(const BigInteger& other);
    void MakeSmaller();

public:
    BigInteger();
    BigInteger(int value);
    BigInteger(unsigned int value);
    BigInteger(long long value);
    BigInteger(unsigned long long value);
    BigInteger(const char* value);
    BigInteger(const BigInteger& other);

    BigInteger& operator=(const BigInteger& other);

    friend BigInteger operator+(const BigInteger& firstValue, const BigInteger& secondValue);
    friend BigInteger& operator+=(BigInteger& first, const BigInteger& second);
    friend BigInteger& operator++(BigInteger& value);
    friend BigInteger operator++(BigInteger& value, int);
    friend void plus(BigInteger& temp, const BigInteger& first, const BigInteger& second);

    friend BigInteger operator-(const BigInteger& firstValue, const BigInteger& secondValue);
    friend BigInteger& operator-=(BigInteger& first, const BigInteger& second);
    friend BigInteger& operator--(BigInteger& value);
    friend BigInteger operator--(BigInteger& value, int);
    friend BigInteger operator-(const BigInteger& other);
    friend void minus(BigInteger& temp, const BigInteger& firstValue, const BigInteger& secondValue);

    friend std::ostream &operator<<(std::ostream& os, const BigInteger& value);
    friend std::istream &operator>>(std::istream& is, BigInteger& value);

    friend bool operator==(const BigInteger&, const BigInteger& secondValue);
    friend bool operator<=(const BigInteger& firstValue, const BigInteger& secondValue);
    friend bool operator>=(const BigInteger& firstValue, const BigInteger& secondValue);
    friend bool operator!=(const BigInteger& firstValue, const BigInteger& secondValue);
    friend bool operator>(const BigInteger& firstValue, const BigInteger& secondValue);
    friend bool operator<(const BigInteger& firstValue, const BigInteger& secondValue);
    friend bool modCompare(const BigInteger& firstValue, const BigInteger& secondValue);


    friend BigInteger operator*(const BigInteger& firstValue, const BigInteger& secondValue);
    friend BigInteger& operator*=(BigInteger& first, const BigInteger& second);

    friend BigInteger operator/(const BigInteger& firstValue, const BigInteger& secondValue);
    friend BigInteger& operator/=(BigInteger& firstValue, const BigInteger& secondValue);
    friend BigInteger operator%(const BigInteger& firstValue, const BigInteger& secondValue);
    friend BigInteger ow(uint32_t value, uint32_t power);

    friend BigInteger findGCF(const BigInteger& first, const BigInteger& second);
    friend BigInteger sqrt(BigInteger& value);
};

void BigInteger::MakeBigInt(unsigned long long value) {
    int index = 0;

    while (value > 0) {
        digits[index] = value % base;
        ++index;
        value /= base;
    }

    if (index > 0) {
        lastNumberIndex = index - 1;
    }

    for ( ; index < maxLength; ++index) {
        digits[index] = 0;
    }
}

BigInteger::BigInteger() {
    MakeBigInt(0);
}

BigInteger::BigInteger(int value) {
    MakeBigInt(std::abs(value));

    IsPositive(value);
}

BigInteger::BigInteger(unsigned int value) {
    MakeBigInt(value);
}

BigInteger::BigInteger(long long value) {
    MakeBigInt(std::abs(value));

    IsPositive(value);
}

BigInteger::BigInteger(unsigned long long value) {
    MakeBigInt(value);
}

BigInteger::BigInteger(const char* value) : BigInteger() {
    uint32_t size = strlen(value);
    int index = 0, power = 0;

    while (size > 0 && (int)value[size - 1] >= '0') {
        power = 0;

        while (power < baseLog && size > 0 && (int)value[size - 1] >= '0') {
            digits[index] += (((int)value[size - 1] - '0') * pow(10, power));
            --size;
            ++power;
        }

        ++index;
    }

    if (size == 1) {
        sign = MINUS;
    }

    lastNumberIndex = index - 1;
}

void BigInteger::IsPositive(long long value) {
    if (value < 0) {
        sign = MINUS;
    }
}

std::ostream &operator<<(std::ostream& os, const BigInteger& value) {
    if (value.sign == BigInteger::MINUS) {
        std::cout << "-";
    }

    for (int i = value.lastNumberIndex; i >= 0; --i) {
        int power = value.baseLog - 1;

        if (i != value.lastNumberIndex) {

            while ((pow(10, power) > value.digits[i])) {
                std::cout << 0;
                --power;

                if (power == 0) {
                    break;
                }
            }
        }

        std::cout << value.digits[i];
    }
    return os;
}

std::istream &operator>>(std::istream& is, BigInteger& value) {
    char tempValue[20010];
    is >> tempValue;
    value = tempValue;
    return is;
}

bool operator==(const BigInteger& firstValue, const BigInteger& secondValue) {
    return !(firstValue > secondValue || secondValue > firstValue);
}

bool operator<=(const BigInteger& firstValue, const BigInteger& secondValue) {
    return !(firstValue > secondValue);
}

bool operator>=(const BigInteger& firstValue, const BigInteger& secondValue) {
    return !(firstValue < secondValue);
}

bool operator!=(const BigInteger& firstValue, const BigInteger& secondValue) {
    return !(firstValue == secondValue);
}

bool modCompare(const BigInteger& firstValue, const BigInteger& secondValue) {
    if (firstValue.lastNumberIndex < secondValue.lastNumberIndex) {
        return false;
    } else if (firstValue.lastNumberIndex == secondValue.lastNumberIndex) {
        for (int i = firstValue.lastNumberIndex; i >= 0; --i) {
            if (firstValue.digits[i] < secondValue.digits[i]) {
                return false;
            } else if (firstValue.digits[i] > secondValue.digits[i]) {
                return true;
            }
        }
    } else {
        return true;
    }

    return false;
}

bool operator>(const BigInteger& firstValue, const BigInteger& secondValue) {
    if (firstValue.sign == secondValue.sign) {
        return (firstValue.sign == BigInteger::PLUS ?
        modCompare(firstValue, secondValue) : modCompare(-secondValue, -firstValue));
    }

    return firstValue.sign == BigInteger::PLUS;
}

bool operator<(const BigInteger& firstValue, const BigInteger& secondValue) {
    return secondValue > firstValue;
}

BigInteger operator-(const BigInteger& other) {
    if (other == 0) {
        return other;
    }

    BigInteger temp;
    temp = other;
    temp.sign = (temp.sign == BigInteger::MINUS ? BigInteger::PLUS : BigInteger::MINUS);
    return temp;
}

BigInteger::BigInteger(const BigInteger& other) {
    Copy(other);
}

BigInteger& BigInteger::operator=(const BigInteger& other) {
    if (this == &other) {
        return *this;
    }

    Copy(other);
    return *this;
}

void BigInteger::Copy(const BigInteger& other) {
    size_t len = std::max(lastNumberIndex, other.lastNumberIndex);
    lastNumberIndex = other.lastNumberIndex;

    if (lastNumberIndex >= maxLength) {
        throw BigIntegerOverflow();
    }

    sign = other.sign;

    for (int i = len; i >= 0; --i) {
        digits[i] = other.digits[i];
    }
}

void plus(BigInteger& temp, const BigInteger& firstValue, const BigInteger& secondValue) {
    int nextLevel = 0;
    bool countWithSecond = true;

    for (int i = 0; i <= temp.lastNumberIndex; ++i) {
        if (i == std::min(firstValue.lastNumberIndex, secondValue.lastNumberIndex) + 1) {
            countWithSecond = false;
        }

        if (countWithSecond) {
            temp.digits[i] = (firstValue.digits[i] + secondValue.digits[i] + nextLevel) % temp.base;
            nextLevel = (firstValue.digits[i] + secondValue.digits[i] + nextLevel) / temp.base;
        } else {
            temp.digits[i] = (firstValue.digits[i] + nextLevel) % temp.base;
            nextLevel = (firstValue.digits[i] + nextLevel) / temp.base;
        }
    }

    if (nextLevel != 0) {
        ++(temp.lastNumberIndex);
        temp.digits[temp.lastNumberIndex] = nextLevel;
    }
}

BigInteger operator+(const BigInteger& firstValue, const BigInteger& secondValue) {
    BigInteger temp;
    temp.lastNumberIndex = std::max(firstValue.lastNumberIndex, secondValue.lastNumberIndex);

    if (firstValue.sign == secondValue.sign) {
        if (firstValue.sign == BigInteger::PLUS) {
            firstValue > secondValue ?
            plus(temp, firstValue, secondValue) : plus(temp, secondValue, firstValue);
        } else {
            firstValue > secondValue ?
            plus(temp, secondValue, firstValue) : plus(temp, firstValue, secondValue);
        }
        temp.sign = firstValue.sign;
    } else if (secondValue.sign == BigInteger::MINUS) {
        temp = firstValue - (-secondValue);
    } else if (secondValue.sign == BigInteger::PLUS) {
        temp = secondValue - (-firstValue);
    }

    if (temp.lastNumberIndex >= temp.maxLength) {
        throw BigIntegerOverflow();
    }
    return temp;
}

void minus(BigInteger& temp, const BigInteger& firstValue, const BigInteger& secondValue) {
    unsigned int nextLevel = 0;
    bool countWithSecond = true;

    for (int i = 0; i <= temp.lastNumberIndex; ++i) {
        if (i == secondValue.lastNumberIndex + 1) {
            countWithSecond = false;
        }

        if (countWithSecond) {
            if (firstValue.digits[i] >= secondValue.digits[i] + nextLevel) {
                temp.digits[i] = (firstValue.digits[i]) - secondValue.digits[i] - nextLevel;
                nextLevel = 0;
            } else {
                temp.digits[i] = temp.base + firstValue.digits[i] - secondValue.digits[i] - nextLevel;
                nextLevel = 1;
            }
        } else {
            if (firstValue.digits[i] >= nextLevel) {
                temp.digits[i] = (firstValue.digits[i]) - nextLevel;
                nextLevel = 0;
            } else {
                temp.digits[i] = temp.base + firstValue.digits[i] - nextLevel;
                nextLevel = 1;
            }
        }
    }
}

BigInteger operator-(const BigInteger& firstValue, const BigInteger& secondValue) {
    BigInteger temp;
    temp.lastNumberIndex = std::max(firstValue.lastNumberIndex, secondValue.lastNumberIndex);

    if (firstValue.sign == secondValue.sign) {
        if (firstValue.sign == BigInteger::PLUS) {
            if (firstValue >= secondValue) {
                minus(temp, firstValue, secondValue);
            } else {
                temp = secondValue - firstValue;
                temp.sign = BigInteger::MINUS;
            }
        } else {
            temp = (firstValue >= secondValue ?
                    (-secondValue) - (-firstValue) : firstValue + (-secondValue));
        }
    } else {
        temp = firstValue + (-secondValue);
    }

    for (int i = temp.lastNumberIndex; i > 0; --i) {
        if (temp.digits[i] == 0) {
            --(temp.lastNumberIndex);
        } else {
            break;
        }
    }

    if (temp.lastNumberIndex >= temp.maxLength) {
        throw BigIntegerOverflow();
    }
    return temp;
}

BigInteger& operator+=(BigInteger& first, const BigInteger& second) {
    first = first + second;
    return first;
}

BigInteger& operator-=(BigInteger& first, const BigInteger& second) {
    first = first - second;
    return first;
}

BigInteger& operator++(BigInteger& value) {
    return value += 1;
}

BigInteger operator++(BigInteger& value, int) {
    BigInteger old = value;
    value += 1;
    return  old;
}

BigInteger& operator--(BigInteger& value) {
    return value -= 1;
}

BigInteger operator--(BigInteger& value, int) {
    BigInteger old = value;
    value -= 1;
    return old;
}

BigInteger operator*(const BigInteger& firstValue, const BigInteger& secondValue) {
    if (firstValue == 0 || secondValue == 0) {
        return 0;
    }

    if (firstValue.lastNumberIndex + secondValue.lastNumberIndex - 1 >= firstValue.maxLength) {
        throw BigIntegerOverflow();
    }

    BigInteger temp;
    temp.lastNumberIndex = firstValue.lastNumberIndex + secondValue.lastNumberIndex + 1;

    if (firstValue.sign != secondValue.sign) {
        temp.sign = BigInteger::MINUS;
    }

    for (int i = firstValue.lastNumberIndex; i >= 0; --i) {
        for (int j = secondValue.lastNumberIndex; j >= 0; --j) {
            temp.digits[i + j] += firstValue.digits[i] * secondValue.digits[j];
        }
    }

    for (int i = 0; i <= temp.lastNumberIndex; ++i) {
        temp.digits[i + 1] += temp.digits[i] / temp.base;
        temp.digits[i] = temp.digits[i] % temp.base;
    }

    for (int i = temp.lastNumberIndex; i > 0; --i) {
        if (temp.digits[i] == 0) {
            --(temp.lastNumberIndex);
        } else {
            break;
        }
    }
    return temp;
}

BigInteger& operator*=(BigInteger& first, const BigInteger& second) {
    first = first * second;
    return first;
}

BigInteger ow(uint32_t value, uint32_t power) {
    if (power == 0) {
        return 1;
    }

    if (power % 2 == 1) {
        return ow(value, power - 1) * value;
    } else {
        BigInteger ans = ow(value, power / 2);
        return ans * ans;
    }
}

void BigInteger::MakeSmaller() {
    if (digits[lastNumberIndex] >= 10) {
        digits[lastNumberIndex] /= 10;
    } else if (lastNumberIndex > 0) {
        digits[lastNumberIndex] = 0;
        --lastNumberIndex;
        digits[lastNumberIndex] = base / 10;
    }
}

BigInteger operator/(const BigInteger& firstValue, const BigInteger& secondValue) {
    BigInteger ans;

    if (firstValue.sign != secondValue.sign) {
        if (firstValue.sign == BigInteger::PLUS) {
            ans = firstValue / (-secondValue);
            ans.sign = BigInteger::MINUS;
            return ans;
        } else {
            ans = (-firstValue) / secondValue;
            ans.sign = BigInteger::MINUS;
            return ans;
        }
    } else if (firstValue.sign == BigInteger::MINUS) {
        ans = (-firstValue) / (-secondValue);
        return ans;
    }

    if (firstValue < secondValue) {
        return ans;
    }

    BigInteger bigConst = ow(ans.base, firstValue.lastNumberIndex - secondValue.lastNumberIndex + 1);

    while (firstValue < secondValue * bigConst) {
        bigConst.MakeSmaller();
    }

    BigInteger temp;
    temp = firstValue - secondValue * bigConst;
    ans += bigConst;

    while (temp >= secondValue) {
        while (temp < secondValue * bigConst) {
            bigConst.MakeSmaller();
        }
        temp -= secondValue * bigConst;
        ans += bigConst;
    }
    return ans;
}

BigInteger& operator/=(BigInteger& firstValue, const BigInteger& secondValue) {
    firstValue = firstValue / secondValue;
    return firstValue;
}

BigInteger operator%(const BigInteger& firstValue, const BigInteger& secondValue) {
    BigInteger ans = firstValue / secondValue;
    return firstValue - ans * secondValue;
}

BigInteger findGCF(const BigInteger& first, const BigInteger& second) {
    if (second == 0) {
        return first;
    } else {
        return findGCF(second, first % second);
    }
}

BigInteger sqrt(BigInteger& value) {
    if (value == 0) {
        return 0;
    }

    BigInteger left = 1, right = value;
    BigInteger mid;

    while (left + 1 < right) {
        mid = (right + left) / 2;
        (mid * mid > value ? right : left) = mid;
    }
    return left;
}