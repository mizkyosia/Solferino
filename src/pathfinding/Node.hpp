#pragma once

#include <set>
#include <SFML/System.hpp>
#include <optional>
#include <raylib.h>
#include <raymath.h>
#include <string>

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
    /// @brief Name of the node, used for debug purposes
    std::string _name;
    /// @brief Color of the node. Used for debug display
    Color _col;

    /// @brief Unique ID of the node
    unsigned long _id;

    /// @brief Bitfield of allowed vehicle types
    char _allowedVehicles;

public:
    constexpr static float radius = 1;

    Node(unsigned long id, const float &x = 0, const float &y = 0);
    Node(const Vector3 &pos, unsigned long id);
    ~Node();

    /// @brief Adds a connection from the current node to another
    /// @param node The node to add a link towards
    /// @param direction The direction of the link
    void link(Node &node);

    /// @brief Deletes the link from this node to the given one
    /// @param node
    /// @return If this node was indeed connected
    bool unlink(Node &node);

    /// @brief Checks if this node is connected to a specific other node
    /// @param node
    /// @return
    bool isLinked(Node &node) const;

    /// @brief Returns a reference to the link containing the given node, if this link exists
    /// @param node The node to find in the link collection
    /// @return
    std::optional<Node *> getLink(Node &node) const;

    /// @brief Get the link collection of this node
    /// @return
    std::set<Node *> getAllLinks() const;

    /// @brief Checks if the node is being blocked (by traffic lights mostly)
    /// @return
    bool isBlocked() const;

    /// @brief Gets the debug color of this node
    /// @return 
    Color getColor();

    Vector3 getPos() const;


    bool operator==(const Node& other) const;

    /// @brief Adds UI as a friend class, for access to private fields
    friend class UI;
};