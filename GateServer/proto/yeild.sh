protoc -I="." --grpc_out="." --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` "message.proto"
protoc --cpp_out=. "message.proto"
