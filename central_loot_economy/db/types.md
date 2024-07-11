# Introduction
The types XML an entry for all objects that should be take into account by the Central Loot System, it's a long list of entries that all look kind of the same, let's break it down.
```
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
A lot of element descriptions are taken from [this thread on the DayZ forums](https://forums.dayz.com/topic/247502-typesxml-need-explanation/) alongside my own experience, thanks to everyone who helped out in that thread!
```
## type
The first line of `<type name="M16A2">` tells the CLE what object to spawn. It should correspond exactly with the class name in either DayZ config files or any item you have added through a mod. So in this case the CLE knows that it is targeting this class in `\DZ\weapons\firearms\m16a2\config.cpp`:
```
class M16A2: M16A2_Base
{
	scope=2;
	displayName="$STR_M16A2_0";
	descriptionShort="$STR_M16A2_1";
};
```
## nominal
Nominal acts like the target amount for the CLE for each item, in this case the CLE knows that it should try to keep 7 instances of the M16A2 on the map at all times.
## lifetime
The amount of seconds until an item that hasn't been interacted with by another player is cleaned up by the CLE. This value makes items change even on empty servers or cleans up unwanted items so the same ugly shirt isn't on the same place forever.
## restock
According to the previously linked thread this should be the amount of seconds before the CLE starts restocking items that disappear from the world, so if we set this to 60 it would take a minute before a new M16A2 would be added.
## min
The minimal amount of instances that should exist on the server, the nominal value is a bit loose so if there's no free loot locations (unlikely) it just won't spawn items, the `min` value will try to force it to spawn the set amount and actually output an error in the server log when it's unable to find a spot for the item.
## quantmin and quantmax
On a high level this modifies the quantity of the item spawned, what quantity means depends on the item. For magazines it means the number of bullets in them and for a bottle of water it reflects the amount of water it will contain. These two are percent values so this means that you read it as 30% and 80%.
## cost
Unknown! It's set to `100` on every item so I guess that's what it should be? The user 'aux7' in the thread on the DayZ forums speculates that it might be some priority thing but without tools to debug the CLE it's very hard to spot any difference from changing this value.
## flags
Modifies how the CLE treats the nominal value. I have not really played around with these so this is mostly explanations from the forum thread linked earlier.
### count_in_cargo
The CLE will count instances in cargo. This mostly affects items that are in vehicles and whatnot but technically all items with an inventory might be treated as "cargo" so this might apply to protective cases and backpacks too. 
### count_in_hoarder
This flag is a bit more divided, the earlier mentioned thread just says "Infected" as explanation but from what I can find on other sources ([1](https://steamcommunity.com/app/221100/discussions/0/1640927348828219479/) [2](https://dzconfig.com/wiki/types)) this should be whether it's including instances in long-term storage items like tents. I don't know where they define what items are "cargo" and what is "hoarder".
### count_in_map
Whether or not the CLE should take instances in the map into account for the nominal value. In general this is set to `1` for everything you want to spawn in the world and `0` for items that aren't supposed to spawn.
### count_in_player
Whether or not to count the item instances in Player's inventory, if set to 1 the CLE won't spawn new instances even if players pick up existing ones. DayZ seems to set this to `1` for high-tier items like rifles and huge backpacks. What I don't know is if this also counts offline player's inventory or not.
### crafted
As the name suggests this is set to `1` for all the crafted items in DayZ, that said, I am not completely sure what setting this flag *actually does*. I have personally spawned a bunch of crafted items on a server and that seems to work just fine? Maybe it's best to set this to `0` if you intend to make an otherise crafted item able to spawn from the CLE.
### deloot
Short for 'dynamic event loot'. Again, it's difficult to understand what changing the value does but I would expect this to only use the nominal value when spawning loot for dynamic events like helicopter crashes and contaminated zones and otherwise it will not spawn the items at all. I would expect this to be mutually exclusive with `count_in_map` but sometimes DayZ exceeds my expectations so it might be counting them both?
## category
Category tells the CLE what group of item spawn points inside buildings to use, this will be further explained in the [mapgroupproto](../mapgroupproto.md) and [cfglimitsdefinition](../cfglimitsdefinition.md) but the short explanation is that this value helps the CLE to put weapons on some spawn points inside a building and backpacks on another.
## usage
Similar to category in that it helps the CLE put items in the correct place inside buildings but `usage` can almost be seen as a which tells which types of buildings an item should be able to spawn in. For example, when set to `Military` the item will spawn inside all buildings that are able to spawn military loot, when set to `Police` you will see the item show up in police stations around the map, and so on. Multiple values can be set for a single item so you can have an item show up in both police stations and military if you want to. Inside the [mapgroupproto.xml](../mapgroupproto.md) file you can see (and modify) each building's usage group. Buildings are able to have multiple groups to be able to spawn multiple kinds of loot.
## value
Primarily used to restrict items to a specific loot tier on the map, you can use multiple values here so making an item spawn in both Tier 2 and 3 areas is done by simply writing them both on an item.
# Preventing an item from spawning
Setting an item `nominal` to 0 will stop new instances from spawn but won't remove existing ones, if you modify a bunch of items at once it's probably best to do it all in one go and then reset the persistence on the server or your players will keep the items that you want to remove in their inventories.
# Spawning more weapons
Increase the nominal on all weapons that you want to find. A lot of military grade weapons only spawn in Tier 3 and 4 so if you want everything spawning on the coast you can simply remove the Tier entries. Remember that there are multiple thousands of loot spots in DayZ so if you actually want to see the weapons spawning everywhere you might need to modify the nominal value a lot. You might also need to modify the flags to make DayZ stop counting weapons in player's inventories if you want more weapons to exist on popular servers. It all depends on what kind of server you want to make.