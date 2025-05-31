#include "er_std_override.h"
#include "csa_web_agent_reply.hpp"
#include "csa_web_commondefs.hpp"

#ifdef _WIN32
#pragma warning(disable:4996)
#endif


static BswLog_c csa_web_agent_reply_log_d(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_DETAILED);
static BswLog_c csa_web_agent_reply_log_i(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_INFO);

//============[ ns1__SaveWorkspaseRequestType::Implemented Methods ]=========
//ctor
AgentRecordReply::AgentRecordReply()
{
  //char address[20];
  ////sprintf(address, "%p", this);
  //er_sprintf(address, "%p", this);
  //csa_web_agent_reply_log_d << "ctor of AgentRecordReply(" << address << ")" << LOG_TERMINATOR;
  //CurrentRow = NULL;
}

//dtor
AgentRecordReply::~AgentRecordReply()
{
  //char address[20];
  ////sprintf(address, "%p", this);
  //er_sprintf(address, "%p", this);
  //csa_web_agent_reply_log_d << "dtor of AgentRecordReply(" << address << ")" << LOG_TERMINATOR;
  //if(CurrentRow != NULL)
  //  CurrentRow->soap_dealloc();
}

//------------------------------------------------------------------------------------------------
void AgentRecordReply::InsertAgentsList(CsaWebServer*  soap,
                                        unsigned int   AgentId,
                                        std::string    AgentName,
                                        std::string    AgentNumber,
                                        ns1__AgentsListReturnType &ReturnArrayData)
{
  ns1__AgentsListDataItemType *tmp;
  tmp = soap_new_ns1__AgentsListDataItemType(soap, 1);
  tmp->agentId = AgentId;
  tmp->agentName = AgentName;
  tmp->agentNumber = AgentNumber;

  //soap->m_net_data_ptr->AgentsListType_vector.push_back(tmp);
  ReturnArrayData.returnArray.push_back(tmp);
}


//------------------------------------------------------------------------------------------------
void AgentRecordReply::AddNewGroupBlock(CsaWebServer *soap,unsigned int groupId, ns1__ReturnMatrixDataTypePerGroup &ReturnData)
{   
  ns1__returnDataRowsPerGroup*   tmp;
  tmp = soap_new_ns1__returnDataRowsPerGroup(soap, 1);
  tmp->Id = groupId;
  tmp->Name = soap->m_db_data_ptr->groupstab.GetGroupNameById(groupId);

  csa_web_agent_reply_log_d << "AgentRecordReply::AddNewGroupBlock <" << tmp->Id << "," << tmp->Name.c_str() << ">" << LOG_TERMINATOR;

  ReturnData.returnDataRowsPerGroup.push_back(tmp);
  //soap->m_net_data_ptr->MATRIX_PER_GROUP_vector.push_back(tmp);
}


//------------------------------------------------------------------------------------------------

void AgentRecordReply::CreateNewRow(CsaWebServer *soap)
{
  CurrentRow = soap_new_ns1__DataItemRow(soap, 1);
}


//------------------------------------------------------------------------------------------------
void AgentRecordReply::InsertFullDataBG(CsaWebServer *soap,unsigned int *bgcolor_ptr, std::string ersid,std::string ersname,std::string value)
{
  ns1__DataItemType* tmp;

  tmp = soap_new_ns1__DataItemType(soap, 1);
  tmp->ersid = ersid;
  tmp->ersname =ersname;
  tmp->value = value;
  if(bgcolor_ptr!=NULL){
    tmp->bgcolor = (unsigned int *)bgcolor_ptr;
  }

  CurrentRow->returnArray.push_back(tmp);

  //soap->m_net_data_ptr->DIT_vector.push_back(tmp);
}


//------------------------------------------------------------------------------------------------
// For Array of Matrix (AGENTS ) per Group
void AgentRecordReply::InsertRowToMatrix(CsaWebServer *soap,ns1__returnDataRowsPerGroup* matrix)
{
  matrix->returnMatrix.push_back(CurrentRow);
  //soap->m_net_data_ptr->ROW_vector.push_back(CurrentRow);
  csa_web_agent_reply_log_d << "AgentRecordReply::InsertRowToMatrix <ns1__returnDataRowsPerGroup>, size of returnMatrix " << matrix->returnMatrix.size() << LOG_TERMINATOR;
}


//--------------------------------------------------------------------------------------------
// For Array of Matrix (AGENTS ) only
void AgentRecordReply::InsertRowToMatrix(CsaWebServer *soap,ns1__ReturnMatrixDataTypeBG& matrix)
{
  matrix.returnMatrix.push_back(CurrentRow);
  //soap->m_net_data_ptr->ROW_vector.push_back(CurrentRow);
  csa_web_agent_reply_log_d << "AgentRecordReply::InsertRowToMatrix <ns1__ReturnMatrixDataTypeBG>, size of returnMatrix " << matrix.returnMatrix.size() << LOG_TERMINATOR;
}


//--------------------------------------------------------------------------------------------
void AgentRecordReply::InsertAgentDataToRow(CsaWebServer *            soap,
                                            //GroupRecordReply          &ReplyObj,
                                            AgentRecord*              agent,
                                            CsaAgentPerGroupRecord_t* agent_per_group,
                                            unsigned int              groupId)
{
  unsigned int *StateColor = NULL;
  unsigned int *TmpColorPtr = NULL;
  Ulong_t  num_in_voice_calls = 0;
  Ulong_t  num_out_voice_calls = 0;
  Ulong_t  number_of_non_acd_calls = 0;
  Ulong_t  num_chats = 0;
  Ulong_t  num_emails = 0;
  Ulong_t  currently_held_calls = 0;
  Ulong_t  longest_held_call = 0;
  Ulong_t  in_conference = 0;
  Ulong_t  time_in_conference = 0;
  Ulong_t  acc_non_acd_time = 0;

  int acd ;
  int chat_acd;
  int email_acd;
  int cb_acd;
  int abnd_acd;
  int dl_acd;
  Ulong_t  state_delta_time=0;
  Ulong_t  total_talk_time=0;
  Ulong_t  answered_calls=0;
  Ulong_t  total_wrap_up_time=0;
  Ulong_t  total_reserved_time=0;
  Ulong_t  chat_answered_calls=0;
  Ulong_t  email_answered_calls=0;
  Ulong_t  total_outbound_acd_time=0;
  Ulong_t  oacd_answered_calls=0;
  string State_name;
  string State_Time;
  InsertFullDataBG(soap,NULL,"f6_3_2_1_0","Agent Id",Ulong2String(agent->Agent_id));
  InsertFullDataBG(soap,NULL,"f6_3_2_1_1","Agent Name",agent->Name);
  InsertFullDataBG(soap,NULL,"f6_3_2_1_2","Agent No.",(char *)agent->Number);
  InsertFullDataBG(soap,NULL,"f6_3_2_1_3","Agent Exten.",(char *)agent->Device);

  soap->m_db_data_ptr->colortab.GetBGColorsByState(agent_per_group->Availability.state,
                                                   agent_per_group->Availability.in_conference.get(),
                                                   &StateColor,
                                                   State_name);
  //{RR: split handle}
  //split_grp_dn :=   csa_online_agent_availability_per_group_login_statistics.split_grp_dn;
  //data_list.InsertState (f6_3_2_2_1, state, split_grp_dn);
  InsertFullDataBG(soap,StateColor,"f6_3_2_2_1","State",State_name);


  //31-Oct-2019 YR BZ#51096
  //state_delta_time = NlaCalcCsaElapsedTime((unsigned long)agent_per_group->Availability.state_time.get());
  state_delta_time = NlaCalcCsaElapsedTime(agent_per_group->Availability.running_state_time.get());
  State_Time = Ulong2String(state_delta_time);
  if( soap->m_db_data_ptr->limitstab.GetColorOfSTATE(groupId,
                                                     agent_per_group->Availability.state,                 //STATE
                                                     soap->m_db_data_ptr->limitstab.ersid_val.f6_3_2_2_2, // ERS_ID  from DB
                                                     state_delta_time,                                    // Value
                                                     &TmpColorPtr) )                                      // returned color
  {
    InsertFullDataBG(soap,TmpColorPtr,"f6_3_2_2_2","State Time",State_Time);
  }
  else
  {
    InsertFullDataBG(soap,NULL,"f6_3_2_2_2","State Time",State_Time);
  }


  InsertFullDataBG(soap, NULL, "f6_3_2_2_11", "Release Code", (char*)agent_per_group->Availability.release_code_name);

  num_in_voice_calls = (unsigned long)agent_per_group->Availability.number_of_current_calls[getOffSetOfAcdCallTypes(CL_CALL_TYPE_ACD)].get();
  InsertFullDataBG(soap,NULL,"f6_3_2_2_7","InB. ACD Calls",Ulong2String(num_in_voice_calls));

  if (Csa_Setup.supportOutbound())
    num_out_voice_calls =(unsigned long)agent_per_group->Availability.number_of_current_calls[getOffSetOfAcdCallTypes(CL_CALL_TYPE_CALLBACK)].get() +
    (unsigned long)agent_per_group->Availability.number_of_current_calls[getOffSetOfAcdCallTypes(CL_CALL_TYPE_ABANDONED)].get();
  if (Csa_Setup.supportDialList())
    num_out_voice_calls += (unsigned long)agent_per_group->Availability.number_of_current_calls[getOffSetOfAcdCallTypes(CL_CALL_TYPE_DIAL_LIST)].get();
  InsertFullDataBG(soap,NULL,"f6_3_2_2_10","O.ACD Calls",Ulong2String(num_out_voice_calls));

  number_of_non_acd_calls = agent_per_group->Availability.number_of_non_acd_calls.get();
  InsertFullDataBG(soap,NULL,"f6_3_2_2_6","NON-ACD Calls", Ulong2String(number_of_non_acd_calls));

  if (Csa_Setup.supportChat())
    num_chats = agent_per_group->Availability.number_of_current_calls[getOffSetOfAcdCallTypes(CL_CALL_TYPE_CHAT)].get();
  InsertFullDataBG(soap,NULL,"f6_3_2_2_8","Chat Contacts",Ulong2String(num_chats));

  if (Csa_Setup.supportEmail())
    num_emails = agent_per_group->Availability.number_of_current_calls[getOffSetOfAcdCallTypes(CL_CALL_TYPE_EMAIL)].get();
  InsertFullDataBG(soap,NULL,"f6_3_2_2_9","Email Contacts",Ulong2String(num_emails));

  InsertFullDataBG(soap,NULL,"f6_3_2_2_4","DNIS",(const char *)agent_per_group->Availability.dnis);

  InsertFullDataBG(soap,NULL,"f6_3_2_2_5","ANI",(const char *)agent_per_group->Availability.ani);

  currently_held_calls = agent_per_group->Availability.currently_held_calls.get();
  InsertFullDataBG(soap,NULL,"f6_3_2_2_3","Hold",Ulong2String(currently_held_calls));

  if(currently_held_calls > 0)
    longest_held_call = NlaCalcCsaElapsedTime((unsigned long)agent_per_group->Availability.hold_time.get());
  InsertFullDataBG(soap,NULL,"f6_3_2_2_12","Longest On Held Time",Ulong2String(longest_held_call));

  in_conference = agent_per_group->Availability.in_conference.get();
  InsertFullDataBG(soap,NULL,"f6_3_2_2_13","In Conference",Ulong2String(in_conference));

  if(in_conference > 0)
    time_in_conference = NlaCalcCsaElapsedTime((unsigned long)agent_per_group->Availability.time_in_conference.get());
  InsertFullDataBG(soap,NULL,"f6_3_2_2_14","Time In Conference",Ulong2String(time_in_conference));

  //
  acc_non_acd_time = agent->Non_acd.total_talk_of_ext_inc_calls.get() +
                     agent->Non_acd.total_talk_of_int_inc_calls.get() +
                     agent->Non_acd.total_time_of_int_out_calls.get() +
                     agent->Non_acd.total_time_of_ext_out_calls.get();

  InsertFullDataBG(soap,NULL,"f6_3_2_3_11","non_acd_time",Ulong2String(acc_non_acd_time));

  //{RR: prim group}
  InsertFullDataBG(soap,NULL,"f6_3_2_1_4","prim_grp_dn",(const char *)agent_per_group->Availability.prim_grp_dn);
  //--------------------
  //{RR: priority}
  InsertFullDataBG(soap,NULL,"f6_3_2_1_5","priority",Ulong2String(agent_per_group->Availability.priority));
  //--------------------
  InsertFullDataBG(soap,NULL,"f6_3_2_3_8","idle_time",Ulong2String(agent_per_group->Availability.total_idle_time.get()));
  //--------------------

  acd = getOffSetOfAcdCallTypes(CL_CALL_TYPE_ACD);
  total_talk_time       = agent_per_group->Acd[acd].total_talk_time.get();
  answered_calls        = agent_per_group->Acd[acd].answered_calls.get();
  total_wrap_up_time    = agent_per_group->Acd[acd].total_wrap_up_time.get();
  total_reserved_time   = agent_per_group->Availability.total_reserved_time.get();

  if (Csa_Setup.supportChat())
  {
    chat_acd = getOffSetOfAcdCallTypes(CL_CALL_TYPE_CHAT);
    total_talk_time += agent_per_group->Acd[chat_acd].total_talk_time.get();
    chat_answered_calls = agent_per_group->Acd[chat_acd].answered_calls.get();
  }

  if (Csa_Setup.supportEmail())
  {
    email_acd = getOffSetOfAcdCallTypes(CL_CALL_TYPE_EMAIL);
    total_talk_time += agent_per_group->Acd[email_acd].total_talk_time.get();
    email_answered_calls = agent_per_group->Acd[email_acd].answered_calls.get();
  }

  if (Csa_Setup.supportOutbound())
  {
    cb_acd = getOffSetOfAcdCallTypes(CL_CALL_TYPE_CALLBACK);
    abnd_acd = getOffSetOfAcdCallTypes(CL_CALL_TYPE_ABANDONED);
    total_outbound_acd_time = agent_per_group->Acd[cb_acd].total_talk_time.get() +
                              agent_per_group->Acd[abnd_acd].total_talk_time.get() +
                              agent_per_group->Availability.total_reserved_time.get();

    total_wrap_up_time += agent_per_group->Acd[cb_acd].total_wrap_up_time.get() +
                          agent_per_group->Acd[abnd_acd].total_wrap_up_time.get();

    oacd_answered_calls = agent_per_group->Acd[cb_acd].answered_calls.get() +
                          agent_per_group->Acd[abnd_acd].answered_calls.get();
  }

  if (Csa_Setup.supportDialList())
  {
    dl_acd = getOffSetOfAcdCallTypes(CL_CALL_TYPE_DIAL_LIST);
    total_outbound_acd_time += agent_per_group->Acd[dl_acd].total_talk_time.get();
    total_wrap_up_time += agent_per_group->Acd[dl_acd].total_wrap_up_time.get();
    oacd_answered_calls += agent_per_group->Acd[dl_acd].answered_calls.get();
  }

  InsertFullDataBG(soap,NULL,"f6_3_2_3_4","total_acd_answered",Ulong2String(answered_calls));
  //--------------------
  InsertFullDataBG(soap,NULL,"f6_3_2_3_9","ACD_total_talk_time",Ulong2String(total_talk_time));
  //--------------------
  InsertFullDataBG(soap,NULL,"f6_3_2_3_10","total_wrap_up_time",Ulong2String(total_wrap_up_time));
  //--------------------
  InsertFullDataBG(soap,NULL,"f6_3_2_3_39","Res+Out time",Ulong2String(total_reserved_time + total_outbound_acd_time));
  //--------------------
  InsertFullDataBG(soap,NULL,"f6_3_2_3_34","split_time",Ulong2String(agent_per_group->Availability.total_split_time.get()));
  //--------------------
  soap->m_db_data_ptr->limitstab.GetColorOfERS(agent_per_group->Group_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_2_3_12,
    agent_per_group->Availability.total_release_time.get(),&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_2_3_12","rls_time",Ulong2String(agent_per_group->Availability.total_release_time.get()));
  //--------------------
  InsertFullDataBG(soap,NULL,"f6_3_2_3_13","busy_time",Ulong2String(agent_per_group->Availability.total_busy_time.get()));
  //----------------
  unsigned long  total_non_acd_inc_calls = agent->Non_acd.external_incoming_calls.get() +
                                           agent->Non_acd.internal_incoming_calls.get();
  InsertFullDataBG(soap,NULL,"f6_3_2_3_26","total_non_acd_inc",Ulong2String(total_non_acd_inc_calls));

  unsigned long  total_non_acd_out_calls = agent->Non_acd.external_outgoing_calls.get() +
                                           agent->Non_acd.internal_outgoing_calls.get();
  InsertFullDataBG(soap,NULL,"f6_3_2_3_28","total_non_acd_out",Ulong2String(total_non_acd_out_calls));
  //----------------

  //----------------
  InsertFullDataBG(soap,NULL,"f6_3_2_3_5","total put on hold",Ulong2String(agent_per_group->Acd[getOffSetOfAcdCallTypes(CL_CALL_TYPE_ACD)].held_calls.get()));
  InsertFullDataBG(soap,NULL,"f6_3_2_3_15","total transferred out",Ulong2String(agent_per_group->Acd[getOffSetOfAcdCallTypes(CL_CALL_TYPE_ACD)].transferred_out_calls.get()));
  //----------------

}


//--------------------------------------------------------------------------------------------
void AgentRecordReply::InsertBriefAgentDataToRow(CsaWebServer*    soap,
                                                 // AgentRecordReply &ReplyObj,
                                                 AgentRecord*     agent,
                                                 BriefAgentInfo&  totals)
{
  int rv;
  unsigned int  *StateColor  = NULL;
  unsigned int  *TmpColorPtr = NULL;
  Ulong_t       num_in_voice_calls = 0;
  Ulong_t       num_out_voice_calls = 0;
  Ulong_t       number_of_non_acd_calls = 0;
  Ulong_t       num_chats = 0;
  Ulong_t       num_emails = 0;
  Ulong_t       currently_held_calls = 0;
  Ulong_t       acc_non_acd_time = 0;


  Ulong_t       state_delta_time = 0;
  Ulong_t       total_talk_time = 0;
  Ulong_t       answered_calls = 0;
  Ulong_t       total_wrap_up_time = 0;
  Ulong_t       total_reserved_time = 0;
  Ulong_t       chat_answered_calls = 0;
  Ulong_t       email_answered_calls = 0;
  Ulong_t       total_outbound_acd_time = 0;
  Ulong_t       oacd_answered_calls = 0;
  string        State_name;
  string        State_Time;

  csa_web_agent_reply_log_d << "AgentRecordReply::InsertBriefAgentDataToRow <AgentId:" << agent->Agent_id << ">" << LOG_TERMINATOR;

  char *s = new char[12000];
  sprintf(s,"0;f6_3_2_1_0;Agent Id;%d,",agent->Agent_id);
  sprintf(&s[strlen(s)],"0;f6_3_2_1_1;Agent Name;%s,",agent->Name);
  sprintf(&s[strlen(s)],"0;f6_3_2_1_2;Agent No;%s,",agent->Number);
  sprintf(&s[strlen(s)],"0;f6_3_2_1_3;Agent Exten.;%s,",(char *)agent->Device);

//==================================================================================
//==================================================================================
//==================================================================================
//==================================================================================
  if(totals.In_ACD_state)
  {
    soap->m_db_data_ptr->colortab.GetBGColorsByState(totals.current_state, totals.in_conference, &StateColor, State_name);
    sprintf(&s[strlen(s)],"%d;f6_3_2_2_1;State;%s,",*StateColor,State_name.c_str());
    State_Time =  Ulong2String(totals.time_in_current_state);
    rv =soap->m_db_data_ptr->limitstab.GetColorOfSTATE(totals.grp_id,
                                                       totals.current_state, //STATE
                                                       soap->m_db_data_ptr->limitstab.ersid_val.f6_3_2_2_2, // ERS_ID of State time  from DB
                                                       totals.time_in_current_state, // Value
                                                       &TmpColorPtr);  // returned color
  }
  else
  {
    soap->m_db_data_ptr->colortab.GetBGColorsByState(totals.Not_In_ACD_FirstLoginGroup_current_state,
                                                     totals.in_conference,
                                                     &StateColor,
                                                     State_name);
    sprintf(&s[strlen(s)],"%d;f6_3_2_2_1;State;%s,",*StateColor,State_name.c_str());
    State_Time = Ulong2String(totals.Not_In_ACD_FirstLoginGroup_time_in_current_state);
    rv = soap->m_db_data_ptr->limitstab.GetColorOfSTATE(totals.Not_In_ACD_FirstLoginGroup_grp_id,
                                                        totals.Not_In_ACD_FirstLoginGroup_current_state, //STATE
                                                        soap->m_db_data_ptr->limitstab.ersid_val.f6_3_2_2_2, // ERS_ID  from DB
                                                        totals.Not_In_ACD_FirstLoginGroup_time_in_current_state, // Value
                                                        &TmpColorPtr);  // returned color
  }
  if(rv)
    sprintf(&s[strlen(s)],"%d;f6_3_2_2_2;State Time;%s,",*TmpColorPtr,State_Time.c_str());
  else
    sprintf(&s[strlen(s)],"0;f6_3_2_2_2;State Time;%s,",State_Time.c_str());

  sprintf(&s[strlen(s)], "0;f6_3_2_2_11;Release Code;%s,", totals.release_code_name.c_str());

  sprintf(&s[strlen(s)],"0;f6_3_2_2_7;ACD Calls;%d,",totals.num_in_voice_calls);

  sprintf(&s[strlen(s)],"0;f6_3_2_2_10;O.ACD Calls;%d,",totals.num_out_voice_calls);

  sprintf(&s[strlen(s)],"0;f6_3_2_2_8;Chat Contacts;%d,",totals.num_chats);

  sprintf(&s[strlen(s)],"0;f6_3_2_2_9;Email Contacts;%d,",totals.num_emails);

  sprintf(&s[strlen(s)],"0;f6_3_2_3_4;ACD Calls Answered;%d,",totals.answered_calls);

  sprintf(&s[strlen(s)],"0;f6_3_2_3_44;O.ACD Calls Answered;%d,",totals.oacd_answered_calls);

  sprintf(&s[strlen(s)],"0;f6_3_2_3_36;Chat Contacts Answered;%d,",totals.chat_answered_calls);

  sprintf(&s[strlen(s)],"0;f6_3_2_3_40;Email Contacts Answered;%d,",totals.mail_answered_calls);

  sprintf(&s[strlen(s)],"0;f6_3_2_3_9;Total ACD Talk Time;%d,",totals.total_acd_talk_time);

  sprintf(&s[strlen(s)],"0;f6_3_2_2_3;Held Calls;%d,",totals.current_held_calls);
  
  sprintf(&s[strlen(s)],"0;f6_3_2_2_12;Longest On Held Time;%d,",totals.longest_held_call);

  sprintf(&s[strlen(s)],"0;f6_3_2_3_5;Total Held Calls;%d,",totals.total_held_calls);

  sprintf(&s[strlen(s)],"0;f6_3_2_3_45;Total Held Time;%d,",totals.total_held_time);

  sprintf(&s[strlen(s)],"0;f6_3_2_2_13;In Conference;%d,",totals.in_conference);
  
  sprintf(&s[strlen(s)],"0;f6_3_2_2_14;Time In Conference;%d,",totals.time_in_conference);

  sprintf(&s[strlen(s)],"0;f6_3_2_3_15;Total Txfr Out Calls;%d,",totals.total_transferred_out_calls);

  sprintf(&s[strlen(s)],"0;f6_3_2_2_6;NON-ACD Calls;%d,",totals.num_non_acd_calls);

  sprintf(&s[strlen(s)],"0;f6_3_2_3_26;Total Inc NON-ACD Calls;%d,",totals.total_non_acd_inc_calls);

  sprintf(&s[strlen(s)],"0;f6_3_2_3_28;Total Out NON-ACD Calls;%d,",totals.total_non_acd_out_calls);

  sprintf(&s[strlen(s)],"0;f6_3_2_3_11;Total NON-ACD Talk Time;%d,",totals.total_non_acd_talk_time);

  //22-Aug-2019 YR BZ#50581
  sprintf(&s[strlen(s)],"0;f6_3_2_3_12;Total Release Time;%d,",totals.total_release_time);

  if(totals.In_ACD_state)
  {
    sprintf(&s[strlen(s)],"0;f6_3_2_2_4;DNIS;%s,",totals.dnis.c_str());
    sprintf(&s[strlen(s)],"0;f6_3_2_2_5;ANI;%s,",totals.ani.c_str());
    sprintf(&s[strlen(s)],"0;f6_3_1_1_2;Current ACD Group;%s",soap->m_db_data_ptr->groupstab.GetGroupNameById(totals.grp_id).c_str());
  }
  else
  {
    sprintf(&s[strlen(s)],"0;f6_3_2_2_4;DNIS;%s,",totals.Not_In_ACD_FirstLoginGroup_dnis.c_str());
    sprintf(&s[strlen(s)],"0;f6_3_2_2_5;ANI;%s,",totals.Not_In_ACD_FirstLoginGroup_ani.c_str());
    sprintf(&s[strlen(s)],"0;f6_3_1_1_2;Current ACD Group;%s"," ");
  }

  InsertFullDataBG(soap,NULL,"f6_3_2_1_0","ALL",s);

  delete [] s;
}


//------------------------------------------------------------
void AgentRecordReply::InsertBrief_LogOut_AgentDataToRow (CsaWebServer *soap,Id_type a_id)
{
  string         State_name;
  unsigned int *StateColor = NULL;

  char *s = new char[6000];

  csa_web_agent_reply_log_d << "AgentRecordReply::InsertBrief_LogOut_AgentDataToRow <AgentId:" << a_id << ">" << LOG_TERMINATOR;

  sprintf(s,"0;f6_3_2_1_0;Agent Id;%d,",soap->m_db_data_ptr->agentstab.tab[a_id].a_id);
  sprintf(&s[strlen(s)],"0;f6_3_2_1_1;Agent Name;%s,",soap->m_db_data_ptr->agentstab.tab[a_id].a_name.c_str());
  sprintf(&s[strlen(s)],"0;f6_3_2_1_2;Agent No;%s,",soap->m_db_data_ptr->agentstab.tab[a_id].a_number.c_str());
  sprintf(&s[strlen(s)],"0;f6_3_2_1_3;Agent Exten.;,");

  soap->m_db_data_ptr->colortab.GetBGColorsByState(CSA_AGENT_LOGGED_OUT, 0, &StateColor, State_name);
  sprintf(&s[strlen(s)],"%d;f6_3_2_2_1;State;%s,",*StateColor,State_name.c_str());
  sprintf(&s[strlen(s)],"0;f6_3_2_2_2;State Time;0,");

  // INSER EMPTY DATA , only for captions
  InsertFullDataBG(soap,NULL,"f6_3_2_2_11","Release Code","");

  sprintf(&s[strlen(s)],"0;f6_3_2_2_7;ACD Calls;0,");

  sprintf(&s[strlen(s)],"0;f6_3_2_2_10;O.ACD Calls;0,");

  sprintf(&s[strlen(s)],"0;f6_3_2_2_8;Chat Contacts;0,");

  sprintf(&s[strlen(s)],"0;f6_3_2_2_9;Email Contacts;0,");

  sprintf(&s[strlen(s)],"0;f6_3_2_3_4;ACD Calls Answered;0,");

  sprintf(&s[strlen(s)],"0;f6_3_2_3_44;O.ACD Calls Answered;0,");

  sprintf(&s[strlen(s)],"0;f6_3_2_3_36;Chat Contacts Answered;0,");

  sprintf(&s[strlen(s)],"0;f6_3_2_3_40;Email Contacts Answered;0,");

  sprintf(&s[strlen(s)],"0;f6_3_2_3_9;Total ACD Talk Time;0,");

  sprintf(&s[strlen(s)],"0;f6_3_2_2_3;Held Calls;0,");
  
  sprintf(&s[strlen(s)],"0;f6_3_2_2_12;Longest On Held Time;0,");

  sprintf(&s[strlen(s)],"0;f6_3_2_3_5;Total Held Calls;0,");

  sprintf(&s[strlen(s)],"0;f6_3_2_3_45;Total Held Time;0,");

  sprintf(&s[strlen(s)],"0;f6_3_2_2_13;Conference;0,");
  
  sprintf(&s[strlen(s)],"0;f6_3_2_2_14;Time In Conference;0,");

  sprintf(&s[strlen(s)],"0;f6_3_2_3_15;Total Txfr Out Calls;0,");

  sprintf(&s[strlen(s)],"0;f6_3_2_2_6;NON-ACD Calls;0,");

  sprintf(&s[strlen(s)],"0;f6_3_2_3_26;Total Inc NON-ACD Calls;0,");

  sprintf(&s[strlen(s)],"0;f6_3_2_3_28;Total Out NON-ACD Calls;0,");

  sprintf(&s[strlen(s)],"0;f6_3_2_3_11;Total NON-ACD Talk Time;0,");

  //22-Aug-2019 YR BZ#50581
  sprintf(&s[strlen(s)],"0;f6_3_2_3_12;Total Release Time;0,");

  sprintf(&s[strlen(s)],"0;f6_3_2_2_4;DNIS;,");

  sprintf(&s[strlen(s)],"0;f6_3_2_2_5;ANI;,");

  sprintf(&s[strlen(s)],"0;f6_3_1_1_2;Current ACD Group; ");

  InsertFullDataBG(soap,NULL,"f6_3_2_1_0","ALL",s);

  delete [] s;
}

void AgentRecordReply::InsertAgentHELPDataToRow(CsaWebServer*             soap,     //PVL 15-06-2010 1004086
                                                AgentRecord*              agent,
                                                CsaAgentPerGroupRecord_t* agent_per_group)
{
  /*
  f6_3_2_1_1  "Agent","Name",0,75,l,0,l
  f6_3_2_1_2  "Agent","Number",0,75,c,0,c
  f6_3_2_1_3  "Extension"," ",0,80,c,0,c
  f6_3_2_2_2  "State Time"," ",0,75,c,0,c
  */
  string State_Time;
  unsigned long  state_delta_time=0;
  InsertFullDataBG(soap,NULL,"f6_3_2_1_0","Agent Id",Ulong2String(agent->Agent_id));
  InsertFullDataBG(soap,NULL,"f6_3_2_1_1","Agent Name",agent->Name);
  InsertFullDataBG(soap,NULL,"f6_3_2_1_2","Agent No.",(char *)agent->Number);
  InsertFullDataBG(soap,NULL,"f6_3_2_1_3","Agent Exten.",(char *)agent->Device);
  state_delta_time = NlaCalcCsaElapsedTime((unsigned long)agent_per_group->Availability.state_time.get());
  State_Time =  Ulong2String(state_delta_time);
  InsertFullDataBG(soap,NULL,"f6_3_2_2_2","State Time",State_Time);
}
