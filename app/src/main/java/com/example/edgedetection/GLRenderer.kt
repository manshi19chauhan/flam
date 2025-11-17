
package com.example.edgedetection

import android.content.Context
import android.graphics.SurfaceTexture
import android.util.AttributeSet
import android.view.SurfaceHolder
import android.view.TextureView
import androidx.camera.core.Preview
import androidx.camera.core.SurfaceProvider
import java.util.concurrent.Executor
import java.util.concurrent.Executors

class GLRenderer @JvmOverloads constructor(
    context: Context,
    attrs: AttributeSet? = null,
    defStyleAttr: Int = 0
) : TextureView(context, attrs, defStyleAttr), TextureView.SurfaceTextureListener {
    
    private var nativeRenderer: Long = 0
    private val renderExecutor: Executor = Executors.newSingleThreadExecutor()
    
    init {
        surfaceTextureListener = this
        System.loadLibrary("edgedetection")
    }
    
    // Native methods
    external fun nativeCreateRenderer(): Long
    external fun nativeRenderFrame(rendererPtr: Long, width: Int, height: Int, data: ByteArray?)
    external fun nativeCleanup(rendererPtr: Long)
    
    fun renderFrame(width: Int, height: Int, data: ByteArray?, processingTime: Long) {
        if (nativeRenderer != 0L) {
            nativeRenderFrame(nativeRenderer, width, height, data)
        }
    }
    
    fun getSurfaceProvider(): SurfaceProvider {
        return SurfaceProvider { request ->
            val surfaceTexture = surfaceTexture
            surfaceTexture?.setDefaultBufferSize(request.resolution.width, request.resolution.height)
            val surface = android.view.Surface(surfaceTexture)
            request.provideSurface(surface, renderExecutor) { }
        }
    }
    
    // SurfaceTextureListener callbacks
    override fun onSurfaceTextureAvailable(surface: SurfaceTexture, width: Int, height: Int) {
        nativeRenderer = nativeCreateRenderer()
    }
    
    override fun onSurfaceTextureSizeChanged(surface: SurfaceTexture, width: Int, height: Int) {
        // Handle surface size changes
    }
    
    override fun onSurfaceTextureDestroyed(surface: SurfaceTexture): Boolean {
        if (nativeRenderer != 0L) {
            nativeCleanup(nativeRenderer)
            nativeRenderer = 0
        }
        return true
    }
    
    override fun onSurfaceTextureUpdated(surface: SurfaceTexture) {
        // Handle surface updates
    }
}