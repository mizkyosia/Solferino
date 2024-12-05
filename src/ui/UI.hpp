#pragma once

#include <imgui.h>

#include "../pathfinding/Node.hpp"
#include "../agents/Vehicle.hpp"
#include "CustomCamera.hpp"

class UI
{
    CustomCamera &_camera;

    bool _showMain, _showAgent, _showNode, _showCamera;

    bool _destroyVehicle = false, _destroyNode = false;

public:

    Node* selectedNode;
    Vehicle* selectedVehicle;

    UI(CustomCamera &camera);
    ~UI();

    void show();

    void mainMenu();

    void agentMenu();

    void nodeMenu();

    void debugCamera();

    Vehicle* destroyedVehicle();
    Node* destroyedNode();
};