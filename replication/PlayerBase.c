modded class PlayerBase
{
	private int m_killedPlayers = 0;
	private bool m_hasReached10Kills = false;

	override void Init()
	{
		super.Init();

		RegisterNetSyncVariableInt("m_killedPlayers");
	}

	override void OnVariablesSynchronized()
	{
		if (m_killedPlayers == 10 && !m_hasReached10Kills)
		{
			m_hasReached10Kills = true;
			// Do something
		}
	}

	void IncrementPlayerKills()
	{
		++m_killedPlayers;
		SetSynchDirty();
	}
}