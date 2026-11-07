/**
 * 第 2-2 章 校园一卡通消费管理系统 —— 函数声明头文件
 *
 * 本文件只放：系统常量、数据类型（Card）、异常类型与函数声明，
 * 具体的函数定义写在 card_operations.cpp 中，主程序只包含本头文件即可调用。
 */
#ifndef CARD_OPERATIONS_H
#define CARD_OPERATIONS_H

#include <stdexcept>
#include <string>

namespace card {

// ---- 系统常量 -------------------------------------------------------------
// 充值手续费率：每笔充值按 2% 收取手续费
constexpr double FEE_RATE = 0.02;
// 低余额警戒值：余额低于该值时在报表中给出提示
constexpr double WARNING_BALANCE = 10.0;
// 分隔线默认长度
constexpr int LINE_LEN = 60;

// ---- 账户数据结构 ---------------------------------------------------------
struct Card {
    int id;             // 学号
    std::string name;   // 姓名
    double balance;     // 当前余额（元）
    int transactions;   // 交易次数
};

// ---- 自定义异常类型 -------------------------------------------------------
// 统一继承 std::runtime_error，主程序按一种类型即可捕获全部业务异常
class CardException : public std::runtime_error {
public:
    explicit CardException(const std::string& message)
        : std::runtime_error(message) {}
};

// ---- 命名空间内的输出格式化函数（模块二）----------------------------------
// 打印分隔线：默认 60 个 '-'
void printLine(char c = '-', int len = LINE_LEN);
// 打印标题：上下各一条 '=' 分隔线，中间为标题文字
void title(const std::string& s);

// ---- 动态内存管理与账户录入（模块三）--------------------------------------
// 用 new[] 创建 n 个账户，失败时抛 CardException
Card* createCards(int n);
// 用 delete[] 释放账户数组，允许传入空指针
void destroyCards(Card* p);
// 循环录入 n 个账户信息，交易次数初始化为 0；非法输入抛异常并重新录入
void inputCards(Card* arr, int n);

// ---- 账户操作（模块四）----------------------------------------------------
// 充值：先从充值金额中按 fee 收取手续费，再把余额增加，交易次数加 1
void deposit(Card& c, double amount, double fee = FEE_RATE);
// 消费：余额足够则扣款、交易次数加 1 并返回 true，否则返回 false
bool spend(Card& c, double cost);
// 消费（重载）：先校验金额与余额，再输出消费项目并完成扣款；
// 金额非法或余额不足时抛出 CardException（对应模块九的消费异常处理）
bool spend(Card& c, double cost, const std::string& item);
// 常引用查询：只读访问，函数内不能修改账户
void showBalance(const Card& c);
// 指向常量的指针查询：只读访问，不能通过 p 修改账户
void viewStudentID(const Card* p);

// ---- 类型转换与报表（模块五、模块六）--------------------------------------
// 元 -> 分，使用 static_cast<int> 完成安全的强制类型转换
int toCent(double amount);
// 以表格形式输出全部账户的 ID / Name / Balance / Transactions
void printReport(const Card* arr, int n);

}  // namespace card

#endif  // CARD_OPERATIONS_H
