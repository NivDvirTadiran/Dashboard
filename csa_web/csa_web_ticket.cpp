#include "er_std_override.h"
#include "csa_gsoap_th_mngr.h"
#include "csa_web_ticket.hpp"
#include "csa_web_main.h"
#include "csa_web_commondefs.hpp"
#include <base64.h>

#ifdef _SM_AUTHENTICATION_


//==========================[ Static Variables ]======================
static BswLog_c   csa_web_ticket_tab_log_d(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_DETAILED);
static BswLog_c   csa_web_ticket_tab_log_i(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_INFO);
static CsaGsoapThMngr_c&  csa_gsoap_th_mngr = CsaGsoapThMngr_c::GetCsaGsoapThMngr();


//===========[ TICKET_tab::Implemented Methods ]======================

//ctor
TICKET_tab::TICKET_tab()
{
  char address[20];
  er_sprintf(address, "%p", this);
  csa_web_ticket_tab_log_i << "Create new TICKET_tab (Address " << address << ")\n" << LOG_TERMINATOR;

  tab.clear();
  app_id_map.clear();
#ifdef _SM_AUTHENTICATION_
  m_proxy_obj = NULL;
  CreateProxyObject();
#endif
}


//dtor
TICKET_tab::~TICKET_tab()
{
  char address[20];
  er_sprintf(address, "%p", this);
  csa_web_ticket_tab_log_i << "Delete TICKET_tab (Address " << address << ")\n" << LOG_TERMINATOR;

#ifdef _SM_AUTHENTICATION_
  BswServiceId_t  sid = BswMakeServiceID(FC_SM,1);
  BswDestroyProxy( sid, (BswService_c*&)m_proxy_obj );
  m_proxy_obj = NULL;
#endif
}


/*------------------------[ CreateProxyObject ]---------------------------*/
RetCode_t TICKET_tab::CreateProxyObject(void)
{
  RetCode_t rc = BSWRC_OK;

  csa_web_ticket_tab_log_i << "TICKET_tab::CreateProxyObject to SM Invoke\n" << LOG_TERMINATOR;

  BswServiceId_t  sid = BswMakeServiceID(FC_SM,1);

  if(!m_proxy_obj)
  {
    rc = BswCreateProxy( sid, NULL, 0, (BswService_c*&)m_proxy_obj );
    csa_web_ticket_tab_log_i << "CreateProxy to SM Invoke\n" << LOG_TERMINATOR;
  }

  if(rc == BSWRC_OK)
    csa_web_ticket_tab_log_i << "CreateProxy to SM - OK\n" << LOG_TERMINATOR;
  else
    csa_web_ticket_tab_log_i << "CreateProxy to SM - FAIL\n" << LOG_TERMINATOR;

  return rc ;
}


// 19-Nov-2018 YR BZ#28548
/*------------------------[ GetSupAgentExtensionSM ]---------------------------*/
bool TICKET_tab::GetSupAgentExtensionSM(Ulong_t     sup_db_agent_id,
                                        string&     sup_agent_ext,
                                        RetCode_t&  executionResult)
{
  // Get Data  from DB

  bool      dbirc = true, no_records = true;

  RetCode_t rc = BSWRC_OK;
  Ulong_t     agent_id = 0;
  string      agent_ext = "";
  //this->csa_web_TryLockInfinite(); //MT

  csa_web_ticket_tab_log_i << "TICKET_tab::GetSupAgentExtensionSM \n"
                           << "sup_db_agent_id " << sup_db_agent_id << "\n"
                           << LOG_TERMINATOR;

  if(!csa_web_TryLock())  //MT
  {
    executionResult = 700;
    return 0;
  }

  executionResult = m_proxy_obj->GetWebVisorAgentExtensionSync(sup_db_agent_id,
                                                               sup_agent_ext);

  csa_web_ticket_tab_log_i << "GetWebVisorAgentExtensionSync got:"
                           << "\nrc            " << executionResult;
  if(rc == WRONG_AGENT)
    csa_web_ticket_tab_log_i << "(AGENT NOT FOUND)";

  csa_web_ticket_tab_log_i << "\nsup_agent_ext " << sup_agent_ext.c_str()
                           << LOG_TERMINATOR;

  if(executionResult == ETAS_OK)
  {
    this->csa_web_UnLock();//MT
    return 1;
  }

  this->csa_web_UnLock();//MT
  return 0;
}


/*------------------------[ GetTicketSM ]---------------------------*/
bool TICKET_tab::GetTicketSM(unsigned long      server_ip,
                             std::string        Name,
                             std::string        Pass,
                             std::string        SessionId,
                             std::string        IpAddress,
                             std::string        AppId,
                             sup_data_t** const new_sup_data,
                             RetCode_t&         executionResult)
{
  // Get Data  from DB

  bool      dbirc = true, no_records = true;

  RetCode_t   rc;
  Ulong_t     client_Id = 0;
  Ushort_t    sup_level = 0;
  Ulong_t     user_id = 0;
  Ulong_t     agent_id = 0;
  Ushort_t    license_type =0; //PVL_STAT
  ACC_WebApps_t acc_web_apps = WEB_REALTIME;
  //this->csa_web_TryLockInfinite(); //MT

  unsigned short ipa[4];
  char temp_buf_ip[20];

  ipa[0] =(unsigned short)((server_ip >> 24)&0xFF);
  ipa[1] =(unsigned short)((server_ip >> 16)&0xFF);
  ipa[2] =(unsigned short)((server_ip >> 8)&0xFF);
  ipa[3] =(unsigned short)(server_ip &0xFF);
  er_sprintf(temp_buf_ip, "%d.%d.%d.%d",ipa[0],ipa[1],ipa[2],ipa[3]);


  //24-Mar-2019 YR BZ#49397
  //********  Dncode - Start  **********
  //string nameEecoded = Base64::Encode(Name);
#ifdef WIN32
  string nameDecoded = Name;
#else
  string nameDecoded = Base64::Decode(Name);
#endif

  //********  Dncode - End  ************

  csa_web_ticket_tab_log_i << "TICKET_tab::GetTicketSM from\n"
                           << "NAME " << Name.c_str() << ", Decoded Name " << nameDecoded.c_str() << "\n"
                           << "SessionId " << SessionId.c_str() << ", IP Address " << IpAddress.c_str() << "\n"
                           << "tab size before: " << tab.size() << LOG_TERMINATOR;

  if(!csa_web_TryLock())  //MT
  {
    executionResult = 700;
    return 0;
  }

  if (Pass.compare("userGate") == 0)  //AccGate
  {
      acc_web_apps = WEB_REALTIME_GATE;
  }

  rc = m_proxy_obj->LoginWebVisorSync(acc_web_apps,
                                      nameDecoded.c_str(),
                                      Pass.c_str(),
                                      SessionId.c_str(),
                                      IpAddress.c_str(),
                                      temp_buf_ip,
                                      executionResult,
                                      client_Id,
                                      sup_level,
                                      user_id,
                                      agent_id);

  csa_web_ticket_tab_log_i << "LoginWebVisorSync got:"
                           << "\nexecutionResult " << executionResult
                           << "\nclient_Id       " << client_Id
                           << "\nsup_level       " << sup_level
                           << "\nuser_id         " << user_id
                           << "\nagent_id        " << agent_id
                           << LOG_TERMINATOR;

  if(executionResult == ETAS_OK)
  {
    (*new_sup_data) = (sup_data_t*)new sup_data_t();
    (*new_sup_data)->client_Id = client_Id;
    (*new_sup_data)->user_id = user_id;
    (*new_sup_data)->sup_level = sup_level;
    (*new_sup_data)->agent_id = agent_id;
    (*new_sup_data)->sup_name = Name; //sup_name;
    (*new_sup_data)->application_id = AppId;
    (*new_sup_data)->license_type = license_type; //PVL_STAT   ADD to LoginWebVisorSync!!!!

    if(GetTicketFromSystem(new_sup_data))
    {
      csa_web_ticket_tab_log_i << Name.c_str() << " new sup_data_t<" << (*new_sup_data)->sup_name.c_str() << "," << (*new_sup_data)->client_Id << "> Got new ticket - " << (*new_sup_data)->ticket.c_str() << "\n" << LOG_TERMINATOR;

      //printf("****** TICKET_tab::GetTicketSM-> insert new ticket to tab attribute : %s\r\n", (*new_sup_data)->ticket.c_str()); fflush(stdout);
      FILE * fp = er_fopen ("emis_web.txt", "w+");
      fprintf(fp, "******* TICKET_tab::GetTicketSM-> insert new ticket <%s,%d> to tab attribute : %s\r\n", (*new_sup_data)->sup_name.c_str(), (*new_sup_data)->client_Id, (*new_sup_data)->ticket.c_str());fflush(fp);
      fclose(fp);
      tab[(*new_sup_data)->ticket] =  (*new_sup_data);
      this->csa_web_UnLock();//MT
      csa_web_ticket_tab_log_i << "tab size after: " << tab.size() << LOG_TERMINATOR;
      return 1;
    }
    else
    {
      FILE * fp = er_fopen ("emis_web.txt", "w+");
      fprintf(fp, "******* TICKET_tab::GetTicketSM-> delete ticket <%s,%d> \r\n", (*new_sup_data)->sup_name.c_str(), (*new_sup_data)->client_Id);fflush(fp);
      fclose(fp);
      delete  (*new_sup_data);
      (*new_sup_data) = NULL;
      this->csa_web_UnLock();//MT
      csa_web_ticket_tab_log_i << "tab size after: " << tab.size() << LOG_TERMINATOR;
      return 0;
    }
  }
  else
  {
    this->csa_web_UnLock();//MT
    return 0;
  }

  this->csa_web_UnLock();//MT
  return 0;
}


//USE SM
/*------------------------[ ReturnTicketSM ]---------------------------*/
bool TICKET_tab::ReturnTicketSM(std::string &Ticket)
{
  RetCode_t rc;

  csa_web_ticket_tab_log_i << "TICKET_tab::ReturnTicketSM for Ticket " << Ticket.c_str() 
                           << "tab size before: " << tab.size() << LOG_TERMINATOR;

  this->csa_web_TryLockInfinite();//MT

  it = tab.find(Ticket);
  if(it != tab.end())
  {
    //printf("****** TICKET_tab::ReturnTicketSM-> remove ticket from  tab attribute : %s\r\n", Ticket.c_str()); fflush(stdout);
    FILE * fp = er_fopen ("emis_web.txt", "w+");
    fprintf(fp, "******* TICKET_tab::ReturnTicketSM-> delete ticket <%s,%d> \r\n", it->second->sup_name.c_str(), it->second->client_Id);fflush(fp);
    fclose(fp);

    rc = m_proxy_obj->LogoutWebVisorSync(it->second->client_Id);
    delete it->second;
    tab.erase (it);

    this->csa_web_UnLock();//MT
    csa_web_ticket_tab_log_i << "tab size after: " << tab.size() << LOG_TERMINATOR;
    return true;
  }
  else
  {
    this->csa_web_UnLock();//MT
    return false;
  }

  this->csa_web_UnLock();//MT
  return false;
}


/*------------------------[ LogoutUsersPerApplicationIdSM ]---------------------------*/
bool TICKET_tab::LogoutUsersPerApplicationIdSM(std::string AppId)
{
  RetCode_t rc;
  std::queue<std::string> del_q;  // ticket
  std::string Ticket;

  csa_web_ticket_tab_log_i << "TICKET_tab::LogoutUsersPerApplicationIdSM for AppId " << AppId.c_str() << "\n"
                           << "tab size before: " << tab.size() << LOG_TERMINATOR;

  this->csa_web_TryLockInfinite();//MT

  //1st phase , add application id to Map if not exist
  //call seperatly
  //UpdateApplicationKeepAliveTime(AppId);

  //2nd phase , find tikets
  if(tab.size() > 0)
  {
    for(it=tab.begin(); it!= tab.end(); ++it)
    {
      if(it->second->application_id == AppId)
      {
        del_q.push(it->first);
      }
    }
  }
  //3th , delete saved tickets
  while(del_q.size() > 0)
  {
    Ticket = del_q.front();
    del_q.pop();
    it =tab.find(Ticket);
    if(it != tab.end())
    {
      rc = m_proxy_obj->LogoutWebVisorSync(it->second->client_Id); //SM
      delete it->second;
      tab.erase (it);
    }
  }

  this->csa_web_UnLock();//MT
  csa_web_ticket_tab_log_i << "tab size after: " << tab.size() << LOG_TERMINATOR;
  return true;
}
#endif


/*------------------------[ AddAplication ]-------------------------*/
bool TICKET_tab::RemoveAplication(std::string  application_id)
{
  csa_web_ticket_tab_log_i << "TICKET_tab::RemoveAplication with id " << application_id.c_str()
                           << " (tab size before: " << tab.size() << ", app_id_map size before: " << app_id_map.size() << ") - "
                           << LOG_TERMINATOR;

  if(!csa_web_TryLock())
  {
    csa_web_ticket_tab_log_i << "ERROR" << LOG_TERMINATOR;
    return false;
  }

  if(app_id_map.size() > 0)
  {
    for(it_app_id_map = app_id_map.begin(); it_app_id_map != app_id_map.end(); ++it_app_id_map)
    {
      if(it_app_id_map->first == application_id)
      {
        csa_web_ticket_tab_log_i << it_app_id_map->first.c_str() << " Found in app_id_map " << LOG_TERMINATOR;
        app_id_map.erase(it_app_id_map);
        it_app_id_map = app_id_map.begin();
        csa_web_UnLock();//MT
        csa_web_ticket_tab_log_i << " - OK (tab size after: " << tab.size() << ")" << LOG_TERMINATOR;
        return true;
      }
    }
  }

  csa_web_UnLock();//MT
  csa_web_ticket_tab_log_i << "ERROR" << LOG_TERMINATOR;
  return false;
}


/*------------------------[ GetTicketLocal ]---------------------------*/
bool TICKET_tab::GetTicketLocal(std::string Name, std::string Pass,std::string AppId,sup_data_t** const new_sup_data)
{
  // Get Data  from DB
  DBManager_c    &db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t      bswrc = BSWRC_OK,executionResult;
  Statement_c    st;
  ResultSet_c    rs;
  bool           dbirc = true, no_records = true;
  char sql_statement[MAX_SQL_QUERY_SIZE];

  this->csa_web_TryLockInfinite(); //MT

  csa_web_ticket_tab_log_i << "TICKET_tab::GetTicketLocal \n"
                           << "NAME " << Name.c_str() << "\n"
                           << "tab size before: " << tab.size() << LOG_TERMINATOR;

  //er_sprintf(sql_statement,"");
  er_sprintf(sql_statement,"select * from sup left outer join agent on sup.agent_id = agent.agent_id where sup_name like '%s' and sup_password like '%s'",
    Name.c_str(),Pass.c_str());
  st.SetQuery(sql_statement);

  bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);

  Ushort_t total_records_num = rs.GetRecordCount();
  Ushort_t left_records_num = rs.GetRecordCount();
  Ushort_t first_record_index;
  unsigned short user_id;
  unsigned short sup_level;
  unsigned short agent_id;
  char           sup_name[51];
  Ushort_t license_type =0; //PVL_STAT
  //Reply Init


  if (bswrc != BSWRC_OK || executionResult != BSWRC_OK)
  {
    //	log(LOGL_ERROR) << "CsaNetMainInit() failed to read from DB: rc = " << bswrc
    //                  << " executionResult = " << executionResult << "\n" << LOG_TERMINATOR;
  }

  //save the first record index
  first_record_index = total_records_num - left_records_num;


  //start to fill buffer with DbiRedValInfo structures
  while(rs.Next() && bswrc == BSWRC_OK)
  {
    if( !(rs.GetUshortByName("license_type",(Ushort_t &)license_type )) ||  //PVL_STAT
        !(rs.GetUshortByName("user_id",(Ushort_t &)user_id )) ||
        !(rs.GetUshortByName("sup_level",(Ushort_t &)sup_level )) ||
        !(rs.GetUshortByName("agent_id",(Ushort_t &)agent_id )) ||
        !(rs.GetStringByName("sup_name",(char *)sup_name))
        //!(rs.GetStringByName("p_nla_template_modif",(char *)&user_info_res_val.nla_allow_to_mod_template)) ||
        //!(rs.GetStringByName("p_allow_to_save_public",(char *)&user_info_res_val.allow_to_save_public))   ||
        //!(rs.GetStringByName("p_modify_sups",(char *)&user_info_res_val.modify_sups)) ||
        //!(rs.GetStringByName("p_nla",(char *)&user_info_res_val.nla)) ||
        //!(rs.GetStringByName("p_fla",(char *)&user_info_res_val.fla))   ||
        //!(rs.GetStringByName("p_caa",(char *)&user_info_res_val.caa)) ||
        //!(rs.GetStringByName("p_allow_to_modify_acd",(char *)&user_info_res_val.allow_to_modify_acd)) ||
        //!(rs.GetStringByName("p_rba",(char *)&user_info_res_val.p2)) ||
        //!(rs.GetStringByName("p_back_up_notification",(char *)&user_info_res_val.p3)) ||
        //!(rs.GetStringByName("p_5",(char *)&user_info_res_val.p5))   ||
        //!(rs.GetUshortByName("sup_hourly_int",(Ushort_t &)user_info_res_val.hourly_interval)) ||

        //!(rs.GetStringByName("p_alarm",(char *)&user_info_res_val.alarm_console)) ||
        //!(rs.GetStringByName("a_number",(char *)user_info_res_val.a_number))  ||
        //!(rs.GetStringByName("a_password",(char *)user_info_res_val.a_password))
        )
    {
      //do nothing. get next result set
      //   GetLog() << " AdiMngr_c::HandleUserInfoReply - opcode: CAC_DBI_get_user_info_req\n"
      //            << " Can't get record from result set\n"<< LOG_TERMINATOR;

      left_records_num--;
    }
    else
    {
      /*no test user_id    if(IsTicketAlreadyExist(user_id,new_sup_data))
      {
      return 1;
      }
      else
      {
      */
      (*new_sup_data) = (sup_data_t*)new sup_data_t();
      (*new_sup_data)->user_id = user_id;
      (*new_sup_data)->sup_level = sup_level;
      (*new_sup_data)->agent_id = agent_id;
      (*new_sup_data)->sup_name = sup_name;
      (*new_sup_data)->application_id = AppId;
      (*new_sup_data)->license_type = license_type; //PVL_STAT

      if(GetTicketFromSystem(new_sup_data))
      {
        csa_web_ticket_tab_log_i << Name.c_str() << " new sup_data_t<" << (*new_sup_data)->sup_name.c_str() << "," << (*new_sup_data)->client_Id << "> Got new ticket - " << (*new_sup_data)->ticket.c_str() << "\n" << LOG_TERMINATOR;
        tab[(*new_sup_data)->ticket] = (*new_sup_data);
        this->csa_web_UnLock();//MT
        csa_web_ticket_tab_log_i << "tab size after: " << tab.size() << LOG_TERMINATOR;
        return 1;
      }
      else
      {
        delete (*new_sup_data);
        (*new_sup_data) = NULL;
        this->csa_web_UnLock();//MT
        csa_web_ticket_tab_log_i << "tab size after: " << tab.size() << LOG_TERMINATOR;
        return 0;
      }
      /*no test user_id	} */

    }
  }

  this->csa_web_UnLock();//MT
  return 0;
}


/*------------------------[ ReturnTicket ]---------------------------*/
bool TICKET_tab::ReturnTicket(std::string &Ticket)
{
  csa_web_ticket_tab_log_i << "TICKET_tab::ReturnTicket for Ticket " << Ticket.c_str() 
                           << "tab size before: " << tab.size() << LOG_TERMINATOR;


  this->csa_web_TryLockInfinite();//MT

  it = tab.find(Ticket);
  if(it != tab.end())
  {
    delete it->second;
    tab.erase (it);
    this->csa_web_UnLock();//MT
    csa_web_ticket_tab_log_i << "tab size after: " << tab.size() << LOG_TERMINATOR;
    return true;
  }
  else
  {
    this->csa_web_UnLock();//MT
    return false;
  }

  this->csa_web_UnLock();//MT
  return false;
}


/*------------------------[ LogoutUsersPerApplicationId ]---------------------------*/
bool  TICKET_tab::LogoutUsersPerApplicationId(std::string AppId)
{
  std::queue<std::string> del_q;  // ticket
  std::string Ticket;

  csa_web_ticket_tab_log_d << "TICKET_tab::LogoutUsersPerApplicationId for AppId " << AppId.c_str() << LOG_TERMINATOR;

  this->csa_web_TryLockInfinite();//MT

  //1st phase , add application id to Map if not exist
  //call seperatly
  //UpdateApplicationKeepAliveTime(AppId);

  //2nd phase , find tikets
  if(tab.size() > 0)
  {
    for(it=tab.begin(); it!= tab.end(); ++it)
    {
      if(it->second->application_id == AppId)
      {
        del_q.push(it->first);
      }
    }
  }

  //3th , delete saved tickets
  while(del_q.size() > 0)
  {
    Ticket = del_q.front();
    del_q.pop();
    it =tab.find(Ticket);
    if(it != tab.end())
    {
      //printf("****** TICKET_tab::GetTicketSM-> remove ticket from  tab attribute : %s\r\n", Ticket.c_str()); fflush(stdout);
      FILE * fp = er_fopen ("emis_web.txt", "w+");
      fprintf(fp, "******* TICKET_tab::LogoutUsersPerApplicationId-> remove ticket from  tab attribute : %s\r\n", Ticket.c_str());
      fclose(fp);
      delete it->second;
      tab.erase (it);
    }
  }

  this->csa_web_UnLock();//MT
  return true;
}


/*------------------------[ VerifyTicketGetData ]---------------------------*/
bool TICKET_tab::VerifyTicketGetData(std::string Ticket, sup_data_t** const sup_data)
{
  //csa_web_ticket_tab_log_d << "TICKET_tab::VerifyTicketGetData for Ticket " << Ticket.c_str();

  this->csa_web_TryLockInfinite();
  if(tab.find(Ticket) != tab.end())
  {
    (*sup_data)= tab.find(Ticket)->second;
    this->csa_web_UnLock();//MT
    csa_web_ticket_tab_log_d << "VerifyTicketGetData for Ticket" << Ticket.c_str() << " - Found sup_data<" << (*sup_data)->sup_name.c_str() << ":" << (*sup_data)->sup_level << ">" << LOG_TERMINATOR;
    return true;
  }
  this->csa_web_UnLock();//MT

  csa_web_ticket_tab_log_i << "TICKET_tab::VerifyTicketGetData for Ticket " << Ticket.c_str() << " NOT FOUND" << LOG_TERMINATOR;
  
  //printf("******* TICKET_tab NOT found ::VerifyTicketGetData-> %s\r\n", Ticket.c_str()); fflush(stdout);
  FILE * fp = er_fopen ("emis_web.txt", "w+");
  fprintf(fp, "******* TICKET_tab NOT found ::VerifyTicketGetData-> %s\r\n", Ticket.c_str());
  fclose(fp);

  return false;
}


/*------------------------[ VerifyTicket ]---------------------------*/
bool TICKET_tab::VerifyTicket(std::string Ticket)
{
  //csa_web_ticket_tab_log_d << "TICKET_tab::VerifyTicket for Ticket " << Ticket.c_str() << LOG_TERMINATOR;

  this->csa_web_TryLockInfinite();
  if(tab.find(Ticket) != tab.end()){
    this->csa_web_UnLock();//MT
    return true;
  }
  this->csa_web_UnLock();//MT

  //printf("******* TICKET_tab NOT found ::VerifyTicket-> %s\r\n", Ticket.c_str()); fflush(stdout);
  FILE * fp = er_fopen ("emis_web.txt", "w+");
  fprintf(fp, "******* TICKET_tab NOT found ::VerifyTicket-> %s\r\n", Ticket.c_str());
  fclose(fp);

  csa_web_ticket_tab_log_i << "TICKET_tab::VerifyTicket for Ticket " << Ticket.c_str() << " NOT FOUND" << LOG_TERMINATOR;

  return false;
}


/*------------------------[ GetTicketFromSystem ]---------------------------*/
bool TICKET_tab::GetTicketFromSystem(sup_data_t** const sup_data)
{
  //16-Sep-2019 YR BZ#50700
#ifdef WIN32
  (*sup_data)->ticket = (*sup_data)->sup_name + Ulong2String((unsigned long)time(NULL));
#else
  #include <sys/time.h>
  struct timeval tv;
  gettimeofday (&tv, NULL);
  (*sup_data)->ticket = (*sup_data)->sup_name + Ulong2String(tv.tv_sec) + Ulong2String(tv.tv_usec);
#endif
  csa_web_ticket_tab_log_d << "TICKET_tab::GetTicketFromSystem return Ticket " << (*sup_data)->ticket.c_str() << LOG_TERMINATOR;

  return 1; // OK
}


/*------------------------[ IsTicketAlreadyExist ]---------------------------*/
bool TICKET_tab::IsTicketAlreadyExist(unsigned short user_id,sup_data_t** const new_sup_data)
{
  csa_web_ticket_tab_log_d << "TICKET_tab::IsTicketAlreadyExist for user_id " << user_id << LOG_TERMINATOR;

  if(tab.size() > 0)
  {
    for(it=tab.begin(); it!= tab.end(); ++it)
      if(it->second->user_id == user_id)
      {
        (*new_sup_data) = it->second;
        return true;
      }
  }
  return false;
}


/*------------------------[ UpdateApplicationKeepAliveTime ]---------------------------*/
bool TICKET_tab::UpdateApplicationKeepAliveTime(std::string AppId)
{
  csa_web_ticket_tab_log_d << "TICKET_tab::UpdateApplicationKeepAliveTime for AppId " << AppId.c_str() << LOG_TERMINATOR;

  it_app_id_map = app_id_map.find(AppId);
  if(it_app_id_map == app_id_map.end())
  {
    app_id_map[AppId] = time( NULL); // create new , time now
  }
  else
  {
    it_app_id_map->second = time( NULL); //use old ,  time now
    csa_gsoap_th_mngr.ZeroCheckAliveCounter(AppId);
  }
  return true;
}


/*------------------------[ IsApplicationKeepAliveTooOld ]---------------------------*/
int TICKET_tab::IsApplicationKeepAliveTooOld(std::string AppId) //PVL_STAT
{
  csa_web_ticket_tab_log_i << "TICKET_tab::IsApplicationKeepAliveTooOld for AppId " << AppId.c_str();

  time_t dif;
  it_app_id_map = app_id_map.find(AppId);
  if(it_app_id_map == app_id_map.end())
  {
    csa_web_ticket_tab_log_i << " - NOTHING_FOUND" << LOG_TERMINATOR;
    return NOTHING_FOUND;  // PVL_STAT
  }
  else
  {
    dif = NlaCalcCsaElapsedTime (it_app_id_map->second);
    if(  dif > APP_ID_MAX_NO_KA_TIME_MS)
    {
      er_fprintf(stderr,"last app registration %ld s. ago - KeepAlive too old\n",dif);
      csa_web_ticket_tab_log_i << " - KA_TOO_OLD" << LOG_TERMINATOR;
      return KA_TOO_OLD; // PVL_STAT
    }
    else
    {
      csa_web_ticket_tab_log_i << " - KA_NORMAL" << LOG_TERMINATOR;
      return KA_NORMAL;	 // PVL_STAT
    }
  }
}


/*------------------------[ IsApplicationRegister ]------------------------------*/
bool TICKET_tab::IsApplicationRegister(std::string AppId)
{
  return csa_gsoap_th_mngr.isApplicationRegister(AppId);
}

