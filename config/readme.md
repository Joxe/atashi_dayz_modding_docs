# Introduction
All objects that can spawn in DayZ has an entry in a file called `Config.cpp` which contains data that both server and client can read. This lets the game know that whey you spawn `ObjectXYZ` it should have a certain model, how large its inventory is and what animations it should have and make sure that those are same on both server and client. When packaging a mod the Addon Builder will look for all files named `Config.cpp` and add them to your mod so the items within can be spawned. The configs themselves are a huge tree of inheritance and they are written like C++ classes, let's take a look at an example from the DayZ code base:
```cpp
class CfgPatches
{
	class DZ_Weapons_Magazines
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};

class CfgMagazines
{
	class DefaultMagazine;
	class Magazine_Base: DefaultMagazine
	{
		scope=0;
		debug_ItemCategory=4;
		storageCategory=1;
		inventorySlot[]=
		{
			"magazine",
			"magazine2",
			"magazine3"
		};
		repairableWithKits[]={1};
		repairCosts[]={25};
		handheld="true";
		armAction="Disarm";
		isMeleeWeapon=1;
		rotationFlags=17;
		quantityBar=0;
		itemSize[]={1,2};
		absorbency=0;
		recoilModifier[]={0.94999999,0.94999999,0.94999999};
		value=1;
		modelSpecial="";
		spawnDamageRange[]={0,0.60000002};
		enlargeInventoryView=0;
		useAction=0;
		useActionTitle="";
		manipulationDamage=0.050000001;
		count=30;
		ammo="";
		ammoItems[]={};
		weightPerQuantityUnit=8;
		varQuantityInit=1;
		varQuantityMin=0;
		varQuantityMax=1;
		selectionFireAnim="zasleh";
		nameSound="magazine";
		soundUse="craft_rounds";
		// Cut to save space
		class DamageSystem {};
		class MeleeModes
		{
			class Default
			{
				ammo="MeleeFist";
				range=1;
			};
			class Heavy
			{
				ammo="MeleeFist_Heavy";
				range=1;
			};
			class Sprint
			{
				ammo="MeleeFist_Heavy";
				range=2.8;
			};
		};
		class Reliability
		{
			ChanceToJam[]={0,0.001,0.0099999998,0.050000001,1};
		};
		soundImpactType="metal";
	};

	class Mag_CZ550_10rnd: Magazine_Base
	{
		scope=2;
		displayName="$STR_CfgMagazines_Mag_CZ550_10rnd0";
		descriptionShort="$STR_CfgMagazines_Mag_CZ550_10rnd1";
		model="\DZ\weapons\attachments\magazine\magazine_cz550_10rnd.p3d";
		weight=220;
		weightPerQuantityUnit=10;
		itemSize[]={1,2};
		count=10;
		ammo="Bullet_308Win";
		ammoItems[]=
		{
			"Ammo_308Win",
			"Ammo_308WinTracer"
		};
		// Cut to save space.
		class DamageSystem {};
		class AnimEvents {};
	};
};
```
This is a part of `\DZ\weapons\attachments\magazine\config.cpp` and if you have seen classes like in C++ before this is nothing new, in general you can see each class as class definition and constructor definition at the same time. You both say that the type `Mag_CZ550_10rd` exists, that it inherits from `Magazine_Base` and on construction it should set `scope` to `2`, `displayName` to the string table entry `$STR_CfgMagazines_Mag_CZ550_10rnd0` and so on.

But we will of course go through each part of this piece of code so even normal people that doesn't have a background in C++ can get an understanding on what's going on. Let's take it from the top.
## CfgPatches
With almost any mod you will end up with potentially *a lot* of `Config.cpp` files in various directories (at the time of writing I have 11 in ZeroRevival) and each of them needs to define a `CfgPatches` at the start with a single class that should can be arbitrarily named. Creating this class makes the `Config.cpp` into an add that can be referred from other files in the `requiredAddons` field, DayZ will also tell you on start up if one of your addons is missing one of its requirements. Unknown speculation following: I guess this could potentially let you read items from other mods and modify them, but I am not sure if that's the case, I have only used them to modify DayZ items. In general this is not something that I know a lot about but just copy-pasting it between files and changing the class name seems to work, if you know more I would love to know.
```cpp
class CfgPatches
{
	class DZ_Weapons_Magazines
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};
```
### units
The webpage [DayZ Modding](https://dayzmodding.dev/en/TheConfigCPP) described this as "You can make a list of your items in units[]=" but I have never personally used this and it's sporadically used in the DayZ code base so I don't really know what this does. Looking through the DayZ code they sometimes add a list of strings that seems to correspond to all the items in the config itself (`DZ\characters\backpacks\config.cpp` does this) and sometimes it's just empty (`DZ\characters\belts\config.cpp` is an example of that. Just leaving it empty doesn't seem like it's necessarily *wrong*, but I would love to know what this *actually* does.
### weapons
This is left empty in every `config.cpp` I have ever seen, it's empty in all the configs in the DayZ code base, it's probably some old Arma thing so leave it empty or omit it completely. If you know what it does, please tell us so we can add some trivia.
### requiredVersion
This is set to `0.1` in every `config.cpp` file in the entire DayZ code base, just like the `weapons` member this might be a remnant from Arma or used in Bohemia's internal development to avoid creating items that shouldn't exist yet. Just follow the precedence and set this to `0.1` until you know why you shouldn't. If you know what it does, please tell us.
### requiredAddons
Requried Addons is actually used, it will make sure that the addons specified are also loaded. This is to make sure that the items you override or use in your configs *actually* exists in the game. If you are unsure you can probably set this to only include `DZ_Data` as it makes sure that most things in DayZ is loaded but you can specify it to only require a specific config if you want. What's important is to set this correctly if you are cross-referencing objects in your own mod files or between mods and in those cases you simply write the class name in the list of required addons, using the example you would write `DZ_Weapons_Magazines` if you want to make sure the magazines are present if you want to modify any magazines.
## CfgMagazines (or Cfg\<Blank\>)
The next part is where we add our object definitions, in the example this is `CfgMagazines` but you will commonly see `CfgVehicles`, `CfgNonAIVehicles`, `CfgAmmo`, and many more. When writing configs that override existing objects you are fine just copying the setup that DayZ is using, when creating new objects the class you should use depends a lot on what you want to create. One thing that might not be very obvious is that all clothing in the game exists within `CfgVehicles`, the reason for this is probably some legacy design thing that you just have to know.
## Forward declarations
The part of the code that says `class DefaultMagazine;` should be apparent to any C++ programmer as a forward declaration and they function in the same way. For anyone else this is a way of telling the Addon Builder that "there will exist a class called `DefaultMagazine` somewhere, but I don't want to make any changes to it" which lets you extend `DefaultMagazine` with the `Magazine_Base` class. If you run into the error `Undefined base class 'DefaultMagazine'` you probably missed a forward declaration somewhere.
## Object definition
With all of the previous things out of the way we can now actually start configuring objects. In the example from DayZ they define both `Magazine_Base` and `Mag_CZ550_10rnd` in an inheritance tree, with the former being used as a base for all magazines in the game. `Magazine_Base` itself is inheriting from `DefaultMagazine` and modifying it somewhat, and the fact that `DefaultMagazine` is located inside `CfgMagazines` helps DayZ understand what kind of item it is. Whenever you spawn items in the game you will generally spawn the objects that are furthest down in the inheritance tree (I like to call these the "leaves"), with the example in mind this means that you would be fine spawning `Mag_CZ550_10rnd` into the world but not `Magazine_Base` or `DefaultMagazine`. The reason for this is going to be a bit more apparent when we talk about how configs interact with scripts later on.
# Making changes to objects
If you are modding this is probably the most interesting part of the document, making data modifications to existing items is actually very simple. As an example we will modify the magazine some.
First off, inside your mod directory create a new file called `Config.cpp` (not case sensitive) and add a new `CfgPatches` class at the top.
```cpp
class CfgPatches
{
	// Name is up to you, but it needs to be unique within your mod.
	class BigMag_Magazines
	{
		requiredAddons[]=
		{
			// Either use "DZ_Data" or "DZ_Weapons_Magazines" to get the base definitions that you need.
			"DZ_Data"
		};
	};
};
```
Just under that we then create our own `CfgMagazines` class, making sure to include any forward declaration that we require. Any changes we want to do should be added to this new class definition, anything not assigned here will be unmodified from the default `Mag_CZ550_10rnd`.
```cpp
class CfgMagazines
{
	class Magazine_Base;
	class Mag_CZ550_10rnd : Magazine_Base
	{
		// Count in magazines is the amount of ammo they can contain.
		count=100;
		// Modifying the item size makes the item larger in the inventory.
		itemSize[]={3,4};
	};
};
```
Once packaged into a mod any CZ550 magazine will now be really large and be able to contain 100 rounds of ammunition. A word of warning is that I have no clue what happens if you install two mods that modify the same values in different ways, it probably is down to mod load order and things like that. To make sure that the modifications that you do don't collide with other mods you can of course take the next option, which is to create new items altogether.
# Creating new objects
Creating new objects is very similar to modifying them but it comes with some extra requirements. The config file looks very similar to the item modification example but instead of modifying `Mag_CZ550_10rnd` we inherit from it and make our changes.
```cpp
class CfgMagazines
{
	class Mag_CZ550_10rnd;

	// Class name is completely up to you.
	class Cooler_Mag_CZ550_10rnd : Mag_CZ550_10rnd
	{
		displayName="Cooler CZ550 10rnd Mag";
		itemSize[]={3,3};
		count=90;
	};
};
```
This is the first part we need to do to make items available in game, the second part is that we need to define this class inside the script files of the mod. Since we currently don't have any special logic for the magazine we only really need to make an empty class declaration.
```cpp
class Cooler_Mag_CZ550_10rnd : Mag_CZ550_10rnd {}
```
Since the magazine is using the `4_World` script module your definition should be somewhere in there. I have added an example mod of this in the `BigMagExample` directory but you should read through the modding basics if you are having trouble. Once the magazine is also in the script files you are ready to actually spawn it into the world, this can be done at any point in either `4_World`, `5_Mission` or even the `init.c` file that the mission use to create new characters.
```cpp
// Example from init.c, but the code can practically be put anywhere.
class CustomMission: MissionServer
{
	override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{
		player.GetInventory().CreateInInventory("Cooler_Mag_CZ550_10rnd");
	}
}
```
If you are using the [CommunityFramework](https://steamcommunity.com/sharedfiles/filedetails/?id=1559212036&searchtext=communityframework) it will also pick up on the new item in its object spawner module and let you spawn it wherever you want.
# Configs and Scripts relationship
I skimmed over this relationship a bit when writing about creating new objects but this is actually kind of important and somewhat cool. Each object you create will look for a script class with the same name and automatically assign that script class to itself upon creation. This lets you write logic for your objects and allows typecasting in your mod's script files, it lets you overwrite the behaviour of objects with your own game logic.

Of course, we should create an example for this. If we take a look into the file `\Scripts\4_world\entities\itembase\magazine\magazine.c` in the DayZ script files you will find the class `Magazine` which contains almost all the functions that a magazine has. As a simple example we can take a look at the `GetChanceToJam()` function which in its default state looks like this:
```cpp
float GetChanceToJam()
{
	int level = GetHealthLevel();
	if (level >= 0 && level < m_ChanceToJam.Count())
		return m_ChanceToJam[level];
	else
		return 0.0;
}
```
In short it gets the health level of the magazine and then uses that to look up the chance to jam in the `m_ChanceToJam` array. If we decide that our cooler magazine shouldn't use the same logic as other magazines we are able to override the behaviour of it in our own class.
```cpp
class Cooler_Mag_CZ550_10rnd : Mag_CZ550_10rnd
{
	// 'override' tells DayZ that we want to create new logic instead of what's already there.
	// When overriding the function name and parameter names (if any) need to be exactly the same
	// as the original.
	override float GetChanceToJam()
	{
		float randomNumber = Math.RandomFloat01();

		// A constant 10% chance to jam, probably worse from a gameplay point of view.
		return randomNumber <= 0.1;
	}
}
```
While this example is pretty forced this does let you override a lot of game objects and make them work in the way you want. You can also add new functions that normal Magazines doesn't have and call them via the use of typecasting. To show of that functionality we will create a new function called `FillMagazine()` which does just that.
```cpp
class Cooler_Mag_CZ550_10rnd : Mag_CZ550_10rnd
{
	// Since this is a new function, we don't use the 'override' keyword at the start
	void FillMagazine()
	{
		// As the name suggests, this only works if you call it on a server.
		ServerSetAmmoCount(GetAmmoMax());
	}
}
```
When you spawn the magazine you have to cast it to the correct type using either `Cooler_Mag_CZ550_10rnd.Cast(object)` or `Class.CastTo(object, target)`, I prefer the former but either one is fine. Casting in DayZ works like dynamic casting in C++ meaning that if the object that you are trying to cast from does not match what you are trying to cast to it will simply return `null`. You can read the casting code as "Is this item I am spawning a `Cooler_Mag_CZ550_10rnd`?" and if it is you will have a variable to use with that type.
```cpp
class CustomMission: MissionServer
{
	void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{
		Cooler_Mag_CZ550_10rnd coolerMag = Cooler_Mag_CZ550_10rnd.Cast(player.GetInventory().CreateInInventory("Cooler_Mag_CZ550_10rnd"));

		if (coolerMag != null)
		{
			coolerMag.FillMagazine();
		}
	}
}
```
If everything works correctly with this code in the `init.c` file of your mission you should spawn with your new magazine and it should be full of ammo.
# Configs and Central Loot Economy relationship
Any objects that you create are also available for the Central Loot Economy to use which lets you rather seamlessly include the items you create into the normal loot system of DayZ. This is true for both items that you find in the world and dynamic events. All the Central Loot Economy files can be found under the DayZ server mission directory, by default it should be something like `\DayZServer\mpmissions\dayzOffline.chernarusplus\` but as an example we will just add our new magazine to also spawn in the world. The Central Loot Economy should have its own set of tutorials and documentation and we won't go too deep into it here.

Start off by opening the file `\DayZServer\mpmissions\dayzOffline.chernarusplus\db\types.xml`, you will see the data that the CLE uses to spawn items in the world in this massive file. But for now we should search for the existing magazine and see how its entry looks.
```xml
<type name="Mag_CZ550_10Rnd">
	<nominal>8</nominal>
	<lifetime>14400</lifetime>
	<restock>0</restock>
	<min>5</min>
	<quantmin>10</quantmin>
	<quantmax>100</quantmax>
	<cost>100</cost>
	<flags count_in_cargo="0" count_in_hoarder="0" count_in_map="1" count_in_player="0" crafted="0" deloot="0"/>
	<category name="weapons"/>
	<usage name="Hunting"/>
	<value name="Tier3"/>
	<value name="Tier4"/>
</type>
```
Again, the full details of what all of these things do should be a part of the CLE documentation, for now we should just copy this text and modify the `<type name="Mag_CZ550_10Rnd">` line to also spawn our new magazine.
```xml
<type name="Cooler_Mag_CZ550_10rnd">
	<nominal>8</nominal>
	<lifetime>14400</lifetime>
	<restock>0</restock>
	<min>5</min>
	<quantmin>10</quantmin>
	<quantmax>100</quantmax>
	<cost>100</cost>
	<flags count_in_cargo="0" count_in_hoarder="0" count_in_map="1" count_in_player="0" crafted="0" deloot="0"/>
	<category name="weapons"/>
	<usage name="Hunting"/>
	<value name="Tier3"/>
	<value name="Tier4"/>
</type>
```
And that is basically it, the CLE will now be able to spawn your magazine as long as the server is running the mod! Just make sure that the XML is valid since DayZ is pretty poor at telling you that a file looks broken, there should be plenty of XML validity checkers online if you aren't using a more feature rich text editor.
# The unfortunate reality of configs
Since config files set data both in script and engine objects it is sometimes very hard to figure out what variables that do exist, what variables that are used and what modifying them does. For example, there is a very obvious one called `model` that almost everything has which is the path to the model. But that sets the model of the object inside the engine so it does not correspond with any variable that you can just read in any script, you can get the name of it using `Object.GetModelName()`. In contrast the `count` variable we modified earlier *seems* to modify `m_Count` inside the class `ItemBase` meaning that it's a reused variable that all items have but Magazines use it as their maximum ammo count (judging by the `GetAmmoMax()` function in `Magazine`). And this just goes on and on, each variable has their own implementation and use case and the only way I have figured things out is by trial and error.

I will create [another file](config_members.md) that should contain my findings.