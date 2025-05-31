#include "er_std_override.h"
#include "csa_web_sg.h"

static BswLog_c csa_web_sg_log_d(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_DETAILED);
static BswLog_c csa_web_sg_log_i(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_INFO);

void SuperGroups_tab::LoadList(Ushort_t user_id, Ushort_t sup_level)
{
  DBManager_c&  db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t     bswrc = BSWRC_OK,executionResult;
  Statement_c   st;
  ResultSet_c   rs;
  bool          dbirc = true, no_records = true;
  char          sql_statement[MAX_SQL_QUERY_SIZE];

  Id_type       Id ;
  char          Name[51] ;

  //20-Dec-2020 YR BZ#53247
  if((sup_level == 0xffff) || (sup_level <= 3))
  {
    er_sprintf(sql_statement,"select sgroup_id,sg_name from s_grp");
  }
  else
  {
    er_sprintf(sql_statement,"SELECT sgroup_id, sg_name FROM s_grp WHERE sgroup_id IN (SELECT group_id FROM grp_sup WHERE user_id = %d AND g_type = 's')",user_id);
  }


  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);

  Ushort_t total_records_num = rs.GetRecordCount(); 
  Ushort_t left_records_num = rs.GetRecordCount(); 

  csa_web_sg_log_d << "SuperGroups_tab::LoadList <";

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
        if (!(rs.GetUlongByName("sgroup_id",(Ulong_t &)Id)) ||
            !(rs.GetStringByName("sg_name",Name)))
        {
          //do nothing. get next result set
          left_records_num--;
        }
        else
        {
          csa_web_sg_log_d << Name << ":" << Id << "";
          tab[Id].setSGroupName(Name);
          left_records_num--;
          //if there are more info records to insert 				
          if (left_records_num == 0)
            break;
          else
            csa_web_sg_log_d << ",";
        }
      }
    }
  }
  csa_web_sg_log_d << ">" << LOG_TERMINATOR;
}


void SuperGroups_tab::LoadGroups(Ulong_t SG_Id)
{
  DBManager_c&  db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t     bswrc = BSWRC_OK,executionResult;
  Statement_c   st;
  ResultSet_c   rs;
  bool          dbirc = true, no_records = true;
  char          sql_statement[MAX_SQL_QUERY_SIZE];

  Id_type       Id ;
  //char          Name[51] ;


  er_sprintf(sql_statement,"select group_id from grp_s_grp where sgroup_id = %d",SG_Id);

  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);

  Ushort_t total_records_num = rs.GetRecordCount(); 
  Ushort_t left_records_num = rs.GetRecordCount(); 

  csa_web_sg_log_d << "SuperGroups_tab::LoadGroups - SG Id: " << SG_Id << " <";

  // CLEAN ALL Items from the map
  tab[SG_Id].ClearGroupList();

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
        if (!(rs.GetUlongByName("group_id",(Ulong_t &)Id)))
        {
          //do nothing. get next result set
          left_records_num--;
        }
        else
        {
          csa_web_sg_log_d << Id << "";
          tab[SG_Id].InsertGroup(Id);
          left_records_num--;
          //if there are more info records to insert 				
          if (left_records_num == 0)
            break;
          else
            csa_web_sg_log_d << ",";
        }
      }
    }
  }
  csa_web_sg_log_d << ">" << LOG_TERMINATOR;
}


//--------------------------
string SuperGroups_tab::GetName(Ulong_t Id)
{
  string s = "";
  if(tab.find(Id) != tab.end())
    return tab.find(Id)->second.getSGroupName();
  else 
    return s;
}

//26-Oct-2021 YR BZ#55354
bool SuperGroups_tab::IsSuperGroupExist(unsigned int sgrpId)
{
  if(tab.find(sgrpId) != tab.end())
    return true;
  return false;
}

//--------------------------
bool SuperGroups_tab::isMonitor(void)
{
  if(isMonitored)
    return true;
  isMonitored = true;
  return false;
}
