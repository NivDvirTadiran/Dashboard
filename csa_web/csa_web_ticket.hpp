#ifndef GSOAP_WEB_TICKET_TAB_H
#define GSOAP_WEB_TICKET_TAB_H

#include "er_std_override.h"
#include <map>
#include <queue>
#include "sm_serviceiface.h"

#include "csa_web_main.h"
#include "csa_web_lock.hpp"

#define APP_ID_MAX_NO_KA_TIME_MS 60
#define _SM_AUTHENTICATION_
//PVL , enable only if modified SM available  #define _SM_AUTHENTICATION_
#ifdef _SM_AUTHENTICATION_
#include "sm_facilities.h"
#endif



typedef  struct SUP_DATA_TYPE
{
  std::string     ticket;
  std::string     sup_name;
  std::string     application_id; //application id
  unsigned short  sup_level;		
  unsigned short  user_id;
  unsigned short  agent_id;
  unsigned short  license_type; //PVL_STAT
  Ulong_t         client_Id;  //for interaction with SM
}sup_data_t;	


class TICKET_tab  : public CSA_WEB_LOCK_obj{
//============================ Public Section ========================
public:

  TICKET_tab();
  ~TICKET_tab();

  enum {
    NOTHING_FOUND = 0,  // PVL_STAT
    KA_TOO_OLD = 1,// PVL_STAT
    KA_NORMAL =2  // PVL_STAT 
  } ;


#ifdef _SM_AUTHENTICATION_
  bool GetTicketSM(unsigned long server_ip,std::string Name, std::string Pass, std::string SessionId, std::string IpAddress,std::string AppId,sup_data_t** const new_sup_data,RetCode_t &executionResult);
  bool ReturnTicketSM(std::string &Ticket);
  bool LogoutUsersPerApplicationIdSM(std::string AppId);
  bool RemoveAplication(std::string AppId);
  // 19-Nov-2018 YR BZ#28548
  bool GetSupAgentExtensionSM(Ulong_t sup_db_agent_id, string& sup_agent_ext, RetCode_t &executionResult);
#endif

  bool GetTicketLocal(std::string Name, std::string Pass,std::string AppId,sup_data_t** const old_sup_data);   
  bool ReturnTicket(std::string &Ticket);
  bool LogoutUsersPerApplicationId(std::string AppId);

  bool VerifyTicketGetData(std::string Ticket, sup_data_t** const sup_data);
  bool VerifyTicket(std::string Ticket);

  bool UpdateApplicationKeepAliveTime(std::string AppId);
  bool IsApplicationRegister(std::string AppId);
  int  IsApplicationKeepAliveTooOld(std::string AppId);

private:
  bool IsTicketAlreadyExist(unsigned short user_id,sup_data_t** const old_sup_data);
  bool GetTicketFromSystem(sup_data_t** const sup_data);
  std::map<std::string,sup_data_t*> tab;  // ticket, DATA
  std::map<std::string,sup_data_t*>::iterator it;

  std::map<std::string, time_t>  app_id_map; // Application_id
  std::map<std::string, time_t>::iterator it_app_id_map; // Application_id

  RetCode_t CreateProxyObject(void);

#ifdef _SM_AUTHENTICATION_
  IsmService_c*          m_proxy_obj;
#endif

};

#endif
	
    
