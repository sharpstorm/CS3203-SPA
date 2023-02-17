## Adapted from
## https://github.com/StableCoder/cmake-scripts/blob/main/code-coverage.cmake

option(
        CODE_COVERAGE
        "Builds targets with code coverage instrumentation. (Requires GCC or Clang)"
        OFF)

# Programs
find_program(LLVM_COV_PATH llvm-cov)
find_program(LLVM_PROFDATA_PATH llvm-profdata)
find_program(LCOV_PATH lcov)
find_program(GENHTML_PATH genhtml)

# Variables
set(CMAKE_COVERAGE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/coverage)

if(CODE_COVERAGE)
    # Common Targets
    file(MAKE_DIRECTORY ${CMAKE_COVERAGE_OUTPUT_DIRECTORY})

    if(CMAKE_C_COMPILER_ID MATCHES "(Apple)?[Cc]lang"
            OR CMAKE_CXX_COMPILER_ID MATCHES "(Apple)?[Cc]lang")
        # Messages
        message(STATUS "LLVM Toolchain Detected, using llvm-cov")

        if(NOT LLVM_COV_PATH)
            message(FATAL_ERROR "llvm-cov not found! Aborting.")
        endif()

        add_custom_target(
                coverage-clean
                COMMAND ${CMAKE_COMMAND} -E rm -f
                ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/binaries.list
                COMMAND ${CMAKE_COMMAND} -E rm -f
                ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/profraw.list)

        # Used to get the shared object file list before doing the main all-
        # processing
        add_custom_target(
                coverage-libs
                COMMAND ;
                COMMENT "libs ready for coverage report.")
    else()
        message(FATAL_ERROR "LLVM Coverage only works with CLang based compilers")
    endif()
endif()

function(target_code_coverage TARGET_NAME)
    # Argument parsing
    set(multi_value_keywords EXCLUDE OBJECTS ARGS)
    cmake_parse_arguments(
            target_code_coverage "${options}" "${single_value_keywords}"
            "${multi_value_keywords}" ${ARGN})

    set(TARGET_VISIBILITY PRIVATE)
    set(TARGET_LINK_VISIBILITY PRIVATE)

    if(CODE_COVERAGE AND (CMAKE_C_COMPILER_ID MATCHES "(Apple)?[Cc]lang"
            OR CMAKE_CXX_COMPILER_ID MATCHES "(Apple)?[Cc]lang"))

        if(LLVM_COV_VERSION VERSION_GREATER_EQUAL "7.0.0")
            message(FATAL_ERROR "Requires LLVM > 7.0")
        endif()

        # Add code coverage instrumentation to the target's linker command
        target_compile_options(${TARGET_NAME} ${TARGET_VISIBILITY}
                -fprofile-instr-generate -fcoverage-mapping)
        target_link_options(${TARGET_NAME} ${TARGET_VISIBILITY}
                -fprofile-instr-generate -fcoverage-mapping)

        # Targets
        get_target_property(target_type ${TARGET_NAME} TYPE)

        # Add shared library to processing for 'all' targets
        if(target_type STREQUAL "SHARED_LIBRARY")
            if(CMAKE_C_COMPILER_ID MATCHES "(Apple)?[Cc]lang"
                    OR CMAKE_CXX_COMPILER_ID MATCHES "(Apple)?[Cc]lang")
                add_custom_target(
                        cov-run-${TARGET_NAME}
                        COMMAND
                        ${CMAKE_COMMAND} -E echo "-object=$<TARGET_FILE:${TARGET_NAME}>" >>
                        ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/binaries.list
                        DEPENDS ${TARGET_NAME})

                add_dependencies(coverage-libs
                        cov-run-${TARGET_NAME})
            endif()
        endif()

        # For executables add targets to run and produce output
        if(target_type STREQUAL "EXECUTABLE")
            if(CMAKE_C_COMPILER_ID MATCHES "(Apple)?[Cc]lang"
                    OR CMAKE_CXX_COMPILER_ID MATCHES "(Apple)?[Cc]lang")

                # If there are shared objects to also work with, generate the string to
                # add them here
                foreach(SO_TARGET ${target_code_coverage_OBJECTS})
                    # Check to see if the target is a shared object
                    if(TARGET ${SO_TARGET})
                        get_target_property(SO_TARGET_TYPE ${SO_TARGET} TYPE)
                        if(${SO_TARGET_TYPE} STREQUAL "SHARED_LIBRARY")
                            set(SO_OBJECTS ${SO_OBJECTS} -object=$<TARGET_FILE:${SO_TARGET}>)
                        endif()
                    endif()
                endforeach()

                # Run the executable, generating raw profile data Make the run data
                # available for further processing. Separated to allow Windows to run
                # this target serially.
                add_custom_target(
                        cov-run-${TARGET_NAME}
                        COMMAND
                        ${CMAKE_COMMAND} -E env
                        LLVM_PROFILE_FILE=${TARGET_NAME}.profraw
                        $<TARGET_FILE:${TARGET_NAME}> ${target_code_coverage_ARGS}
                        COMMAND
                        ${CMAKE_COMMAND} -E echo "-object=$<TARGET_FILE:${TARGET_NAME}>"
                        ${SO_OBJECTS} >> ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/binaries.list
                        COMMAND
                        ${CMAKE_COMMAND} -E echo
                        "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.profraw"
                        >> ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/profraw.list
                        DEPENDS coverage-libs ${TARGET_NAME})

                # Merge the generated profile data so llvm-cov can process it
                add_custom_target(
                        cov-processing-${TARGET_NAME}
                        COMMAND
                        ${LLVM_PROFDATA_PATH} merge -sparse
                        ${TARGET_NAME}.profraw -o
                        ${TARGET_NAME}.profdata
                        DEPENDS cov-run-${TARGET_NAME})

                foreach(EXCLUDE_ITEM ${target_code_coverage_EXCLUDE})
                    set(EXCLUDE_REGEX ${EXCLUDE_REGEX}
                            -ignore-filename-regex='${EXCLUDE_ITEM}')
                endforeach()

                # Print out details of the coverage information to the command line
                add_custom_target(
                        cov-show-${TARGET_NAME}
                        COMMAND
                        ${LLVM_COV_PATH} show $<TARGET_FILE:${TARGET_NAME}> ${SO_OBJECTS}
                        -instr-profile=${TARGET_NAME}.profdata
                        -show-line-counts-or-regions ${EXCLUDE_REGEX}
                        DEPENDS cov-processing-${TARGET_NAME})

                # Print out a summary of the coverage information to the command line
                add_custom_target(
                        cov-report-${TARGET_NAME}
                        COMMAND
                        ${LLVM_COV_PATH} report $<TARGET_FILE:${TARGET_NAME}> ${SO_OBJECTS}
                        -instr-profile=${TARGET_NAME}.profdata
                        ${EXCLUDE_REGEX}
                        DEPENDS cov-processing-${TARGET_NAME})

                # Export coverage information so continuous integration tools (e.g.
                # Jenkins) can consume it
                add_custom_target(
                        cov-export-${TARGET_NAME}
                        COMMAND
                        ${LLVM_COV_PATH} export $<TARGET_FILE:${TARGET_NAME}> ${SO_OBJECTS}
                        -instr-profile=${TARGET_NAME}.profdata
                        -format="text" ${EXCLUDE_REGEX} >
                        ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/${TARGET_NAME}.json
                        DEPENDS cov-processing-${TARGET_NAME})

                # Generates HTML output of the coverage information for perusal
                add_custom_target(
                        cov-${TARGET_NAME}
                        COMMAND
                        ${LLVM_COV_PATH} show $<TARGET_FILE:${TARGET_NAME}> ${SO_OBJECTS}
                        -instr-profile=${TARGET_NAME}.profdata
                        -show-line-counts-or-regions
                        -output-dir=${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/${TARGET_NAME}
                        -format="html" ${EXCLUDE_REGEX}
                        DEPENDS cov-processing-${TARGET_NAME})
            endif()

            add_custom_command(
                    TARGET cov-${TARGET_NAME}
                    POST_BUILD
                    COMMAND ;
                    COMMENT
                    "Open ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/${TARGET_NAME}/index.html in your browser to view the coverage report."
            )
        endif()
    endif()
endfunction()
