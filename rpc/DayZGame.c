modded class DayZGame {
	const float CLOSE_SOUND_DISTANCE = 800.0;
	const float DISTANT_SOUND_DISTANCE = 2000.0;

	void HelicopterCrashStart(vector location) {
		if (GetGame().IsServer()) {
			auto sendData = new array<ref Param>();

			sendData.Insert(new Param1<int>(ZeroRevivalRPCs.STC_HELICOPTER_CRASH_START));
			sendData.Insert(new Param1<vector>(location));

			GetDayZGame().RPC(null, ZRConstants.ZERO_REVIVAL_RPC, sendData, true);
		} else {
			if (vector.Distance(GetGame().GetPlayer().GetPosition(), location) <= CLOSE_SOUND_DISTANCE) {
				m_CrashSound = SEffectManager.PlaySound("Artillery_Close_SoundSet", location, 0.1, 0.1);
				m_CrashSound.SetAutodestroy(true);
			} else if (vector.Distance(GetGame().GetPlayer().GetPosition(), location) <= DISTANT_SOUND_DISTANCE) {
				m_CrashSound = SEffectManager.PlaySound("Artillery_Distant_SoundSet", location, 0.1, 0.1);
				m_CrashSound.SetAutodestroy(true);
			}
		}
	}

	void HelicopterCrashFinished(vector location) {
		if (GetGame().IsServer()) {
			auto sendData = new array<ref Param>();

			sendData.Insert(new Param1<int>(ZeroRevivalRPCs.STC_HELICOPTER_CRASH_FINISHED));
			sendData.Insert(new Param1<vector>(location));

			GetDayZGame().RPC(null, ZRConstants.ZERO_REVIVAL_RPC, sendData, true);
		} else {
			if (vector.Distance(GetGame().GetPlayer().GetPosition(), location) <= CLOSE_SOUND_DISTANCE) {
				m_CrashSound = SEffectManager.PlaySound("Artillery_Close_SoundSet", location, 0.1, 0.1);
				m_CrashSound.SetAutodestroy(true);
			} else if (vector.Distance(GetGame().GetPlayer().GetPosition(), location) <= DISTANT_SOUND_DISTANCE) {
				m_CrashSound = SEffectManager.PlaySound("Artillery_Distant_SoundSet", location, 0.1, 0.1);
				m_CrashSound.SetAutodestroy(true);
			}
		}
	}

	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) {
		super.OnRPC(sender, target, rpc_type, ctx);

		if (rpc_type == ZRConstants.ZERO_REVIVAL_RPC) {
			Param1<int> param;

			if (!ctx.Read(param)) {
				return;
			}

			switch (param.param1) {
				case ZeroRevivalRPCs.STC_HELICOPTER_CRASH_START: {
					Param1<vector> backupLocation;

					if (!ctx.Read(backupLocation)) {
						return;
					}

					HelicopterCrashStart(backupLocation.param1);
					break;
				}
				case ZeroRevivalRPCs.STC_HELICOPTER_CRASH_FINISHED: {
					Param1<vector> finishedBackupLocation;

					if (!ctx.Read(finishedBackupLocation)) {
						return;
					}

					HelicopterCrashFinished(finishedBackupLocation.param1);
					break;
				}
			}
		}
	}
}
