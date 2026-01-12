const fs = require('fs');
const path = require('path');
const grpc=require('@grpc/grpc-js')
const protoLoader=require('@grpc/proto-loader')

const PROTO_PATH=path.join(__dirname,"proto/message.proto")
const packageDefinition=protoLoader.loadSync(PROTO_PATH,{
    keepCase:true,//保持原始字段名称的大小写格式，不转换为驼峰命名
    longs:String,//将 Protocol Buffer 中的 long 类型转换为 JavaScript 字符串，避免精度丢失问题
    enums:String,//将枚举值转换为字符串形式而不是数字形式
    defaults:true,//在生成的对象中包含默认值，即使没有显式设置也会显示默认值
    oneofs:true//启用对 oneof 字段的支持，确保正确处理 Protocol Buffer 中的 oneof 选项组
})

const protoDescriptor=grpc.loadPackageDefinition(packageDefinition)
const message_proto=protoDescriptor.message

module.exports=message_proto