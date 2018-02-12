add_executable(deqp.js deqp.js.cpp ${DEQP_MODULE_ENTRY_POINTS})
target_link_libraries(deqp.js tcutil-platform ${DEQP_MODULE_LIBRARIES})
add_custom_target(deqp.js-data)
add_dependencies(deqp.js deqp.js-data)
set_target_properties(deqp.js PROPERTIES
	OUTPUT_NAME "deqp"
	SUFFIX ".js"
	LINK_FLAGS "--bind ${DEQP_EMSCRIPTEN_PRELOADS}")
