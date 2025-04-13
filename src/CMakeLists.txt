

add_executable(${PROJECT_NAME}
    hello_freertos.c
)

# target_include_directories(${TARGET_NAME} PRIVATE
#     ${CMAKE_CURRENT_LIST_DIR}/../FreeRTOS-Config
# )
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    pico_async_context_freertos
    FreeRTOS-Kernel-Heap4
    pico_stdlib
)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(${PROJECT_NAME})