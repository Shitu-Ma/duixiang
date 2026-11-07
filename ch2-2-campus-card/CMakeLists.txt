# 第 2-2 章：校园一卡通消费管理系统（两个源文件一起编译链接）
add_executable(CampusCardSystem
    CampusCardSystem.cpp
    card_operations.cpp
)

# 中文 Windows 控制台使用 GBK 代码页，MinGW 下把中文字面量按 GBK 输出，避免乱码
if(MINGW)
    target_compile_options(CampusCardSystem PRIVATE -fexec-charset=GBK)
endif()

set_target_properties(CampusCardSystem PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data")
