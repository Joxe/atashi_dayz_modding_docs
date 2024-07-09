class Cooler_Mag_CZ550_10rnd : Mag_CZ550_10rnd
{
	float GetChanceToJam()
	{
		float randomNumber = Math.RandomFloat01();

		return randomNumber <= 0.1;
	}
}