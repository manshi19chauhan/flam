
package com.example.edgedetection

import android.Manifest
import android.content.pm.PackageManager
import android.os.Bundle
import android.widget.Button
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat

class MainActivity : AppCompatActivity() {
    
    private lateinit var cameraManager: CameraManager
    private lateinit var fpsCounter: TextView
    private lateinit var toggleBtn: Button
    private var currentMode = 0 // 0: Original, 1: Grayscale, 2: Canny
    
    // Mode descriptions for UI
    private val modeDescriptions = listOf(
        "Original",
        "Grayscale", 
        "Canny Edge"
    )
    
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        
        initializeViews()
        setupCameraManager()
        setupUIListeners()
        
        // Check and request camera permissions
        if (allPermissionsGranted()) {
            startCamera()
        } else {
            ActivityCompat.requestPermissions(
                this, REQUIRED_PERMISSIONS, REQUEST_CODE_PERMISSIONS
            )
        }
    }
    
    private fun initializeViews() {
        fpsCounter = findViewById(R.id.fps_counter)
        toggleBtn = findViewById(R.id.toggle_btn)
        
        // Set initial button text
        updateToggleButtonText()
    }
    
    private fun setupCameraManager() {
        val glRenderer: GLRenderer = findViewById(R.id.gl_surface)
        cameraManager = CameraManager(this, glRenderer, fpsCounter)
    }
    
    private fun setupUIListeners() {
        toggleBtn.setOnClickListener {
            cycleProcessingMode()
        }
        
        // Optional: Add long press to reset to original mode
        toggleBtn.setOnLongClickListener {
            currentMode = 0
            updateToggleButtonText()
            cameraManager.setProcessingMode(currentMode)
            Toast.makeText(this, "Reset to Original mode", Toast.LENGTH_SHORT).show()
            true
        }
    }
    
    private fun cycleProcessingMode() {
        currentMode = (currentMode + 1) % 3
        updateToggleButtonText()
        cameraManager.setProcessingMode(currentMode)
        
        // Show mode change toast
        Toast.makeText(
            this, 
            "Mode: ${modeDescriptions[currentMode]}", 
            Toast.LENGTH_SHORT
        ).show()
    }
    
    private fun updateToggleButtonText() {
        toggleBtn.text = "Mode: ${modeDescriptions[currentMode]}"
    }
    
    private fun startCamera() {
        if (::cameraManager.isInitialized) {
            cameraManager.startCamera()
            Toast.makeText(this, "Camera started successfully", Toast.LENGTH_SHORT).show()
        }
    }
    
    private fun allPermissionsGranted(): Boolean {
        return REQUIRED_PERMISSIONS.all { permission ->
            ContextCompat.checkSelfPermission(
                baseContext, permission
            ) == PackageManager.PERMISSION_GRANTED
        }
    }
    
    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        
        if (requestCode == REQUEST_CODE_PERMISSIONS) {
            if (allPermissionsGranted()) {
                startCamera()
            } else {
                // Permission denied - show message and close app
                Toast.makeText(
                    this,
                    "Camera permission is required to use this app",
                    Toast.LENGTH_LONG
                ).show()
                finish()
            }
        }
    }
    
    override fun onResume() {
        super.onResume()
        // Restart camera if permissions are granted
        if (allPermissionsGranted() && ::cameraManager.isInitialized) {
            startCamera()
        }
    }
    
    override fun onPause() {
        super.onPause()
        // Clean up camera resources
        if (::cameraManager.isInitialized) {
            cameraManager.release()
        }
    }
    
    override fun onDestroy() {
        super.onDestroy()
        // Ensure proper cleanup
        if (::cameraManager.isInitialized) {
            cameraManager.release()
        }
    }
    
    companion object {
        private const val REQUEST_CODE_PERMISSIONS = 100
        private val REQUIRED_PERMISSIONS = arrayOf(Manifest.permission.CAMERA)
    }
}