cmake_minimum_required(VERSION 3.5)
project(canceld VERSION 1.0.0)

if(WASM_ROOT STREQUAL "" OR NOT WASM_ROOT)
    set(WASM_ROOT ${CMAKE_INSTALL_PREFIX})
endif()
list(APPEND CMAKE_MODULE_PATH ${WASM_ROOT}/lib/cmake)

include_directories(/usr/local/eosio.wasmsdk/include)
link_directories(/usr/local/eosio.wasmsdk/lib)

include(EosioWasmToolchain)

add_executable( canceld canceld.cpp )
