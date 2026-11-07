/**
 * 第 2-1 章 实验任务一：C++ 的输入输出控制
 *
 * 题目：输入若干名学生的信息（姓名、三门课程成绩），计算每名学生的平均成绩，
 *       并输出每位学生的成绩信息表。
 *
 * 输入：第一行是学生人数 n；
 *       之后每行格式为 "姓名 语文 数学 英语"。
 * 输出：姓名、三门成绩与平均分（保留两位小数），平均分 >= 90 的学生标注 Excellent。
 *
 * 说明：题目样例中姓名与数值、数值与数值之间均为一个空格，因此本程序严格按样例格式输出；
 *       为了在成绩出现三位数（如 100.00）时数值列仍然逐行对齐，所有数值字段统一右对齐到
 *       同一宽度（宽度 = 最长数值文本长度 + 1）。在样例数据下该宽度恰好等于"一个空格"，
 *       输出与题目样例逐字一致。
 *
 * 编译：g++ -std=c++11 task1_student_scores.cpp -o task1_student_scores
 */
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

// 学生成绩记录：姓名 + 三门课成绩 + 平均分
struct Student {
    std::string name;
    double chinese;
    double math;
    double english;
    double average;
};

// 把数值格式化为保留两位小数的文本
std::string fixed2(double value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << value;
    return oss.str();
}

}  // namespace

int main() {
    int n = 0;
    if (!(std::cin >> n) || n <= 0) {
        std::cerr << "Error: the number of students must be a positive integer."
                  << std::endl;
        return 1;
    }

    // 用 vector 保存全部记录：先读入、再统一排版输出
    std::vector<Student> students;
    students.reserve(static_cast<std::size_t>(n));

    for (int i = 0; i < n; ++i) {
        Student s;
        if (!(std::cin >> s.name >> s.chinese >> s.math >> s.english)) {
            std::cerr << "Error: invalid record for student No." << (i + 1)
                      << std::endl;
            return 1;
        }
        // 平均分 = 三门成绩之和 / 3，先保留 double 精度，输出时再统一格式化
        s.average = (s.chinese + s.math + s.english) / 3.0;
        students.push_back(s);
    }

    // 预先把所有数值格式化为两位小数的字符串，顺便求出最长的数值文本宽度
    std::vector<std::vector<std::string> > numbers(
        students.size(), std::vector<std::string>(4, ""));
    std::size_t maxNumberLen = 0;
    for (std::size_t i = 0; i < students.size(); ++i) {
        const Student& s = students[i];
        numbers[i][0] = fixed2(s.chinese);
        numbers[i][1] = fixed2(s.math);
        numbers[i][2] = fixed2(s.english);
        numbers[i][3] = fixed2(s.average);
        for (int j = 0; j < 4; ++j) {
            if (numbers[i][j].size() > maxNumberLen) {
                maxNumberLen = numbers[i][j].size();
            }
        }
    }
    // 数值字段宽 = 最长数值长度 + 1：样例数据下为 6（"90.00" 占 5 位 + 1 个空格）
    const std::size_t fieldWidth = maxNumberLen + 1;

    for (std::size_t i = 0; i < students.size(); ++i) {
        std::cout << students[i].name;  // 姓名左对齐，与样例一致，不额外补空格
        for (int j = 0; j < 4; ++j) {
            std::cout << std::right << std::setw(static_cast<int>(fieldWidth))
                      << numbers[i][j];
        }
        if (students[i].average >= 90.0) {
            std::cout << " Excellent";  // 平均分达到 90 分的优秀学生
        }
        std::cout << '\n';
    }

    return 0;
}
