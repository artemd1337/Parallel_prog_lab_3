# CMakeList.txt: проект CMake для Parallel_prog_lab_3; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Добавьте источник в исполняемый файл этого проекта.
find_package(MPI REQUIRED)
add_executable (Parallel_prog_lab_3 "Parallel_prog_lab_3.cpp" "Parallel_prog_lab_3.h")

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
target_link_libraries(Parallel_prog_lab_3 PUBLIC MPI::MPI_CXX)