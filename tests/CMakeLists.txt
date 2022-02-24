include(FetchContent)
option(BUILD_GMOCK "Builds the googlemock subproject" OFF)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)

FetchContent_MakeAvailable(googletest)


add_executable(test_map test_map.cpp)
target_link_libraries(test_map PRIVATE map gtest_main)
target_compile_options(test_map PRIVATE -Wall -Wextra -Wpedantic -Werror)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(test_map PRIVATE -fsanitize=address)
    target_link_options(test_map PRIVATE -fsanitize=address)
endif ()

add_test(NAME test_map COMMAND test_map)

add_executable(test_rbtree test_rbtree.cpp)
target_link_libraries(test_rbtree PRIVATE map gtest_main)
target_compile_options(test_rbtree PRIVATE -Wall -Wextra -Wpedantic -Werror)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(test_rbtree PRIVATE -fsanitize=address)
    target_link_options(test_rbtree PRIVATE -fsanitize=address)
endif ()

add_test(NAME test_rbtree COMMAND test_rbtree)