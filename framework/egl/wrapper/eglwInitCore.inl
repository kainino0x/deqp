/* WARNING: This is auto-generated file. Do not modify, since changes will
 * be lost! Modify the generating script instead.
 *
 * Generated from Khronos EGL API description (egl.xml) revision 13a26984991216cbf9a44fbf390f95dc05b5493a.
 */
dst->bindAPI						= (eglBindAPIFunc)							loader->get("eglBindAPI");
dst->bindTexImage					= (eglBindTexImageFunc)						loader->get("eglBindTexImage");
dst->chooseConfig					= (eglChooseConfigFunc)						loader->get("eglChooseConfig");
dst->copyBuffers					= (eglCopyBuffersFunc)						loader->get("eglCopyBuffers");
dst->createContext					= (eglCreateContextFunc)					loader->get("eglCreateContext");
dst->createPbufferFromClientBuffer	= (eglCreatePbufferFromClientBufferFunc)	loader->get("eglCreatePbufferFromClientBuffer");
dst->createPbufferSurface			= (eglCreatePbufferSurfaceFunc)				loader->get("eglCreatePbufferSurface");
dst->createPixmapSurface			= (eglCreatePixmapSurfaceFunc)				loader->get("eglCreatePixmapSurface");
dst->createWindowSurface			= (eglCreateWindowSurfaceFunc)				loader->get("eglCreateWindowSurface");
dst->destroyContext					= (eglDestroyContextFunc)					loader->get("eglDestroyContext");
dst->destroySurface					= (eglDestroySurfaceFunc)					loader->get("eglDestroySurface");
dst->getConfigAttrib				= (eglGetConfigAttribFunc)					loader->get("eglGetConfigAttrib");
dst->getConfigs						= (eglGetConfigsFunc)						loader->get("eglGetConfigs");
dst->getCurrentContext				= (eglGetCurrentContextFunc)				loader->get("eglGetCurrentContext");
dst->getCurrentDisplay				= (eglGetCurrentDisplayFunc)				loader->get("eglGetCurrentDisplay");
dst->getCurrentSurface				= (eglGetCurrentSurfaceFunc)				loader->get("eglGetCurrentSurface");
dst->getDisplay						= (eglGetDisplayFunc)						loader->get("eglGetDisplay");
dst->getError						= (eglGetErrorFunc)							loader->get("eglGetError");
dst->initialize						= (eglInitializeFunc)						loader->get("eglInitialize");
dst->makeCurrent					= (eglMakeCurrentFunc)						loader->get("eglMakeCurrent");
dst->queryAPI						= (eglQueryAPIFunc)							loader->get("eglQueryAPI");
dst->queryContext					= (eglQueryContextFunc)						loader->get("eglQueryContext");
dst->queryString					= (eglQueryStringFunc)						loader->get("eglQueryString");
dst->querySurface					= (eglQuerySurfaceFunc)						loader->get("eglQuerySurface");
dst->releaseTexImage				= (eglReleaseTexImageFunc)					loader->get("eglReleaseTexImage");
dst->releaseThread					= (eglReleaseThreadFunc)					loader->get("eglReleaseThread");
dst->surfaceAttrib					= (eglSurfaceAttribFunc)					loader->get("eglSurfaceAttrib");
dst->swapBuffers					= (eglSwapBuffersFunc)						loader->get("eglSwapBuffers");
dst->swapInterval					= (eglSwapIntervalFunc)						loader->get("eglSwapInterval");
dst->terminate						= (eglTerminateFunc)						loader->get("eglTerminate");
dst->waitClient						= (eglWaitClientFunc)						loader->get("eglWaitClient");
dst->waitGL							= (eglWaitGLFunc)							loader->get("eglWaitGL");
dst->waitNative						= (eglWaitNativeFunc)						loader->get("eglWaitNative");
#if !EMSCRIPTEN
dst->clientWaitSync					= (eglClientWaitSyncFunc)					loader->get("eglClientWaitSync");
dst->createImage					= (eglCreateImageFunc)						loader->get("eglCreateImage");
dst->createPlatformPixmapSurface	= (eglCreatePlatformPixmapSurfaceFunc)		loader->get("eglCreatePlatformPixmapSurface");
dst->createPlatformWindowSurface	= (eglCreatePlatformWindowSurfaceFunc)		loader->get("eglCreatePlatformWindowSurface");
dst->createSync						= (eglCreateSyncFunc)						loader->get("eglCreateSync");
dst->destroyImage					= (eglDestroyImageFunc)						loader->get("eglDestroyImage");
dst->destroySync					= (eglDestroySyncFunc)						loader->get("eglDestroySync");
dst->getPlatformDisplay				= (eglGetPlatformDisplayFunc)				loader->get("eglGetPlatformDisplay");
dst->getProcAddress					= (eglGetProcAddressFunc)					loader->get("eglGetProcAddress");
dst->getSyncAttrib					= (eglGetSyncAttribFunc)					loader->get("eglGetSyncAttrib");
dst->waitSync						= (eglWaitSyncFunc)							loader->get("eglWaitSync");
#endif
