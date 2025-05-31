#include "er_std_override.h"
#include "csa_web_mailbox.hpp"

void MAILbox_tab::LoadList(void)
{
	DBManager_c		&db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
	RetCode_t		bswrc = BSWRC_OK,executionResult;
	Statement_c	st;
	ResultSet_c rs;
	bool			dbirc = true, no_records = true;
	char sql_statement[MAX_SQL_QUERY_SIZE];

	Id_type              Id ;
    char                       Name[51] ;
    

	er_sprintf(sql_statement,"select mail_acc_id,mail_acc_nick_name from mail_accounts");

	st.SetQuery(sql_statement);
	bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);
   	
	Ushort_t total_records_num = rs.GetRecordCount(); 
	Ushort_t left_records_num = rs.GetRecordCount(); 

    // CLEAN ALL Items from the map
	tab.clear();


	if (total_records_num == 0)
	{
			//NOTHING
		}
	else
	{
		while(left_records_num > 0)
		{ 			
			//start to fill buffer with NlaGroupEntityInfo structures
			while(rs.Next() && bswrc == BSWRC_OK)
			{ 
    		if (!(rs.GetUlongByName("mail_acc_id",(Ulong_t &)Id))   ||
			!(rs.GetStringByName("mail_acc_nick_name",Name)))
				{
					//do nothing. get next result set
					left_records_num--;
				}
				else
				{		
					tab[Id]=Name;
					left_records_num--;
				//if there are more info records to insert 				
				if (left_records_num == 0)
				break;
				}
			}
          
		}
	}
}
//--------------------------
string MAILbox_tab::GetName(unsigned int Id)
	{ string s = "";
	  if(tab.find(Id) != tab.end())
		  return tab.find(Id)->second;
	  else 
		return s;
	}
//--------------------------
bool MAILbox_tab::isMonitor(void)
	{
		if(isMonitored)
			return true;
     isMonitored = true;
	 return false;
	}
