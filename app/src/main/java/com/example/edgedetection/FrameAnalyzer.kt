
package com.example.edgedetection

import android.graphics.Bitmap
import android.graphics.ImageFormat
import androidx.camera.core.ImageAnalysis
import androidx.camera.core.ImageProxy
import android.widget.TextView
import java.nio.ByteBuffer

class FrameAnalyzer(
    private val opencvProcessor: OpenCVProcessor,
    private val glRenderer: GLRenderer,
    private var processingMode: Int,
    private val fpsCounter: TextView
) : ImageAnalysis.Analyzer {
    
    private var frameCount = 0
    private var lastFpsTime = System.currentTimeMillis()
    private var startProcessingTime = 0L
    
    override fun analyze(image: ImageProxy) {
        startProcessingTime = System.currentTimeMillis()
        
        when (image.format) {
            ImageFormat.YUV_420_888 -> processYUVImage(image)
            ImageFormat.NV21 -> processNV21Image(image)
            else -> {
                // Unsupported format
                image.close()
                return
            }
        }
        
        updateFpsCounter()
        image.close()
    }
    
    private fun processYUVImage(image: ImageProxy) {
        val bitmap = yuvToBitmap(image)
        bitmap?.let {
            val processedData = opencvProcessor.processFrame(bitmap, processingMode)
            val processingTime = System.currentTimeMillis() - startProcessingTime
            glRenderer.renderFrame(image.width, image.height, processedData, processingTime)
            bitmap.recycle()
        }
    }
    
    private fun processNV21Image(image: ImageProxy) {
        // Handle NV21 format (common on older devices)
        val bitmap = nv21ToBitmap(image)
        bitmap?.let {
            val processedData = opencvProcessor.processFrame(bitmap, processingMode)
            val processingTime = System.currentTimeMillis() - startProcessingTime
            glRenderer.renderFrame(image.width, image.height, processedData, processingTime)
            bitmap.recycle()
        }
    }
    
    private fun yuvToBitmap(image: ImageProxy): Bitmap? {
        val planes = image.planes
        val yPlane = planes[0].buffer
        val uvPlane = planes[1].buffer
        
        val ySize = yPlane.remaining()
        val uvSize = uvPlane.remaining()
        
        val nv21 = ByteArray(ySize + uvSize)
        
        // Copy Y plane
        yPlane.get(nv21, 0, ySize)
        
        // Copy UV plane
        uvPlane.get(nv21, ySize, uvSize)
        
        return nv21ToBitmap(nv21, image.width, image.height)
    }
    
    private fun nv21ToBitmap(image: ImageProxy): Bitmap? {
        val buffer = image.planes[0].buffer
        val bytes = ByteArray(buffer.remaining())
        buffer.get(bytes)
        return nv21ToBitmap(bytes, image.width, image.height)
    }
    
    private fun nv21ToBitmap(nv21: ByteArray, width: Int, height: Int): Bitmap {
        val yuvImage = android.graphics.YuvImage(nv21, ImageFormat.NV21, width, height, null)
        val out = java.io.ByteArrayOutputStream()
        yuvImage.compressToJpeg(android.graphics.Rect(0, 0, width, height), 100, out)
        val imageBytes = out.toByteArray()
        return android.graphics.BitmapFactory.decodeByteArray(imageBytes, 0, imageBytes.size)
    }
    
    private fun updateFpsCounter() {
        frameCount++
        val currentTime = System.currentTimeMillis()
        if (currentTime - lastFpsTime >= 1000) {
            val fps = (frameCount * 1000 / (currentTime - lastFpsTime)).toFloat()
            fpsCounter.post {
                fpsCounter.text = "FPS: ${"%.1f".format(fps)} | Mode: ${
                    when(processingMode) {
                        0 -> "Original"
                        1 -> "Grayscale"
                        2 -> "Canny"
                        else -> "Unknown"
                    }
                }"
            }
            frameCount = 0
            lastFpsTime = currentTime
        }
    }
    
    fun updateProcessingMode(mode: Int) {
        processingMode = mode
    }
}