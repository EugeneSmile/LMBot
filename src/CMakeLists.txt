
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION_BRANCH "N/A")
set(PROJECT_VERSION_COMMIT "N/A")

# Get the current working branch
execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE PROJECT_VERSION_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE)
    
# Get the latest commit hash
execute_process(
        COMMAND git rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE PROJECT_VERSION_COMMIT
        OUTPUT_STRIP_TRAILING_WHITESPACE)

set(PROJECT_VERSION_FULL ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${PROJECT_VERSION_BRANCH}-${PROJECT_VERSION_COMMIT})

configure_file(
    ${PROJECT_SOURCE_DIR}/src/utils/Version.h.in
    ${PROJECT_BINARY_DIR}/src/utils/Version.h)

set(BOT_DEPENDENCIES TgBot spdlog cxxopts tomlplusplus::tomlplusplus)

file(GLOB_RECURSE src_bot bot/*.cc bot/*.h)
file(GLOB_RECURSE src_managers managers/*.cc managers/*.h)
file(GLOB_RECURSE src_modules modules/*.cc modules/*.h)
file(GLOB_RECURSE src_utils utils/*.cc utils/*.h)
file(GLOB_RECURSE src_vars vars/*.cc vars/*.h)
add_executable(${CMAKE_PROJECT_NAME} Main.cc ${src_bot} ${src_managers} ${src_modules} ${src_utils} ${src_vars})
add_dependencies(${CMAKE_PROJECT_NAME} ${BOT_DEPENDENCIES})

include_directories(bot managers modules utils vars ${PROJECT_BINARY_DIR}/src/utils)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSQLITE3 sqlite3)
include_directories(${LIBSQLITE3_INCLUDE_DIRS})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs)
link_directories(${CMAKE_BINARY_DIR}/bin)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
set(Boost_USE_MULTITHREADED ON)
find_package(CURL)
include_directories(include ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} ${BOT_DEPENDENCIES} ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${LIBSQLITE3_LIBRARIES})
