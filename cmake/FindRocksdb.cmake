# Find Rocksdb
#
# Rocksdb_LIBRARIES
# Rocksdb_FOUND

find_path(Rocksdb_INCLUDE_DIR 
	NAMES rocksdb/db.h 
	PATHS
)

find_library(Rocksdb_LIBRARY 
	NAMES rocksdb
	PATHS /lib /usr/lib /usr/local/lib /opt/local/lib
	NO_DEFAULT_PATH
)

if (Rocksdb_LIBRARY AND Rocksdb_INCLUDE_DIR)
	set(Rocksdb_FOUND TRUE)
	set(Rocksdb_LIBRARIES ${Rocksdb_LIBRARY})
else()
	set(Rocksdb_FOUND FALSE)
	set(Rocksdb_LIBRARIES)
endif()

if (Rocksdb_FOUND)
	message(STATUS "Found Rocksdb lib: ${Rocksdb_LIBRARY}")
	message(STATUS "Found Rocksdb header file: ${Rocksdb_INCLUDE_DIR}")
else()
	message(FATAL_ERROR "Not Found Rocksdb")
endif()

