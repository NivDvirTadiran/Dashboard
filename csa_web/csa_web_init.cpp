#include "er_std_override.h"
#include "csa_web_main.h"

#include "asutil.hpp"
#include "csa_web_init.h"


  //const static  int  norm_val = 1;
  //const static  int red_val = 2;
  //const static  int yel_val = 3;
  const static  int agn_help_state = 1;
  const static  int agn_idle_state = 4;
  const static  int agn_busy_state = 5;
  const static  int agn_rls_state = 6;
  const static  int agn_ring_state = 7;
  const static  int agn_wrap_state = 8;
  const static  int agn_acd_state = 9;
  const static  int agn_out_r_state = 10;
  const static  int agn_inc_r_state = 11;
  const static  int agn_inc_state = 12;
  const static  int agn_out_state = 13;
  const static  int agn_busy_r_state = 14;
  const static  int agn_log_out_state = 15;
  const static  int agn_frc_busy_state = 20;
  const static  int agn_o_rsrvd_state = 32;
  const static  int agn_o_acd_state = 30;
  const static  int agn_o_ring_state = 31;
  const static  int trunk_acd_state = 16;
  const static  int trunk_idle_state = 17;
  const static  int trunk_busy_state = 18;
  const static  int trunk_que_state = 19;
  const static  int ivr_port_idle_state  = 33;
  const static  int ivr_port_acd_state   = 34;
  const static  int ivr_port_o_acd_state = 35;
  const static  int ivr_port_error_state = 36;
  const static  int agn_email_state      = 37;
  const static  int agn_out_email_state  = 38;
  const static  int agn_inc_email_state  = 39;
  const static  int agn_busy_email_state = 40;
 // {NS 2.12.97 add new agent state} {22.3.98 index has been changed to support info from DBI}
  const static  int agn_dnd_state = 26;
  const static  int agn_frc_wrap_state = 21;
  const static  int agn_exter_alert_state = 25;
  const static  int agn_inter_alert_state = 24;
  const static  int agn_inter_inc_state = 22;
  const static  int agn_inter_out_state = 28;
  const static  int agn_exter_inc_state = 23;
  const static  int agn_exter_out_state = 29;
  const static  int agn_split_state = 27;

  //Add new states for Conference - Not in DB
  const static  int agn_acd_conf_state    = 41;
  const static  int agn_inc_conf_state    = 42;
  const static  int agn_inc_r_conf_state  = 43;
  const static  int agn_out_conf_state    = 44;
  const static  int agn_out_r_conf_state  = 45;
  const static  int agn_busy_conf_state   = 46;
  const static  int agn_busy_r_conf_state = 47;


unsigned int BGRtoRGBcolor(unsigned long BGRcolor){
return (BGRcolor & 0x000000ff) << 16 | (BGRcolor & 0x0000FF00) | (BGRcolor & 0x00FF0000) >> 16;
}


State_colors_tab::State_colors_tab()
{
  isAvailable = false;
  LoadColors();
}


CsaStaAgentState_t State_colors_tab::Get_CSA_state_by_ColorId(unsigned long color_id,string &state_name)
{
 // //const static  int  norm_val = 1;
 // //const static  int red_val = 2;
 // //const static  int yel_val = 3;
 // const static  int agn_help_state = 1;
 // const static  int agn_idle_state = 4;
 // const static  int agn_busy_state = 5;
 // const static  int agn_rls_state = 6;
 // const static  int agn_ring_state = 7;
 // const static  int agn_wrap_state = 8;
 // const static  int agn_acd_state = 9;
 // const static  int agn_out_r_state = 10;
 // const static  int agn_inc_r_state = 11;
 // const static  int agn_inc_state = 12;
 // const static  int agn_out_state = 13;
 // const static  int agn_busy_r_state = 14;
 // const static  int agn_log_out_state = 15;
 // const static  int agn_frc_busy_state = 20;
 // const static  int agn_o_rsrvd_state = 32;
 // const static  int agn_o_acd_state = 30;
 // const static  int agn_o_ring_state = 31;
 // const static  int trunk_acd_state = 16;
 // const static  int trunk_idle_state = 17;
 // const static  int trunk_busy_state = 18;
 // const static  int trunk_que_state = 19;
 // const static  int ivr_port_idle_state  = 33;
 // const static  int ivr_port_acd_state   = 34;
 // const static  int ivr_port_o_acd_state = 35;
 // const static  int ivr_port_error_state = 36;
 // const static  int agn_email_state      = 37;
 // const static  int agn_out_email_state  = 38;
 // const static  int agn_inc_email_state  = 39;
 // const static  int agn_busy_email_state = 40;
 //// {NS 2.12.97 add new agent state} {22.3.98 index has been changed to support info from DBI}
 // const static  int agn_dnd_state = 26;
 // const static  int agn_frc_wrap_state = 21;
 // const static  int agn_exter_alert_state = 25;
 // const static  int agn_inter_alert_state = 24;
 // const static  int agn_inter_inc_state = 22;
 // const static  int agn_inter_out_state = 28;
 // const static  int agn_exter_inc_state = 23;
 // const static  int agn_exter_out_state = 29;
 // const static  int agn_split_state = 27;

 // //Add new states for Conference - Not in DB
 // const static  int agn_acd_conf_state    = 41;
 // const static  int agn_inc_conf_state    = 42;
 // const static  int agn_inc_r_conf_state  = 43;
 // const static  int agn_out_conf_state    = 44;
 // const static  int agn_out_r_conf_state  = 45;
 // const static  int agn_busy_conf_state   = 46;
 // const static  int agn_busy_r_conf_state = 47;

  switch (color_id)
  { 
    case agn_log_out_state : state_name = "Logout";     return CSA_AGENT_LOGGED_OUT; 
    case agn_idle_state: state_name = "Idle";           return CSA_AGENT_IDLE;
    case agn_ring_state: state_name = "Ring";           return CSA_AGENT_ALERT;
    case agn_acd_state: state_name = "ACD";             return CSA_AGENT_ACD_CALL;
    case agn_wrap_state: state_name = "Wrap";           return CSA_AGENT_WRAP_UP;
    case agn_inc_state: state_name = "Inc";             return CSA_AGENT_INCOMING;
    case agn_out_state: state_name = "Out";             return CSA_AGENT_OUTGOING;
    case agn_rls_state: state_name = "Release";         return CSA_AGENT_RELEASE;
    case agn_out_r_state: state_name = "Out+Rls";       return CSA_AGENT_OUT_RLS;
    case agn_inc_r_state: state_name = "Inc+Rls";       return CSA_AGENT_IN_RLS;
    case agn_busy_r_state: state_name = "Busy+Rls";     return CSA_AGENT_BUSY_RLS;
    case agn_busy_state: state_name = "Busy";           return CSA_AGENT_BUSY;
    case agn_frc_busy_state: state_name = "F.Busy";     return CSA_AGENT_FORCED_BUSY;
    case agn_dnd_state: state_name = "DND";             return CSA_AGENT_DND;
    case agn_frc_wrap_state: state_name = "F.Wrap";     return CSA_AGENT_FORCED_WRAP;
    case agn_exter_alert_state: state_name = "E.Ring";  return CSA_AGENT_EXTER_ALERT;
    case agn_inter_alert_state: state_name = "I.Ring";  return CSA_AGENT_INTER_ALERT;
    case agn_inter_inc_state: state_name = "I.Inc";     return CSA_AGENT_INTER_INC;
    case agn_inter_out_state: state_name = "I.Out";     return CSA_AGENT_INTER_OUT;
    case agn_exter_inc_state: state_name = "E.Inc";     return CSA_AGENT_EXTER_INC;
    case agn_exter_out_state: state_name = "E.Out";     return CSA_AGENT_EXTER_OUT;
    case agn_split_state: state_name = "Split";         return CSA_AGENT_SPLIT;
    case 0: state_name = "Help";                        return CSA_AGENT_HELP;
 //   case 0: state_name = "Qual";  return  CSA_AGENT_QUALIFY;
    case agn_o_rsrvd_state: state_name = "O.Reserved";  return CSA_AGENT_O_RSVD;
    case agn_o_ring_state: state_name = "O.Ring";       return CSA_AGENT_O_RING;
    case agn_o_acd_state: state_name = "O.ACD";         return CSA_AGENT_O_ACD;
    case agn_email_state: state_name = "EMAIL";         return CSA_AGENT_EMAIL;
    case agn_out_email_state: state_name = "EMAIL+Out"; return CSA_AGENT_OUT_EMAIL;
    case agn_inc_email_state: state_name = "EMAIL+Inc"; return CSA_AGENT_INC_EMAIL;
    case agn_busy_email_state: state_name = "EMAIL+Busy"; return  CSA_AGENT_BUSY_EMAIL;

    case agn_acd_conf_state: state_name     = "ACD+Conf";      return CSA_AGENT_ACD_CALL_CONF;
    case agn_inc_conf_state: state_name     = "Inc+Conf";      return CSA_AGENT_INCOMING_CONF;
    case agn_inc_r_conf_state: state_name   = "Inc+Rls+Conf";  return CSA_AGENT_INC_RLS_CONF;
    case agn_out_conf_state: state_name     = "Out+Conf";      return CSA_AGENT_OUTGOING_CONF;
    case agn_out_r_conf_state: state_name   = "Out+Rls+Conf";  return CSA_AGENT_OUT_RLS_CONF;
    case agn_busy_conf_state: state_name    = "Busy+Conf";     return CSA_AGENT_BUSY_CONF;
    case agn_busy_r_conf_state: state_name  = "Busy+Rls+Conf"; return CSA_AGENT_BUSY_RLS_CONF;
  }

  return CSA_LAST_STATE;
}




bool State_colors_tab::LoadColors(void)
{
  DBManager_c&  db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t     bswrc = BSWRC_OK,executionResult;
  Statement_c   st;
  ResultSet_c   rs;
  bool          dbirc = true, no_records = true;
  char          sql_statement[MAX_SQL_QUERY_SIZE];

  Ushort_t total_records_num ; 
  Ushort_t left_records_num; 
  local_record_index = 0;

  Ulong_t  bg_BGR_color;
  Ulong_t  fg_BGR_color;
  Ulong_t  color_id;
  Ulong_t  csa_state;
  string   tmp_state_name;

  er_sprintf(sql_statement,"select color_id, background_color, foreground_color from nla_colors");
  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);

  total_records_num = rs.GetRecordCount(); 
  left_records_num = rs.GetRecordCount(); 
  if(total_records_num==0 || bswrc!= BSWRC_OK )
    return 1;

  while(rs.Next() && bswrc == BSWRC_OK)
  { 
    if (!(rs.GetUlongByName("color_id",color_id))   ||
        !(rs.GetUlongByName("background_color",bg_BGR_color))||
        !(rs.GetUlongByName("foreground_color",fg_BGR_color)))
    {
      //do nothing. get next result set
      left_records_num--;
    }
    else
    {
      //process color.info  
      csa_state = (unsigned long)Get_CSA_state_by_ColorId(color_id,tmp_state_name) ;
      tab[csa_state].state = (CsaStaAgentState_t)csa_state;
      tab[csa_state].state_name = tmp_state_name;  
      tab[csa_state].bg_color = BGRtoRGBcolor(bg_BGR_color);
      tab[csa_state].fg_color = BGRtoRGBcolor(fg_BGR_color);
      //isAvailable = true;
      local_record_index++;
      left_records_num--; 
      if (left_records_num == 0)
        break;
    }
  }

  csa_state = (unsigned long)Get_CSA_state_by_ColorId(agn_acd_conf_state,tmp_state_name) ;
  tab[csa_state].state = (CsaStaAgentState_t)csa_state;
  tab[csa_state].state_name = tmp_state_name;  
  tab[csa_state].bg_color = tab[CSA_AGENT_ACD_CALL].bg_color;
  tab[csa_state].fg_color = tab[CSA_AGENT_ACD_CALL].fg_color;

  csa_state = (unsigned long)Get_CSA_state_by_ColorId(agn_inc_conf_state,tmp_state_name) ;
  tab[csa_state].state = (CsaStaAgentState_t)csa_state;
  tab[csa_state].state_name = tmp_state_name;  
  tab[csa_state].bg_color = tab[CSA_AGENT_INCOMING].bg_color;
  tab[csa_state].fg_color = tab[CSA_AGENT_INCOMING].fg_color;

  csa_state = (unsigned long)Get_CSA_state_by_ColorId(agn_inc_r_conf_state,tmp_state_name) ;
  tab[csa_state].state = (CsaStaAgentState_t)csa_state;
  tab[csa_state].state_name = tmp_state_name;  
  tab[csa_state].bg_color = tab[CSA_AGENT_IN_RLS].bg_color;
  tab[csa_state].fg_color = tab[CSA_AGENT_IN_RLS].fg_color;

  csa_state = (unsigned long)Get_CSA_state_by_ColorId(agn_out_conf_state,tmp_state_name) ;
  tab[csa_state].state = (CsaStaAgentState_t)csa_state;
  tab[csa_state].state_name = tmp_state_name;  
  tab[csa_state].bg_color = tab[CSA_AGENT_OUTGOING].bg_color;
  tab[csa_state].fg_color = tab[CSA_AGENT_OUTGOING].fg_color;

  csa_state = (unsigned long)Get_CSA_state_by_ColorId(agn_out_r_conf_state,tmp_state_name) ;
  tab[csa_state].state = (CsaStaAgentState_t)csa_state;
  tab[csa_state].state_name = tmp_state_name;  
  tab[csa_state].bg_color = tab[CSA_AGENT_OUT_RLS].bg_color;
  tab[csa_state].fg_color = tab[CSA_AGENT_OUT_RLS].fg_color;

  csa_state = (unsigned long)Get_CSA_state_by_ColorId(agn_busy_conf_state,tmp_state_name) ;
  tab[csa_state].state = (CsaStaAgentState_t)csa_state;
  tab[csa_state].state_name = tmp_state_name;  
  tab[csa_state].bg_color = tab[CSA_AGENT_BUSY].bg_color;
  tab[csa_state].fg_color = tab[CSA_AGENT_BUSY].fg_color;

  csa_state = (unsigned long)Get_CSA_state_by_ColorId(agn_busy_r_conf_state,tmp_state_name) ;
  tab[csa_state].state = (CsaStaAgentState_t)csa_state;
  tab[csa_state].state_name = tmp_state_name;  
  tab[csa_state].bg_color = tab[CSA_AGENT_BUSY_RLS].bg_color;
  tab[csa_state].fg_color = tab[CSA_AGENT_BUSY_RLS].fg_color;

  isAvailable = true;

  return 0;
}


int State_colors_tab::GetBGColorsByState(int     state,     //IN
                                         int     in_conf,
                                         Ulong_t **color_ptr, //OUT
                                         string& state_name)//OUT
{
  if(in_conf > 0)
  {
    if(state == CSA_AGENT_ACD_CALL)
      state = CSA_AGENT_ACD_CALL_CONF;
    else if(state == CSA_AGENT_INCOMING)
      state = CSA_AGENT_INCOMING_CONF;
    else if(state == CSA_AGENT_IN_RLS)
      state = CSA_AGENT_INC_RLS_CONF;
    else if(state == CSA_AGENT_OUTGOING)
      state = CSA_AGENT_OUTGOING_CONF;
    else if(state == CSA_AGENT_OUT_RLS)
      state = CSA_AGENT_OUT_RLS_CONF;
    else if(state == CSA_AGENT_BUSY)
      state = CSA_AGENT_BUSY_CONF;
    else if(state == CSA_AGENT_BUSY_RLS)
      state = CSA_AGENT_BUSY_RLS_CONF;
  }

  *color_ptr = &(tab[state].bg_color); 
  state_name = tab[state].state_name;
  return 0;
}
