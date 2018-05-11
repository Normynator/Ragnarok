//
//  discordbot.h
//  Hercules
//
//  Created by Norman Ziebal on 07.05.18.
//  Copyright © 2018 herc.ws. All rights reserved.
//

#ifndef discordbot_h
#define discordbot_h

#include <stdio.h>

#define DISCORD_MESSAGE_LENGTH 500

enum channel_types HCS_TYPE_DISCORD = 5;

struct discord_bot_interface {
	int fd;
	bool isOn;
	uint32 ip;
	char* ip_name;
	unsigned short port;
	int64 last_try;
	unsigned char fails;
	struct channel_data *channel;
	
	/**
	 * Bot initializer
	 */
	void (*init) (void);
	
	/**
	 *  Bot finalizer
	 */
	void (*final) (void);
	
	/**
	 * Parser for the Discord API connection
	 * @see do_sockets
	 */
	int (*recv_api) (int fd);
	
	//DOC
	void (*send_api) (char *str, bool force);
	
	//DOC
	void (*recv_chn) (struct map_session_data *sd, const char *msg);
	
	//DOC
	void (*send_chn) (void);
	
	//DOC
	int(*connect_timer) (int tid, int64 tick, int id, intptr_t data);
	
};

void discord_bot_defaults(void);

#endif /* discordbot_h */
