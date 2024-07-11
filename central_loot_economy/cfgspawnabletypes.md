# Introduction
The `cfgspawnabletypes.xml` seems to primarily be for allowing items and zombies to contain loot and attachments. It is also closely coupled with [types.xml](db/types.md) and [cfgrandompresets.xml](cfgrandompresets.md). Each entry type needs to match the name in [types.xml](db/types.md) and each item spawned must either be an item from [types.xml](db/types.md) or a preset defined in [cfgrandompresets.xml](cfgrandompresets.md). As an example we can look at `DuffelBagSmall_Medical` which uses both at the same time.
```xml
<type name="DuffelBagSmall_Medical">
	<cargo chance="0.8">
		<item name="BandageDressing" />
	</cargo>
	<cargo chance="0.15">
		<item name="TetracyclineAntibiotics" />
	</cargo>
	<cargo chance="0.01">
		<item name="AntiChemInjector" />
	</cargo>
	<cargo preset="foodArmy" />
	<cargo preset="mixArmy" />
</type>
```
What this entry says is that upon spawning it should roll whether or not to also spawn a few items in its cargo. It explicitly mentions `BandageDressing`, `TetracyclineAntibiotics` and `AntiChemInjector` but also refers to the two presets `foodArmy` and `mixArmy`. Each cargo has a chance of spawning which is set in a value from `0.0` to `1.0` so the `BandageDressing` has 80% chance to spawn in this example. This file can be modified if you want to add or remove items from spawning inside the cargo of other items.

This file also controls the attachments spawned on weapons in the game, so if you want to control how certain weapons look when spawning you can modify them here.
```xml
<type name="AKS74U">
	<attachments chance="1.00">
		<item name="AKS74U_Bttstck" chance="1.00" />
	</attachments>
	<attachments chance="0.30">
		<item name="Mag_AK74_30Rnd" chance="1.00" />
	</attachments>
</type>
```
The attachments work very similar to the cargo above, but the distinction is there to tell the game whether to attach an item or spawn it inside the cargo. This makes a difference for items that has both cargo and attachments (like most backpacks).