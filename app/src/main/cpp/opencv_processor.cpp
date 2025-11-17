
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>

#define LOG_TAG "OpenCVProcessor"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

using namespace cv;

extern "C" {

JNIEXPORT void JNICALL
Java_com_example_edgedetection_OpenCVProcessor_nativeProcessFrame(
        JNIEnv *env,
        jobject thiz,
        jlong input_mat_addr,
        jlong output_mat_addr,
        jint mode) {
    
    try {
        Mat* input = (Mat*)input_mat_addr;
        Mat* output = (Mat*)output_mat_addr;
        
        if (input == nullptr || output == nullptr) {
            LOGE("Invalid matrix pointers");
            return;
        }
        
        if (input->empty()) {
            LOGE("Input matrix is empty");
            return;
        }
        
        switch (mode) {
            case 0: // Original
                input->copyTo(*output);
                break;
                
            case 1: // Grayscale
                if (input->channels() == 4) {
                    cvtColor(*input, *output, COLOR_RGBA2GRAY);
                    cvtColor(*output, *output, COLOR_GRAY2RGBA);
                } else if (input->channels() == 3) {
                    cvtColor(*input, *output, COLOR_RGB2GRAY);
                    cvtColor(*output, *output, COLOR_GRAY2RGBA);
                } else {
                    input->copyTo(*output);
                }
                break;
                
            case 2: // Canny Edge Detection
                {
                    Mat gray, edges;
                    if (input->channels() == 4) {
                        cvtColor(*input, gray, COLOR_RGBA2GRAY);
                    } else if (input->channels() == 3) {
                        cvtColor(*input, gray, COLOR_RGB2GRAY);
                    } else {
                        gray = *input;
                    }
                    
                    // Apply Gaussian blur to reduce noise
                    GaussianBlur(gray, gray, Size(5, 5), 1.5, 1.5);
                    
                    // Canny edge detection
                    Canny(gray, edges, 50, 150);
                    
                    // Convert back to RGBA
                    cvtColor(edges, *output, COLOR_GRAY2RGBA);
                }
                break;
                
            default:
                input->copyTo(*output);
                break;
        }
        
    } catch (const cv::Exception& e) {
        LOGE("OpenCV exception: %s", e.what());
    } catch (const std::exception& e) {
        LOGE("Standard exception: %s", e.what());
    }
}

JNIEXPORT jlong JNICALL
Java_com_example_edgedetection_OpenCVProcessor_nativeCreateMat(
        JNIEnv *env,
        jobject thiz,
        jint width,
        jint height) {
    try {
        Mat* mat = new Mat(height, width, CV_8UC4);
        return (jlong)mat;
    } catch (const std::exception& e) {
        LOGE("Failed to create Mat: %s", e.what());
        return 0;
    }
}

JNIEXPORT void JNICALL
Java_com_example_edgedetection_OpenCVProcessor_nativeReleaseMat(
        JNIEnv *env,
        jobject thiz,
        jlong mat_addr) {
    if (mat_addr != 0) {
        Mat* mat = (Mat*)mat_addr;
        delete mat;
    }
}

JNIEXPORT void JNICALL
Java_com_example_edgedetection_OpenCVProcessor_nativeBitmapToMat(
        JNIEnv *env,
        jobject thiz,
        jobject bitmap,
        jlong mat_addr) {
    
    AndroidBitmapInfo info;
    void* pixels = nullptr;
    
    try {
        // Get bitmap info
        if (AndroidBitmap_getInfo(env, bitmap, &info) != ANDROID_BITMAP_RESULT_SUCCESS) {
            LOGE("Failed to get bitmap info");
            return;
        }
        
        // Lock pixels
        if (AndroidBitmap_lockPixels(env, bitmap, &pixels) != ANDROID_BITMAP_RESULT_SUCCESS) {
            LOGE("Failed to lock bitmap pixels");
            return;
        }
        
        Mat* mat = (Mat*)mat_addr;
        if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
            *mat = Mat(info.height, info.width, CV_8UC4, pixels);
        } else if (info.format == ANDROID_BITMAP_FORMAT_RGB_565) {
            Mat temp(info.height, info.width, CV_8UC2, pixels);
            cvtColor(temp, *mat, COLOR_BGR5652RGBA);
        }
        
        AndroidBitmap_unlockPixels(env, bitmap);
        
    } catch (const std::exception& e) {
        LOGE("Bitmap to Mat conversion failed: %s", e.what());
        if (pixels != nullptr) {
            AndroidBitmap_unlockPixels(env, bitmap);
        }
    }
}

}