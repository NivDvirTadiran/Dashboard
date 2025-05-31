#include "er_std_override.h"
#include "csa_web_brief_agent_inf.hpp"
#include "csa_web_commondefs.hpp"


static BswLog_c csa_web_brief_agent_inf_log_d(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_DETAILED);

//------------------------------------------------------------------------------------------------
//------------------BriefAgentInfo---------------------------------------------------------------------
//-----------------------------
bool BriefAgentInfo::IsAcdState( CsaStaAgentState_t state ) 
{
  return  (state == CSA_AGENT_ALERT)|| (state == CSA_AGENT_ACD_CALL) ||
          (state == CSA_AGENT_WRAP_UP) || (state == CSA_AGENT_FORCED_WRAP)||
          (state == CSA_AGENT_O_ACD) || (state == CSA_AGENT_O_RSVD) ||
           (state == CSA_AGENT_O_RING);
}
//-----------------------------
void  BriefAgentInfo::SetAcdData(AgentRecord* agent, CsaAgentPerGroupRecord_t*  agent_per_group)
{
  int acd ;
  int chat_acd;
  int email_acd;
  int cb_acd;
  int abnd_acd;
  int dl_acd;

  unsigned long delta_login_time;
  //check if the agent is in the ACD state
  acd = getOffSetOfAcdCallTypes(CL_CALL_TYPE_ACD);

  //30-Jan-2022 YR BZ#55901
  Ulong_t current_held_calls_tmp = 0;
  Ulong_t longest_held_call_tmp = 0;
  current_held_calls_tmp = agent_per_group->Availability.currently_held_calls.get();

  if(in_conference == 0)
    in_conference = agent_per_group->Availability.in_conference.get();

  csa_web_brief_agent_inf_log_d << "BriefAgentInfo::SetAcdData <AgentId:" << agent->Agent_id << ",Group" << agent_per_group->Group_id << ">"
                                << "\ncurrent_held_calls_tmp: " << current_held_calls_tmp
                                << "\nin_conference:          " << in_conference
                                << LOG_TERMINATOR;

  if ( IsAcdState((CsaStaAgentState_t)agent_per_group->Availability.state) )
  {
    dnis          = (const char *)agent_per_group->Availability.dnis;
    ani           = (const char *)agent_per_group->Availability.ani;
    grp_id        = agent_per_group->Group_id;
    current_state = agent_per_group->Availability.state;
    //PVL 23-06-2010 1002576 time_in_current_state = NlaCalcCsaElapsedTime((unsigned long)agent_per_group->Availability.state_time.get());
    time_in_current_state = NlaCalcCsaElapsedTime((unsigned long)agent_per_group->Availability.running_state_time.get());//PVL 23-06-2010 1002576 

    //start PVL 23-06-2010 1002576
    if(current_state == CL_CALL_TYPE_ACD)  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   
    {
      if(time_in_current_state < longest_time_in_current_state) 
        time_in_current_state= longest_time_in_current_state;
      else
        longest_time_in_current_state = time_in_current_state;
    }
    //end PVL 23-06-2010 1002576
    In_ACD_state = true;
  }
  else  // 
  {
    delta_login_time = (unsigned long)NlaCalcCsaElapsedTime(agent_per_group->Availability.login_time.get());
    if( delta_login_time >= Not_In_ACD_FirstLoginGroup_TotalLoginTime)
    {
      Not_In_ACD_FirstLoginGroup_TotalLoginTime = delta_login_time;
      Not_In_ACD_FirstLoginGroup_dnis = (const char *)agent_per_group->Availability.dnis;
      Not_In_ACD_FirstLoginGroup_ani= (const char *)agent_per_group->Availability.ani;
      Not_In_ACD_FirstLoginGroup_grp_id = agent_per_group->Group_id;
      Not_In_ACD_FirstLoginGroup_current_state= agent_per_group->Availability.state;
      //PVL 23-06-2010 1002576 Not_In_ACD_FirstLoginGroup_time_in_current_state = NlaCalcCsaElapsedTime((unsigned long)agent_per_group->Availability.state_time.get());
      Not_In_ACD_FirstLoginGroup_time_in_current_state = NlaCalcCsaElapsedTime((unsigned long)agent_per_group->Availability.running_state_time.get());//PVL 23-06-2010 1002576 MAIN SOLUTION OF BUG
    }
    else
    {
      cout << "DELTA= " << delta_login_time <<"\n"; //DEBUG ONLY 
    }
  }

  answered_calls = agent_per_group->Acd[acd].answered_calls.get() +
                   answered_calls;

  total_acd_talk_time += agent_per_group->Acd[acd].total_talk_time.get();

  if(current_held_calls_tmp > 0)
    longest_held_call_tmp = NlaCalcCsaElapsedTime((unsigned long)agent_per_group->Availability.hold_time.get());

  if(current_held_calls_tmp > current_held_calls)
    current_held_calls = current_held_calls_tmp;

  if(longest_held_call_tmp > longest_held_call)
    longest_held_call = longest_held_call_tmp;

  total_held_calls += agent_per_group->Acd[acd].held_calls.get();
  total_held_time += agent_per_group->Acd[acd].total_hold_time.get();

  if((in_conference > 0) && (time_in_conference == 0))
  {
    time_in_conference = NlaCalcCsaElapsedTime((unsigned long)agent_per_group->Availability.time_in_conference.get());
  }

  total_transferred_out_calls += agent_per_group->Acd[acd].transferred_out_calls.get();

  //
  total_non_acd_talk_time = agent->Non_acd.total_talk_of_ext_inc_calls.get() +
                            agent->Non_acd.total_talk_of_int_inc_calls.get() +
                            agent->Non_acd.total_time_of_int_out_calls.get() +
                            agent->Non_acd.total_time_of_ext_out_calls.get();

  //
  total_non_acd_inc_calls = agent->Non_acd.external_incoming_calls.get() +
                            agent->Non_acd.internal_incoming_calls.get();

  total_non_acd_out_calls = agent->Non_acd.external_outgoing_calls.get() +
                            agent->Non_acd.internal_outgoing_calls.get();

  //total_inc_non_acd_calls += agent_per_group->Non_acd..

  if (Csa_Setup.supportChat())
  {
    chat_acd = getOffSetOfAcdCallTypes(CL_CALL_TYPE_CHAT);
    num_chats = agent_per_group->Availability.number_of_current_calls[chat_acd].get() + //PVL 3-06-2010 1004114
                num_chats;
    chat_answered_calls = agent_per_group->Acd[chat_acd].answered_calls.get() +
                          chat_answered_calls;
  }
  if (Csa_Setup.supportEmail())
  {
    email_acd = getOffSetOfAcdCallTypes(CL_CALL_TYPE_EMAIL);
    num_emails = agent_per_group->Availability.number_of_current_calls[email_acd].get() +
                 num_emails;
    mail_answered_calls = agent_per_group->Acd[email_acd].answered_calls.get()+
                          mail_answered_calls;
  }
  if (Csa_Setup.supportOutbound())
  {
    cb_acd = getOffSetOfAcdCallTypes(CL_CALL_TYPE_CALLBACK);
    abnd_acd = getOffSetOfAcdCallTypes(CL_CALL_TYPE_ABANDONED);
    oacd_answered_calls = agent_per_group->Acd[cb_acd].answered_calls.get() + 
                          agent_per_group->Acd[abnd_acd].answered_calls.get()+
                          oacd_answered_calls;
    num_out_voice_calls = (unsigned long)agent_per_group->Availability.number_of_current_calls[cb_acd].get() +
                          (unsigned long)agent_per_group->Availability.number_of_current_calls[abnd_acd].get() +
                          num_out_voice_calls;
  }
  if (Csa_Setup.supportDialList())
  {
    dl_acd = getOffSetOfAcdCallTypes(CL_CALL_TYPE_DIAL_LIST);
    oacd_answered_calls = agent_per_group->Acd[dl_acd].answered_calls.get()+
                          oacd_answered_calls;
    num_out_voice_calls = (unsigned long)agent_per_group->Availability.number_of_current_calls[dl_acd].get()+
                          num_out_voice_calls;
  }

  total_wrap_up_time = agent_per_group->Acd[acd].total_wrap_up_time.get() +
                       total_wrap_up_time;


  num_in_voice_calls = agent_per_group->Availability.number_of_current_calls[acd].get() +
                       num_in_voice_calls;




  release_code_name = (char*)agent_per_group->Availability.release_code_name;
  //22-Aug-2019 YR BZ#50581
  total_release_time = agent_per_group->Availability.total_release_time.get();

  //NON_ACD
  //29-Oct-2019 YR BZ#51091
  num_non_acd_calls = agent_per_group->Availability.number_of_non_acd_calls.get();
}

//--------------------------------------------------------
void BriefAgentInfo::InitCounters(void){
  state_delta_time        = 0;
  total_acd_talk_time     = 0;
  total_non_acd_talk_time = 0;
  total_non_acd_inc_calls = 0;
  total_non_acd_out_calls = 0;
  answered_calls          = 0;
  total_wrap_up_time      = 0;
  total_reserved_time     = 0;
  total_release_time      = 0;
  chat_answered_calls     = 0;
  mail_answered_calls     = 0;
  total_outbound_acd_time = 0;
  oacd_answered_calls     = 0;
  dnis.clear();//28-06-2010 crash
  ani.clear();//28-06-2010 crash
  grp_id                      = 0;
  current_state               = 0;
  time_in_current_state       = 0;
  num_in_voice_calls          = 0;
  num_emails                  = 0;
  num_out_voice_calls         = 0;
  num_chats                   = 0;
  current_held_calls          = 0;
  longest_held_call           = 0;
  total_held_calls            = 0;
  total_held_time             = 0;
  total_transferred_out_calls = 0;
  in_conference               = 0;
  time_in_conference          = 0;
  release_code_name.clear();//28-06-2010 crash
  num_non_acd_calls           = 0;
  In_ACD_state                = false;
  Not_In_ACD_FirstLoginGroup_TotalLoginTime = 0;
  Not_In_ACD_FirstLoginGroup_dnis.clear();//28-06-2010 crash
  Not_In_ACD_FirstLoginGroup_ani.clear();//28-06-2010 crash
  Not_In_ACD_FirstLoginGroup_grp_id = 0;
  Not_In_ACD_FirstLoginGroup_current_state = 0;
  Not_In_ACD_FirstLoginGroup_time_in_current_state = 0;
  longest_time_in_current_state = 0;//PVL 23-06-2010 1002576
}
