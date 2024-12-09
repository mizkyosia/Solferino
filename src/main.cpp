#include <iostream>
#include <cmath>
#include <vector>

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include <imgui.h>
#include <rlImGui.h>

#include "pathfinding/Graph.hpp"
#include "agents/Car.hpp"
#include "ui/UI.hpp"
#include "ui/CustomCamera.hpp"
#include "util/Misc.hpp"
#include "agents/TrafficLights.hpp"

using namespace std;

vector<Vehicle *> toDespawn;

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

    TrafficLightController trafficLights;
    Graph graph(trafficLights);
    CustomCamera camera(graph);
    UI Ui(camera);

    // Shader testShader = LoadShader("assets/shaders/test.vs", "assets/shaders/test.fs");

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

    RenderTexture world = LoadRenderTexture(screenWidth, screenHeight),
                  overlay = LoadRenderTexture(screenWidth, screenHeight);

    Util::LoadAllModels();


    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Get deltaTime : time the previous frame took to complete. Enables scaling of movement relative to FPS
        float dt = GetFrameTime();

        // Update our custom camera
        camera.update(dt);

        // Control debug displays
        if (IsKeyPressed(Util::KeyDebugVehicles))
            Util::DebugVehicles = !Util::DebugVehicles;
        if (IsKeyPressed(Util::KeyDebugNodes))
            Util::DebugNodes = !Util::DebugNodes;

        if (IsKeyPressed(Util::KeySpawnVehicle))
            graph.spawnVehicle(Util::VehicleType::Car);

        // Start the OpenGL drawing context
        BeginTextureMode(world);
        ClearBackground(DARKGRAY);

        BeginMode3D(camera);

        DrawModel(Util::Road, Vector3Zeros, 1.0f, WHITE);

        DrawBoundingBox(GetModelBoundingBox(Util::Road), RED);

        for (auto v : graph.getVehicles())
        {
            v->draw(Util::DebugVehicles);
            if (v->shouldDespawn())
                toDespawn.push_back(v);
        }

        trafficLights.draw();

        for (auto v : toDespawn)
            graph.despawnVehicle(v);

        toDespawn.clear();

        EndMode3D();
        EndTextureMode();

        BeginTextureMode(overlay);
        ClearBackground(Fade(BLACK, Util::DebugNodes ? .4f : 0));
        BeginMode3D(camera);

        if (Util::DebugNodes)
        {
            for (auto n : graph.getNodes())
            {
                Color col = ColorAlpha(n->getColor(), .7f);
                if (&*n == Ui.selectedNode)
                    col = n->getColor();

                DrawSphere(n->getPos(), n->radius, col);

                for (auto l : n->getAllLinks())
                {
                    if (l != Ui.selectedNode || !l->isLinked(n))
                        DrawLine3D(n->getPos(), l->getPos(), col);
                }

                // If we have to link this node with the currently selected one
                if (Ui.selectedNode != nullptr && IsKeyPressed(Util::KeyLinkNode) && camera.lookingAtNode(*n))
                    Ui.selectedNode->link(n);
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

        // end ImGui Content
        rlImGuiEnd();

        // End OpenGL drawing context
        // Past this point, nothing can be drawn onto screen
        EndDrawing();

        // Post-render cleanup and check
        auto destroyedVehicle = Ui.destroyedVehicle();
        if (destroyedVehicle != nullptr)
            graph.despawnVehicle(destroyedVehicle);

        auto destroyedNode = Ui.destroyedNode();
        if (destroyedNode != nullptr)
            graph.removeNode(destroyedNode);
    }
    rlImGuiShutdown();

    Util::UnloadAllModels();

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}