#include "pch-il2cpp.h"
#include "_hooks.h"
#include "utility.h"
#include "logger.h"
#include <state.hpp>

void dExileController_ReEnableGameplay(ExileController* __this, MethodInfo* method) {
    app::ExileController_ReEnableGameplay(__this, method);

	try {// ESP: Reset Kill Cooldown
		for (auto pc : GetAllPlayerControl()) {
			if (auto player = PlayerSelection(pc).validate();
				player.has_value() && !player.is_LocalPlayer() && !player.is_Disconnected()) {
				if (auto role = player.get_PlayerData()->fields.Role;
					role != nullptr && role->fields.CanUseKillButton && !player.get_PlayerData()->fields.IsDead) {
					pc->fields.killTimer = (std::max)(GameOptions().GetKillCooldown(), 0.f);
					//STREAM_DEBUG("Player " << ToString(pc) << " KillTimer " << pc->fields.killTimer);
				}
			}
		}
	}
	catch (...) {
		LOG_ERROR("Exception occurred in ExileController_ReEnableGameplay (ExileController)");
	}
}