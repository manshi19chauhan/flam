
#include <jni.h>
#include <android/log.h>

#define LOG_TAG "JNIBridge"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

extern "C" {

// Helper function to throw Java exceptions from native code
void throwJavaException(JNIEnv* env, const char* className, const char* message) {
    jclass exClass = env->FindClass(className);
    if (exClass != nullptr) {
        env->ThrowNew(exClass, message);
    }
}

JNIEXPORT jstring JNICALL
Java_com_example_edgedetection_OpenCVProcessor_nativeGetLibraryInfo(
        JNIEnv* env,
        jobject thiz) {
    try {
        std::string info = "Edge Detection Native Library\n";
        info += "Version: 1.0\n";
        info += "Features: OpenCV, OpenGL ES, JNI";
        return env->NewStringUTF(info.c_str());
    } catch (const std::exception& e) {
        throwJavaException(env, "java/lang/RuntimeException", e.what());
        return nullptr;
    }
}

}