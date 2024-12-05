#include "UI.hpp"
#include "util/Misc.hpp"

UI::UI(CustomCamera &_camera) : _camera(_camera), _showMain(false), _showAgent(false), _showNode(false), _showCamera(false)
{
}

UI::~UI()
{
}

void UI::show()
{
    // Get the selected node, if it still exists
    selectedNode = _camera.getSelectedNode(selectedNode);

    if (IsKeyPressed(Util::KeyDebugMenu))
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

    if (ImGui::Button(Util::SimPaused ? ">" : "||"))
    {
        Util::SimPaused = !Util::SimPaused;
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
    ImGui::Text("Number of nodes : %u", _camera._graph.nodeCount());
    ImGui::Text("Number of vehicles : %u", _camera._graph.vehicleCount());
    ImGui::EndChild();

    ImGui::End();
}

void UI::agentMenu()
{
    if (!_showAgent)
        return;

    // Get the currently selected vehicle, if it still exists
    selectedVehicle = _camera.getSelectedVehicle(selectedVehicle);

    ImGui::Begin("Vehicle menu", &_showAgent);

    if (selectedVehicle == nullptr)
    {
        ImGui::TextColored(ImVec4(.5f, .5f, .5f, 1), "No vehicle selected");
    }
    else
    {
        _destroyVehicle = ImGui::Button("Destroy");
        float pos[2] = {selectedVehicle->_pos.x, selectedVehicle->_pos.z};
        ImGui::DragFloat2("Position", pos);
        selectedVehicle->_pos.x = pos[0];
        selectedVehicle->_pos.z = pos[1];
        ImGui::SliderAngle("Angle", &selectedVehicle->_rot, 0, 360);
        ImGui::SliderFloat("Speed", &selectedVehicle->_velocity, 0, selectedVehicle->_maxSpeed);
        ImGui::Text("Acceleration : %f", selectedVehicle->_acceleration);
        ImGui::Text("Velocity : current=%f, max=%f", selectedVehicle->_velocity, selectedVehicle->_maxSpeed);

        ImGui::BeginChild("Path", ImVec2(0, 0), ImGuiChildFlags_Border);
        for(auto i : selectedVehicle->_path)
        {
            ImGui::TextColored(ImVec4(i->_col.r / 255.f, i->_col.g / 255.f, i->_col.b / 255.f, i->_col.a / 255.f), "Node : dist=%f", Vector2Distance({selectedNode->_pos.x, selectedNode->_pos.z}, {i->_pos.x, i->_pos.z}));
            ImGui::SameLine();
            if (ImGui::Button(TextFormat("Remove##%x", i)))
                TraceLog(LOG_WARNING, "Should remove node from path");
        }
        ImGui::EndChild();
    }

    ImGui::End();
}

void UI::nodeMenu()
{
    if (!_showNode)
        return;

    ImGui::Begin("Node menu", &_showNode);

    if (selectedNode == nullptr)
        ImGui::TextColored(ImVec4(.5f, .5f, .5f, 1), "No node selected");
    else
    {
        _destroyNode = ImGui::Button("Destroy");

        float pos[2] = {selectedNode->_pos.x, selectedNode->_pos.z};
        ImGui::DragFloat2("Position", pos);
        selectedNode->_pos.x = pos[0];
        selectedNode->_pos.z = pos[1];

        float color[4] = {selectedNode->_col.r / 255.f, selectedNode->_col.g / 255.f, selectedNode->_col.b / 255.f, selectedNode->_col.a / 255.f};
        ImGui::ColorEdit4("Color", color);
        selectedNode->_col = {
            static_cast<unsigned char>(color[0] * 255),
            static_cast<unsigned char>(color[1] * 255),
            static_cast<unsigned char>(color[2] * 255),
            static_cast<unsigned char>(color[3] * 255)};

        ImGui::Checkbox("Start", &selectedNode->_start);
        ImGui::SameLine();
        ImGui::Checkbox("End", &selectedNode->_end);

        ImGui::Text("Number of links : %lu", selectedNode->_links.size());
        ImGui::BeginChild("links", ImVec2(0, 0), ImGuiChildFlags_Border);
        for (auto i : selectedNode->getAllLinks())
        {
            ImGui::TextColored(ImVec4(i->_col.r / 255.f, i->_col.g / 255.f, i->_col.b / 255.f, i->_col.a / 255.f), "Node : dist=%f", Vector2Distance({selectedNode->_pos.x, selectedNode->_pos.z}, {i->_pos.x, i->_pos.z}));
            ImGui::SameLine();
            if (ImGui::Button(TextFormat("Unlink##%x", i)))
                selectedNode->unlink(i);
        }
        ImGui::EndChild();
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
    ImGui::SliderFloat("Radius", &_camera._nextPosition.x, Util::ZoomMin, Util::ZoomMax);

    ImGui::TextColored(ImVec4(.75f, .45f, 0, 1), "Target : %f, %f, %f", _camera._nextPosition.x, _camera._nextPosition.y, _camera._nextPosition.z);

    ImGui::End();
}

Vehicle *UI::destroyedVehicle()
{
    if (_destroyVehicle)
    {
        auto ptr = selectedVehicle;
        selectedVehicle = nullptr;
        return ptr;
    }
    return nullptr;
}

Node *UI::destroyedNode()
{
    if (_destroyNode)
    {
        auto ptr = selectedNode;
        selectedNode = nullptr;
        return ptr;
    }
    return nullptr;
}
