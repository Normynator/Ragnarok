//Stats - Plugin
//by Normynator
//Beta Version
//Ignore ShowWarning since its used for debugging
//Ignore comments in beta version
//Ignore coding styl during beta
//Triggers by using script bonus3 99,X,X,X;

#include "common/hercules.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "common/HPMi.h"
#include "common/mmo.h"
#include "common/socket.h"
#include "common/memmgr.h"
#include "common/strlib.h"
#include "common/nullpo.h"
#include "common/timer.h"

#include "map/battle.h"
#include "map/script.h"
#include "map/pc.h"
#include "map/clif.h"
#include "map/status.h"
#include "map/npc.h"
#include "map/mob.h"
#include "map/map.h"

#include "plugins/HPMHooking.h"
#include "common/HPMDataCheck.h"

HPExport struct hplugin_info pinfo = {   
	"bAtkEnemyStat",
	SERVER_TYPE_MAP,
	"1.0",
	HPM_VERSION,
};

#define MAX_BONUS 10
int bAtkEnemyStat = -1;
			
struct stat_data_struct {
	int stat[MAX_BONUS];
	int rate[MAX_BONUS];
	int increase[MAX_BONUS];
	int size;
};

int pc_bonus3_pre(struct map_session_data **sd, int *type, int *type2, int *type3, int *val)
{
	struct stat_data_struct *data;
	
	if(*type == bAtkEnemyStat) {
		if(!(data = getFromMSD(*sd,0))){
			CREATE(data,struct stat_data_struct,1);
			data->stat[0] = *type2;
			data->rate[0] = *type3;
			data->increase[0] = *val;
			data->size = 1;
			addToMSD(*sd,data,0,true);
		}else{
			int i = data->size;
			if(i > MAX_BONUS){
				ShowError("pc_bonus3_pre: MAX_BONUS reached!\n");
				hookStop();
				return 0;
			}
			data->stat[i] = *type2;
			data->rate[i] = *type3;
			data->increase[i] = *val;
			data->size = data->size + 1;
		}
	}
	hookStop();
	return 0;
}

int status_calc_pc_pre(struct map_session_data **sd, enum e_status_calc_opt *opt)
{
	removeFromMSD(*sd,0);
	return 1;
}


int64 battle_calc_cardfix_post(int64 retVal, int attack_type, struct block_list *src, struct block_list *target, int nk, int s_ele, int s_ele_, int64 damage, int cflag, int wflag)
{
	if(retVal < 0){
		return retVal;
	}
	struct map_session_data *sd;
	struct stat_data_struct *data;
	struct status_data *tstatus;
	int value;
	int memincrease = 0;
	
	nullpo_ret(src);
	
	sd = BL_CAST(BL_PC, src);
	tstatus = status->get_status_data(target);
	
	if(src->type != 1)
		return retVal;
		
	if((data = getFromMSD(sd,0))){
		for(int i = 0; i < data->size; i++){
			switch(data->stat[i]){
				case 1:
					value = tstatus->str;
					break;
				case 2:
					value = tstatus->agi;
					break;
				case 3:
					value = tstatus->vit;
					break;
				case 4:
					value = tstatus->int_;
					break;
				case 5:
					value = tstatus->dex;
					break;
				case 6:
					value = tstatus->luk;
					break;
				default:
					value = 0;
					break;
			}
			if(value >= data->rate[i])
				memincrease += data->increase[i];
		}
	}
	retVal += retVal * memincrease / 100;
	return retVal;
}

HPExport void plugin_init(void)
{	
	bAtkEnemyStat = map->get_new_bonus_id();
	script->set_constant("bAtkEnemyStat", bAtkEnemyStat, false, false);
	addHookPre(status, calc_pc_, status_calc_pc_pre);
	addHookPre(pc, bonus3, pc_bonus3_pre);
	addHookPost(battle, calc_cardfix, battle_calc_cardfix_post);
}

HPExport void server_online (void) {
	ShowInfo ("'%s' Plugin by Normynator/Hercules. Version '%s'\n",pinfo.name,pinfo.version);
}
