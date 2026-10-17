include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(pic_test_default_library_list )

# Handle files with suffix (s|as|asm|AS|ASM|As|aS|Asm), for group default-XC8
if(pic_test_default_default_XC8_FILE_TYPE_assemble)
add_library(pic_test_default_default_XC8_assemble OBJECT ${pic_test_default_default_XC8_FILE_TYPE_assemble})
    pic_test_default_default_XC8_assemble_rule(pic_test_default_default_XC8_assemble)
    list(APPEND pic_test_default_library_list "$<TARGET_OBJECTS:pic_test_default_default_XC8_assemble>")

endif()

# Handle files with suffix S, for group default-XC8
if(pic_test_default_default_XC8_FILE_TYPE_assemblePreprocess)
add_library(pic_test_default_default_XC8_assemblePreprocess OBJECT ${pic_test_default_default_XC8_FILE_TYPE_assemblePreprocess})
    pic_test_default_default_XC8_assemblePreprocess_rule(pic_test_default_default_XC8_assemblePreprocess)
    list(APPEND pic_test_default_library_list "$<TARGET_OBJECTS:pic_test_default_default_XC8_assemblePreprocess>")

endif()

# Handle files with suffix [cC], for group default-XC8
if(pic_test_default_default_XC8_FILE_TYPE_compile)
add_library(pic_test_default_default_XC8_compile OBJECT ${pic_test_default_default_XC8_FILE_TYPE_compile})
    pic_test_default_default_XC8_compile_rule(pic_test_default_default_XC8_compile)
    list(APPEND pic_test_default_library_list "$<TARGET_OBJECTS:pic_test_default_default_XC8_compile>")

endif()


# Main target for this project
add_executable(pic_test_default_image_0t72d_8P ${pic_test_default_library_list})

set_target_properties(pic_test_default_image_0t72d_8P PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    ADDITIONAL_CLEAN_FILES "${output_extensions}"
    RUNTIME_OUTPUT_DIRECTORY "${pic_test_default_output_dir}")
target_link_libraries(pic_test_default_image_0t72d_8P PRIVATE ${pic_test_default_default_XC8_FILE_TYPE_link})
# Add the link options from the rule file.
pic_test_default_link_rule( pic_test_default_image_0t72d_8P)



