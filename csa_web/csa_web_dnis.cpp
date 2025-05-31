#include "er_std_override.h"
#include "csa_web_dnis.hpp"

void DNIS_tab::LoadDNISList(void)
{
  DBManager_c&  db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t     bswrc = BSWRC_OK,executionResult;
  Statement_c   st;
  ResultSet_c   rs;
  bool          dbirc = true, no_records = true;
  char          sql_statement[MAX_SQL_QUERY_SIZE];

  //2-Jul-2020 YR BZ#52652
  Ulong_t  dnis_info_id ;
  char     dnis_info_name[51] ;
  char     dnis_info_number[51];
  Ulong_t  dnis_info_email_flag; 

  er_sprintf(sql_statement,"select dnis_id,dn_name,dn_number,email_enabled from  dnis ");

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
        if( !(rs.GetUlongByName("dnis_id",dnis_info_id)) ||
            !(rs.GetStringByName("dn_name",dnis_info_name)) ||
            !(rs.GetStringByName("dn_number",dnis_info_number)) ||
            !(rs.GetUlongByName("email_enabled",dnis_info_email_flag)) )
        {
          //do nothing. get next result set
          left_records_num--;
        }
        else
        {
          if(dnis_info_id != 99999)
          {
            tab[dnis_info_id].is_email =dnis_info_email_flag;
            tab[dnis_info_id].dnis_name =(char *) dnis_info_name;
            tab[dnis_info_id].dnis_number =(char *)dnis_info_number;
          }
          left_records_num--;
          //if there are more info records to insert 				
          if (left_records_num == 0)
            break;
        }
      }
    }
  }
}


string DNIS_tab::GetName(Id_type  Id)
{
  string s = "";
  if(tab.find(Id) != tab.end())
    return tab.find(Id)->second.dnis_name;
  else 
    return s;
}


string DNIS_tab::GetNumber(Id_type  Id)
{
  string s = "";
  if(tab.find(Id) != tab.end())
    return tab.find(Id)->second.dnis_number;
  else 
    return s;
}


bool DNIS_tab::isDNISbyID(Id_type Id, Ulong_t  Is_email_enable)
{
  if(tab.find(Id) != tab.end())
  {
    if(tab.find(Id)->second.is_email == Is_email_enable)
      return true;
    return false;
  }
  else
    return false;
}

//{
//  int i =0;
//  string s;
//  s = "";
//  while (i < NumberOfDNIS)
//  {
//	  if( (strcmp(tab[i].dnis_number.c_str(),Number)==0)   &&
//		  (tab[i].is_email ==Is_email_enable ) 
//		)
//	   return true ;
//      else 
//	   i++;
//  }
//  return false;
//}
