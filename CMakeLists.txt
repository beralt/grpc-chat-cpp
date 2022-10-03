project(grpc-chat-cxx CXX)
cmake_minimum_required(VERSION 3.6)

# dependencies
find_package(Protobuf REQUIRED)
# find the gRPC plugin
find_program(grpc_cpp_plugin_executable grpc_cpp_plugin DOC "The gRPC C++ plugin for protoc")
mark_as_advanced(grpc_cpp_plugin_executable)
# find the gRPC library
find_library(grpc++_library_path NAMES grpc++)
mark_as_advanced(grpc++_library_path)
# find the gRPC headers
find_path(grpc_include_dir "grpcpp/grpcpp.h")
if (NOT grpc_include_dir)
	message(FATAL_ERROR "Failed to locate gRPC headers")
endif()
mark_as_advanced(grpc_include_dir)
add_library(grpc UNKNOWN IMPORTED)
set_target_properties(grpc PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${grpc_include_dir}")
set_target_properties(grpc PROPERTIES IMPORTED_LOCATION "${grpc++_library_path}")

get_filename_component(greet_proto_file "${CMAKE_CURRENT_SOURCE_DIR}/protos/greet.proto" ABSOLUTE)
get_filename_component(greet_proto_path "${greet_proto_file}" PATH)

set(greet_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/greet.pb.cc")
set(greet_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/greet.pb.h")
set(greet_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/greet.grpc.pb.cc")
set(greet_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/greet.grpc.pb.h")

add_custom_command(
	OUTPUT "${greet_proto_srcs}" "${greet_proto_hdrs}" "${greet_grpc_srcs}" "${greet_grpc_hdrs}"

	COMMAND ${Protobuf_PROTOC_EXECUTABLE}
	ARGS
		--grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
		--cpp_out  "${CMAKE_CURRENT_BINARY_DIR}"
		-I "${greet_proto_path}"
		--plugin=protoc-gen-grpc="${grpc_cpp_plugin_executable}"
		"${greet_proto_file}"
)

add_library(protos ${greet_proto_srcs} ${greet_grpc_srcs})
target_include_directories(protos PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(protos PUBLIC protobuf::libprotobuf grpc)

# setup the server executable
add_executable(server server.cpp)
target_link_libraries(server PRIVATE protos)
target_compile_options(server PRIVATE -Wall -Wextra -pedantic -Werror)
