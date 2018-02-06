add_executable(deqp.js deqp.js.cpp ${DEQP_MODULE_ENTRY_POINTS})
target_link_libraries(deqp.js tcutil-platform ${DEQP_MODULE_LIBRARIES})
add_custom_target(deqp.js-data)
add_dependencies(deqp.js deqp.js-data)
set_target_properties(deqp.js PROPERTIES
	OUTPUT_NAME "deqp"
	SUFFIX ".js"
	LINK_FLAGS "--bind")

# Adds a new LINK_FLAGS directive to the given target, while keeping the previous linker flags.
macro (append_linker_flag MODULE_NAME FLAG)
	get_property(link_flags TARGET ${MODULE_NAME} PROPERTY LINK_FLAGS)
	set(link_flags "${link_flags} ${FLAG}")
	set_target_properties(${MODULE_NAME} PROPERTIES LINK_FLAGS "${link_flags}")
endmacro (append_linker_flag)

macro (deqpjs_add_data_dir MODULE_NAME SRC_DIR DST_DIR)
	append_linker_flag(${MODULE_NAME} "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}@./${DST_DIR}")
endmacro (deqpjs_add_data_dir)

# TODO(kainino@chromium.org): These should happen automatically in add_data_dir somehow.
# Similar to how DEQP_MODULE_ENTRY_POINTS or DEQP_MODULE_LIBRARIES works, probably.
deqpjs_add_data_dir(deqp.js ../data/gles2/data gles2/data)
deqpjs_add_data_dir(deqp.js ../data/gles2/shaders gles2/shaders)
deqpjs_add_data_dir(deqp.js ../data/gles3/data gles3/data)
deqpjs_add_data_dir(deqp.js ../data/gles3/shaders gles3/shaders)
