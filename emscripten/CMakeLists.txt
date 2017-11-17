add_executable(deqp.js deqp.js.cpp ${DEQP_MODULE_ENTRY_POINTS})
target_link_libraries(deqp.js tcutil-platform ${DEQP_MODULE_LIBRARIES})
add_custom_target(deqp.js-data)
add_dependencies(deqp.js deqp.js-data)
add_data_dir(deqp.js ../data/gles2/data gles2/data)
add_data_dir(deqp.js ../data/gles2/shaders gles2/shaders)
set_target_properties(deqp.js PROPERTIES
	OUTPUT_NAME "deqp"
	SUFFIX ".js"
	LINK_FLAGS "--bind")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/deqp.html
	${CMAKE_CURRENT_BINARY_DIR}/deqp.html COPYONLY)
