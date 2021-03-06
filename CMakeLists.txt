cmake_minimum_required(VERSION 3.10)
project(naruto)

set(CMAKE_CXX_STANDARD 17)

#include(${CMAKE_SOURCE_DIR}/cmake/genPreprocessorFile.cmake)

include_directories(.)
include_directories(utils)
include_directories(unittest)
include_directories(connection)
include_directories(command)
include_directories(database)
include_directories(link)
include_directories(replication)
include_directories(cluster)
include_directories(third_party)
include_directories(/usr/local/include)

link_directories(/usr/local/lib)
link_directories(/usr/lib)
link_libraries(glog ev gflags protobuf gtest)

aux_source_directory(. NARUTO_SRC)
aux_source_directory(clients/c++ CLIENT_SRC)
aux_source_directory(utils UTILS_SRC)
aux_source_directory(unittest UT_SRC)
aux_source_directory(protocol PROTOCOL_SRC)
aux_source_directory(connection CONNECTION_SRC)
aux_source_directory(database DATABASE_SRC)
aux_source_directory(command COMMAND_SRC)
aux_source_directory(sink SINK_SRC)
aux_source_directory(link LINK_SRC)
aux_source_directory(cluster CLUSTER_SRC)
aux_source_directory(replication REPLICA_SRC)
list(REMOVE_ITEM NARUTO_SRC ./main.cpp)

#aux_source_directory(. NARUTO_SRC)
#source_header_dir(H_D .)
#source_header_dir(H_D utils)
#source_header_dir(H_D protocol)
#source_header_dir(H_D connection)
#source_header_dir(H_D database)
#source_header_dir(H_D command)
#source_header_dir(H_D sink)
#gen_preprocessor_file(${H_D} ${NARUTO_SRC} ${DATABASE_SRC})

add_executable(client ./clients/example/c++/example.cpp
        ${CLIENT_SRC}
        ${UTILS_SRC}
        ${PROTOCOL_SRC}
        ${CONNECTION_SRC})

add_executable(naruto main.cpp
        ${DATABASE_SRC}
        ${COMMAND_SRC}
        ${NARUTO_SRC}
        ${UTILS_SRC}
        ${PROTOCOL_SRC}
        ${CONNECTION_SRC}
        ${SINK_SRC}
        ${REPLICA_SRC}
        ${LINK_SRC}
        ${CLUSTER_SRC})

add_executable(ut ./unittest/ut_main.cpp
        ${DATABASE_SRC}
        ${COMMAND_SRC}
        ${NARUTO_SRC}
        ${UTILS_SRC}
        ${PROTOCOL_SRC}
        ${CONNECTION_SRC}
        ${SINK_SRC}
        ${REPLICA_SRC}
        ${LINK_SRC}
        ${CLUSTER_SRC})