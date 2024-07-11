# Introduction
The `cfglimitsdefinition.xml` file contains the definition of everything that the CLE use to place items in the world. The usage of each value in this file can be found all over in the other files that are related to buildings and items, but let's go through them all.
## categories
The name of the categories for loot spawn points inside buildings. These are used to categorise the spawn points so the CLE knows which ones that are for spawning weapons, which ones that spawn tools, and so on. In the [types.xml](db/types.md) file you can see that each item that is meant to spawn in the world has at least one loot category and in [mapgroupproto.xml](mapgroupproto.md) you can see that all buildings have these categories assigned to groups of spawn points. This way you can prevent items from spawning on certain places inside a building (like car doors on shelves). To my knowledge (which isn't saying a lot) these are just arbitrary strings that are matched together so you are free to add new ones if you feel like it.
## tags
In [mapgroupproto.xml](mapgroupproto.md) these appear on some categories and I think these are a similar system to categories, we can see some items in [types.xml](db/types.md) with the tags and they are also present in [cfgspawnabletypes.xml](cfgspawnabletypes.md). The exact effect the tags have on the spawning is unknown to me so if you know gladly inform me.
## usageflags
The level above `category`, this helps you decide what type of loot that can spawn in each building. Almost every item in [types.xml](db/types.md) contains at least one `usage` flag and every building in [mapgroupproto.xml](mapgroupproto.md) has at least one assigned. You are free to add your own and the game will understand it, simply create a new entry in this list, add it to a building and then add it to an item.
## valueflags
Defines the different loot tiers in the world and you can find any combination of these on a bunch of items in [types.xml](db/types.md) to make better items only spawn further away from spawn points. I don't think you can modify this part too much since the tier system seems to be controlled by the engine. To my knowledge there is no way of adding a fifth tier of loot, for example.
# Example: Make food only spawn in stores
This is modification that I have in ZeroRevival to make the store buildings way more valuable, which of course is a design choice but I think it can be used as an example of making changes to a few files to see a wildly loot in-game. First off we add a new `usageflags` entry in `cfglimitsdefinition.xml` for our store:
```xml
<usageflags>
	<usage name="Store"/>
	<usage name="Military"/>
	<usage name="Police"/>
```
This creates the definition and we are now able to use it in the two other relevant files. First off we go to [types.xml](db/types.md) and modify any item to now only spawn in the `Store` usage flag. For our example I am using the `SpaghettiCan` but this is repeated on all good types of food and drinks in my mod.
```xml
<type name="SpaghettiCan">
	<nominal>20</nominal>
	<lifetime>14400</lifetime>
	<restock>0</restock>
	<min>15</min>
	<quantmin>-1</quantmin>
	<quantmax>-1</quantmax>
	<cost>100</cost>
	<flags count_in_cargo="0" count_in_hoarder="0" count_in_map="1" count_in_player="0" crafted="0" deloot="0"/>
	<category name="food"/>
	<tag name="shelves"/>
	<usage name="Store"/>
	<value name="Tier2"/>
	<value name="Tier3"/>
</type>
```
The can of spaghetti normally has the usage flags "Town", "Village" and "Office" but by removing those and adding "Store" as the only one we should be able to limit the spawning to the type of building we want. Next up we go to the [mapgroupproto.xml](mapgroupproto.md) file and look for our store buildings. DayZ contains four types of stores; `Land_City_Store`, `Land_City_Store_WithStairs`, `Land_Village_store` and `Land_Village_Store_Enoch` with the latter only appearing on Livonia. The `Land_City_Store` (which is the old store from Arma 2, big windows at the front and lots of shelves in the room behind the counter) is normally able to spawn a bunch of items due to its many categories, which we can see from the entry:
```xml
<group name="Land_City_Store" lootmax="25">
	<usage name="Town" />
	<usage name="School" />
	<usage name="Office" />
	<usage name="SeasonalEvent" />
	<!-- rest of entry cut to save space -->
```
Since I want stores to be hot spots for loot I have removed all but `Town` and added the `Store` usage flag:
```xml
<group name="Land_City_Store" lootmax="25">
	<usage name="Town" />
	<usage name="Store" />
	<usage name="SeasonalEvent" />
	<!-- rest of entry cut to save space -->
```
This is then repeated for the other stores in the game and with these changes the can of spaghetti can now only spawn inside stores!