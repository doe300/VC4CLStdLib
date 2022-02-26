find_package(OpenCL REQUIRED)
find_package(Threads REQUIRED)

add_executable(compare_implementations compare_implementations.cpp)
target_compile_features(compare_implementations PRIVATE cxx_std_14)
target_compile_options(compare_implementations PRIVATE -g -Og)
target_link_libraries(compare_implementations OpenCL::OpenCL Threads::Threads)
