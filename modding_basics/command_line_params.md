# Introduction
DayZ accepts a multitude of command line arguments being sent to the executable on startup, many of these are available in the launcher you use when you start the game from Steam but there's a lot of them that aren't and the documentation for them is sometimes a bit lacking. The aim is of course to make this list of commands complete and documented, but since there's no available list of *all* the command line arguments (at least that I can find) it has to be patched together from a few sources on the internet. This will most likely just end up as yet another incomplete list as per [XKCD](https://xkcd.com/927/).
# Command Line Basics
If you haven't worked with command line arguments before they are information that you can pass to a program upon startup, normally from a command prompt/terminal or a script. As in example, if you run `.\DayZ_x64.exe -connect=127.0.0.1` the game will start and try to connect to the IP that you passed to the `-connect` parameter. This is essentially what the different launchers that are available do when you try to join a server and it allows you to skip finding the server you are looking for by using the in-game server browser. Some command line parameters take paths to something on disk as argument and and since paths might have spaces you can also surround parameters by "quotation marks" to allow for spaces as they otherwise break the list of parameters. As an example we can use the `-mod` parameter. Say that you have your mod at `C:\Users\My Name\My Mod\`, to load it you would pass `"-mod=C:\Users\My Name\My Mod"` surrounded by quotation marks.
# Fast start of the server/client
These are two scripts that I use for development to quickly start the server and start the client so it connects to the server, they are mostly here as examples of longer command line arguments lists as just seeing a working example might tell more than any documentation to some people (by that I mean me).
```powershell
# Start Client
E:\SteamLibrary\steamapps\common\DayZ\DayZDiag_x64.exe "-mod=D:\DayZServer\@ZeroRevival;E:\SteamLibrary\steamapps\common\DayZ\!Workshop\@CF;E:\SteamLibrary\steamapps\common\DayZ\!Workshop\@Community-Online-Tools" "-connect=192.168.50.206" "-port=2302" -doLogs -filePatching -name=Atashi

# Start Server
D:\DayZServer\DayZDiag_x64.exe "-config=D:\DayZServer\zr_serverDZ.cfg" "-mod=@ZeroRevival;1559212036;1564026768;1565871491" -filePatching -server -doLogs -profiles=profiles -adminlog -limitFPS=300
```
Don't put too much effort in figuring out why I use the workshop mods for the server and not for the client, it's just how it ended up, luckily this shows that you can use both. I have these two Powershell script files that run these commands, that way I can start the server and client without having to search for my local dev server.
# The list of parameters
Should be sorted by alphabetical order, otherwise it implies an order of importance. Try to always list if a parameter is exclusive to client, server or if it can be used on both. Provide a source if possible for each parameter.
## adminlog
Available on Server. When this parameter is set the server will output the admin log to a file named `DayZServer_x64.ADM` or `DayZDiag_x64.ADM` which ends up either in your `%LOCALAPPDATA%\DayZ\` directory or in your assigned `Profiles` directory. The admin log contains a bunch of messages that should help you figure out what's happening on a server from a gameplay point of view, so it will output things like if a player was killed, connections or disconnections and player chat. It will not output error messages from mods or the game itself. From scripts you can also output your own messages:
```cpp
GetGame().AdminLog("this is my message");
```
The messages you post will show up with a timestamp like this:
```
13:23:13 | this is my message
```
## BEpath
Available on Server. According to the [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration) this " Sets a custom path to the BattlEye files" but I have no idea when you would want to do that. The [Linux Server Example](https://community.bistudio.com/wiki/DayZ:Hosting_a_Linux_Server#Without_Mods_2) seems to use it as a relative path but I run my Linux server without it and it seems to work fine? Maybe this is something that you use when you run more than one server per machine?
```
-BEpath=battleye
```
## buldozer
Available on Client. Starts the game in Buldozer mode which can be used to look at models and modify maps (I think, I have not used it much).
```
-buldozer
```
## config
Required on Server. A server requires a [Server Config](server_config.md) to run as it tells the server a bunch of information that it needs, like server name, password and what map to load. More information should be available in the Server Config page. The path here *should* be able to take a relative path but I have had issues with that so I went with an absolute path. The Server installation on Steam comes with a default file named `serverDZ.cfg` which you can either modify or use as a template to create your own.
```powershell
# Either of these should be fine, the server won't start without this config file
-config=serverDZ.cfg
-config=D:\DayZServer\serverDZ.cfg
```
## connect
Available on Client. Immediately try to connect to a server on startup, takes an IP without the port (the port is a parameter by itself). *Probably* also accepts domain names but I haven't tried that.
```
-connect=127.0.0.1
```
## cpuCount
Available on Server (and maybe on Client?). Restricts the number of cores that DayZ is able to use which should be helpful when you run multiple servers on the same machine so one of the servers don't hog 7 cores while the other is getting 1. The number you should set depends on your setup and can probably be omitted if you only run one server instance on a machine. The [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration#Launch_Parameters) mentions that this is logical cores so if you want to use half of a 8 core/16 thread CPU you should set this to `8`.
```
-cpuCount=4
```
## doLogs
Available on Server and Client. If set it tells the game to print a bunch of stuff to a file called something like `DayZDiag_x64_2024-07-15_11-28-24.RPT`. This file does not contain any admin logs but it will print things like script or content errors which can help out with modding. You can also print to this file manually from code by calling `PrintToRPT(void var);` but I have found this to be a bit brittle. It's not super performant, which is fine, but if you call `PrintToRPT` at certain points in scripts (probably too early or something) you generally end up with crashes so it's something you should use with caution.
```
-doLogs
```
## filePatching
Available on Server and Client. *Supposedly* lets you do faster iteration of scripting by allowing new scripts to be patched in to the game while it is running but I have not got it to work yet and it's incredibly poorly documented online. There's *some* info on the [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Workbench_Script_Debugging#Filepatching) and there's also [this blog](https://medium.com/@jkamienski/beginners-guide-to-dayz-mods-development-de3055a10d31) that mentions it. So once I understand what's going on this description should change and there should be a new document with a step-by-step guide to get File Patching to work.
```
-filePatching
```
## freezeCheck
Available on Server. According to the [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration#Launch_Parameters) this "Stops the server when frozen for more than 5 min and create a dump file" which is very helpful for server hosting. This probably lets the server realise that a script is stuck in an infinite loop or something and quit itself, if you have set up automatic restarts on your server host this should at least not lock up the server forever if you're running mods that has some nasty bugs.
```
-freezeCheck
```
## limitFPS
Available on Server. Limits the server frame rate to the specified value, if not set the server ticks as fast as possible. Setting this to a good value will let you run more servers on a single machine without sacrificing the gameplay experience. Without adding too many mods DayZ normally has great performance (on my 7800X3D it runs at like 10-13k FPS) so setting this to a decently high value like `300` will let you have great performance without going overboard. You still want a kind of high value for the physics simulation to run properly so going under 100 FPS is probably not recommended.
```
-limitFPS=300
```
## mission
Available on Server. According to the [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration#Launch_Parameters) this "Defines the mission used by the server" but I have no clue what makes this different from setting the mission in the `serverDZ.cfg` file. If I try to set this to `-mission=dayzOffline.enoch` instead of my normal mission I get the error "Mission script has no main function, player connect will stay disabled!" meaning that I am probably missing some piece to the puzzle. So this parameter is an unknown for now.
## mod
Available on Server and Client. If you host servers this is probably one of the ones that you know about as it lets you set the mod list that clients needs to match to join your server. This parameter takes a list of mods separated by a semicolon `;`, it either take a relative path or absolute path of a mod and you can mix and match as you want.
```powershell
# Relative paths, this will look for directories with this name inside the DayZServer directory.
-mod=@ZeroRevival;@CF;MySuperCoolMod
# Absolute paths, this will check the exact directory on disk for the mod
-mod=D:\DayZServer\@ZeroRevival;E:\SteamLibrary\steamapps\common\DayZ\!Workshop\@CF;E:\SteamLibrary\steamapps\common\DayZ\!Workshop\@Community-Online-Tools
```
A very common use case you will see if you read a lot of guides for server hosting is that they use symbolic links to the workshop IDs of the mods so seeing a mod list look like the following is not uncommon. How to set up a modded server is outside the scope of this document, though.
```
"-mod=1559212036;1564026768;1565871491"
```
## name
Available on Client. Sets your name on the server. DayZ doesn't use your Steam name so if you don't set this you will just show up as "Survivor" on servers. The name is bound a character upon spawning so if you change the name it will apply on your *next* character. Remember to surround this with quotation marks if you want spaces in your name.
```
"-name=John Zombieman"
-name=xXx_DeAtH_SlAyEr_xXx
```
## netLog
Available on Server. According to the [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration#Launch_Parameters) this "Enables the network traffic logging". TODO: Test what this does and describe it.
```
-netLog
```
## noPause
Available on Client. The game usually stops rendering and sound effects when you Alt+Tab out of the game, setting `noPause` to `true` means that it will keep rendering even when focus is lost. This is also available to set from within the game options nowadays so you can probably just ignore this.
```
-noPause=true
```
## par
Available on Client (and maybe Server?). This is available in the DayZ launcher and is described as "Reads startup parameters from a file" which sounds like a straightforward way of quickly switching between sets of parameters but I don't know what format these should be in. Probably just a line of text? This probably takes both relative and absolute values like most file parameters.
```
-par=my_parameter_list.txt
-par=C:\ParametersLibrary\parameters1.txt
```
## password
Available on Client. Used alongside `-connect` and `-port` to also automatically send the server password to the server you are joining on startup so you can join password protected servers from the server browser. The only thing you might need to know if you're a streamer or something is that this password is stored in clear text in client logs so make sure to not leak it.
```
-password=password123
```
## port
Available on Server and Client. On server this specifies what port to bind to for incoming connections, clients then need to specify the port when connecting. By default DayZ uses `2302` but this is required if you want to run multiple servers on the same machine. If that's the case you can use port `2302` for one server, `2303` for another, and so on, and clients would use the same IP but specify a different port to be able to choose server instance to connect to. Clients only choose port when they also specify what server to connect to using the `-connect` parameter, when connecting from the server list the game handles ports automatically.
```
-port=2302
```
## profiles
Available on Server (and possibly Client?). Specifies the directory to use as the "profile" directory. This directory is where the server logs, crash reports and anything that explicitly use the profile directory in scripts end up. This takes either a relative or absolute path and in many guides to host servers you will see that they set this to simply be `-profile=profile` which creates a new directory called just that in the DayZServer directory. If not set these things normally end up in `%localappdata%\DayZ` on Windows or `~/.local/share/DayZ/` on Linux. Best practice on how to use `storage` alongside `profile` and what goes what is probably out of scope for this document.
```
-profile=profile
-profile=C:\Users\MyUser\MyDayZProfile
```
You explicitly interact with this directory from scripts like this:
```cpp
// Opens file inside the profile directory which locks the file and gives you a file handle to work with.
OpenFile("$profile:my_test_file.txt", FileMode.WRITE);
```
## server
Available on Diag Client. When developing you normally use the `DayZDiag_x64.exe` to get some diagnostics tools and support file patching and whatnot. This file normally starts a client but to launch it as a server you pass it the `-server` parameter. A Diag client can only connect to a Diag server so you need to manually copy the `DayZDiag_x64.exe` file from your DayZ client directory to your DayZ server directory before running it.
```
-server
```
## serverMod
Available on Server. This works just like the `-mod` parameter but it does not require the connecting clients to have those mods enabled. From the [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration#Launch_Parameters) "Loads the specified sub-folders for different server-side (not broadcasted to clients) mods. Separated by semi-colons. Absolute path and multiple stacked folders are possible.". This lets you load mods that only run on the server. Some mods are probably server only which is probably the most common usage of this but you can also use this to create a server-only variant of your mod if you want to hide some scripting completely from the clients. I don't know how easy it is to read binaries from downloaded mods but if you have some anti-cheat stuff you probably don't want to expose that to clients.
```powershell
# Relative paths, this will look for directories with this name inside the DayZServer directory.
-mod=@ZeroRevival;@CF;MySuperCoolMod
# Absolute paths, this will check the exact directory on disk for the mod
-mod=D:\DayZServer\@ZeroRevival;E:\SteamLibrary\steamapps\common\DayZ\!Workshop\@CF;E:\SteamLibrary\steamapps\common\DayZ\!Workshop\@Community-Online-Tools
```
## storage
Available on Server. According to the [Bohemia Wiki](https://community.bistudio.com/wiki/DayZ:Server_Configuration#Launch_Parameters) this "Defines custom root folder for storage location.". The storage is where the server state and persistance is stored and by default it's stored in the `DayZServerRoot\mpmissions\dayzOffline.chernarusplus\` directory as `Storage_1` (which can probably count up if you use multiple servers on the same machine). Best practice on how to use `storage` alongside `profile` and what goes what is probably out of scope for this document.
You explicitly interact with this directory from scripts like this:
```cpp
// Opens file inside the profile directory which locks the file and gives you a file handle to work with.
OpenFile("$saves:my_test_file.txt", FileMode.WRITE);
```
## window
Available on Client. Forces the game to start in a Window rather than reading what mode you last started.
```
-window
```
## world
Available on Client. Tells the game what world or map to load on startup. Not sure if this does anything nowadays but back in the days we used this to get a bit faster load times when joining servers from inside the game by setting this to `chernarusplus`. Basically it preloads a map upon starting rather than when you join a server.
```
-world=worldname
```
