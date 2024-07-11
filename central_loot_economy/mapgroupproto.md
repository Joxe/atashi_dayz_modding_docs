# Introduction
The `mapgroupproto.xml` file contains the usage tags and loot position for each type of building in DayZ. Alongside the `usage` tag in [types.xml](db/types.md) this is what makes a Military Tent know that it should spawn military gear and that a farm building should spawn farm gear. As an example we will take a loot at the military grade Fire Station which you can find on the North-West Airfield since it shows off most of the things that you can do in this file.
```
<group name="Land_Mil_FireStation" lootmax="15">
	<usage name="Military" />
	<usage name="Firefighter" />
	<container name="lootFloor" lootmax="10">
		<category name="tools" />
		<category name="containers" />
		<category name="clothes" />
		<tag name="floor" />
		<point pos="-0.535154 0.037323 6.022705" range="0.479004" height="1.197510" />
		<!-- List of spawn points has been removed to save space -->
	</container>
	<container name="lootshelves" lootmax="12">
		<category name="tools" />
		<category name="containers" />
		<category name="clothes" />
		<category name="books" />
		<tag name="shelves" />
		<point pos="6.925292 0.881584 -10.102784" range="0.100000" height="0.250000" />
		<!-- List of spawn points has been removed to save space -->
	</container>
	<container name="lootweapons" lootmax="6">
		<category name="weapons" />
		<category name="explosives" />
		<point pos="4.152833 0.126587 4.024901" range="0.100000" height="0.250000" />
		<!-- List of spawn points has been removed to save space -->
	</container>
</group>
```
## group name="string" lootmax="number"
Each building has a name and a maximum amount of items that it can contain. Since the name is highly coupled with a model name this is probably nothing you will modify unless you are adding new buildings, `lootmax` can be modified freely, though, if you feel like there's too many/few items in a building. The `lootmax` variable won't necessarily make more items spawn in a building as it is driven by the Central Loot Economy but it could affect it for sure. If you want a building to stop spawning loot setting `lootmax` to `0` is probably the easiest way.
## usage
If you have read the [types.xml](db/types.md) documentation you will have already seen this variable in a lot of items and this is simply the second piece (or first piece if you read this before the [types.xml](db/types.md) file) of that puzzle. Adding `Military` as a usage group will make all items with the `Military` usage group able to spawn in this building. You can specify multiple usage groups per building just like the example, so you can totally make military loot spawn in hospitals without removing the medical items.
## container
Each build then has multiple "subcategories" that dictate what set of spawn points different items use, this is to avoid issues like car doors spawning on top of shelves instead of on the floor. The container name seems to be arbitrary and probably just have to be unique within the building. There is also an (seemingly optional) `lootmax` variable for `container` to control how much of each type of loot that will spawn in the building. Setting `lootmax` higher than the `group` `lootmax` probably doesn't make sense.
### category name="categoryName"
Again, the second piece of the `category` in [types.xml](db/types.md) which tells the CLE what spawn points to use. For example, the M16A2 has the loot category `weapons` which means that for our fire station it would only spawn on the third set of loot spots. Loot categories are defined in [cfglimitsdefinition.xml](cfglimitsdefinition.md) and you can add as many you want for each `container`.
### tag
I am not sure if this does anything or if it's just for debugging purposes. In [cfglimitsdefinition.xml](cfglimitsdefinition.md) there are the three tags `floor`, `shelves` and `ground` but they seems to not always be used and you'd think the spawn point would be enough to place an item. Maybe it has to do with finding the correct rotation or something for items, if you know what this does please tell me!
### point pos="vector" range="float" height="float"
The actual spawn point inside a building for items. Locations are expressed in local coordinates from the building's origin. `range` and `height` probably prevents items stacking and clipping through the ground, respectively. I'm not going into too much detail both because I haven't messed around with this and that you would probably use some kind of tool to generate these anyway. From what I can tell you can use the [DayZ Editor Mod](https://steamcommunity.com/sharedfiles/filedetails/?id=2250764298) to edit these and it's probably something you have to do if you want to introduce new buildings to the game.
# Modifying the file
One of the simple things you can do in this file is to add or remove certain types of items from buildings. For example, if you want medical items to spawn in the fire station that we had as an example, you simply add the line `<usage name="Medical" />` next to the other `usage` lines. The same can be done for the categories, if you want weapons to spawn on more spawn points inside a building you can add that category name, beware that not all spawn points might work for larger items.