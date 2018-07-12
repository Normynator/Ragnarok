# roDiscordBot for rAthena
This bot communitcation between discord and ingame chat.
The bot prints everything written to #discord into the selected discord server and vice versa.

!!! This is a beta version, somethings might not work as intended and crashes could occure !!!
(See todo or issues)

## Features:
- Basic Discord to ingame channel and ingame channel to Discord.
- Script command, which sends a message to the discord server.
- Supports "rare_drop_announce". (Idea: Stolao)

## Install:
1. copy discordbot.cpp/.h into src/map/
2. apply diffs (can be found in diffs/) onto existing files in src/
3. (optional) configure channel
4. configure bot by changing index.js (you need to providea token and define a discord channel id)
5. start bot before starting rathena.

## How to configure:
Basic Config:
```
	char* name = "#discord";     
	char* alias = "[Discord]";
	tmp_chn.color = channel_getColor("Blue");
```
(To see supported colors check channel.conf)

## Script commands:
Example scirpt: TODO

```
Command: 
  discord(<string>);
Sends a message to discord.
```
## Ideas:
- Multi channel support.

## TODO:
- Remove \<Username\> palceholder.
- Fix |00 message bug. (Currently fixed by a workarround)

## Working with:
rev 042b886
