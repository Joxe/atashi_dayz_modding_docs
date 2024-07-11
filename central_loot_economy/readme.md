# Introduction
The Central Loot Economy (CLE from here on) is one of the major additions in DayZ compared with the Arma mod and it aims to spread out loot somewhat evenly on the thousands of loot points on the map. The core idea is that the CLE has a huge list of items and a value that decides how many of those items that should be on the server at all times. If players don't pick up items they stick around until their lifetime ends and if items are consumed new items will take their place. The CLE also takes care of dynamic events like police car and helicopter crashes and makes sure that there's a set amount of vehicle on the map. It also contains the zones in which zombies and animals spawn and takes care of spawning and despawning those.

To control the CLE there are a bunch of XML files located in the mission directory (`\DayZServer\mpmissions\dayzOffline.chernarusplus` by default) and we will go through them all in detail but we first need to establish some concepts. The CLE divides the map into Tiers ranking from 1 to 4 with 4 being the highest tier loot. In each of these tiers on the map there are buildings and each building has its own set of spawn points set and a category set for that building type. So a barracks building in Tisy and one in Berezino will spawn different sets of loot but they will be physically placed on the same spots inside the building. Spawning things outside of houses is possible but they still need to be linked to some sort of static object with defined spawn points, this is how the crashed cars all over the map spawn items. Simply spawning an item in the middle of a field is not possible unless you get creative with invisible houses or static objects.

The exact piece of loot that spawns is not decided by any controllable chance and all of this seems to happen inside the engine meaning that it's not possible (to my knowledge) to change how the CLE selects items and loot positions. Making weapons spawn more often, for example, can then only be done by modifying the amount of weapons that are available on the map as a whole and which tiers or categories they exist in. Simply doing something as simple as doubling the amount of M4s that should exist doesn't necessarily mean that players will find them *that* more often due to the sheer amount of loot spots that exist. The best way to modify the CLE depends on the goal you want to achieve and I hope to give you an introduction to the tools available so you can make the changes that you deem necessary. To make it even more difficult any changes that you do make to the CLE data will not instantly be reflected on the server or it might take a while until the economy "settles" with old items being removed and new ones added, testing loot changes is best done by constantly resetting the server persistence and inspecting the results. Sometimes the CLE is not fit for purpose for spawning items the way you want so creating your own loot systems might be necessary if you want to do major changes.
# Modify item occurance
The first thing that I expect most people want to do is to modify how often an item appears in the world. This is primarily done by modifying the `nominal` value inside `\DayZServer\mpmissions\dayzOffline.chernarusplus\db\types.xml`, lets look at an example:
```xml
<type name="M16A2">
	<nominal>7</nominal>
	<lifetime>28800</lifetime>
	<restock>0</restock>
	<min>4</min>
	<quantmin>30</quantmin>
	<quantmax>80</quantmax>
	<cost>100</cost>
	<flags count_in_cargo="0" count_in_hoarder="0" count_in_map="1" count_in_player="0" crafted="0" deloot="0"/>
	<category name="weapons"/>
	<usage name="Military"/>
	<value name="Tier3"/>
	<value name="Tier4"/>
</type>
```
The full breakdown of this piece of code can be found in the [types.xml documentation](db/types.md) but for now we can see the line `<nominal>7</nominal>` which is telling the CLE to attempt to keep 7 instances of the M16A2 on the map. If we increase this to 20 it will start spawning more of them until it reaches 20 instances, if we change the value to 0 it will no longer spawn the M16A2 at all. Setting the value to a lower value than what exists on the map does not mean that the CLE will go around and delete them. Items also store their remaining lifetime in the server persistence so setting the lifetime to `0` won't delete any existing items, and nothing you do can make the CLE delete items from players. This reality means that deleting items that once spawned might be a bit difficult and you probably have to do some scripting to make sure it happens (like cleaning up player's inventory upon login).

Taking all this into account this should at least let you steer the CLE towards the kind of loot you want on your server.
# Resetting the server
The easiest way to reset the server is to simply delete or move the `\DayZServer\mpmissions\dayzOffline.chernarusplus\storage_1` directory. There are probably ways of doing resetting only selected parts of the persistance but I haven't tried it. With the `storage_1` directory out of the way the server will be completely fresh and the CLE will do an initial sweep all over the map and spawn all the items it thinks should be there. This lets you do rapid testing with different loot settings and see the result instantly (bar any dynamic events that might happen later in the server lifetime).
# Play around!
The most important part about the CLE is that you should play around with values, make use of the fact this is data-driven development. You don't have to know how to mod script or mod in general to make quite drastic changes to you server. I will try to include a readme file for each XML file that explains how they work (to the best of my knowledge) and from there it's really up to you.