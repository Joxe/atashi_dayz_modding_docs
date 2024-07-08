# Introduction
DayZ supports saving with deserialisation and loading by serialising data to and from JSON files, very similar to how C#/.NET does it. It can act as an alternative to the XML files that DayZ generally use in the Mission files and as a mission specific alternative to config files. Storing values as configs should let you modify the behaviour of mods on a server without having to patch the mods themselves. Serialisation supports all types that makes sense to JSON, meaning that you can use integers, decimals, strings and arrays. Enums are supported but by converting an integer in the JSON to an enum value, so it is not very legible what enum value it is when you look in the JSON. Saving and loading any kind of binary data is probably best done as a string if you can find a way of serialise it in some way.
# Loading JSON from disk
The JSON file can be located in 'DayZServer/mpmissions/dayzOffline.chernarusplus/' if you use the standard server setup.
```
{
	"MyInteger": 1234,
	"MyDecimal": 6.9,
	"MyString": "Hello, world!",
	"MyEnum": 2,
	"MyStringArray": [
		"Hello",
		"World",
		"!"
	],
	"MyObjectArray": [
		{
			"Name": "Zombie",
			"Id": 1234
		},
		{
			"Name": "Human",
			"Id": 420
		}
	]
}
```
We then need to set up the class structure that matches the JSON inside the mod. All the class names are arbitrary (you can name them whatever you want) but it might be a good idea to prefix them with something to make it apparent that they are read from a JSON. Classes can be written in any order.
```
enum AnEnum
{
	SomeValue,
	AnotherValue,
	OhLookAThirdValue
}

class JSON_TestJson
{
	// Integral types are simple, just match the data type. (yes, I know string isn't an integral type)
	int MyInteger;
	float MyDecimal;
	string MyString;

	// Enums needs to be converted from an integer in the JSON, so 0 would become 'SomeValue', 1 would become 'AnotherValue', and so on.
	AnEnum MyEnum;

	// Arrays should be preceded with 'ref' to make them into strong references.
	ref array<string> MyStringArray;

	// Same with objects that needs to kept with a strong reference, precede them with the 'ref' keyword.
	ref array<ref JSON_ObjectArrayElement> MyObjectArray;
}

class JSON_ObjectArrayElement
{
	string Name;
	int Id;
}
```
With the class structure set up we can now load the JSON at any point in our mod code. I have not yet run into a case where the load fails for being too early or anything.
```
class TestJson
{
	static JSON_TestJson LoadDataFromDisk()
	{
		// Both the data and the error message are out-parameters so we need to allocate them first.
		JSON_TestJson testJsonData;
		string errorMessage;

		// Put the class name of the JSON you are trying to load between the < >, this tells the JsonFileLoader what type it should expect to find which
		// lets it output a rather good error message. If you know about generics from C++, C# or other languages this is nothing new, of course.
		if (JsonFileLoader<JSON_TestJson>.LoadFile("$mission:my_load_test.json", testJsonData, errorMessage))
		{
			// The JsonFileLoader.LoadFile function returns true if it successfully loads the file.
			return testJsonData;
		}
		else
		{
			// If you fail to load the file for any reason, you should make sure to output the error message in a way you can read it since it
			// generally tells you quite correctly what went wrong.
			Error(string.Format("Failed to load JSON: %1", errorMessage));
			return null;
		}
	}
}
```
If you call 'LoadDataFromDisk' you will get an object back containing the serialised version of the JSON file. So from this point on you can just use it as any other object in your code.
```
void DoSomething()
{
	auto jsonData = TestJson.LoadDataFromDisk();

	if (jsonData)
	{
		// Should print "Hello, world!" as defined in the example JSON.
		Print(jsonData.MyString); 
	}
}
```
# Saving JSON to disk
Just like you can serialise a JSON file into data you can do the reverse and deserialise game data into a JSON file. It's very much just loading in reverse so if you set up the `JSON_TestJson` and `JSON_ObjectArrayElement` classes like in the Loading example you can use `JsonFileLoader.SaveFile` to save it to a file.
```
bool SaveJsonToFile()
{
	auto myData = new JSON_TestJson();

	// Fill it with some random data so we can see if it's being saved.
	myData.MyInteger = 123;
	myData.MyDecimal = 420.69;
	myData.MyString = "X-Potato";
	myData.MyEnum = AnEnum.OhLookAThirdValue;

	// Remember to instantiate the array if you haven't done so in the class definition.
	myData.MyStringArray = new array<string>();
	myData.MyStringArray.Insert("DayZ");

	// Create an object to put into the object array.
	auto someObject = new JSON_ObjectArrayElement();
	someObject.Name = "John";
	someObject.Id = 125;

	myData.MyObjectArray = new array<ref JSON_ObjectArrayElement>();
	myData.MyObjectArray.Insert(someObject);

	// Just like when loading we need to allocate a string for it to write errors to.
	string errorMessage;
	if (JsonFileLoader<JSON_TestJson>.SaveFile("$mission:my_save_test.json", myData, errorMessage))
	{
		// The JSON should now 
		return true;
	}
	else
	{
		Error(string.Format("Failed to save JSON file %1", errorMessage));
		return false;
	}
}
```
Running this code should create a JSON file in 'DayZServer/mpmissions/dayzOffline.chernarusplus/' called 'my_save_test.json' which should have all the data in it.
# Error Messages
Error messages from LoadFile are generally quite simple to understand, but we can go through one of them as an example that I got while developing.
```
Failed to load map zones: Cannot load data from "$mission:ZeroRevivalConfigs/cfg_map_zones.json":
JSON ERROR:
Reading variable: data
Expecting instance
Reading variable: mapZones
Expecting array
At index 0, Expecting instance
Reading variable: shape
Expecting int
Cannot convert to int
```
You can generally see this as an upside-down call stack. The JSON I was trying to load looked something like this:
```
{
	"mapZones": [
		{
			"name": "NWAF_Tier2",
			"origin": [4400.0, 0.0, 10200.0],
			"shape": "Circle",
			"radius": 2000.0,
			"lootTier": 2
		},
		{
			"name": "NWAF_Tier1",
			"origin": [4400.0, 0.0, 10200.0],
			"shape": "Circle",
			"radius": 4000.0,
			"lootTier": 1
		},
		{
			"name": "Zeleno_Tier1",
			"origin": [3000.0, 0.0, 6000.0],
			"shape": "Circle",
			"radius": 1000.0,
			"lootTier": 1
		}
	]
}
```
So translated the error message is:
- Looking at variable 'data', I expect it to be an object ('data' is the placeholder name for the top level in JSON files).
- Looking at variable 'mapZones', I expect it to be an array.
- Looking at index 0 (the first one), I expect it to be an object.
- Looking at the variable 'shape' inside index 0, I expect it to be an integer.
- It was not an integer!

The error in this case is that my 'shape' variable is a string rather than an integer and it can be resolved by either changing "Circle" to 0 or changing the class to instead take a string. This is an error I ran into when I was trying to figure out if you could read enum values by string or not. My classes for reference:
```
class ZRCfgMapZones {
	ref array<ref ITEM_MapZone> mapZones = new array<ref ITEM_MapZone>();
}

class ITEM_MapZone {
	string name;
	vector origin;
	ZRMapZoneShape shape;
	float radius;
	int lootTier;
}
```
In general, reading JSON loading errors means that you read the last line of the error, it seems to stop trying to parse the JSON once it hits the first error.
# Pitfalls
## Default Values
Any default initialisation values set in the classes in the code of your mod will be stomped over by their type default if they don't exist in the JSON that you are loading.
```
class Stomped
{
	int MyInt = 1;
	float MyFloat = 133.7;
	string MyString = "Hello";
}
```
If we try to load this JSON.
```
{
	"MyInt": 3
}
```
'MyFloat' will be set to '0.0' and 'MyString' will be an empty string since they were not present in the JSON. This kind of requires you to write out default values or handling them in some other way.
```
{
	"MyInt": 3,
	"MyFloat": 133.7,
	"MyString": "Hello"
}
```