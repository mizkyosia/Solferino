#include "UI.hpp"
#include "util/Misc.hpp"

UI::UI(CustomCamera &_camera) : _camera(_camera), _simPaused(false), _showMain(false), _showAgent(false), _showNode(false), _showCamera(false)
{
}

UI::~UI()
{
}

void UI::show()
{
    // Get the selected node, if it still exists
    _selectedNode = _camera.getSelectedNode(_selectedNode);

    if (IsKeyPressed(Constants::KeyDebugMenu))
        _showMain = !_showMain;

    if (!_showMain)
        return;

    mainMenu();
    if (_showAgent)
        agentMenu();
    if (_showNode)
        nodeMenu();
    if (_showCamera)
        debugCamera();
}

void UI::mainMenu()
{
    ImGui::Begin("Main menu", &_showMain, ImGuiWindowFlags_Modal);

    if (ImGui::Button(_simPaused ? ">" : "||"))
    {
        _simPaused = !_simPaused;
    }

    if (ImGui::Button("Open node menu"))
    {
        _showNode = true;
    }
    if (ImGui::Button("Vehicle debug"))
    {
        _showAgent = true;
    }

    if (ImGui::Button("Camera debug"))
    {
        _showCamera = true;
    }

    ImGui::TextColored(ImVec4(0, 1, 0, 1), "Debug info");
    ImGui::BeginChild("Debug info");
    ImGui::Text("Delta time : %f\nFPS : %i", GetFrameTime(), GetFPS());
    ImGui::Text("Number of nodes : %i", _camera._graph.nodeCount());
    ImGui::Text("Number of vehicles : %i", _camera._vehicles.size());
    ImGui::EndChild();

    ImGui::End();
}

void UI::agentMenu()
{
    if (!_showAgent)
        return;

    // Get the currently selected vehicle, if it still exists
    _selectedVehicle = _camera.getSelectedVehicle(_selectedVehicle);

    ImGui::Begin("Vehicle menu", &_showAgent);

    if (_selectedVehicle == nullptr)
    {
        ImGui::TextColored(ImVec4(.5f, .5f, .5f, 1), "No vehicle selected");
    }
    else
    {
        float pos[2] = {_selectedVehicle->_pos.x, _selectedVehicle->_pos.z};
        ImGui::DragFloat2("Position", pos);
        _selectedVehicle->_pos.x = pos[0];
        _selectedVehicle->_pos.z = pos[1];
        ImGui::SliderAngle("Angle", &_selectedVehicle->_rot, 0, 360);
        ImGui::SliderFloat("Speed", &_selectedVehicle->_velocity, 0, _selectedVehicle->_maxSpeed);
        ImGui::Text("Acceleration : %f", _selectedVehicle->_acceleration);
        ImGui::Text("Velocity : current=%f, max=%f", _selectedVehicle->_velocity, _selectedVehicle->_maxSpeed);
        _destroyVehicle = ImGui::Button("Destroy");
    }

    ImGui::End();
}

void UI::nodeMenu()
{
    if (!_showNode)
        return;

    ImGui::Begin("Node menu", &_showNode);

    if (_selectedNode == nullptr)
        ImGui::TextColored(ImVec4(.5f, .5f, .5f, 1), "No node selected");
    else
    {
        float pos[2] = {_selectedNode->_pos.x, _selectedNode->_pos.z};
        ImGui::DragFloat2("Position", pos);
        _selectedNode->_pos.x = pos[0];
        _selectedNode->_pos.z = pos[1];

        float color[4] = {_selectedNode->_col.r / 255.f, _selectedNode->_col.g / 255.f, _selectedNode->_col.b / 255.f, _selectedNode->_col.a / 255.f};
        ImGui::ColorEdit4("Color", color);
        _selectedNode->_col = {
            static_cast<unsigned char>(color[0] * 255),
            static_cast<unsigned char>(color[1] * 255),
            static_cast<unsigned char>(color[2] * 255),
            static_cast<unsigned char>(color[3] * 255)};

        ImGui::Text("Number of links : %i", _selectedNode->_links.size());
        ImGui::BeginChild("links", ImVec2(0, 0), ImGuiChildFlags_Border);
        for (auto i = _selectedNode->_links.begin(); i != _selectedNode->_links.end(); i++)
            ImGui::TextColored(ImVec4(_selectedNode->_col.r, _selectedNode->_col.g, _selectedNode->_col.b, _selectedNode->_col.a), "Node : dist=%f", Vector2Distance({_selectedNode->_pos.x, _selectedNode->_pos.z}, {(*i)->_pos.x, (*i)->_pos.z}));
        ImGui::EndChild();

        _destroyNode = ImGui::Button("Destroy");
    }

    ImGui::End();
}

void UI::debugCamera()
{
    if (!_showCamera)
        return;

    ImGui::Begin("Camera debug", &_showCamera);

    ImGui::TextColored(ImVec4(.75f, .45f, 0, 1), "Target coordinates : ");
    ImGui::SliderAngle("Phi", &_camera._nextPosition.z, -180, 180);
    ImGui::SliderAngle("Theta", &_camera._nextPosition.y, 0.0001f, 90);
    ImGui::SliderFloat("Radius", &_camera._nextPosition.x, Constants::ZoomMin, Constants::ZoomMax);

    ImGui::TextColored(ImVec4(.75f, .45f, 0, 1), "Target : %f, %f, %f", _camera._nextPosition.x, _camera._nextPosition.y, _camera._nextPosition.z);

    ImGui::End();
}

bool &UI::isSimulationPaused()
{
    return _simPaused;
}

Vehicle *UI::destroyedVehicle()
{
    if (_destroyVehicle)
    {
        auto ptr = _selectedVehicle;
        _selectedVehicle = nullptr;
        return ptr;
    }
    return nullptr;
}

Node *UI::destroyedNode()
{
    if (_destroyNode)
    {
        auto ptr = _selectedNode;
        _selectedNode = nullptr;
        return ptr;
    }
    return nullptr;
}
