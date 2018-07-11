//
// Created by Norman Ziebal on 10.07.18.
//
#include "discordbot.h"
// by normy

#include "../common/mmo.hpp"
#include "../common/socket.hpp"
#include "../common/strlib.hpp"
#include "../common/nullpo.hpp"
#include "../common/timer.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../map/clif.hpp"
#include "../map/pc.hpp"
#include "../map/channel.hpp"
#include "../common/showmsg.hpp"
#include "../common/socket.hpp"

struct discord_bot_interface discord_bot_s;
struct discord_bot_interface *discord;

char send_string[DISCORD_MESSAGE_LENGTH];


void do_init_discord() {
    discord_bot_defaults();
    discord->init();
	ShowInfo("Successfully loaded Discord Bot\n");
}

void discord_bot_init() {
	if (!(discord->ip = host2ip(discord->ip_name))) {
		ShowError("Unable to resolve %s (discord bridge)!", discord->ip_name);
		return;
	}
	discord->channel = channel_create_simple("discord", NULL, CHAN_TYPE_PUBLIC, NULL);

	discord->fails = 0;
	discord->fd = 0;
	discord->isOn = false;

	add_timer_func_list(discord->connect_timer, "discord_connect_timer");
	add_timer(gettick() + 7000, discord->connect_timer, 0, 0);
}

int discord_connect_timer(int tid, unsigned int tick, int id, intptr_t data) {
	if (discord->isOn || ++discord->fails >= 3) {
		return -1;
	}

	if ((discord->fd = make_connection(discord->ip, 1337, 0, 0)) > 0) {
		session[discord->fd]->func_parse = discord->recv_api;
		session[discord->fd]->flag.server = 1;
		discord->isOn = true;
		sprintf(send_string, "Login to Bridge!");
		discord->send_api(send_string, true);
	}
	return 0;
}

void discord_bot_final() {
    // TODO
	// do some cleanup!
}

int discord_bot_recv_api(int fd) {
	// TODO
    return 0;
}

void discord_bot_send_api(char *str, bool force) {
	if (discord->isOn != true) {
		return;
	}

	size_t len;
	nullpo_retv(str);

	len = strlen(str) + 2;
	if (len > DISCORD_MESSAGE_LENGTH - 3) {
		len = DISCORD_MESSAGE_LENGTH - 3;
	}

	WFIFOHEAD(discord->fd, len);
	sprintf(send_string, "%s\r\n", str);
	memcpy(WFIFOP(discord->fd, 0), send_string, DISCORD_MESSAGE_LENGTH);
	WFIFOSET(discord->fd, len);
}	

void discord_bot_send_channel(char *msg) {
   // TODO
}

void discord_bot_recv_channel(struct map_session_data *sd, const char *msg) {
	nullpo_retv(sd);

	sprintf(send_string, "< %s > : %s", sd->status.name, msg);
	discord->send_api(send_string, false);
}

void discord_bot_hook(struct Channel *channel, struct map_session_data *sd, const char *msg) {
	if (channel->name != "discord") {
		return;
	}
	discord->recv_chn(sd, msg);
	
}

void discord_bot_defaults(void) {
    discord = &discord_bot_s;

    discord->ip_name = "127.0.0.1";
    discord->port = 1337;
    discord->isOn = false;
    discord->channel = NULL;

    discord->init = discord_bot_init;
    discord->final = discord_bot_final;
    discord->recv_api = discord_bot_recv_api;
    discord->send_api = discord_bot_send_api;
    discord->send_chn = discord_bot_send_channel;
    discord->recv_chn = discord_bot_recv_channel;
    discord->connect_timer = discord_connect_timer;
}
