/**
 * 第 2-2 章 校园一卡通消费管理系统 —— 功能函数实现文件
 *
 * 对应头文件 card_operations.h 中的声明，本文件只写定义。
 * 编译（Visual Studio 中把 card_operations.cpp 与 CampusCardSystem.cpp 一起加入项目）：
 *   g++ -std=c++11 -fexec-charset=GBK card_operations.cpp \
 *       CampusCardSystem.cpp -o CampusCardSystem
 */
#include "card_operations.h"

#include <iomanip>
#include <iostream>
#include <sstream>

namespace {

// 把金额格式化为保留两位小数的字符串，避免修改 std::cout 的全局格式状态
std::string money(double amount) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << amount;
    return oss.str();
}

}  // namespace

namespace card {

// ---------------------------------------------------------------------------
// 模块二：命名空间与格式化输出
// ---------------------------------------------------------------------------
void printLine(char c, int len) {
    if (len <= 0) {
        return;
    }
    std::cout << std::string(static_cast<std::size_t>(len), c) << '\n';
}

void title(const std::string& s) {
    printLine('=', LINE_LEN);
    std::cout << s << '\n';
    printLine('=', LINE_LEN);
}

// ---------------------------------------------------------------------------
// 模块三：动态内存管理与账户录入
// ---------------------------------------------------------------------------
Card* createCards(int n) {
    if (n <= 0) {
        throw CardException("the number of accounts must be a positive integer");
    }

    Card* arr = nullptr;
    try {
        // new[] 动态分配数组；new 失败时抛出 std::bad_alloc
        arr = new Card[static_cast<std::size_t>(n)]();
    } catch (const std::bad_alloc&) {
        throw CardException("failed to allocate memory for accounts");
    }
    return arr;
}

void destroyCards(Card* p) {
    delete[] p;  // delete[] 与 new[] 成对出现；对空指针是安全的
}

void inputCards(Card* arr, int n) {
    if (arr == nullptr || n <= 0) {
        throw CardException("invalid account array");
    }

    for (int i = 0; i < n; ++i) {
        bool done = false;
        while (!done) {
            std::cout << "Account " << (i + 1) << " (id name balance): ";

            std::string line;
            if (!std::getline(std::cin, line)) {
                // 输入流结束（如重定向文件读完），无法继续录入，向上抛出终止
                throw CardException(
                    "input stream ended before all accounts were read");
            }
            if (line.empty()) {
                continue;  // 空行直接忽略，重新提示
            }

            try {
                std::istringstream iss(line);
                Card tmp = Card();
                if (!(iss >> tmp.id >> tmp.name >> tmp.balance)) {
                    throw CardException(
                        "format error: expected <id> <name> <balance>");
                }
                if (tmp.id <= 0) {
                    throw CardException("student id must be a positive integer");
                }
                if (tmp.balance < 0.0) {
                    throw CardException("initial balance can not be negative");
                }
                tmp.transactions = 0;  // 交易次数初始化
                arr[i] = tmp;
                done = true;
            } catch (const CardException& e) {
                // 捕获本行的非法数据，给出提示后重新录入当前账户
                std::cout << "[input error] " << e.what()
                          << ", please try again.\n";
            }
        }
    }
}

// ---------------------------------------------------------------------------
// 模块四：充值、消费、只读查询
// ---------------------------------------------------------------------------
void deposit(Card& c, double amount, double fee) {
    if (amount <= 0.0) {
        throw CardException("deposit amount must be greater than zero");
    }
    if (fee < 0.0 || fee >= 1.0) {
        throw CardException("invalid fee rate");
    }

    const double charge = amount * fee;   // 手续费
    c.balance += amount - charge;         // 实际到账 = 充值金额 - 手续费
    ++c.transactions;                     // 交易次数累加

    std::cout << "Deposit " << money(amount) << " (fee rate " << money(fee)
              << "): charge " << money(charge) << ", credited "
              << money(amount - charge) << ", balance " << money(c.balance) << '\n';
}

bool spend(Card& c, double cost) {
    if (cost < 0.0) {
        throw CardException("cost can not be negative");  // 非法金额主动抛异常
    }
    if (cost > c.balance) {
        return false;  // 余额不足：不扣款，返回 false
    }
    c.balance -= cost;
    ++c.transactions;
    return true;
}

bool spend(Card& c, double cost, const std::string& item) {
    // 先校验再输出并扣款：余额不足时不打印"消费项目"，避免失败的交易看起来像成功
    if (cost < 0.0) {
        throw CardException("cost can not be negative");
    }
    if (cost > c.balance) {
        throw CardException("insufficient balance for item: " + item);
    }
    std::cout << "Purchase item: " << item << ", cost: " << money(cost) << '\n';
    return spend(c, cost);  // 调用同名的基础版本完成扣款
}

void showBalance(const Card& c) {
    // 常引用：只能读取 c 的成员，若写 c.balance -= 1.0 会编译失败
    std::cout << "Account " << c.id << " (" << c.name << ") balance: "
              << money(c.balance) << " yuan\n";
}

void viewStudentID(const Card* p) {
    // 指向常量的指针：可以读取 *p，但不能通过 p 修改账户
    if (p == nullptr) {
        std::cout << "Student ID: (null account pointer)\n";
        return;
    }
    std::cout << "Student ID: " << p->id << '\n';
}

// ---------------------------------------------------------------------------
// 模块五：强制类型转换（元 -> 分）
// ---------------------------------------------------------------------------
int toCent(double amount) {
    // 1 元 = 100 分；先乘 100 再四舍五入，最后用 static_cast<int> 安全截断，
    // 比 C 风格的 (int) 转换更明确，也更容易被编译器检查
    const double scaled = amount * 100.0 + 0.5;
    return static_cast<int>(scaled);
}

// ---------------------------------------------------------------------------
// 模块六：报表输出
// ---------------------------------------------------------------------------
void printReport(const Card* arr, int n) {
    if (arr == nullptr || n <= 0) {
        throw CardException("no account to report");
    }

    // 按最长姓名动态计算姓名列宽，保证任意长度的姓名都不会把后面的列挤歪
    std::size_t nameWidth = 4;
    for (int i = 0; i < n; ++i) {
        if (arr[i].name.size() > nameWidth) {
            nameWidth = arr[i].name.size();
        }
    }
    nameWidth += 4;
    const int idWidth = 10;
    const int balanceWidth = 12;
    const int transWidth = 14;

    title("Account Report");
    std::cout << std::left << std::setw(idWidth) << "ID"
              << std::setw(static_cast<int>(nameWidth)) << "Name"
              << std::right << std::setw(balanceWidth) << "Balance"
              << std::setw(transWidth) << "Transactions" << '\n';
    printLine('-', LINE_LEN);

    std::cout << std::fixed << std::setprecision(2);  // 余额保留两位小数
    for (int i = 0; i < n; ++i) {
        std::cout << std::left << std::setw(idWidth) << arr[i].id
                  << std::setw(static_cast<int>(nameWidth)) << arr[i].name
                  << std::right << std::setw(balanceWidth) << arr[i].balance
                  << std::setw(transWidth) << arr[i].transactions;

        if (arr[i].balance < WARNING_BALANCE) {
            std::cout << "   [LOW BALANCE]";  // 使用系统常量做低余额提醒
        }
        std::cout << '\n';
    }
    printLine('-', LINE_LEN);
}

}  // namespace card
