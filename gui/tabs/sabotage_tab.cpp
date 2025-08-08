#include "pch-il2cpp.h"
#include "sabotage_tab.h"
#include "utility.h"
#include "gui-helpers.hpp"
#include "_rpc.h"
#include "game.h"
#include "state.hpp"

namespace SabotageTab {
    void Render() {
        if (IsInGame()) {
            ImGui::SameLine(100 * State.dpiScale);
            ImGui::BeginChild("###Sabotage", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
            ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
            if (IsHost() && ToggleButton("Disable Sabotages", &State.DisableSabotages)) {
                ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
                ImGui::Separator();
                ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
            }
            if (ImGui::Button("Repair Sabotage")) {
                RepairSabotage(*Game::pLocalPlayer);
            }
            ImGui::NewLine();
            if (State.DisableSabotages)
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Sabotages have been disabled. Nothing can be sabotaged.");
            //i skidded some code from https://github.com/scp222thj/MalumMenu/ for making the no cooldown sabotages (right back up :D)
            if (ImGui::Button("Sabotage All")) {
                if (State.mapType != Settings::MapType::Fungle) { //lights don't work on fungle
                    for (size_t i = 0; i < 5; i++)
                        State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Electrical, i));
                }

                if (State.mapType == Settings::MapType::Ship || State.mapType == Settings::MapType::Hq || State.mapType == Settings::MapType::Fungle)
                    State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Reactor, 128));
                else if (State.mapType == Settings::MapType::Pb)
                    State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Laboratory, 128));
                else if (State.mapType == Settings::MapType::Airship)
                    State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::HeliSabotage, 128));

                if (State.mapType == Settings::MapType::Ship || State.mapType == Settings::MapType::Hq)
                    State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::LifeSupp, 128));

                if (State.mapType == Settings::MapType::Fungle)
                    State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::MushroomMixupSabotage, 1));

                State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Comms, 128));
            }

            if (ImGui::Button("Random Sabotage")) {
                switch (State.mapType) {
                case Settings::MapType::Pb:
                {
                    int randIndex = randi(1, 3);
                    switch (randIndex) {
                    case 1:
                    {
                        for (size_t i = 0; i < 5; i++)
                            State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Electrical, i));
                    } break;
                    case 2: State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Laboratory, 128)); break;
                    case 3: State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Comms, 128)); break;
                    }
                }
                break;
                case Settings::MapType::Airship:
                {
                    int randIndex = randi(1, 3);
                    switch (randIndex) {
                    case 1:
                    {
                        for (size_t i = 0; i < 5; i++)
                            State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Electrical, i));
                    } break;
                    case 2: State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::HeliSabotage, 128)); break;
                    case 3: State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Comms, 128)); break;
                    }
                }
                break;
                case Settings::MapType::Fungle:
                {
                    int randIndex = randi(1, 3);
                    switch (randIndex) {
                    case 1: State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::MushroomMixupSabotage, 1)); break;
                    case 2: State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Reactor, 128)); break;
                    case 3: State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Comms, 128)); break;
                    }
                }
                break;
                default: //skeld and mira have same sabotages
                {
                    int randIndex = randi(1, 4);
                    switch (randIndex) {
                    case 1:
                    {
                        for (size_t i = 0; i < 5; i++)
                            State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Electrical, i));
                    } break;
                    case 2: State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Reactor, 128)); break;
                    case 3: State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::LifeSupp, 128)); break;
                    case 4: State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Comms, 128)); break;
                    }
                }
                break;
                }
            }

            if (State.mapType != Settings::MapType::Fungle && ImGui::Button("Sabotage Lights")) {
                for (size_t i = 0; i < 5; i++)
                    State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Electrical, i));
            }
            if (State.mapType == Settings::MapType::Ship || State.mapType == Settings::MapType::Hq || State.mapType == Settings::MapType::Fungle) {
                if (ImGui::Button("Sabotage Reactor")) {
                    State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Reactor, 128));
                }
            }
            else if (State.mapType == Settings::MapType::Pb) {
                if (ImGui::Button("Sabotage Seismic Stabilizers")) {
                    State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Laboratory, 128));
                }
            }
            else if (State.mapType == Settings::MapType::Airship) {
                if (ImGui::Button("Sabotage Crash Course")) {
                    State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::HeliSabotage, 128));
                }
            }
            if (State.mapType == Settings::MapType::Ship || State.mapType == Settings::MapType::Hq) {
                if (ImGui::Button("Sabotage Oxygen")) {
                    State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::LifeSupp, 128));
                }
            }
            if (State.mapType == Settings::MapType::Fungle) {
                if (ImGui::Button("Activate Mushroom Mixup")) {
                    State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::MushroomMixupSabotage, 1));
                }
            }
            if (ImGui::Button("Sabotage Comms")) {
                State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Comms, 128));
            }

            ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);
            ImGui::Separator();
            ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);

            if (State.mapType != Settings::MapType::Fungle && ToggleButton("Disable Lights", &State.DisableLights)) {
                if (auto switchSystem = (SwitchSystem*)il2cpp::Dictionary((*Game::pShipStatus)->fields.Systems)[SystemTypes__Enum::Electrical]) {
                    auto actualSwitches = switchSystem->fields.ActualSwitches;
                    auto expectedSwitches = switchSystem->fields.ExpectedSwitches;

                    auto switchMask = 1 << ((State.DisableLights ? 0 : 5) & 0x1F);

                    if ((actualSwitches & switchMask) != ((State.DisableLights ? ~expectedSwitches : expectedSwitches) & switchMask))
                        State.rpcQueue.push(new RpcUpdateSystem(SystemTypes__Enum::Electrical, 5));
                }
            }

            ImGui::EndChild();
        }
    }
}