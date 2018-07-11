//
// Created by Norman Ziebal on 10.07.18.
//

#ifndef RATHENA_DISCORDBOT_H
#define RATHENA_DISCORDBOT_H

#include <stdio.h>
#include <stdint.h>

#define DISCORD_MESSAGE_LENGTH 500
typedef uint32_t uint32;
typedef int64_t int64;

struct discord_bot_interface {
    int fd;
    bool isOn;
    uint32 ip;
    char* ip_name;
    unsigned short port;
    int64 last_try;
    unsigned char fails;
    struct Channel *channel;

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
    void (*send_chn) (char* msg);

    //DOC
    int(*connect_timer) (int tid, unsigned int tick, int id, intptr_t data);
};

int discord_connect_timer(int tid, unsigned int tick, int id, intptr_t data);
void discord_bot_final(void);
int discord_bot_recv_api(int fd);
void discord_bot_send_api(char *str, bool force);
void discord_bot_send_channel(char *msg);
void discord_bot_recv_channel(struct map_session_data *sd, const char *msg);
void discord_bot_hook(struct Channel *channel, struct map_session_data *sd, const char *msg);

void do_init_discord(void);
void discord_bot_defaults(void);


#endif //RATHENA_DISCORDBOT_H
