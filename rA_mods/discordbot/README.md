# roDiscordBot for rAthena
This bot communitcation between discord and ingame chat.
The bot prints everything written to #discord into the selected discord server and vice versa.

## Features:
- Basic Discord to ingame channel and ingame channel to Discord.
- Script command, which sends a message to the discord server.
- Supports "rare_drop_announce". (Idea: Stolao)

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

## Working with:
rev 042b886
