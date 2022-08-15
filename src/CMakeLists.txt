set(base_SRCs
	base/appendfile.cc	
	base/asynclogging.cc
	base/logfile.cc
	base/logger.cc
	base/logstream.cc
	base/timestamp.cc
)

add_library(base ${base_SRCs})

set(primary_SRCs 
	praft_options.cc
	praft_impl.cc
	praft_context.cc
	praft_raft_log.cc
	praft_raft_state.cc
	praft_state_machine.cc
)

set(headers 
	${PROJECT_SOURCE_DIR}/base/appendfile.h
	${PROJECT_SOURCE_DIR}/base/asynclogging.h
	# todo

)

add_library(praft ${primary_SRCs})
target_include_directories(praft PRIVATE ${Rocksdb_INCLUDE_DIR})
target_link_libraries(praft 
	base 
	${Rocksdb_LIBRARY} -lz -lsnappy -lbz2 -llz4 -lzstd -ldl
	raft_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF}
)

install(TARGETS praft base raft_grpc_proto
	    EXPORT praftTarget
		RUNTIME DESTINATION ${PLAINRAFT_RUNTIME_INSTALL_DIR}
		ARCHIVE DESTINATION ${PLAINRAFT_ARCHIVE_INSTALL_DIR}
		#todo LIBRARY
)

install(FILES ${headers} DESTINATION ${PLAINRAFT_INC_INSTALL_DIR})

