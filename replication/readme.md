# Introduction
Similar to other game engines like Unreal and Unity Day does support variable replication, but as with any engine it has its own set of quirks. For the uninitated replication is the concept of making sure that a variable is the same on clients as they are on the server and automatically update the clients whenever the value changes on the server.

In DayZ replication can be done on any EntityAI (and below) that exists on both client and server, but for simplicity's sake we will use `PlayerBase` in these examples.
```cpp
modded class PlayerBase
{
	// Set up a new variable.
	protected int m_killedPlayers = 0;

	override void Init()
	{
		super.Init();

		// Register the variable for replication by sending the name of the variable to 'RegisterNetSyncVariableInt'.
		// Unknown: Can you do this at any point in the object's lifetime or only during initialisation?
		RegisterNetSyncVariableInt("m_killedPlayers");
		/*
		// If you want to sync other types than integers you can use any of the following functions:
		RegisterNetSyncVariableBool(string);
		RegisterNetSyncVariableBoolSignal(string);
		RegisterNetSyncVariableInt(string, int, int);
		RegisterNetSyncVariableFloat(string, int, int);
		RegisterNetSyncVariableObject(string);
		*/
	}
}
```
And that is basically it, the variable `m_killedPlayers` will now be the same for both server and clients, but there are of course more things to know.
# OnVariablesSynchronized
Whenever a synced variable is updated the object that is replicating it will get a call in `OnVariablesSynchronized`, but if you inspect that function you will see that it has no parameters or anything like that which means that you will not know *what* has changed only that something *did* change. Without proper planning this is probably very annoying in some situations, using our example with a player kill count you might want to do something on the client as you reach 10 kills. If you did something like this:
```cpp
modded class PlayerBase
{
	override void OnVariablesSynchronized()
	{
		if (m_killedPlayers == 10)
		{
			// Do something
		}
	}
}
```
You would successfully do something as you reach 10 kills, but you would also trigger the 10 kills code whenever *something else* synced while you were at 10 kills. The best way to solve something like this generally comes down to a case-by-case basis but a simple bool to keep if the code has run or not is one way.
```cpp
modded class PlayerBase
{
	private bool m_hasReached10Kills = false;

	override void OnVariablesSynchronized()
	{
		if (m_killedPlayers == 10 && !m_hasReached10Kills)
		{
			m_hasReached10Kills = true;
			// Do something, but only once
		}
	}
}
```
# SetSynchDirty
Figuring out if variables are supposed to be replicated is generally run on a kind of slow polling rate on the server. This is because there could be a huge amount of variables per player and going through each one for each entity in the game on a full server every frame would take very long. From what I've seen you can expect the value to be synced from server to the client within a second of the value changing on the server, which is fine for some values but worse for others, especially those that drive events on clients.

To partially solve this there is a function called `SetSynchDirty()` which marks the EntityAI as 'dirty' and the replication system will make sure that the variables are synced to clients faster. `SetSynchDirty()` has to be called manually whenever you have changed a variable.
```cpp
modded class PlayerBase
{
	protected int m_killedPlayers = 0;

	override void Init()
	{
		super.Init();

		RegisterNetSyncVariableInt("m_killedPlayers");
	}

	// Create functions to modify variables to more easily pair them together with the 'SetSynchDirty()' call.
	void IncrementPlayerKills()
	{
		++m_killedPlayers;
		SetSynchDirty();
	}
}
```
`SetSynchDirty()` function probably has to be used responsibly or you'd defeat one of the purposes of variable replication. If you want to drive client events from the server and time is of the essence you should probably look into RPCs instead.