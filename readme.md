
# Real-Time Edge Detection Viewer

A real-time Android application that captures camera frames, processes them using OpenCV C++ for edge detection, renders with OpenGL ES, and includes a TypeScript web viewer for debugging and visualization.

## 📱 Features Implemented

### Android App

* **Real-time Camera Feed** using CameraX API
* **OpenCV C++ Integration** via JNI for high-performance image processing
* **Multiple Processing Modes** :
* Original camera feed
* Grayscale conversion
* Canny Edge Detection
* **OpenGL ES 2.0 Rendering** for smooth real-time display
* **Performance Monitoring** with live FPS counter
* **Mode Toggle** with visual feedback

### Web Viewer

* **TypeScript-based** frame display interface
* **Real-time Statistics** overlay (FPS, resolution, processing time)
* **Interactive Mode Selection**
* **Frame Capture** functionality
* **Mock WebSocket** simulation for real-time updates

## 🏗️ Architecture

**text**

```
Camera Feed (CameraX) 
    → ImageAnalysis 
    → JNI Bridge 
    → OpenCV C++ Processing 
    → OpenGL ES Rendering 
    → Android Display
        ↓
    TypeScript Web Viewer (Debug Interface)
```

### Technology Stack

* **Android** : Kotlin, CameraX, OpenGL ES
* **Native** : C++, OpenCV, JNI
* **Graphics** : OpenGL ES 2.0, GLSL Shaders
* **Web** : TypeScript, HTML5 Canvas
* **Build** : CMake, Gradle, npm

## 📁 Project Structure

**text**

```
FLAM_ASSIGNMENT/
├── app/                          # Android Application
│   ├── src/main/
│   │   ├── java/com/example/edgedetection/
│   │   │   ├── MainActivity.kt           # UI & Permission handling
│   │   │   ├── CameraManager.kt          # Camera lifecycle management
│   │   │   ├── FrameAnalyzer.kt          # Frame processing coordinator
│   │   │   ├── OpenCVProcessor.kt        # JNI bridge for OpenCV
│   │   │   └── GLRenderer.kt             # OpenGL ES surface
│   │   ├── cpp/                      # Native C++ Code
│   │   │   ├── CMakeLists.txt            # Native build configuration
│   │   │   ├── native-lib.cpp            # Library entry point
│   │   │   ├── opencv_processor.cpp      # OpenCV image processing
│   │   │   └── opengl_renderer.cpp       # OpenGL rendering engine
│   │   └── res/                      # Android resources
│   └── build.gradle                     # Android build config
├── jni/                          # JNI Components
│   ├── include/                    # Native headers
│   │   ├── opencv_processor.h
│   │   └── opengl_renderer.h
│   ├── jni_bridge.cpp              # JNI helper functions
│   └── native_helper.cpp           # Platform utilities
├── gl/                           # OpenGL ES Components
│   ├── shaders/                   # GLSL Shaders
│   │   ├── vertex.glsl
│   │   └── fragment.glsl
│   └── texture_manager.cpp          # Texture management
├── web/                          # TypeScript Web Viewer
│   ├── src/
│   │   └── viewer.ts                  # Main web viewer
│   ├── index.html                    # Web interface
│   ├── package.json                  # TypeScript build config
│   └── tsconfig.json                 # TypeScript compiler
└── README.md                       # This file
```

## 🚀 Setup Instructions

### Prerequisites

* **Android Studio** Arctic Fox or later
* **Android NDK** 25.0 or later
* **OpenCV Android SDK** 4.5.0 or later
* **Node.js** 16.0 or later (for web viewer)
* **Android device/emulator** with API level 24+

### Android Setup

1. **Clone the repository**
   **bash**

   ```
   git clone https://github.com/your-username/EdgeDetectionViewer.git
   cd EdgeDetectionViewer
   ```
2. **Install OpenCV Android SDK**

   * Download OpenCV Android SDK from [OpenCV.org](https://opencv.org/releases/)
   * Extract to your preferred location
   * Set OpenCV path in `local.properties`:
     **properties**

     ```
     opencv.dir=/path/to/OpenCV-android-sdk
     ```
3. **Build and Run**

   * Open project in Android Studio
   * Build project (Build → Make Project)
   * Run on connected device or emulator

### Web Viewer Setup

1. **Navigate to web directory**
   **bash**

   ```
   cd web
   ```
2. **Install dependencies**
   **bash**

   ```
   npm install
   ```
3. **Build TypeScript**
   **bash**

   ```
   npm run build
   ```
4. **Start local server**
   **bash**

   ```
   npm run serve
   # or
   python3 -m http.server 8000
   ```
5. **Open in browser**
   **text**

   ```
   http://localhost:8000
   ```

## 🔧 Configuration

### OpenCV Integration

The project uses OpenCV C++ for image processing:

* **Canny Edge Detection** with Gaussian blur preprocessing
* **Grayscale conversion** with optimized color space transforms
* **Real-time processing** at 15-20 FPS

### JNI Implementation

* **Efficient data transfer** between Java and C++
* **Minimal memory copying** using direct buffer access
* **Error handling** with proper exception propagation

### OpenGL ES Rendering

* **Texture-based rendering** for processed frames
* **GLSL shaders** for potential visual effects
* **Double buffering** for smooth performance

## 🎯 Usage

### Android App

1. Grant camera permissions when prompted
2. View real-time camera feed with edge detection
3. Tap "Mode" button to cycle through:
   * **Original** : Raw camera feed
   * **Grayscale** : Monochrome conversion
   * **Canny Edge** : Real-time edge detection
4. Monitor FPS and performance stats

### Web Viewer

1. Open `http://localhost:8000`
2. View sample edge detection results
3. Change processing modes using dropdown
4. Capture frames using "Capture Frame" button
5. Monitor real-time statistics

## **Optimizations Implemented:**

* Native C++ processing for maximum performance
* Texture recycling in OpenGL
* Efficient JNI data transfer
* Frame skipping under heavy load

## 🐛 Known Issues & Limitations

1. **YUV Conversion** : Simplified YUV to RGB conversion may need optimization
2. **Memory Management** : Manual native memory management requires careful attention
3. **Device Compatibility** : Some older devices may have performance issues
4. **Web Integration** : Currently uses mock data; real WebSocket integration needed for production


## 📄 License

This project is developed as part of a technical assessment for R&D Intern position.
