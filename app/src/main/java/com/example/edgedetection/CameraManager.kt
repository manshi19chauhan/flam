
package com.example.edgedetection

import android.content.Context
import android.util.Log
import android.widget.TextView
import androidx.camera.core.CameraSelector
import androidx.camera.core.ImageAnalysis
import androidx.camera.core.Preview
import androidx.camera.lifecycle.ProcessCameraProvider
import androidx.core.content.ContextCompat
import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors

class CameraManager(
    private val context: Context,
    private val glRenderer: GLRenderer,
    private val fpsCounter: TextView
) {
    
    private lateinit var cameraExecutor: ExecutorService
    private var cameraProvider: ProcessCameraProvider? = null
    private var frameAnalyzer: FrameAnalyzer? = null
    private var processingMode = 0
    
    private val opencvProcessor = OpenCVProcessor()
    
    fun startCamera() {
        val cameraProviderFuture = ProcessCameraProvider.getInstance(context)
        
        cameraProviderFuture.addListener({
            cameraProvider = cameraProviderFuture.get()
            
            val preview = Preview.Builder().build().also {
                it.setSurfaceProvider(glRenderer.getSurfaceProvider())
            }
            
            val imageAnalysis = ImageAnalysis.Builder()
                .setBackpressureStrategy(ImageAnalysis.STRATEGY_KEEP_ONLY_LATEST)
                .build()
            
            frameAnalyzer = FrameAnalyzer(opencvProcessor, glRenderer, processingMode, fpsCounter)
            imageAnalysis.setAnalyzer(getExecutor(), frameAnalyzer!!)
            
            val cameraSelector = CameraSelector.DEFAULT_BACK_CAMERA
            
            try {
                cameraProvider?.unbindAll()
                cameraProvider?.bindToLifecycle(
                    context as androidx.lifecycle.LifecycleOwner,
                    cameraSelector,
                    preview,
                    imageAnalysis
                )
            } catch (exc: Exception) {
                Log.e("CameraManager", "Use case binding failed", exc)
            }
        }, ContextCompat.getMainExecutor(context))
        
        cameraExecutor = Executors.newSingleThreadExecutor()
    }
    
    fun setProcessingMode(mode: Int) {
        processingMode = mode
        frameAnalyzer?.updateProcessingMode(mode)
    }
    
    fun release() {
        cameraExecutor.shutdown()
        opencvProcessor.release()
        cameraProvider?.unbindAll()
    }
    
    private fun getExecutor(): ExecutorService {
        if (!::cameraExecutor.isInitialized) {
            cameraExecutor = Executors.newSingleThreadExecutor()
        }
        return cameraExecutor
    }
}