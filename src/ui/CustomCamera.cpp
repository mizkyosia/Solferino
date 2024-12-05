#include "CustomCamera.hpp"

CustomCamera::CustomCamera(Graph &graph) : _nextPosition({Util::BaseZoom, PI / 4, 0}),
                                              _zoom(50),
                                              _graph(graph)
{
    position = (Vector3){50.0f, 50.0f, 50.0f}; // Camera position
    target = (Vector3){20, 0, 20};             // Camera looking at center of road
    up = (Vector3){0.0f, 1.0f, 0.0f};          // Camera up vector (rotation towards target)
    fovy = 45.0f;                              // Camera field-of-view Y
    projection = CAMERA_PERSPECTIVE;           // Camera mode type
}

CustomCamera::~CustomCamera()
{
}

void CustomCamera::update(float dt)
{
    Vector2 dmouse = GetMouseDelta();

    auto pos = Math::cartesianToSpherical(position - target);

    // If right mouse button is down
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        HideCursor();

        float scroll = GetMouseWheelMove();

        // Restricts zoom to a range
        _zoom = std::clamp(_zoom - scroll * Util::ZoomMultiplier * log(_zoom), Util::ZoomMin, Util::ZoomMax);

        // Define spherical coordinates of the camera
        // Distance to center
        _nextPosition.x = _zoom;
        // Vertical tilt (theta angle). Forces camera to stay above ground
        _nextPosition.y = std::clamp(_nextPosition.y + dt * -dmouse.y, 0.001f, PI / 2);
        // Horizontal azimuth angle (phi)
        _nextPosition.z += dt * dmouse.x;
    }
    else if (Util::DebugNodes && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(Util::KeyAddNode) || IsKeyPressed(Util::KeyLinkNode)))
    {
        _ray = GetScreenToWorldRay(GetMousePosition(), *this);
        if (IsKeyPressed(Util::KeyAddNode))
        {
            for (int i = 0; i < Util::Road.meshCount; i++)
            {
                auto hitInfo = GetRayCollisionMesh(_ray, Util::Road.meshes[i], Util::Road.transform);
                if (hitInfo.hit)
                {
                    _graph.addNode(hitInfo.point);
                    break;
                }
            }
        }
    }
    else
    {
        ShowCursor();
    }

    // Smooth transition towards target position
    pos = Vector3Lerp(pos, _nextPosition, Util::CameraMovementSmoothing);

    // Restricts y-axis rotation in range [-pi, +pi]
    if (pos.z > PI)
    {
        _nextPosition.z -= 2 * PI;
        pos.z -= 2 * PI;
    }
    else if (pos.z < -PI)
    {
        _nextPosition.z += 2 * PI;
        pos.z += 2 * PI;
    }

    position = Math::sphericalToCartesian(pos) + target;
}

bool CustomCamera::lookingAtNode(Node &node) const
{
    return Util::DebugNodes && GetRayCollisionSphere(_ray, node.getPos(), node.radius).hit;
}

Vehicle *CustomCamera::getSelectedVehicle(Vehicle *previous)
{
    Vehicle *newPtr = nullptr;
    for (auto v : _graph.getVehicles())
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            auto transform = v->getTransform();

            // // Offset the transform by the vehicle's positions
            // transform.m12 += v->_pos.x;
            // transform.m13 += v->_pos.y;
            // transform.m14 += v->_pos.z;

            for (int i = 0; i < v->_model.meshCount; i++)
            {
                auto meshHitInfo = GetRayCollisionMesh(_ray, v->_model.meshes[i], transform);

                // If we hit the vehicle, return a pointer to it
                if (meshHitInfo.hit)
                {
                    // If we click the same vehicle, we un-select it
                    if (v == previous)
                        return nullptr;
                    return v;
                }
            }
        }
        if (v == previous)
        {
            newPtr = previous;
            if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                return newPtr;
            }
        }
    }

    // If we didn't hit anything
    return newPtr;
}

Node *CustomCamera::getSelectedNode(Node *previous)
{
    Node *newPtr = nullptr;
    for (auto n : _graph.getNodes())
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && Util::DebugNodes)
        {
            auto sphereHitInfo = GetRayCollisionSphere(_ray, n->getPos(), n->radius);

            // If we hit the node, return a pointer to it
            if (sphereHitInfo.hit)
            {
                // If we click the same node, we un-select it
                if (n == previous)
                    return nullptr;
                return n;
            }
        }
        if (n == previous)
        {
            newPtr = previous;
            if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                return newPtr;
            }
        }
    }

    // If we didn't hit anything
    return newPtr;
}
