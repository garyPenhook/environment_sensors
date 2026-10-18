# The following variables contains the files used by the different stages of the build process.
set(pic_test_default_default_XC8_FILE_TYPE_assemble)
set_source_files_properties(${pic_test_default_default_XC8_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${pic_test_default_default_XC8_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(pic_test_default_default_XC8_FILE_TYPE_assemblePreprocess)
set_source_files_properties(${pic_test_default_default_XC8_FILE_TYPE_assemblePreprocess} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${pic_test_default_default_XC8_FILE_TYPE_assemblePreprocess})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(pic_test_default_default_XC8_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/aht30.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/i2c1.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/oled.c")
set_source_files_properties(${pic_test_default_default_XC8_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(pic_test_default_default_XC8_FILE_TYPE_link)
set(pic_test_default_image_name "default.elf")
set(pic_test_default_image_base_name "default")

# The output directory of the final image.
set(pic_test_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/pic_test")

# The full path to the final image.
set(pic_test_default_full_path_to_image ${pic_test_default_output_dir}/${pic_test_default_image_name})

# Potential output file extensions
set(output_extensions
    .hex
    .hxl
    .mum
    .o
    .sdb
    .sym
    .cmf)
list(TRANSFORM output_extensions PREPEND "${pic_test_default_output_dir}/${pic_test_default_image_base_name}")
