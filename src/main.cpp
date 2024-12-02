#include <iostream>
#include <cmath>
#include <vector>

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include <imgui.h>
#include "rlImGui.h"

#include "pathfinding/Graph.hpp"
#include "agents/Car.hpp"
#include "ui/UI.hpp"
#include "ui/CustomCamera.hpp"
#include "util/Misc.hpp"

using namespace std;

Model road, carModels[2];

Graph graph;

bool debugCars = false, debugNodes = false;

/// @brief Collection of the vehicles currently in the simulation
vector<Vehicle> vehicles;

/// @brief Load all models needed by the program
void loadModels()
{
    double t0 = GetTime() * 1000.0;

    road = LoadModel("assets/models/road.vox");
    road.transform.m13 -= Constants::GroundLevel;

    for (int i = 0; i < Constants::CarVariantsNb; i++)
    {
        carModels[i] = LoadModel(TextFormat("assets/models/car%i.vox", i));
    }

    double t1 = GetTime() * 1000.0;

    TraceLog(LOG_INFO, TextFormat("Models loaded in %.3f ms", t1 - t0));
}

/// @brief Unloads all models used by the program
void unloadModels()
{
    UnloadModel(road);

    for (int i = 0; i < Constants::CarVariantsNb; i++)
    {
        UnloadModel(carModels[i]);
    }

    TraceLog(LOG_INFO, "Unloaded all models");
}

int main(int argc, char *argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 1280;
    int screenHeight = 800;

    srand(time(0));

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Solferino Project - Basic traffic simulation");
    SetTargetFPS(144);
    rlImGuiSetup(true);

    CustomCamera camera(graph, vehicles, road, debugNodes);

    UI Ui(camera);

    Shader testShader = LoadShader("assets/shaders/test.vs", "assets/shaders/test.fs");

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

    Texture2D arrow = LoadTexture("assets/img/arrow.png");

    RenderTexture world = LoadRenderTexture(screenWidth, screenHeight),
                  overlay = LoadRenderTexture(screenWidth, screenHeight);

    loadModels();

    vehicles.push_back(Car(0, 0, 0, Ui.isSimulationPaused(), carModels[0]));
    vehicles.push_back(Car(-5, 0, 0, Ui.isSimulationPaused(), carModels[0]));

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Get deltaTime : time the previous frame took to complete. Enables scaling of movement relative to FPS
        float dt = GetFrameTime();

        // Update our custom camera
        camera.update(dt);

        // Control debug displays
        if (IsKeyPressed(Constants::KeyDebugVehicles))
            debugCars = !debugCars;
        if (IsKeyPressed(Constants::KeyDebugNodes))
            debugNodes = !debugNodes;

        // Start the OpenGL drawing context
        BeginTextureMode(world);
        ClearBackground(DARKGRAY);

        BeginMode3D(camera);

        DrawModel(road, Vector3Zeros, 1.0f, WHITE);

        DrawBoundingBox(GetModelBoundingBox(road), RED);

        for (auto v : vehicles)
        {
            v.update();
            v.draw(debugCars);
        }

        EndMode3D();
        EndTextureMode();

        BeginTextureMode(overlay);
        ClearBackground(Fade(BLACK, debugNodes ? .4f : 0));
        BeginMode3D(camera);

        if (debugNodes)
        {
            for (auto n = graph.getNodes().begin(); n < graph.getNodes().end(); n++)
            {
                Color col = ColorAlpha(n->getColor(), .7f);
                if (&*n == Ui._selectedNode)
                    col = n->getColor();
                
                // if(&*n == Ui._selectedNode) BeginShaderMode(testShader);
                DrawSphere(n->getPos(), n->radius, col);
                // if(&*n == Ui._selectedNode) EndShaderMode();

                for (auto l : n->getAllLinks())
                {
                    DrawLine3D(n->getPos(), l->getPos() + Vector3(0, .1f, 0), col);
                }

                // If we have to link this node with the currently selected one
                if (Ui._selectedNode != nullptr && IsKeyPressed(Constants::KeyLinkNode) && camera.lookingAtNode(*n))
                    Ui._selectedNode->link(*n);
            }
        }

        EndMode3D();
        EndTextureMode();

        BeginDrawing();
        DrawTextureRec(world.texture, {0, 0, (float)screenWidth, -(float)screenHeight}, {0, 0}, WHITE);
        DrawTextureRec(overlay.texture, {0, 0, (float)screenWidth, -(float)screenHeight}, {0, 0}, WHITE);

        // start ImGui Content
        rlImGuiBegin();

        // Manage UI
        Ui.show();

        auto destroyedVehicle = Ui.destroyedVehicle();
        if (destroyedVehicle != nullptr)
        {
            for (auto v = vehicles.begin(); v < vehicles.end(); v++)
            {
                if (&*v == destroyedVehicle)
                {
                    vehicles.erase(v);
                    break;
                }
            }
        }

        auto destroyedNode = Ui.destroyedNode();
        if (destroyedNode != nullptr)
            graph.removeNode(destroyedNode);

        // end ImGui Content
        rlImGuiEnd();

        // End OpenGL drawing context
        // Past this point, nothing can be drawn onto screen
        EndDrawing();
    }
    rlImGuiShutdown();

    unloadModels();

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}