cmake_minimum_required(VERSION 3.12)
project(service_template CXX)

include(third_party/userver/cmake/SetupEnvironment.cmake)
include(GNUInstallDirs)

add_subdirectory(third_party/userver)


# Common sources
add_library(${PROJECT_NAME}_objs OBJECT
        include/handlers
        src/handlers/upload.cpp
        src/handlers/draw_line.cpp
        src/handlers/storage_component.cpp
        src/handlers/scaling.cpp
        src/handlers/dithering.cpp
        src/handlers/gradient.cpp
        src/handlers/gamma.cpp
        include/core/bytes.h
        include/core/pnm.h
        src/core/pnm.cpp
        include/core/pnm_type.h
        include/core/body.h src/core/body.cpp
        include/core/header.h src/core/header.cpp
        include/utils/file.h
        src/utils/file.cpp
        include/core/color_space.h
        src/core/pixel.cpp
        include/core/pixel.h
        include/entities/line.h
        src/entities/line.cpp
        include/entities/point.h
        src/entities/point.cpp
        include/entities/canvas.h
        src/entities/canvas.cpp
        src/dithering/atkinson.cpp
        src/dithering/floyd_steinberg.cpp
        src/dithering/ordered.cpp
        src/dithering/random.cpp
        include/dithering
        include/scaling
        src/scaling/nearest_neighbor.cpp
        src/scaling/shifting.cpp
        src/scaling/bi_linear.cpp
        include/utils
        src/utils/useful.cpp
        src/scaling/lanczos.cpp
        src/scaling/bc_splines.cpp
        include/gamma
        src/gamma/gamma.cpp
        include/jpeg
        src/jpeg/jpeg.cpp
        src/jpeg/haffman.cpp
        src/jpeg/idct.cpp
        src/handlers/jpeg.cpp
        )
target_link_libraries(${PROJECT_NAME}_objs PUBLIC userver-core)
target_include_directories(${PROJECT_NAME}_objs PRIVATE src/core)

include_directories(
        include/
)

# The Service
add_executable(${PROJECT_NAME} src/main.cpp include/handlers/convert.h src/handlers/convert.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)

file(GLOB UNITTEST_FILES
        unittests/*_test.cpp
        unittests/*_test.h
        )

# Unit Tests
add_executable(${PROJECT_NAME}_unittest
        ${UNITTEST_FILES}
        )
target_link_libraries(${PROJECT_NAME}_unittest PRIVATE ${PROJECT_NAME}_objs userver-utest)
add_google_tests(${PROJECT_NAME}_unittest)


# Benchmarks
add_executable(${PROJECT_NAME}_benchmark

        )
target_link_libraries(${PROJECT_NAME}_benchmark PRIVATE ${PROJECT_NAME}_objs userver-ubench)
add_google_benchmark_tests(${PROJECT_NAME}_benchmark)

# Functional Tests
add_subdirectory(tests)

if (DEFINED ENV{PREFIX})
    message(STATUS "Set install prefix: $ENV{PREFIX}")
    file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
    set(CMAKE_INSTALL_PREFIX ${PREFIX_PATH})
endif ()

set(CONFIG_VAR_PATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}/config_vars.yaml)
set(CONFIG_FALLBACK_PATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}/dynamic_config_fallback.json)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/configs/static_config.yaml.in ${CMAKE_CURRENT_SOURCE_DIR}/configs/static_config.yaml)

FILE(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})
install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})
