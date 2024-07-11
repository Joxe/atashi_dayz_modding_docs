# Introduction
Remote Procedure Calls (RPCs) are ways of, as the name suggests, calling functions remotely. In practice this means that you on the server can find a player in the code and run a function on that player's client, or the other way around. In general there are two functions to work with RPCs but they do require a fair bit of planning, especially with a game like DayZ which has a multitude of mods that also use RPCs that you probably won't know about at all when writing your code.

To send an RPC you need to run the function `GetGame().RPC(Object, int, notnull array<ref Param>, bool, PlayerIdentity)` function:
```cpp
modded class PlayerBase
{
	void SendSomeDataAsRpc()
	{
		auto sendData = new array<ref Param>();

		sendData.Insert(new Param1<int>(1337));

		GetGame().RPC(this, 12345, sendData, true);
	}
}
```
The function declaration in 'game.c' of the DayZ script code base describes this function rather well, so use that alongside any explanation that I write. Calling `SendSomeDataAsRpc()` on the server will instead execute it on all clients, calling it on a client will execute it on the server.

We will look into each parameter later, first we should look at its counterpart which is what is executed on the other side of the internet.
```cpp
modded class PlayerBase
{
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		if (rpc_type == 12345)
		{
			Param1<int> param;

			if (!ctx.Read(param)) {
				return;
			}

			Print(string.Format("Got RPC, the data was '%1', param.param1));
		}
	}
}
```
This function is called for *every* RPC that the object receives and it's up to you to figure out if the RPC was something that you should do something with. To help us we get the `rpc_type` parameter which corresponds with the second parameter in the `GetGame().RPC()` call, in this case we check if that `rpc_type` is `12345` which means that we should print the value sent.
# Reading Parameters in OnRPC
The somewhat weird part if you're newer to programming is the `ctx` parameter, this is technically just a row of bytes that we need to turn into data that we can use. Understanding the details isn't too important, we only need to understand how to use it. You should basically see this as a queue of data that is sent between the server and client, so where you send the RPC you add data in a specific order, and then you read it from `ctx` in the same order. Probably best shown with another example:
```cpp
modded class PlayerBase
{
	void SendSomeDataAsRpc()
	{
		auto sendData = new array<ref Param>();

		// The parameters are sent in the order of: Integer, Float and String
		sendData.Insert(new Param1<int>(1337));
		sendData.Insert(new Param1<float>(420.69));
		sendData.Insert(new Param1<string>("My String"));

		GetGame().RPC(this, 12345, sendData, true);
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		if (rpc_type == 12345)
		{
			// And then we read them in the same order, Integer, Float and String
			Param1<int> intParam;
			Param<float> floatParam;
			Param<string> stringParam;

			if (!ctx.Read(intParam)) {
				return;
			}

			if (!ctx.Read(floatParam)) {
				return;
			}

			if (!ctx.Read(stringParam)) {
				return;
			}

			Print(string.Format("Got RPC, Integer Data '%1' Float Data '%2' String Data '%3'", intParam.param1, floatParam.param1, stringParam.param1));
		}
	}
}
```
Reading them in the wrong order will most likely give you the wrong data or make the `ctx.Read()` call fail.

Another way to ensure the order being the same is to use the `ParamX` (Param2, Param3...and so on), but I have not personally used this yet since I haven't done many RPCs. This should be classed as an **UNKNOWN**, expect this to be updated like tomorrow when I finally get around trying this. The only drawback for this solution is that you cannot use optional data, you won't gain anything when checking if `param2` is `false` or whatever and based on that decide to read `param3`, which you could if you used the multiple param instances solution.
```cpp
modded class PlayerBase
{
	void SendSomeDataAsRpc()
	{
		auto sendData = new array<ref Param>();

		// The parameters are sent in the order of: Integer, Float and String
		sendData.Insert(new Param3<int, float, string>(1337, 420.69, "My String"));

		GetGame().RPC(this, 12345, sendData, true);
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		if (rpc_type == 12345)
		{
			// And then we read them in the same order, Integer, Float and String
			Param3<int, float, string> params;

			if (!ctx.Read(params)) {
				return;
			}

			Print(string.Format("Got RPC, Integer Data '%1' Float Data '%2' String Data '%3'", params.param1, params.param2, params.param3));
		}
	}
}
```
# RPC parameters explanation
## Parameter 1: Object target
This should be the EntityAI that should receive the RPC, in many cases you will just pass 'this' into the target to run a function on either the client or server on the same object. If you have a manager object that sends RPCs you could set this to the object that the RPC should be called on.
```cpp
// I literally don't know where this is.
auto someLocation = Vector(10000, 300, 7000);

foreach (auto player : players)
{
	// This will only call the RPC on players within a certain range of the location.
	if (vector.Distance(player.GetPosition(), someLocation) < 1000)
	{
		auto sendData = new array<ref Param>();

		sendData.Insert(new Param1<int>(1337));

		GetGame().RPC(player, 12345, sendData, true);
	}
}
```
It also has a special case when you send `null` as the parameter, the RPC will then be called on the `DayZGame` object (unknown: does it also call on any base type of `DayZGame`?) and this is something you can use for events in the world when you are not sure if a specific EntityAI exists on a client. DayZ uses this to play the helicopter crash site sound so it's audible for players that are outside the network bubble of the crash site itself.
## Parameter 2: int rpc_type
The `rpc_type` parameter tells the receiver of the RPC what function that they want to call. It is sent as an integer to save some space in the RPC packet (as it is part of every RPC), so if you send the number `42` when calling the RPC the receiver will know that if the number is `42` it should call `FunctionX`. This is generally handled with a `switch` in the `OnRPC` function since objects generally have more than one RPC.
```cpp
modded class PlayerBase
{
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		switch (rpc_type)
		{
			case 42:
			{
				Param1<int> params42;

				if (!ctx.Read(params)) {
					return;
				}

				FunctionX(params42.param1);
				break;
			}
			case 133:
			{
				Param3<int, float, string> params133;

				if (!ctx.Read(params)) {
					return;
				}

				FunctionY(params133.param1, params133.param2, params133.param3);
				break;
			}
		}
	}
}
```
For readability you should probably create some integer constants to compare with, it makes way more sense for humans to see `case MyRpcConstants.FunctionX:` instead of `42`. The naming scheme and all that is up to you.
## Parameter 3: notnull array<ref Param> params
In short this is the parameter data that should be sent alongside with your RPC. Almost all types of data can be sent this way, even object references (as long as they exists on both server and client). There is probably some limit to how much data you can send with a RPC but I haven§t run into any issues at all with...yet. The params array has already been shown off in the introduction but we can expand it a bit.
```cpp
modded class PlayerBase
{
	private float m_memberFloat = 420.69;

	void SendLotsOfData(ZombieBase someRandomZombie)
	{
		auto sendData = new array<ref Param>();

		// Data like this is just copied into the parameter array, very straightforward.
		// Both constant values and variables can be added to the parameters.
		sendData.Insert(new Param4<int, float, string, bool>(1234, m_memberFloat, "some string", false));

		// EntityAI can also be sent and if it also exists on the receiving end DayZ will fill this out with
		// the correct data before OnRPC is called. I really like that it works like that and it's not a given
		// in all game engines.
		// PlayerBase of course derives from EntityAI so we can just send 'this' as a value.
		sendData.Insert(new Param1<PlayerBase>(this));

		// We can also send references to other entities than ourselves and it will just work.
		sendData.Insert(new Param1<ZombieBase>(someRandomZombie));

		GetGame().RPC(null, 12345, sendData, true);
	}
}
```
The big thing to remember is to always input data in the same order as you read it in the `OnRPC` function.
## Parameter 4: bool guaranteed
Unfortunately this one isn't as documented but the name kind of clues us into thinking that we might be able to selectively drop RPCs if there are already a lot of network data to send. This is very rarely set to `false` in the DayZ code and each time it only seems to be related to debug code. You should probably leave this as `true` unless you know why you wouldn't, I sure don't know why you would.
## Parameter 5: PlayerIdentity recipient
RPCs that are sent to clients are sent to ALL relevant clients by default, with relevant meaning "if they are within the network bubble of the target object" (parameter 1). Sometimes this is not desireable and you might only want to send an RPC to a specific subset of players which this lets you do. The DayZ explanation mentions that it increases security and reduces network traffic, which is true, this is probably something you look into more when you start working with anti-cheat solutions. The risk with using recipients is that you have to make the RPC call more complex (by figuring out each recipient) and introduce a risk for desyncs if you do it incorrectly, it's probably defaulted to sending to everyone for that reason. If you are starting out with RPCs this shouldn't be your first worry.

The `PlayerIdentity` object can be found in all player objects by calling `GetIdentity()`, you can probably just set a recipient on the server (untested, I have no idea if this is the case, but it should be). A forced example of this would be the following:
```cpp
modded class MissionServer
{
	void SomeSecretGamePlayEvent(vector secretLocation)
	{
		foreach (auto player : players)
		{
			// Sends the secret location only to bleeding players within a 1km radius of the secret location.
			if (vector.Distance(player.GetPosition(), secretLocation) < 1000 && player.IsBleeding())
			{
				auto sendData = new array<ref Param>();

				sendData.Insert(new Param1<vector>(secretLocation));

				GetGame().RPC(player, 12345, sendData, true, player.GetIdentity());
			}
		}
	}
}
```
The main difference with the first parameter, `Object target`, is that RPCs sent to an EntityAI will be called on all clients where this EntityAI is present. This will prevent the RPC from ever getting to the client and you can therefore use it for hiding information from players that are in the network bubble of each other. There is probably a better way of explaining this, but hopefully you will understand, understanding this is not super important when you are starting out.
# The `rpc_type` issue with mods
Something you will run into almost instantly is that you will accidentally use a number for `rpc_type` that either DayZ or another mod is already using. This is just an unfortunate reality that you have to deal with in some way. If you are using the [CommunityFramework](https://github.com/Arkensor/DayZ-CommunityFramework/blob/production/docs/RPC/index.md) they should have an out of box solution for handling multiple mods sending RPCs and seeing how popular that is you should most likely try to use that first. It's way easier to start with that solution than to modify all your RPCs later on when you realise that some random mod that 7 servers in the world use also use the same `rpc_type` value. RPC issues like these are hard to diagnose so taking the time to do it properly from the start is worth it.

If you cannot use the CommunityFramework for whatever reason another way is to use the same value for `rpc_type` for all RPCs and then send data for what function to call as parameters. This is essentially what the CommunityFramework [does](https://github.com/Arkensor/DayZ-CommunityFramework/blob/production/JM/CF/Scripts/3_Game/CommunityFramework/RPC/RPCManager.c) but it does this dynamically to work with multiple mods. So all RPCs get the `rpc_type` value `10042` and then the first parameter is a pair of strings that tells the CommunityFramework what mod it relates to and what function to call in that mod. We can replicate the same behaviour by choosing a value that's probably not taken by any other mod, surely no one else would take the number 34585046! (I use 183183 for ZeroRevival, please do not steal).
```cpp
class MyModRpcs
{
	// You should define all your RPCs in the same place to make sure that you don't get collisions.
	// Using an enum would guarantee no collisions, but the actual integers used would change if the enum changes
	// and I'm not too sure if that's good.
	const int PRINT_TO_LOG = 1;
	const int INCREMENT_KILLS = 2;
}

modded class PlayerBase
{
	protected int m_kills = 0;

	// Select a value that's probably not taken by anyone else in the world.
	const int MY_MOD_RPC_TYPE = 34585046;

	void SendSomeDataAsRpc()
	{
		auto sendData = new array<ref Param>();

		// Send what function we want to call as the first param.
		sendData.Insert(new Param1<int>(MyModRpcs.PRINT_TO_LOG));
		sendData.Insert(new Param3<int, float, string>(1337, 420.69, "My String"));

		GetGame().RPC(this, MY_MOD_RPC_TYPE, sendData, true);
	}

	void IncrementKills()
	{
		auto sendData = new array<ref Param>();

		// Send what function we want to call as the first param.
		sendData.Insert(new Param1<int>(MyModRpcs.INCREMENT_KILLS));

		GetGame().RPC(this, MY_MOD_RPC_TYPE, sendData, true);
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		if (rpc_type == MY_MOD_RPC_TYPE)
		{
			// If the 'rpc_type' corresponds with the one we chose for our mod, we then parse the first param to figure out what to do.
			Param1<int> functionParam;

			if (!ctx.Read(functionParam))
			{
				Error("Failed to parse what function we want to call!");
				return;
			}

			switch (functionParam.param1)
			{
				case MyModRpcs.PRINT_TO_LOG:
				{
					Param3<int, float, string> params;

					if (!ctx.Read(params)) {
						return;
					}

					Print(string.Format("Got RPC, Integer Data '%1' Float Data '%2' String Data '%3'", params.param1, params.param2, params.param3));
					break;
				}
				case MyModRpcs.INCREMENT_KILLS:
				{
					++m_kills;
					break;
				}
			}
		}
	}
}
```
# Atashi's preferred RPC pattern
This part is highly opinionated and you are free to completely ignore it, but I want to share how I do RPCs in ZeroRevival since I believe a real world example could help someone out. The solution under the `rpc_type` does mean that you will end up with a bunch of game logic inside the OnRPC function and I prefer to move execution out from that function as soon as possible. As an example I will use the code that plays the helicopter crash site audio, which is somewhat similar to how the vanilla game does it. The problem we have to solve is that if a helicopter crashes outside the network bubble of the player any sound or visuals would not be present on that client, but DayZ has a function to play a sound at a location in the world so we will use that.

I generally write my RPC functions in a way that they can be called on both servers and clients but they essentially have different execution paths depending on the platform. The server path generally gathers up the data required and then sends an RPC, the client will then parse that data in the `OnRPC` function and call the same function on the client. It's not foolproof but this means that we can just call `GetDayZGame().HelicopterCrashStart(GetPosition())` inside the crashsite helicopter object and be kind of sure that the correct thing happens. I feel like this pattern creates pretty readable code from where we call the RPC function and it keep the OnRPC function as small as possible, having the server code in each function probably does waste a bunch of lines but it's pretty obvious what's going on. You are free to disagree! And yes, I know that the two functions are literally doing the same thing, but I will probably change the sound that plays when the crash starts or finishes...sometime soon...for sure.
```cpp
modded class DayZGame {
	const float CLOSE_SOUND_DISTANCE = 800.0;
	const float DISTANT_SOUND_DISTANCE = 2000.0;

	void HelicopterCrashStart(vector location) {
		if (GetGame().IsServer()) {
			// Gather up the data and send it as an RPC to all clients.
			auto sendData = new array<ref Param>();

			sendData.Insert(new Param1<int>(ZeroRevivalRPCs.STC_HELICOPTER_CRASH_START));
			sendData.Insert(new Param1<vector>(location));

			GetDayZGame().RPC(null, ZRConstants.ZERO_REVIVAL_RPC, sendData, true);
		} else {
			// When we then end up on the client we should instead play a sound at the location we sent.
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
					Param1<vector> crashStartLocation;

					if (!ctx.Read(crashStartLocation)) {
						return;
					}

					HelicopterCrashStart(crashStartLocation.param1);
					break;
				}
				case ZeroRevivalRPCs.STC_HELICOPTER_CRASH_FINISHED: {
					Param1<vector> crashFinishLocation;

					if (!ctx.Read(crashFinishLocation)) {
						return;
					}

					HelicopterCrashFinished(crashFinishLocation.param1);
					break;
				}
			}
		}
	}
}
```
