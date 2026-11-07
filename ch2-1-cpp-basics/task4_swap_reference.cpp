/**
 * 第 2-1 章 实验任务四：引用特性考察
 *
 * 题目：编写函数 void swap(int& x, int& y)，使用 C++ 引用交换两个整数；
 *       再重载该函数，交换其它类型的数据。
 *
 * 考点：引用是变量的别名，函数内对引用的操作直接作用于实参，因此不需要指针、
 *       也不会产生拷贝；同时用 double 版本与 int* 版本展示函数重载。
 *
 * 运行：默认输出与题目样例完全一致的交换前后两行；
 *       加 --demo 参数运行可额外演示两个重载版本的交换效果。
 *         task4_swap_reference.exe --demo
 *
 * 编译：g++ -std=c++11 task4_swap_reference.cpp -o task4_swap_reference
 */
#include <iostream>
#include <string>

// ---------------------------------------------------------------------------
// 引用版本：x、y 是实参的别名，交换后实参本身的值被改变
// ---------------------------------------------------------------------------
void swap(int& x, int& y) {
    const int temp = x;
    x = y;
    y = temp;
}

// 重载版本 1：交换两个 double
void swap(double& x, double& y) {
    const double temp = x;
    x = y;
    y = temp;
}

// 重载版本 2：交换两个 int 指针所指向的值（与引用版本对比，说明引用更简洁安全）
void swap(int* x, int* y) {
    if (x == nullptr || y == nullptr) {
        return;  // 防御空指针
    }
    const int temp = *x;
    *x = *y;
    *y = temp;
}

// 可选的重载演示：说明同名函数靠参数类型区分
void demoOverloads() {
    std::cout << "\n[overloaded versions]\n";

    double d1 = 3.14;
    double d2 = 2.72;
    std::cout << "Before swap(double): d1=" << d1 << ", d2=" << d2 << '\n';
    swap(d1, d2);
    std::cout << "After  swap(double): d1=" << d1 << ", d2=" << d2 << '\n';

    int p = 11;
    int q = 22;
    std::cout << "Before swap(int*): p=" << p << ", q=" << q << '\n';
    swap(&p, &q);  // 指针版本需要显式取地址
    std::cout << "After  swap(int*): p=" << p << ", q=" << q << '\n';
}

int main(int argc, char* argv[]) {
    int x = 0;
    int y = 0;
    if (!(std::cin >> x >> y)) {
        std::cerr << "Error: expected two integers." << std::endl;
        return 1;
    }

    // 题目要求的主演示：引用版本（输出与题目样例逐字一致）
    std::cout << "Before swap: x=" << x << ", y=" << y << '\n';
    swap(x, y);  // 实参直接书写，无需取地址
    std::cout << "After  swap: x=" << x << ", y=" << y << '\n';

    // 只有显式加 --demo 时才输出重载演示，保证默认输出与题目样例一致
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--demo") {
            demoOverloads();
            break;
        }
    }

    return 0;
}
