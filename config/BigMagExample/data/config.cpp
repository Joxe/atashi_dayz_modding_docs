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

	class Cooler_Mag_CZ550_10rnd : Mag_CZ550_10rnd
	{
		displayName="Cooler CZ550 10rnd Mag";
		itemSize[]={3,3};
	};
};
