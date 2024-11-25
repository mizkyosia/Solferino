/**
 * Main.cpp skeleton
 * Contributors:
 *            * Arthur Sonzogni
 * Licence:
 *            * MIT
 */

#include "MyApplication.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main(int argc, const char* argv[]) {
    MyApplication app;
    app.run();
    return 0;
}
