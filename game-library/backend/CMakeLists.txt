cmake_minimum_required(VERSION 3.5)
project(GameLibrary CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Drogon
find_package(Drogon REQUIRED CONFIG)
find_package(jsoncpp REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/models/Jeu.cpp
    src/models/Membre.cpp
    src/models/Emprunt.cpp
    src/services/BibliothequeService.cpp
    src/controllers/JeuxController.cpp
    src/controllers/MembresController.cpp
    src/controllers/EmpruntsController.cpp
    src/supabase/SupabaseService.cpp
)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Fetch and link nlohmann_json and cpp-httplib if not provided
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_Declare(
    cpp_httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.11.9
)
FetchContent_MakeAvailable(nlohmann_json cpp_httplib)

# Create executable
add_executable(game-library ${SOURCES})

# Link libraries
target_link_libraries(game-library PRIVATE Drogon::Drogon jsoncpp nlohmann_json)

# httplib is header-only; include its directory
target_include_directories(game-library PRIVATE ${cpp_httplib_SOURCE_DIR})

# Test target (smoke tests for Supabase integration)
add_executable(supabase_test tests/supabase_test.cpp)
target_include_directories(supabase_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${cpp_httplib_SOURCE_DIR})
target_link_libraries(supabase_test PRIVATE nlohmann_json)

# Install target
install(TARGETS game-library DESTINATION bin)
