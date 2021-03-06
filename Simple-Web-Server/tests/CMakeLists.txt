if(NOT MSVC)
    add_compile_options(-fno-access-control)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(-Wno-thread-safety)
    endif()
    
    if(BUILD_TESTING AND 0) # i dont care about web server thing ill test it later
        add_executable(sws_io_test io_test.cpp)
        target_link_libraries(sws_io_test simple-web-server)
        add_test(NAME sws_io_test COMMAND sws_io_test)
    
        add_executable(sws_parse_test parse_test.cpp)
        target_link_libraries(sws_parse_test simple-web-server)
        add_test(NAME sws_parse_test COMMAND sws_parse_test)
    endif()
endif()

if(OPENSSL_FOUND AND BUILD_TESTING AND 0)
    add_executable(sws_crypto_test crypto_test.cpp)
    target_link_libraries(sws_crypto_test simple-web-server)
    add_test(NAME sws_crypto_test COMMAND sws_crypto_test)
endif()

if(BUILD_TESTING AND 0)
    add_executable(status_code_test status_code_test.cpp)
    target_link_libraries(status_code_test simple-web-server)
    add_test(NAME status_code_test COMMAND status_code_test)
endif()

if(BUILD_FUZZING AND 0) # poop this too
    add_executable(percent_decode fuzzers/percent_decode.cpp)
    target_compile_options(percent_decode PRIVATE -fsanitize=address,fuzzer)
    target_link_options(percent_decode PRIVATE -fsanitize=address,fuzzer)
    target_link_libraries(percent_decode simple-web-server)
    
    add_executable(query_string_parse fuzzers/query_string_parse.cpp)
    target_compile_options(query_string_parse PRIVATE -fsanitize=address,fuzzer)
    target_link_options(query_string_parse PRIVATE -fsanitize=address,fuzzer)
    target_link_libraries(query_string_parse simple-web-server)
    
    add_executable(http_header_parse fuzzers/http_header_parse.cpp)
    target_compile_options(http_header_parse PRIVATE -fsanitize=address,fuzzer)
    target_link_options(http_header_parse PRIVATE -fsanitize=address,fuzzer)
    target_link_libraries(http_header_parse simple-web-server)
    
    add_executable(http_header_field_value_semicolon_separated_attributes_parse fuzzers/http_header_field_value_semicolon_separated_attributes_parse.cpp)
    target_compile_options(http_header_field_value_semicolon_separated_attributes_parse PRIVATE -fsanitize=address,fuzzer)
    target_link_options(http_header_field_value_semicolon_separated_attributes_parse PRIVATE -fsanitize=address,fuzzer)
    target_link_libraries(http_header_field_value_semicolon_separated_attributes_parse simple-web-server)
    
    add_executable(request_message_parse fuzzers/request_message_parse.cpp)
    target_compile_options(request_message_parse PRIVATE -fsanitize=address,fuzzer)
    target_link_options(request_message_parse PRIVATE -fsanitize=address,fuzzer)
    target_link_libraries(request_message_parse simple-web-server)
    
    add_executable(response_message_parse fuzzers/response_message_parse.cpp)
    target_compile_options(response_message_parse PRIVATE -fsanitize=address,fuzzer)
    target_link_options(response_message_parse PRIVATE -fsanitize=address,fuzzer)
    target_link_libraries(response_message_parse simple-web-server)
endif()