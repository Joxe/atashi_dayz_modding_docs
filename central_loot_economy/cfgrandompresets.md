# Introduction
This file is a helper file for [cfgspawnabletypes.xml](cfgspawnabletypes.md), to prevent having to write the same XML entry for each item of the same-ish type you are able to create a preset here and then use it in [cfgspawnabletypes.xml](cfgspawnabletypes.md). Let's take look at an example:
```xml
<cargo chance="0.15" name="foodHermit">
	<item name="TunaCan" chance="0.11" />
	<item name="SardinesCan" chance="0.11" />
	<item name="Apple" chance="0.07" />
</cargo>
```
And then inside [cfgspawnabletypes.xml](cfgspawnabletypes.md) we can see this being used for a bunch of zombies:
```xml
<type name="ZmbF_HikerSkinny_Blue">
	<cargo preset="foodHermit" />
	<cargo preset="toolsHermit" />
	<attachments preset="hatsFarm" />
</type>
```
For all intents and purposes you can see this preset as simply pasting your preset text into the [cfgspawnabletypes.xml](cfgspawnabletypes.md) file when the game starts.