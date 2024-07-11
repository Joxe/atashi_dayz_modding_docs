> [!WARNING] Disclaimer
> This list is bad and incomplete. It is full of guesswork and you shouldn't take anything as truth until it has like 600 peer reviews.
# All Entities
## scope
I am not completely sure what this does but it has something to do with the visibility of the item (similar to private, protected and public in code). As far as I understand setting this to `scope=2;` will mean that your scripts and everything know about the item but setting it lower means that they might not be able to. Set to `scope=2;` until you figure out why you shouldn't.
## model
Should be the relative path to your model.p3d file. Can be left empty for objects without a model and those will be completely invisible in game. The paths are relative to your mod root directory so in my case I have my mod under `P:\ZeroRevival\` which means that the model string should look something like this:
```cpp
model="ZeroRevival\Data\Vehicles\Mi35\Mi35.p3d";
```
There are plenty of examples of this in the DayZ code base since a lot of items has models, you are able to use any of them in your own items as well.
## displayName
Either a string or string table entry that will show on the item in the inventory, so make this human readable. String table entries are prefixed by `$STR_` from the looks of it and lets you give items names that are localised to other languages.
```cpp
displayName="Mi-35 Helicopter";
displayName="$STR_CfgMagazines_Mag_CZ550_10rnd0";
```
## descriptionShort
The text string or string table entry that is displayed when a player inspects an item.
```cpp
descriptionShort="This is a very short description";
descriptionShort="$STR_cfgVehicles_CourierBag1";
```
## description
For some reason there is also `description="";` but that variable is never set in the DayZ configs so I guess it's some legacy thing?
## animClass
I *think* this has to do with the new animation system introduced in DayZ. This probably loads the correct class for the Entity's animations to be possible to play. I don't know if there is a comprehensive list anywhere with all the possible `animClass` values but these are the ones that I can find in the DayZ configs. If you know more, please add descriptions to these and what they are used for in a pull request or issue.
```cpp
animClass="Binoculars";
animClass="Shotgun";
animClass="Bow";
animClass="NoFireClass";
animClass="Compass";
animClass="Knife";
animClass="Chainsaw";
animClass="Pistol";
animClass="Rifle";
```
## rotationFlags
I think this has something to do with how objects are rotated when placed on the ground. The value itself seems to be a set of flags stored as an integer meaning that if you see `12` it means that it's bit 3 and 4 set and the others unset. This only seems to be referred to in scripts with some constants and the function `PlaceOnSurfaceRotated` in `EntityAI`, `rotationFlags` is probably only necessary when you create your own models for things.
## simulation
No clue what this does, but it probably has to do with toggling something inside the engine. The clue we get is that the value for existing items seems to be somewhat related to the item type so almost all optics in the game has `simulation="itemoptics";` and all animals seems to have `simulation="animal";`. A list of all options does not seem to be available anywhere so I would love to know more what this actually does.
## reversed
Unknown.
## weight
The item weight in grams, only affects the weight shown in game and how much max stamina you lose by keeping the item in your inventory from what I can tell.
## weightPerQuantityUnit
When items can contain things this sets how much the item weight increases per quantity. This can for example be used for increasing the weight of a water bottle when you add water into it.
# Inventory Items
## itemSize
Sets the width and height of the item when picked up. First value is the width and the second is the height.
```cpp
itemSize[]={4,3};
```
## Quantity
If the item should have some sort of quantity you set the min and max value here and an initial value. `quantityBar` makes the bar show in the UI for the item if it's set to `1` and hides it if it's set to `0`. `varQuantityDestroyOnMin` sets whether the item should be destroyed when the quantity goes down to 0, which makes sense for things like food but not magazines.
```cpp
quantityBar=1;
varQuantityInit=20;
varQuantityMin=0;
varQuantityMax=20;
varQuantityDestroyOnMin=0;
```