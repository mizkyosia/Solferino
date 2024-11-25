/**
 * Application.cpp
 * Contributors:
 *            * Arthur Sonzogni (author)
 * Licence:
 *            * MIT
 */

#ifndef OPENGL_CMAKE_SKELETON_APPLICATION_HPP
#define OPENGL_CMAKE_SKELETON_APPLICATION_HPP

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <string>

struct GLFWwindow;

/// Application class:
/// * init OpenGL
/// * provide:
///     * getWidth()
///     * getHeight()
///     * getFrameDeltaTime()
///     * getWindowRatio()
///     * windowDimensionChanged()
/// * let the user define the "loop" function.
class Application {
 public:
    Application();

    static Application& getInstance();

    // get the window id
    GLFWwindow* getWindow() const;

    // window control
    void exit();

    // delta time between frame and time from beginning
    float getFrameDeltaTime() const;
    float getTime() const;

    // application run
    void run();

    // Application informations
    //
    int getWidth();
    int getHeight();
    float getWindowRatio();
    bool windowDimensionChanged();

 private:
    enum State { stateReady, stateRun, stateExit };

    State state;

    Application& operator=(const Application&) { return *this; }

    // Time:
    float time;
    float deltaTime;

    // Dimensions:
    int width;
    int height;
    bool dimensionChanged;
    void detectWindowDimensionChange();

 protected:
    Application(const Application&){};

    GLFWwindow* window;

    std::string title;

    virtual void loop();
};

#endif /* end of include guard: OPENGL_CMAKE_SKELETON_APPLICATION_HPP */
