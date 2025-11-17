
#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

// Function declarations
JNIEXPORT jlong JNICALL Java_com_example_edgedetection_GLRenderer_nativeCreateRenderer(
    JNIEnv*, jobject);

JNIEXPORT void JNICALL Java_com_example_edgedetection_GLRenderer_nativeRenderFrame(
    JNIEnv*, jobject, jlong, jint, jint, jbyteArray);

JNIEXPORT void JNICALL Java_com_example_edgedetection_GLRenderer_nativeCleanup(
    JNIEnv*, jobject, jlong);

#ifdef __cplusplus
}
#endif

#endif // OPENGL_RENDERER_H