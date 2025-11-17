
#ifndef OPENCV_PROCESSOR_H
#define OPENCV_PROCESSOR_H

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

// Function declarations
JNIEXPORT void JNICALL Java_com_example_edgedetection_OpenCVProcessor_nativeProcessFrame(
    JNIEnv*, jobject, jlong, jlong, jint);

JNIEXPORT jlong JNICALL Java_com_example_edgedetection_OpenCVProcessor_nativeCreateMat(
    JNIEnv*, jobject, jint, jint);

JNIEXPORT void JNICALL Java_com_example_edgedetection_OpenCVProcessor_nativeReleaseMat(
    JNIEnv*, jobject, jlong);

JNIEXPORT void JNICALL Java_com_example_edgedetection_OpenCVProcessor_nativeBitmapToMat(
    JNIEnv*, jobject, jobject, jlong);

#ifdef __cplusplus
}
#endif

#endif // OPENCV_PROCESSOR_H