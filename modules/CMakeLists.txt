# dEQP Modules
add_subdirectory(glshared)
add_subdirectory(gles2)

if (GLES3)
	add_subdirectory(gles3)
endif ()

if (NOT EMSCRIPTEN)
	add_subdirectory(gles31)
endif ()

if (EGL)
	add_subdirectory(egl)
endif ()

# Misc
add_subdirectory(internal)

# Pass DEQP_MODULE_LIBRARIES and DEQP_MODULE_ENTRY_POINTS
set(DEQP_MODULE_LIBRARIES ${DEQP_MODULE_LIBRARIES} PARENT_SCOPE)
set(DEQP_MODULE_ENTRY_POINTS ${DEQP_MODULE_ENTRY_POINTS} PARENT_SCOPE)
