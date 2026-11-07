# 第 2-1 章：四个任务各含一个 main()，分别生成四个可执行文件
set(TASKS
    task1_student_scores
    task2_count_vowels
    task3_calculator
    task4_swap_reference
)

foreach(task ${TASKS})
    add_executable(${task} ${task}.cpp)
endforeach()

# 让 Visual Studio 解决方案里能直接看到样例输入
set_target_properties(task1_student_scores PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data")
