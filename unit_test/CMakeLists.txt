set(Wall_Flag "-Wall -Wno-sign-compare -Wno-format -Wno-reorder -Wno-unknown-pragmas")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++17 ${Wall_Flag} -g -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++17 ${Wall_Flag} -g")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/unit_test)

set(MY_LIBS
    libboost_unit_test_framework.so
    libboost_test_exec_monitor.so
    bbt_coroutine
    bbt_conet
    event_pthreads
    ybbt
)

enable_testing()

add_executable(Test_copool Test_copool.cc)
target_link_libraries(Test_copool ${MY_LIBS})
add_test(NAME Test_copool COMMAND Test_copool)