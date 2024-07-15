# Introduction
The server configuration is a file that the server requires to run, by installing the DayZ Server package from Steam you get a pretty well documented template for this file called `serverDZ.cfg` which alongside the [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration) page is a great starting point to get a server up and running. I still think that for the sake of being complete it's worth going through them and show off some examples. Many of these might be best learned by proficient server hosts, especially the ones that affect network stuff and if there's something wrong in the list don't hesitate to create a pull request or create an issue. The list is ordered by alphabetical order.

## adminLogBuildActions
According to the [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration) this outputs an admin log on base building actions. Could probably be used to catch when players find a way to exploit and build bases in places where they shouldn't. The Admin Log only appears if you start a server with the `-adminLog` parameter.
```
adminLogBuildActions = 1;
```
## adminLogPlacement
Whether or not you want an admin log entry when players put down placable things, the Bohemia Wiki page lists traps and tents but I guess this affects more items that are placed in the world. The Admin Log only appears if you start a server with the `-adminLog` parameter.
```
adminLogPlacement = 1;
```
## adminLogPlayerHitsOnly
Whether or not you want only players hitting other players in the log or also want to include AI, from what I can tell this is when player's get hit by others so if this is set to `0` you will see an admin log entry each time a player gets hit by an Infected, which of course could get a bit spammy. The Admin Log only appears if you start a server with the `-adminLog` parameter.
```
adminLogPlayerHitsOnly = 1;
```
## adminLogPlayerList
According to the [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration) this "logs periodic player list with position every 5 minutes" which could be useful to catch people moving around in the world too fast (although 5 minutes interval is a bit long for that). What's more interesting is that if you really like feature creep you could potentially parse through this and create a map replay for a server to see the players move around over the course of a server instance. It would be cool if the interval was configurable but creating something similar from within a mod shouldn't be too hard. If you have a lot of players on your server this probably creates a lot of spam in the admin log.
```
adminLogPlayerList = 0;
```
## allowFilePatching
If set to 1 it will enable connection of clients with "-filePatching" launch parameter enabled. File patching should allow you to hot reload script files during mod development so this should probably be set to `0` for all servers that are supposed to actually be played on by players.
```
allowFilePatching = 1;
```
## clientPort
Sets the port that the server should bind to and players connecting need to specify that port to be able to connect at all (this is done automatically by the server browser). If you are running multiple servers on the same machine you need to give each of them a unique port to use. By default DayZ seems to use port `2302` so this could be completely omitted from the server configuration if you only run one server instance. As with all ports this value needs to be between `1024` and `65535`, the ports below `1024` are allocated by Windows for various tasks and from my 5 seconds of searching the web it seems like you better use a port below `49152` on Linux. Just make sure you don't have any other program running on the port.
```
clientPort = 2302;
```
## defaultObjectViewDistance
The max range of object visibility (in metres) that you allow on your server, if clients have a lower value set the game will use that instead. This value should be *at least* 10% higher than `networkRangeFar` so far away objects aren't synced without being rendered, this value also affect how far you see terrain objects like trees. The default that of `1375` that the wiki lists is probably a decent starting point but if you want the far clip plane to be a bit further out you can increase it a bit. If you increase this over `1736` your clients need to manually change the value `preferredObjectViewDistance` in `C:\User\UserName\Documents\DayZ\ProfileName_settings.DayZProfile` to get the increased visibility as `Extreme` in game is `1736` metres, odd number really...
```
defaultObjectViewDistance = 1375;
```
## defaultVisibility
The max range of terrain visibility (in metres) that you allow on your server, if clients have a lower value set the game will use that instead. This value should be *at least* 10% higher than `networkRangeFar` so players far away gets properly rendered. The default that of `1375` that the wiki lists is probably a decent starting point but if you want the far clip plane to be a bit further out you can increase it a bit. If you increase this over `3000` your clients need to manually change the value `viewDistance` in `C:\User\UserName\Documents\DayZ\ProfileName_settings.DayZProfile` to get the increased visibility as `Extreme` in game is `3000` metres.
```
defaultVisibility = 1375;
```
## disable3rdPerson
Improves the game by at least 514 times by disabling the third-person mode (unbiased opinion) if set to `1`.
```
disable3rdPerson = 1;
```
## disableBanlist
Banned players are stored in a text file called `ban.txt` next to the server executable and is used by default. You can avoid the server using the banlist by setting this value to `true`. From the looks of it you can write either the 18 character long SteamID64 in the file or the hashed value that is available prints next to each player in the admin log.
```
disableBanlist = false;
```
## disableBaseDamage
The description on the [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration) says to "set to 1 to disable damage/destruction of fence and watchtower". I have never built a base in DayZ so I have no idea what this means? Maybe it makes bases invulnerable from players to prevent raiding while you're offline?
```
disableBaseDamage = 1;
```
## disableContainerDamage
The description on the [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration) says to "set to 1 to disable damage/destruction of tents, barrels, wooden crate and seachest". I have never built a base in DayZ so I have no idea what this means, maybe it makes it impossible to destroy containers or something?
```
disableContainerDamage = 1;
```
## disableCrosshair
Disables the static on-screen cross-hair if set to `1`. Cross-hairs from in-game scopes are still visible, of course.
```
disableCrosshair = 1;
```
## disableMultiAccountMitigation
The description on the [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration) says that this "disables multi account mitigation on consoles when true (default: false)" and from the looks of it this prevents the same console from logging in to the same server with different accounts in an effort to stop alts. Whether this should be disabled or not probably depends a lot on your community.
```
disableMultiAccountMitigation = 1;
```
## disablePersonalLight
Disables the personal light when set to `1`. The personal light is a light that follows the player around on each client (invisible to other players) which makes it a bit easier to see indoors during nights. Personal preference if this should be used or not.
```
disablePersonalLight = 1;
```
## disablePrioritylist
Priority players are stored in a text file called `priority.txt` next to the server executable and is used by default. Players with priority will skip to the front of the login queue if there is one so adding your server admins to this list is probably a good idea. You can avoid the server using the priority list by setting this value to `true`. From the looks of it you can write either the 18 character long SteamID64 in the file or the hashed value that is available prints next to each player in the admin log.
```
disablePriorityList = false;
```
## disableRespawnDialog
Whenever you die you normally get a choice to respawn as a random character or as the one the player sets in the main menu. Setting this to `1` means that the random option is always picked by default.
```
disableRespawnDialog = 0;
```
## disableVoN
Disables the Voice over Network if set to 1, which means that players will not be able to talk to each other at all.
```
disableVoN = 0;
```
## enableDebugMonitor
Shows a debug UI element which lists the player's location, blood and health values. Should be set to `0` unless you're specifically debugging during development.
```
enableDebugMonitor = 0;
```
## enableWhitelist
Servers by default let any (unbanned) player to connect, you can instead set the server into a whitelist mode which only lets whitelisted players join. This is a great setting for popular servers to reduce the number of random scripters joining just to ruin things before getting banned. Players can be added to the whitelist by adding their hashed Steam ID to the `whitelist.txt`.
```
enableWhitelist = 1;
```
## forceSameBuild
Prevents clients from connecting to the server if they are on another build than the server, should probably always be set to `1` unless there's some odd reason to allow other builds. The only reason I could see this being used is if a certain server build is bugged or something so you use an older build but let newer clients in. Experienced server hosters might know more.
```
forceSameBuild = 1;
```
## guaranteedUpdates
The description on the [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration) says "Communication protocol used with game server (use only number 1)" which means that this is probably some internal value used for the network bubble technology added early on in DayZ, keep this as `1` unless you know why you are switching.
```
guaranteedUpdates = 1;
```
## hostname
The name of the server, this is what people see in their server browser.
```
hostname = "EXAMPLE NAME";
```
## instanceId
If running multiple servers on the same machine this number tells it what storage to use. The storage is saved under `DayZServerRoot\mpmissions\dayzOffline.chernarusplus\Storage_X` where `X` is the instance ID. This means that you have to create one server config file per instance to give each instance its own ID and server name and all that.
```
instanceId = 1;
instanceId = 2;
instanceId = 3;
```
## lightingConfig
Toggles the two lighting mods, 0 for brighter night, 1 for darker night. Shouldn't affect anything during daytime.
```
lightingConfig = 0;
```
## logAverageFps
If combined with the `-doLogs` launch parameter your server will spit out the server FPS every second both in the log window/terminal and in the `RPT` file. Probably becomes quite spammy which might make log reading more difficult.
```
logAverageFps = 1;
// Outputs a message like
// 20:49:37 Average server FPS: 64.55 (measured interval: 1 s)
```
## loginQueueConcurrentPlayers
Sets the number of players that are going through the login process at a time, this can be used to not spawn 100 players at once upon server restart. Can probably be left at the default value `5`.
```
loginQueueConcurrentPlayers = 5;
```
## loginQueueMaxPlayers
The maximum number of players for the login queue, default seems to be 500 which should be enough for 99% of servers.
```
loginQueueMaxPlayers = 500;
```
## logMemory
If combined with the `-doLogs` launch parameter your server will output the current memory usage in Kilobytes every second. That said, the number seems to be very off. I get about 2 300 000 KB which is about 2.3 GBytes but Windows reports the process using 3.7 GByte of memory. So in practice you can use this value to plot the memory usage increase over time or something to find memory leaks.
```
logMemory = 1;
// Outputs a message like
// 20:50:16 Used memory: 2368195 KB
```
## logPlayers
If combined with the `-doLogs` launch parameter your server will output the current amount of players connected to the server every second, feels rather unnecessary since the log also contains connections and disconnections but there are probably uses.
```
logPlayers = 1;
// Outputs a message like
// 20:50:16 Players: 1 in total
```
## logFile
Sets the server log output, this file is appended to for each run of the server so it can grow quite large if you log a bunch of stuff. The server logs seems to mostly look like the `RPT` file and probably does require the `-doLogs` startup parameter set.
```
logFile = "server_console.log";
```
## MaxPing
Set to define the ping value from which a player is kicked from the server (value in milliseconds). The number you should set probably depends a lot on where you are hosting the server and how much you value low lag gameplay, RP servers might be fine with higher ping and PvP focused might require lower.
```
MaxPing = 300;
```
## maxPlayers
The maximum amount of players, when the server hits the max it starts putting new players into the login queue. This number depends on a bunch of factors like how many players you want on the server and how good hardware your server is running. Default is 60.
```
maxPlayers = 60;
```
## Missions
This part tells the server what mission to actually load, this should match a directory name under `DayZServerRoot\mpmission\`. By default you have `dayzOffline.chernarus` and `dayzOffline.enoch` but you can also add other missions and select them here. Not sure why this looks like a class but just go with the flow.
```cpp
class Missions
{
	class DayZ
	{
		template = "dayzOffline.chernarusplus";	// Mission to load on server startup. <MissionName>.<TerrainName>
	};
};
```
## motd[]
An array of string with messages that will be displayed to players in a rotating manner. If you want more control over messages displayed to your players you should take a look at the `DayZServerRoot\mpmission\dayzOffline.chernarusplus\db\messages.xml` file.
```
motd[] = { "Welcome to my server!", "ur mom lol" };
```
## motdInterval
The interval at which the messages of the day is displayed, in seconds.
```
motdInterval = 60;
```
## multithreadedReplication
According to the [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration) this "enables multi-threaded processing of server's replication system. number of worker threads is derived by settings of jobsystem in dayzSettings.xml by "maxcores" and "reservedcores" parameters (value 0-1)". Not sure what this means but "multithreaded = yes" generally means faster in video games and "multithreaded = no" generally means more stable. It defaults to `1` so surely it's stable enough to use!
```
multithreadedReplication = 1;
```
## networkRangeClose
DayZ seems to be using multiple ranges for syncing different things in the world with clients. The closest range seems to be defaulted to `20` metres and the description says "network bubble distance for spawn of close objects with items in them (f.i. backpacks)" which indicates to me that this might be the range at which container contents are synced. Containers themselves are visible and synced from way further away than 20 metres. As a general rule a smaller network bubble should be more performance but might make gameplay suffer.
```
networkRangeClose = 20;
```
## networkRangeNear
DayZ seems to be using multiple ranges for syncing different things in the world with clients. The near range seems to be defaulted to `150` metres and the description says "network bubble distance for spawn (despawn +10%) of near inventory items objects" which indicates to me that this is the distance at which you see objects on the ground that inherit from the `InventoryItem` class in scripts. As a general rule a smaller network bubble should be more performance but might make gameplay suffer.
```
networkRangeNear = 150;
```
## networkRangeFar
DayZ seems to be using multiple ranges for syncing different things in the world with clients. The far range seems to be defaulted to `1000` metres and the description says "network bubble distance for spawn (despawn +10%) of far objects (other than inventory items)" which basically means all entities like infected, players, objects, etc. As a general rule a smaller network bubble should be more performance but might make gameplay suffer.
```
networkRangeFar = 1000;
```
## networkRangeDistantEffect
This config confuses me. DayZ does have a new-looking Effects system for both sounds and particles and this description "network bubble distance for spawn of effects (currently only sound effects)" *seems* to indicate that you should be able to spawn a sound effect on the server and that it would just sync to clients within the network bubble. But as far as I can see this is not the case or not used as both long-range sound effects (contaminated zones and crash sites) use [RPCs](../rpc/readme.md) to play their sound effects. I could be wrong, of course.
```
networkRangeDistantEffect = 4000;
```
## networkObjectBatchLogSlow
Unknown, the [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration) says "Maximum time a bubble can take to iterate in seconds before it is logged to the console" so perhaps this is a way of logging when the server is struggling?
```
networkObjectBatchLogSlow = 5;
```
## networkObjectBatchEnforceBandwidthLimits
Unknown, the [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration) says "Enables a limiter for object creation based on bandwidth statistics" which might mean that if you create a bunch of objects at once they are batched up and sequentally sent to clients.
```
networkObjectBatchEnforceBandwidthLimits = 1;
```
## networkObjectBatchUseEstimatedBandwidth
Unknown, the [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration) says "Switch between the method behind finding the bandwidth usage of a connection. If set to 0, it will use the total of the actual data sent since the last server frame, and if set to 1, it will use a crude estimation" which probably means that it's related to the `networkObjectBatchEnforceBandwidthLimits` option.
```
networkObjectBatchUseEstimatedBandwidth = 0;
```
## networkObjectBatchUseDynamicMaximumBandwidth
Unknown, the [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration) says "Determines if the bandwidth limit should be a factor of the maximum bandwidth that can be sent or a hard limit. The maximum bandwidth that can be sent fluctuates depending on demand in the system." which probably means that it's related to the `networkObjectBatchEnforceBandwidthLimits` option.
```
networkObjectBatchUseDynamicMaximumBandwidth = 1;
```
## networkObjectBatchBandwidthLimit
Unknown, the [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration) says "The actual limit, could be a [0,1] value or a [1,inf] value depending on `networkObjectBatchUseDynamicMaximumBandwidth`.".
```
// Used if networkObjectBatchUseDynamicMaximumBandwidth == 1
networkObjectBatchBandwidthLimit = 0.8;
// Used if networkObjectBatchUseDynamicMaximumBandwidth == 0
networkObjectBatchBandwidthLimit = 100;
```
## networkObjectBatchCompute
Sets the batch size of the objects that are being created or destroyed per frame, if more objects are being created or destroyed on this frame than this value they will be deferred to the next frame to let the server update all the other things that servers do.
```
networkObjectBatchCompute = 1000;
```
## networkObjectBatchSendCreate
The [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration) says "Maximum number of objects that can be sent for creation" which is rather self-explanatory, to avoid sending all the object creations at once it batches them out over multiple frames. What's not apparent is if this value is total on frame or per player per frame, I would guess the latter due to how low the default value is.
```
networkObjectBatchSendCreate = 10;
```
## networkObjectBatchSendDelete = 10;	//Maximum number of objects that can be sent for deletion
The [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration) says "Maximum number of objects that can be sent for deletion" which is rather self-explanatory, to avoid sending all the object creations at once it batches them out over multiple frames. What's not apparent is if this value is total on frame or per player per frame, I would guess the latter due to how low the default value is.
```
networkObjectBatchSendDelete = 10;
```
## password
The password required to connect to the server, can be left empty if you don't want your server to be password protected (most aren't).
```
password = "password123";
```
## passwordAdmin
The password for logging in as an admin. You log in as an admin by writing `#login my_admin_password` in the in-game chat (including the `#` before `login`) which lets you run the [admin commands](admin_commands.md).
```
passwordAdmin = "my_admin_password";
```
## pingWarning
When the ping between the client and the server reaches above this value they get the ping warning or "yellow chain" icon showing, indicating that there might be an issue. This value should be set below `pingCritical` and `MaxPing` to make sense.
```
pingWarning = 200;
```
## pingCritical
When the ping between the client and the server reaches above this value they get the critical ping warning or "red chain" icon showing, indicating that there might be an issue. This value should be set below `MaxPing` to make sense.
```
pingCritical = 250;
```
## verifySignatures
The [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration) says "Verifies .pbos against .bisign files. (only 2 is supported)" and while this is correct for any server you are hosting to the public you can change this value to be something else. Setting this value to `0` and there won't be any verification done for the mods that players connect with meaning that you can essentially join with any mod of any version and if you're lucky it works. For my local development server I have this set to `0` just in case I want to test stuff. This variable actually goes way back to Arma 2 and you can see this being explained by [Bohemia on their forums](https://forums.bohemia.net/forums/topic/110512-new-version-of-signatures-coming-in-158/).
```
verifySignatures = 2;
```
## vonCodecQuality
The quality of the voice over network codec, expressed in a value between 1-20. With the bandwidth, servers and clients we have nowadays there's probably no reason to lower this below `20`.
```
vonCodecQuality = 20;
```
## respawnTime
The time in seconds until you are able to start a new character when your previous one died. Probably mostly set to a moderatly high value (like a minute or two) to avoid players suiciding to reroll spawn positions or set very high if you want permadeath-ish server where dying locks you out for the night or something. Not sure if this value survives a server shutdown but it should prevent players from spawning even when they rejoin the server.
```
respawnTime = 5;
```
## serverFpsWarning
The [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration) says "set to define the server fps value under which the initial server fps warning is triggered (minimum value is 11)" but it doesn't go into detail as to *where* the warning ends up but it's probably in the server log or the RPT file.
```
serverFpsWarning = 15;
```
## serverTime
The time in which the server starts up in, should probably be combined with `serverTimePersistent` to get the behaviour that you want. Can be used to always start a server at like 08:00 to avoid having night time at all or you can set it to `SystemTime` to make the server read from the system clock and match the value. If you have `serverTimePersistent` set to `1` and the server has saved at least once, `serverTime` does nothing.
```
serverTime = "SystemTime";
serverTime = "2015/4/8/17/23"; // YYYY/MM/DD/HH/MM
```
## serverTimeAcceleration
Controls accelerated time expressed as a multiplier which lets you either speed up or slow down the time on the server. The valid values are between `0.1` for 10% of the normal speed and `64` for 64 times faster than normal speed (an hour becomes slightly less than a minute long).
```
serverTimeAcceleration = 1; // Time runs at normal speed
serverTimeAcceleration = 10; // Time is run 10 times faster
```
## serverNightTimeAcceleration
Same as `serverTimeAcceleration` but it's only applied during night, should be a great tool if you want nights but not the super long ones (with 1x speed they are like 8 hours long or something). The accepted values are the same as `serverTimeAcceleration` so `0.1` for 10% of the speed and `64` for 64 times the speed. This value is *multiplied* with `serverTimeAcceleration` meaning that 2x general time acceleration and 2x `serverNightTimeAcceleration` means that night time accelerates to 4x the speed. There might however be a cap to the speed since setting both time accelerations to `64` still only seem to apply one of them (an hour is still about a minute).
```
serverNightTimeAcceleration = 1;
```
## serverTimePersistent
Whether to save the time to the persistance or not, if set to `1` the server will start up at the same time as it shut down and if set to `0` it will use the `serverTime` setting to set the time on every startup.
```
serverTimePersistent = 0;
```
## shotValidation
This seems to be set to `1` but there's very little information as to what this *actually* does, but judging by the fact that searching for this only brings up cheating forums this probably has to do with some sort of anti-cheat and it should probably be set to `1` unless you know why it shouldn't be.
```
shotValidation = 1;
```
## simulatedPlayersBatch
Sets how many players that are simulated per frame, not too much information about what this simulation involves. If you have freeze lags on full servers maybe lowering this value will help?
```
simulatedPlayersBatch = 20;
```
## speedhackDetection
Lets you configure the speedhack detection a bit, the default is `1` which is the most strict but if your players are lagging a lot this could run into false positives so it might be best to increase this value a bit. The max value is `10` which should be the most benevolent, no real details as to what that means and since I have no idea how to speedhack it's hard to test.
```
speedhackDetection = 1;
```
## steamQueryPort
Defines the port that the game use to talk with Steam to make your server show up in the server list. I don't know if you *have to* provide this since I run my server without this set in my `serverDZ.cfg`, but if you have issues seeing your server in the server list this might be the config value to check.
```
steamQueryPort = 2305;
```
## storageAutoFix
The [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration) says "Checks if the persistence files are corrupted and replaces corrupted ones with empty ones (value 0-1)". Hard to say exactly what this does but it might be to fix items that become corrupt from changing mods or new patches or something, purely guesswork from my side. The way I see it is "corrupt == bad" so I just leave this at `1`.
```
storageAutoFix = 1;
```
## timeStampFormat = "Short";		// Format for timestamps in the .rpt file (value Full/Short)
Sets the format for timestamps in the RPT file, the `Short` is just the timestamp while `Full` also adds the date to the timestamp.
```
timeStampFormat = "Short"; // Format: 23:06:25.941
timeStampFormat = "Full"; // Format: 2024/07/15, 23:26:49.390
```
