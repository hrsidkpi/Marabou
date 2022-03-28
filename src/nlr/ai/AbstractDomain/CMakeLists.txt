file(GLOB SRCS "*.cpp")
file(GLOB HEADERS "*.h")

target_sources(${MARABOU_LIB} PRIVATE ${SRCS})
target_include_directories(${MARABOU_LIB} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(Zonotope)
add_subdirectory(ZonotopeN)
add_subdirectory(PolyhedronUnder)
add_subdirectory(PolyhedronUnderN)