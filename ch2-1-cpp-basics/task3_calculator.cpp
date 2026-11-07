/**
 * 第 2-1 章 实验任务三：C++ 特色函数考察（简易多功能计算器）
 *
 * 考点：
 *   1. 内联函数 inline double square(double x);
 *   2. 带默认参数值的函数 double power(double base, int exponent = 2);
 *   3. 函数重载 double calc(double a, double b, char op) /
 *                double calc(int a, int b, char op);
 *
 * 输入：两个操作数与一个运算符，如 "3 5 +"。
 * 输出：依次输出 square、power、calc 的计算结果，除数为 0 时输出
 *       "Error: divide by zero"。
 *
 * 编译：g++ -std=c++11 task3_calculator.cpp -o task3_calculator
 */
#include <climits>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

// ---------------------------------------------------------------------------
// 1. 内联函数：函数体短小、调用频繁，用 inline 建议编译器在调用处展开
// ---------------------------------------------------------------------------
inline double square(double x) {
    return x * x;
}

// ---------------------------------------------------------------------------
// 2. 带默认参数：调用 power(x) 时按 exponent = 2 计算平方
// ---------------------------------------------------------------------------
double power(double base, int exponent = 2) {
    if (exponent < 0) {
        throw std::invalid_argument("Error: negative exponent is not supported");
    }
    double result = 1.0;
    for (int i = 0; i < exponent; ++i) {
        result *= base;
    }
    return result;
}

namespace {

// 除零检查：失败时抛出异常，由 main 中的 catch 统一提示
double checkDivisor(double divisor) {
    if (divisor == 0.0) {
        throw std::runtime_error("Error: divide by zero");
    }
    return divisor;
}

// 把字符串解析为实数；解析失败时抛出统一风格的错误信息
double parseNumber(const std::string& s) {
    try {
        std::size_t used = 0;
        const double value = std::stod(s, &used);
        if (used != s.size()) {
            throw std::runtime_error("Error: invalid operand");
        }
        return value;
    } catch (const std::exception&) {
        throw std::runtime_error("Error: invalid operand");
    }
}

// 把 3.0 这样的整数型实数格式化为 "3"，让输出标签与题目样例一致
std::string formatLabel(double v) {
    std::ostringstream oss;
    if (v > -1e15 && v < 1e15 && v == static_cast<long long>(v)) {
        oss << static_cast<long long>(v);
    } else {
        oss << v;
    }
    return oss.str();
}

// 判断一个记号是否写成整数（不含小数点、指数），用于选择 calc 的 int 重载
bool isIntegerToken(const std::string& s) {
    if (s.empty()) {
        return false;
    }
    std::size_t i = (s[0] == '+' || s[0] == '-') ? 1 : 0;
    if (i >= s.size()) {
        return false;
    }
    for (; i < s.size(); ++i) {
        if (s[i] < '0' || s[i] > '9') {
            return false;
        }
    }
    return true;
}

// 判断能否安全地转换为 int（防止 static_cast 溢出）
bool fitsInInt(double v) {
    return v >= -static_cast<double>(INT_MAX) && v <= static_cast<double>(INT_MAX);
}

}  // namespace

// ---------------------------------------------------------------------------
// 3. 重载函数：double 版本
// ---------------------------------------------------------------------------
double calc(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / checkDivisor(b);
        default: throw std::invalid_argument("Error: unsupported operator");
    }
}

// 3. 重载函数：int 版本。注意返回值仍是 double：
//    两个操作数是整数，但除法按实数计算，避免整数截断丢失精度
double calc(int a, int b, char op) {
    switch (op) {
        case '+': return static_cast<double>(a + b);
        case '-': return static_cast<double>(a - b);
        case '*': return static_cast<double>(a * b);
        case '/': return static_cast<double>(a) /
                          checkDivisor(static_cast<double>(b));
        default: throw std::invalid_argument("Error: unsupported operator");
    }
}

int main() {
    std::string sa, sb, sop;
    if (!(std::cin >> sa >> sb >> sop) || sop.size() != 1) {
        std::cerr << "Error: expected input like: 3 5 +" << std::endl;
        return 1;
    }
    const char op = sop[0];

    std::cout << std::fixed << std::setprecision(2);  // 结果统一保留两位小数

    try {
        const double a = parseNumber(sa);
        const double b = parseNumber(sb);

        // 依次调用 square()、power()、calc()
        const double sq = square(a);
        const double sqByDefault = power(a);  // 默认参数：exponent = 2，应与 square(a) 相同
        const double cube = power(a, 3);      // 显式给出指数 3（与题目样例一致）

        std::cout << "square(" << formatLabel(a) << ") = " << sq << '\n';
        std::cout << "power(" << formatLabel(a) << ",3) = " << cube << '\n';

        // 若两次求平方结果不同，说明默认参数实现有误（仅内部自检，不输出）
        if (sqByDefault != sq) {
            std::cerr << "Warning: power(a) and square(a) disagree." << std::endl;
        }

        double result = 0.0;
        if (isIntegerToken(sa) && isIntegerToken(sb)
            && fitsInInt(a) && fitsInInt(b)) {
            // 两个操作数都写成整数：调用 int 重载
            result = calc(static_cast<int>(a), static_cast<int>(b), op);
        } else {
            // 否则调用 double 重载
            result = calc(a, b, op);
        }

        std::cout << formatLabel(a) << ' ' << op << ' ' << formatLabel(b)
                  << " = " << result << '\n';
    } catch (const std::exception& e) {
        // 除数为 0 时输出 "Error: divide by zero"（与题目样例一致）
        std::cout << e.what() << std::endl;
    }

    return 0;
}
