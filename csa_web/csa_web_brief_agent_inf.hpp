#ifndef CSA_WEB_BRIEF_AGENT_REPLY_H
#define CSA_WEB_BRIEF_AGENT_REPLY_H

#include "aragent.hpp"
#include "asutil.hpp"
class BriefAgentInfo 
{ 

public:
  BriefAgentInfo(){}; //PVL 28-06-2010 CRASH
  ~BriefAgentInfo(){};
  bool IsAcdState( CsaStaAgentState_t state ) ;
  void SetAcdData(AgentRecord* agent, CsaAgentPerGroupRecord_t*  agent_per_group);

  Ulong_t  state_delta_time;
  Ulong_t  total_acd_talk_time;
  Ulong_t  total_non_acd_talk_time;
  Ulong_t  total_non_acd_inc_calls;
  Ulong_t  total_non_acd_out_calls;
  Ulong_t  answered_calls;
  Ulong_t  total_wrap_up_time;
  Ulong_t  total_reserved_time;
  //22-Aug-2019 YR BZ#50581
  Ulong_t  total_release_time;
  Ulong_t  chat_answered_calls;
  Ulong_t  mail_answered_calls;
  Ulong_t  total_outbound_acd_time;
  Ulong_t  oacd_answered_calls;
 // totals_group.number_oacd_handled_by_agent := totals_group.number_oacd_handled_by_agent+
//                                               number_oacd_handled_by_agent;
//number_oacd_handled_by_agent :=  stat.csa_online_agent_acd_per_group_login_statistics.oacd_answered_calls;
  string    dnis;
  string    ani;
  Ulong_t   grp_id;
  Ulong_t   current_state;
  Ulong_t   time_in_current_state;
  Ulong_t   longest_time_in_current_state;//PVL 23-06-2010 1002576
  Ulong_t   num_in_voice_calls;
  Ulong_t   num_emails;
  Ulong_t   num_out_voice_calls;
  Ulong_t   num_chats;
  Ulong_t   current_held_calls;
  Ulong_t   longest_held_call;
  Ulong_t   total_held_calls;
  Ulong_t   total_held_time;
  Ulong_t   total_transferred_out_calls;
  string    release_code_name;
  //Conference
  Ulong_t   in_conference;
  Ulong_t   time_in_conference;
  //NON_ACD 
  Ulong_t   num_non_acd_calls;
  bool      In_ACD_state;
  //Not in ACD CALL  , In_ACD_state =false
  Ulong_t   Not_In_ACD_FirstLoginGroup_TotalLoginTime;
  string    Not_In_ACD_FirstLoginGroup_dnis;
  string    Not_In_ACD_FirstLoginGroup_ani;
  Ulong_t   Not_In_ACD_FirstLoginGroup_grp_id;
  Ulong_t   Not_In_ACD_FirstLoginGroup_current_state;
  Ulong_t   Not_In_ACD_FirstLoginGroup_time_in_current_state;


  void InitCounters(void);

};
#endif
