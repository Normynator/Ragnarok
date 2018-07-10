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

struct discord_bot_interface discord_bot_s;
struct discord_bot_interface *discord;

char send_string[DISCORD_MESSAGE_LENGTH];


void do_init_discord() {
    discord_bot_defaults();
    discord->init();
}

void discord_bot_init() {
    // TODO
}

int discord_connect_timer(int tid, int64 tick, int id, intptr_t data) {
    // TODO
    return -1;
}

void discord_bot_final() {
    // TODO
}

int discord_bot_recv_api(int fd) {
    // TODO
    return 0;
}

void discord_bot_send_api(char *str, bool force) {
    // TODO
}

void discord_bot_send_channel(char *msg) {
   // TODO
}

void discord_bot_recv_channel(struct map_session_data *sd, const char *msg) {
    // TODO
}

void discord_bot_hook(struct Channel *channel, struct map_session_data *sd, const char *msg) {
    // TODO
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
