
#include <jni.h>
#include <android/log.h>
#include <string>

#define LOG_TAG "NativeHelper"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_example_edgedetection_OpenCVProcessor_nativeGetCurrentTime(
        JNIEnv* env,
        jobject thiz) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000LL) + (ts.tv_nsec / 1000000LL);
}

JNIEXPORT jstring JNICALL
Java_com_example_edgedetection_OpenCVProcessor_nativeGetPlatformInfo(
        JNIEnv* env,
        jobject thiz) {
    std::string info;
    
#if defined(__arm__)
    info = "Platform: ARM";
#elif defined(__aarch64__)
    info = "Platform: ARM64";
#elif defined(__i386__)
    info = "Platform: x86";
#elif defined(__x86_64__)
    info = "Platform: x86_64";
#else
    info = "Platform: Unknown";
#endif
    
    info += "\nCPU Cores: ";
    info += std::to_string(sysconf(_SC_NPROCESSORS_CONF));
    
    return env->NewStringUTF(info.c_str());
}

}