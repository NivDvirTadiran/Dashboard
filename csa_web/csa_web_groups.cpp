#include "er_std_override.h"
#include "csa_web_groups.hpp"
#include "csa_web_main.h"

static BswLog_c csa_web_groups_log_d(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_DETAILED);
static BswLog_c csa_web_groups_log_i(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_INFO);

void Groups_tab::LoadGroupList(Ushort_t user_id, Ushort_t sup_level)
{
  DBManager_c&    db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t       bswrc = BSWRC_OK,executionResult;
  Statement_c     st;
  ResultSet_c     rs;
  bool            dbirc = true, no_records = true;
  char sql_statement[MAX_SQL_QUERY_SIZE];

  unsigned int               grpId ;
  char                       grpName[51] ;
  char                       grpNumber[51];
  unsigned int               grpEmailEnable; 


  //20-Dec-2020 YR BZ#53247
  if((sup_level == 0xffff) || (sup_level <= 3))
  {
    er_sprintf(sql_statement,"select group_id, g_name, g_number, email_enabled from grp");
  }
  else
  {
    //string sql_grp_base = "SELECT group_id, g_name, g_number, email_enabled FROM grp WHERE group_id IN";
    //string sql_grp_id_in_grp_sup = "SELECT group_id FROM grp_sup WHERE user_id = 26 AND g_type = 'g'";
    //string sql_grp_id_in_grp_s_grp = "SELECT group_id FROM grp_s_grp WHERE sgroup_id IN (SELECT sgroup_id FROM s_grp WHERE sgroup_id IN (SELECT group_id FROM grp_sup WHERE user_id = 26 AND g_type = 's')))";
    er_sprintf(sql_statement,"SELECT group_id, g_name, g_number, email_enabled FROM grp WHERE group_id IN (SELECT group_id FROM grp_sup WHERE user_id = %d AND g_type = 'g' UNION SELECT group_id FROM grp_s_grp WHERE sgroup_id IN (SELECT sgroup_id FROM s_grp WHERE sgroup_id IN (SELECT group_id FROM grp_sup WHERE user_id = %d AND g_type = 's')))",user_id,user_id);
  }

  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);

  Ushort_t total_records_num = rs.GetRecordCount(); 
  Ushort_t left_records_num = rs.GetRecordCount(); 

  csa_web_groups_log_d << "Groups_tab::LoadGroupList <";

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
        if (!(rs.GetUlongByName("group_id",(Ulong_t &)grpId))          ||
            !(rs.GetStringByName("g_name",grpName))                    ||
            !(rs.GetStringByName("g_number",(char *)grpNumber))        ||
            !(rs.GetUlongByName("email_enabled",(Ulong_t &)grpEmailEnable)))
        {
          //do nothing. get next result set
          left_records_num--;
        }
        else
        {
          csa_web_groups_log_d << grpName << ":" << grpNumber << "";
          tab[grpId].grpEmailEnable = grpEmailEnable;
          tab[grpId].grpName =(char *) grpName;
          tab[grpId].grpNumber =(char *)grpNumber;

          left_records_num--;
          //if there are more info records to insert
          if (left_records_num == 0)
            break;
          else
            csa_web_groups_log_d << ",";
        }
      }
    }
  }
  csa_web_groups_log_d << ">" << LOG_TERMINATOR;
}


string Groups_tab::GetGroupNameById(unsigned int Id)
{
  string s = "";
  if(tab.find(Id) != tab.end())
    return tab.find(Id)->second.grpName.data();
  else 
    return s;
}


string Groups_tab::GetGroupNumberById(unsigned int  Id)
{
  string s = "";
  if(tab.find(Id) != tab.end())
    return tab.find(Id)->second.grpNumber.data();
  else 
    return s;
}


bool Groups_tab::IsGroupExist(unsigned int grpId)
{
  if(tab.find(grpId) != tab.end())
    return true;
  return false;
}
