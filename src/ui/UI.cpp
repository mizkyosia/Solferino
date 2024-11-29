#include "UI.hpp"
#include "util/Misc.hpp"

UI::UI() : showAgent(false), showMain(false)
{
}

UI::~UI()
{
}

void UI::show(bool &gamePaused, Node *selectedNode, Vehicle *selectedVehicle)
{
    mainMenu(gamePaused);
    if (showAgent)
        agentMenu(selectedVehicle);
    if (showNode)
        nodeMenu(selectedNode);
}

void UI::mainMenu(bool &gamePaused)
{
    ImGui::Begin("Main menu", &showMain, ImGuiWindowFlags_Modal);

    if (ImGui::Button(gamePaused ? ">" : "||"))
    {
        gamePaused = !gamePaused;
    }

    // if (ImGui::Button("Open node menu"))
    // {
    //     showNode = true;
    // }
    if (ImGui::Button("Vehicle debug"))
    {
        showAgent = true;
    }

    if (ImGui::Button("Camera debug"))
    {
        showCamera = true;
    }

    ImGui::TextColored(ImVec4(0, 1, 0, 1), "Debug info");
    ImGui::BeginChild("Debug info");
    ImGui::Text("Delta time : %f\nFPS : %i", GetFrameTime(), GetFPS());
    ImGui::EndChild();

    ImGui::End();
}

void UI::agentMenu(Vehicle *selectedVehicle)
{
    if(!showAgent) 
        return;

    ImGui::Begin("Vehicle menu", &showAgent);

    if (selectedVehicle == nullptr) {
        ImGui::TextColored(ImVec4(.5f, .5f, .5f, 1), "No vehicle selected");
    } else {
        float pos[2] = {selectedVehicle->_pos.x, selectedVehicle->_pos.z};
        ImGui::DragFloat2("Position", pos);
        selectedVehicle->_pos.x = pos[0];
        selectedVehicle->_pos.z = pos[1];
        ImGui::SliderAngle("Angle", &selectedVehicle->_rot, 0, 360);
        ImGui::SliderFloat("Speed", &selectedVehicle->_velocity, 0, selectedVehicle->_maxSpeed);
        ImGui::Text("Acceleration : %f", selectedVehicle->_acceleration);
        ImGui::Text("Velocity : current=%f, max=%f", selectedVehicle->_velocity, selectedVehicle->_maxSpeed);
    }

    ImGui::End();
}

void UI::nodeMenu(Node *selectedNode)
{
}

void UI::debugCamera(Camera3D &camera, Vector3 &target)
{
    if (!showCamera)
        return;

    ImGui::Begin("Camera debug", &showCamera);

    auto pos = Math::cartesianToSpherical(camera.position);

    ImGui::TextColored(ImVec4(.75f, .45f, 0, 1), "Target coordinates : ");
    ImGui::SliderAngle("Phi", &target.z, -180, 180);
    ImGui::SliderAngle("Theta", &target.y, 0.0001f, 90);
    ImGui::SliderFloat("Radius", &target.x, Constants::ZoomMin, Constants::ZoomMax);

    ImGui::TextColored(ImVec4(.75f, .45f, 0, 1), "Target : %f, %f, %f", target.x, target.y, target.z);

    ImGui::End();
}
