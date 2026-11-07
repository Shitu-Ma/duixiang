/**
 * 第 2-2 章 校园一卡通消费管理系统 —— 主程序
 *
 * 主程序只负责"用户交互 + 功能调度"，所有业务逻辑都在 card_operations.cpp 中，
 * 通过包含 card_operations.h 调用模块接口（多文件编程）。
 *
 * 编译：
 *   g++ -std=c++11 -fexec-charset=GBK card_operations.cpp \
 *       CampusCardSystem.cpp -o CampusCardSystem
 *   （-fexec-charset=GBK 让中文字符串按 GBK 编码输出，与中文 Windows 控制台一致；
 *     Visual Studio 下源文件带 UTF-8 BOM 即可自动按当前代码页输出，无需该选项。）
 */
#include "card_operations.h"

#include <iostream>
#include <limits>
#include <string>

namespace {

// 读取账户数量：非法输入（非整数、非正数）时给出提示并重新读取
int readAccountCount() {
    int n = 0;
    while (true) {
        std::cout << "Number of accounts: ";
        if (std::cin >> n && n > 0) {
            // 读走本行剩余字符，避免影响后面 inputCards 中的 getline
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return n;
        }
        std::cout << "[input error] please input a positive integer.\n";
        std::cin.clear();  // 清除失败状态
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (std::cin.eof() || !std::cin) {
            // 输入流已结束或底层出错，继续循环只会原地打转，直接抛出终止
            throw card::CardException("input stream ended");
        }
    }
}

}  // namespace

int main() {
    card::Card* accounts = nullptr;  // 动态账户数组，初始为空

    try {
        // 步骤 1：输出系统标题
        card::title("Campus Card Consumption Management System");

        // 步骤 2：输入学生数量，并动态创建账户数组后录入信息
        const int n = readAccountCount();
        accounts = card::createCards(n);  // new[] 动态分配
        card::inputCards(accounts, n);
        card::printLine();

        // 步骤 3：对第一个账户充值 100 元、消费 8.5 元（项目："食堂午餐"）
        std::cout << "Step 3: deposit 100 yuan and spend 8.5 yuan on account 1.\n";
        card::deposit(accounts[0], 100.0);              // 不传 fee，使用默认手续费率
        card::spend(accounts[0], 8.5, "食堂午餐");       // 余额不足时重载版本会抛出异常
        card::printLine();

        // 步骤 4：显示该账户余额与账户 ID
        std::cout << "Step 4: show the balance and the student id.\n";
        card::showBalance(accounts[0]);              // 常引用查询
        card::viewStudentID(&accounts[0]);           // 指向常量的指针查询
        card::printLine();

        // 步骤 5：把余额换算为"分"
        std::cout << "Step 5: convert the balance to cents.\n";
        const int cents = card::toCent(accounts[0].balance);
        std::cout << "Balance in cents: " << cents << '\n';
        card::printLine();

        // 步骤 6：输出全部账户报表
        std::cout << "Step 6: print the report of all accounts.\n";
        card::printReport(accounts, n);
        card::printLine();

        // 步骤 7：异常处理演示——消费金额超过余额时抛出并捕获异常
        std::cout << "Step 7: exception demo (spend more than the balance).\n";
        try {
            const double tooMuch = accounts[0].balance + 1000.0;
            card::spend(accounts[0], tooMuch, "laptop");  // 余额不足，抛 CardException
        } catch (const card::CardException& e) {
            std::cout << "[caught] " << e.what() << '\n';
        }

        // 步骤 8：释放动态内存
        std::cout << "Step 8: release the dynamic array.\n";
        card::destroyCards(accounts);  // delete[]
        accounts = nullptr;
        std::cout << "Done. All memory released, goodbye.\n";
    } catch (const std::bad_alloc&) {
        std::cout << "[fatal] out of memory.\n";
        card::destroyCards(accounts);
        return 1;
    } catch (const std::exception& e) {
        std::cout << "[fatal] " << e.what() << '\n';
        card::destroyCards(accounts);
        return 1;
    }

    return 0;
}
