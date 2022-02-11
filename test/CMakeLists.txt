include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(TESTS 
  constructor-test.cpp 
  operator-equals-test.cpp 
  iterator-test.cpp
  capacity-test.cpp
  access-test.cpp
)

add_executable(
  testing
  ${TESTS}
)
target_link_libraries(
  testing
  gtest_main
)

target_include_directories(
    testing
    PRIVATE
    ../include/
)

include(GoogleTest)
gtest_discover_tests(testing)
