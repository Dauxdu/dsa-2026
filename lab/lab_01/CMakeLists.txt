project(lab_01 LANGUAGES CXX)

add_executable(${PROJECT_NAME} main.cpp)

target_sources(${PROJECT_NAME}
    PUBLIC
    FILE_SET CXX_MODULES FILES
    src/vector.cppm
)
