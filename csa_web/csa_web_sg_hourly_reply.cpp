#include "er_std_override.h"
#include "csa_web_sg_hourly_reply.h"
#include "csa_web_group_hourly_reply.hpp"
#include "csa_web_commondefs.hpp"



//------------------------------------------------------------------------------------------------
static BswLog_c csa_web_sg_hourly_reply_log_d(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_DETAILED);
static BswLog_c csa_web_sg_hourly_reply_log_i(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_INFO);


//------------------------------------------------------------------------------------------------
void SuperGroupHourlyReply::AddNewSuperGroupBlock(CsaWebServer *soap,unsigned int sgroupId, ns1__ReturnMatrixDataTypePerSuperGroup &ReturnData)
{   
  ns1__returnDataRowsPerSuperGroup*   tmp;
  tmp = soap_new_ns1__returnDataRowsPerSuperGroup(soap, 1);
  tmp->Id = sgroupId;
  ReturnData.returnDataRowsPerSuperGroup.push_back(tmp);
  //soap->m_net_data_ptr->MATRIX_PER_GROUP_vector.push_back(tmp);
}

//------------------------------------------------------------------------------------------------

void SuperGroupHourlyReply::CreateNewRow(CsaWebServer *soap)
{
  CurrentRow = soap_new_ns1__DataItemRow(soap, 1);
}
//------------------------------------------------------------------------------------------------

void SuperGroupHourlyReply::InsertFullDataBG(CsaWebServer *soap,unsigned int *bgcolor_ptr, std::string ersid, std::string ersname, std::string value)
{
  ns1__DataItemType* tmp;

  csa_web_sg_hourly_reply_log_d << "SuperGroupHourlyReply::InsertFullDataBG"
                                   << "\ners_id   - " << ersid.c_str()
                                   << "\ners_name - " << ersname.c_str()
                                   << "\nvalue    - " << value.c_str()
                                   << LOG_TERMINATOR;

  tmp = soap_new_ns1__DataItemType(soap, 1);
  tmp->ersid = ersid;
  tmp->ersname = ersname;
  tmp->value = value;
  if(bgcolor_ptr!=NULL){
    tmp->bgcolor = (unsigned int *)bgcolor_ptr;
  }

  CurrentRow->returnArray.push_back(tmp);
  //soap->m_net_data_ptr->DIT_vector.push_back(tmp);
}


//------------------------------------------------------------------------------------------------
// For Array of Matrix
void SuperGroupHourlyReply::InsertRowToMatrix(CsaWebServer *soap,ns1__returnDataRowsPerSuperGroup* matrix)
{
  matrix->returnMatrix.push_back(CurrentRow);
  //soap->m_net_data_ptr->ROW_vector.push_back(CurrentRow);
}


//--------------------------------------------------------------------------------------------
// Not Used - Example
void SuperGroupHourlyReply::InsertDataToRow(CsaWebServer *soap,unsigned int groupId)
{
  unsigned int *StateColor   = NULL;
  unsigned int  *TmpColorPtr = NULL;
  unsigned long  oacd_answered_calls=0;

  InsertFullDataBG(soap,NULL,"7_3_1_0","Interval Start At",Ulong2String(groupId));
  InsertFullDataBG(soap,NULL,"7_3_1_2_8","Total In",Ulong2String(10));
  InsertFullDataBG(soap,NULL,"7_3_1_2_22","Calls Ansd",Ulong2String(10));
  InsertFullDataBG(soap,NULL,"7_3_1_2_10","Calls Abnd",Ulong2String(10));
  InsertFullDataBG(soap,NULL,"7_3_1_3_5a","Avg. Talk",Ulong2String(10));
  InsertFullDataBG(soap,NULL,"7_3_1_3_10","Avg. Abnd",Ulong2String(10));
  InsertFullDataBG(soap,NULL,"7_3_1_3_6a","Avg. Wrap",Ulong2String(10));

  /*
  7_3_1_0,0,"Interval","Start At",0,80,l,0,l
  7_3_1_2_8,0,"Total","In",0,60,c,0,c
  7_3_1_2_22,0,"Calls","Ansd",0,60,c,0,c
  7_3_1_2_10,0,"Calls","Abnd",0,60,c,0,c
  7_3_1_3_5a,0,"Avg.","Talk",0,60,c,0,c
  7_3_1_3_10,0,"Avg.","Abnd",0,60,c,0,c
  7_3_1_3_6a,0,"Avg.","Wrap",0,60,c,0,c

  */
}


RetCode_t SuperGroupHourlyReply::InsertDataToRows(CsaWebServer*                           soap,
                                                  unsigned int                            sgroupId,
                                                  ns1__ReturnMatrixDataTypePerSuperGroup& ReturnData,
                                                  unsigned int                            blockInd)
{
  // it's 3th steps for create row
  //CreateNewRow(soap); //Start Row
  //InsertDataToRow(soap,1);
  //InsertRowToMatrix(soap,ReturnData->returnDataRowsPerGroup[0]);//End Row

  csa_web_sg_hourly_reply_log_d << "SuperGroupHourlyReply::InsertDataToRows <" << sgroupId << ">" << LOG_TERMINATOR;

  RetCode_t  rc = BSWRC_OK, brc = BSWRC_OK;
  //NLA_DBI_get_grp_full_info_req_t  *hourly_rep_full_info_req;
  Ushort_t   nla_int = 0;
  char       date_today[11];
  char       nla_field_types[11]; // MAX_FLA_FIELDS
  Uint_t     time_now_rounded = 0, time_from = 0;
  Ulong_t    server_time = 0;
  char       arg_sql_code[32768+1];
  char       arg_sql_format[11];
  char       arg_ers_code[17];
  string     final_sum_st,final_st ;
  int        i = 0;
  char       sql_err[2000];
  char       ers_code[7][17];

  string     replace_rep; /* string for replace @REP*/
  string     replace_par; /* string for replace @PAR */
  string     sum_replace_rep; /* replace strings for sum select */
  string     replace_int; /* string for replace @PAR */
  string     replace_str; /* string for replace @PAR */
  Uint_t     report_period = 0; /* report period for Hourly reports*/
  Uint_t     sum_report_period = 0; /* report period for 'Total' line */
  short      par_value = 1;  /* @PAR = num of days */
  //MaxIntervalsDataBuffer_t         full_info_buffer;

  nla_int = 15;// m_fla_interval*hourly_rep_full_info_req->num_offline_int;
  server_time = (Ulong_t) time(0);
  SetDateTodayAndTimeNow(0, nla_int,server_time , date_today ,&time_now_rounded, &time_from);

  char sql_statement[MAX_SQL_QUERY_SIZE];
  Statement_c statement;
  ResultSet_c rs1,rs2,rs3;

  //// start build SQL Query - final_sum_st,final_st
  sprintf(sql_statement, "select ers_code, sql_code, format from trnsltn where ers_code in ('7_3_1_0','7_3_1_2_8','7_3_1_2_22','7_3_1_2_10','7_3_1_3_5a','7_3_1_3_10','7_3_1_3_6a')" );
  statement.SetQuery(sql_statement);
  rs1.clear();

  csa_web_sg_hourly_reply_log_d << "\n   ExecuteSelect 1: \"select ers_code, sql_code, format from trnsltn where ers_code in ('7_3_1_0','7_3_1_2_8','7_3_1_2_22','7_3_1_2_10','7_3_1_3_5a','7_3_1_3_10','7_3_1_3_6a')\"" << LOG_TERMINATOR;

  DBManager_c&  db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  rc = db_mngr.ExecuteSelectSyn(statement, rs1, brc, sql_err);
  if ( (rc == BSWRC_OK) && (brc == BSWRC_OK))
  {
    i = 0;
    while (rs1.Next())
    {
      memset(arg_sql_code, 0, sizeof(arg_ers_code));
      memset(arg_sql_code, 0, sizeof(arg_sql_code));
      memset(arg_sql_format,0,sizeof(arg_sql_format));
      rs1.GetStringByName("ers_code", arg_ers_code);
      rs1.GetStringByName("sql_code", arg_sql_code);
      rs1.GetStringByName("format", arg_sql_format);
      nla_field_types[i] = GetNlaFieldFormat(arg_sql_format);
      //we're using this list of ERS codes later when we build the event to NLA.
      //The prolbem though is that it may be that some of the requested ers code
      //are no longer supported. We need to rebuild the list of ERS codes according to
      //those who exists in the trnsltn
      //strncpy(hourly_rep_full_info_req->ers_code[i], arg_ers_code,
      //        sizeof(hourly_rep_full_info_req->ers_code[i]));

      strncpy(ers_code[i], arg_ers_code,sizeof(ers_code[i]));

      if (i==0)
        final_st = final_st + arg_sql_code + " as col_" + er_ltoa(i,sql_err,10);
      else
        final_st = final_st + ',' + arg_sql_code + " as col_" + er_ltoa(i,sql_err,10);
      i++;
    }
  }
  //for (int j=i; j< 11; j++) // MAX_FLA_FIELDS = 11
  //  memset(hourly_rep_full_info_req->ers_code[j], 0 , sizeof(hourly_rep_full_info_req->ers_code[j]));

  //now final_st almost ready, only have to replace the @ variables:
  report_period = nla_int * 60; //60 secs in a minute;
  time_now_rounded+=nla_int; //pvl
  sum_report_period = time_now_rounded ;// - nla_int;

  /* Set the @REP and @PAR values */
  /* @PAR always 1 day */
  replace_rep.clear();
  replace_par.clear();
  sum_replace_rep.clear();
  replace_int.clear();
  replace_str.clear();
  replace_rep = er_ltoa(report_period,sql_err,10);
  replace_par = er_ltoa(par_value,sql_err,10);
  sum_replace_rep = er_ltoa(sum_report_period,sql_err,10);
  replace_int = er_ltoa(nla_int,sql_err,10);
  string replace_isnull = "IfNull";
  //the report start time range is always 00:00 which is translated to 0
  replace_str = '0';
  int pos = string::npos;
  while ((pos = final_st.find("@GB"))!=string::npos)
    final_st.replace(pos,3,"");
  while ((pos = final_st.find("@PAR"))!=string::npos)
    final_st.replace(pos,4,replace_par);
  while ((pos = final_st.find("@INT"))!=string::npos)
    final_st.replace(pos,4,replace_int);
  while ((pos = final_st.find("@START_TIME_RANGE"))!=string::npos)
    final_st.replace(pos,17,replace_str);
  while ((pos = final_st.find("ISNULL"))!=string::npos)
    final_st.replace(pos,6,replace_isnull);
  while ((pos = final_st.find("IsNull"))!=string::npos)
    final_st.replace(pos,6,replace_isnull);

  //1001679 - clean white spaces
  pos = 0;
  while ((pos = final_st.find("SUM",pos))!=string::npos)
  {
    char tmpc = final_st.at(pos+strlen("SUM"));
    if (tmpc==' ')
      final_st.replace(pos+strlen("SUM"),1,"");
    else
      pos++;
  }

  pos = 0;
  while ((pos = final_st.find("IfNull",pos))!=string::npos)
  {
    char tmpc = final_st.at(pos+strlen("IfNull"));
    if (tmpc==' ')
      final_st.replace(pos+strlen("IfNull"),1,"");
    else
      pos++;
  }

  final_st = "select " + final_st;
  //end of 1001679 - clean white spaces
  final_sum_st = final_st;

  while ((pos = final_st.find("@REP"))!=string::npos)
    final_st.replace(pos,4,replace_rep);
  while ((pos = final_sum_st.find("@REP"))!=string::npos)
    final_sum_st.replace(pos,4,sum_replace_rep);

  final_st = final_st + " from grpo where group_id = " +
                        itoa(sgroupId, sql_err, 10) +
                        " and g_date = \'" + date_today + "\'" +
                        //" and g_time >= " + itoa(time_from, sql_err, 10) +
                        //" and g_time < " + itoa(time_now_rounded, sql_err, 10) +
                        " group by g_time / " + itoa(nla_int, sql_err, 10);

  final_sum_st = final_sum_st + " from grpo where group_id = " +
                        itoa(sgroupId, sql_err, 10) +
                        " and g_date = \'" + date_today + "\'"; //  +
                        //" and g_time >= 0 and g_time < " + itoa(time_now_rounded, sql_err, 10) ;

  //////////////////////////////////////////////////
  ///// Get data from DB///////////////////////////////////////////////
  csa_web_sg_hourly_reply_log_d << "\n   ExecuteSelect 2: \"" << final_st.c_str() << "\"" << LOG_TERMINATOR;
  int tmp = final_st.length();
  statement.SetQuery(final_st.c_str());
  rs2.clear();
  rc = db_mngr.ExecuteSelectSyn(statement, rs2, brc, sql_err);
  if ( (rc == BSWRC_OK) && (brc == BSWRC_OK))
  {
    csa_web_sg_hourly_reply_log_d << "\n   ExecuteSelect 3: \"" << final_sum_st.c_str() << "\"" << LOG_TERMINATOR;
    statement.SetQuery(final_sum_st.c_str());
    rs3.clear();
    rc = db_mngr.ExecuteSelectSyn(statement, rs3, brc, sql_err);
  }
  //build the result event to WebNLA:
  int no_of_intervals = 0;
  int no_of_columns = 0;
  if ( (rc == BSWRC_OK) && (brc == BSWRC_OK))
  {
    csa_web_sg_hourly_reply_log_d << "\n   Build the results " << LOG_TERMINATOR;
    rs2.Next();
    no_of_intervals = rs2.GetRecordCount(); //must be less then or equal to 96
    no_of_columns = rs2.GetColumnCount(); //must be less then or equal to 11
    for (int x=0; x< no_of_intervals; x++)
    {
      CreateNewRow(soap); //Start Row
      for (int y=0; y< no_of_columns; y++)
      {
        //unsigned long ul_tmp = 0;
        double d_tmp = 0;
        char s_tmp[11];

        if (nla_field_types[y]==NLA_NUMBER_FORMAT)
        {
          rs2.GetDoubleByIndex(y,d_tmp);
          InsertFullDataBG(soap,NULL,(&ers_code[y])[0],(&ers_code[y])[0],Ulong2String((Ulong_t)d_tmp));
        }
        else
        {
          rs2.GetStringByIndex(y,s_tmp);
          InsertFullDataBG(soap,NULL,(&ers_code[y])[0],(&ers_code[y])[0],&s_tmp[0]);
        }
      }
      InsertRowToMatrix(soap,ReturnData.returnDataRowsPerSuperGroup[blockInd]);//End Row
      rs2.Next();
    }

    //now treat the total line which ignore the col_0 (the interval start time)
    rs3.Next();
    CreateNewRow(soap); //Start Row
    InsertFullDataBG(soap,NULL,(&ers_code[0])[0],(&ers_code[0])[0],"Totals");
    csa_web_sg_hourly_reply_log_d << "\n   Insert totals " << LOG_TERMINATOR;
    for (int y=1; y< no_of_columns; y++)
    {
      //unsigned long ul_tmp = 0;
      double d_tmp = 0;
      char s_tmp[11];
      if (nla_field_types[y]==NLA_NUMBER_FORMAT)
      {
        rs3.GetDoubleByIndex(y,d_tmp);
        InsertFullDataBG(soap,NULL,(&ers_code[y])[0],(&ers_code[y])[0],Ulong2String(d_tmp));
      }
      else
      {
        rs3.GetStringByIndex(y,s_tmp);
        InsertFullDataBG(soap,NULL,(&ers_code[y])[0],(&ers_code[y])[0],&s_tmp[0]);
      }
    }

    InsertRowToMatrix(soap,ReturnData.returnDataRowsPerSuperGroup[blockInd]);//End Row
  }

  //csa_web_sg_hourly_reply_log_d << LOG_TERMINATOR;

  return rc;
}


unsigned char SuperGroupHourlyReply::GetNlaFieldFormat( char *arg_sql_format)
{
  unsigned char field_type;

  if (strcmp( arg_sql_format, DB_NUMBER_FORMAT) == 0)
  {
      field_type = NLA_NUMBER_FORMAT;
  }
  else if ((strcmp( arg_sql_format, DB_TIME_FORMAT) == 0) || (strcmp( arg_sql_format, FIRST_COLUMN_FORMAT)))
  {
      field_type = NLA_TIME_FORMAT;
  }
  else
  {
      field_type = NLA_STRING_FORMAT;
  }

  return(field_type);

}/*ReturnNlaFieldFormat*/


void   SuperGroupHourlyReply::SetDateTodayAndTimeNow(EosOpCode_t op_code,
                                         Uint_t nla_int,
                                         Ulong_t   server_time, //input
                                         char   *date_today,
                                         Uint_t *time_now_rounded,
                                         Uint_t *time_from)
{
  Uint_t       minutes_from_midnight = 0;
  int          year_now;
  int          month_now;
  int          day_now;
  //char         ser_t[16];
  //struct tm    t;
  struct tm    *t1;
  time_t       st = (long) server_time;
  char*        tptr = NULL;

  //tptr = ctime(&st);
  t1 = gmtime(&st);
  //tptr = asctime(t1);
  /*if (tptr) {
    //only the hour should be fixed since gmtime may changes the hour & as a result the
    //month day
    t1->tm_hour = t.tm_hour = atoi((const char *)&tptr[11]);
    t1->tm_mday = t.tm_mday = atoi((const char *)&tptr[8]);
  }*/
  minutes_from_midnight = (t1->tm_hour)*60 + t1->tm_min;

  /*Round time now to the next hourly interval */
  /* Set the time now */
  if ( nla_int == 0)
  {
    nla_int = 15; // m_fla_interval;
  }

  if ((op_code == NLA_DBI_get_grp_full_info_req) && ((minutes_from_midnight % nla_int) == 0 )){
       minutes_from_midnight -= 15; //m_fla_interval;
  }

  *time_now_rounded = ((minutes_from_midnight / nla_int)*nla_int);

  switch (op_code){
    case  NLA_DBI_get_grp_full_info_req:
      {
        *time_from = 0;
      }
    break;
    case  NLA_DBI_get_grp_last_int_req:
      {
        *time_from = *time_now_rounded - nla_int;
      }
      break;
    default:
      time_from = 0;
      break;
  }


  /* Set the date today */
  year_now = t1->tm_year + 1900;
  month_now = t1->tm_mon + 1;
  day_now = t1->tm_mday;

  sprintf(date_today, "%d%c%02d%c%02d", year_now, '-',month_now, '-',day_now);
}/* End SetDateTodayAndTimeNow*/
