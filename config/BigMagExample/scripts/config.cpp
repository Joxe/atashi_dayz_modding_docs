class CfgPatches {
	class BigMag_Scripts {
		requiredAddons[] = {
			"DZ_Data",
		};
	};
};

class CfgMods {
	class BigMag {
		name="BigMag Mod";
		type = "mod";
		credits = "Atashi";
		author = "Atashi";
		action = "";
		version = "0.1";
		dependencies[] = {
			"World"
		};
			
		class defs {
			class worldScriptModule {
				value = "";
				files[] = { "BigMagExample/Scripts/4_World" };
			};
		};
	};
};
