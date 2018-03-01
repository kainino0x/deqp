# dEQP Modules
add_subdirectory(glshared)
add_subdirectory(gles2)
add_subdirectory(gles3)
if (NOT EMSCRIPTEN) # Remove this if() to enable ES31 in Emscripten
	add_subdirectory(gles31)
endif ()
if (NOT EMSCRIPTEN)
	add_subdirectory(egl)
endif ()

# Misc
add_subdirectory(internal)

# Pass DEQP_MODULE_LIBRARIES, DEQP_MODULE_ENTRY_POINTS, DEQP_EMSCRIPTEN_PRELOADS
set(DEQP_MODULE_LIBRARIES ${DEQP_MODULE_LIBRARIES} PARENT_SCOPE)
set(DEQP_MODULE_ENTRY_POINTS ${DEQP_MODULE_ENTRY_POINTS} PARENT_SCOPE)
set(DEQP_EMSCRIPTEN_PRELOADS ${DEQP_EMSCRIPTEN_PRELOADS} PARENT_SCOPE)
