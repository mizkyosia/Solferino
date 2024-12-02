#pragma once

#include <imgui.h>

#include "../pathfinding/Node.hpp"
#include "../agents/Vehicle.hpp"
#include "CustomCamera.hpp"

class UI
{
    CustomCamera &_camera;

    bool _simPaused;

    bool _showMain, _showAgent, _showNode, _showCamera;

    bool _destroyVehicle, _destroyNode;

public:

    Node* _selectedNode;
    Vehicle* _selectedVehicle;

    UI(CustomCamera &camera);
    ~UI();

    void show();

    void mainMenu();

    void agentMenu();

    void nodeMenu();

    void debugCamera();

    bool& isSimulationPaused();

    Vehicle* destroyedVehicle();
    Node* destroyedNode();
};