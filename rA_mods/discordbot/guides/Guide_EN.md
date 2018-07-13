# How to install roCORD
Thanks Vegas for providing a server setup for me!

It is important to use a supported rAthena version else this could fail.
If you have problems ask here or in rAthena forum and please provide an error log.

## SRC PART
Go into your home directory.
```console
$ cd
```

Clone the repo.
```console
$ git clone https://github.com/Normynator/Ragnarok.git
```

Go to your rAthena installation. Replace /path/to/your/rathena with the path to your rAthena folder. And go into the src area.
```console
$ cd /path/to/your/rathena/
$ cd src/map/
```
Create backups of the modified files in case something bad happens!
```console
$ cp channel.cpp channel.cpp.bk
$ cp script.cpp script.cpp.bk
$ cp mob.cpp mob.cpp.bk
```

Since we have backups no we don't need to worry about modifiying your src files and just apply the provided diffs.
```console
$ patch channel.cpp ~/Ragnarok/rA_mods/discordbot/diffs/channel.diff
$ patch script.cpp ~/Ragnarok/rA_mods/discordbot/diffs/script.diff
$ patch mob.cpp ~/Ragnarok/rA_mods/discordbot/diffs/mob.diff
```

src modification was done in the previous step. Now we are going to add the discordbot to our src files.
```console
$ cp ~/Ragnarok/rA_mods/discordbot/discordbot.h .
$ cp ~/Ragnarok/rA_mods/discordbot/discordbot.cpp .
```

src changes are finally done. Let's go back to the rAthena root folder.
```console
$ cd ../../
```

First we need to check if the server is still running.
```console
$ ./athena-start status
```
If see this output you can go to "Recompile".
```
'login-server' seems to be down
'char-server' seems to be down
'map-server' seems to be down
```

else stop your rAthena server.
```console
$ ./athena-start stop
```

#### Recompile: 
Now it's time to recompile your rAthena server.
```console
$ ./configure
$ make server
```
(* if you have an compile ERROR post it in forum.*)

(* you can ignore WARNINGS for now. *)


## BRIDGE PART
Make sure to have an updateted system and install nodejs and npm.
```console
$ sudo apt-get update
$ sudo apt-get install npm
$ sudo apt-get install curl
$ sudo apt-get install screen

$ curl -sL https://deb.nodesource.com/setup_10.x | sudo -E bash -
$ sudo apt-get install -y nodejs

$ npm install -S discord.js

$ mkdir bridge
$ cd bridge

$ cp ~/Ragnarok/rA_mods/discordbot/bridge/*.js* .
```

## DISCORD PART

To generate a discord bot you have to visit: https://discordapp.com/developers/applications/me

![alt text](https://i.imgur.com/2s5jFDK.png)

Click on new app to create a bot. Choose the name of your bot and continue.

Next you have to scroll down to create an actual bot user.

![alt text](https://i.imgur.com/IR8oa3h.png)

To get your token click on "click to reveal". You will need your token later (keep it secret).

![alt text](https://i.imgur.com/hOctUMR.png)

Now to give the bot some permission click on "Generate OAuth2 URL".

Here you can choose the permissions for your bot and when your are done, go and copy that link!

![alt text](https://i.imgur.com/vI9Eakb.png)

Follow your saved link and you'll be able to add your bot to a server!

As a last step you need to choose a channel and get it's ID.
![alt text](https://i.imgur.com/hwXPTL9.png)
Click on the last option: Copy ID.

For the last step you will need the Channel ID and your secret token.

## FINAL PART

Go back to your server.

```console
$ vi index.js
```

Now use Secret Token, Channel ID and Channel Name as shown in the picture. (Channel name without #)
![alt text](https://i.imgur.com/HXZsvOQ.png)

```console
$ screen
$ node index.js
```

Than press Ctrl + A + D and start your rAthena server.

Done!
