file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers} "src/common/Types.h" "src/pkb/queryHandlers/FollowsQueryHandler.h" "src/pkb/queryHandlers/ParentQueryHandler.h" "src/pkb/storage/FollowsStorage.h" "src/pkb/storage/RelationTable.h" "src/pkb/storage/ParentStorage.h" "src/pkb/queryHandlers/FollowsQueryHandler.cpp" "src/pkb/storage/FollowsStorage.cpp" "src/pkb/storage/ParentStorage.cpp" "src/pkb/queryHandlers/ParentQueryHandler.cpp" "src/pkb/queryHandlers/PkbQueryHandler.cpp" "src/pkb/writers/PkbWriter.h" "src/pkb/writers/FollowsWriter.h" "src/pkb/writers/ParentWriter.h" "src/pkb/writers/FollowsWriter.cpp" "src/pkb/writers/ParentWriter.cpp")
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


