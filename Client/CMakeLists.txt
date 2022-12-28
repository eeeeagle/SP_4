add_executable(Client Client.cpp)
include_directories(${CMAKE_SOURCE_DIR}/lib)

install(TARGETS Client RUNTIME DESTINATION client)