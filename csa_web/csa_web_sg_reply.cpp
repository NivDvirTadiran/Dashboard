#include "er_std_override.h"
#include "csa_web_sg_reply.h"
#include "csa_web_commondefs.hpp"

static BswLog_c csa_web_sg_reply_log_d(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_DETAILED);
static BswLog_c csa_web_sg_reply_log_i(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_INFO);
static BswLog_c csa_web_sg_reply_log_e(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_ERROR);

//------------------------------------------------------------------------------------------------
CsaRc_t SuperGroup_Reply::MakeSuperGroupInfo(CsaWebServer *soap,SuperGroupRecord *supergroup_rec, ns1__BlockItemType *ReturnArrayData )
{
  CsaRc_t                           rc = ARC_OK;
  Flag_t                            changed;
  //CsaOnlineGroupStatistics_t*       event_data1 = NULL;
  //CsaFlaSgroupAcdStat_t*            event_data2 = NULL;
  //CsaFlaSgroupNAcdStat_t*           event_data3 = NULL;
  //CsaFlaSgroupAgentsAvailability_t* event_data4 = NULL;

  csa_web_sg_reply_log_d << "SuperGroup_Reply::MakeSuperGroupInfo <id:" << supergroup_rec->getSuperGroupId() << ">" << LOG_TERMINATOR;

  rc = supergroup_rec->MakeEventData(&supergroup_rec->getOnlineGroupStatistics(),changed, false);
  MakeSuperGroupInfo_OnlineStat(soap,supergroup_rec,ReturnArrayData );
  supergroup_rec->DumpToLog(csa_web_sg_reply_log_d);
  csa_web_sg_reply_log_d << LOG_TERMINATOR;
  //rc = supergroup_rec->MakeEventData(event_data2);
  //MakeSuperGroupInfo_AcdStat(soap,supergroup_rec,ReturnArrayData );
  //rc = supergroup_rec->MakeEventData(event_data3);
  //MakeSuperGroupInfo_AgentsAvailability(soap,supergroup_rec,ReturnArrayData );

  return rc;
}


//------------------------------------------------------------------------------------------------
CsaRc_t SuperGroup_Reply::MakeSuperGroupDailyInfo(CsaWebServer *soap,SuperGroupRecord *supergroup_rec, ns1__BlockItemType *ReturnArrayData )
{
  CsaRc_t                           rc = ARC_OK;
  Flag_t                            changed;
  //CsaOnlineGroupStatistics_t*       event_data1 = NULL;
  //CsaFlaSgroupAcdStat_t*            event_data2 = NULL;
  //CsaFlaSgroupNAcdStat_t*           event_data3 = NULL;
  //CsaFlaSgroupAgentsAvailability_t* event_data4 = NULL;

  csa_web_sg_reply_log_d << "SuperGroup_Reply::MakeSuperGroupDailyInfo <id:" << supergroup_rec->getSuperGroupId() << ">" << LOG_TERMINATOR;

  rc = supergroup_rec->MakeEventData(&supergroup_rec->getOnlineGroupStatistics(),changed,true);
  MakeSuperGroupInfo_OnlineStat(soap,supergroup_rec,ReturnArrayData );
  supergroup_rec->DumpToLog(csa_web_sg_reply_log_d);
  csa_web_sg_reply_log_d << LOG_TERMINATOR;
  //rc = supergroup_rec->MakeEventData(event_data2);
  //MakeSuperGroupInfo_AcdStat(soap,supergroup_rec,ReturnArrayData );
  //rc = supergroup_rec->MakeEventData(event_data3);
  //MakeSuperGroupInfo_AgentsAvailability(soap,supergroup_rec,ReturnArrayData );

  return rc;
}


//------------------------------------------------------------------------------------------------
CsaRc_t SuperGroup_Reply::MakeSuperGroupInfo_OnlineStat(CsaWebServer *soap,SuperGroupRecord *supergroup_rec, ns1__BlockItemType *ReturnArrayData )
{
  CsaRc_t   rc = ARC_OK;
  Ulong_t   accepted_calls = 0;
  Ulong_t   answered_calls = 0;

  if(supergroup_rec != NULL)
  {
    InsertFullDataBG(soap,NULL,"f6_3_4_2_1","In Q",supergroup_rec->getOnlineGroupStatistics().queue.queued_calls,ReturnArrayData); 
    InsertFullDataBG(soap,NULL,"f6_3_4_2_2","Above T.ASA",supergroup_rec->getOnlineGroupStatistics().queue.waiting_above_target_asa,ReturnArrayData); 
    InsertFullDataBG(soap,NULL,"f6_3_4_2_3","Ave. in Q.", (Ulong_t)NlaCalcCsaElapsedTime(supergroup_rec->getOnlineGroupStatistics().queue.avg_time_in_queue),ReturnArrayData);
    InsertFullDataBG(soap,NULL,"f6_3_4_2_4","Max. in Q.",supergroup_rec->getOnlineGroupStatistics().queue.longest_queue_time,ReturnArrayData); 
    //InsertFullDataBG(soap,NULL,"f6_3_4_2_4","longest wait time",supergroup_rec->getOnlineGroupStatistics().acd.longest_que_time_on_cur_int,ReturnArrayData); 
    //28-Jan-2021 YR BZ#53704
    InsertFullDataBG(soap,NULL,"f6_3_4_2_5","Outbound Pend",supergroup_rec->getOnlineGroupStatistics().pending_outb.queued_calls,ReturnArrayData); 
    InsertFullDataBG(soap,NULL,"f6_3_4_2_7","Outbound Longest Pend",supergroup_rec->getOnlineGroupStatistics().pending_outb.longest_queue_time,ReturnArrayData); 
    InsertFullDataBG(soap,NULL,"f6_3_4_3_1","No. Agents",supergroup_rec->getOnlineGroupStatistics().agents.number_of_agents,ReturnArrayData); 
    InsertFullDataBG(soap,NULL,"f6_3_4_3_2","No. Release",supergroup_rec->getOnlineGroupStatistics().agents.agents_on_release,ReturnArrayData); 
    InsertFullDataBG(soap,NULL,"f6_3_4_3_4","No. Idle",supergroup_rec->getOnlineGroupStatistics().agents.idle_agents,ReturnArrayData); 
    InsertFullDataBG(soap,NULL,"f6_3_4_3_5","No. ACD",supergroup_rec->getOnlineGroupStatistics().agents.acd_busy_agents,ReturnArrayData); 
    InsertFullDataBG(soap,NULL,"f6_3_4_3_6","No. Busy",supergroup_rec->getOnlineGroupStatistics().agents.non_acd_busy_agents + supergroup_rec->getOnlineGroupStatistics().agents.busy_agents,ReturnArrayData); 
    InsertFullDataBG(soap,NULL,"f6_3_4_3_10","No. Split",supergroup_rec->getOnlineGroupStatistics().agents.agents_on_split,ReturnArrayData); 
    InsertFullDataBG(soap,NULL,"f6_3_4_3_14","No. OACD",supergroup_rec->getOnlineGroupStatistics().agents.o_acd_busy_agents,ReturnArrayData); 
    InsertFullDataBG(soap,NULL,"f6_3_4_4_2","Accepted",supergroup_rec->getOnlineGroupStatistics().acd.accepted_calls,ReturnArrayData); 
    InsertFullDataBG(soap,NULL,"f6_3_4_4_3","Answered",supergroup_rec->getOnlineGroupStatistics().acd.answered_calls,ReturnArrayData); 
    InsertFullDataBG(soap,NULL,"f6_3_4_4_4","Abandoned",supergroup_rec->getOnlineGroupStatistics().acd.abandoned_calls,ReturnArrayData); 
    InsertFullDataBG(soap,NULL,"f6_3_4_4_18","Outbound handled",supergroup_rec->getOnlineGroupStatistics().o_acd.handled_calls,ReturnArrayData); 
    InsertFullDataBG(soap,NULL,"f6_3_4_4_20","Outbound Failed",supergroup_rec->getOnlineGroupStatistics().o_acd.failed_calls,ReturnArrayData); 

    accepted_calls = supergroup_rec->getOnlineGroupStatistics().acd.accepted_calls;
    answered_calls = supergroup_rec->getOnlineGroupStatistics().acd.answered_calls;
    if (accepted_calls > 0 )
    {
      InsertFullDataBG(soap,NULL,"f6_3_4_4_3a","Answered Calls Percentage",(unsigned int)((answered_calls * 100 ) /accepted_calls),ReturnArrayData);
    }
    else
      InsertFullDataBG(soap,NULL,"f6_3_4_4_3a","Answered Calls Percentage",0,ReturnArrayData);
    }

  return rc;
}


//------------------------------------------------------------------------------------------------
CsaRc_t SuperGroup_Reply::MakeSuperGroupInfo_AcdStat(CsaWebServer *soap,SuperGroupRecord *supergroup_rec, ns1__BlockItemType *ReturnArrayData )
{
  CsaRc_t                       rc = ARC_OK;   

  return rc;
}


//------------------------------------------------------------------------------------------------
CsaRc_t SuperGroup_Reply::MakeSuperGroupInfo_AgentsAvailability(CsaWebServer *soap,SuperGroupRecord *supergroup_rec, ns1__BlockItemType *ReturnArrayData )
{
  CsaRc_t                       rc = ARC_OK;   

  return rc;
}


//------------------------------------------------------------------------------------------------
CsaRc_t  SuperGroup_Reply::MakeSuperGroupInfoEMPTY(CsaWebServer *soap,unsigned int supergroup_id, ns1__BlockItemType *ReturnArrayData )
{
  CsaRc_t                       rc = ARC_OK;   

  InsertFullDataBG(soap,NULL,"f6_3_4_2_1","In Q",0,ReturnArrayData); 
  InsertFullDataBG(soap,NULL,"f6_3_4_2_2","Above T.ASA",0,ReturnArrayData); 
  InsertFullDataBG(soap,NULL,"f6_3_4_2_4","Max. in Q.",0,ReturnArrayData); 
  InsertFullDataBG(soap,NULL,"f6_3_4_2_5","Outbound In Q",0,ReturnArrayData); 
  InsertFullDataBG(soap,NULL,"f6_3_4_3_1","No. Agents",0,ReturnArrayData); 
  InsertFullDataBG(soap,NULL,"f6_3_4_3_2","No. Release",0,ReturnArrayData); 
  InsertFullDataBG(soap,NULL,"f6_3_4_3_4","No. Idle",0,ReturnArrayData); 
  InsertFullDataBG(soap,NULL,"f6_3_4_3_5","No. ACD",0,ReturnArrayData); 
  InsertFullDataBG(soap,NULL,"f6_3_4_3_6","No. Busy",0,ReturnArrayData); 
  InsertFullDataBG(soap,NULL,"f6_3_4_3_10","No. Split",0,ReturnArrayData); 
  InsertFullDataBG(soap,NULL,"f6_3_4_3_14","No. OACD",0,ReturnArrayData); 
  InsertFullDataBG(soap,NULL,"f6_3_4_4_2","Accepted",0,ReturnArrayData); 
  InsertFullDataBG(soap,NULL,"f6_3_4_4_3","Answered",0,ReturnArrayData); 
  InsertFullDataBG(soap,NULL,"f6_3_4_4_4","Abandoned",0,ReturnArrayData); 
  InsertFullDataBG(soap,NULL,"f6_3_4_4_18","Outbound handled",0,ReturnArrayData); 
  InsertFullDataBG(soap,NULL,"f6_3_4_4_20","Outbound Failed",0,ReturnArrayData); 

  return rc;
}


//------------------------------------------------------------------------------------------------
void SuperGroup_Reply::InsertSuperGroupList(CsaWebServer *soap, 
                                            unsigned int   Id,
                                            std::string    Name,
                                            vector<Id_type> group_list,
                                            ns1__SuperGroupListReturnType &ReturnArrayData)
{
  ns1__SuperGroupListDataItemType* tmp_sg;
  tmp_sg = soap_new_ns1__SuperGroupListDataItemType(soap, 1);

  tmp_sg->superGroupId = Id; 
  tmp_sg->superGroupName = Name;

  csa_web_sg_reply_log_d << "SuperGroup_Reply::InsertSuperGroupList <" 
                         << tmp_sg->superGroupId << "," << tmp_sg->superGroupName.c_str() << "<";

  for( vector<Id_type>::iterator it = group_list.begin();
       it != group_list.end();
       ++it)
  {
    ns1__GroupListDataItemType *tmp_g;
      /* std::cout << *it; ... */
    tmp_g = soap_new_ns1__GroupListDataItemType(soap, 1);
    tmp_g->grpId = *it;
    tmp_g->grpName = "";            //grpName;
    tmp_g->grpNumber = "";          //grpNumber;
    tmp_g->grpEmailEnable = false;  //grpEmailEnable;
    tmp_sg->returnArray.push_back(tmp_g);
    csa_web_sg_reply_log_d << "," << *it; 
  }

  csa_web_sg_reply_log_d << ">>"
                         << LOG_TERMINATOR;
  //tmp->returnArray = group_list;

  soap->m_net_data_ptr->SGroupL_vector.push_back(tmp_sg);
  ReturnArrayData.returnArray.push_back(tmp_sg); 
}

///////////////////////////////////////////////////////
CsaRc_t SuperGroup_Reply::StartMonitor(unsigned int super_group_id)
{ //we can send multiple messages  to ARMAIN.cpp according the same group_id ,
  // becouse of internal validation in setNlaDumpOn() ,
  CsaRc_t             rc = ARC_OK;

  EosEventHeader_t    event_header;
  EosRetCode_t        erc;
  NLA_CSA_001         reg_req;

  event_header.dest_info = -1; //PVL 7-02-2010 
  event_header.op_code = NLA_CSA_reg_for_super_grps_req ;
  event_header.dest_que_id = CSA_Q4;
  event_header.reply_que_id = 0;//DUMMY , not in use  //RBE_Q2;
  event_header.data_len = sizeof(reg_req);
  reg_req.entity_id = super_group_id;
  reg_req.refresh_interval = 1;
  reg_req.tbl_index = 0;
  erc = EosCreateAndSendEvent (NLA_TH1, event_header.dest_que_id , event_header, &reg_req, event_header.data_len);
  return rc;
}
