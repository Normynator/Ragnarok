
// 0 = Off
// 1 = HP
// 2 = SP
int TEMP_DEBUG_SP_HP = 1;

/**
 * Player config
 * state: 1 = On, 0 = Off
 * border: % where the effect should trigger
 * type: 0 = HP, 1 = SP
 * item: item id which should be used
 **/
struct player_config_struct {
	int state;
	int border;
	int type;
	int item;
	int timer;
};

// How frequently should the autopot trigger?
int tick_freq = 100; // default value of battle config is 100

int autopot_timer(int tid, int64 tick, int id, intptr_t data);
void autopot(struct map_session_data *sd, int hp, int sp);
void itemdb_readdb_additional_fields_pre(int *itemid, struct config_setting_t **it, int *n, const char **source);
int status_damage_post(int retVal, struct block_list *src,struct block_list *target,int64 in_hp, int64 in_sp, int walkdelay, int flag);
int item_get_pos(struct map_session_data *sd, int item);
