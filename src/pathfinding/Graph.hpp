#pragma once

#include "Node.hpp"
#include "agents/Vehicle.hpp"
#include <set>
#include <queue>
#include <vector>
#include <map>

class Graph
{
private:
    std::set<Node *> _nodes;

    std::set<Vehicle *> _vehicles;

public:
    Graph();
    ~Graph();

    /// @brief Adds a node to the graph at the given position
    /// @param pos 
    Node* addNode(Vector3 pos, bool start = false, bool end = false, Util::VehicleType allowedVehicles = Util::VehicleType::Car, Color color = YELLOW);

    /// @brief Removes a node from the graph, along with all of its links towards other nodes
    /// @param node
    void removeNode(Node *node);

    /// @brief Reference to the underlying node set of the graph
    /// @return 
    std::set<Node *> &getNodes();

    /// @brief Reference to the vehicle node set of the graph
    /// @return 
    std::set<Vehicle *> &getVehicles();

    /// @brief The number of nodes inside the graph
    /// @return 
    unsigned int nodeCount();

    /// @brief The number of vehicles currently in the simulation
    /// @return 
    unsigned int vehicleCount();

    /// @brief Spawns a new vehicle, and gives it a path to follow
    void spawnVehicle(Util::VehicleType kind);

    /// @brief Despawns a vehicle, removing from both the graph and memory
    /// @param vehicle 
    void despawnVehicle(Vehicle* vehicle);

    /// @brief Finds a path between two nodes
    /// @param from
    /// @param to
    /// @param vehicleType
    /// @return
    std::vector<Node *> findPath(Node *from, Node *to, const Util::VehicleType vehicleType) const;
};