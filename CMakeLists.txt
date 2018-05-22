cmake_minimum_required(VERSION 3.2)

if($ENV{TRAVIS_BUILD_NUMBER})
    set(VERSION_BUILD $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(VERSION_BUILD 0)
endif()

project(allocator VERSION 0.0.${VERSION_BUILD})

# set variables for directories with source code
set(SOURCE_PATH "${PROJECT_SOURCE_DIR}/src")

add_executable(
allocator
${SOURCE_PATH}/main.cpp
${SOURCE_PATH}/forward_list.h
${SOURCE_PATH}/allocator.h
${SOURCE_PATH}/utils.h)

add_executable(
allocator_test_main
${SOURCE_PATH}/test_main.cpp
${SOURCE_PATH}/allocator.h
${SOURCE_PATH}/utils.h)

set_target_properties(
  allocator
  allocator_test_main
  PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_compile_definitions(allocator_test_main PRIVATE BOOST_TEST_DYN_LINK)
target_link_libraries(allocator_test_main
  Boost::unit_test_framework
)

install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT vladimir.englisbox@gmail.com)

include(CPack)

enable_testing()

add_test(tests allocator_test_main)