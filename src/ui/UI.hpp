#pragma once

#include <imgui.h>

#include "../pathfinding/Node.hpp"
#include "../agents/Vehicle.hpp"

class UI
{
    bool showMain, showAgent, showNode, showCamera;

public:
    UI();
    ~UI();

    void show(bool &gamePaused, Node *selectedNode, Vehicle *selectedVehicle);

    void mainMenu(bool &gamePaused);

    void agentMenu(Vehicle *selectedVehicle);

    void nodeMenu(Node *selectedNode);

    void debugCamera(Camera3D &camera, Vector3& target);
};