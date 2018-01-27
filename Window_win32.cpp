#include "Window.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <memory>

#include <GL\gl3w.h>

namespace GGJ
{
    static LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
    static LRESULT CALLBACK WndProc       (HWND, UINT, WPARAM, LPARAM);

    const char* (WINAPI *wglGetExtensionsStringARB) (HDC);
    BOOL        (WINAPI *wglSwapIntervalEXT)        (int);

    struct window_state_t
    {
        HINSTANCE hInst;
        LPCTSTR   appName;
        HWND      hWnd;
        HDC       hDC;
        HGLRC     hGlrc;
    };

    Window* Window::InitWindow(int width, int height) noexcept
    {
        static Window window{};

        //Check that we have not already initialised the window
        if (window.data != nullptr) return &window;

        //Allocate space for the data
        window.data = new window_state_t{};

        //Get the application instance
        window.data->hInst = GetModuleHandle(NULL);

        //Store the name of the application
        window.data->appName = TEXT("GGJ HAMAR 2018");

        //Setup the window class
        WNDCLASSEX wc;
        wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.lpfnWndProc   = WndProc;
        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = 0;
        wc.hInstance     = window.data->hInst;
        wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
        wc.hIconSm       = wc.hIcon;
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wc.lpszMenuName  = NULL;
        wc.lpszClassName = window.data->appName;
        wc.cbSize        = sizeof(WNDCLASSEX);

        //Register the window class
        RegisterClassEx(&wc);

        //Get display settings
        DEVMODE displaySettings;
        std::memset(&displaySettings, 0, sizeof(displaySettings));
        displaySettings.dmSize = sizeof(displaySettings);
        EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &displaySettings);

        //Store the refresh rate
        window.refreshRate = displaySettings.dmDisplayFrequency;

        //If 0 got passed in for the dimensions just scale to match the display
        if (width  == 0) width  = displaySettings.dmPelsWidth;
        if (height == 0) height = displaySettings.dmPelsHeight;

        //Get offset to the middle of the screen
        int posX = (displaySettings.dmPelsWidth  -  width) / 2;
        int posY = (displaySettings.dmPelsHeight - height) / 2;

        //Create the window
        window.data->hWnd = CreateWindowEx
        (
            WS_EX_APPWINDOW, window.data->appName, window.data->appName,
            WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_CAPTION | WS_SYSMENU,
            posX, posY, width, height, NULL, NULL,
            window.data->hInst, NULL
        );

        //Return the constructed window
        return &window;
    }

    bool Window::InitGL() noexcept
    {
        //Get the device context
        this->data->hDC = GetDC(this->data->hWnd);
        if (this->data->hDC == nullptr) return false;

        //Setup the pixel format
        PIXELFORMATDESCRIPTOR pfd =
        {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            32,
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            32,
            0,
            0,
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };

        //Get the best match
        int pixelFormat = ChoosePixelFormat(this->data->hDC, &pfd);
        if (pixelFormat == 0)                                            return false;

        //Apply the pixel format
        if (SetPixelFormat(this->data->hDC, pixelFormat, &pfd) == FALSE) return false;

        //Create rendering context
        this->data->hGlrc = wglCreateContext(this->data->hDC);
        if (this->data->hGlrc == nullptr)                                return false;

        //Make it current
        if (wglMakeCurrent(this->data->hDC, this->data->hGlrc) == FALSE) return false;

        //Load OpenGL functions
        if (gl3wInit() == -1)                                            return false;

        //Check that minimum OpenGL 3.2 is supported
        if (gl3wIsSupported(3, 2) == 0)                                  return false;

        //Load OpenGL extensions
        wglGetExtensionsStringARB = reinterpret_cast<decltype(wglGetExtensionsStringARB)>(wglGetProcAddress("wglGetExtensionsStringARB"));
        if (wglGetExtensionsStringARB == nullptr)                        return false;

        wglSwapIntervalEXT = reinterpret_cast<decltype(wglSwapIntervalEXT)>(wglGetProcAddress("wglSwapIntervalEXT"));
        if (wglSwapIntervalEXT == nullptr)                               return false;

        //Enable/Disable V-Sync (1 is off, 0 is on)
        wglSwapIntervalEXT(0);

        //Bind a VAO
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        //Get viewport
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        //Store the viewport size
        this->width  = viewport[2];
        this->height = viewport[3];

        //Set clear color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        //Set blending mode
        glEnable   (GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //Disable stuff we don't want
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DITHER);

        //Return success
        return true;
    }

    bool Window::Poll() noexcept
    {
        //Setup quit flag
        bool quit = false;

        //Setup message structure
        MSG msg;
        ZeroMemory(&msg, sizeof(MSG));

        //Handle window messages
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            //Look for quit message
            if (msg.message == WM_QUIT) quit = true;

            //Process message
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        //Return quit flag
        return quit;
    }

    void Window::Show() noexcept
    {
        ShowWindow         (this->data->hWnd, SW_SHOW);
        SetForegroundWindow(this->data->hWnd);
        SetFocus           (this->data->hWnd);
    }

    void Window::Present() noexcept
    {
        //Present our buffer to the screen
        wglSwapLayerBuffers(this->data->hDC, WGL_SWAP_MAIN_PLANE);

        //Synchronise with GPU
        glFinish();
    }

    void Window::Destroy() noexcept
    {
        //Release the window
        if (this->data->hWnd)
        {
            wglMakeCurrent(this->data->hDC, nullptr);
            wglDeleteContext(this->data->hGlrc);
            ReleaseDC(this->data->hWnd, this->data->hDC);
            DestroyWindow(this->data->hWnd);
        }

        //Release the application instance
        if (this->data->hInst)
        {
            UnregisterClass(this->data->appName, this->data->hInst);
        }

        //Clear the data structure
        delete this->data;
        this->data = nullptr;
    }

    static LRESULT CALLBACK MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        //Process message
	    switch (uMsg)
	    {
		//Check if key has been pressed
	    case WM_KEYDOWN:
            std::printf("Key pressed! %d\n", (int)wParam);//InputManager::GetInputManager()->SetState((unsigned int)wParam, true);
		    return 0;

		//Check if key has been released
	    case WM_KEYUP:
            std::printf("Key released! %d\n", (int)wParam);//InputManager::GetInputManager()->SetState((unsigned int)wParam, false);
		    return 0;

		//Send other messages to the default message handler
	    default:
		    return DefWindowProc(hWnd, uMsg, wParam, lParam);
	    }
    }

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        //Process message
        switch (uMsg)
        {
        //Check if window is being destroyed
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        //Check if window is being closed
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;

        //Send other messages to the message handler
        default:
            return MessageHandler(hWnd, uMsg, wParam, lParam);
        }
    }
};
