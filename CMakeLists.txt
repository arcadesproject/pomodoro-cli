cmake_minimum_required(VERSION 3.10)
project(PomodoroCLI C)

# Output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable compile_commands.json for clangd and clang-tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Prefer Clang if available
set(CMAKE_C_COMPILER "clang" CACHE STRING "C compiler")
set(CMAKE_CXX_COMPILER "clang++" CACHE STRING "C++ compiler")

# Set to 99 standards, avoid 11
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)


# Enable AddressSanitizer and debug info for Clang/GCC only for testing with asan dynamic x86-64 dll from llvm clang lib folder
# if (CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
#     add_compile_options(-fsanitize=address -g)
#     add_link_options(-fsanitize=address)
# endif()

# Enable clang-tidy if available
find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if (CLANG_TIDY_EXE)
    set(CMAKE_C_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()

# Include headers
include_directories(${PROJECT_SOURCE_DIR}/src)

# Source files
set(SOURCES
    src/main.c
    src/timer.c
    src/input_utils.c
    src/log_session.c
    resources/pomodoro.rc
)

# Create executable (CLI app)
# add_executable(pomodoro ${SOURCES})
# Create executable (Windows subsystem app)
add_executable(pomodoro WIN32 ${SOURCES})

