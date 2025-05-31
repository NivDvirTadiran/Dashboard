#include "er_std_override.h"
#include "csa_web_report.h"
#include "csa_web_commondefs.hpp"
#include <base64.h>

#define MAX_ERS_IDS_OLD_REPORT  22

static BswLog_c csa_web_report_reply_log_d(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_DETAILED);
static BswLog_c csa_web_report_reply_log_i(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_INFO);


void ReportsReply::InsertReportForDefaultWorkspace(CsaWebServer *soap,
                                                   unsigned short reportId,
                                                   const geometry_t &geometry,
                                                   ns1__DefaultWorkspaceReturnType &ReturnArrayData)
{
  csa_web_report_reply_log_i << "ReportsReply::InsertReportForDefaultWorkspace, reportId " << reportId << LOG_TERMINATOR;

  ns1__DefaultWorkspaceItemType* tmp;
  tmp = soap_new_ns1__DefaultWorkspaceItemType(soap, 1);
  tmp->reportId = reportId ;
  tmp->x = geometry.x;
  tmp->y = geometry.y;
  tmp->width= geometry.width;
  tmp->height = geometry.height;

  //soap->m_net_data_ptr->DW_vector.push_back(tmp);
  ReturnArrayData.returnArray.push_back(tmp);

  csa_web_report_reply_log_i << "ReturnArrayData has " << ReturnArrayData.returnArray.size() << " reports" << LOG_TERMINATOR;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------

bool ReportsReply::GetAllReportsByAgentID(CsaWebServer *soap,unsigned short agentId,ns1__ReportListDataReturnType &ReturnArrayData)
{
  GetOLDReportsByAgentID(soap,agentId,ReturnArrayData);
  GetNEWReportsByAgentID(soap,agentId,ReturnArrayData);
  InsertReportsToReply(soap,ReturnArrayData);
  return 0;
}


//--------------------------------------------------------------------------------------
bool ReportsReply::GetOLDReportsByAgentID(CsaWebServer *soap,unsigned short agentId,ns1__ReportListDataReturnType &ReturnArrayData)
{
  DBManager_c&  db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t     bswrc = BSWRC_OK,executionResult;
  Statement_c   st;
  ResultSet_c   rs;
  bool          dbirc = true, no_records = true;
  char          sql_statement[MAX_SQL_QUERY_SIZE];
  //report_info_t report_info;

  er_sprintf(sql_statement,
             "select report_id, report_name, creator_id from nla_report_hdr where creator_id = %d or creator_id =%d order by report_id",
             agentId,PUBLIC_OWNER_ID);

  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);

  Ushort_t total_records_num = rs.GetRecordCount();
  Ushort_t left_records_num = rs.GetRecordCount();
  Ushort_t first_record_index;
  Ushort_t Report_index=0;

  csa_web_report_reply_log_d << "ReportsReply::GetOLDReportsByAgentID (agentId " << agentId << ")" 
                             << "\nQuery: " << sql_statement
                             << "\nNum Reports recieved: " << total_records_num << LOG_TERMINATOR;


  unsigned long creator_id;

  if (total_records_num == 0)
  {
    //NOTHING
  }
  else
  {
    while(left_records_num > 0)
    {
      report_info_t report_info;
      report_info.report_render = "";
      char  report_name_str[200];
      memset(report_info.report_name, 0, REPORT_MAX_NAME_LEN);
      memset(report_info.db_info_buffer, 0, REPORT_MAX_DB_DATA_LEN);

      //save the first record index
      first_record_index = total_records_num - left_records_num;
      //start to fill buffer with NlaGroupEntityInfo structures
      if(rs.Next())
      {
        if (!(rs.GetUshortByName("report_id",(Ushort_t &)report_info.report_id)) ||
            !(rs.GetStringByName("report_name",report_name_str)) ||
            //!(rs.GetStringByName("report_name",report_info.report_name)) ||
            !(rs.GetUlongByName("creator_id",(Ulong_t  &)creator_id)))
        {
          //do nothing. get next result set
          //GetLog() << " AdiMngr_c::HandleAgentGroupListReply - opcode: Capp_Dbi_get_lst_grp_entities_req_op_code\n"
          //	<< " Can't get record from result set\n"<< LOG_TERMINATOR;
          left_records_num--;
        }
        else
        {
          if ( creator_id == PUBLIC_OWNER_ID )
            report_info.ppublic = true; //PUBLIC;
          else
          {
            if (creator_id == agentId)
              report_info.ppublic = false; //PRIVATE
          }

          //24-Mar-2019 YR BZ#49397
          //********  Dncode - Start  **********
          string report_name_encoded = report_name_str;
          string report_name_decoded = report_name_str;
#ifndef WIN32
          report_name_encoded = Base64::Encode(report_name_str);
          report_name_decoded = Base64::Decode(report_name_str);
#endif
          //********  Dncode - End  ************

          csa_web_report_reply_log_d << "[" << Report_index << "]Parse old report <" << report_info.report_id << "," << report_name_str << "> (name length " << strlen(report_name_str) << ")"
                                     << "\nDecoded Name " << report_name_decoded.c_str() << ", Encoded Name " << report_name_encoded.c_str() << LOG_TERMINATOR;

          er_strcpy(report_info.report_name,report_name_encoded.c_str());

          //Process report info for report_id
          if(0==GetReportInfoByRepotId(&report_info))
          {
            //Parse Report Data
            if (0== XcodeReportDataForWeb (&report_info)) //PVL 24-06-2010 1004301
            {//Update Report
              InsertReportToMap(soap,&report_info);
              Report_index++;
            }
          }

          left_records_num--;
          //if there are more info records to insert
          if (left_records_num == 0)
            break;
        }
      }//NEXT
    }//WHILE
  }//ELSE
  return true;
}


bool ReportsReply::GetNEWReportsByAgentID(CsaWebServer *soap,unsigned short agentId,ns1__ReportListDataReturnType &ReturnArrayData)
{
//		if(!isReportInWebTab(&report_info))

  DBManager_c&  db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t     bswrc = BSWRC_OK,executionResult;
  Statement_c   st;
  ResultSet_c   rs;
  bool          dbirc = true, no_records = true;
  char sql_statement[MAX_SQL_QUERY_SIZE];
  report_info_t report_info;

  char    Params[2050] ; //8-Feb-2022 YR BZ#55938 was [513]
  char    Render[52] ;

  er_sprintf(sql_statement,
             "select report_id,creator_id,report_name,report_render,report_params from nla_report_info_web  where creator_id = %d or creator_id =%d order by report_id",
             agentId,PUBLIC_OWNER_ID);

  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);

  Ushort_t total_records_num = rs.GetRecordCount();
  Ushort_t left_records_num = rs.GetRecordCount();
  Ushort_t first_record_index;
  Ushort_t Report_index=0;

  csa_web_report_reply_log_d << "ReportsReply::GetNEWReportsByAgentID (agentId " << agentId << ")" 
                             << "\nQuery: " << sql_statement
                             << "\nNum Reports recieved: " << total_records_num << LOG_TERMINATOR;

  unsigned long creator_id;

  if (total_records_num == 0)
  {
    //NOTHING
  }
  else
  {
    while(left_records_num > 0)
    {
      char  report_name_str[200];
      memset(report_info.report_name, 0, REPORT_MAX_NAME_LEN);
      //save the first record index
      first_record_index = total_records_num - left_records_num;
      //start to fill buffer with NlaGroupEntityInfo structures
      if(rs.Next())
      {
        if (!(rs.GetUshortByName("report_id",(Ushort_t &)report_info.report_id)) ||
            !(rs.GetStringByName("report_name",report_name_str)) ||
            //!(rs.GetStringByName("report_name",report_info.report_name)) ||
            !(rs.GetUlongByName("creator_id",(Ulong_t  &)creator_id))||
            !(rs.GetStringByName("report_render",(char *)Render))  ||
            !(rs.GetStringByName("report_params",(char *)Params)) )
        {
          //do nothing. get next result set
          //GetLog() << " AdiMngr_c::HandleAgentGroupListReply - opcode: Capp_Dbi_get_lst_grp_entities_req_op_code\n"
          //	<< " Can't get record from result set\n"<< LOG_TERMINATOR;
          left_records_num--;
        }
        else
        {
          //10-Jun-2019 YR BZ#49999
          //********  Dncode - Start  **********
          string report_name_encoded = report_name_str;
          string report_name_decoded = report_name_str;
#ifndef WIN32
          report_name_encoded = Base64::Encode(report_name_str);
          report_name_decoded = Base64::Decode(report_name_str);
#endif
          //********  Dncode - End  ************

          csa_web_report_reply_log_d << "[" << Report_index << "]Parse report <" << report_info.report_id << "," << report_name_str << "> (name length " << strlen(report_name_str) << ")"
                                     << "\nDecoded Name " << report_name_decoded.c_str() << ", Encoded Name " << report_name_encoded.c_str() << LOG_TERMINATOR;

          er_strcpy(report_info.report_name,report_name_encoded.c_str());

          report_info.reply_buffer = Params;
          report_info.report_render = Render;

          if ( creator_id == PUBLIC_OWNER_ID )
            report_info.ppublic = true; //PUBLIC;
          else
          {
            if (creator_id == agentId)
              report_info.ppublic = false; //PRIVATE
          }
          InsertReportToMap(soap,&report_info);

          Report_index++;
          left_records_num--;
          //if there are more info records to insert
          if (left_records_num == 0)
            break;
        }
      }//NEXT
    }//WHILE
  }//ELSE
  return true;
}


//--------------------------------------------------------------------------------------
bool ReportsReply::GetReportInfoByRepotId(report_info_t* report_info)
{
  DBManager_c&  db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t     bswrc = BSWRC_OK,executionResult;
  Statement_c   st;
  ResultSet_c   rs;
  bool          dbirc = true, no_records = true;
  char sql_statement[MAX_SQL_QUERY_SIZE];
  er_sprintf(sql_statement,
             "select report_text from nla_report_info  where report_id=%d order by  report_sequence_num ",
             report_info->report_id);
  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);

  Ushort_t total_records_num = rs.GetRecordCount();
  Ushort_t left_records_num = rs.GetRecordCount();
  Ushort_t first_record_index;
  Ushort_t index = 0;

  DbiDataRecord_t tmp_buf;
  memset(report_info->db_info_buffer,0,sizeof(report_info->db_info_buffer));

  csa_web_report_reply_log_d << "ReportsReply::GetReportInfoByRepotId <" << report_info->report_id << "," << report_info->report_name << "> got " << total_records_num << " records" << LOG_TERMINATOR;

  if (total_records_num == 0)
  {
    csa_web_report_reply_log_i << "ERROR: No Records Recieved - IGNORE Report" << LOG_TERMINATOR;
    return 1; //PVL 24-06-2010 1004301
  }
  else
  {
    while(left_records_num > 0)
    {
      //save the first record index
      first_record_index = total_records_num - left_records_num;
      //start to fill buffer with NlaGroupEntityInfo structures
      if(rs.Next())
      {
        if (!rs.GetStringByName("report_text",tmp_buf))
        {
          //do nothing. get next result set
          //GetLog() << " AdiMngr_c::HandleAgentGroupListReply - opcode: Capp_Dbi_get_lst_grp_entities_req_op_code\n"
          //	<< " Can't get record from result set\n"<< LOG_TERMINATOR;
          left_records_num--;
        }
        //7-May-2019 YR BZ#49699
        else if(strlen(tmp_buf) == 0)
        {
          csa_web_report_reply_log_i << "ERROR: Empty Record - IGNORE Report" << LOG_TERMINATOR;
          return 1;
        }
        //19-May-2019 YR BZ#49793
        else if( (index > 0) && (strstr(tmp_buf,"[template]")) )
        {
          csa_web_report_reply_log_i << "ERROR: repeated rows starts with [template] - IGNORE Report" << LOG_TERMINATOR;
          return 1;
        }
        else
        {
          //UPDATE db_info_buffer
          // mik 2012/03/20
          //memcpy(report_info->db_info_buffer+(sizeof(DbiDataRecord_t)-1)*index,tmp_buf, sizeof(DbiDataRecord_t)-1 );

          csa_web_report_reply_log_d << "ReportsReply::GetReportInfoByRepotId [" << report_info->report_id << "] for record index " << index << " size of report_text " << strlen(tmp_buf) << ", size of db_info_buffer " << strlen(report_info->db_info_buffer) << LOG_TERMINATOR;

          if(!index)
            er_strncpy_s(report_info->db_info_buffer,sizeof(report_info->db_info_buffer),tmp_buf,sizeof(tmp_buf));
          //10-Sep-2020 YR BZ#53119
          else if( strlen(report_info->db_info_buffer)+strlen(tmp_buf) > sizeof(report_info->db_info_buffer) )
          {
            csa_web_report_reply_log_i << "ERROR: too many records (size is too long) - IGNORE Report" << LOG_TERMINATOR;
            return 1;
          }
          else
            er_strcat_s(report_info->db_info_buffer,sizeof(report_info->db_info_buffer),tmp_buf);

          index++;
          left_records_num--;
          //if there are more info records to insert
          if (left_records_num == 0)
          {
            csa_web_report_reply_log_d << "OK, final size of db_info_buffer " << strlen(report_info->db_info_buffer) << LOG_TERMINATOR;
            break;
          }
        }
      }
    }
  }

  return 0;
}

//--------------------------------------------------------------------------------------

void ReportsReply::InsertReportToMap(CsaWebServer *soap, report_info_t* report_info )
{
  ns1__ReportListDataItemType *  tmp;
  bool found =false;
  if(soap->m_net_data_ptr->RLT_map.find(report_info->report_id) != soap->m_net_data_ptr->RLT_map.end())
    found=true;

  csa_web_report_reply_log_d << "ReportsReply::InsertReportToMap <" << report_info->report_id << "," << report_info->report_name << "> - ";

  if(found)
  {
    csa_web_report_reply_log_d << " exist in list" << LOG_TERMINATOR;
    tmp = (soap->m_net_data_ptr->RLT_map.find(report_info->report_id))->second; //rewrite old report
  }
  else
  {
    csa_web_report_reply_log_d << " new in list" << LOG_TERMINATOR;
    tmp = soap_new_ns1__ReportListDataItemType(soap, 1); //create new
  }

  tmp->reportId = report_info->report_id;
  tmp->isPublic = report_info->ppublic;
  tmp->renderName = report_info->report_render;
  tmp->reportName = report_info->report_name;
  tmp->params = report_info->reply_buffer;

  if(!found)
    soap->m_net_data_ptr->RLT_map[report_info->report_id]=tmp; // save report , old or new

  csa_web_report_reply_log_d << "ReportsReply::InsertReportToMap <" << report_info->report_id << "," << report_info->report_name << "> Size of RLT_map " << soap->m_net_data_ptr->RLT_map.size() << LOG_TERMINATOR;

  report_info->reply_buffer.clear();

}


//-------------------------------------------------------------------------------
void ReportsReply::InsertReportsToReply(CsaWebServer *soap,ns1__ReportListDataReturnType &ReturnArrayData)
{
  csa_web_report_reply_log_d << "ReportsReply::InsertReportsToReply - Size of RLT_map " << soap->m_net_data_ptr->RLT_map.size() << LOG_TERMINATOR;
  if(soap->m_net_data_ptr->RLT_map.size() > 0)
  {
    for(soap->m_net_data_ptr->RLT_iter = soap->m_net_data_ptr->RLT_map.begin(); soap->m_net_data_ptr->RLT_iter!= soap->m_net_data_ptr->RLT_map.end(); ++soap->m_net_data_ptr->RLT_iter)
      ReturnArrayData.returnArray.push_back(soap->m_net_data_ptr->RLT_iter->second);
  }
  csa_web_report_reply_log_d << "ReportsReply::InsertReportsToReply - Size of ReturnArrayData.returnArray " << ReturnArrayData.returnArray.size() << LOG_TERMINATOR;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
bool ReportsReply::XcodeReportDataForWeb( report_info_t* report_info)
{
  char * pch;
  int index =0;
  const char delim[] = "\n";//(const char)0x0A;
  char tmp_report_render[51];
  pch = er_strtok (report_info->db_info_buffer,delim);
  while (pch != NULL)
  {
    switch(index)
    {
      case 4:
        er_strcpy(tmp_report_render,pch);
        report_info->report_render  = tmp_report_render ;
        if("BriefAgentStatus" == report_info->report_render )
        {//Brief Agent Report
          report_info->report_render = "AgentsBriefStatus";
          return (Parse_BriefAgentStatus(report_info,pch,delim) != 0);//PVL 24-06-2010 1004301
        }

        ////////////////
        //if( ("DetGrpStatus"     == report_info->report_render )||
        //    ("DetMailGrpStatus" == report_info->report_render )||
        //    ("GrpOvflIntfl"     == report_info->report_render )||
        //    ("MailGrpOvflIntfl" == report_info->report_render ) )
        //{//Detail Group Report
        //  Parse_DetailStatus(report_info,pch,delim,"groupId=");
        //  return 0;
        //}
        if("DetGrpStatus" == report_info->report_render )
        {//Detailed Group Status
          report_info->report_render = "GroupDetailedStatus";
          Parse_DetailStatus(report_info,pch,delim,"groupId=");
          return 0;
        }
        if("DetMailGrpStatus" == report_info->report_render )
        {//Detailed Group Status
          report_info->report_render = "GroupEmailDetailedStatus";
          Parse_DetailStatus(report_info,pch,delim,"groupId=");
          return 0;
        }
        if("GrpOvflIntfl" == report_info->report_render )
        {//Detailed Group Status
          report_info->report_render = "GroupOvflIntfl";
          Parse_DetailStatus(report_info,pch,delim,"groupId=");
          return 0;
        }
        if("MailGrpOvflIntfl" == report_info->report_render )
        {//Detailed Group Status
          report_info->report_render = "GroupEmailOvflIntfl";
          Parse_DetailStatus(report_info,pch,delim,"groupId=");
          return 0;
        }

        ////////////////
        //if( ("GrpSti"        ==  report_info->report_render )||
        //    ("GrpQueOACDCall"==  report_info->report_render )||
        //    ("GrpOACDSti"    ==  report_info->report_render )||
        //    ("GrpQueCall"    ==  report_info->report_render ))
        //{
        //  Parse_GrpGraph(report_info,pch,delim,"groupId=");
        //  return 0;
        //}
        if("GrpSti" == report_info->report_render )
        {//Detailed Group Status
          report_info->report_render = "GroupSTIGraph";
          Parse_GrpGraph(report_info,pch,delim,"groupId=");
          return 0;
        }
        if("GrpQueOACDCall" == report_info->report_render )
        {//Detailed Group Status
          report_info->report_render = "GroupOACDQueuedCallsGraph";
          Parse_GrpGraph(report_info,pch,delim,"groupId=");
          return 0;
        }
        if("GrpOACDSti" == report_info->report_render )
        {//Detailed Group Status
          report_info->report_render = "GroupOACDSTIGraph";
          Parse_GrpGraph(report_info,pch,delim,"groupId=");
          return 0;
        }
        if("GrpQueCall" == report_info->report_render )
        {//Detailed Group Status
          report_info->report_render = "GroupQueuedCallsGraph";
          Parse_GrpGraph(report_info,pch,delim,"groupId=");
          return 0;
        }

        ////////////////
        //if( ("BriefGrpStatus"    == report_info->report_render )||
        //    ("BriefMailGrpStatus"== report_info->report_render ) )
        //{//Brief Grp Status
        //  Parse_BriefStatus(report_info,pch,delim,"groupId=");
        //  return 0;
        //}
        if("BriefGrpStatus" == report_info->report_render )
        {//Detailed Group Status
          report_info->report_render = "GroupBriefStatus";
          Parse_BriefStatus(report_info,pch,delim,"groupId=");
          return 0;
        }
        if("BriefMailGrpStatus" == report_info->report_render )
        {//Detailed Group Status
          report_info->report_render = "GroupEmailBriefStatus";
          Parse_BriefStatus(report_info,pch,delim,"groupId=");
          return 0;
        }

        ////////////////
        //if("GrpAgentStatus1" == report_info->report_render )
        //{//Detail Agent Report
        //  return (Parse_GrpAgentStatus1(report_info,pch,delim,"groupId=") != 0);//PVL 24-06-2010 1004301
        //}
        if("GrpAgentStatus1" == report_info->report_render )
        {//Detail Agent Report
          report_info->report_render = "GroupAgentsStatus";
          return (Parse_GrpAgentStatus1(report_info,pch,delim,"groupId=") != 0);//PVL 24-06-2010 1004301
        }

        ////////////////
        //if( ("AgentDistrGraph" == report_info->report_render )||
        //    ("CallDistrGraph" == report_info->report_render ))
        //{//Parse_AgentDistrGraph
        //  Parse_DistrGraph(report_info,pch,delim,"groupId=");
        //  return 0;
        //}
        if("AgentDistrGraph" == report_info->report_render )
        {//Parse_AgentDistrGraph
          report_info->report_render = "GroupAgentsDistrGraph";
          Parse_DistrGraph(report_info,pch,delim,"groupId=");
          return 0;
        }
        if("AgentDistrGraph" == report_info->report_render )
        {//Parse_AgentDistrGraph
          report_info->report_render = "GroupAgentsCallsDistrGraph";
          Parse_DistrGraph(report_info,pch,delim,"groupId=");
          return 0;
        }

        ////////////////
        //if( ("BriefDnisStatus" == report_info->report_render )||
        //    ("BriefMailDnisStatus" == report_info->report_render ) )
        //{//Parse_BriefDNIS
        //  return (Parse_BriefStatus(report_info,pch,delim,"dnisId=") != 0);
        //}
        if("BriefDnisStatus" == report_info->report_render )
        {//Parse_AgentDistrGraph
          report_info->report_render = "DnisBriefStatus";
          return (Parse_BriefStatus(report_info,pch,delim,"dnisId=") != 0);
        }
        if("BriefMailDnisStatus" == report_info->report_render )
        {//Parse_AgentDistrGraph
          report_info->report_render = "DnisEmailBriefStatus";
          return (Parse_BriefStatus(report_info,pch,delim,"dnisId=") != 0);
        }

        ////////////////
        //if( ("BriefIvrAppStatus" == report_info->report_render )||
        //    ("IvrPortGrpStatus1"== report_info->report_render ) )
        //{
        //  return (Parse_BriefStatus(report_info,pch,delim,"ivrId=") != 0);
        //}
        if("BriefIvrAppStatus" == report_info->report_render )
        {//Parse_AgentDistrGraph
          report_info->report_render = "IvrAppBriefStatus";
          return (Parse_BriefStatus(report_info,pch,delim,"ivrId=") != 0);
        }
        if("IvrPortGrpStatus1" == report_info->report_render )
        {//Parse_AgentDistrGraph
          report_info->report_render = "IvrPortGrpStatus1";
          return (Parse_BriefStatus(report_info,pch,delim,"ivrId=") != 0);
        }

        ////////////////
        //if("BriefMailboxStatus"== report_info->report_render )
        //{
        //  return (Parse_BriefStatus(report_info,pch,delim,"mailboxId=") != 0);
        //}
        if("BriefMailboxStatus"== report_info->report_render )
        {
          report_info->report_render = "GroupEmailBriefStatus";
          return (Parse_BriefStatus(report_info,pch,delim,"mailboxId=") != 0);
        }

        ////////////////
        //if("DetIvrAppStatus"== report_info->report_render )
        //{
        //  Parse_DetailStatus(report_info,pch,delim,"ivrId=");
        //  return 0;
        //}
        if("DetIvrAppStatus"== report_info->report_render )
        {
          report_info->report_render = "IvrAppDetailedStatus";
          Parse_DetailStatus(report_info,pch,delim,"ivrId=");
          return 0;
        }

        ////////////////
        if( ("IvrAppACDDistrGraph" == report_info->report_render )||
            ("IvrPortDistrGraph"   == report_info->report_render ) )
        {
          Parse_DistrGraph(report_info,pch,delim,"ivrId=");
          return 0;
        }

        ////////////////
        if( ("IvrAppsActiveACDGraph"  ==  report_info->report_render )||
            ("IvrAppsActiveOACDGraph" ==  report_info->report_render )||
            ("IvrAppOACDDistrGraph"   ==  report_info->report_render ) )
        {
          Parse_GrpGraph(report_info,pch,delim,"ivrId=");
          return 0;
        }

        ////////////////
        if( "IvrPortGrpIvrPortStatus2" ==  report_info->report_render )
        {
          return (Parse_GrpAgentStatus1(report_info,pch,delim,"ivrId=") != 0);
        }
      break;
    }//end switch

    index++;
    pch = er_strtok (NULL, delim);
  }//end while

  return 0;
}


//-------------------------------------------------------------------------------
int  ReportsReply::Parse_BriefAgentStatus( report_info_t* report_info,char * pch,const char *delim)//PVL 24-06-2010 1004301
{
  int after_params_counter=0;
  int before_ers_counter=0;
  int ers_counter=0;
  char *Ers_id;
  char *is_visible;
  pch = er_strtok (NULL, delim);

  string agents_id = "agentId=";
  string all_ers_id_arr[MAX_ERS_IDS_OLD_REPORT];
  string visible_ers_id ="fields=";
  while (pch != NULL)
  {
    if(before_ers_counter <3)
      before_ers_counter++;
    //12-Feb-2020 YR BZ#51856
    else if(ers_counter >= MAX_ERS_IDS_OLD_REPORT)
    {
      csa_web_report_reply_log_i << "ERROR in ReportsReply::Parse_BriefAgentStatus: " << report_info->report_name << "[" << report_info->report_id << "] Meny ERS Params - IGNORE Report" << LOG_TERMINATOR;
      return 1;
    }
    else if((before_ers_counter>=3) & (0==after_params_counter))
    {
      all_ers_id_arr[ers_counter] = pch;
      before_ers_counter++;
      ers_counter++;
    }
    //Proccess [Parameters]
    switch(after_params_counter)
    {
      case 1:  //[params..
      case 2:  //number of params here
        after_params_counter++;
      break;
      case 3: //actual params here
        agents_id +=pch;
      break;
    }

    pch = er_strtok (NULL,delim);
    if(pch != NULL)
      if(!strcmp("[Parameters]",  pch ))
        after_params_counter++;
  }//end while(pch != NULL)

  //proccess ers
  bool if_not_first=false;

  for(int i =0;i< ers_counter;i++)
  {
    Ers_id = er_strtok ((char*)all_ers_id_arr[i].c_str(),",");
    is_visible = er_strtok (NULL,",");
    if (is_visible == NULL){//PVL 24-06-2010 1004301
      return 1;           //PVL 24-06-2010 1004301
      //report is cuted , not complete
    }
    if((atoi(is_visible) == 0)|| (atoi(is_visible) == 1))
    {
      if(if_not_first)
        visible_ers_id+=",";
      visible_ers_id+=Ers_id;
      if_not_first = true;

    }
  }

  //make final string
  report_info->reply_buffer+=visible_ers_id;
  report_info->reply_buffer+= "&" + agents_id;
  return 0; //PVL 24-06-2010 1004301
}


//---------------------------------------------
void ReportsReply::Parse_DetailStatus( report_info_t* report_info,char * pch,const char *delim,string id_str )
{
  int after_params_counter=0;
  int before_ers_counter=0;
  int ers_counter=0;
  pch = er_strtok (NULL, delim);

  string groupid = id_str;
  while (pch != NULL)
  {
    //Proccess [Parameters]
    if(after_params_counter==1){
      //actual params here
      groupid +=pch;
      report_info->reply_buffer+=groupid;
      return;
    }

    if(!strcmp("[Parameters]",  pch ))
      after_params_counter++;
    pch = er_strtok (NULL,delim);
  }
}


void ReportsReply::Parse_GrpGraph( report_info_t* report_info,char * pch,const char *delim,string id_str)
{
    int after_params_counter=0;
	int before_ers_counter=0;
    int ers_counter=0;
	pch = er_strtok (NULL, delim);
    char *str_gr_id;
	char *str_color;
	//string str_group_param[NUMBER_OF_GROUPS];
	vector<string> PARAM_vector;
    vector<string>::iterator PARAM_iter;
	PARAM_vector.clear(); //PVL
	string tmp_param_string;
	int NumOfGroups =0;
	string groupid = id_str;
	bool if_not_first =false;
   	while (pch != NULL)
	{
		//Proccess [Parameters]
		switch(after_params_counter){
			case 1://number of groups
				 after_params_counter++;
			break;
			case 2:
				//actual params here
			   //str_group_param[NumOfGroups]
			   tmp_param_string=pch;
			   PARAM_vector.push_back(tmp_param_string);
			   NumOfGroups++;
			break;
			}


	   if(!strcmp("[Parameters]",  pch ))
		   after_params_counter++;
       pch = er_strtok (NULL,delim);
    }
	//for(int i=0;i<NumOfGroups ;i++){
	for(PARAM_iter=PARAM_vector.begin(); PARAM_iter!= PARAM_vector.end(); ++PARAM_iter) {
       str_gr_id = er_strtok ((char*)(PARAM_iter->c_str()),",");
	   str_color=  er_strtok (NULL,","); //2nd is color of graph
	   if(if_not_first)
	  	 groupid+=",";
	   if_not_first = true;
	   groupid +=str_gr_id;
	}
	PARAM_vector.clear();
	report_info->reply_buffer+=groupid;
 }

void ReportsReply::Parse_DistrGraph( report_info_t* report_info,char * pch,const char *delim,string id_str)
{
  int after_params_counter = 0;
  int before_ers_counter = 0;
  int ers_counter = 0;
  pch = er_strtok(NULL, delim);
  char* str_gr_id;
  char* str_color;
  string str_group_param;
  int NumOfGroups = 0;
  string groupid = id_str;
  bool if_not_first = false;
  while (pch != NULL)
  {
    //Proccess [Parameters]
    switch (after_params_counter)
    {
      case 1://number of groups
        after_params_counter++;
      break;
      case 2:
        //actual params here
        str_group_param = pch;
        NumOfGroups++;
      break;
    }

    if (!strcmp("[Parameters]", pch))
      after_params_counter++;
    pch = er_strtok(NULL, delim);
  }

  //1-May-2023 YR BZ#57777
  if (!str_group_param.empty())
  {
    str_gr_id = er_strtok((char*)str_group_param.c_str(), ",");
    str_color = er_strtok(NULL, ","); //2nd is color of graph
    if (if_not_first)
      groupid += ",";
    if_not_first = true;
    groupid += str_gr_id;
  }

  report_info->reply_buffer += groupid;
 }
//-----------------------------------------------------------------------
//void ReportsReply::Parse_BriefGrpStatus( report_info_t* report_info,char * pch,const char *delim)
//{
//
//    int after_params_counter=0;
//	int before_ers_counter=0;
//    int ers_counter=0;
//    char *Ers_id;
//	char *is_visible;
//	pch = er_strtok (NULL, delim);
//
//	string groups_id = "groupId=";
//    string all_ers_id_arr[20];
//	string visible_ers_id ="fields=";
//	while (pch != NULL)
//	{
//       if(before_ers_counter <3)
//		   before_ers_counter++;
//	   else if((before_ers_counter>=3) & (0==after_params_counter))
//	   {
//           all_ers_id_arr[ers_counter] = pch;
//		   before_ers_counter++;
//		   ers_counter++;
//	   }
//		//Proccess [Parameters]
//		switch(after_params_counter){
//           case 1:  //[params..
//		   case 2:  //number of params here
//		      after_params_counter++;
//		  break;
//		   case 3: //actual params here
//			   groups_id +=pch;
//			   break;
//		}
//
//
//
//       pch = er_strtok (NULL,delim);
//	   if(pch != NULL)
//	     if(!strcmp("[Parameters]",  pch ))
//		   after_params_counter++;
//
//       	}
//     //proccess ers
//	bool if_not_first=false;
//
//    for(int i =0;i< ers_counter;i++)
//	{
//		Ers_id = er_strtok ((char*)all_ers_id_arr[i].c_str(),",");
//        is_visible = er_strtok (NULL,",");
//		if((atoi(is_visible) == 0)|| (atoi(is_visible) == 1))
//		{
//			if(if_not_first)
//                visible_ers_id+=",";
//            visible_ers_id+=Ers_id;
//			if_not_first = true;
//
//		}
//	}
//	//make final string
//    report_info->reply_buffer+=visible_ers_id;
//	report_info->reply_buffer+= "&" + groups_id;
//
//}
//-----------------------------------------------------------------------
int  ReportsReply::Parse_BriefStatus( report_info_t* report_info,char * pch,const char *delim,string id_str)//PVL 24-06-2010 1004301
{
  int after_params_counter=0;
  int before_ers_counter=0;
  int ers_counter=0;
  char *Ers_id;
  char *is_visible;
  pch = er_strtok (NULL, delim);

  string _id = id_str;
  string all_ers_id_arr[MAX_ERS_IDS_OLD_REPORT];
  string visible_ers_id ="fields=";
  while (pch != NULL)
  {
    if(before_ers_counter <3)
      before_ers_counter++;
    //12-Feb-2020 YR BZ#51856
    else if(ers_counter >= MAX_ERS_IDS_OLD_REPORT)
    {
      csa_web_report_reply_log_i << "ERROR in ReportsReply::Parse_BriefStatus: " << report_info->report_name << "[" << report_info->report_id << "] Meny ERS Params - IGNORE Report" << LOG_TERMINATOR;
      return 1;
    }
    else if((before_ers_counter>=3) & (0==after_params_counter))
    {
      all_ers_id_arr[ers_counter] = pch;
      before_ers_counter++;
      ers_counter++;
    }
    //Proccess [Parameters]
    switch(after_params_counter){
      case 1:  //[params..
      case 2:  //number of params here
        after_params_counter++;
      break;
      case 3: //actual params here
        _id +=pch;
      break;
    }

    pch = er_strtok (NULL,delim);
    if(pch != NULL)
      if(!strcmp("[Parameters]",  pch ))
        after_params_counter++;
  }//end while (pch != NULL)

  //proccess ers
  bool if_not_first=false;

  for(int i =0;i< ers_counter;i++)
  {
    Ers_id = er_strtok ((char*)all_ers_id_arr[i].c_str(),",");
    is_visible = er_strtok (NULL,",");
    if (is_visible == NULL){//PVL 24-06-2010 1004301
      return 1;           //PVL 24-06-2010 1004301
      //report is cuted , not complete
    }
    if((atoi(is_visible) == 0)|| (atoi(is_visible) == 1))
    {
      if(if_not_first)
        visible_ers_id+=",";
      visible_ers_id+=Ers_id;
      if_not_first = true;

    }
  }

  //make final string
  report_info->reply_buffer+=visible_ers_id;
  report_info->reply_buffer+= "&" + _id;
  return 0; //PVL 24-06-2010 1004301
}


//------------------------------------------------------------
int  ReportsReply::Parse_GrpAgentStatus1( report_info_t* report_info,char * pch,const char *delim,string id_str) //PVL 24-06-2010 1004301
{
  int after_params_counter=0;
  int before_ers_counter=0;
  int ers_counter=0;
  char *Ers_id;
  char *is_visible;
  pch = er_strtok (NULL, delim);

  string group_id = id_str ;
  string all_ers_id_arr[MAX_ERS_IDS_OLD_REPORT];
  string visible_ers_id ="fields=";
  while (pch != NULL)
  {
    if(before_ers_counter <3)
      before_ers_counter++;
    //12-Feb-2020 YR BZ#51856
    else if(ers_counter >= MAX_ERS_IDS_OLD_REPORT)
    {
      csa_web_report_reply_log_i << "ERROR in ReportsReply::Parse_GrpAgentStatus1: " << report_info->report_name << "[" << report_info->report_id << "] Meny ERS Params - IGNORE Report" << LOG_TERMINATOR;
      return 1;
    }
    else if((before_ers_counter>=3) & (0==after_params_counter))
    {
      all_ers_id_arr[ers_counter] = pch;
      before_ers_counter++;
      ers_counter++;
    }
    //Proccess [Parameters]
    switch(after_params_counter)
    {
      case 1:  //[params..
      case 2:  //number of params here
        after_params_counter++;
      break;
      case 3: //actual params here
        group_id +=pch;
        after_params_counter++; //ignore next parameter
      break;
    }

    pch = er_strtok (NULL,delim);
    if(pch != NULL)
      if(!strcmp("[Parameters]",  pch ))
        after_params_counter++;
  }

  //proccess ers
  bool if_not_first=false;

  for(int i =0;i< ers_counter;i++)
  {
    Ers_id = er_strtok ((char*)all_ers_id_arr[i].c_str(),",");
    is_visible = er_strtok (NULL,",");
    if (is_visible == NULL){//PVL 24-06-2010 1004301
      return 1;           //PVL 24-06-2010 1004301
      //report is cuted , not complete
    }
    if((atoi(is_visible) == 0)|| (atoi(is_visible) == 1))
    {
      if(if_not_first)
        visible_ers_id+=",";
      visible_ers_id+=Ers_id;
      if_not_first = true;

    }
  }

  //make final string
  report_info->reply_buffer+=visible_ers_id;
  report_info->reply_buffer+= "&" + group_id;
  return 0;           //PVL 24-06-2010 1004301
}


  //-------------------------------------------------------------------------------
/*void ReportsReply::Parse_BriefDNIS( report_info_t* report_info,char * pch,const char *delim)
{

    int after_params_counter=0;
	int before_ers_counter=0;
    int ers_counter=0;
    char *Ers_id;
	char *is_visible;
	pch = er_strtok (NULL, delim);

	string dnis_id = "dnisId=";
    string all_ers_id_arr[20];
	string visible_ers_id ="fields=";
	while (pch != NULL)
	{
       if(before_ers_counter <3)
		   before_ers_counter++;
	   else if((before_ers_counter>=3) & (0==after_params_counter))
	   {
           all_ers_id_arr[ers_counter] = pch;
		   before_ers_counter++;
		   ers_counter++;
	   }
		//Proccess [Parameters]
		switch(after_params_counter){
           case 1:  //[params..
		   case 2:  //number of params here
		      after_params_counter++;
		  break;
		   case 3: //actual params here
			   dnis_id +=pch;
			   break;
		}



       pch = er_strtok (NULL,delim);
	   if(pch != NULL)
	     if(!strcmp("[Parameters]",  pch ))
		   after_params_counter++;

       	}
     //proccess ers
	bool if_not_first=false;

    for(int i =0;i< ers_counter;i++)
	{
		Ers_id = er_strtok ((char*)all_ers_id_arr[i].c_str(),",");
        is_visible = er_strtok (NULL,",");
		if((atoi(is_visible) == 0)|| (atoi(is_visible) == 1))
		{
			if(if_not_first)
                visible_ers_id+=",";
            visible_ers_id+=Ers_id;
			if_not_first = true;

		}
	}
	//make final string
    report_info->reply_buffer+=visible_ers_id;
	report_info->reply_buffer+= "&" + dnis_id;

}
*/


//---------------------------------------------------------------------------------------------
bool ReportsReply::SaveWorkspase(CsaWebServer *soap,ns1__SaveWorkspaseRequestType *Workspace)
{
  DBManager_c&  db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t     bswrc = BSWRC_OK,executionResult;
  Statement_c   st;
  char          sql_statement[MAX_SQL_QUERY_SIZE];
  char          error_msg[MAX_SQL_QUERY_SIZE];
  sup_data_t*   tmp_sup_data;

  csa_web_report_reply_log_i << "ReportsReply::SaveWorkspase - ";

  if(!soap->m_db_data_ptr->TicketTab.VerifyTicketGetData(Workspace->requestInfoHeader->ticket, &tmp_sup_data))
  {
    csa_web_report_reply_log_i << "Ticket Not Verified (Error 501)" << LOG_TERMINATOR;
    return 501;
  }

  //Delete OLD configuration from "def_cfg_web"
  er_sprintf(sql_statement,"delete from def_cfg_web where user_id = %d", tmp_sup_data->user_id);

  csa_web_report_reply_log_i << "STEP #1 Delete from def_cfg_web table - Query: " << sql_statement << LOG_TERMINATOR;

  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteUpdateSyn(st, executionResult,error_msg);
  if(bswrc != BSWRC_OK)
  {
    //Failed to execute DELETE , may be table not exist
    csa_web_report_reply_log_i << "Failed to execute DELETE , may be table not exist" << LOG_TERMINATOR;
  }

  csa_web_report_reply_log_i << "STEP #2 Insert into def_cfg_web table (num reprts " << Workspace->RequestArray.size() << ")" << LOG_TERMINATOR;

  for (unsigned int i=0; i<Workspace->RequestArray.size(); i++)
  {
    er_sprintf(sql_statement,"insert into def_cfg_web values (%d, %d, %d, %d, %d, %d, %d)",
               tmp_sup_data->user_id,
               i+1,   //insert_index
               Workspace->RequestArray[i]->reportId,
               Workspace->RequestArray[i]->x,
               Workspace->RequestArray[i]->y,
               Workspace->RequestArray[i]->width,
               Workspace->RequestArray[i]->height);
    st.SetQuery(sql_statement);

    csa_web_report_reply_log_i << "Insert [ " << i+1 << "] - Query: " << sql_statement << LOG_TERMINATOR;

    bswrc = db_mngr.ExecuteUpdateSyn(st,executionResult,error_msg);
    if(bswrc != BSWRC_OK)
    {
      return 1;
    }
  }

  //15-Aug-2019 BZ#50532
  setFontLangInDB(Workspace,tmp_sup_data->user_id);

  return 0;
}


//---------------------------------------------------------------------------------------------
int ReportsReply::SaveReport(CsaWebServer *soap,ns1__RequestSaveReportType *Report, unsigned int &SavedReportID)
{
  RetCode_t   rc = BSWRC_OK;
  bool        bAllowPublic = false;
  bool        bIsPublic = false;
  bool        bSameName = false;
  sup_data_t* tmp_sup_data;

  //5-Nov-2019 YR BZ#51134
  char tmp_report_name[REPORT_MAX_NAME_LEN];
  const char *tmp_report_name_src_ptr;

  tmp_report_name_src_ptr = (const char *)(Report->RequestItem->reportName.c_str());
  memcpy(tmp_report_name,tmp_report_name_src_ptr,REPORT_MAX_NAME_LEN-1);
  tmp_report_name[REPORT_MAX_NAME_LEN-1] = '\0';

  //28-Aug-2019 YR BZ#50620
  //********  Decode - Start  **********
  string report_name_encoded = tmp_report_name;
  string report_name_decoded = tmp_report_name;
#ifndef WIN32
  report_name_encoded = Base64::Encode(Report->RequestItem->reportName);
  report_name_decoded = Base64::Decode(Report->RequestItem->reportName);
#endif
  //********  Decode - End  ************

  csa_web_report_reply_log_i << "ReportsReply::SaveReport";

  if(!soap->m_db_data_ptr->TicketTab.VerifyTicketGetData(Report->requestInfoHeader->ticket, &tmp_sup_data))
  {
    csa_web_report_reply_log_i << " - Ticket Not Verified (Error 501)" << LOG_TERMINATOR;
    return 501;
  }

  if(GetLastReportNumber())
  {
    csa_web_report_reply_log_i << " - GetLastReportNumber Error (Error 301)" << LOG_TERMINATOR;
    return 301;  //DB error or Tab not exist
  }

  //PUBLIC
  rc = IsAllowPublic(tmp_sup_data->user_id,bAllowPublic);

  if(Report->RequestItem->reportId != ER_NULL_ID)
  {
    csa_web_report_reply_log_i << " - Id " << Report->RequestItem->reportId
                               << "\nReport Name - " << tmp_report_name
                               //<< "\nEncoded - " << report_name_encoded.c_str()
                               << "\nDecoded - " << report_name_decoded.c_str() << LOG_TERMINATOR;

    //2-Oct-2019 YR BZ#50926
    if(IsReportExist(tmp_sup_data->user_id,Report,bSameName,bIsPublic))
    {
      if(Report->RequestItem->isPublic)
      {
        csa_web_report_reply_log_i << "UpdateReport as Public - ";
        if(bAllowPublic)
        {
          csa_web_report_reply_log_i << "OK" << LOG_TERMINATOR;
          SavedReportID = Report->RequestItem->reportId;
          return(UpdateReport(tmp_sup_data->user_id,Report,Report->RequestItem->reportId)); // UPDATE PUBLIC
        }
        else
        {
          csa_web_report_reply_log_i << "Not Allowed" << LOG_TERMINATOR;
          return 302;  //Save public is Not allowed
        }
      }
      else if(bIsPublic)
      {
        csa_web_report_reply_log_i << "Trying to save PUBLIC Report as PRIVATE is NOT ALLOW" << LOG_TERMINATOR;
        return 303;  //Trying to save public as private
      }
      else
      {
        csa_web_report_reply_log_i << "UpdateReport as Private - OK" << LOG_TERMINATOR;
        SavedReportID = Report->RequestItem->reportId;
        return(UpdateReport(tmp_sup_data->user_id,Report,Report->RequestItem->reportId)); // UPDATE PUBLIC
      }
    }//end IsReportExist
    else
    {
      csa_web_report_reply_log_i << " - New Report"
                                 << "\nReport Name - " << tmp_report_name
                                 //<< "\nEncoded - " << report_name_encoded.c_str()
                                 << "\nDecoded - " << report_name_decoded.c_str() << LOG_TERMINATOR;
      return(SaveNewReport(tmp_sup_data->user_id,Report,SavedReportID)); //NEW PUBLIC
    }
  }// end if Report->RequestItem->reportId != ER_NULL_ID
  else
  {
    csa_web_report_reply_log_i << " - New Report"
                               << "\nReport Name - " << tmp_report_name
                               //<< "\nEncoded - " << report_name_encoded.c_str()
                               << "\nDecoded - " << report_name_decoded.c_str() << LOG_TERMINATOR;
    SavedReportID = m_LastReportId;
    return(SaveNewReport(tmp_sup_data->user_id,Report,m_LastReportId)); //NEW PUBLIC
  }

  return 0;
}//SaveReport


//---------------------------------------------------------------------------------------------
int ReportsReply::DeleteReport(CsaWebServer *soap,ns1__RequestDeleteReportType *ns1__RequestDeleteReport )
{
  DBManager_c&  db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t     bswrc = BSWRC_OK,executionResult;
  Statement_c   st;
  char          sql_statement[MAX_SQL_QUERY_SIZE];
  char          error_msg[MAX_SQL_QUERY_SIZE];
  RetCode_t     rc = BSWRC_OK;
  bool          bAllowPublic;
  bool          isReportExist = false;

  csa_web_report_reply_log_i << "ReportsReply::DeleteReport - ";

  sup_data_t *tmp_sup_data;
  if( !soap->m_db_data_ptr->TicketTab.VerifyTicketGetData(ns1__RequestDeleteReport->requestInfoHeader->ticket,
      &tmp_sup_data) )
  {
    csa_web_report_reply_log_i << "Error(1), Ticket Not Verified" << LOG_TERMINATOR;
    return 1;
  }

  if( isReportPublic(ns1__RequestDeleteReport->reportId,
                     tmp_sup_data->user_id,
                     isReportExist) )
  {
    //PUBLIC
    rc = IsAllowPublic(tmp_sup_data->user_id,bAllowPublic);
    if( (rc == BSWRC_OK) && !bAllowPublic )
    {
      csa_web_report_reply_log_i << "Error(2), Delete Public NOT Allowed" << LOG_TERMINATOR;
      return 2; //NOT ALLOW TO SAVE /DELETE PUBLIC REPORT
    }
  }

  if(!isReportExist)
  {
    csa_web_report_reply_log_i << "Error(6), Report NOT Exist" << LOG_TERMINATOR;
    return 6; //REPORT NOT EXIST
  }

  //Delete NEW  REPORT from NEW INFO "nla_report_info_web"
  er_sprintf(sql_statement,
             "delete from nla_report_info_web where report_id = %d",ns1__RequestDeleteReport->reportId);
  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteUpdateSyn(st, executionResult,error_msg);
  csa_web_report_reply_log_i << "ReportsReply::DeleteReport - Query1: " << sql_statement << LOG_TERMINATOR;


  //Delete NEW  REPORT from NEW WORKSPACE "def_cfg_web"
  er_sprintf(sql_statement,
             "delete from def_cfg_web where report_id = %d",ns1__RequestDeleteReport->reportId);
  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteUpdateSyn(st, executionResult,error_msg);
  csa_web_report_reply_log_i << "ReportsReply::DeleteReport - Query2: " << sql_statement << LOG_TERMINATOR;

  //Delete OLD  REPORT from OLD  HDR "nla_report_hdr"
  er_sprintf(sql_statement,
             "delete from nla_report_hdr where report_id = %d",ns1__RequestDeleteReport->reportId);
  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteUpdateSyn(st, executionResult,error_msg);
  csa_web_report_reply_log_i << "ReportsReply::DeleteReport - Query3: " << sql_statement << LOG_TERMINATOR;

  //Delete OLD  REPORT from OLD INFO "nla_report_info"
  er_sprintf(sql_statement,
             "delete from nla_report_info where report_id = %d",ns1__RequestDeleteReport->reportId);
  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteUpdateSyn(st, executionResult,error_msg);
  csa_web_report_reply_log_i << "ReportsReply::DeleteReport - Query4: " << sql_statement << LOG_TERMINATOR;

  //Delete OLD  REPORT from OLD WORKSPACE "def_cfg"
  er_sprintf(sql_statement,
             "delete from def_cfg where report_id = %d",ns1__RequestDeleteReport->reportId);
  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteUpdateSyn(st, executionResult,error_msg);
  csa_web_report_reply_log_i << "ReportsReply::DeleteReport - Query5: " << sql_statement << LOG_TERMINATOR;

  return 0; //OK
}


//---------------------------------------------------------------------------------------------
bool ReportsReply::IsAllowPublic(unsigned int UserId,bool &bAllowPublic)
{
  DBManager_c&  db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t     bswrc = BSWRC_OK,executionResult;
  Statement_c   st;
  ResultSet_c   rs;
  char          sql_statement[MAX_SQL_QUERY_SIZE];
  char          allow_to_save_public[10];

  er_sprintf(sql_statement,"select p_allow_to_save_public from sup where user_id = %d",UserId);
  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);
  Ushort_t total_records_num = rs.GetRecordCount();

  if (total_records_num > 0)
  {
    if(rs.Next() && bswrc == BSWRC_OK)
    {
      if((rs.GetStringByName("p_allow_to_save_public",allow_to_save_public)))
      {
        if(strcmp(allow_to_save_public,"t")==0)
          bAllowPublic = true;
        else
          bAllowPublic = false;
        return 0;

      }
    }
  }
  return 1;
}


//---------------------------------------------------------------------------------------------
bool ReportsReply::GetLastReportNumber(void)
{
  //SELECT report_id FROM `nla_report_hdr`order by report_id desc;
  DBManager_c&  db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t     bswrc = BSWRC_OK,executionResult;
  Statement_c   st;
  ResultSet_c   rs;
  char          sql_statement[MAX_SQL_QUERY_SIZE];

  er_sprintf(sql_statement,"select report_id from nla_report_info_web order by report_id desc");

  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);
  Ushort_t total_records_num = rs.GetRecordCount();
  if (total_records_num > 0)
  {
    if(rs.Next() && (bswrc == BSWRC_OK))
    {
      if (rs.GetUlongByName("report_id",(Ulong_t &)m_LastReportId))
      {
        m_LastReportId++;
        return 0;
      }
    }
  }
  else{ //First record in Tab
    m_LastReportId = CSA_WEB_FIRST_REPORT_ID+1;
    return 0;
  }

  m_LastReportId = CSA_WEB_FIRST_REPORT_ID+1;
  return 1;
}


//---------------------------------------------------------------------------------------------
bool ReportsReply::IsReportExist(unsigned int UserId,ns1__RequestSaveReportType *Report,bool &sameName, bool &isPublic)
{
  DBManager_c&  db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t     bswrc = BSWRC_OK,executionResult;
  Statement_c   st;
  ResultSet_c   rs;
  char          sql_statement[MAX_SQL_QUERY_SIZE];
  //5-Nov-2019 YR BZ#51134
  char          saved_report_name[REPORT_MAX_NAME_LEN];
  char          tmp_report_name[REPORT_MAX_NAME_LEN];
  const char *  tmp_report_name_src_ptr;
  Ulong_t       saved_creator_id = ER_NULL_ID;

  tmp_report_name_src_ptr = (const char *)(Report->RequestItem->reportName.c_str());
  memcpy(tmp_report_name,tmp_report_name_src_ptr,REPORT_MAX_NAME_LEN-1);
  tmp_report_name[REPORT_MAX_NAME_LEN-1] = '\0';

  //28-Aug-2019 YR BZ#50620
  //********  Decode - Start  **********
  string report_name_encoded = tmp_report_name;
  string report_name_decoded = tmp_report_name;
#ifndef WIN32
  report_name_encoded = Base64::Encode(Report->RequestItem->reportName);
  report_name_decoded = Base64::Decode(Report->RequestItem->reportName);
#endif
  //********  Decode - End  ************

  er_sprintf(sql_statement,"select * from nla_report_info_web where report_id=%d",
             Report->RequestItem->reportId);

  csa_web_report_reply_log_i << "ReportsReply::IsReportExist - Query: " << sql_statement;

  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);
  Ushort_t total_records_num = rs.GetRecordCount();

  if (total_records_num == 0)
  {
    csa_web_report_reply_log_i << "\n Not Exist" << LOG_TERMINATOR;
    return false;
  }
  else if((rs.Next() && bswrc == BSWRC_OK))
  {
    csa_web_report_reply_log_i << "\n Found in DB Report with name ";
    if ((rs.GetStringByName("report_name",saved_report_name)) )
    {
      csa_web_report_reply_log_i << saved_report_name;
      if(er_strcmp(report_name_decoded.c_str(),saved_report_name) == ER_NULL_ID)
      {
        csa_web_report_reply_log_i << " (Same Name)";
        sameName = true;
      }
    }
    if ((rs.GetUlongByName("creator_id",saved_creator_id)) )
    {
      csa_web_report_reply_log_i << " and Creator " << saved_creator_id;
      if(saved_creator_id == PUBLIC_OWNER_ID)
      {
        csa_web_report_reply_log_i << " (Public)";
        isPublic = true;
      }
    }
    csa_web_report_reply_log_i << LOG_TERMINATOR;
    return true;
  }

  csa_web_report_reply_log_i << "\n Not Exist" << LOG_TERMINATOR;

  return false;
}


//---------------------------------------------------------------------------------------------
bool ReportsReply::UpdateReport (unsigned int UserId,ns1__RequestSaveReportType *Report,unsigned int &SavedReportID)
{
  DBManager_c&  db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t     bswrc = BSWRC_OK,executionResult;
  Statement_c   st;
  char          error_msg[MAX_SQL_QUERY_SIZE];
  char          sql_statement[MAX_SQL_QUERY_SIZE];
  //5-Nov-2019 YR BZ#51134
  char          tmp_report_name[REPORT_MAX_NAME_LEN];
  const char*   tmp_report_name_src_ptr;

  tmp_report_name_src_ptr = Report->RequestItem->reportName.c_str();
  memcpy(tmp_report_name,tmp_report_name_src_ptr,REPORT_MAX_NAME_LEN-1);
  tmp_report_name[REPORT_MAX_NAME_LEN-1] = '\0';

  //********  Decode - Start  **********
  string report_name_encoded = tmp_report_name;
  string report_name_decoded = tmp_report_name;
#ifndef WIN32
  report_name_encoded = Base64::Encode(Report->RequestItem->reportName);
  report_name_decoded = Base64::Decode(Report->RequestItem->reportName);
#endif
  //********  Decode - End  ************

  er_sprintf(sql_statement,
             "replace into nla_report_info_web SET report_id=%d,creator_id=%d,report_name=\"%s\",report_render=\"%s\",report_params=\"%s\"",
             SavedReportID,
             Report->RequestItem->isPublic?PUBLIC_OWNER_ID:UserId,
             //tmp_report_name,
             report_name_decoded.c_str(),
             Report->RequestItem->renderName.c_str(),
             Report->RequestItem->params.c_str());

  csa_web_report_reply_log_i << "ReportsReply::UpdateReport - Query: " << sql_statement << LOG_TERMINATOR;

  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteUpdateSyn(st, executionResult,error_msg);
  if(bswrc != BSWRC_OK)
    return 1; //error
  else
    return 0;
}


//---------------------------------------------------------------------------------------------
bool ReportsReply::SaveNewReport(unsigned int UserId,ns1__RequestSaveReportType *Report,unsigned int &SavedReportID)
{
  DBManager_c&    db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t       bswrc = BSWRC_OK,executionResult;
  Statement_c     st;
  char            error_msg[MAX_SQL_QUERY_SIZE];
  char            sql_statement[MAX_SQL_QUERY_SIZE];
  unsigned int    ReportId;
  //5-Nov-2019 YR BZ#51134
  char            tmp_report_name[REPORT_MAX_NAME_LEN];
  const char*     tmp_report_name_src_ptr;
  //const wchar_t*  tmp_report_name_src_ptr_w;

  tmp_report_name_src_ptr = Report->RequestItem->reportName.c_str();
  memcpy(tmp_report_name,tmp_report_name_src_ptr,REPORT_MAX_NAME_LEN-1);
  tmp_report_name[REPORT_MAX_NAME_LEN] = '\0';

  //10-Jun-2019 YR BZ#49999
  //********  Decode - Start  **********
  string report_name_encoded = tmp_report_name;
  string report_name_decoded = tmp_report_name;
#ifndef WIN32
  report_name_encoded = Base64::Encode(Report->RequestItem->reportName);
  report_name_decoded = Base64::Decode(Report->RequestItem->reportName);
#endif
  //********  Decode - End  ************

  ReportId = Report->RequestItem->reportId;
  SavedReportID = (0 == ReportId? m_LastReportId : ReportId);

  er_sprintf(sql_statement,
             "insert into nla_report_info_web SET report_id=%d,creator_id=%d,report_name=\"%s\",report_render=\"%s\",report_params=\"%s\"",
             SavedReportID,
             Report->RequestItem->isPublic?PUBLIC_OWNER_ID:UserId,
             //tmp_report_name,
             report_name_decoded.c_str(),
             Report->RequestItem->renderName.c_str(),
             Report->RequestItem->params.c_str());

  csa_web_report_reply_log_i << "ReportsReply::SaveNewReport:"
                             << "\nReport Name - " << tmp_report_name
                             << "\nEncoded - " << report_name_encoded.c_str()
                             << "\nDecoded - " << report_name_decoded.c_str()
                             << "\nQuery: " << sql_statement << LOG_TERMINATOR;

  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteUpdateSyn(st, executionResult,error_msg);
  if(bswrc != BSWRC_OK)
    return 1; //DB error
  else
    return 0;
}


//---------------------------------------------------------------------------------------------
bool ReportsReply::isReportInWebTab(report_info_t* report_info)
{
  DBManager_c&  db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t     bswrc = BSWRC_OK,executionResult;
  Statement_c   st;
  ResultSet_c   rs;
  bool          dbirc = true, no_records = true;
  char          sql_statement[MAX_SQL_QUERY_SIZE];

  char          Name[33] ;
  char          Params[513] ;
  char          Render[52] ;

  er_sprintf(sql_statement,
             "select report_name,report_render,report_params from nla_report_info_web  where report_id=%d",
             report_info->report_id);

  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);

  Ushort_t total_records_num = rs.GetRecordCount();

  if (total_records_num > 0)
  {
    if(rs.Next() && bswrc == BSWRC_OK){
      if( !(rs.GetStringByName("report_name",(char *) Name))    ||
          !(rs.GetStringByName("report_render",(char *)Render)) ||
          !(rs.GetStringByName("report_params",(char *)Params)) )
      {
        return 0;
      }
      else
      {
        er_strcpy(report_info->report_name,Name);
        report_info->reply_buffer = Params;
        report_info->report_render = Render;
        return 1;
      }
    }
  }
  return 0;
}


//--------------------------------------------------------------
int ReportsReply::LoadOldDefConf(CsaWebServer *soap,unsigned int userId, ns1__DefaultWorkspaceReturnType &Response)
{
  DBManager_c&    db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t       bswrc = BSWRC_OK,executionResult;
  Statement_c     st;
  ResultSet_c     rs;
  bool            dbirc = true, no_records = true;
  char            sql_statement[MAX_SQL_QUERY_SIZE];
  geometry_t      geometry;
  unsigned short  report_id;

  er_sprintf(sql_statement,"select report_id, x, y, width, height from def_cfg where user_id = %d order by sequence_number",userId);

  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);

  Ushort_t total_records_num = rs.GetRecordCount();
  Ushort_t left_records_num = rs.GetRecordCount();
  Ushort_t first_record_index;

  csa_web_report_reply_log_i << "ReportsReply::LoadOldDefConf (userId " << userId << ")" 
                             << "\nQuery: " << sql_statement
                             << "\nNum Reports recieved: " << total_records_num << LOG_TERMINATOR;

  if (total_records_num == 0)
  {
    //NOTHING
  }
  else
  {
    while(left_records_num > 0)
    {
      //save the first record index
      first_record_index = total_records_num - left_records_num;
      //start to fill buffer with NlaGroupEntityInfo structures
      while(rs.Next() && bswrc == BSWRC_OK)
      {
        if( !(rs.GetUshortByName("report_id",(Ushort_t &)report_id))  ||
            !(rs.GetSshortByName("x",(Sshort_t &)geometry.x))         || //CAN BE NEGATIVE !!!
            !(rs.GetSshortByName("y",(Sshort_t &)geometry.y))         || //CAN BE NEGATIVE !!!
            !(rs.GetUshortByName("width",(Ushort_t &)geometry.width)) ||
            !(rs.GetUshortByName("height",(Ushort_t &)geometry.height)) )
        {
          //do nothing. get next result set
          //GetLog() << " AdiMngr_c::HandleAgentGroupListReply - opcode: Capp_Dbi_get_lst_grp_entities_req_op_code\n"
          //	<< " Can't get record from result set\n"<< LOG_TERMINATOR;
          left_records_num--;
        }
        else
        {
          InsertReportForDefaultWorkspace(soap,report_id,geometry,Response);
          left_records_num--;
          //if there are more info records to insert
          if (left_records_num == 0)
            break;
        }
      }
    }
  }

  return SOAP_OK;
}


//-------------------------------------------------------------------------------------
bool ReportsReply::TryLoadNewDefConf(CsaWebServer *soap,unsigned int userId,ns1__DefaultWorkspaceReturnType &Response)
{
  DBManager_c&    db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t       bswrc = BSWRC_OK,executionResult;
  Statement_c     st;
  ResultSet_c     rs;
  bool            dbirc = true, no_records = true;
  char            sql_statement[MAX_SQL_QUERY_SIZE];
  geometry_t      geometry;
  unsigned short  report_id;

  //15-Aug-2019 BZ#50532
  //3-Mar-2020  BZ#51021
  getFontLangFromDB(Response,userId);

  er_sprintf(sql_statement,"select report_id, x, y, width, height from def_cfg_web where user_id = %d order by sequence_number",userId);

  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);

  Ushort_t total_records_num = rs.GetRecordCount();
  Ushort_t left_records_num = rs.GetRecordCount();
  Ushort_t first_record_index;

  csa_web_report_reply_log_i << "ReportsReply::TryLoadNewDefConf (userId " << userId << ")" 
                             << "\nQuery: " << sql_statement
                             << "\nNum Reports recieved: " << total_records_num << LOG_TERMINATOR;

  if (total_records_num == 0)
  {
    return 0;
  }
  else
  {
    while(left_records_num > 0)
    {
      //save the first record index
      first_record_index = total_records_num - left_records_num;
      //start to fill buffer with NlaGroupEntityInfo structures
      while(rs.Next() && bswrc == BSWRC_OK)
      {
        if( !(rs.GetUshortByName("report_id",(Ushort_t &)report_id))  ||
            !(rs.GetSshortByName("x",(Sshort_t &)geometry.x))         || //CAN BE NEGATIVE !!!
            !(rs.GetSshortByName("y",(Sshort_t &)geometry.y))         ||  //CAN BE NEGATIVE !!!
            !(rs.GetUshortByName("width",(Ushort_t &)geometry.width)) ||
            !(rs.GetUshortByName("height",(Ushort_t &)geometry.height)) )
        {
          //do nothing. get next result set
          //GetLog() << " AdiMngr_c::HandleAgentGroupListReply - opcode: Capp_Dbi_get_lst_grp_entities_req_op_code\n"
          //         << " Can't get record from result set\n"<< LOG_TERMINATOR;
          left_records_num--;
        }
        else
        {
          InsertReportForDefaultWorkspace(soap ,report_id,geometry,Response);
          left_records_num--;
          //if there are more info records to insert
          if (left_records_num == 0)
            break;
        }
      }
    }
  }

  return 1;
}


//---------------------------------------------------------------------------------------------
bool ReportsReply::isReportPublic(unsigned short  report_id,unsigned int UserId,bool &isReportExist)
{
  Ulong_t         creator_id;
  DBManager_c&    db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t       bswrc = BSWRC_OK,executionResult;
  Statement_c     st;
  ResultSet_c     rs;
  bool            dbirc = true, no_records = true;
  char            sql_statement[MAX_SQL_QUERY_SIZE];
  Ushort_t        total_records_num;

  er_sprintf(sql_statement,"select creator_id from nla_report_hdr where report_id = %d ",report_id);
  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);
  total_records_num = rs.GetRecordCount();

  csa_web_report_reply_log_i << "isReportPublic (UserId " << UserId << ") - STEP#1"
                             << "\nQuery - " << sql_statement
                             << "\nNum of Records received - " << total_records_num
                             << LOG_TERMINATOR;

  if (total_records_num > 0)
  {
    if(rs.Next() && bswrc == BSWRC_OK)
    {
      csa_web_report_reply_log_i << "Report Exist - ";
      if (!(rs.GetUlongByName("creator_id",creator_id)))
      {
        csa_web_report_reply_log_i << "NOT EXIST" << LOG_TERMINATOR;
      }
      else
      {
        isReportExist = true;
        if ( creator_id == PUBLIC_OWNER_ID )
        {
          csa_web_report_reply_log_i << "PUBLIC" << LOG_TERMINATOR;
          return true; //PUBLIC;
        }
        else
        {
          if (creator_id == UserId)
          {
            csa_web_report_reply_log_i << "PRIVATE" << LOG_TERMINATOR;
            return  false; //PRIVATE
          }
          else
          {
            csa_web_report_reply_log_i << "NOT EXIST" << LOG_TERMINATOR;
            isReportExist = false;
            return false;
          }
        }
      }
    }
  }

  // If Report not found in the OLD TAB , Try to Find it in NEW TAB
  er_sprintf(sql_statement,"select creator_id from nla_report_info_web where report_id = %d ",report_id);
  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);
  total_records_num = rs.GetRecordCount();

  csa_web_report_reply_log_i << "ReportsReply::isReportPublic (UserId " << UserId << ") - STEP#2"
                             << "\nQuery - " << sql_statement
                             << "\nNum of Records received - " << total_records_num
                             << LOG_TERMINATOR;

  if (total_records_num > 0)
  {
    if(rs.Next() && bswrc == BSWRC_OK)
    {
      csa_web_report_reply_log_i << "Report Exist - ";
      if (!(rs.GetUlongByName("creator_id",creator_id)))
      {
        csa_web_report_reply_log_i << "NOT EXIST" << LOG_TERMINATOR;
      }
      else
      {
        csa_web_report_reply_log_i << " (creator_id " << creator_id << ") ";
        isReportExist = true;
        if ( creator_id == PUBLIC_OWNER_ID )
        {
          csa_web_report_reply_log_i << "PUBLIC" << LOG_TERMINATOR;
          return true; //PUBLIC;
        }
        else
        {
          if (creator_id == UserId)
          {
            csa_web_report_reply_log_i << "PRIVATE" << LOG_TERMINATOR;
            return  false; //PRIVATE
          }
          else
          {
            csa_web_report_reply_log_i << "NOT EXIST" << LOG_TERMINATOR;
            isReportExist = false;
            return false;
          }
        }
      }
    }
  }

  csa_web_report_reply_log_i << "NOT EXIST" << LOG_TERMINATOR;
  isReportExist = false;
  return  false;  //NOT FOUND in BOTH TABs
}

//15-Aug-2019 BZ#50532
//---------------------------------------------------------------------------------------------
void ReportsReply::setFontLangInDB(ns1__SaveWorkspaseRequestType *Workspace, Ushort_t user_id)
{
  DBManager_c&  db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t     bswrc = BSWRC_OK,executionResult;
  Statement_c   st;
  ResultSet_c   rs;
  char          sql_statement[MAX_SQL_QUERY_SIZE];
  char          error_msg[MAX_SQL_QUERY_SIZE];

  char font_lang[8];
  memset(font_lang,0,8);

  if(er_strcmp(Workspace->fontSize.c_str(),"smallSize")==0)
    strcat(font_lang,"S:");
  else if(er_strcmp(Workspace->fontSize.c_str(),"meduimSize")==0)
    strcat(font_lang,"M:");
  else if(er_strcmp(Workspace->fontSize.c_str(),"bigSize")==0)
    strcat(font_lang,"B:");
  else
    strcat(font_lang,"S:");

  strcat(font_lang,Workspace->language.c_str());

  //if(strcmp(Workspace->language.c_str(),"English"))
  //  strcat(font_lang,"En");
  //else if(strcmp(Workspace->language.c_str(),"Hebrew"))
  //  strcat(font_lang,"He");
  //else if(strcmp(Workspace->language.c_str(),"Chinese"))
  //  strcat(font_lang,"Ch");
  //else if(strcmp(Workspace->language.c_str(),"Russian"))
  //  strcat(font_lang,"Ru");

  er_sprintf(sql_statement,"UPDATE sup SET sup_extension=\"%s\" WHERE user_id = %d", font_lang, user_id);
  st.SetQuery(sql_statement);

  csa_web_report_reply_log_i << "STEP #3 update Font Size " << Workspace->fontSize.c_str() << " & Language " << Workspace->language.c_str() << " in sup table\n"
                             << "Query - " << sql_statement << LOG_TERMINATOR;

  bswrc = db_mngr.ExecuteUpdateSyn(st,executionResult,error_msg);
  if(bswrc != BSWRC_OK)
  {
    csa_web_report_reply_log_i << "Fail to update Font Size & Language in sup table\n" << LOG_TERMINATOR;
  }
}

//---------------------------------------------------------------------------------------------
void ReportsReply::getFontLangFromDB(ns1__DefaultWorkspaceReturnType &Response, Ushort_t user_id)
{
  DBManager_c&    db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t       bswrc = BSWRC_OK,executionResult;
  Statement_c     st;
  ResultSet_c     rs;
  bool            dbirc = true, no_records = true;
  char            sql_statement[MAX_SQL_QUERY_SIZE];

  er_sprintf(sql_statement,"select sup_extension from sup where user_id = %d",user_id);

  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);

  char sup_extension[8];
  if(rs.Next() && rs.GetStringByName("sup_extension",sup_extension))
  {
    csa_web_report_reply_log_i << "ReportsReply::TryLoadNewDefConf (userId " << user_id << ")" 
                               << "\nQuery: " << sql_statement
                               << "\nsup_extension: " << sup_extension << LOG_TERMINATOR;

    //char *next_token;
    char* pchr = er_strtok(sup_extension,":");
    //char* pchr = strtok_s(sup_extension,":",&next_token);
    if(pchr != NULL)
    {
      //if(*pchr == 'S')
      //  Response.fontSize = "smallSize";
      if(*pchr == 'M')
        Response.fontSize = "meduimSize";
      else if(*pchr == 'B')
        Response.fontSize = "bigSize";
    }

    pchr = er_strtok(NULL,":");
    //pchr = strtok_s(sup_extension,":",&next_token);
    if(pchr != NULL)
      Response.language = pchr;
  }
}


