cmake_minimum_required(VERSION 3.11)

project(
	PlainRaft
	LANGUAGES CXX
)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	add_compile_options(-Wall -Wextra -g) #-Werror
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -std=c++17 -pthread")

	include(CTest)
endif()

# installation destination
if (UNIX)
	include(GNUInstallDirs)
	set(PLAINRAFT_INC_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}/praft")
	set(PLAINRAFT_RUNTIME_INSTALL_DIR "${CMAKE_INSTALL_BINDIR}")
	set(PLAINRAFT_LIBRARY_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
	set(PLAINRAFT_ARCHIVE_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
	set(PLAINRAFT_CMAKE_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/praft/cmake")
	set(PLAINRAFT_ADDITIONAL_FILES_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/praft")
elseif (WIN32)
	message(FATAL_ERROR "Could not support windows now")
else()
	message(FATAL_ERROR "Could not set install folder for this platform")

endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# find_package(Protobuf REQUIRED)
find_package(Rocksdb REQUIRED)

include(./cmake/grpc_common.cmake)

# Proto file
get_filename_component(raft_proto "./protos/raft.proto" ABSOLUTE)
get_filename_component(raft_proto_path ${raft_proto} PATH)

# Generated sources
set(raft_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/raft.pb.cc")
set(raft_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/raft.pb.h")
set(raft_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/raft.grpc.pb.cc")
set(raft_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/raft.grpc.pb.h")

add_custom_command(      
	  OUTPUT "${raft_proto_srcs}" "${raft_proto_hdrs}" "${raft_grpc_srcs}" "${raft_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${raft_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${raft_proto}"
      DEPENDS "${raft_proto}"
)

# include genetated *pb.h file
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# raft_grpc_proto
add_library( raft_grpc_proto
	${raft_grpc_srcs}
	${raft_grpc_hdrs}
	${raft_proto_srcs}
	${raft_proto_hdrs}
)
target_link_libraries( raft_grpc_proto
	${_REFLECTION}
	${_GRPC_GRPCPP}
	${_PROTOBUF_LIBPROTOBUF}
)

# include project header
include_directories(${PROJECT_SOURCE_DIR}/include)

# subdirectory
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(examples)

# install license
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE" 
        DESTINATION ${PLAINRAFT_ADDITIONAL_FILES_INSTALL_DIR})

install(EXPORT praftTarget 
        DESTINATION ${PLAINRAFT_CMAKE_CONFIG_INSTALL_DIR}
		FILE plainraft-config.cmake)
