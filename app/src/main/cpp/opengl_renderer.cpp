
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <jni.h>
#include <android/log.h>
#include <cstdlib>
#include <cstring>

#define LOG_TAG "OpenGLRenderer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Vertex shader source
static const char* VERTEX_SHADER_SOURCE =
    "attribute vec4 aPosition;\n"
    "attribute vec2 aTexCoord;\n"
    "varying vec2 vTexCoord;\n"
    "void main() {\n"
    "    gl_Position = aPosition;\n"
    "    vTexCoord = aTexCoord;\n"
    "}\n";

// Fragment shader source
static const char* FRAGMENT_SHADER_SOURCE =
    "precision mediump float;\n"
    "varying vec2 vTexCoord;\n"
    "uniform sampler2D uTexture;\n"
    "void main() {\n"
    "    gl_FragColor = texture2D(uTexture, vTexCoord);\n"
    "}\n";

// Vertex coordinates
static const float VERTICES[] = {
    -1.0f, -1.0f,  // bottom left
     1.0f, -1.0f,  // bottom right
    -1.0f,  1.0f,  // top left
     1.0f,  1.0f   // top right
};

// Texture coordinates
static const float TEX_COORDS[] = {
    0.0f, 1.0f,  // bottom left
    1.0f, 1.0f,  // bottom right
    0.0f, 0.0f,  // top left
    1.0f, 0.0f   // top right
};

class OpenGLRenderer {
private:
    GLuint mProgram;
    GLuint mTexture;
    GLuint mPositionHandle;
    GLuint mTexCoordHandle;
    GLuint mTextureHandle;
    bool mInitialized;
    
public:
    OpenGLRenderer() : mProgram(0), mTexture(0), mInitialized(false) {}
    
    bool initialize() {
        if (mInitialized) {
            return true;
        }
        
        // Create shader program
        mProgram = createProgram(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
        if (mProgram == 0) {
            LOGE("Failed to create shader program");
            return false;
        }
        
        // Get attribute/uniform locations
        mPositionHandle = glGetAttribLocation(mProgram, "aPosition");
        mTexCoordHandle = glGetAttribLocation(mProgram, "aTexCoord");
        mTextureHandle = glGetUniformLocation(mProgram, "uTexture");
        
        // Create texture
        glGenTextures(1, &mTexture);
        glBindTexture(GL_TEXTURE_2D, mTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        mInitialized = true;
        LOGI("OpenGL renderer initialized successfully");
        return true;
    }
    
    void render(int width, int height, const unsigned char* data) {
        if (!mInitialized) {
            LOGE("Renderer not initialized");
            return;
        }
        
        // Set viewport
        glViewport(0, 0, width, height);
        
        // Clear screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Use program
        glUseProgram(mProgram);
        
        // Update texture if data is provided
        if (data != nullptr) {
            glBindTexture(GL_TEXTURE_2D, mTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, 
                        GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        
        // Set vertex attributes
        glVertexAttribPointer(mPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, VERTICES);
        glEnableVertexAttribArray(mPositionHandle);
        
        glVertexAttribPointer(mTexCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, TEX_COORDS);
        glEnableVertexAttribArray(mTexCoordHandle);
        
        // Set texture uniform
        glUniform1i(mTextureHandle, 0);
        
        // Draw
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        // Cleanup
        glDisableVertexAttribArray(mPositionHandle);
        glDisableVertexAttribArray(mTexCoordHandle);
    }
    
    void cleanup() {
        if (mTexture != 0) {
            glDeleteTextures(1, &mTexture);
            mTexture = 0;
        }
        if (mProgram != 0) {
            glDeleteProgram(mProgram);
            mProgram = 0;
        }
        mInitialized = false;
        LOGI("OpenGL renderer cleaned up");
    }
    
private:
    GLuint loadShader(GLenum type, const char* source) {
        GLuint shader = glCreateShader(type);
        if (shader) {
            glShaderSource(shader, 1, &source, nullptr);
            glCompileShader(shader);
            
            GLint compiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
            if (!compiled) {
                GLint infoLen = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
                if (infoLen > 0) {
                    char* infoLog = (char*)malloc(infoLen);
                    glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);
                    LOGE("Shader compilation failed: %s", infoLog);
                    free(infoLog);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
        return shader;
    }
    
    GLuint createProgram(const char* vertexSource, const char* fragmentSource) {
        GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
        if (!vertexShader) {
            return 0;
        }
        
        GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
        if (!fragmentShader) {
            glDeleteShader(vertexShader);
            return 0;
        }
        
        GLuint program = glCreateProgram();
        if (program) {
            glAttachShader(program, vertexShader);
            glAttachShader(program, fragmentShader);
            glLinkProgram(program);
            
            GLint linkStatus = GL_FALSE;
            glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
            if (linkStatus != GL_TRUE) {
                GLint infoLen = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
                if (infoLen > 0) {
                    char* infoLog = (char*)malloc(infoLen);
                    glGetProgramInfoLog(program, infoLen, nullptr, infoLog);
                    LOGE("Program linking failed: %s", infoLog);
                    free(infoLog);
                }
                glDeleteProgram(program);
                program = 0;
            }
        }
        
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        return program;
    }
};

// JNI functions
extern "C" {

JNIEXPORT jlong JNICALL
Java_com_example_edgedetection_GLRenderer_nativeCreateRenderer(JNIEnv *env, jobject thiz) {
    auto* renderer = new OpenGLRenderer();
    if (renderer->initialize()) {
        return reinterpret_cast<jlong>(renderer);
    }
    delete renderer;
    return 0;
}

JNIEXPORT void JNICALL
Java_com_example_edgedetection_GLRenderer_nativeRenderFrame(
        JNIEnv *env, jobject thiz, jlong renderer_ptr, 
        jint width, jint height, jbyteArray data) {
        
    auto* renderer = reinterpret_cast<OpenGLRenderer*>(renderer_ptr);
    if (!renderer) {
        LOGE("Renderer pointer is null");
        return;
    }
    
    jbyte* data_ptr = nullptr;
    if (data != nullptr) {
        data_ptr = env->GetByteArrayElements(data, nullptr);
        if (data_ptr == nullptr) {
            LOGE("Failed to get byte array elements");
            return;
        }
    }
    
    renderer->render(width, height, reinterpret_cast<unsigned char*>(data_ptr));
    
    if (data_ptr != nullptr) {
        env->ReleaseByteArrayElements(data, data_ptr, JNI_ABORT);
    }
}

JNIEXPORT void JNICALL
Java_com_example_edgedetection_GLRenderer_nativeCleanup(
        JNIEnv *env, jobject thiz, jlong renderer_ptr) {
    auto* renderer = reinterpret_cast<OpenGLRenderer*>(renderer_ptr);
    if (renderer) {
        renderer->cleanup();
        delete renderer;
    }
}

}