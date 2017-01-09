#include "common/hercules.h" /* Should always be the first Hercules file included! (if you don't make it first, you won't be able to use interfaces) */
#include "common/memmgr.h"
#include "common/mmo.h"
#include "common/socket.h"
#include "common/strlib.h"
#include "map/clif.h"
#include "map/pc.h"
#include "map/script.h"

//----------
#include "common/nullpo.h"
#include "common/sql.h"
//----------

#include "plugins/HPMHooking.h"
#include "common/HPMDataCheck.h" /* should always be the last Hercules file included! (if you don't make it last, it'll intentionally break compile time) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HPExport struct hplugin_info pinfo = {
	"FluxAddon - InGameShop",    // Plugin name
	SERVER_TYPE_MAP,// Which server types this plugin works with?
	"1.0",       // Plugin version
	HPM_VERSION, // HPM Version (don't change, macro is automatically updated)
};

int pc_reg_received_post(int retVal, struct map_session_data *sd)
{
	if(retVal != 1){
		return retVal;
	}
	nullpo_ret(sd);
	char *data;
	int value = -1;
		
	if (SQL_ERROR == SQL->Query(map->mysql_handle, "SELECT `balance` FROM `cp_credits` WHERE `account_id` = '%d'", sd->status.account_id)){
		Sql_ShowDebug(map->mysql_handle);	
	}
						
	while (SQL_SUCCESS == SQL->NextRow(map->mysql_handle)) {
		SQL->GetData(map->mysql_handle, 0, &data, NULL); value = atoi(data);
	}
	
	SQL->FreeResult(map->mysql_handle);
	
	if(value < 1){
		value = 0;
	}
	sd->cashPoints = value;
	
	return 1;
}


int pc_paycash_post(int retVal, struct map_session_data *sd, int price, int points)
{
	if(retVal < 0){
		return retVal;
	}
	
	if (SQL_ERROR == SQL->Query(map->mysql_handle, "UPDATE `cp_credits` SET  `balance` = '%d' WHERE `account_id` = '%d'",sd->cashPoints, sd->status.account_id)){
		Sql_ShowDebug(map->mysql_handle);
	}				
	SQL->FreeResult(map->mysql_handle);	

	return retVal;
}

int pc_getcash_post(int retVal, struct map_session_data *sd, int cash, int points)
{
	if(retVal < 0){
		return retVal;
	}
	
	if (SQL_ERROR == SQL->Query(map->mysql_handle, "INSERT INTO `cp_credits` (`account_id`, `balance`) VALUES ('%d', '%d') ON DUPLICATE KEY UPDATE `balance`=VALUES(`balance`)", sd->status.account_id, sd->cashPoints)){
		Sql_ShowDebug(map->mysql_handle);
	}				
	SQL->FreeResult(map->mysql_handle);
	
	return retVal;
	
}

HPExport void plugin_init(void) {
		addHookPost(pc, reg_received, pc_reg_received_post);
		addHookPost(pc, paycash, pc_paycash_post);
		addHookPost(pc, getcash, pc_getcash_post);
}

HPExport void server_online (void) {
	ShowInfo ("'%s' Plugin by Normynator/Hercules. Version '%s'\n",pinfo.name,pinfo.version);
}
