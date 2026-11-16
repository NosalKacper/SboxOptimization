add_executable(NonlinearityTest
    ../src/costfunctions/tests/NonlinearityTest.cc
    ../src/costfunctions/Nonlinearity.cc
)

target_link_libraries(NonlinearityTest
    PRIVATE
    GTest::gtest_main
)

target_include_directories(NonlinearityTest
    PRIVATE ${CMAKE_SOURCE_DIR}
)

include(GoogleTest)
gtest_discover_tests(NonlinearityTest)

add_executable(DifferentialUniformityTest
    ../src/costfunctions/tests/DifferentialUniformityTest.cc
    ../src/costfunctions/DifferentialUniformity.cc
)

target_link_libraries(DifferentialUniformityTest
    PRIVATE
    GTest::gtest_main
)

target_include_directories(DifferentialUniformityTest
    PRIVATE ${CMAKE_SOURCE_DIR}
)

gtest_discover_tests(DifferentialUniformityTest)
