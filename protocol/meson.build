grpc_deps = []
grpc_deps += dependency('protobuf')
grpc_deps += dependency('grpc')
grpc_deps += dependency('grpc++')

grpc_protoc_prog = find_program('protoc')
grpc_cpp_plug_prog = find_program('grpc_cpp_plugin')

grpc_pb_gen = generator(
    grpc_protoc_prog,
    output : ['@BASENAME@.pb.cc', '@BASENAME@.pb.h'],
    arguments : ['--proto_path=@CURRENT_SOURCE_DIR@', '--cpp_out=@BUILD_DIR@', '@INPUT@']
)

grpc_grpc_gen = generator(
    grpc_protoc_prog,
    output : ['@BASENAME@.grpc.pb.cc', '@BASENAME@.grpc.pb.h'],
    arguments : ['--proto_path=@CURRENT_SOURCE_DIR@', '--grpc_out=@BUILD_DIR@', '--plugin=protoc-gen-grpc='+grpc_cpp_plug_prog.path(), '@INPUT@']
)

grpc_pb_src = grpc_pb_gen.process(files('aurum.proto'))
grpc_src = grpc_grpc_gen.process(files('aurum.proto'))

ui_protocol_bin = executable('ui_protocol_bin',
    files('empty.cpp'), grpc_pb_src, grpc_src,
    dependencies: [grpc_deps]
)
