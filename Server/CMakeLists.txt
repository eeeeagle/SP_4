add_executable(Server Server.cpp)
include_directories(${CMAKE_SOURCE_DIR}/lib)

install(TARGETS Server RUNTIME DESTINATION server)