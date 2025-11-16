# Real-Time Computer Vision Web Application

## Project Overview

A web-based computer vision demonstration application that mirrors Android NDK/OpenCV architecture but implemented for browsers using TypeScript, WebGL, and Canvas API.

**Purpose**: Technical assessment demonstrating real-time image processing, WebGL rendering, and modular TypeScript architecture.

**Current State**: Frontend complete with full processing pipeline, WebGL rendering, and comprehensive UI controls.

## Architecture

### Modular Structure

```
/client/src/lib/
  ├── camera.ts              - WebRTC camera capture (equivalent to Android Camera2 API)
  ├── imageProcessing.ts     - OpenCV-style algorithms (Canny, Sobel, Grayscale)
  └── webglRenderer.ts       - WebGL texture rendering with shaders (OpenGL ES equivalent)

/client/src/components/
  ├── StatsOverlay.tsx       - Real-time FPS and performance metrics
  ├── ControlPanel.tsx       - Processing mode and shader controls
  ├── CameraPermissionModal.tsx - Permission request UI
  └── ThemeToggle.tsx        - Dark/Light mode switcher

/client/src/pages/
  └── ComputerVision.tsx     - Main application page

/shared/
  └── schema.ts              - TypeScript interfaces and Zod schemas
```

### Data Flow

1. **Camera Capture** (camera.ts) → WebRTC MediaStream → HTMLVideoElement
2. **Frame Extraction** (imageProcessing.ts) → Canvas 2D context → ImageData
3. **Processing** (imageProcessing.ts) → Algorithm application → Processed ImageData
4. **Rendering** (webglRenderer.ts) → WebGL texture upload → Shader application → Canvas display

## Features Implemented

### Core Features (MVP)

- ✅ **Camera Feed Integration**: WebRTC camera access with permission handling
- ✅ **Real-Time Processing**:
  - Canny Edge Detection (full implementation)
  - Sobel Operator
  - Grayscale conversion
  - Threshold processing
- ✅ **WebGL Rendering**: Texture-based rendering with shader effects (15+ FPS)
- ✅ **Shader Effects**:
  - Invert
  - Sepia
  - Brightness adjustment
  - Contrast adjustment
- ✅ **Live Statistics**: FPS counter, resolution display, processing time tracking
- ✅ **Interactive Controls**:
  - Toggle between processing modes
  - Adjustable algorithm parameters
  - Play/Pause processing
  - Start/Stop camera

## Technical Implementation

### Image Processing Algorithms

**Canny Edge Detection** (Full 4-step implementation):

1. Gaussian blur for noise reduction
2. Sobel operator for gradient calculation
3. Non-maximum suppression
4. Double threshold with edge tracking by hysteresis

**Sobel Operator**: 3x3 convolution kernels for gradient magnitude and direction

**Performance**: Optimized for 15-30 FPS on standard webcams (720p)

### WebGL Shader System

- Vertex shader: Simple quad rendering with texture coordinates
- Fragment shader: Supports 5 shader effects with uniform parameters
- Texture management: Efficient texture updates per frame

## Tech Stack

- **Frontend**: React, TypeScript, Vite
- **Styling**: Tailwind CSS, shadcn/ui components
- **WebGL**: WebGL 1.0/2.0 for rendering
- **Camera**: WebRTC MediaStream API
- **State Management**: React hooks
- **Build Tool**: Vite
