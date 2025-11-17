
#include <GLES2/gl2.h>
#include <android/log.h>
#include <cstdlib>

#define LOG_TAG "TextureManager"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

class TextureManager {
private:
    GLuint mTextures[3]; // Triple buffering for performance
    int mCurrentTexture;
    int mWidth, mHeight;
    
public:
    TextureManager() : mCurrentTexture(0), mWidth(0), mHeight(0) {
        glGenTextures(3, mTextures);
        initializeTextures();
    }
    
    ~TextureManager() {
        glDeleteTextures(3, mTextures);
    }
    
    void initializeTextures() {
        for (int i = 0; i < 3; i++) {
            glBindTexture(GL_TEXTURE_2D, mTextures[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
    }
    
    GLuint getCurrentTexture() const {
        return mTextures[mCurrentTexture];
    }
    
    GLuint getNextTexture() {
        return mTextures[(mCurrentTexture + 1) % 3];
    }
    
    void updateTexture(int width, int height