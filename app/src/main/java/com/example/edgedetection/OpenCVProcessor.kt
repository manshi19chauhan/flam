
package com.example.edgedetection

import android.graphics.Bitmap

class OpenCVProcessor {
    
    init {
        System.loadLibrary("edgedetection")
    }
    
    // Native methods
    external fun nativeVersion(): String
    external fun nativeInit()
    external fun nativeProcessFrame(inputMat: Long, outputMat: Long, mode: Int)
    external fun nativeCreateMat(width: Int, height: Int): Long
    external fun nativeReleaseMat(matAddr: Long)
    external fun nativeBitmapToMat(bitmap: Bitmap, matAddr: Long)
    
    private var inputMat: Long = 0
    private var outputMat: Long = 0
    private var isInitialized = false
    
    fun init(): Boolean {
        if (!isInitialized) {
            nativeInit()
            isInitialized = true
        }
        return isInitialized
    }
    
    fun processFrame(bitmap: Bitmap, mode: Int): ByteArray? {
        if (!isInitialized) return null
        
        // Create Mats if needed
        if (inputMat == 0L) {
            inputMat = nativeCreateMat(bitmap.width, bitmap.height)
            outputMat = nativeCreateMat(bitmap.width, bitmap.height)
        }
        
        // Convert bitmap to Mat
        nativeBitmapToMat(bitmap, inputMat)
        
        // Process frame
        nativeProcessFrame(inputMat, outputMat, mode)
        
        // Return processed data (simplified - in real implementation, you'd get data from outputMat)
        val pixels = IntArray(bitmap.width * bitmap.height)
        bitmap.getPixels(pixels, 0, bitmap.width, 0, 0, bitmap.width, bitmap.height)
        val byteArray = ByteArray(pixels.size * 4)
        
        return byteArray
    }
    
    fun release() {
        if (inputMat != 0L) nativeReleaseMat(inputMat)
        if (outputMat != 0L) nativeReleaseMat(outputMat)
        inputMat = 0
        outputMat = 0
        isInitialized = false
    }
}