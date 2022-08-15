find_package(Git REQUIRED)

include(FetchContent)

#测试框架
FetchContent_Declare(
	catch2
	GIT_REPOSITORY git@gitee.com:zhzhanp/Catch2.git
	GIT_TAG v2.13.6
)
FetchContent_MakeAvailable(catch2)

# add_executable(testsync test_sync.cc)
# target_link_libraries(testsync PRIVATE synclib Catch2::Catch2)
# add_test(NAME testsynctest COMMAND testsync) # Command can be a target

add_executable(test_option test_praft_options.cc)
target_link_libraries(test_option praft)






