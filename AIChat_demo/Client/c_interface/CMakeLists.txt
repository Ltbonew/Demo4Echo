# Add the C wrapper source file
add_library(AIchat-c-interface STATIC
    AIchat_c_interface.cc  # C wrapper implementation
)
# Link the core library into the C wrapper
target_link_libraries(AIchat-c-interface PRIVATE AIChatCore)

# Add the test executable
add_executable(test_c_interface test.c)
# Link the test executable with the C wrapper and core libraries
target_link_libraries(test_c_interface PRIVATE AIchat-c-interface AIChatCore)
