// by normy

#include "common/hercules.h" /* Should always be the first Hercules file included! (if you don't make it first, you won't be able to use interfaces) */
#include "common/memmgr.h"
#include "common/mmo.h"
#include "common/socket.h"
#include "common/strlib.h"
#include "common/nullpo.h"
#include "common/HPMi.h"
#include "common/timer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map/clif.h"
#include "map/pc.h"
#include "map/channel.h"
#include "plugins/discordbot.h"
#include "plugins/HPMHooking.h"

#include "common/HPMDataCheck.h" /* should always be the last Hercules file included! (if you don't make it last, it'll intentionally break compile time) */


struct discord_bot_interface discord_bot_s;
struct discord_bot_interface *discord;

char send_string[DISCORD_MESSAGE_LENGTH];

HPExport struct hplugin_info pinfo = {
	"discordbot",
	SERVER_TYPE_MAP,
	"0.1",
	HPM_VERSION,
};

void discord_bot_init() {
	if (!(discord->ip = sockt->host2ip(discord->ip_name))) {
		ShowError("Unable to resolve '%s' (discord bridge), disabling discord channel...\n", discord->ip_name);
		return;
	}
	discord->channel = channel->create(HCS_TYPE_DISCORD, "discord", channel->config->irc_color); // placeholder color
	
	discord->fails = 0;
	discord->fd = 0;
	discord->isOn = false;
	
	timer->add_func_list(discord->connect_timer, "discord_connect_timer");
	timer->add(timer->gettick()+7000, discord->connect_timer, 0, 0);
}

int discord_connect_timer(int tid, int64 tick, int id, intptr_t data) {
	struct hSockOpt opt;
	if (discord->isOn || ++discord->fails >= 3)
		return 0;
	
	opt.silent = 0;
	opt.setTimeo = 0;
	
	if ((discord->fd = sockt->make_connection(discord->ip, 1337, &opt)) > 0) {
		sockt->session[discord->fd]->func_parse = discord->recv_api;
		sockt->session[discord->fd]->flag.server = 1;
		discord->isOn = true;
		sprintf(send_string, "Login to Bridge!");
		discord->send_api(send_string, true);
	}
	return 0;
}

void discord_bot_final() {
	//TODO
}

int discord_bot_recv_api(int fd) {
	//
	char *parse_string = NULL;

	
	if( !RFIFOREST(fd) )
		return 0;
	
	parse_string = aMalloc(RFIFOREST(fd));
	safestrncpy(parse_string, RFIFOP(fd,0), RFIFOREST(fd));
	RFIFOSKIP(fd, RFIFOREST(fd));
	RFIFOFLUSH(fd);

	ShowError("Received Message from '%s' (discord bridge)...\n", parse_string);
	discord->send_chn(parse_string);
	
	aFree(parse_string);
	return 0;
}

void discord_bot_send_api(char *str, bool force) {
	if (discord->isOn != true) {
		return;
	}
	
	size_t len;
	nullpo_retv(str);
	
	len = strlen(str) + 2;
	if (len > DISCORD_MESSAGE_LENGTH-3) {
		len = DISCORD_MESSAGE_LENGTH-3;
	}
	
	WFIFOHEAD(discord->fd, len);
	snprintf(WFIFOP(discord->fd,0), DISCORD_MESSAGE_LENGTH, "%s\r\n", str);
	WFIFOSET(discord->fd, len);
}

void discord_bot_send_channel(char *msg) {
	snprintf(send_string, 150, "< %s > %s ", "Placeholder", msg);
	clif->channel_msg2(discord->channel,send_string);
}

void discord_bot_recv_channel(struct map_session_data *sd, const char *msg) {
	nullpo_retv(sd);
	
	// TODO DEBUG PRINT
	char output[100];
	safesnprintf(output, 100, "%s: %s", sd->status.name, msg);
	clif->messagecolor_self(sd->fd, COLOR_RED, output);
	// TODO DEBUG END
	
	sprintf(send_string,"< %s > : %s", sd->status.name, msg);
	discord->send_api(send_string, false);
}

void channel_send_pre(struct channel_data **chan, struct map_session_data **sd, const char **msg) {
	if ((*chan)->type != HCS_TYPE_DISCORD) {
		return;
	}
	discord->recv_chn(*sd, *msg);
}

HPExport void server_online(void) {
	discord_bot_defaults();
	discord->init();
}

HPExport void plugin_final(void) {
	discord->final();
}

HPExport void plugin_init (void) {
	addHookPre(channel, send, channel_send_pre); // void channel_send(struct channel_data *chan, struct map_session_data *sd, const char *msg)
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
