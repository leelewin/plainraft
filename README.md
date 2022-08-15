### 🌱项目简介：
praft是raft共识算法的C++实现，基于grpc和rocksdb，可以用来构建分布式存储系统、分布式元数据服务等。

### 😀 项目依赖：
- grpc
- protobuff
- rocksdb
- cmake

### 🔨编译与使用
- compile with cmake
`$ mkdir build && cd build && cmake .. && make`
`$ make install`

- play praft with examples

### 🧾文档
- docs 

### 📆TODO
- 实现集群成员关系变更、日志压缩特性
- 统一客户端访问方式（rpc和http）
