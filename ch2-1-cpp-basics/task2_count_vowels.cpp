/**
 * 第 2-1 章 实验任务二：指向常量的指针特性考察
 *
 * 题目：编写 C++ 函数 int countVowels(const char* p)，
 *       统计字符串中元音字母（a, e, i, o, u）的个数。
 *
 * 要点：形参 const char* 是"指向常量的指针"——
 *       1) 不能通过 p 修改所指向的字符（*p = 'x' 编译报错）；
 *       2) 指针变量 p 本身不是常量，仍可执行 ++p 遍历字符串。
 *
 * 编译：g++ -std=c++11 task2_count_vowels.cpp -o task2_count_vowels
 */
#include <iostream>
#include <string>

// 统计以 '\0' 结尾的字符串中元音字母的个数（不区分大小写）
int countVowels(const char* p) {
    int count = 0;
    while (p != nullptr && *p != '\0') {
        char ch = *p;  // 只能读取 *p，不能写入 *p（p 指向 const char）

        // 大写字母统一转成小写，实现大小写不敏感的比较
        if (ch >= 'A' && ch <= 'Z') {
            ch = static_cast<char>(ch - 'A' + 'a');
        }

        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
            ++count;
        }

        ++p;  // 指针本身可以移动，这是 const char* 与 const char* const 的区别
    }
    return count;
}

int main() {
    std::string line;
    if (!std::getline(std::cin, line)) {
        std::cerr << "Error: no input." << std::endl;
        return 1;
    }

    // std::string::c_str() 返回 const char*，与函数形参类型完全匹配
    std::cout << "Number of vowels: " << countVowels(line.c_str()) << std::endl;
    return 0;
}
