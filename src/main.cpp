#include <iostream>
#include <cmath>

#include <raylib.h>
#include <raymath.h>

#include <imgui.h>
#include "rlImGui.h"

#include "pathfinding/Graph.hpp"
#include "agents/Car.hpp"
#include "ui/UI.hpp"
#include "util/Misc.hpp"

using namespace std;

float zoom = Constants::BaseZoom;

bool simPaused = false;

Model road, cars[2];

Camera3D camera{0};

/// @brief Target position for the camera, in **SPHERICAL COORDINATES**
Vector3 cameraTargetPosition(zoom, PI / 4, 0);

/// @brief Load all models needed by the program
void loadModels()
{
    double t0 = GetTime() * 1000.0;

    road = LoadModel("assets/models/road.vox");
    
    for (int i = 0; i < Constants::CarVariantsNb; i++)
    {
        cars[i] = LoadModel(TextFormat("assets/models/car%i.vox", i));
    }
    
    double t1 = GetTime() * 1000.0;

    TraceLog(LOG_INFO, TextFormat("Models loaded in %.3f ms", t1 - t0));
}

/// @brief Unloads all models used by the program
void unloadModels() {
    UnloadModel(road);

    for (int i = 0; i < Constants::CarVariantsNb; i++)
    {
        UnloadModel(cars[i]);
    }
}

/// @brief Sets the camera, prior to the main loop
void setupCamera()
{
    camera.position = (Vector3){50.0f, 50.0f, 50.0f}; // Camera position
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};      // Camera looking at origin point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                              // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;           // Camera mode type
}

/// @brief Updates the camera's position
/// @param dt Delta time
void updateCamera(float dt)
{
    Vector2 dmouse = GetMouseDelta();

    auto pos = Math::cartesianToSpherical(camera.position);

    // If right mouse button is down
    if (IsMouseButtonDown(1))
    {
        float scroll = GetMouseWheelMove();

        // Restricts zoom to a range
        zoom = clamp(zoom - scroll * Constants::ZoomMultiplier * log(zoom), Constants::ZoomMin, Constants::ZoomMax);

        // Define spherical coordinates of the camera
        // Distance to center
        cameraTargetPosition.x = zoom;
        // Vertical tilt (theta angle). Forces camera to stay above ground
        cameraTargetPosition.y = clamp(cameraTargetPosition.y + dt * -dmouse.y, 0.001f, PI / 2);
        // Horizontal azimuth angle (phi)
        cameraTargetPosition.z += dt * dmouse.x;
    }

    // Smooth transition towards target position
    pos = Vector3Lerp(pos, cameraTargetPosition, Constants::CameraMovementSmoothing);

    // Restricts y-axis rotation in range [-pi, +pi]
    if (pos.z > PI)
    {
        cameraTargetPosition.z -= 2*PI;
        pos.z -= 2 * PI;
    }
    else if (pos.z < -PI)
    {
        cameraTargetPosition.z += 2*PI;
        pos.z += 2 * PI;
    }

    camera.position = Math::sphericalToCartesian(pos);
}

int main(int argc, char *argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 1280;
    int screenHeight = 800;

    bool open = true;

    srand(time(0));

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Solferino Project - Basic traffic simulation");
    SetTargetFPS(144);
    rlImGuiSetup(true);

    UI Ui;

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

    setupCamera();

    loadModels();

    Car car(0, 0, 0, simPaused, cars[0]);

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Get deltaTime : time the previous frame took to complete. Enables scaling of movement relative to FPS
        float dt = GetFrameTime();

        updateCamera(dt);

        BeginDrawing();
        ClearBackground(DARKGRAY);
        BeginMode3D(camera);

        DrawModel(road, (Vector3){-20, 0, -20}, 1.0f, WHITE);

        car.update();
        car.draw(true);

        EndMode3D();
        // start ImGui Conent
        rlImGuiBegin();
        Ui.mainMenu(simPaused);

        Ui.agentMenu(&car);

        Ui.debugCamera(camera, cameraTargetPosition);

        // end ImGui Content
        rlImGuiEnd();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    rlImGuiShutdown();

    unloadModels();

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}