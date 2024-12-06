#pragma once

#include <set>
#include <SFML/System.hpp>
#include <optional>
#include <raylib.h>
#include <raymath.h>
#include <string>

#include "util/Misc.hpp"

enum NodeType
{
    Start,
    End,
    Regular
};

class Node
{
private:
    /// @brief Position of the graph node
    Vector3 _pos;
    /// @brief List of all the nodes this one is connected to
    std::set<Node *> _links;
    /// @brief Is the node blocked ?
    bool _blocked = false;
    bool _start = false;
    bool _end = false;
    /// @brief Name of the node, used for debug purposes
    std::string _name;
    /// @brief Color of the node. Used for debug display
    Color _col;

    /// @brief Bitfield of allowed vehicle types
    Util::VehicleType _allowedVehicles;

public:
    constexpr static float radius = 1;

    Node(const float &x = 0, const float &y = 0);
    Node(const Vector3 &pos, bool start = false, bool end = false, Util::VehicleType allowedVehicles = Util::VehicleType::Car, Color color = YELLOW);
    ~Node();

    /// @brief Adds a connection from the current node to another
    /// @param node The node to add a link towards
    /// @param direction The direction of the link
    void link(Node *node);

    /// @brief Deletes the link from this node to the given one
    /// @param node
    /// @return If this node was indeed connected
    bool unlink(Node *node);

    /// @brief Checks if this node is connected to a specific other node
    /// @param node
    /// @return
    bool isLinked(Node *node) const;

    /// @brief Get the link collection of this node
    /// @return
    std::set<Node *> getAllLinks() const;

    /// @brief Checks if the node is being blocked (by traffic lights mostly)
    /// @return
    bool isBlocked() const;

    /// @brief Checks if the node accepts this vehicle type
    /// @param vehicleType
    /// @return
    bool allowsVehicle(Util::VehicleType vehicleType) const;

    /// @brief Gets the debug color of this node
    /// @return
    Color getColor();

    /// @brief Is it a valid start for a path ?
    /// @return
    bool isStart();

    /// @brief Is it a valid end for a path ?
    /// @return
    bool isEnd();

    Vector3 getPos() const;

    /// @brief Adds UI as a friend class, for access to private fields
    friend class UI;
};

/// @brief Structure representing a node. Used for pathfinding algorithm only
struct PathNode
{
public:
    Node *inner;
    float cost;
};

struct PathComparator
{
    bool operator()(const PathNode a, PathNode b) { return a.cost > b.cost; }
};