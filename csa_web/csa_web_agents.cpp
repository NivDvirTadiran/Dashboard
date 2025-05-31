#include "er_std_override.h"
#include "csa_web_agents.hpp"
#include "csa_web_main.h"

void Agents_tab::LoadAgentsList(void)
{	
	DBManager_c		&db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
	RetCode_t		bswrc = BSWRC_OK,executionResult;
	Statement_c	st;
	ResultSet_c rs;
	bool			dbirc = true, no_records = true;
	char sql_statement[MAX_SQL_QUERY_SIZE];

    unsigned int               a_id ;
    char                       a_name[51] ;
    char                       a_number[51];
    

    er_sprintf(sql_statement,"select agent_id, a_name,a_number from agent where agent_status = 'a' order by agent_id"); //only active agents
	
       
   st.SetQuery(sql_statement);
   bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);
			
  
   Ushort_t total_records_num = rs.GetRecordCount(); 
   Ushort_t left_records_num = rs.GetRecordCount(); 

  	if (total_records_num == 0)
		{
			//NOTHING
		
		}
	else
	{
	       while(left_records_num > 0)
			{ 								    									
			while(rs.Next() && bswrc == BSWRC_OK)
			{ 
				if (!(rs.GetUlongByName("agent_id",(Ulong_t &)a_id))   ||
				!(rs.GetStringByName("a_name",a_name))||
				!(rs.GetStringByName("a_number",(char *)a_number)))
				{

					//do nothing. get next result set
					//GetLog() << " AdiMngr_c::HandleAgentGroupListReply - opcode: Capp_Dbi_get_lst_grp_entities_req_op_code\n"
					//	<< " Can't get record from result set\n"<< LOG_TERMINATOR;
					left_records_num--;
				}
				else
				{		
					tab[a_id].a_id = a_id;
					tab[a_id].a_name = (char *)a_name;
					tab[a_id].a_number = (char *)a_number;	;
					
				     left_records_num--;
				//if there are more info records to insert 				
				if (left_records_num == 0)
				break;
				}
			}
          
		}
	}	
	
	return ;
}
