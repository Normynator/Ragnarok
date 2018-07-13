# How to install roCORD

It is important to use a supported rAthena version else this could fail.
If you have problems ask here or in rAthena forum and please provide an error log.

## SRC PART

$ cd

$ git clone https://github.com/Normynator/Ragnarok.git

$ cd /path/to/your/rathena/

$ cd src/map/

// create backups
$ cp channel.cpp channel.cpp.bk
$ cp script.cpp script.cpp.bk
$ cp mob.cpp mob.cpp.bk

//apply diffs
$ patch channel.cpp ~/Ragnarok/rA_mods/discordbot/diffs/channel.diff
$ patch script.cpp ~/Ragnarok/rA_mods/discordbot/diffs/script.diff
$ patch mob.cpp ~/Ragnarok/rA_mods/discordbot/diffs/mob.diff

//copy bot files
$ cp ~/Ragnarok/rA_mods/discordbot/discordbot.h .
$ cp ~/Ragnarok/rA_mods/discordbot/discordbot.cpp .

//go back to rA root
$ cd ../../

//check server is running
$ ./athena-start status
Ex Output:
'login-server' seems to be down
'char-server' seems to be down
'map-server' seems to be down

If not:
$ ./athena-start stop

//Recompile
$ ./configure
$ make server

(* if you have an compile ERROR post it in forum.*)
(* you can ignore WARNINGS for now. *)


## BRIDGE PART

$ sudo apt-get update
$ sudo apt-get install nodejs
$ sudo apt-get install npm

