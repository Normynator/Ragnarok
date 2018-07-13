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
#include "../common/malloc.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../map/clif.hpp"
#include "../map/pc.hpp"
#include "../map/channel.hpp"
#include "../common/showmsg.hpp"
#include "../common/socket.hpp"

#define DEBUG

struct discord_bot_interface discord_bot_s;
struct discord_bot_interface *discord;

char send_string[DISCORD_MESSAGE_LENGTH];


void do_init_discord() {
    discord_bot_defaults();
    discord->init();
	ShowInfo("Successfully loaded Discord Bot\n");
}

void discord_bot_init() {
	struct Channel tmp_chn;
	char* name = "#discord";
	char* alias = "[Discord]";

	memset(&tmp_chn, 0, sizeof(struct Channel));

	if (!(discord->ip = host2ip(discord->ip_name))) {
		ShowError("Unable to resolve %s (discord bridge)!\n", discord->ip_name);
		return;
	}
	
	safestrncpy(tmp_chn.name, name + 1, sizeof(tmp_chn.name));
	safestrncpy(tmp_chn.alias, alias, sizeof(tmp_chn.alias));
	tmp_chn.pass[0] = '\0';
	tmp_chn.type = CHAN_TYPE_PUBLIC;
	tmp_chn.color = channel_getColor("Blue");
	tmp_chn.opt = CHAN_OPT_BASE;
	tmp_chn.msg_delay = 1000;

	discord->channel = channel_create(&tmp_chn);
	if (discord->channel == NULL) {
		ShowError("Discord: Channel creation failed!\n");
	}

	discord->fails = 0;
	discord->fd = 0;
	discord->isOn = false;

	add_timer_func_list(discord->connect_timer, "discord_connect_timer");
	discord->connect_timer_id = add_timer(gettick() + 7000, discord->connect_timer, 0, NULL);
}

int discord_connect_timer(int tid, unsigned int tick, int id, intptr_t data) {
#ifdef DEBUG
	ShowDebug("Calling discord_connect_timer\n");
#endif
	if (discord->isOn) {
		return 0;
	}

	if (++discord->fails > 3) {
		ShowError("Unable to restart bot, bridge seems down! Tried to reconnect %d times!\n", discord->fails-1);
		return 0;
	}

	if ((discord->fd = make_connection(discord->ip, discord->port, 0, 0)) > 0) {
		session[discord->fd]->func_parse = discord->recv_api;
		session[discord->fd]->flag.server = 1;
		discord->isOn = true;
		sprintf(send_string, "Login to Bridge!");
		discord->send_api(send_string, true);
	} else {
		discord->connect_timer_id = add_timer(gettick() + 60000, discord->connect_timer, 0, NULL); //wait 60 seconds before reconnecting
	}
	return 0;
}

void discord_bot_final() {
	if (discord->isOn) {
		do_close(discord->fd);
	}
	discord->fd = 0;
	discord->isOn = false;
	discord->fails = 0;
}

int discord_bot_recv_api(int fd) {
	char *parse_string = NULL;

	if (session[discord->fd]->flag.eof) {
		ShowError("Connection from Bridge was closed!\n");
		discord_bot_final(); // reset bot and try to reconnect.
		discord->connect_timer_id = add_timer(gettick() + 1000, discord->connect_timer, 0, NULL);
		return 0;
	}

	if (!RFIFOREST(fd))
		return 0;

	parse_string = (char *)aMalloc(RFIFOREST(fd));
	safestrncpy(parse_string, (const char*)RFIFOP(fd, 0), RFIFOREST(fd));
	RFIFOSKIP(fd, RFIFOREST(fd));
	RFIFOFLUSH(fd);

	discord->send_chn(parse_string);

	aFree(parse_string);
	return 0;
}

inline void discord_bot_send_api(const char *str, bool force) {
	if (discord->isOn != true) {
		return;
	}

	size_t len;
	nullpo_retv(str);

	len = strlen(str);

	if (len > DISCORD_MESSAGE_LENGTH - 1) {
		len = DISCORD_MESSAGE_LENGTH - 1;
	}

	WFIFOHEAD(discord->fd, len);
	snprintf((char *)WFIFOP(discord->fd, 0), DISCORD_MESSAGE_LENGTH, str);
	WFIFOSET(discord->fd, len);
}	

inline void discord_bot_send_channel(const char *msg) {
	snprintf(send_string, 150, " %s%s", discord->channel->alias, msg);
	clif_channel_msg(discord->channel, send_string, discord->channel->color);
}

void discord_bot_recv_channel(struct map_session_data *sd, const char *msg) {
	nullpo_retv(sd);
	snprintf(send_string, 150, "<%s>: %s", sd->status.name, msg);
	discord->send_api(send_string, false);
}

void discord_bot_join_hook(struct Channel *channel, struct map_session_data *sd) {
	if (strcmp(channel->name, "discord") != 0) {
		return;
	}
	char output[CHAT_SIZE_MAX];
	snprintf(output, CHAT_SIZE_MAX, "Welcome to %s.\nMessages from discord users are marked with <Username>!", discord->channel->alias);
	clif_displaymessage(sd->fd, output);
}

void discord_bot_script_hook(const char *msg) {
	discord->send_api(msg, false);
}

void discord_bot_hook(struct Channel *channel, struct map_session_data *sd, const char *msg) {
	if (strcmp(channel->name, "discord") != 0) {
		return;
	}
#ifdef DEBUG
	ShowDebug("Discord: Received: %s , with len %d \n", &msg[3], strlen(msg));
#endif
#ifdef WIN32
	discord->recv_chn(sd, &msg[3]); // because all messages from channels start with |00 for an unknown reason.
#else
	discord->recv_chn(sd, msg);
#endif
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
	discord->connect_timer_id = INVALID_TIMER;
}
