
#include "glew.h"
#include "GLFW/glfw3.h"
#include "gl/GrGLInterface.h"
//#include "GrBackendSurface.h"
#include "GrContext.h"
#include "SkCanvas.h"
#include "SkColorSpace.h"
#include "SkSurface.h"
#include <cstdio>
#include <cstdlib>
#include <Windows.h>
#include "SkPaint.h"
#include "GrContextOptions.h"
#include "SkImageInfo.h"



//eventually, you will want color-correct spaces, and there are 5 places below (Ctrl+F "enable correct color spaces"), where you should replace/uncomment lines to enable this.
//warning: color-correct spaces don't work in VMWare, because mesa doesn't support it.


//uncomment the two lines below to enable correct color spaces

//#define GL_FRAMEBUFFER_SRGB 0x8DB9
//#define GL_SRGB8_ALPHA8 0x8C43

GrContext* sContext = nullptr;
SkSurface* sSurface = nullptr;

void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void init_skia(int w, int h) {
    GrContextOptions options;
//    options. = false;
    sContext = GrContext::MakeGL(nullptr, options).release();

    GrGLFramebufferInfo framebufferInfo;
    framebufferInfo.fFBOID = 0; // assume default framebuffer
    // We are always using OpenGL and we use RGBA8 internal format for both RGBA and BGRA configs in OpenGL.
    //(replace line below with this one to enable correct color spaces) framebufferInfo.fFormat = GL_SRGB8_ALPHA8;
    framebufferInfo.fFormat = GL_RGBA8;

    SkColorType colorType;
    if (kRGBA_8888_GrPixelConfig == kSkia8888_GrPixelConfig) {
        colorType = kRGBA_8888_SkColorType;
    }
    else {
        colorType = kBGRA_8888_SkColorType;
    }
    GrBackendRenderTarget backendRenderTarget(w, h,
                                              0, // sample count
                                              0, // stencil bits
                                              framebufferInfo);

    //(replace line below with this one to enable correct color spaces) sSurface = SkSurface::MakeFromBackendRenderTarget(sContext, backendRenderTarget, kBottomLeft_GrSurfaceOrigin, colorType, SkColorSpace::MakeSRGB(), nullptr).release();
    sSurface = SkSurface::MakeFromBackendRenderTarget(sContext, backendRenderTarget, kBottomLeft_GrSurfaceOrigin, colorType, nullptr, nullptr).release();
    if (sSurface == nullptr) abort();
}

void cleanup_skia() {
    delete sSurface;
    delete sContext;
}

const int kWidth = 960;
const int kHeight = 640;

int main(void) {
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //(uncomment to enable correct color spaces) glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);
    glfwWindowHint(GLFW_STENCIL_BITS, 0);
    //glfwWindowHint(GLFW_ALPHA_BITS, 0);
    glfwWindowHint(GLFW_DEPTH_BITS, 0);

    window = glfwCreateWindow(kWidth, kHeight, "Simple example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    //(uncomment to enable correct color spaces) glEnable(GL_FRAMEBUFFER_SRGB);

    init_skia(kWidth, kHeight);

    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);

    // Draw to the surface via its SkCanvas.
    SkCanvas* canvas = sSurface->getCanvas(); // We don't manage this pointer's lifetime.

    while (!glfwWindowShouldClose(window)) {
        glfwWaitEvents();
        SkPaint paint;
        paint.setColor(SK_ColorWHITE);
        canvas->drawPaint(paint);
        paint.setColor(SK_ColorBLUE);
        canvas->drawRect({100, 200, 300, 500}, paint);
        sContext->flush();

        glfwSwapBuffers(window);
    }

    cleanup_skia();

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}