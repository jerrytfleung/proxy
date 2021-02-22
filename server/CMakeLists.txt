# Proto file
get_filename_component(hw_proto "../protos/proxy.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/proxy.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/proxy.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/proxy.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/proxy.grpc.pb.h")
set(hw_grpc_mock_hdrs "${CMAKE_CURRENT_BINARY_DIR}/proxy_mock.grpc.pb.h")

add_custom_command(
        OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_mock_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out=generate_mock_code=true:"${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
        DEPENDS "${hw_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

add_executable(server
        main.cc
        observer.h
        subject.h
        generator.h
        server.cc server.h
        call_data.cc call_data.h
        systemtime_generator.cc systemtime_generator.h
        upstream.cc upstream.h
        ${hw_proto_srcs}
        ${hw_grpc_srcs})

target_link_libraries(server
        ${_GRPC_GRPCPP_UNSECURE}
        ${_PROTOBUF_LIBPROTOBUF})