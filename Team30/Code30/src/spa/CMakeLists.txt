file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/" *.cpp)
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/src/" *.h *.hpp)
add_library(spa ${srcs} ${headers} src/sp/extractor/concrete_extractors/UsesExtractor.cpp src/sp/extractor/concrete_extractors/UsesExtractor.h src/sp/extractor/concrete_extractors/ModifiesExtractor.cpp src/sp/extractor/concrete_extractors/ModifiesExtractor.h src/sp/extractor/concrete_extractors/StatementListExtractor.cpp src/sp/extractor/concrete_extractors/StatementListExtractor.h)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
