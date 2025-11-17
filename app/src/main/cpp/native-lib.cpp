
#include <jni.h>
#include <string>
#include <android/log.h>

#define LOG_TAG "EdgeDetection"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C" {

JNIEXPORT jstring JNICALL
Java_com_example_edgedetection_OpenCVProcessor_nativeVersion(
        JNIEnv *env,
        jobject thiz) {
    std::string version = "OpenCV Native Module v1.0";
    return env->NewStringUTF(version.c_str());
}

JNIEXPORT void JNICALL
Java_com_example_edgedetection_OpenCVProcessor_nativeInit(
        JNIEnv *env,
        jobject thiz) {
    LOGI("OpenCV Native module initialized");
}

}