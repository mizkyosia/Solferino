#pragma once

#include <algorithm>
#include <vector>
#include <raylib.h>

#include "util/Misc.hpp"
#include "agents/Vehicle.hpp"
#include "pathfinding/Graph.hpp"

class CustomCamera : public Camera3D
{
private:
    /// @brief The desired position of the camera. Used for smoothing movement
    Vector3 _nextPosition;

    /// @brief Zoom of the camera
    float _zoom;

    /// @brief Ray used by the camera for picking
    Ray _ray = { 0 };
    /// @brief Collision of the raycast
    RayCollision _collision = { 0 };

    /// @brief Safe reference to the graph of the simulation
    Graph& _graph;

    /// @brief Safe reference to the ground mesh
    Model& _ground;

    bool& _nodeDebug;
    
    /// @brief Safe reference to the vehicles of the simulation
    std::vector<Vehicle>& _vehicles;

    friend class UI;
public:
    CustomCamera(Graph& graph, std::vector<Vehicle>& vehicles, Model& ground, bool& nodeDebug);
    ~CustomCamera();

    void update(float dt);

    bool lookingAtNode(Node &node) const;

    Vehicle* getSelectedVehicle(Vehicle* previous);
    Node* getSelectedNode(Node* previous);
};
