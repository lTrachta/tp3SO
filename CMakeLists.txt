# Verify cmake version
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Create project
project(
    "ShellProject"
    VERSION 1.0.0
    DESCRIPTION "Shell para monitorear métricas de nuestro sistema en la era postapocalíptica"
    LANGUAGES C
)

# Enable testing
include(CTest)

# Include headers for shell and tests
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/tests/include)
include_directories(${CMAKE_BINARY_DIR}/include) # Conan integration

# Incluye el toolchain de Conan
include(${CMAKE_BINARY_DIR}/Release/generators/conan_toolchain.cmake)

message("Library paths for cJSON: ${CMAKE_BINARY_DIR}/generators")

# Flags for compiling
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wpedantic -Werror -Wextra -Wunused-parameter -Wmissing-prototypes -Wstrict-prototypes")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

# Habilitar la cobertura en tu proyecto
set(CMAKE_CXX_FLAGS "--coverage")
set(CMAKE_C_FLAGS "--coverage")

# Configure paths for Conan
set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/Release/generators" ${CMAKE_BINARY_DIR})

# Find the cJSON dependency managed by Conan
find_package(cJSON REQUIRED)

# Define the executable for the shell
add_executable(ShellProject
    src/main.c src/prompt.c src/shell.c src/pipes.c src/json.c
    tp2SO/src/getinfojson.c src/filesystem.c
)

# Define the executable for tests
add_executable(run_tests
    tests/src/test_json.c tests/src/test_pipes.c tests/src/test_shell.c
    tests/src/main.c src/json.c src/shell.c src/pipes.c
    tp2SO/src/getinfojson.c
)

# Link libraries for the shell
target_link_libraries(ShellProject PRIVATE
    cjson::cjson # Link cJSON
)

# Link libraries for tests
target_link_libraries(run_tests PRIVATE
    cjson::cjson # Link cJSON for tests
)

# Ejecutar make en la subcarpeta tp2SO automáticamente
execute_process(
    COMMAND make
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tp2SO
)

# Copiar la carpeta tests a la carpeta de construcción (build)
file(COPY ${CMAKE_SOURCE_DIR}/tests DESTINATION ${CMAKE_BINARY_DIR})

# Enable tests with CTest
add_test(NAME run_tests COMMAND run_tests)

# Custom target for generating coverage report
add_custom_target(coverage
    COMMAND ./run_tests                 # Ejecuta los tests
    COMMAND lcov --directory . --capture --output-file coverage.info
    COMMAND genhtml coverage.info --output-directory coverage_report
    #COMMAND xdg-open coverage_report/index.html
    COMMENT "Running tests and generating code coverage report with lcov and genhtml..."
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}  # Cambia a la carpeta de compilación
    DEPENDS run_tests                    # Asegura que los tests se compilen antes de ejecutarse
)

# Mensaje opcional para indicar que el informe está listo
add_custom_command(
    TARGET coverage POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Coverage report generated in: ${CMAKE_BINARY_DIR}/coverage_report/index.html"
)
