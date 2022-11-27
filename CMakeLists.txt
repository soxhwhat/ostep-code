project(ostep)

add_executable(cpu intro/cpu.c)
add_executable(mem intro/mem.c)
add_executable(mutil cmake-build-debug/mutil.cpp)
add_executable(IO_Redirect cmake-build-debug/IO_Redirect.cpp)