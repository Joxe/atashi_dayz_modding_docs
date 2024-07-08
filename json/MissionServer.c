enum AnEnum
{
	SomeValue,
	AnotherValue,
	OhLookAThirdValue
}

class JSON_TestJson
{
	int MyInteger;
	float MyDecimal;
	string MyString;
	AnEnum MyEnum;
	ref array<string> MyStringArray;
	ref array<ref JSON_ObjectArrayElement> MyObjectArray;
}

class JSON_ObjectArrayElement
{
	string Name;
	int Id;
}

modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();

		// Load Example
		auto jsonData = LoadDataFromDisk();

		Print(string.Format("MyInteger: %1", jsonData.MyInteger));

		// Save Example
		auto myData = new JSON_TestJson();
		
		myData.MyInteger = 123;
		myData.MyDecimal = 420.69;
		myData.MyString = "X-Potato";
		myData.MyEnum = AnEnum.OhLookAThirdValue;

		myData.MyStringArray = new array<string>();
		myData.MyStringArray.Insert("DayZ");

		auto someObject = new JSON_ObjectArrayElement();
		someObject.Name = "John";
		someObject.Id = 125;

		myData.MyObjectArray = new array<ref JSON_ObjectArrayElement>();
		myData.MyObjectArray.Insert(someObject);

		SaveAsJsonFile(myData);
	}

	JSON_TestJson LoadDataFromDisk()
	{
		JSON_TestJson testJsonData;
		string errorMessage;

		if (JsonFileLoader<JSON_TestJson>.LoadFile("$mission:my_load_test.json", testJsonData, errorMessage))
		{
			return testJsonData;
		}
		else
		{
			Error(string.Format("Failed to load JSON: %1", errorMessage));
			return null;
		}
	}

	bool SaveAsJsonFile(JSON_TestJson myData)
	{
		string errorMessage;

		if (JsonFileLoader<JSON_TestJson>.SaveFile("$mission:my_save_test.json", myData, errorMessage))
		{
			return true;
		}
		else
		{
			Error(string.Format("Failed to save JSON file %1", errorMessage));
			return false;
		}
	}
}
