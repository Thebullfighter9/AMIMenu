
#include "pch-il2cpp.h"
#include "menu.hpp"
#include "imgui/imgui.h"
#include "tabs/about_tab.h"
#include "tabs/doors_tab.h"
#include "tabs/game_tab.h"
#include "tabs/host_tab.h"
#include "tabs/players_tab.h"
#include "tabs/radar_tab.h"
#include "tabs/replay_tab.h"
#include "tabs/esp_tab.h"
#include "tabs/sabotage_tab.h"
#include "tabs/self_tab.h"
#include "tabs/settings_tab.h"
#include "tabs/tasks_tab.h"
#ifdef _DEBUG
#include "tabs/debug_tab.h"
#endif
#include "state.hpp"
#include "gui-helpers.hpp"
#include "cloak.h"

namespace Menu {
    static bool openAbout = false;
    static bool openSettings = false;
    static bool openGame = false;
    static bool openSelf = false;
    static bool openRadar = false;
    static bool openReplay = false;
    static bool openEsp = false;
    static bool openPlayers = false;
    static bool openTasks = false;
    static bool openSabotage = false;
    static bool openDoors = false;
    static bool openHost = false;
#ifdef _DEBUG
    static bool openDebug = false;
#endif

    void CloseAllOtherTabs(Tabs openTab) {
        openAbout = openTab == Tabs::About;
        openSettings = openTab == Tabs::Settings;
        openGame = openTab == Tabs::Game;
        openSelf = openTab == Tabs::Self;
        openRadar = openTab == Tabs::Radar;
        openReplay = openTab == Tabs::Replay;
        openEsp = openTab == Tabs::Esp;
        openPlayers = openTab == Tabs::Players;
        openTasks = openTab == Tabs::Tasks;
        openSabotage = openTab == Tabs::Sabotage;
        openDoors = openTab == Tabs::Doors;
        openHost = openTab == Tabs::Host;
#ifdef _DEBUG
        openDebug = openTab == Tabs::Debug;
#endif
    }

    void Init() {
        ImGui::SetNextWindowSize(ImVec2(600, 400) * State.dpiScale, ImGuiCond_None);
        ImGui::SetNextWindowBgAlpha(State.MenuThemeColor.w);
    }

    bool init = false;
    bool firstRender = true;
    void Render() {
        try {
            if (!init)
                Menu::Init();
            std::string modText = "DragonMenu " + State.SickoVersion;
            ImGui::Begin(modText.c_str(), &State.ShowMenu, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

            auto titleCol = State.RgbMenuTheme ? State.RgbColor : State.MenuThemeColor;
            titleCol.w = 1.f;
            ImGui::TextColored(titleCol, modText.c_str());
            ImGui::SameLine(ImGui::GetWindowWidth() - 20 * State.dpiScale);
            if (ImGui::Button("-")) State.ShowMenu = false;

            ImGui::BeginChild("###DragonMenu", ImVec2(90 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
            if (ImGui::Selectable("About", openAbout)) CloseAllOtherTabs(Tabs::About);
            if (ImGui::Selectable("Settings", openSettings)) CloseAllOtherTabs(Tabs::Settings);
            if (ImGui::Selectable("Game", openGame)) CloseAllOtherTabs(Tabs::Game);
            if (ImGui::Selectable("Self", openSelf)) CloseAllOtherTabs(Tabs::Self);
            if (ImGui::Selectable("Radar", openRadar)) CloseAllOtherTabs(Tabs::Radar);
            if (ImGui::Selectable("Replay", openReplay)) CloseAllOtherTabs(Tabs::Replay);
            if (ImGui::Selectable("ESP", openEsp)) CloseAllOtherTabs(Tabs::Esp);
            if ((IsInGame() || IsInLobby()) && ImGui::Selectable("Players", openPlayers)) CloseAllOtherTabs(Tabs::Players);
            if ((IsInGame() && GetPlayerData(*Game::pLocalPlayer)->fields.Tasks != NULL) && ImGui::Selectable("Tasks", openTasks)) CloseAllOtherTabs(Tabs::Tasks);
            if (IsInGame() && ImGui::Selectable("Sabotage", openSabotage)) CloseAllOtherTabs(Tabs::Sabotage);
            if ((IsInGame() && !State.mapDoors.empty()) && ImGui::Selectable("Doors", openDoors)) CloseAllOtherTabs(Tabs::Doors);
            if (IsHost() && ImGui::Selectable("Host", openHost)) CloseAllOtherTabs(Tabs::Host);
#ifdef _DEBUG
            if (State.showDebugTab && ImGui::Selectable("Debug", openDebug)) CloseAllOtherTabs(Tabs::Debug);
#endif
            if (firstRender) { firstRender = false; CloseAllOtherTabs(Tabs::About); }
            ImGui::EndChild();

            if (openAbout) AboutTab::Render();
            else if (!State.HasOpenedMenuBefore) State.HasOpenedMenuBefore = true;
            if (openSettings) SettingsTab::Render();
            if (openGame) GameTab::Render();
            if (openSelf) SelfTab::Render();
            if (openRadar) RadarTab::Render();
            if (openReplay) ReplayTab::Render();
            if (openEsp) EspTab::Render();
            if (openPlayers) {
                if (IsInGame() || IsInLobby()) PlayersTab::Render();
                else { CloseAllOtherTabs(Tabs::Game); GameTab::Render(); }
            }
            if (openTasks) {
                if (IsInGame() && GetPlayerData(*Game::pLocalPlayer)->fields.Tasks != NULL) TasksTab::Render();
                else { CloseAllOtherTabs(Tabs::Game); GameTab::Render(); }
            }
            if (openSabotage) {
                if (IsInGame()) SabotageTab::Render();
                else { CloseAllOtherTabs(Tabs::Game); GameTab::Render(); }
            }
            if (openDoors) {
                if (IsInGame() && !State.mapDoors.empty()) DoorsTab::Render();
                else { CloseAllOtherTabs(Tabs::Game); GameTab::Render(); }
            }
            if (openHost) {
                if (IsHost()) HostTab::Render();
                else { CloseAllOtherTabs(Tabs::Game); GameTab::Render(); }
            }
#ifdef _DEBUG
            if (openDebug) {
                if (State.showDebugTab) DebugTab::Render();
                else { CloseAllOtherTabs(Tabs::Game); GameTab::Render(); }
            }
#endif
            ImGui::End();
        }
        catch (...) {
            LOG_ERROR("Exception occurred when rendering menu");
        }
    }
}
