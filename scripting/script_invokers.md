# Introduction
Script invokers are super cool, slightly more advanced concept of Enscript. It's their system for delegates which are common in most programming languages, `std::function` in C++ and `Action<T>` in C# are two examples of similar systems. The file `DayZ\scripts\2_gamelib\tools.c` has a good example of how they are used so I will be pasting that here.
```cpp
class Player
{
	ref ScriptInvoker m_DeathInvoker = new ScriptInvoker();
	
	void OnKilled()
	{
		m_DeathInvoker.Invoke(this);
	}
}

void LogPlayerDeath(Player p)
{
	Print("RIP " + p);
}

class Game
{
	void RemovePlayer(Player p) { }
	void GameOver() { }
}

void OnPlayerSpawned(Player p)
{
	Game game = GetGame();
	p.m_DeathInvoker.Insert(LogPlayerDeath);
	p.m_DeathInvoker.Insert(game.RemovePlayer);
	p.m_DeathInvoker.Insert(game.GameOver);
}
```
You kind of have to jump around a bit in this to get the full flow, but what's happening here is that `OnPlayerSpawned` is called, three functions are inserted into `m_DeathInvoker`. Those three functions are then called when `OnKilled()` is called inside the player with `m_DeathInvoker.Invoke(this)` meaning that the game will run `LogPlayerDeath()`, `Game.RemovePlayer()` and `Game.GameOver()`. This example shows off that you can do both static/global functions and functions inside instances. Signing up functions to the Script Invoker is done via name, so you don't have to deal with different parameters or anything like that.

One of the cool aspects of Script Invokers is that they don't need to know any type data from any other [script layer](script_module_layers.c) to execute, so you can freely call things in the GUI (which generally resides in `5_Mission`) from either `3_Game` or `4_World`. Let's use an example:
```cpp
// File in 2_GameLib directory
// Script Invokers can both be static or members of other objects, choose what's best for you.
class MyScriptInvokers
{
	static ref ScriptInvoker OnLocalPlayerHit = new ScriptInvoker();
}
```
We now have a ScriptInvoker we can sign up to, time to modify one of our GUI elements. I chose `IngameHud` since it's easy enough to work with, but you can essentially do this from anywhere.
```cpp
// File in 5_Mission directory
modded class IngameHud
{
	override void Init(Widget hud_panel_widget)
	{
		super.Init(hud_panel_widget);
		MyScriptInvokers.OnLocalPlayerHit.Insert(this.LocalPlayerHit);
	}

	void LocalPlayerHit()
	{
		Print("We got hit!");
	}
}
```
With our ScriptInvoker defined and a callback assigned to it we can then modify our `PlayerBase` a bit. `OnReceivedHit` is a function that is called when something hits a player (big surprise there), so being hit by a zombie or something should trigger this. If `this` is the local player they call the script invoker.
```cpp
// File in 4_World directory
modded class PlayerBase
{
	override void OnReceivedHit(ImpactEffectsData hitData)
	{
		super.OnReceivedHit(hitData);

		#ifndef SERVER
		if (this == GetGame().GetPlayer())
		{
			MyScriptInvokers.OnLocalPlayerHit(this);
		}
		#endif
	}
}
```