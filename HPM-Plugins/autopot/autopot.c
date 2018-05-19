// BY NOIL and bit of Normy

#include "common/hercules.h" /* Should always be the first Hercules file included! (if you don't make it first, you won't be able to use interfaces) */
#include "common/memmgr.h"
#include "common/mmo.h"
#include "common/socket.h"
#include "common/strlib.h"
#include "common/nullpo.h"
#include "common/HPMi.h"
#include "common/timer.h"

#include "map/clif.h"
#include "map/pc.h"
#include "map/script.h"
#include "map/itemdb.h"
#include "map/npc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "plugins/autopot.h"
#include "plugins/HPMHooking.h"

#include "common/HPMDataCheck.h" /* should always be the last Hercules file included! (if you don't make it last, it'll intentionally break compile time) */

HPExport struct hplugin_info pinfo = {
	"autopot",
	SERVER_TYPE_MAP,
	"0.1",
	HPM_VERSION,
};

ACMD(autopot_cmd) {//@sample command - 5 params: const int fd, struct map_session_data* sd, const char* command, const char* message, struct AtCommandInfo *info
	
	int buffer_size = 50;
	struct player_config_struct *config;
	char output[buffer_size]; //Fix me, buffer to large
	char type_str[] = "hp"; //fix me
	int border = 90;
	int item_id = -1;
	int state = 0;
	
	nullpo_ret(sd);
	
	sscanf(message, "%1d %2s %2d %d", &state, type_str, &border, &item_id); //fix me
	
	if (!(config = getFromMSD(sd,0))) {
		CREATE(config,struct player_config_struct,1);
	}
	
	if (strcasecmp("hp", type_str) == 0) {
		config->type = 0;
	} else if (strcasecmp("sp", type_str) == 0) {
		config->type = 1;
	} else {
		safesnprintf(output, buffer_size, "Please use 'sp' or 'hp'"); //fix me, change to buffer size
		clif->messagecolor_self(sd->fd, COLOR_RED, output);
		return false;
	}
	
	config->state = state;
	config->border = border;
	config->item = item_id;
	config->timer = 0;
	addToMSD(sd,config,0,true);
	
	// TODO: Debug output
	clif->message(fd, message);
	// TODO: end debug output
	return true;
}

int item_get_pos(struct map_session_data *sd, int item) {
	int inv_pos = -1;
	int i = 0;
	
	for (i = 0; i<MAX_INVENTORY; i++) {
		if (sd->status.inventory[i].nameid == item) {
			inv_pos = i;
			break;
		}
	}
	if (inv_pos == -1) {
		return -1; //fix me, return is not checked by callee
	}
	return inv_pos;
}

int autopot_timer(int tid, int64 tick, int id, intptr_t data) {
	struct block_list *bl;
	struct map_session_data *sd;
	struct player_config_struct *config;
	
	bl = map->id2bl(id);
	nullpo_ret(bl);
	
	sd = BL_UCAST(BL_PC, bl);
	nullpo_ret(sd);
	
	if ((config = getFromMSD(sd,0))) {
		//pc->useitem(sd,item_get_pos(sd, config->item));
		config->timer = 0;
		autopot(sd, -1, -1);
		return 0;
	}
	return 1;
}

void autopot(struct map_session_data *sd, int hp, int sp)
{
	/* TODO:
	 * - check item exist
	 * - string for on and off
	 * - impl border
	 * - break if no item is left
	 */
	//int buffer_size = 100; //fix me
	//char output[buffer_size]; //Fix me, buffer to large
	struct player_config_struct *config;
	struct item_data *item_data;
	
	nullpo_retv(sd);
	
	if ((config = getFromMSD(sd,0))) {
		if (sd->battle_status.hp == 0) {
			return;
		}
		if ((config->type == 0 && hp == 0) || (config->type == 1 && sp == 0)) {
			return;
		}
		if (config->state == 0) {
			return;
		}
		if (config->type == 0 && sd->battle_status.hp == sd->battle_status.max_hp) {
			return;
		}
		
		if ((item_data = itemdb->exists(config->item)) == NULL ) {
			ShowError("plugin_autopot: Nonexistant item %d requested.\n", config->item );
			return;
		}
		
		pc->useitem(sd, item_get_pos(sd, config->item));
		if (config->timer == 0) {
			timer->add(timer->gettick()+tick_freq, autopot_timer, sd->bl.id,0);
			config->timer = 1;
		}
	}
	return;
}

void itemdb_readdb_additional_fields_pre(int *itemid, struct config_setting_t **it, int *n, const char **source)
{
	struct item_data* item;
	item = itemdb->array[*itemid];
	if( item == NULL )
		return;
		
	//addToITEMDATA(item, data, 0, true);
}

int status_damage_post(int retVal, struct block_list *src,struct block_list *target,int64 in_hp, int64 in_sp, int walkdelay, int flag)
{
	struct map_session_data *sd;
	
	nullpo_ret(target);
	if (target->type != BL_PC) {
		return retVal;
	}
	
	sd = BL_UCAST(BL_PC, target);
	
	if ((sd)->state.dead_sit == 1) {
		return retVal;
	}

	autopot(sd, in_hp, in_sp); //fix me, int64 to int
	return retVal;
}

HPExport void plugin_init (void) {
	addAtcommand("autopot", autopot_cmd);
	addHookPost(status, damage, status_damage_post);
	addHookPre(itemdb, readdb_additional_fields, itemdb_readdb_additional_fields_pre); // hook db reader
}
