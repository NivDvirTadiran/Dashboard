#include "er_std_override.h"


#include <bswlog.h>

#include "csa_web_serverMT.hpp"
#include "csa_web_commondefs.hpp"
#include "csa_web_group_reply.hpp"
#include "csa_web_agent_reply.hpp"
#include "csa_web_report.h"
#include "csa_web_sg_reply.h"
#include "csa_web_dnis_reply.hpp"
#include "csa_web_ivr_reply.hpp"
#include "csa_web_mailbox_reply.hpp"
#include "csa_web_ticket.hpp"
#include "csa_web_sg_hourly_reply.h"
#include "csa_web_group_hourly_reply.hpp"
#include "csa_web_sg.h"
#include "base64.h"

extern "C" {
#include <dyn_params_defs.h>
}


static BswLog_c csa_web_serverMT_log_d(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_DETAILED);
static BswLog_c csa_web_serverMT_log_i(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_INFO);
static BswLog_c csa_web_serverMT_log_e(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_ERROR);


Ushort_t do_reports_area[SuperGroupInfo_E + 1];


//------------------------------------------------------------------------------------------------
CsaWebServer::CsaWebServer()
{
  //BswLog_c  glog(BswMakeLogSite(FC_CSA_WEB, 1), LM_INTER_CSCI, LOGL_DETAILED);

  //char address[20];
  //er_sprintf(address, "%p", this);
  //er_fprintf(stderr, "Create new CsaWebServer - Default Constructor (%s)\n",address);
  //glog << "Create new CsaWebServer - Default Constructor (Address " << address << ")\n" << LOG_TERMINATOR;

  m_ReportCode = 0;
}


//------------------------------------------------------------------------------------------------
CsaWebServer::CsaWebServer(const struct soap &soap)
{
  //char address[20];
  //er_sprintf(address, "%p", this);
  //er_fprintf(stderr, "Create new CsaWebServer - Copy Constructor (%s)\n",address);
  //csa_web_serverMT_log_d << "Create new CsaWebServer - Copy Constructor (Address " << address << ")\n" << LOG_TERMINATOR;

  soap_copy_context(this, &soap);
  m_ReportCode = 0;
  //this->m_net_data_ptr->CleanAll(&soap);

  //27-Jul-2021 YR BZ#54709 - EMISwebBindingService_init is called only from WebNlaServeThread_c::MyMainFunction
  //EMISwebBindingService_init(soap.imode, soap.omode);
}


//------------------------------------------------------------------------------------------------
CsaWebServer::~CsaWebServer()
{
  //char address[20];
  //er_sprintf(address, "%p", this);
  //er_fprintf(stderr, "Dtor of CsaWebServer (%s)\n",address);
  //csa_web_serverMT_log_d << "Dtor of CsaWebServer (Address " << address << ")\n" << LOG_TERMINATOR;
  destroy();
}


//------------------------------------------------------------------------------------------------
CsaWebServer *CsaWebServer::copy()
{
  //char address[20];
  //er_sprintf(address, "%p", this);
  //csa_web_serverMT_log_d << "CsaWebServer::copy (My Address " << address << ")\n" << LOG_TERMINATOR;

  CsaWebServer *dup = new CsaWebServer(*(struct soap*)this);
  m_ReportCode = 0;
  return dup;
}


//------------------------------------------------------------------------------------------------
void CsaWebServer::CleanAll()
{
  //char address[20];
  //er_sprintf(address, "%p", this);
  //csa_web_serverMT_log_d << "CsaWebServer::CleanAll (My Address " << address << ")\n" << LOG_TERMINATOR;

  if(m_net_data_ptr != NULL)
  {
    m_net_data_ptr->CleanAll(this,ThreadIndex);
    //delete m_net_data_ptr;
    //m_net_data_ptr = NULL;
  }

  //5-Nov-2018 YR BZ#48254
  soap_end((struct soap*)this); // clean up everything and close socket
  soap_destroy((struct soap*)this); // clean up class instances
}


//------------------------------------------------------------------------------------------------
void CsaWebServer::InsertGenListDataItemType( unsigned int            Id,
                                              std::string             Name,
                                              ns1__GenListReturnType &ReturnArrayData )
{
  ns1__GenListDataItemType* tmp;
  tmp =  soap_new_ns1__GenListDataItemType(this, 1);

  tmp->Id = Id;
  tmp->Name = Name;

  this->m_net_data_ptr->GENL_vector.push_back(tmp);
  ReturnArrayData.returnArray.push_back(tmp);
}


//------------------------------------------------------------------------------------------------
int  CsaWebServer::GroupDetailFullInfo(ns1__GenInfoRequestType *ns1__RequestGroupDetailFullInfo,
                                       ns1__ReturnArrayDataType &ns1__ReturnGroupDetailFullInfo)
{
  unsigned int*     TmpColorPtr = NULL;
  GroupRecordReply  ReplyObj;
  CsaRc_t           rc = ARC_OK;
  GroupRecord*      grec;
  unsigned int      ret_index =0; //PVL 14-06-2010 1004180 not valid index problem

  //Fool protector
  csa_web_serverMT_log_i << "Invoke CsaWebServer::GroupDetailFullInfo \n" << LOG_TERMINATOR;

  if(ns1__RequestGroupDetailFullInfo == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::GroupDetailFullInfo \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data", "ns1__RequestGroupDetailFullInfo is NULL");
  }


  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  //header
  ns1__ReturnGroupDetailFullInfo.responseInfoHeader = m_net_data_ptr->ResponseHeader;

  ns1__ReturnGroupDetailFullInfo.responseInfoHeader->errorCause = (unsigned int)0;
  ns1__ReturnGroupDetailFullInfo.responseInfoHeader->serviceReqId =
    ns1__RequestGroupDetailFullInfo->requestInfoHeader->serviceReqId;

  if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestGroupDetailFullInfo->requestInfoHeader->ticket))
  {
    ns1__ReturnGroupDetailFullInfo.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::GroupDetailFullInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  if(!m_db_data_ptr->groupstab.csa_web_TryLock())  //MT
  {
    ns1__ReturnGroupDetailFullInfo.responseInfoHeader->errorCause = (unsigned int)700;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT//MT BUSY
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::GroupDetailFullInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  for (unsigned int i=0; i<ns1__RequestGroupDetailFullInfo->Id.size(); i++)
  {
    m_db_data_ptr->limitstab.VerifyGroupInLimitTab(ns1__RequestGroupDetailFullInfo->Id[i]);//PVL 28-07-2009 1002567
    rc = Csa_Grps_Tbls[TBL_ONLINE]->get(ns1__RequestGroupDetailFullInfo->Id[i], &grec);

    if (rc==ARC_OK) //PVL 14-06-2010 1004180 not valid index problem
    {
      //array
      if(ReplyObj.AddNewGroupBlock(this,grec->Grp_id,ns1__ReturnGroupDetailFullInfo))
      {
        //{ERS : 6.3.1.3.10 - primary_agents, secondary }
        ReplyObj.InsertFullData(this,"f6_3_1_3_10","primary_agents",grec->Current_agents_state.primary_agents.get(),ns1__ReturnGroupDetailFullInfo.returnArrayOfBlocks[ret_index]);//PVL 14-06-2010 1004180 not valid index problem

        //ERS:  f6_3_1_3_1 - num_of_agents
        m_db_data_ptr->limitstab.GetColorOfERS(grec->Grp_id,
                                               m_db_data_ptr->limitstab.ersid_val.f6_3_1_3_1,
                                               grec->Current_agents_state.number_of_agents.get(),
                                               &TmpColorPtr);

        ReplyObj.InsertFullDataBG(this,TmpColorPtr,"f6_3_1_3_1","Logged in",grec->Current_agents_state.number_of_agents.get(),ns1__ReturnGroupDetailFullInfo.returnArrayOfBlocks[ret_index]);//PVL 14-06-2010 1004180 not valid index problem

        //ERS : 6.3.1.3.2 - agents_on_release}
        m_db_data_ptr->limitstab.GetColorOfERS(grec->Grp_id,
                                               m_db_data_ptr->limitstab.ersid_val.f6_3_1_3_2,
                                               grec->Current_agents_state.agents_on_release.get(),
                                               &TmpColorPtr);

        ReplyObj.InsertFullDataBG(this,TmpColorPtr,"f6_3_1_3_2","Release",grec->Current_agents_state.agents_on_release.get(),ns1__ReturnGroupDetailFullInfo.returnArrayOfBlocks[ret_index]);//PVL 14-06-2010 1004180 not valid index problem

        //ERS : 6.3.1.3.3 - agents_on_wrap_up}
        ReplyObj.InsertFullData(this,"f6_3_1_3_3","Wrap-up",grec->Current_agents_state.agents_on_wrap_up.get(),ns1__ReturnGroupDetailFullInfo.returnArrayOfBlocks[ret_index]);//PVL 14-06-2010 1004180 not valid index problem

        //ERS : 6.3.1.3.4 - idle_agents}
        m_db_data_ptr->limitstab.GetColorOfERS(grec->Grp_id,
                                               m_db_data_ptr->limitstab.ersid_val.f6_3_1_3_4,
                                               grec->Current_agents_state.idle_agents.get(),
                                               &TmpColorPtr);

        ReplyObj.InsertFullDataBG(this,TmpColorPtr,"f6_3_1_3_4","Idle",grec->Current_agents_state.idle_agents.get(),ns1__ReturnGroupDetailFullInfo.returnArrayOfBlocks[ret_index]);//PVL 14-06-2010 1004180 not valid index problem

        //ERS : 6.3.1.3.5 - acd_busy_agents}
        ReplyObj.InsertFullData(this,"f6_3_1_3_5","ACD",grec->Current_agents_state.busy_agents.get(),ns1__ReturnGroupDetailFullInfo.returnArrayOfBlocks[ret_index]);//PVL 14-06-2010 1004180 not valid index problem

        //ERS : 6.3.1.3.6 - non_acd_busy_agents}
        m_db_data_ptr->limitstab.GetColorOfERS(grec->Grp_id,
                                               m_db_data_ptr->limitstab.ersid_val.f6_3_1_3_6,
                                               grec->Current_agents_state.non_acd_busy_agents.get(),
                                               &TmpColorPtr );

        ReplyObj.InsertFullDataBG(this,TmpColorPtr,"f6_3_1_3_6","Non-ACD",grec->Current_agents_state.non_acd_busy_agents.get(),ns1__ReturnGroupDetailFullInfo.returnArrayOfBlocks[ret_index]);//PVL 14-06-2010 1004180 not valid index problem

        //if (non_acd_busy_agents != grp_stat.agents.non_acd_busy_agents){
        //  non_acd_busy_agents = grp_stat.agents.non_acd_busy_agents;
        //  ReplyObj.InsertFullData(this,"f6_3_1_3_6, non_acd_busy_agents);
        //};



        //{ERS : 6.3.1.3.14 - agents_on_dnd}
        m_db_data_ptr->limitstab.GetColorOfERS(grec->Grp_id,
                                               m_db_data_ptr->limitstab.ersid_val.f6_3_1_3_14,
                                               grec->Current_agents_state.agents_on_dnd.get(),
                                               &TmpColorPtr);

        ReplyObj.InsertFullDataBG(this,TmpColorPtr,"f6_3_1_3_14","agents_on_dnd",grec->Current_agents_state.agents_on_dnd.get(),ns1__ReturnGroupDetailFullInfo.returnArrayOfBlocks[ret_index]);//PVL 14-06-2010 1004180 not valid index problem

        //{ERS : 6.3.1.3.15 - agents_on_split}
        ReplyObj.InsertFullData(this,"f6_3_1_3_15","agents_on_split",grec->Current_agents_state.agents_on_split.get(),ns1__ReturnGroupDetailFullInfo.returnArrayOfBlocks[ret_index]);//PVL 14-06-2010 1004180 not valid index problem


        //{ERS : 6.3.1.3.13 - agents_on_f_wrap}
        ReplyObj.InsertFullData(this,"f6_3_1_3_13","agents_on_f_wrap",grec->Current_agents_state.agents_on_forced_wrap_up.get(),ns1__ReturnGroupDetailFullInfo.returnArrayOfBlocks[ret_index]);//PVL 14-06-2010 1004180 not valid index problem


        //{ERS: 6.3.1.3.16 - agents wrap + f. wrap}
        ReplyObj.InsertFullData(this,"f6_3_1_3_16","num_of_agents",
                                grec->Current_agents_state.agents_on_wrap_up.get() +
                                grec->Current_agents_state.agents_on_forced_wrap_up.get(),ns1__ReturnGroupDetailFullInfo.returnArrayOfBlocks[ret_index]);//PVL 14-06-2010 1004180 not valid index problem
        // if ((wrap_changed == TRUE) || (f_wrap_changed == TRUE))
        //   ReplyObj.InsertFullData(this,"f6_3_1_3_16, agents_on_wrap_up + agents_on_f_wrap);

        ret_index++; //PVL 14-06-2010 1004180 not valid index problem
      }
    }//PVL 14-06-2010 1004180 not valid index problem
  }

  m_db_data_ptr->groupstab.csa_web_UnLock();//MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,0);//PVL_STAT

  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::GroupDetailFullInfo \n" << LOG_TERMINATOR;

  return SOAP_OK;
}


/// Web service operation 'GroupEntityUpdateQueInfo' (returns error code or SOAP_OK)
int CsaWebServer::GroupQueInfo(ns1__GenInfoRequestType *ns1__RequestGroupQueInfo,
                               ns1__ReturnArrayDataType &ns1__ReturnGroupQueInfo)
{
  GroupRecordReply  ReplyObj;
  CsaRc_t           rc = ARC_OK;
  GroupRecord*      grec;
  unsigned int      ret_index=0; //PVL 14-06-2010 1004180 not valid index problem

  //Fool protector
  csa_web_serverMT_log_i << "Invoke CsaWebServer::GroupQueInfo \n" << LOG_TERMINATOR;

  if(ns1__RequestGroupQueInfo == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::GroupQueInfo \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data", "ns1__RequestGroupQueInfo is NULL");
  }


  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__ReturnGroupQueInfo.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__ReturnGroupQueInfo.responseInfoHeader->serviceReqId =
    ns1__RequestGroupQueInfo->requestInfoHeader->serviceReqId;

  if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestGroupQueInfo->requestInfoHeader->ticket))
  {
    ns1__ReturnGroupQueInfo.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::GroupQueInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  if(!m_db_data_ptr->groupstab.csa_web_TryLock())  //MT
  {
    ns1__ReturnGroupQueInfo.responseInfoHeader->errorCause = (unsigned int)700;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT//MT BUSY
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::GroupQueInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  for (unsigned int i=0; i<ns1__RequestGroupQueInfo->Id.size(); i++)
  {
    m_db_data_ptr->limitstab.VerifyGroupInLimitTab(ns1__RequestGroupQueInfo->Id[i]); //PVL 28-07-2009 1002567
    rc = Csa_Grps_Tbls[TBL_ONLINE]->get(ns1__RequestGroupQueInfo->Id[i], &grec);
    if (rc == ARC_OK)
    {
      if(ReplyObj.AddNewGroupBlock(this,grec->Grp_id,ns1__ReturnGroupQueInfo))
      {
        ns1__ReturnGroupQueInfo.responseInfoHeader->errorCause = (unsigned int)0;
        ReplyObj.MakePerGroup_QueInfo(this,grec,ns1__ReturnGroupQueInfo.returnArrayOfBlocks[ret_index]); //PVL 14-06-2010 1004180 not valid index problem
        ret_index++; //PVL 14-06-2010 1004180 not valid index problem
      }
    }
    else
    {
      ns1__ReturnGroupQueInfo.responseInfoHeader->errorCause = (unsigned int)1;
    }
  }
  m_db_data_ptr->groupstab.csa_web_UnLock();//MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__ReturnGroupQueInfo->responseInfoHeader->errorCause);//PVL_STAT

  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::GroupQueInfo \n" << LOG_TERMINATOR;

  return SOAP_OK;
}


/// Web service operation 'UserLogin' (returns error code or SOAP_OK)
int CsaWebServer::UserLogin(ns1__LoginRequestType *ns1__RequestUserLogin,
                            ns1__LoginReturnType& ns1__ReturnUserLogin)
{
  csa_web_serverMT_log_i << "Invoke CsaWebServer::UserLogin \n" << LOG_TERMINATOR;

  //Fool protector
  if(ns1__RequestUserLogin == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::UserLogin \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data", "ns1__RequestUserLogin is NULL");
  }


  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  sup_data_t *new_sup_data = NULL;
  RetCode_t executionResult = 700 ;
  ns1__ReturnUserLogin.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__ReturnUserLogin.responseInfoHeader->errorCause = 0;
  ns1__ReturnUserLogin.responseInfoHeader->serviceReqId = ns1__RequestUserLogin->serviceReqId;
  ns1__ReturnUserLogin.loginEnable = FALSE;

  ha_status_is_active = IsHAActive();
  if(ha_status_is_active)
    csa_web_serverMT_log_i << "Server is ACTIVE";
  else
  {
    ns1__ReturnUserLogin.responseInfoHeader->errorCause = 300;
    csa_web_serverMT_log_i << "Server is NOT ACTIVE";
  }
  csa_web_serverMT_log_i << LOG_TERMINATOR;

  if(ha_status_is_active)
  {
#ifdef _SM_AUTHENTICATION_
    //USE SM
    if( this->m_db_data_ptr->TicketTab.GetTicketSM(ip,
                                                   ns1__RequestUserLogin->name,
                                                   ns1__RequestUserLogin->pass,
                                                   ns1__RequestUserLogin->session_id,
                                                   ns1__RequestUserLogin->ip_address,
                                                   ns1__RequestUserLogin->applicationId,
                                                   &new_sup_data,executionResult) )
    {
#else
    //USE LOCAL
    if(this->m_db_data_ptr->TicketTab.GetTicketLocal(ns1__RequestUserLogin->name, ns1__RequestUserLogin->pass,ns1__RequestUserLogin->applicationId, &new_sup_data)){
#endif
      if(new_sup_data != NULL)
      {
        ns1__ReturnUserLogin.ticket= new_sup_data->ticket;
        ns1__ReturnUserLogin.loginEnable = TRUE;
        ns1__ReturnUserLogin.supLevel = new_sup_data->sup_level; //PVL 13-06-2010 1004161
        m_db_data_ptr->TraceTab.ChangeNumberByLicenceType(this->ip,new_sup_data->license_type,TRUE);//PVL_STAT
        m_db_data_ptr->limitstab.ReloadAll(); //PVL 29-07-2009 1002487
      }
      //m_db_data_ptr->TraceTab.EndConnection(this->ip,0);//PVL_STAT
    }
    else
    {
      ns1__ReturnUserLogin.responseInfoHeader->errorCause = executionResult;
      //m_db_data_ptr->TraceTab.EndConnection(this->ip,executionResult);//PVL_STAT
    }
  }

  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::UserLogin \n" << LOG_TERMINATOR;
  return SOAP_OK;
}


// Web service operation 'UserLogout' (returns error code or SOAP_OK)
int CsaWebServer::UserLogout(ns1__GenRequestType *ns1__RequestUserLogout,
                             ns1__ResponseInfoHeaderType &ns1__NoDataResponse)
{
  csa_web_serverMT_log_i << "Invoke CsaWebServer::UserLogout \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__RequestUserLogout == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::UserLogout \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__RequestUserLogout", "ns1__RequestGroupDetailFullInfo is NULL");
  }


  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__NoDataResponse.serviceReqId =ns1__RequestUserLogout->requestInfoHeader->serviceReqId;

  //PVL_STAT
  sup_data_t *tmp_sup_data;
  if(!this->m_db_data_ptr->TicketTab.VerifyTicketGetData(ns1__RequestUserLogout->requestInfoHeader->ticket,
    &tmp_sup_data))
  {
    ns1__NoDataResponse.errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::UserLogout \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  //PVL_STAT

  m_db_data_ptr->TraceTab.ChangeNumberByLicenceType(this->ip,tmp_sup_data->license_type,FALSE);//PVL_STAT
#ifdef _SM_AUTHENTICATION_
  //USE SM
  if(this->m_db_data_ptr->TicketTab.ReturnTicketSM(ns1__RequestUserLogout->requestInfoHeader->ticket))
#else
  if(this->m_db_data_ptr->TicketTab.ReturnTicket(ns1__RequestUserLogout->requestInfoHeader->ticket))
#endif
    ns1__NoDataResponse.errorCause = 0;
  else
    ns1__NoDataResponse.errorCause = 1;

  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__NoDataResponse->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::UserLogout \n" << LOG_TERMINATOR;
  return SOAP_OK;
}


// Web service operation 'GroupListForSupervisor' (returns error code or SOAP_OK)
int CsaWebServer::GroupList(ns1__GenRequestType *ns1__RequestGroupList, ns1__GroupListReturnType &ns1__ReturnGroupList)
{
  GroupRecordReply  ReplyObj;
  DBManager_c&      db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t         bswrc = BSWRC_OK;//,executionResult;
  //Statement_c       st;
  //ResultSet_c       rs;
  bool              dbirc = true, no_records = true;
  //char              sql_statement[MAX_SQL_QUERY_SIZE];
  sup_data_t*       tmp_sup_data;

  csa_web_serverMT_log_i << "Invoke CsaWebServer::GroupList \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__RequestGroupList == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::GroupList \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__RequestGroupList", "ns1__RequestGroupList is NULL");
  }


  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT

  //unsigned int               grpId ;
  //char                       grpName[51] ;
  //char                       grpNumber[51];
  //unsigned int               grpEmailEnable;
  //unsigned int               user_is_owner;
  //char                       nla_priv[5];
  ns1__ReturnGroupList.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__ReturnGroupList.responseInfoHeader->serviceReqId = ns1__RequestGroupList->requestInfoHeader->serviceReqId;
  ns1__ReturnGroupList.responseInfoHeader->errorCause = 0;

  //sup_data_t *tmp_sup_data;

  //====================================
  //printf("  +++ time: %d, check if GroupList. already in process report_code: [%d]  thid: [%d]\n",time(0), do_reports_area[GroupList_E], this->ThreadIndex);  fflush(stdout);
  if (do_reports_area[GroupList_E] == 1) // already process brief agent report in another thread
  {
    //12-Oct-2021 YR BZ#55296
    ns1__ReturnGroupList.responseInfoHeader->errorCause = (unsigned int)400;//MT SERVER BUSY
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 400. SERVER BUSY GroupList \n" << LOG_TERMINATOR;
    printf("+++ errorCause = 400. SERVER BUSY CsaWebServer::GroupList  \n");  fflush(stdout);
    return SOAP_OK;
  }
  do_reports_area[GroupList_E] = 1;
  m_ReportCode = GroupList_E;
  //=====================================

  //20-Dec-2020 YR BZ#53247
  if(!m_db_data_ptr->TicketTab.VerifyTicketGetData(ns1__RequestGroupList->requestInfoHeader->ticket, &tmp_sup_data))
  {
    ns1__ReturnGroupList.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::GroupList \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  // Mik only for test
  if(!m_db_data_ptr->groupstab.csa_web_TryLock())  //MT
  {
    ns1__ReturnGroupList.responseInfoHeader->errorCause = (unsigned int)700;//MT SERVER BUSY
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::GroupList \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  m_db_data_ptr->groupstab.LoadGroupList(tmp_sup_data->user_id, tmp_sup_data->sup_level);
  m_db_data_ptr->agentstab.LoadAgentsList();


  //if((tmp_sup_data->sup_level== 1)|| // supervisor
  //       (tmp_sup_data->sup_level == 3)) //Entity Administrator //PVL, 1-12-2008 1001249
  //er_sprintf(sql_statement,"select group_id,g_name, g_number,email_enabled from grp");
  //     else
  //er_sprintf(sql_statement,"select grp.group_id,grp.g_name,grp.g_number,grp.email_enabled,grp_sup.nla_priv from grp,grp_sup where grp_sup.group_id=grp.group_id and grp_sup.g_type = 'g' and grp_sup.user_id=%d",
  //        tmp_sup_data->user_id);

  //st.SetQuery(sql_statement);
  //bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);

  //printf("ServiceReqId - %d\n",ns1__RequestGroupList->requestInfoHeader->serviceReqId);
  ns1__ReturnGroupList.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__ReturnGroupList.responseInfoHeader->serviceReqId = ns1__RequestGroupList->requestInfoHeader->serviceReqId;

  for( m_db_data_ptr->groupstab.it = m_db_data_ptr->groupstab.tab.begin();
       m_db_data_ptr->groupstab.it != m_db_data_ptr->groupstab.tab.end();
       ++m_db_data_ptr->groupstab.it )
  {
    ReplyObj.InsertGroupList(this,
                             m_db_data_ptr->groupstab.it->first,
                             m_db_data_ptr->groupstab.it->second.grpName,
                             m_db_data_ptr->groupstab.it->second.grpNumber,
                             m_db_data_ptr->groupstab.it->second.grpEmailEnable,
                             ns1__ReturnGroupList);
  }

  //InsertGroupList(this,1,"SupportGroup","10",0,ns1__ReturnGroupListForSupervisor);
  //InsertGroupList(this,2,"SupportGroup2Email","2",1,ns1__ReturnGroupListForSupervisor);

  //Ushort_t total_records_num = rs.GetRecordCount();
  //Ushort_t left_records_num = rs.GetRecordCount();
  //Ushort_t first_record_index;
  //if (total_records_num == 0)
  //{
  //  //NOTHING
  //}
  //else
  //{
  //  while(left_records_num > 0)
  //  {
  //    //save the first record index
  //    first_record_index = total_records_num - left_records_num;
  //    //start to fill buffer with NlaGroupEntityInfo structures
  //    while(rs.Next() && bswrc == BSWRC_OK)
  //    {
  //      if (!(rs.GetUlongByName("group_id",(Ulong_t &)grpId))          ||
  //          !(rs.GetStringByName("g_name",grpName))                    ||
  //          !(rs.GetStringByName("g_number",(char *)grpNumber))        ||
  //          !(rs.GetUlongByName("email_enabled",(Ulong_t &)grpEmailEnable)))
  //      {
  //        //do nothing. get next result set
  //        //GetLog() << " AdiMngr_c::HandleAgentGroupListReply - opcode: Capp_Dbi_get_lst_grp_entities_req_op_code\n"
  //        //  << " Can't get record from result set\n"<< LOG_TERMINATOR;
  //        left_records_num--;
  //      }
  //      else
  //      {
  //        user_is_owner = (Flag_t)false;
  //        //if((tmp_sup_data->sup_level == 1)|| // supervisor
  //        //  (tmp_sup_data->sup_level == 3)) //Entity Administrator //PVL, 8-02-2009 1001249
  //        user_is_owner = (Flag_t)true;

  //        //else
  //        //{
  //        //rs.GetStringByName("nla_priv",(char *)nla_priv);
  //        //if(nla_priv[0] == 't')
  //        //  user_is_owner = (Flag_t)true;
  //        //}

  //        //********  Encode - Start  **********
  //        //string grp_name = grpName;
  //        //string b64 = "";
  //        //Base64::Encode(grp_name, &b64);
  //        //ReplyObj.InsertGroupList(this,grpId,b64,grpNumber,grpEmailEnable,ns1__ReturnGroupList);
  //        //********  Encode - End  ************

  //        ReplyObj.InsertGroupList(this,grpId,grpName,grpNumber,grpEmailEnable,ns1__ReturnGroupList);
  //        left_records_num--;
  //        //if there are more info records to insert
  //        if (left_records_num == 0)
  //          break;
  //      }
  //    }
  //  }
  //}
  m_db_data_ptr->groupstab.csa_web_UnLock(); //MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,0);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::GroupList \n" << LOG_TERMINATOR;
  return SOAP_OK;
}


//----------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
int CsaWebServer::GroupAgentsInfo(ns1__GenInfoRequestType *ns1__RequestGroupAgentsInfo, ns1__ReturnArrayDataType &ns1__ReturnGroupAgentsInfo)
{
  GroupRecordReply    ReplyObj;
  CsaRc_t             rc = ARC_OK;
  GroupRecord*        grec;
  unsigned int        ret_index=0; //PVL 14-06-2010 1004180 not valid index problem

  csa_web_serverMT_log_i << "Invoke CsaWebServer::GroupAgentsInfo \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__RequestGroupAgentsInfo == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::GroupAgentsInfo \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__RequestGroupAgentsInfo", "ns1__RequestGroupAgentsInfo is NULL");
  }


  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__ReturnGroupAgentsInfo.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__ReturnGroupAgentsInfo.responseInfoHeader->serviceReqId = ns1__RequestGroupAgentsInfo->requestInfoHeader->serviceReqId;

  if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestGroupAgentsInfo->requestInfoHeader->ticket))
  {
    ns1__ReturnGroupAgentsInfo.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::GroupAgentsInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  if(!m_db_data_ptr->groupstab.csa_web_TryLock())	//MT
  {
    ns1__ReturnGroupAgentsInfo.responseInfoHeader->errorCause = (unsigned int)700;//MT BUSY
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::GroupAgentsInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  //9-Aug-2022 YR BZ#56641
  char servers_info[256];
  memset(servers_info, 0, 256);
  AddServerTimeToServersInfo(servers_info);
  ns1__ReturnGroupAgentsInfo.responseInfoHeader->serversInfo = servers_info;

  for (unsigned int i=0; i<ns1__RequestGroupAgentsInfo->Id.size(); i++)
  {
    m_db_data_ptr->limitstab.VerifyGroupInLimitTab(ns1__RequestGroupAgentsInfo->Id[i]); //PVL 28-07-2009 1002567
    rc = Csa_Grps_Tbls[TBL_ONLINE]->get(ns1__RequestGroupAgentsInfo->Id[i], &grec);

    if(rc == ARC_OK)
    {
      if(ReplyObj.AddNewGroupBlock(this,grec->Grp_id,ns1__ReturnGroupAgentsInfo))
      {
        ns1__ReturnGroupAgentsInfo.responseInfoHeader->errorCause = (unsigned int)0;
        ReplyObj.MakePerGroup_AgentsInfo(this,grec,ns1__ReturnGroupAgentsInfo.returnArrayOfBlocks[ret_index]); //PVL 14-06-2010 1004180 not valid index problem
        ret_index++; //PVL 14-06-2010 1004180 not valid index problem
      }
    }
    else
    {
      ns1__ReturnGroupAgentsInfo.responseInfoHeader->errorCause = (unsigned int)1;
    }
  }

  m_db_data_ptr->groupstab.csa_web_UnLock();//MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__ReturnGroupAgentsInfo->responseInfoHeader->errorCause);//PVL_STAT

  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::GroupAgentsInfo \n" << LOG_TERMINATOR;

  return SOAP_OK;
}


//--------------------------------------------
int CsaWebServer::AgentsPerGroupInfo(ns1__GenInfoRequestType *ns1__RequestGroupAgentsMatrix, ns1__ReturnMatrixDataTypePerGroup &ns1__ReturnGroupAgentsMatrix)
{
  AgentRecordReply            ReplyObj;
  AgentRecord*                agent;
  CsaAgentPerGroupRecord_t*   agent_per_group;
  Uint_t                      token;

  CsaRc_t rc;
  CsaRc_t rc2;

  csa_web_serverMT_log_i << "Invoke CsaWebServer::AgentsPerGroupInfo \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__RequestGroupAgentsMatrix == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::AgentsPerGroupInfo \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data", "ns1__RequestGroupAgentsMatrix is NULL");
  }

  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__ReturnGroupAgentsMatrix.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__ReturnGroupAgentsMatrix.responseInfoHeader->errorCause = (unsigned int)1;
  ns1__ReturnGroupAgentsMatrix.responseInfoHeader->serviceReqId =
    ns1__RequestGroupAgentsMatrix->requestInfoHeader->serviceReqId;

  if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestGroupAgentsMatrix->requestInfoHeader->ticket))
  {
    ns1__ReturnGroupAgentsMatrix.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::AgentsPerGroupInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  if(!m_db_data_ptr->groupstab.csa_web_TryLock())	//MT
  {
    ns1__ReturnGroupAgentsMatrix.responseInfoHeader->errorCause = (unsigned int)700; //MT BUSY
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::AgentsPerGroupInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  //9-Aug-2022 YR BZ#56641
  char servers_info[256];
  memset(servers_info, 0, 256);
  AddServerTimeToServersInfo(servers_info);
  ns1__ReturnGroupAgentsMatrix.responseInfoHeader->serversInfo = servers_info;

  for (unsigned int i=0; i<ns1__RequestGroupAgentsMatrix->Id.size(); i++)
  {
    m_db_data_ptr->limitstab.VerifyGroupInLimitTab(ns1__RequestGroupAgentsMatrix->Id[i]); //PVL 28-07-2009 1002567
    ns1__ReturnGroupAgentsMatrix.responseInfoHeader->errorCause = (unsigned int)0;
    ReplyObj.AddNewGroupBlock(this, ns1__RequestGroupAgentsMatrix->Id[i], ns1__ReturnGroupAgentsMatrix);
    for( rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getFirstLoginForGroup(ns1__RequestGroupAgentsMatrix->Id[i],
                                                                  &agent_per_group,
                                                                  token);
         rc2 == ARC_OK;
         rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getNextLoginForGroup(&agent_per_group,token) )
    {
      // if agent logged in, add full info for agent to event
      if (agent_per_group->Logged_in)
      {
        m_db_data_ptr->limitstab.VerifyGroupInLimitTab(agent_per_group->Group_id); //PVL 28-07-2009 1002567
        // static info - need agent record
        ReplyObj.CreateNewRow(this); //Start Row
        rc = Csa_Agents_Tbls[TBL_ONLINE]->get(agent_per_group->Internal_id,&agent);
        if (rc == ARC_OK)
        {
          ReplyObj.InsertAgentDataToRow(this,agent,agent_per_group,ns1__RequestGroupAgentsMatrix->Id[i]);
          // At the END , inser Row
          ReplyObj.InsertRowToMatrix(this,ns1__ReturnGroupAgentsMatrix.returnDataRowsPerGroup[i]);//End Row
        }
      }
    }// END for
  }

  m_db_data_ptr->groupstab.csa_web_UnLock();//MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__ReturnGroupAgentsMatrix->responseInfoHeader->errorCause);//PVL_STAT

  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::AgentsPerGroupInfo \n" << LOG_TERMINATOR;

  return SOAP_OK;
}



// Web service operation 'AgentsList' (returns error code or SOAP_OK)
int CsaWebServer::AgentsList(ns1__GenRequestType *ns1__RequestAgentsList, ns1__AgentsListReturnType &ns1__ReturnAgentsList)
{
  AgentRecordReply ReplyObj;

  csa_web_serverMT_log_i << "soap_receiver_fault Data. CsaWebServer::AgentsList \n" << LOG_TERMINATOR;

  //Fool protector
  if(ns1__RequestAgentsList == NULL)
  {
    csa_web_serverMT_log_i << "Invoke CsaWebServer::AgentsList \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__RequestAgentsList", "ns1__RequestAgentsList is NULL");
  }


  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT

  //====================================
  //printf("  +++ time: %d, check if agents list. already in process report_code: [%d]  SthID: [%d]\n",time(0), do_reports_area[BriefAgents_E], this->ThreadIndex);  fflush(stdout);
  if (do_reports_area[AgentsList_E] == 1) // already process brief agent report in another thread
  {
    //12-Oct-2021 YR BZ#55296
    ns1__ReturnAgentsList.responseInfoHeader->errorCause = (unsigned int)400;//MT SERVER BUSY
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 400. SERVER BUSY AgentsList \n" << LOG_TERMINATOR;
    printf("+++ errorCause = 400. SERVER BUSY CsaWebServer::AgentsList \n");  fflush(stdout);
    return SOAP_OK;
  }
  do_reports_area[AgentsList_E] = 1;
  m_ReportCode = AgentsList_E;
  //=====================================

  //if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestAgentsList->requestInfoHeader->ticket))
  //{
  //  ns1__ReturnAgentsList->responseInfoHeader->errorCause = 500;
  //  m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
  //  csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::AgentsList \n" << LOG_TERMINATOR;
  //  return SOAP_OK;
  //}


  ns1__ReturnAgentsList.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__ReturnAgentsList.responseInfoHeader->serviceReqId = ns1__RequestAgentsList->requestInfoHeader->serviceReqId;
  ns1__ReturnAgentsList.responseInfoHeader->errorCause = 0;

  if(!m_db_data_ptr->agentstab.csa_web_TryLock())	//MT
  {
    ns1__ReturnAgentsList.responseInfoHeader->errorCause = (unsigned int)700;//MT SERVER BUSY
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::AgentsList \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  m_db_data_ptr->agentstab.LoadAgentsList();

  if(m_db_data_ptr->agentstab.tab.size() > 0)
  {
    for(m_db_data_ptr->agentstab.it=m_db_data_ptr->agentstab.tab.begin(); m_db_data_ptr->agentstab.it!= m_db_data_ptr->agentstab.tab.end(); ++m_db_data_ptr->agentstab.it)
    {
      ReplyObj.InsertAgentsList(this,
                                (*m_db_data_ptr->agentstab.it).second.a_id,//a_id,
                                (*m_db_data_ptr->agentstab.it).second.a_name,//a_name,
                                (*m_db_data_ptr->agentstab.it).second.a_number,//a_number,
                                ns1__ReturnAgentsList);
    }
  }

  m_db_data_ptr->agentstab.csa_web_UnLock();//MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,0);//PVL_STAT

  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::AgentsList \n" << LOG_TERMINATOR;

  return SOAP_OK;
}


//------------------------------------------------------------------------------------------------
	/// Web service operation 'BriefAgents' (returns error code or SOAP_OK)
int CsaWebServer::BriefAgents(ns1__BriefInfoRequestType *ns1__RequestBriefAgentsMatrix, ns1__ReturnMatrixDataTypeBG &ns1__ReturnBriefAgentsMatrix)
{
  AgentRecordReply          ReplyObj;
  AgentRecord*              agent = NULL;
  CsaAgentPerGroupRecord_t* agent_per_group = NULL;
  Uint_t                    token;
  BriefAgentInfo            TotalsAgentInfo; //CRASH 28-06-2010
  CsaRc_t rc;
  CsaRc_t rc2;

  csa_web_serverMT_log_i << "Invoke CsaWebServer::BriefAgents \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__RequestBriefAgentsMatrix == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::BriefAgents \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data", "ns1__RequestBriefAgentsMatrix is NULL");
  }


  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  //TEST
  //	this->m_db_data_ptr->GetData(this->ThreadIndex);
  //TEST
  ns1__ReturnBriefAgentsMatrix.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__ReturnBriefAgentsMatrix.responseInfoHeader->errorCause = (unsigned int)1;
  ns1__ReturnBriefAgentsMatrix.responseInfoHeader->serviceReqId = ns1__RequestBriefAgentsMatrix->requestInfoHeader->serviceReqId;
  //====================================

  if (do_reports_area[BriefAgents_E] == 1) // already process brief agent report in another thread
  {
    //12-Oct-2021 YR BZ#55296
    ns1__ReturnBriefAgentsMatrix.responseInfoHeader->errorCause = (unsigned int)400;//MT SERVER BUSY
    //DData->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 400. SERVER BUSY BriefAgents \n" << LOG_TERMINATOR;
    printf("+++ errorCause = 400. SERVER BUSY CsaWebServer::BriefAgents \n");  fflush(stdout);
    return SOAP_OK;
  }

  do_reports_area[BriefAgents_E] = 1;
  m_ReportCode = BriefAgents_E;
  //=====================================


  //   if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestBriefAgentsMatrix->requestInfoHeader->ticket))
  //{
  //  ns1__ReturnBriefAgentsMatrix->responseInfoHeader->errorCause = 500;
  //  m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
  //  csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::BriefAgents \n" << LOG_TERMINATOR;
  //  return SOAP_OK;
  //}

  if(!m_db_data_ptr->agentstab.csa_web_TryLock())	//MT
  {
    ns1__ReturnBriefAgentsMatrix.responseInfoHeader->errorCause = (unsigned int)700;//MT SERVER BUSY
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::BriefAgents \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  //9-Aug-2022 YR BZ#56641
  char servers_info[256];
  memset(servers_info, 0, 256);
  AddServerTimeToServersInfo(servers_info);
  ns1__ReturnBriefAgentsMatrix.responseInfoHeader->serversInfo = servers_info;

  Id_type a_id ;
  if(m_db_data_ptr->agentstab.tab.size() > 0)
  {
    for( m_db_data_ptr->agentstab.it = m_db_data_ptr->agentstab.tab.begin();
         m_db_data_ptr->agentstab.it != m_db_data_ptr->agentstab.tab.end();
         ++m_db_data_ptr->agentstab.it )
    {
      TotalsAgentInfo.InitCounters();//CRASH 28-06-2010
      a_id = (*m_db_data_ptr->agentstab.it).second.a_id;
      rc = Csa_Agents_Tbls[TBL_ONLINE]->getByDataBaseId( a_id,&agent);
      if (rc == ARC_OK)
      {
        // cycle through login records
        for( rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getFirstLoginForAgent(agent->Internal_id, &agent_per_group, token);
             rc2 == ARC_OK;
             rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getNextLoginForAgent(&agent_per_group, token) )
        {
          ns1__ReturnBriefAgentsMatrix.responseInfoHeader->errorCause  = (unsigned int)0;
          // if agent logged in, add full info for agent to event
          if (agent_per_group->Logged_in)
          {
            // static info - need agent record
          }
          //Collect Data over All Groups for Agent
          TotalsAgentInfo.SetAcdData(agent,agent_per_group);
        } //End of All groups per Agent

        // Build Brief Data
        ReplyObj.CreateNewRow(this); //Start Row
        {//MT NESTED!!!! Critical Section for groups inside of Agents!
          if(!m_db_data_ptr->groupstab.csa_web_TryLock())	//MT
          {
            ns1__ReturnBriefAgentsMatrix.responseInfoHeader->errorCause = (unsigned int)700;//MT BUSY
            //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT
            m_db_data_ptr->agentstab.csa_web_UnLock();//MT
            return SOAP_OK;
          }
          ReplyObj.InsertBriefAgentDataToRow(this,agent,TotalsAgentInfo); //MT GROUP LOCK HERE
          m_db_data_ptr->groupstab.csa_web_UnLock();
        }//END of Critical Section of Group

        // At the END , inser Row
        ReplyObj.InsertRowToMatrix(this,ns1__ReturnBriefAgentsMatrix);//End Row

      } // If agent FOUND
      else
      { //Agent not Found , so show LogOut
        ns1__ReturnBriefAgentsMatrix.responseInfoHeader->errorCause  = (unsigned int)0;
        ReplyObj.CreateNewRow(this); //Start Row
        ReplyObj.InsertBrief_LogOut_AgentDataToRow (this,a_id);
        ReplyObj.InsertRowToMatrix(this,ns1__ReturnBriefAgentsMatrix);//End Row
      }
    }
  }
  m_db_data_ptr->agentstab.csa_web_UnLock();//MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip, ns1__ReturnBriefAgentsMatrix->responseInfoHeader->errorCause);//PVL_STAT
  //Sleep(10000);
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::BriefAgents \n" << LOG_TERMINATOR;
  return SOAP_OK;
}


int CsaWebServer::GroupAbndInfo(ns1__GenInfoRequestType *ns1__RequestGroupAbndInfo, ns1__ReturnArrayDataType &ns1__ReturnGroupAbndInfo)
{
  GroupRecordReply      ReplyObj;
  CsaRc_t               rc = ARC_OK;
  GroupRecord*          grec;
  unsigned int          ret_index = 0; //PVL 14-06-2010 1004180 not valid index problem

  csa_web_serverMT_log_i << "Invoke CsaWebServer::GroupAbndInfo \n" << LOG_TERMINATOR;

  //Fool protector
  if(ns1__RequestGroupAbndInfo == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::GroupAbndInfo \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__RequestGroupAbndInfo", "ns1__RequestGroupAbndInfo is NULL");
  }

  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT


  ns1__ReturnGroupAbndInfo.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__ReturnGroupAbndInfo.responseInfoHeader->serviceReqId = ns1__RequestGroupAbndInfo->requestInfoHeader->serviceReqId;

  if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestGroupAbndInfo->requestInfoHeader->ticket))
  {
    ns1__ReturnGroupAbndInfo.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::GroupAbndInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  if(!m_db_data_ptr->groupstab.csa_web_TryLock())	//MT
  {
    ns1__ReturnGroupAbndInfo.responseInfoHeader->errorCause = (unsigned int)700;//MT BUSY
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::GroupAbndInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  for (unsigned int i=0; i<ns1__RequestGroupAbndInfo->Id.size(); i++)
  {
    m_db_data_ptr->limitstab.VerifyGroupInLimitTab(ns1__RequestGroupAbndInfo->Id[i]); //PVL 28-07-2009 1002567
    rc = Csa_Grps_Tbls[TBL_ONLINE]->get(ns1__RequestGroupAbndInfo->Id[i], &grec);
    if (rc== ARC_OK)
    {
      if(ReplyObj.AddNewGroupBlock(this,grec->Grp_id,ns1__ReturnGroupAbndInfo))
      {
        ns1__ReturnGroupAbndInfo.responseInfoHeader->errorCause = (unsigned int)0;
        ReplyObj.MakePerGroup_AbandInfo (this,grec,ns1__ReturnGroupAbndInfo.returnArrayOfBlocks[ret_index]);//PVL 14-06-2010 1004180 not valid index problem
        ret_index++; //PVL 14-06-2010 1004180 not valid index problem
      }
    }
    else
    {
      ns1__ReturnGroupAbndInfo.responseInfoHeader->errorCause = (unsigned int)1;
    }
  }

  m_db_data_ptr->groupstab.csa_web_UnLock();//MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__ReturnGroupAbndInfo->responseInfoHeader->errorCause);//PVL_STAT
  return SOAP_OK;
}


/// Web service operation 'GroupEntityUpdateShiftInfo' (returns error code or SOAP_OK)
int  CsaWebServer::GroupShiftInfo(ns1__GenInfoRequestType *ns1__RequestGroupShiftInfo, ns1__ReturnArrayDataType &ns1__ReturnGroupShiftInfo)
{
  GroupRecordReply    ReplyObj;
  CsaRc_t             rc = ARC_OK;
  GroupRecord*        grec;
  unsigned int        ret_index=0; //PVL 14-06-2010 1004180 not valid index problem
  //Fool protector
  if(ns1__RequestGroupShiftInfo == NULL)
    return soap_receiver_fault(this, "No Request Data in ns1__RequestGroupShiftInfo", "ns1__RequestGroupShiftInfo is NULL");

  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__ReturnGroupShiftInfo.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__ReturnGroupShiftInfo.responseInfoHeader->errorCause = (unsigned int)0;
  ns1__ReturnGroupShiftInfo.responseInfoHeader->serviceReqId = ns1__RequestGroupShiftInfo->requestInfoHeader->serviceReqId;

  if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestGroupShiftInfo->requestInfoHeader->ticket))
  {
    ns1__ReturnGroupShiftInfo.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT//MT BUSY
    return SOAP_OK;
  }

  if(!m_db_data_ptr->groupstab.csa_web_TryLock())	//MT
  {
    ns1__ReturnGroupShiftInfo.responseInfoHeader->errorCause = (unsigned int)700; //MT BUSY
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT
    return SOAP_OK;
  }

  for (unsigned int i=0; i<ns1__RequestGroupShiftInfo->Id.size(); i++)
  {
    m_db_data_ptr->limitstab.VerifyGroupInLimitTab(ns1__RequestGroupShiftInfo->Id[i]); //PVL 28-07-2009 1002567
    rc = Csa_Grps_Tbls[TBL_ONLINE]->get(ns1__RequestGroupShiftInfo->Id[i], &grec);
    if (rc== ARC_OK )
    {
      if(ReplyObj.AddNewGroupBlock(this,grec->Grp_id,ns1__ReturnGroupShiftInfo))
      {
        ns1__ReturnGroupShiftInfo.responseInfoHeader->errorCause = (unsigned int)0;
        ReplyObj.MakePerGroup_ShiftInfo (this,grec,ns1__ReturnGroupShiftInfo.returnArrayOfBlocks[ret_index]);//PVL 14-06-2010 1004180 not valid index problem
        ret_index++; //PVL 14-06-2010 1004180 not valid index problem
      }
    }
    else
    {
      ns1__ReturnGroupShiftInfo.responseInfoHeader->errorCause = (unsigned int)1;
    }
  }

  m_db_data_ptr->groupstab.csa_web_UnLock();//MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__ReturnGroupShiftInfo->responseInfoHeader->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::GroupAbndInfo \n" << LOG_TERMINATOR;
  return SOAP_OK;
}


//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
int CsaWebServer::GroupIntOvfInfo(ns1__GenInfoRequestType *ns1__RequestGroupIntOvfInfo, ns1__ReturnArrayDataType &ns1__ReturnGroupIntOvfInfo)
{
  GroupRecordReply    ReplyObj;
  CsaRc_t             rc = ARC_OK;
  GroupRecord*        grec;
  unsigned int        ret_index = 0; //PVL 14-06-2010 1004180 not valid index problem

  csa_web_serverMT_log_i << "Invoke CsaWebServer::GroupIntOvfInfo \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__RequestGroupIntOvfInfo == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::GroupIntOvfInfo \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__RequestGroupIntOvfInfo", "ns1__RequestGroupIntOvfInfo is NULL");
  }

  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__ReturnGroupIntOvfInfo.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__ReturnGroupIntOvfInfo.responseInfoHeader->errorCause = (unsigned int)0;
  ns1__ReturnGroupIntOvfInfo.responseInfoHeader->serviceReqId = ns1__RequestGroupIntOvfInfo->requestInfoHeader->serviceReqId;

  if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestGroupIntOvfInfo->requestInfoHeader->ticket))
  {
    ns1__ReturnGroupIntOvfInfo.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::GroupIntOvfInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  if(!m_db_data_ptr->groupstab.csa_web_TryLock())	//MT
  {
    ns1__ReturnGroupIntOvfInfo.responseInfoHeader->errorCause = (unsigned int)700; //MT BUSY
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::GroupIntOvfInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  for (unsigned int i=0; i<ns1__RequestGroupIntOvfInfo->Id.size(); i++)
  {
    m_db_data_ptr->limitstab.VerifyGroupInLimitTab(ns1__RequestGroupIntOvfInfo->Id[i]);//PVL 28-07-2009 1002567
    rc = Csa_Grps_Tbls[TBL_ONLINE]->get(ns1__RequestGroupIntOvfInfo->Id[i], &grec);
    if (rc== ARC_OK)
    {
      if(ReplyObj.AddNewGroupBlock(this,grec->Grp_id,ns1__ReturnGroupIntOvfInfo))
      {
        ns1__ReturnGroupIntOvfInfo.responseInfoHeader->errorCause = (unsigned int)0;
        ReplyObj.MakePerGroup_InterflInfo (this,grec,ns1__ReturnGroupIntOvfInfo.returnArrayOfBlocks[ret_index]);//PVL 14-06-2010 1004180 not valid index problem
        ret_index++; //PVL 14-06-2010 1004180 not valid index problem
      }
    }
    else
    {
      ns1__ReturnGroupIntOvfInfo.responseInfoHeader->errorCause = (unsigned int)1;
    }
  }
  m_db_data_ptr->groupstab.csa_web_UnLock();//MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__ReturnGroupIntOvfInfo->responseInfoHeader->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::GroupIntOvfInfo \n" << LOG_TERMINATOR;
  return SOAP_OK;
}


//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
/// Web service operation 'GroupGeneralInfo' (returns error code or SOAP_OK)
int CsaWebServer::GroupGeneralInfo(ns1__GenInfoRequestType *ns1__RequestGroupGeneralInfo, ns1__ReturnArrayDataType &ns1__ReturnGroupGeneralInfo)
{
  GroupRecordReply  ReplyObj;
  CsaRc_t           rc = ARC_OK;
  GroupRecord*      grec;
  unsigned int      ret_index = 0; //PVL 14-06-2010 1004180 not valid index problem
  unsigned int*     TmpColorPtr = NULL;

  csa_web_serverMT_log_i << "Invoke CsaWebServer::GroupGeneralInfo \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__RequestGroupGeneralInfo == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::GroupGeneralInfo \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__RequestGroupGeneralInfo", "ns1__RequestGroupGeneralInfo is NULL");
  }


  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT

  int acd_type_acd_offset = getOffSetOfAcdCallTypes(CL_CALL_TYPE_ACD);
  int acd_type_email_offset = 0;//getOffSetOfAcdCallTypes(CL_CALL_TYPE_EMAIL);
  int acd_type_chat_offset = 0;// = getOffSetOfAcdCallTypes(CL_CALL_TYPE_CHAT);
  int acd_type_dial_list_offset = 0;

  bool supportedOutbound = Csa_Setup.supportOutbound();
  bool supportedChat =     Csa_Setup.supportChat();
  bool supportedEmail =    Csa_Setup.supportEmail();
  bool supportedDialList = Csa_Setup.supportDialList();

  // unsigned int
  ns1__ReturnGroupGeneralInfo.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__ReturnGroupGeneralInfo.responseInfoHeader->errorCause = (unsigned int)0;
  ns1__ReturnGroupGeneralInfo.responseInfoHeader->serviceReqId = ns1__RequestGroupGeneralInfo->requestInfoHeader->serviceReqId;

  if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestGroupGeneralInfo->requestInfoHeader->ticket))
  {
    ns1__ReturnGroupGeneralInfo.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::GroupGeneralInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  if(!m_db_data_ptr->groupstab.csa_web_TryLock()) //MT
  {
    ns1__ReturnGroupGeneralInfo.responseInfoHeader->errorCause = (unsigned int)700;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::GroupGeneralInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  for (unsigned int i=0; i<ns1__RequestGroupGeneralInfo->Id.size(); i++)
  {
    m_db_data_ptr->limitstab.VerifyGroupInLimitTab(ns1__RequestGroupGeneralInfo->Id[i]);//PVL 28-07-2009 1002567
    rc = Csa_Grps_Tbls[TBL_ONLINE]->get(ns1__RequestGroupGeneralInfo->Id[i], &grec);
    if (rc == ARC_OK)
    {
      if(ReplyObj.AddNewGroupBlock(this,grec->Grp_id,ns1__ReturnGroupGeneralInfo))
      {
        ns1__ReturnGroupGeneralInfo.responseInfoHeader->errorCause = (unsigned int)0;
        ReplyObj.MakePerGroup_QueInfo(this,grec,ns1__ReturnGroupGeneralInfo.returnArrayOfBlocks[ret_index]);//PVL 14-06-2010 1004180 not valid index problem
        ret_index++; //PVL 14-06-2010 1004180 not valid index problem
      }
    }
    else
    {
      ns1__ReturnGroupGeneralInfo.responseInfoHeader->errorCause = (unsigned int)1;
    }
  }
  m_db_data_ptr->groupstab.csa_web_UnLock();//MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__ReturnGroupGeneralInfo->responseInfoHeader->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::GroupGeneralInfo \n" << LOG_TERMINATOR;
  return SOAP_OK;
}


//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
/// Web service operation 'GroupPeriodInfo' (returns error code or SOAP_OK)
int CsaWebServer::GroupPeriodInfo(ns1__GenInfoRequestType *ns1__RequestGroupPeriodInfo, ns1__ReturnArrayDataType &ns1__ReturnGroupPeriodInfo)
{
  GroupRecordReply  ReplyObj;
  CsaRc_t           rc = ARC_OK;
  GroupRecord*      grec;
  unsigned int      ret_index=0; //PVL 14-06-2010 1004180 not valid index problem

  csa_web_serverMT_log_i << "Invoke CsaWebServer::GroupPeriodInfo \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__RequestGroupPeriodInfo == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::GroupPeriodInfo \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__RequestGroupPeriodInfo", "ns1__RequestGroupPeriodInfo is NULL");
  }


  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__ReturnGroupPeriodInfo.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__ReturnGroupPeriodInfo.responseInfoHeader->errorCause = (unsigned int)0;
  ns1__ReturnGroupPeriodInfo.responseInfoHeader->serviceReqId = ns1__RequestGroupPeriodInfo->requestInfoHeader->serviceReqId;

  if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestGroupPeriodInfo->requestInfoHeader->ticket))
  {
    ns1__ReturnGroupPeriodInfo.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::GroupPeriodInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  if(!m_db_data_ptr->groupstab.csa_web_TryLock())  //MT
  {
    ns1__ReturnGroupPeriodInfo.responseInfoHeader->errorCause = (unsigned int)700;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::GroupPeriodInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  for (unsigned int i=0; i<ns1__RequestGroupPeriodInfo->Id.size(); i++)
  {
    m_db_data_ptr->limitstab.VerifyGroupInLimitTab(ns1__RequestGroupPeriodInfo->Id[i]);//PVL 28-07-2009 1002567
    rc = Csa_Grps_Tbls[TBL_ONLINE]->get(ns1__RequestGroupPeriodInfo->Id[i], &grec);
    if (rc == ARC_OK)
    {
      if(ReplyObj.AddNewGroupBlock(this,grec->Grp_id,ns1__ReturnGroupPeriodInfo))
      {
        ns1__ReturnGroupPeriodInfo.responseInfoHeader->errorCause = (unsigned int)0;
        //ReplyObj.MakePerGroup_QueInfo (this,grec,ns1__ReturnGroupPeriodInfo.returnArrayOfBlocks[ret_index]);
        ReplyObj.MakePerGroup_ShiftInfo (this,grec,ns1__ReturnGroupPeriodInfo.returnArrayOfBlocks[ret_index]);//PVL 14-06-2010 1004180 not valid index problem
        ReplyObj.MakePerGroup_AbandInfo (this,grec,ns1__ReturnGroupPeriodInfo.returnArrayOfBlocks[ret_index]);//PVL 14-06-2010 1004180 not valid index problem
        ReplyObj.MakePerGroup_InterflInfo (this,grec,ns1__ReturnGroupPeriodInfo.returnArrayOfBlocks[ret_index]);//PVL 14-06-2010 1004180 not valid index problem
        ret_index++; //PVL 14-06-2010 1004180 not valid index problem
      }
    }
    else
    {
      ns1__ReturnGroupPeriodInfo.responseInfoHeader->errorCause = (unsigned int)1;
    }
  }
  m_db_data_ptr->groupstab.csa_web_UnLock();//MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__ReturnGroupPeriodInfo->responseInfoHeader->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::GroupPeriodInfo \n" << LOG_TERMINATOR;
  return SOAP_OK;
}

//--------------------------------------------------------------------------------------------------------------
/// Web service operation 'DefaultWorkspace' (returns error code or SOAP_OK)
int CsaWebServer::DefaultWorkspace(ns1__DefaultWorkspaceRequestType *ns1__RequestDefaultWorkspace, ns1__DefaultWorkspaceReturnType &ns1__ReturnDefaultWorkspace)
{
    ReportsReply ReplyObj;

    csa_web_serverMT_log_i << "Invoke CsaWebServer::DefaultWorkspace \n" << LOG_TERMINATOR;
    //Fool protector
	if(ns1__RequestDefaultWorkspace == NULL)
	{
		csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::DefaultWorkspace \n" << LOG_TERMINATOR;
		return soap_receiver_fault(this, "No Request Data in ns1__RequestDefaultWorkspace", "ns1__RequestDefaultWorkspace is NULL");
	}


	//start connection
    //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT

    ns1__ReturnDefaultWorkspace.responseInfoHeader = m_net_data_ptr->ResponseHeader;
	ns1__ReturnDefaultWorkspace.responseInfoHeader->serviceReqId = ns1__RequestDefaultWorkspace->requestInfoHeader->serviceReqId;

	ns1__ReturnDefaultWorkspace.responseInfoHeader->errorCause = (unsigned int)0;

   	sup_data_t *tmp_sup_data;
	if(!m_db_data_ptr->TicketTab.VerifyTicketGetData(ns1__RequestDefaultWorkspace->requestInfoHeader->ticket, &tmp_sup_data))
	{
		ns1__ReturnDefaultWorkspace.responseInfoHeader->errorCause = 500;
		//m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
		csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::DefaultWorkspace \n" << LOG_TERMINATOR;
		return SOAP_OK;
	}

	if(ReplyObj.TryLoadNewDefConf(this,tmp_sup_data->user_id,ns1__ReturnDefaultWorkspace))
	{
		csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::DefaultWorkspace for User Id " << tmp_sup_data->user_id
                               << " (" << ns1__ReturnDefaultWorkspace.fontSize.c_str() << ":" 
                               << ns1__ReturnDefaultWorkspace.language.c_str() << ")"
                               << LOG_TERMINATOR;
		return SOAP_OK;
	}
    else
        ReplyObj.LoadOldDefConf(this,tmp_sup_data->user_id,ns1__ReturnDefaultWorkspace);

	//m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__ReturnDefaultWorkspace->responseInfoHeader->errorCause );//PVL_STAT
	csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::DefaultWorkspace \n" << LOG_TERMINATOR;
	return SOAP_OK;
}

	/// Web service operation 'ReportListData' (returns error code or SOAP_OK)
int CsaWebServer::ReportListData(ns1__ReportListDataRequestType *ns1__RequestReportListData, ns1__ReportListDataReturnType &ns1__ReturnReportListData)
{
	ReportsReply ReplyObj;

	csa_web_serverMT_log_i << "Invoke CsaWebServer::ReportListData \n" << LOG_TERMINATOR;
	//Fool protector
	if(ns1__RequestReportListData == NULL)
	{
		csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::ReportListData \n" << LOG_TERMINATOR;
		return soap_receiver_fault(this, "No Request Data in ns1__RequestReportListData", "ns1__RequestReportListData is NULL");
	}


	//start connection
    //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
	ReplyObj.SetReturnArray(ns1__ReturnReportListData);
    ns1__ReturnReportListData.responseInfoHeader = m_net_data_ptr->ResponseHeader;
	ns1__ReturnReportListData.responseInfoHeader->errorCause = (unsigned int)0;
	ns1__ReturnReportListData.responseInfoHeader->serviceReqId = ns1__RequestReportListData->requestInfoHeader->serviceReqId;

	sup_data_t *tmp_sup_data;
	if(!m_db_data_ptr->TicketTab.VerifyTicketGetData(ns1__RequestReportListData->requestInfoHeader->ticket, &tmp_sup_data))
	{
		ns1__ReturnReportListData.responseInfoHeader->errorCause = 500;
		//m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
		csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::ReportListData \n" << LOG_TERMINATOR;
		return SOAP_OK;
	}
	//Get Reports for agentId
	if(ReplyObj.GetAllReportsByAgentID(this,tmp_sup_data->user_id,ns1__ReturnReportListData))
        ns1__ReturnReportListData.responseInfoHeader->errorCause = (unsigned int)1;

	//m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__ReturnReportListData->responseInfoHeader->errorCause);//PVL_STAT
	csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::ReportListData \n" << LOG_TERMINATOR;
	return SOAP_OK;
}
//--------------------------------------------------------------------------------------------------------------
/// Web service operation 'VoiceDNISList' (returns error code or SOAP_OK)
int CsaWebServer::VoiceDNISList(ns1__GenRequestType *ns1__RequestVoiceDNISList, ns1__DNISListReturnType &ns1__ReturnVoiceDNISList)
{
    unsigned int error;   //PVL_STAT
	DNISRecordReply ReplyObj;

	csa_web_serverMT_log_i << "Invoke CsaWebServer::VoiceDNISList \n" << LOG_TERMINATOR;
	//Fool protector
	if(ns1__RequestVoiceDNISList == NULL)
	{
		csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::VoiceDNISList \n" << LOG_TERMINATOR;
		return soap_receiver_fault(this, "No Request Data in ns1__RequestVoiceDNISList", "ns1__RequestVoiceDNISList is NULL");
	}


	//start connection
    //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
	error= ReplyObj.GetHandleDNISList(this,ns1__RequestVoiceDNISList,ns1__ReturnVoiceDNISList,false);	//Voice
	//m_db_data_ptr->TraceTab.EndConnection(this->ip,error);//PVL_STAT
	csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::VoiceDNISList \n" << LOG_TERMINATOR;
	return SOAP_OK;
}
//--------------------------------------------------------------------------------------------------------------
/// Web service operation 'EmailDNISList' (returns error code or SOAP_OK)
int CsaWebServer::EmailDNISList(ns1__GenRequestType *ns1__RequestEmailDNISList, ns1__DNISListReturnType &ns1__ReturnEmailDNISList)
{ unsigned int error;   //PVL_STAT
	DNISRecordReply ReplyObj;

	csa_web_serverMT_log_i << "Invoke CsaWebServer::EmailDNISList \n" << LOG_TERMINATOR;
	//Fool protector
	if(ns1__RequestEmailDNISList == NULL)
	{
		csa_web_serverMT_log_i << "soap_receiver_fault Request Data. CsaWebServer::EmailDNISList \n" << LOG_TERMINATOR;
		return soap_receiver_fault(this, "No Request Data in ns1__RequestEmailDNISList", "ns1__RequestEmailDNISList is NULL");
	}


	//start connection
    //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT

	error= ReplyObj.GetHandleDNISList(this,ns1__RequestEmailDNISList,ns1__ReturnEmailDNISList,true); //Email
	//m_db_data_ptr->TraceTab.EndConnection(this->ip,error);//PVL_STAT
	csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::EmailDNISList \n" << LOG_TERMINATOR;
	return SOAP_OK;
}
//--------------------------------------------------------------------------------------------------------------
/// Web service operation 'BriefVoiceDNIS' (returns error code or SOAP_OK)
int CsaWebServer::BriefVoiceDNIS(ns1__BriefInfoRequestType *ns1__RequestBriefVoiceDNISMatrix, ns1__ReturnMatrixDataTypeBG &ns1__ReturnBriefVoiceDNISMatrix)
{
	CsaRc_t         rc = ARC_OK;
	DNISRecordReply ReplyObj;
	CsaDNIS_Record* drec;

	csa_web_serverMT_log_i << "Invoke CsaWebServer::BriefVoiceDNIS \n" << LOG_TERMINATOR;
	//Fool protector
	if(ns1__RequestBriefVoiceDNISMatrix == NULL)
	{
		csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::BriefVoiceDNIS \n" << LOG_TERMINATOR;
		return soap_receiver_fault(this, "No Request Data in ns1__RequestBriefVoiceDNISMatrix", "ns1__RequestBriefVoiceDNISMatrix is NULL");
	}


	//start connection
    //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
    ns1__ReturnBriefVoiceDNISMatrix.responseInfoHeader = m_net_data_ptr->ResponseHeader;
	ns1__ReturnBriefVoiceDNISMatrix.responseInfoHeader->errorCause = 0;
	ns1__ReturnBriefVoiceDNISMatrix.responseInfoHeader->serviceReqId = ns1__RequestBriefVoiceDNISMatrix->requestInfoHeader->serviceReqId;
  //====================================
   //printf("  +++ time: %d, check if BriefVoiceDNIS. already in process report_code: [%d]  thid: [%d]\n",time(0), do_reports_area[BriefAgents_E], this->ThreadIndex);  fflush(stdout);
   if (do_reports_area[BriefVoiceDNIS_E] == 1) // already process brief agent report in another thread
   {
     //12-Oct-2021 YR BZ#55296
     ns1__ReturnBriefVoiceDNISMatrix.responseInfoHeader->errorCause = (unsigned int)400;//MT SERVER BUSY
     //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
     csa_web_serverMT_log_i << "errorCause = 400. SERVER BUSY BriefVoiceDNIS \n" << LOG_TERMINATOR;
     printf("+++ errorCause = 400. SERVER BUSY CsaWebServer::BriefVoiceDNIS \n");  fflush(stdout);
     return SOAP_OK;
   }
   do_reports_area[BriefVoiceDNIS_E] = 1;
   m_ReportCode = BriefVoiceDNIS_E;
   //=====================================
	//if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestBriefVoiceDNISMatrix->requestInfoHeader->ticket))
	//{
	//	ns1__ReturnBriefVoiceDNISMatrix->responseInfoHeader->errorCause = 500;
	//	//m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
	//	csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::BriefVoiceDNIS \n" << LOG_TERMINATOR;
	//	return SOAP_OK;
	//}
   if(!this->m_db_data_ptr->dnistab.csa_web_TryLock())	//MT
   {
    ns1__ReturnBriefVoiceDNISMatrix.responseInfoHeader->errorCause = (unsigned int)700;//MT SERVER BUSY
	//m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT//MT SERVER BUSY
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::BriefVoiceDNIS \n" << LOG_TERMINATOR;
	return SOAP_OK;
   }
	// cycle through records
	rc = Csa_Dnis_Tbls[TBL_ONLINE]->getFirstDNIS(&drec);
	if (rc == ARC_OK) {
	while ( drec ) {
			if(	drec->Id != 99999){
				if(ReplyObj.IsDNIS(this,drec,NOT_EMAIL_DNIS))
				{
					ReplyObj.CreateNewRow(this); //Start Row
					ReplyObj.InsertBriefVoiceDNISDataToRow(this,drec);
					ReplyObj.InsertRowToMatrix(this,ns1__ReturnBriefVoiceDNISMatrix);//End Row
				}
				Csa_Dnis_Tbls[TBL_ONLINE]->getNextDNIS(&drec);
			}
			else {
				 this->m_db_data_ptr->dnistab.csa_web_UnLock();//MT
                 return SOAP_OK;
			}
		}
	}
	else
	{
       ns1__ReturnBriefVoiceDNISMatrix.responseInfoHeader->errorCause =(unsigned int)1;
	}
	this->m_db_data_ptr->dnistab.csa_web_UnLock();//MT
	//m_db_data_ptr->TraceTab.EndConnection(this->ip, ns1__ReturnBriefVoiceDNISMatrix->responseInfoHeader->errorCause);//PVL_STAT
	csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::BriefVoiceDNIS \n" << LOG_TERMINATOR;
	return SOAP_OK;
}
//--------------------------------------------------------------------------------------------------------------
/// Web service operation 'BriefEmailDNIS' (returns error code or SOAP_OK)
int CsaWebServer::BriefEmailDNIS(ns1__BriefInfoRequestType *ns1__RequestBriefEMailDNISMatrix, ns1__ReturnMatrixDataTypeBG &ns1__ReturnBriefEMailDNISMatrix)
{
    CsaRc_t         rc = ARC_OK;
    DNISRecordReply ReplyObj;
	CsaDNIS_Record* drec;

	csa_web_serverMT_log_i << "Invoke CsaWebServer::BriefEmailDNIS \n" << LOG_TERMINATOR;
	//Fool protector
	if(ns1__RequestBriefEMailDNISMatrix == NULL)
	{
		csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::BriefEmailDNIS \n" << LOG_TERMINATOR;
		return soap_receiver_fault(this, "No Request Data in ns1__RequestBriefEMailDNISMatrix", "ns1__RequestBriefEMailDNISMatrix is NULL");
	}


	//start connection
    //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
    ns1__ReturnBriefEMailDNISMatrix.responseInfoHeader = m_net_data_ptr->ResponseHeader;
	ns1__ReturnBriefEMailDNISMatrix.responseInfoHeader->errorCause = 0;
	ns1__ReturnBriefEMailDNISMatrix.responseInfoHeader->serviceReqId = ns1__RequestBriefEMailDNISMatrix->requestInfoHeader->serviceReqId;

	if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestBriefEMailDNISMatrix->requestInfoHeader->ticket))
	{
		ns1__ReturnBriefEMailDNISMatrix.responseInfoHeader->errorCause = 500;
		//m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
		csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::BriefEmailDNIS \n" << LOG_TERMINATOR;
		return SOAP_OK;
	}

   if(!this->m_db_data_ptr->dnistab.csa_web_TryLock())	//MT
   {
    ns1__ReturnBriefEMailDNISMatrix.responseInfoHeader->errorCause = (unsigned int)700;//MT SERVER BUSY
	//m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT//MT SERVER BUSY
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::BriefEmailDNIS \n" << LOG_TERMINATOR;
	return SOAP_OK;
   }

	// cycle through records
	rc= Csa_Dnis_Tbls[TBL_ONLINE]->getFirstDNIS(&drec);
	if (rc == ARC_OK) {
		while ( drec ) {
			if(	drec->Id != 99999){
				if(ReplyObj.IsDNIS(this,drec,EMAIL_DNIS)){
					ReplyObj.CreateNewRow(this); //Start Row
					ReplyObj.InsertBriefEmailDNISDataToRow(this,drec);
					ReplyObj.InsertRowToMatrix(this,ns1__ReturnBriefEMailDNISMatrix);//End Row
					}
				Csa_Dnis_Tbls[TBL_ONLINE]->getNextDNIS(&drec);
			}
			else{
				this->m_db_data_ptr->dnistab.csa_web_UnLock();//MT
				csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::BriefEmailDNIS \n" << LOG_TERMINATOR;
				return SOAP_OK;
			}
		}
	}
	else
	{
         ns1__ReturnBriefEMailDNISMatrix.responseInfoHeader->errorCause = (unsigned int)1;
	}
	this->m_db_data_ptr->dnistab.csa_web_UnLock();//MT
	//m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__ReturnBriefEMailDNISMatrix->responseInfoHeader->errorCause);//PVL_STAT
	csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::BriefEmailDNIS \n" << LOG_TERMINATOR;
	return SOAP_OK;
}

//------------------------------------------------------------------------------------
/// Web service operation 'IvrApplInfo' (returns error code or SOAP_OK)
int CsaWebServer::IvrApplInfo(ns1__GenInfoRequestType *ns1__RequestIvrApplInfo, ns1__ReturnArrayDataType &ns1__ReturnIvrApplInfo)
{
  IVRApplicationReply ReplyObj;
  CsaRc_t             rc = ARC_OK;
  CsaRc_t             rc2 = ARC_OK; //PVL 14-06-2010 1004180
  unsigned int ivrapp_id;  // PVL_WEB 3-8-2009
  IvrAppRecord_c	*ivrapp;
  unsigned int ret_index=0; //PVL 14-06-2010 1004180

  csa_web_serverMT_log_i << "Invoke CsaWebServer::IvrApplInfo \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__RequestIvrApplInfo == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::IvrApplInfo \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__RequestIvrApplInfo", "ns1__RequestIvrApplInfo is NULL");
  }

  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__ReturnIvrApplInfo.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__ReturnIvrApplInfo.responseInfoHeader->errorCause = (unsigned int)0;
  ns1__ReturnIvrApplInfo.responseInfoHeader->serviceReqId = ns1__RequestIvrApplInfo->requestInfoHeader->serviceReqId;

  if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestIvrApplInfo->requestInfoHeader->ticket))
  {
    ns1__ReturnIvrApplInfo.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::IvrApplInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  if(!m_db_data_ptr->ivrapptab.csa_web_TryLock()) //MT
  {
    ns1__ReturnIvrApplInfo.responseInfoHeader->errorCause = (unsigned int)700;//MT SERVER BUSY
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT//MT SERVER BUSY
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::IvrApplInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  //start PVL 14-06-2010 1004180

  for (unsigned int i=0; i<ns1__RequestIvrApplInfo->Id.size(); i++)
  {

    ivrapp_id = ns1__RequestIvrApplInfo->Id[i];
    ReplyObj.StartMonitor(ivrapp_id);
  }
  //end PVL 14-06-2010 1004180

  for (unsigned int i=0; i<ns1__RequestIvrApplInfo->Id.size(); i++)
  {
    rc = IvrAppMngr_c::GetTable().Get(ns1__RequestIvrApplInfo->Id[i],ivrapp);
    if (rc == ARC_OK)
    {
      ReplyObj.AddNewIvrAppBlock(this,ivrapp->m_app_id,ns1__ReturnIvrApplInfo);
      ns1__ReturnIvrApplInfo.responseInfoHeader->errorCause = (unsigned int)0;

      rc2=ReplyObj.MakeIvrAppInfo (this,ivrapp,ns1__ReturnIvrApplInfo.returnArrayOfBlocks[ret_index]); //PVL 14-06-2010 1004180
      if (rc2 != ARC_OK)
      {
        ReplyObj.MakeIvrAppInfoEMPTY (this,ivrapp->m_app_id ,ns1__ReturnIvrApplInfo.returnArrayOfBlocks[ret_index]);  //PVL 14-06-2010 1004180
        ns1__ReturnIvrApplInfo.responseInfoHeader->errorCause = (unsigned int)2;
      }
      ret_index++; //PVL 14-06-2010 1004180
    }
    else
    {
      ns1__ReturnIvrApplInfo.responseInfoHeader->errorCause = (unsigned int)1;
    }
  }

  m_db_data_ptr->ivrapptab.csa_web_UnLock();//MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__ReturnIvrApplInfo->responseInfoHeader->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::IvrApplInfo \n" << LOG_TERMINATOR;

  return SOAP_OK;
}


//------------------------------------------------------------------------------------
/// Web service operation 'IvrPortInfo' (returns error code or SOAP_OK)
int CsaWebServer::IvrPortInfo(ns1__GenInfoRequestType *ns1__RequestIvrPortInfo, ns1__ReturnArrayDataType &ns1__ReturnIvrPortInfo)
{
  IVRGrpPortReply       ReplyObj;
  CsaRc_t               rc1 = ARC_OK;
  CsaRc_t               rc = ARC_OK;
  CsaIvrPortRecord_c*   prec;
  unsigned int          ret_index=0; //PVL 14-06-2010 1004180 not valid index problem
  CsaIvrGroupRecord_c*  group_ptr; //PVL 20-06-2010 1004048

  csa_web_serverMT_log_i << "Invoke CsaWebServer::IvrPortInfo \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__RequestIvrPortInfo == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::IvrPortInfo \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__RequestIvrPortInfo", "ns1__RequestIvrPortInfo is NULL");
  }

  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT

  ns1__ReturnIvrPortInfo.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__ReturnIvrPortInfo.responseInfoHeader->errorCause = (unsigned int)1;
  ns1__ReturnIvrPortInfo.responseInfoHeader->serviceReqId = ns1__RequestIvrPortInfo->requestInfoHeader->serviceReqId;

  if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestIvrPortInfo->requestInfoHeader->ticket))
  {
    ns1__ReturnIvrPortInfo.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::IvrPortInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  if(!m_db_data_ptr->ivrgrptab.csa_web_TryLock())	//MT
  {
    ns1__ReturnIvrPortInfo.responseInfoHeader->errorCause = (unsigned int)700;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT//MT SERVER BUSY
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::IvrPortInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  for (unsigned int i=0; i<ns1__RequestIvrPortInfo->Id.size(); i++) //PVL 20-06-2010 1004048
    ReplyObj.StartMonitorPort(ns1__RequestIvrPortInfo->Id[i]);

  for (unsigned int i=0; i<ns1__RequestIvrPortInfo->Id.size(); i++)
  {
    rc1 = MainIvrTable_c::GetMainIvrTable()->GetOnlineGroupsTable()->get(ns1__RequestIvrPortInfo->Id[i],&group_ptr);//PVL 20-06-2010 1004048
    if (rc1 == ARC_OK) //PVL 20-06-2010 1004048
    {
      for( rc = MainIvrTable_c::GetMainIvrTable()->getFirstForGroupOnline(ns1__RequestIvrPortInfo->Id[i],&prec);
           rc == ARC_OK;
           rc = MainIvrTable_c::GetMainIvrTable()->getNextForGroupOnline(&prec) )
      {
        ReplyObj.AddNewIvrGrpBlock(this,ns1__RequestIvrPortInfo->Id[i],ns1__ReturnIvrPortInfo);//PVL 20-06-2010 1004048
        ns1__ReturnIvrPortInfo.responseInfoHeader->errorCause = (unsigned int)0;
        ReplyObj.MakeIvrGrpPortInfo(this,prec,ns1__ReturnIvrPortInfo.returnArrayOfBlocks[ret_index]);//PVL 14-06-2010 1004180 not valid index problem
        ret_index++; //PVL 14-06-2010 1004180 not valid index problem
      }
    }
    else //PVL 20-06-2010 1004048
    {
      ns1__ReturnIvrPortInfo.responseInfoHeader->errorCause = (unsigned int)1;
    }
  }

  this->m_db_data_ptr->ivrgrptab.csa_web_UnLock(); //MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__ReturnIvrPortInfo->responseInfoHeader->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::IvrPortInfo \n" << LOG_TERMINATOR;

  return SOAP_OK;
}


int CsaWebServer::IvrGrpInfo(ns1__GenInfoRequestType *ns1__RequestIvrGrpInfo, ns1__ReturnArrayDataType &ns1__ReturnIvrGrpInfo)
{
  IVRGrpPortReply       ReplyObj;
  CsaRc_t               rc = ARC_OK;
  CsaIvrGroupRecord_c*  ivr_grp_rec = NULL;
  unsigned int          ret_index = 0; //PVL 14-06-2010 1004180 not valid index problem

  csa_web_serverMT_log_i << "Invoke CsaWebServer::IvrGrpInfo \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__RequestIvrGrpInfo == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::IvrGrpInfo \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__RequestIvrGrpInfo", "ns1__RequestIvrGrpInfo is NULL");
  }

  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT

  ns1__ReturnIvrGrpInfo.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__ReturnIvrGrpInfo.responseInfoHeader->errorCause = (unsigned int)1;
  ns1__ReturnIvrGrpInfo.responseInfoHeader->serviceReqId = ns1__RequestIvrGrpInfo->requestInfoHeader->serviceReqId;

  if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestIvrGrpInfo->requestInfoHeader->ticket))
  {
    ns1__ReturnIvrGrpInfo.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::IvrGrpInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  if(!m_db_data_ptr->ivrgrptab.csa_web_TryLock()) //MT
  {
    ns1__ReturnIvrGrpInfo.responseInfoHeader->errorCause = (unsigned int)700;//MT SERVER BUSY
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT//MT SERVER BUSY
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::IvrGrpInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  for (unsigned int i=0; i<ns1__RequestIvrGrpInfo->Id.size(); i++)
  {
    ReplyObj.StartMonitorGroup(ns1__RequestIvrGrpInfo->Id[i]); //PVL_WEB 3-8-2009 ivr_group_id
    rc = MainIvrTable_c::GetMainIvrTable()->GetOnlineGroupsTable()->get(ns1__RequestIvrGrpInfo->Id[i],&ivr_grp_rec);
    if (rc == ARC_OK)
    {
      ns1__ReturnIvrGrpInfo.responseInfoHeader->errorCause = (unsigned int)0;
      ReplyObj.AddNewIvrGrpBlock(this,ivr_grp_rec->m_ivr_port_grp_id,ns1__ReturnIvrGrpInfo);
      ReplyObj.MakeIvrGrpInfo(this,ivr_grp_rec,ns1__ReturnIvrGrpInfo.returnArrayOfBlocks[ret_index]);//PVL 14-06-2010 1004180 not valid index problem
      ret_index++; //PVL 14-06-2010 1004180 not valid index problem
    }
  }

  this->m_db_data_ptr->ivrgrptab.csa_web_UnLock(); //MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__ReturnIvrGrpInfo->responseInfoHeader->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::IvrGrpInfo \n" << LOG_TERMINATOR;

  return SOAP_OK;
}




//-------------------------------------------------------------------------------------------------
// Web service operation 'IvrPortGroupList' (returns error code or SOAP_OK)
int CsaWebServer::IvrPortGroupList(ns1__GenRequestType *ns1__RequestIvrPortGroupList, ns1__GenListReturnType &ns1__ReturnIvrPortGroupList)
{
	IVRGrpPortReply ReplyObj;

	csa_web_serverMT_log_i << "Invoke CsaWebServer::IvrPortGroupList \n" << LOG_TERMINATOR;
	//Fool protector
	if(ns1__RequestIvrPortGroupList == NULL)
	{
		csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::IvrPortGroupList \n" << LOG_TERMINATOR;
		return soap_receiver_fault(this, "No Request Data in ns1__RequestIvrPortGroupList", "ns1__RequestIvrPortGroupList is NULL");
	}

	//start connection
    //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT

    ns1__ReturnIvrPortGroupList.responseInfoHeader = m_net_data_ptr->ResponseHeader;
	ns1__ReturnIvrPortGroupList.responseInfoHeader->errorCause = 1;
	ns1__ReturnIvrPortGroupList.responseInfoHeader->serviceReqId = ns1__RequestIvrPortGroupList->requestInfoHeader->serviceReqId;

  //====================================
   //printf("  +++ time: %d, check if IvrPortGroupList. already in process report_code: [%d]  thid:[%d]\n",time(0), do_reports_area[BriefAgents_E], this->ThreadIndex);  fflush(stdout);
    if (do_reports_area[IvrPortGroupList_E] == 1) // already process brief agent report in another thread
    {
      //12-Oct-2021 YR BZ#55296
        ns1__ReturnIvrPortGroupList.responseInfoHeader->errorCause = (unsigned int)400;//MT SERVER BUSY
        //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
        csa_web_serverMT_log_i << "errorCause = 400. SERVER BUSY IvrPortGroupList \n" << LOG_TERMINATOR;
        printf("+++ errorCause = 400. SERVER BUSY CsaWebServer::IvrPortGroupList \n");  fflush(stdout);
        return SOAP_OK;
    }
    do_reports_area[IvrPortGroupList_E] = 1;
    m_ReportCode = IvrPortGroupList_E;
   
	//if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestIvrPortGroupList->requestInfoHeader->ticket))
	//{
	//	ns1__ReturnIvrPortGroupList->responseInfoHeader->errorCause = 500;
    //       //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
	//	csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::IvrPortGroupList \n" << LOG_TERMINATOR;
	//	return SOAP_OK;
	//}
	if(!m_db_data_ptr->ivrgrptab.csa_web_TryLock())	//MT
	{
	    ns1__ReturnIvrPortGroupList.responseInfoHeader->errorCause = (unsigned int)700;//MT SERVER BUSY
        //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT//MT SERVER BUSY
	    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::IvrPortGroupList \n" << LOG_TERMINATOR;
	    return SOAP_OK;
	}
	this->m_db_data_ptr->ivrgrptab.LoadList();

	for ( this->m_db_data_ptr->ivrgrptab.it=this->m_db_data_ptr->ivrgrptab.tab.begin() ;
		   this->m_db_data_ptr->ivrgrptab.it != this->m_db_data_ptr->ivrgrptab.tab.end();
		   this->m_db_data_ptr->ivrgrptab.it++ )
	 {
		 ns1__ReturnIvrPortGroupList.responseInfoHeader->errorCause = 0;
		 ReplyObj.InsertIVRGrpPortList(this,
			              this->m_db_data_ptr->ivrgrptab.it->first,
						  this->m_db_data_ptr->ivrgrptab.it->second,
			              ns1__ReturnIvrPortGroupList);
	 }
	 this->m_db_data_ptr->ivrgrptab.csa_web_UnLock(); //MT
	 //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__ReturnIvrPortGroupList->responseInfoHeader->errorCause);//PVL_STAT
	 csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::IvrPortGroupList \n" << LOG_TERMINATOR;
	 return SOAP_OK;
}


//-------------------------------------------------------------------------------------------------
// Web service operation 'IvrAppList' (returns error code or SOAP_OK)
int CsaWebServer::IvrAppList(ns1__GenRequestType *ns1__RequestIvrAppList, ns1__GenListReturnType &ns1__ReturnIvrAppList)
{
  IVRApplicationReply ReplyObj;

  csa_web_serverMT_log_i << "Invoke CsaWebServer::IvrAppList \n" << LOG_TERMINATOR;

  //Fool protector
  if(ns1__RequestIvrAppList == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::IvrAppList \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__RequestIvrAppList", "ns1__RequestIvrAppList is NULL");
  }


  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__ReturnIvrAppList.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__ReturnIvrAppList.responseInfoHeader->errorCause = 1;
  ns1__ReturnIvrAppList.responseInfoHeader->serviceReqId = ns1__RequestIvrAppList->requestInfoHeader->serviceReqId;
  //====================================

  //printf("  +++ time: %d, check if IvrAppList. already in process report_code: [%d]  thid:[%d]\n",time(0), do_reports_area[BriefAgents_E], this->ThreadIndex);  fflush(stdout);
  if (do_reports_area[IvrAppList_E] == 1) // already process brief agent report in another thread
  {
    //12-Oct-2021 YR BZ#55296
    ns1__ReturnIvrAppList.responseInfoHeader->errorCause = (unsigned int)400;//MT SERVER BUSY
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 400. SERVER BUSY IvrAppList \n" << LOG_TERMINATOR;
    printf("+++ errorCause = 400. SERVER BUSY CsaWebServer::IvrAppList \n");  fflush(stdout);
    return SOAP_OK;
  }
  do_reports_area[IvrAppList_E] = 1;
  m_ReportCode = IvrAppList_E;
  //=====================================

  //if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestIvrAppList->requestInfoHeader->ticket))
  //{
  //	ns1__ReturnIvrAppList->responseInfoHeader->errorCause = 500;
  //	//m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
  //	csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::IvrAppList \n" << LOG_TERMINATOR;
  //	return SOAP_OK;
  //}
  if(!m_db_data_ptr->ivrapptab.csa_web_TryLock())	//MT
  {
    ns1__ReturnIvrAppList.responseInfoHeader->errorCause = (unsigned int)700;//MT SERVER BUSY
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT//MT SERVER BUSY
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::IvrAppList \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  this->m_db_data_ptr->ivrapptab.LoadList();

  for ( this->m_db_data_ptr->ivrapptab.it=this->m_db_data_ptr->ivrapptab.tab.begin() ;
        this->m_db_data_ptr->ivrapptab.it != this->m_db_data_ptr->ivrapptab.tab.end();
        this->m_db_data_ptr->ivrapptab.it++ )
  {
    ns1__ReturnIvrAppList.responseInfoHeader->errorCause = 0;
    InsertGenListDataItemType(this->m_db_data_ptr->ivrapptab.it->first,this->m_db_data_ptr->ivrapptab.it->second,ns1__ReturnIvrAppList);
  }

  m_db_data_ptr->ivrapptab.csa_web_UnLock();//MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__ReturnIvrAppList->responseInfoHeader->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::IvrAppList \n" << LOG_TERMINATOR;
  return SOAP_OK;
}


//////////////////
int CsaWebServer::MailboxInfo(ns1__GenInfoRequestType *ns1__RequestMailboxInfo, ns1__ReturnArrayDataType &ns1__ReturnMailboxInfo)
{
  MAILbox_Reply ReplyObj;
  CsaRc_t       rc = ARC_OK;
  CsaRc_t       rc2 = ARC_OK; //PVL 14-06-2010 1004180
  MailboxRecord *mailbox_rec = NULL;
  unsigned int  ret_index=0; //PVL 14-06-2010 1004180 mailbox

  csa_web_serverMT_log_i << "Invoke CsaWebServer::MailboxInfo \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__RequestMailboxInfo == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::IvrAppList \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__RequestMailboxInfo", "ns1__RequestMailboxInfo is NULL");
  }

  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT

  ns1__ReturnMailboxInfo.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__ReturnMailboxInfo.responseInfoHeader->errorCause = (unsigned int)1;
  ns1__ReturnMailboxInfo.responseInfoHeader->serviceReqId = ns1__RequestMailboxInfo->requestInfoHeader->serviceReqId;

  if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestMailboxInfo->requestInfoHeader->ticket))
  {
    ns1__ReturnMailboxInfo.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::IvrAppList \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  if(!m_db_data_ptr->mailboxacctab.csa_web_TryLock())	//MT
  {
    ns1__ReturnMailboxInfo.responseInfoHeader->errorCause = (unsigned int)700;//MT SERVER BUSY
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT//MT SERVER BUSY
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::IvrAppList \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  //start PVL 14-06-2010 1004180 mailbox
  for (unsigned int i=0; i<ns1__RequestMailboxInfo->Id.size(); i++)
  {
    ReplyObj.StartMonitor(ns1__RequestMailboxInfo->Id[i]);
  }
  //end PVL 14-06-2010 1004180 mailbox

  for (unsigned int i=0; i<ns1__RequestMailboxInfo->Id.size(); i++)
  {
    // ReplyObj.StartMonitor(ns1__RequestMailboxInfo->Id[i]);//PVL_WEB 3-8-2009 mailbox_id
    rc = MainMailboxTable::getMailboxTable().GetMailbox(MailboxesTable::MB_TBL_ONLINE,ns1__RequestMailboxInfo->Id[i],&mailbox_rec);
    if (rc == ARC_OK)
    {
      ReplyObj.AddNewMailAccBlock(this,mailbox_rec->Mailbox_id,ns1__ReturnMailboxInfo);
      ns1__ReturnMailboxInfo.responseInfoHeader->errorCause = (unsigned int)0;
      rc2= ReplyObj.MakeMailboxInfo(this,mailbox_rec,ns1__ReturnMailboxInfo.returnArrayOfBlocks[ret_index]);//PVL 14-06-2010 1004180
      if (rc2 != ARC_OK)
      {
        ReplyObj.MakeMailboxInfoEMPTY (this,mailbox_rec->Mailbox_id ,ns1__ReturnMailboxInfo.returnArrayOfBlocks[ret_index]);  //PVL 14-06-2010 1004180
        ns1__ReturnMailboxInfo.responseInfoHeader->errorCause = (unsigned int)2;
      }
      ret_index++; //PVL 14-06-2010 1004180
    }
    else
    {
      ns1__ReturnMailboxInfo.responseInfoHeader->errorCause = (unsigned int)1;
    }
  }

  this->m_db_data_ptr->mailboxacctab.csa_web_UnLock(); //MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__ReturnMailboxInfo->responseInfoHeader->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::IvrAppList \n" << LOG_TERMINATOR;

  return SOAP_OK;
}


/// Web service operation 'SuperGroupInfo' (returns error code or SOAP_OK)
int CsaWebServer::SuperGroupInfo(ns1__GenInfoRequestType *ns1__RequestSuperGroupInfo, ns1__ReturnArrayDataType &ns1__ReturnSuperGroupInfo)
{
  SuperGroup_Reply  ReplyObj;
  CsaRc_t           rc = ARC_OK;
  SuperGroupRecord* sg_rec_arr_ptr = NULL;
  //GenId_t           sgrp_id;
  unsigned int      ret_index = 0;

  csa_web_serverMT_log_i << "Invoke CsaWebServer::SuperGroupInfo \n" << LOG_TERMINATOR;
  //Full protector
  if(ns1__RequestSuperGroupInfo == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::SuperGroupInfo \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__RequestSuperGroupInfo", "ns1__RequestSuperGroupInfo is NULL");
  }

  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT

  ns1__ReturnSuperGroupInfo.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__ReturnSuperGroupInfo.responseInfoHeader->errorCause = (unsigned int)1;
  ns1__ReturnSuperGroupInfo.responseInfoHeader->serviceReqId = ns1__RequestSuperGroupInfo->requestInfoHeader->serviceReqId;

  csa_web_serverMT_log_i << "CsaWebServer::SuperGroupInfo - VerifyTicket ... - ";
  if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestSuperGroupInfo->requestInfoHeader->ticket))
  {
    ns1__ReturnSuperGroupInfo.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "FAIL, errorCause = 500" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  csa_web_serverMT_log_i << "OK" << LOG_TERMINATOR;

  csa_web_serverMT_log_i << "CsaWebServer::SuperGroupInfo - csa_web_TryLock on supergrouptab ... - ";
  if(!m_db_data_ptr->supergrouptab.csa_web_TryLock())	//MT
  {
    ns1__ReturnSuperGroupInfo.responseInfoHeader->errorCause = (unsigned int)700;//MT SERVER BUSY
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT//MT SERVER BUSY
    csa_web_serverMT_log_i << "FAIL, errorCause = 700" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  csa_web_serverMT_log_i << "OK" << LOG_TERMINATOR;

  csa_web_serverMT_log_i << "ns1__RequestSuperGroupInfo->Id.size() " << ns1__RequestSuperGroupInfo->Id.size() << "\n SG Ids: ";
  //start PVL 14-06-2010 1004180 mailbox
  for (unsigned int i=0; i<ns1__RequestSuperGroupInfo->Id.size(); i++)
  {
    if(i > 0)
      csa_web_serverMT_log_i << ",";
    csa_web_serverMT_log_i << ns1__RequestSuperGroupInfo->Id[i];
    ReplyObj.StartMonitor(ns1__RequestSuperGroupInfo->Id[i]);
  }
  csa_web_serverMT_log_i << LOG_TERMINATOR;

  csa_web_serverMT_log_i << "Start Iterating - ns1__RequestSuperGroupInfo->Id" << LOG_TERMINATOR;

  sg_rec_arr_ptr = new SuperGroupRecord[ns1__RequestSuperGroupInfo->Id.size()];
  ns1__ReturnSuperGroupInfo.responseInfoHeader->errorCause = (unsigned int)0;

  //9-Aug-2022 YR BZ#56641
  char servers_info[256];
  memset(servers_info, 0, 256);
  AddServerTimeToServersInfo(servers_info);
  ns1__ReturnSuperGroupInfo.responseInfoHeader->serversInfo = servers_info;

  for( unsigned int i=0; i<ns1__RequestSuperGroupInfo->Id.size(); i++ )
  {
    if(ReplyObj.AddNewSuperGroupBlock(this,ns1__RequestSuperGroupInfo->Id[i],ns1__ReturnSuperGroupInfo))
    {
      sg_rec_arr_ptr[i].setSuperGroupId(ns1__RequestSuperGroupInfo->Id[i]);
      sg_rec_arr_ptr[i].Clear();

      rc = ReplyObj.MakeSuperGroupInfo(this,&sg_rec_arr_ptr[i],ns1__ReturnSuperGroupInfo.returnArrayOfBlocks[ret_index]);
      if (rc != ARC_OK)
      {
        ReplyObj.MakeSuperGroupInfoEMPTY(this,sg_rec_arr_ptr[i].getSuperGroupId(),ns1__ReturnSuperGroupInfo.returnArrayOfBlocks[ret_index]);  //PVL 14-06-2010 1004180
        ns1__ReturnSuperGroupInfo.responseInfoHeader->errorCause = (unsigned int)2;
      }
      ret_index++;
    }
  }

  delete []sg_rec_arr_ptr;

  this->m_db_data_ptr->supergrouptab.csa_web_UnLock(); //MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__ReturnMailboxInfo->responseInfoHeader->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::SuperGroupInfo \n" << LOG_TERMINATOR;

  return SOAP_OK;
}


/// Web service operation 'SuperGroupList' (returns error code or SOAP_OK)
int CsaWebServer::SuperGroupList(ns1__GenRequestType *ns1__RequestSuperGroupList, ns1__SuperGroupListReturnType &ns1__ReturnSuperGroupList)
{
  SuperGroup_Reply  ReplyObj;
  sup_data_t*       tmp_sup_data;

  csa_web_serverMT_log_i << "Invoke CsaWebServer::SuperGroupList \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__RequestSuperGroupList == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::SuperGroupList \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__RequestSuperGroupList", "ns1__RequestSuperGroupList is NULL");
  }

  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT

  ns1__ReturnSuperGroupList.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__ReturnSuperGroupList.responseInfoHeader->errorCause = 1;
  ns1__ReturnSuperGroupList.responseInfoHeader->serviceReqId = ns1__RequestSuperGroupList->requestInfoHeader->serviceReqId;

  //20-Dec-2020 YR BZ#53247
  if(!m_db_data_ptr->TicketTab.VerifyTicketGetData(ns1__RequestSuperGroupList->requestInfoHeader->ticket, &tmp_sup_data))
  {
    ns1__ReturnSuperGroupList.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::SuperGroupList \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  if(!m_db_data_ptr->supergrouptab.csa_web_TryLock())	//MT
  {
    ns1__ReturnSuperGroupList.responseInfoHeader->errorCause = (unsigned int)700;//MT SERVER BUSY
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT//MT SERVER BUSY
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::SuperGroupList \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  this->m_db_data_ptr->supergrouptab.LoadList(tmp_sup_data->user_id, tmp_sup_data->sup_level);

  for ( m_db_data_ptr->supergrouptab.it = m_db_data_ptr->supergrouptab.tab.begin() ;
        m_db_data_ptr->supergrouptab.it != m_db_data_ptr->supergrouptab.tab.end();
        m_db_data_ptr->supergrouptab.it++ )
  {
    ns1__ReturnSuperGroupList.responseInfoHeader->errorCause = 0;
    this->m_db_data_ptr->supergrouptab.LoadGroups(m_db_data_ptr->supergrouptab.it->first);
    ReplyObj.InsertSuperGroupList(this,
                                  m_db_data_ptr->supergrouptab.it->first,
                                  m_db_data_ptr->supergrouptab.it->second.getSGroupName(),
                                  m_db_data_ptr->supergrouptab.it->second.getGroupList(),
                                  ns1__ReturnSuperGroupList);
  }

  this->m_db_data_ptr->supergrouptab.csa_web_UnLock(); //MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__ReturnMailAccList->responseInfoHeader->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK CsaWebServer::SuperGroupList \n" << LOG_TERMINATOR;
  return SOAP_OK;
}


/// Web service operation 'MailAccList' (returns error code or SOAP_OK)
int CsaWebServer::MailAccList(ns1__GenRequestType *ns1__RequestMailAccList, ns1__GenListReturnType &ns1__ReturnMailAccList)
{
	MAILbox_Reply ReplyObj;

    csa_web_serverMT_log_i << "Invoke CsaWebServer::MailAccList \n" << LOG_TERMINATOR;
	//Fool protector
	if(ns1__RequestMailAccList == NULL)
	{
		csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::MailAccList \n" << LOG_TERMINATOR;
		return soap_receiver_fault(this, "No Request Data in ns1__RequestMailAccList", "ns1__RequestMailAccList is NULL");
	}


	//start connection
    //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT

    ns1__ReturnMailAccList.responseInfoHeader = m_net_data_ptr->ResponseHeader;
	ns1__ReturnMailAccList.responseInfoHeader->errorCause = 1;
	ns1__ReturnMailAccList.responseInfoHeader->serviceReqId = ns1__RequestMailAccList->requestInfoHeader->serviceReqId;

	if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestMailAccList->requestInfoHeader->ticket))
	{
		ns1__ReturnMailAccList.responseInfoHeader->errorCause = 500;
		//m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
		csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::MailAccList \n" << LOG_TERMINATOR;
		return SOAP_OK;
	}

	if(!m_db_data_ptr->mailboxacctab.csa_web_TryLock())	//MT
	{
 		ns1__ReturnMailAccList.responseInfoHeader->errorCause = (unsigned int)700;//MT SERVER BUSY
		//m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT//MT SERVER BUSY
		csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::MailAccList \n" << LOG_TERMINATOR;
		return SOAP_OK;
	}

	this->m_db_data_ptr->mailboxacctab.LoadList();

	for ( this->m_db_data_ptr->mailboxacctab.it=this->m_db_data_ptr->mailboxacctab.tab.begin() ;
		   this->m_db_data_ptr->mailboxacctab.it != this->m_db_data_ptr->mailboxacctab.tab.end();
		   this->m_db_data_ptr->mailboxacctab.it++ )
    {
        ns1__ReturnMailAccList.responseInfoHeader->errorCause = 0;
        ReplyObj.InsertMAILboxAccList(this,
							        this->m_db_data_ptr->mailboxacctab.it->first,
							        this->m_db_data_ptr->mailboxacctab.it->second,
							        ns1__ReturnMailAccList);
    }
    this->m_db_data_ptr->mailboxacctab.csa_web_UnLock(); //MT
	//m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__ReturnMailAccList->responseInfoHeader->errorCause);//PVL_STAT
	csa_web_serverMT_log_i << "SOAP_OK CsaWebServer::MailAccList \n" << LOG_TERMINATOR;
	return SOAP_OK;
}

/// Web service operation 'CommandAgentLoginLogout' (returns error code or SOAP_OK)
int CsaWebServer::CommandAgentLoginLogout(ns1__ACDMultiRequestType *ns1__LoginLogoutRequestAcd, ns1__ResponseInfoHeaderType &ns1__NoDataResponse)
{
    csa_web_serverMT_log_i << "Invoke CsaWebServer::CommandAgentLoginLogout \n" << LOG_TERMINATOR;
	//Fool protector
	if(ns1__LoginLogoutRequestAcd == NULL)
	{
		csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::CommandAgentLoginLogout \n" << LOG_TERMINATOR;
		return soap_receiver_fault(this, "No Request Data in ns1__LoginLogoutRequestAcd", "ns1__LoginLogoutRequestAcd is NULL");
	}


	//start connection
    //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
	csa_web_serverMT_log_i << "LoginManagerOneAgentToMultiGroups() CsaWebServer::CommandAgentLoginLogout \n" << LOG_TERMINATOR;
	return LoginManagerOneAgentToMultiGroups(ns1__LoginLogoutRequestAcd,ns1__NoDataResponse);

}

/// Web service operation 'CommandAgentReleaseResume' (returns error code or SOAP_OK)
int CsaWebServer::CommandAgentReleaseResume(ns1__ACDRequestType *ns1__ReleaseResumeRequestAcd, ns1__ResponseInfoHeaderType &ns1__NoDataResponse)
{
    CsaRc_t                       rc = ARC_OK;

	csa_web_serverMT_log_i << "Invoke CsaWebServer::CommandAgentReleaseResume \n" << LOG_TERMINATOR;
	//Fool protector
	if(ns1__ReleaseResumeRequestAcd == NULL)
	{
		csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::CommandAgentReleaseResume \n" << LOG_TERMINATOR;
		return soap_receiver_fault(this, "No Request Data in ns1__ReleaseResumeRequestAcd", "ns1__ReleaseResumeRequestAcd is NULL");
	}


	//start connection
    //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
	ns1__NoDataResponse.errorCause = 0;
	ns1__NoDataResponse.serviceReqId = ns1__ReleaseResumeRequestAcd->requestInfoHeader->serviceReqId;

	if(!this->m_db_data_ptr->TicketTab.VerifyTicket(ns1__ReleaseResumeRequestAcd->requestInfoHeader->ticket))
	{
		ns1__NoDataResponse.errorCause =500;
		//m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
		csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::CommandAgentReleaseResume \n" << LOG_TERMINATOR;
		return SOAP_OK;
	}
	if(ns1__ReleaseResumeRequestAcd->Operation == "RELEASE")
	{
		NlaCsaAgentReleaseReq_t notify_data;
		notify_data.agent_db_id = ns1__ReleaseResumeRequestAcd->AgentId;
		notify_data.group_id = ns1__ReleaseResumeRequestAcd->GroupId;

		CsaUtilSendEvent(CSA_TH3,CSA_release_request,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
		//m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__NoDataResponse->errorCause);//PVL_STAT
		csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::CommandAgentReleaseResume \n" << LOG_TERMINATOR;
		return SOAP_OK;
	}
	if(ns1__ReleaseResumeRequestAcd->Operation == "RESUME")
	{
		NlaCsaAgentResumeReq_t notify_data;
		notify_data.agent_db_id = ns1__ReleaseResumeRequestAcd->AgentId;
		notify_data.group_id = ns1__ReleaseResumeRequestAcd->GroupId;

		CsaUtilSendEvent(CSA_TH3,CSA_resume_request,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
		//m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__NoDataResponse->errorCause);//PVL_STAT
        csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::CommandAgentReleaseResume \n" << LOG_TERMINATOR;
		return SOAP_OK;
	}
	else
       ns1__NoDataResponse.errorCause = 1;

    //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__NoDataResponse->errorCause);//PVL_STAT
    csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::CommandAgentReleaseResume \n" << LOG_TERMINATOR;
	return SOAP_OK;
}

/// Web service operation 'CommandAgentStartChat' (returns error code or SOAP_OK)
int CsaWebServer::CommandAgentStartChat(ns1__TFRequestType *ns1__StartChatRequestAcd, ns1__ChatResponseType &ns1__StartChatResponse)
{
    CsaRc_t rc = ARC_OK;

    csa_web_serverMT_log_i << "Invoke CsaWebServer::CommandAgentStartChat \n" << LOG_TERMINATOR;
	//Fool protector
	if(ns1__StartChatRequestAcd == NULL)
	{
		csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::CommandAgentStartChat \n" << LOG_TERMINATOR;
		return soap_receiver_fault(this, "No Request Data in ns1__StartChatRequestAcd", "ns1__StartChatRequestAcd is NULL");
	}


	//start connection
    //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
	ns1__StartChatResponse.errorCause = 0;
	ns1__StartChatResponse.serviceReqId = ns1__StartChatRequestAcd->requestInfoHeader->serviceReqId;


	sup_data_t *tmp_sup_data;

	if(!this->m_db_data_ptr->TicketTab.VerifyTicketGetData(ns1__StartChatRequestAcd->requestInfoHeader->ticket, &tmp_sup_data))
	{
		ns1__StartChatResponse.errorCause =500;
		//m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
		csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::CommandAgentStartChat \n" << LOG_TERMINATOR;
		return SOAP_OK;
	}
    if(ns1__StartChatRequestAcd->Operation == "START_CHAT")
	{
		NlaCsaAgentChatReq_t notify_data;
		notify_data.agent_db_id = ns1__StartChatRequestAcd->AgentId;
		notify_data.group_id = tmp_sup_data->agent_id; //sup user id

		char    temp[20] = "";
		ns1__StartChatResponse.sessionId += "admin";
		er_itoa(tmp_sup_data->agent_id,temp,10);
		ns1__StartChatResponse.sessionId +=temp;
		ns1__StartChatResponse.sessionId += "_";
		er_itoa(ns1__StartChatRequestAcd->AgentId,temp,10);
		ns1__StartChatResponse.sessionId +=temp;


		CsaUtilSendEvent(CSA_TH3,CSA_chat_request,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
      	//m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__StartChatResponse->errorCause);//PVL_STAT
		csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::CommandAgentStartChat \n" << LOG_TERMINATOR;
		return SOAP_OK;
	}
	else
       ns1__StartChatResponse.errorCause = 1;
	//m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__StartChatResponse->errorCause);//PVL_STAT
	csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::CommandAgentStartChat \n" << LOG_TERMINATOR;
	return SOAP_OK;
}

/// Web service operation 'CommandAgentStartRecord' (returns error code or SOAP_OK)
int CsaWebServer::CommandAgentStartRecord(ns1__TFRequestType *ns1__StartRecordRequestAcd, ns1__ResponseInfoHeaderType &ns1__NoDataResponse)
{
  CsaRc_t rc = ARC_OK;

  csa_web_serverMT_log_i << "Invoke CsaWebServer::CommandAgentStartRecord \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__StartRecordRequestAcd == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::CommandAgentStartRecord \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__StartRecordRequestAcd", "ns1__StartRecordRequestAcd is NULL");
  }

  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__NoDataResponse.errorCause = 0;
  ns1__NoDataResponse.serviceReqId = ns1__StartRecordRequestAcd->requestInfoHeader->serviceReqId;
  sup_data_t *tmp_sup_data;
  if(!this->m_db_data_ptr->TicketTab.VerifyTicketGetData(ns1__StartRecordRequestAcd->requestInfoHeader->ticket, &tmp_sup_data))
  {
    ns1__NoDataResponse.errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::CommandAgentStartRecord \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  //12-Nov-2019 YR BZ#51013
  if(ns1__StartRecordRequestAcd->Operation == "START_RECORD")
  {
    NlaCsaAgentLogoutReq_t notify_data;
    notify_data.agent_db_id = ns1__StartRecordRequestAcd->AgentId; //Agent to record

    csa_web_serverMT_log_i << "START_RECORD for Agent " << notify_data.agent_db_id << LOG_TERMINATOR;
    
    CsaUtilSendEvent(CSA_TH3,CSA_startrecord_request,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
    return SOAP_OK;
  }
  else
    ns1__NoDataResponse.errorCause = 1;

  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__NoDataResponse->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::CommandAgentStartRecord \n" << LOG_TERMINATOR;
  return SOAP_OK;
}


/// Web service operation 'CommandAgentTelephonyFeatures' (returns error code or SOAP_OK)
int CsaWebServer::CommandAgentTelephonyFeatures(ns1__TFRequestType *ns1__RequestTf, ns1__ResponseInfoHeaderType &ns1__NoDataResponse)
{
  CsaRc_t  rc = ARC_OK;
  csa_web_serverMT_log_i << "Invoke CsaWebServer::CommandAgentTelephonyFeatures \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__RequestTf == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::CommandAgentTelephonyFeatures \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__RequestTf", "ns1__RequestTf is NULL");
  }


  //12-Nov-2019 YR BZ#51014 + BZ#51015
  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__NoDataResponse.errorCause = 0;
  ns1__NoDataResponse.serviceReqId = ns1__RequestTf->requestInfoHeader->serviceReqId;
  sup_data_t *tmp_sup_data;
  if(!this->m_db_data_ptr->TicketTab.VerifyTicketGetData(ns1__RequestTf->requestInfoHeader->ticket, &tmp_sup_data))
  {
    ns1__NoDataResponse.errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::CommandAgentTelephonyFeatures \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  // 19-Nov-2018 YR BZ#28548
  RetCode_t executionResult = 700 ;
  string sup_agent_ext;
  if(!this->m_db_data_ptr->TicketTab.GetSupAgentExtensionSM(tmp_sup_data->agent_id, sup_agent_ext, executionResult))
  {
    ns1__NoDataResponse.errorCause = 503;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 503. CsaWebServer::CommandAgentTelephonyFeatures Sup Agent NOT Found\n" << LOG_TERMINATOR;
    return SOAP_OK;
  }


  AgentRecord*    agent = NULL;
  rc = Csa_Agents_Tbls[TBL_ONLINE]->getByDataBaseId(ns1__RequestTf->AgentId,&agent);

  if( (rc == ARC_OK) && (agent != NULL) )
  {
    if(ns1__RequestTf->Operation == "BREAK_IN")
    {
      NlaCsaAgentBrakeInReq_t notify_data;

      memcpy(notify_data.init_sup_agent_ext , sup_agent_ext.c_str(), sizeof(CstaDevId_t));
      notify_data.dest_agent_db_id = ns1__RequestTf->AgentId;

      CsaUtilSendEvent(CSA_TH3,CSA_agent_breakin_req,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
      return SOAP_OK;
    }
    if(ns1__RequestTf->Operation == "WHISPER")
    {
      NlaCsaAgentWhisperReq_t notify_data;

      memcpy(notify_data.init_sup_agent_ext , sup_agent_ext.c_str(), sizeof(CstaDevId_t));
      notify_data.dest_agent_db_id = ns1__RequestTf->AgentId;

      CsaUtilSendEvent(CSA_TH3,CSA_agent_whisper_req,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
      return SOAP_OK;
    }
    if(ns1__RequestTf->Operation == "SILENT_MONITOR")
    {
      NlaCsaAgentSilentmonitorReq_t notify_data;

      memcpy(notify_data.init_sup_agent_ext , sup_agent_ext.c_str(), sizeof(CstaDevId_t));
      notify_data.dest_agent_db_id = ns1__RequestTf->AgentId;

      CsaUtilSendEvent(CSA_TH3,CSA_agent_silentmonitor_req,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
      return SOAP_OK;
    }
    else
      ns1__NoDataResponse.errorCause = 1;
  }
  else
  {
    ns1__NoDataResponse.errorCause = 501;
    return SOAP_OK;
  }

  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__NoDataResponse->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::CommandAgentTelephonyFeatures \n" << LOG_TERMINATOR;
  return SOAP_OK;
}


/// Web service operation 'SaveWorkspase' (returns error code or SOAP_OK)
int CsaWebServer::SaveWorkspase(ns1__SaveWorkspaseRequestType *ns1__RequestSaveWorkspase, ns1__ResponseInfoHeaderType &ns1__NoDataResponse)
{
  ReportsReply ReplyObj;
  CsaRc_t rc = ARC_OK;

  csa_web_serverMT_log_i << "Invoke CsaWebServer::SaveWorkspase \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__RequestSaveWorkspase == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::SaveWorkspase \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__RequestSaveWorkspase", "ns1__RequestSaveWorkspase is NULL");
  }


  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__NoDataResponse.errorCause = 0;
  ns1__NoDataResponse.serviceReqId = ns1__RequestSaveWorkspase->requestInfoHeader->serviceReqId;

  if(!this->m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestSaveWorkspase->requestInfoHeader->ticket))
  {
    ns1__NoDataResponse.errorCause =500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::SaveWorkspase \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  if(ReplyObj.SaveWorkspase(this,ns1__RequestSaveWorkspase))
  {
    csa_web_serverMT_log_i << "errorCause = 1. CsaWebServer::SaveWorkspase \n" << LOG_TERMINATOR;
    ns1__NoDataResponse.errorCause = 1;
  }
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__NoDataResponse->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::SaveWorkspase \n" << LOG_TERMINATOR;
  return SOAP_OK;
}


/// Web service operation 'SaveReport' (returns error code or SOAP_OK)
int CsaWebServer::SaveReport(ns1__RequestSaveReportType *ns1__RequestSaveReport, ns1__ResponseSaveReportType &ns1__ResponseSaveReport)
{
	ReportsReply ReplyObj;
	CsaRc_t rc = ARC_OK;
	int res;

	csa_web_serverMT_log_i << "Invoke CsaWebServer::SaveReport \n" << LOG_TERMINATOR;
	//Fool protector
	if(ns1__RequestSaveReport == NULL)
	{
		csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::SaveReport \n" << LOG_TERMINATOR;
		return soap_receiver_fault(this, "No Request Data in ns1__RequestSaveReport", "ns1__RequestSaveReport is NULL");
	}


	//start connection
    //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
	ns1__ResponseSaveReport.errorCause = 0;
	ns1__ResponseSaveReport.serviceReqId = ns1__RequestSaveReport->requestInfoHeader->serviceReqId;
	if(!this->m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestSaveReport->requestInfoHeader->ticket))
	{
		ns1__ResponseSaveReport.errorCause =500;
		//m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
		csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::SaveReport \n" << LOG_TERMINATOR;
		return SOAP_OK;
	}
	res = ReplyObj.SaveReport(this,ns1__RequestSaveReport,ns1__ResponseSaveReport.reportId);
	if(res > 0){
          ns1__ResponseSaveReport.errorCause = res;
	}
	//m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__ResponseSaveReport->errorCause);//PVL_STAT
	csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::SaveReport \n" << LOG_TERMINATOR;
	return SOAP_OK;
}

/// Web service operation 'DeleteReport' (returns error code or SOAP_OK)
int CsaWebServer::DeleteReport(ns1__RequestDeleteReportType *ns1__RequestDeleteReport, ns1__ResponseInfoHeaderType &ns1__NoDataResponse)
{
  ReportsReply ReplyObj;
  CsaRc_t      rc = ARC_OK;

  csa_web_serverMT_log_i << "Invoke CsaWebServer::DeleteReport \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__RequestDeleteReport == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::DeleteReport \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__RequestDeleteReport", "ns1__RequestDeleteReport is NULL");
  }


  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__NoDataResponse.errorCause = 0;
  ns1__NoDataResponse.serviceReqId = ns1__RequestDeleteReport->requestInfoHeader->serviceReqId;

  if(!this->m_db_data_ptr->TicketTab.VerifyTicket(ns1__RequestDeleteReport->requestInfoHeader->ticket))
  {
    ns1__NoDataResponse.errorCause =500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::DeleteReport \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  if(ReplyObj.DeleteReport(this,ns1__RequestDeleteReport)){
    ns1__NoDataResponse.errorCause = 1;
  }
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__NoDataResponse->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::DeleteReport \n" << LOG_TERMINATOR;
  return SOAP_OK;
}


/// Web service operation 'LoginManagerAgentsPerGroup' (returns error code or SOAP_OK)
int CsaWebServer::LoginManagerAgentsPerGroup(ns1__LoginManagerRequestType *ns1__LoginManagerRequest, ns1__GenListReturnType &ns1__LoginManagerResponse)
{
  AgentRecord*                agent = NULL;
  CsaAgentPerGroupRecord_t*   agent_per_group = NULL;
  Uint_t                      token;
  CsaRc_t                     rc;
  CsaRc_t                     rc2;
  GenId_t                     group_id;

  csa_web_serverMT_log_i << "Invoke CsaWebServer::LoginManagerAgentsPerGroup \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__LoginManagerRequest == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::LoginManagerAgentsPerGroup \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__LoginManagerRequest", "ns1__LoginManagerRequest is NULL");
  }

  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__LoginManagerResponse.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__LoginManagerResponse.responseInfoHeader->errorCause = (unsigned int)103; //AGENTS not found

  ns1__LoginManagerResponse.responseInfoHeader->serviceReqId = ns1__LoginManagerRequest->requestInfoHeader->serviceReqId;

  if(!this->m_db_data_ptr->TicketTab.VerifyTicket(ns1__LoginManagerRequest->requestInfoHeader->ticket))
  {
    ns1__LoginManagerResponse.responseInfoHeader->errorCause =500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::LoginManagerAgentsPerGroup \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  group_id = ns1__LoginManagerRequest->Id;


  if(!m_db_data_ptr->groupstab.csa_web_TryLock()) //MT
  {
      ns1__LoginManagerResponse.responseInfoHeader->errorCause = (unsigned int)700;//MT SERVER BUSY
      //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT//MT SERVER BUSY
      csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::LoginManagerAgentsPerGroup \n" << LOG_TERMINATOR;
      return SOAP_OK;
  }
  //OPTIONAL , DELETE FOR PERFORMANCE
  this->m_db_data_ptr->groupstab.LoadGroupList();

  if (this->m_db_data_ptr->groupstab.IsGroupExist(group_id))
  {
    //csa_web_serverMT_log_i << "CsaWebServer::LoginManagerAgentsPerGroup: IsGroupExist for group_id " << group_id << " - OK\n" << LOG_TERMINATOR;
    // cycle through login records  by GroupID
    for (rc = Csa_Agents_Tbls[TBL_ONLINE]->getFirstLoginForGroup(group_id, &agent_per_group,token) ;//Group_ID
         rc == ARC_OK;
         rc = Csa_Agents_Tbls[TBL_ONLINE]->getNextLoginForGroup(&agent_per_group,token) )
    {
      //csa_web_serverMT_log_i << "CsaWebServer::LoginManagerAgentsPerGroup: getFirstLoginForGroup for group_id " << group_id << " - OK\n"
      //                       << "got agent_per_group record for Agent Id " << agent_per_group->Agent_id << " And Group Id " << agent_per_group->Group_id
      //                       << LOG_TERMINATOR;
      ns1__LoginManagerResponse.responseInfoHeader->errorCause = (unsigned int)0;
      // if agent logged in, add full info for agent to event
      if (agent_per_group->Logged_in)
      {
        //csa_web_serverMT_log_i << "CsaWebServer::LoginManagerAgentsPerGroup: agent_per_group->Logged_in - OK\n" << LOG_TERMINATOR;
        rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getByDataBaseId(agent_per_group->Agent_id,&agent);
        // put Record of Group per Agent
        if (rc2 == ARC_OK)
        {
          //csa_web_serverMT_log_i << "CsaWebServer::LoginManagerAgentsPerGroup: getByDataBaseId - OK  ---   Insert to List" << LOG_TERMINATOR;
          InsertGenListDataItemType(agent_per_group->Agent_id,
                                    agent->Name,
                                    ns1__LoginManagerResponse);
        }
        //else
        //  csa_web_serverMT_log_i << "CsaWebServer::LoginManagerAgentsPerGroup: getByDataBaseId - FAIL" << LOG_TERMINATOR;
      }
      //else
      //  csa_web_serverMT_log_i << "CsaWebServer::LoginManagerAgentsPerGroup: agent_per_group->Logged_in - NOT Logged In" << LOG_TERMINATOR;
    } //End of All groups per Agent
  } // If agent FOUND
  else
  { //Agent not Found , so show LogOut
    ns1__LoginManagerResponse.responseInfoHeader->errorCause = (unsigned int)102; //Group NOT FOUND
  }
  m_db_data_ptr->groupstab.csa_web_UnLock();	//MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__LoginManagerResponse->responseInfoHeader->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::LoginManagerAgentsPerGroup \n" << LOG_TERMINATOR;
  return SOAP_OK;
}


/// Web service operation 'LoginManagerGroupsPerAgent' (returns error code or SOAP_OK)
int CsaWebServer::LoginManagerGroupsPerAgent(ns1__LoginManagerRequestType *ns1__LoginManagerRequest, ns1__GenListReturnType &ns1__LoginManagerResponse)
{
  AgentRecord*                agent = NULL;
  CsaAgentPerGroupRecord_t*   agent_per_group = NULL;
  Uint_t                      token;
  CsaRc_t                     rc;
  CsaRc_t                     rc2;
  sup_data_t *                tmp_sup_data;

  csa_web_serverMT_log_i << "Invoke CsaWebServer::LoginManagerGroupsPerAgent \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__LoginManagerRequest == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::LoginManagerGroupsPerAgent \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__LoginManagerRequest", "ns1__LoginManagerRequest is NULL");
  }


  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__LoginManagerResponse.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__LoginManagerResponse.responseInfoHeader->errorCause = (unsigned int)102;
  ns1__LoginManagerResponse.responseInfoHeader->serviceReqId = ns1__LoginManagerRequest->requestInfoHeader->serviceReqId;

  //20-Dec-2020 YR BZ#53247
  if(!m_db_data_ptr->TicketTab.VerifyTicketGetData(ns1__LoginManagerRequest->requestInfoHeader->ticket, &tmp_sup_data))
  {
    ns1__LoginManagerResponse.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::LoginManagerGroupsPerAgent \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  if(!m_db_data_ptr->groupstab.csa_web_TryLock())	//MT
  {
    ns1__LoginManagerResponse.responseInfoHeader->errorCause = (unsigned int)700;//MT SERVER BUSY
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT//MT SERVER BUSY
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::LoginManagerGroupsPerAgent \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  //OPTIONAL , DELETE FOR PERFORMANCE
  this->m_db_data_ptr->groupstab.LoadGroupList(tmp_sup_data->user_id, tmp_sup_data->sup_level);

  rc = Csa_Agents_Tbls[TBL_ONLINE]->getByDataBaseId(ns1__LoginManagerRequest->Id,&agent);
  if (rc == ARC_OK)
  {
    // cycle through login records
    for( rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getFirstLoginForAgent(agent->Internal_id, &agent_per_group, token);
         rc2 == ARC_OK;
         rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getNextLoginForAgent(&agent_per_group, token) )
    {
      ns1__LoginManagerResponse.responseInfoHeader->errorCause = (unsigned int)0;
      // if agent logged in, add full info for agent to event
      if (agent_per_group->Logged_in)
      {
        // put Record of Group per Agent
        InsertGenListDataItemType(agent_per_group->Group_id,
                                  this->m_db_data_ptr->groupstab.GetGroupNameById(agent_per_group->Group_id),
                                  ns1__LoginManagerResponse);
      }
    } //End of All groups per Agent
  } // If agent FOUND
  else
  { //Agent not Found , so show LogOut
    ns1__LoginManagerResponse.responseInfoHeader->errorCause = (unsigned int)103; //AGENT NOT FOUND
  }

  m_db_data_ptr->groupstab.csa_web_UnLock();
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__LoginManagerResponse->responseInfoHeader->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::LoginManagerGroupsPerAgent \n" << LOG_TERMINATOR;
  return SOAP_OK;
}


/// Web service operation 'LoginManagerOneAgentToMultiGroups' (returns error code or SOAP_OK)
int CsaWebServer::LoginManagerOneAgentToMultiGroups(ns1__ACDMultiRequestType *ns1__LoginLogoutRequestAcd, ns1__ResponseInfoHeaderType &ns1__NoDataResponse)
{
  CsaRc_t     rc = ARC_OK;
  sup_data_t* tmp_sup_data;

  csa_web_serverMT_log_i << "Invoke CsaWebServer::LoginManagerOneAgentToMultiGroups \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__LoginLogoutRequestAcd == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::LoginManagerOneAgentToMultiGroups \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__LoginLogoutRequestAcd", "ns1__LoginLogoutRequestAcd is NULL");
  }

  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__NoDataResponse.errorCause = 0;
  ns1__NoDataResponse.serviceReqId = ns1__LoginLogoutRequestAcd->requestInfoHeader->serviceReqId;


  if (!m_db_data_ptr->TicketTab.VerifyTicketGetData(ns1__LoginLogoutRequestAcd->requestInfoHeader->ticket, &tmp_sup_data))
  //if (!this->m_db_data_ptr->TicketTab.VerifyTicket(ns1__LoginLogoutRequestAcd->requestInfoHeader->ticket))
  {
    ns1__NoDataResponse.errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::LoginManagerOneAgentToMultiGroups \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  //12-Nov-2019 YR BZ#51007
  if(ns1__LoginLogoutRequestAcd->Operation == "LOGIN")
  {
    NlaCsaAgentLoginReq_t notify_data ;
    for (unsigned int i=0; i<ns1__LoginLogoutRequestAcd->multiId.size(); i++)
    {
      notify_data.agent_db_id =ns1__LoginLogoutRequestAcd->Id ;
      notify_data.group_id = ns1__LoginLogoutRequestAcd->multiId[i];
      csa_web_serverMT_log_i << "CsaWebServer::LoginManagerOneAgentToMultiGroups - User " << tmp_sup_data->sup_name.c_str() << " Request LoginGroup for Agent ID " << notify_data.agent_db_id << " to Group ID " << notify_data.group_id << "\n" << LOG_TERMINATOR;
      CsaUtilSendEvent(CSA_TH3,CSA_logged_in_request,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
    }
    csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::LoginManagerOneAgentToMultiGroups \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  else if(ns1__LoginLogoutRequestAcd->Operation == "LOGOUT")
  {
    NlaCsaAgentLogoutReq_t notify_data;
    notify_data.agent_db_id = ns1__LoginLogoutRequestAcd->Id;

    if(ns1__LoginLogoutRequestAcd->multiId.size()==0)
    {
      csa_web_serverMT_log_i << "CsaWebServer::LoginManagerOneAgentToMultiGroups - User " << tmp_sup_data->sup_name.c_str() << " Request LogoutPrimary for Agent ID " << notify_data.agent_db_id << "\n" << LOG_TERMINATOR;
      notify_data.group_id = 0;
      CsaUtilSendEvent(CSA_TH3,CSA_logged_out_request,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
    }
    else
    {
      for (unsigned int i=0; i<ns1__LoginLogoutRequestAcd->multiId.size(); i++)
      {
        notify_data.group_id = ns1__LoginLogoutRequestAcd->multiId[i];
        csa_web_serverMT_log_i << "CsaWebServer::LoginManagerOneAgentToMultiGroups - User " << tmp_sup_data->sup_name.c_str() << " Request LogoutGroup for Agent ID " << notify_data.agent_db_id << " from Group ID " << notify_data.group_id << "\n" << LOG_TERMINATOR;
        CsaUtilSendEvent(CSA_TH3,CSA_logged_out_request,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
      }
    }

    //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__NoDataResponse->errorCause);//PVL_STAT
    csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::LoginManagerOneAgentToMultiGroups \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  else
  {
    csa_web_serverMT_log_i << "Unknown Operation. CsaWebServer::LoginManagerOneAgentToMultiGroups \n" << LOG_TERMINATOR;
    ns1__NoDataResponse.errorCause = 1;
  }

  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__NoDataResponse->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::LoginManagerOneAgentToMultiGroups \n" << LOG_TERMINATOR;
  return SOAP_OK;
}

/// Web service operation 'LoginManagerMultiAgentsToOneGroup' (returns error code or SOAP_OK)
int CsaWebServer::LoginManagerMultiAgentsToOneGroup(ns1__ACDMultiRequestType *ns1__LoginLogoutRequestAcd, ns1__ResponseInfoHeaderType &ns1__NoDataResponse)
{
  CsaRc_t     rc = ARC_OK;
  sup_data_t* tmp_sup_data;

  csa_web_serverMT_log_i << "Invoke CsaWebServer::LoginManagerMultiAgentsToOneGroup \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__LoginLogoutRequestAcd == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::LoginManagerMultiAgentsToOneGroup \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__LoginLogoutRequestAcd", "ns1__LoginLogoutRequestAcd is NULL");
  }

  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__NoDataResponse.errorCause = 0;
  ns1__NoDataResponse.serviceReqId = ns1__LoginLogoutRequestAcd->requestInfoHeader->serviceReqId;

  if (!m_db_data_ptr->TicketTab.VerifyTicketGetData(ns1__LoginLogoutRequestAcd->requestInfoHeader->ticket, &tmp_sup_data))
  //if(!this->m_db_data_ptr->TicketTab.VerifyTicket(ns1__LoginLogoutRequestAcd->requestInfoHeader->ticket))
  {
    ns1__NoDataResponse.errorCause =500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::LoginManagerMultiAgentsToOneGroup \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  if(ns1__LoginLogoutRequestAcd->Operation == "LOGIN")
  {
    NlaCsaAgentLoginReq_t notify_data ;
    for (unsigned int i=0; i<ns1__LoginLogoutRequestAcd->multiId.size(); i++)
    {
      notify_data.agent_db_id =ns1__LoginLogoutRequestAcd->multiId[i] ;
      notify_data.group_id =   ns1__LoginLogoutRequestAcd->Id ;
      csa_web_serverMT_log_i << "CsaWebServer::LoginManagerMultiAgentsToOneGroup - User " << tmp_sup_data->sup_name.c_str() << " Request LoginGroup for Agent ID " << notify_data.agent_db_id << " to Group ID " << notify_data.group_id << "\n" << LOG_TERMINATOR;
      CsaUtilSendEvent(CSA_TH3,CSA_logged_in_request,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
    }
    csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::LoginManagerMultiAgentsToOneGroup \n" << LOG_TERMINATOR;
    return SOAP_OK;

  }

  if(ns1__LoginLogoutRequestAcd->Operation == "LOGOUT")
  {
    NlaCsaAgentLogoutReq_t notify_data;
    for (unsigned int i=0; i<ns1__LoginLogoutRequestAcd->multiId.size(); i++)
    {
      notify_data.agent_db_id = ns1__LoginLogoutRequestAcd->multiId[i];//PVL 13-06-2010 1004198
      notify_data.group_id = ns1__LoginLogoutRequestAcd->Id ;
      csa_web_serverMT_log_i << "CsaWebServer::LoginManagerMultiAgentsToOneGroup - User " << tmp_sup_data->sup_name.c_str() << " Request LogoutGroup for Agent ID " << notify_data.agent_db_id << " from Group ID " << notify_data.group_id << "\n" << LOG_TERMINATOR;
      CsaUtilSendEvent(CSA_TH3,CSA_logged_out_request,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
    }
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__NoDataResponse->errorCause);//PVL_STAT
    csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::LoginManagerMultiAgentsToOneGroup \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  else
    ns1__NoDataResponse.errorCause = 1;

  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__NoDataResponse->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::LoginManagerMultiAgentsToOneGroup \n" << LOG_TERMINATOR;
  return SOAP_OK;
}


/// Web service operation 'LoginManagerAvailableAgentsForGroup' (returns error code or SOAP_OK)
int CsaWebServer::LoginManagerAvailableAgentsForGroup(ns1__LoginManagerRequestType *ns1__LoginManagerRequestAvailableAgents, ns1__GenListReturnType &ns1__LoginManagerResponse)
{
  csa_web_serverMT_log_i << "Invoke CsaWebServer::LoginManagerAvailableAgentsForGroup \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__LoginManagerRequestAvailableAgents == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::LoginManagerAvailableAgentsForGroup \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__LoginManagerRequestAvailableAgents", "ns1__LoginManagerRequestAvailableAgents is NULL");
  }

  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT

  ns1__LoginManagerResponse.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__LoginManagerResponse.responseInfoHeader->errorCause = (unsigned int)102;
  ns1__LoginManagerResponse.responseInfoHeader->serviceReqId = ns1__LoginManagerRequestAvailableAgents->requestInfoHeader->serviceReqId;
  // Test Ticket
  if(!this->m_db_data_ptr->TicketTab.VerifyTicket(ns1__LoginManagerRequestAvailableAgents->requestInfoHeader->ticket))
  {
    ns1__LoginManagerResponse.responseInfoHeader->errorCause =500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::LoginManagerAvailableAgentsForGroup \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  if(!m_db_data_ptr->agentstab.csa_web_TryLock())	//MT
  {
    ns1__LoginManagerResponse.responseInfoHeader->errorCause = (unsigned int)700;//MT SERVER BUSY
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT//MT SERVER BUSY
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::LoginManagerAvailableAgentsForGroup \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  if(m_db_data_ptr->agentstab.tab.size() >0)
    for(m_db_data_ptr->agentstab.it=m_db_data_ptr->agentstab.tab.begin(); m_db_data_ptr->agentstab.it!= m_db_data_ptr->agentstab.tab.end(); ++m_db_data_ptr->agentstab.it)
    {
      //Test , is Agent Logged in Another Group
      if( IsAgentLoggedOnlyToAnotherGroup( (*m_db_data_ptr->agentstab.it).second.a_id,     //Agent Id
                                            ns1__LoginManagerRequestAvailableAgents->Id) ) //Group Id
      {
        ns1__LoginManagerResponse.responseInfoHeader->errorCause  = 0;
        InsertGenListDataItemType((*m_db_data_ptr->agentstab.it).second.a_id,
                                  (*m_db_data_ptr->agentstab.it).second.a_name,
                                  ns1__LoginManagerResponse);
      }
    }
    m_db_data_ptr->agentstab.csa_web_UnLock();//MT
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__LoginManagerResponse->responseInfoHeader->errorCause);//PVL_STAT
    csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::LoginManagerAvailableAgentsForGroup \n" << LOG_TERMINATOR;
    return SOAP_OK;
}


//////////////////////////////////////////////////////////////////////////////
bool CsaWebServer::IsAgentLoggedOnlyToAnotherGroup(unsigned int AgentId,unsigned int GroupId )
{
  AgentRecord*              agent = NULL;
  CsaAgentPerGroupRecord_t* agent_per_group = NULL;
  Uint_t                    token;
  bool FoundInTheGroup = false;
  bool FoundInAnotherGroup = false;
  CsaRc_t rc;
  CsaRc_t rc2;

  BriefAgentInfo TotalsAgentInfo;
  TotalsAgentInfo.InitCounters();//CRASH 28-06-2010
  rc = Csa_Agents_Tbls[TBL_ONLINE]->getByDataBaseId(AgentId,&agent);
  if (rc == ARC_OK)
  {
    // cycle through login records
    for ( rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getFirstLoginForAgent(agent->Internal_id, &agent_per_group, token);
          rc2 == ARC_OK;
          rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getNextLoginForAgent(&agent_per_group, token) )
    {
      if (agent_per_group->Logged_in)
      { // if agent logged in, Test GroupId
        if(GroupId == agent_per_group->Group_id  )
          FoundInTheGroup = true;
        else
          FoundInAnotherGroup = true;
      }
    }
  } // If agent FOUND
  else
  { //Agent not Found ,
    return false;
  }

  if(!FoundInTheGroup && FoundInAnotherGroup)
    return true;
  else
    return false;

}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool CsaWebServer::IsAgentNotLoggedToGroup(unsigned int AgentId,unsigned int GroupId )
{
	AgentRecord*                                   agent = NULL;
	CsaAgentPerGroupRecord_t*                      agent_per_group = NULL;
	Uint_t                                         token;
	bool FoundInTheGroup =false;
	bool FoundInAnotherGroup= false;
	CsaRc_t rc;
	CsaRc_t rc2;

        BriefAgentInfo TotalsAgentInfo;
		TotalsAgentInfo.InitCounters();//CRASH 28-06-2010
		rc = Csa_Agents_Tbls[TBL_ONLINE]->getByDataBaseId(AgentId,&agent);
		if (rc == ARC_OK){
			// cycle through login records
			for (
				rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getFirstLoginForAgent(agent->Internal_id, &agent_per_group, token);
				rc2 == ARC_OK;
				rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getNextLoginForAgent(&agent_per_group, token)
				)
				if (agent_per_group->Logged_in)
				{ // if agent logged in, Test GroupId
					if(GroupId == agent_per_group->Group_id  )
						FoundInTheGroup = true;
					else
						FoundInAnotherGroup = true;
				}

		} // If agent FOUND
		else
		{ //Agent not Found ,
		return true;
		}
	if(FoundInTheGroup)
		return false;
	else
		return true;


}

//////////////////////////////////////////////////////////////////////////////
	/// Web service operation 'LoginManagerAvailableGroupsForAgent' (returns error code or SOAP_OK)
int CsaWebServer::LoginManagerAvailableGroupsForAgent(ns1__LoginManagerRequestType *ns1__LoginManagerRequestAvailableGroups, ns1__GenListReturnType &ns1__LoginManagerResponse)
{
  csa_web_serverMT_log_i << "Invoke CsaWebServer::LoginManagerAvailableGroupsForAgent \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__LoginManagerRequestAvailableGroups == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::LoginManagerAvailableGroupsForAgent \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__LoginManagerRequestAvailableGroups", "ns1__LoginManagerRequestAvailableGroups is NULL");
  }

  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT

  ns1__LoginManagerResponse.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__LoginManagerResponse.responseInfoHeader->errorCause = (unsigned int)102;
  ns1__LoginManagerResponse.responseInfoHeader->serviceReqId = ns1__LoginManagerRequestAvailableGroups->requestInfoHeader->serviceReqId;
  // Test Ticket
  if(!this->m_db_data_ptr->TicketTab.VerifyTicket(ns1__LoginManagerRequestAvailableGroups->requestInfoHeader->ticket))
  {
    ns1__LoginManagerResponse.responseInfoHeader->errorCause =500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::LoginManagerAvailableGroupsForAgent \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }


  if(this->m_db_data_ptr->groupstab.tab.size() > 0)
  {
    for( m_db_data_ptr->groupstab.it = m_db_data_ptr->groupstab.tab.begin();
         m_db_data_ptr->groupstab.it != m_db_data_ptr->groupstab.tab.end();
         ++m_db_data_ptr->groupstab.it )
    {
      if(IsAgentNotLoggedToGroup( ns1__LoginManagerRequestAvailableGroups->Id, //Agent Id
                                  (*m_db_data_ptr->groupstab.it).first) )      //Group Id
      {
        ns1__LoginManagerResponse.responseInfoHeader->errorCause  = 0;
        InsertGenListDataItemType( (*m_db_data_ptr->groupstab.it).first, //id
                                   (*m_db_data_ptr->groupstab.it).second.grpName,//name
                                   ns1__LoginManagerResponse );
      }
    }
  }
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__LoginManagerResponse->responseInfoHeader->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::LoginManagerAvailableGroupsForAgent \n" << LOG_TERMINATOR;
  return SOAP_OK;
}


// Web service operation 'ApplicationRegister' (returns error code or SOAP_OK)
int CsaWebServer::ApplicationRegister(ns1__ApplicationRegisterRequestType *ns1__ApplicationRegisterRequest,
                                      ns1__ApplicationReturnType &ns1__ApplicationRegisterResponse)
{
  csa_web_serverMT_log_i << "Invoke CsaWebServer::ApplicationRegister - ";
  //Fool protector
  if(ns1__ApplicationRegisterRequest == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::ApplicationRegister \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__ApplicationRegisterRequest", "ns1__ApplicationRegisterRequest is NULL");
  }

  char servers_info[256];
  memset(servers_info, 0, 256);     
  EosHAPrepareAllStatus(servers_info, __FILE__, __LINE__);
  AddDPToServersInfo_AppCenter(servers_info);

  ha_status_is_active = IsHAActive();
  if(ha_status_is_active)
    csa_web_serverMT_log_i << "Server is ACTIVE";
  else
    csa_web_serverMT_log_i << "Server is NOT ACTIVE";
  csa_web_serverMT_log_i << LOG_TERMINATOR;

  ns1__ApplicationRegisterResponse.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__ApplicationRegisterResponse.responseInfoHeader->errorCause = (unsigned int)0;
  ns1__ApplicationRegisterResponse.responseInfoHeader->serviceReqId = ns1__ApplicationRegisterRequest->serviceReqId;
  ns1__ApplicationRegisterResponse.responseInfoHeader->serversInfo = servers_info;

  if(IsHAActive())
  {
    // Verify duplicity of application
    //START CHANGED BY PVL_STAT
    switch( this->m_db_data_ptr->TicketTab.IsApplicationKeepAliveTooOld(ns1__ApplicationRegisterRequest->applicationId) )
    {
      case TICKET_tab::NOTHING_FOUND:
#ifdef _SM_AUTHENTICATION_
        m_db_data_ptr->TicketTab.LogoutUsersPerApplicationIdSM(ns1__ApplicationRegisterRequest->applicationId );
#else
        this->m_db_data_ptr->TicketTab.LogoutUsersPerApplicationId(ns1__ApplicationRegisterRequest->applicationId);
#endif
        m_db_data_ptr->TicketTab.UpdateApplicationKeepAliveTime(ns1__ApplicationRegisterRequest->applicationId);
        m_db_data_ptr->TraceTab.AddAplication(ip, port, ns1__ApplicationRegisterRequest->applicationId);//PVL_STAT
      break;
      case TICKET_tab::KA_TOO_OLD:
#ifdef _SM_AUTHENTICATION_
        this->m_db_data_ptr->TicketTab.LogoutUsersPerApplicationIdSM(ns1__ApplicationRegisterRequest->applicationId );
#else
        this->m_db_data_ptr->TicketTab.LogoutUsersPerApplicationId(ns1__ApplicationRegisterRequest->applicationId);
#endif
        //bData->TraceTab.ZeroAplication(this->ip);//PVL_STAT
        //m_db_data_ptr->TraceTab.AddKeepAlive(this->ip);//PVL_STAT
      break;
      case TICKET_tab::KA_NORMAL:
        //ns1__ApplicationRegisterResponse->responseInfoHeader->errorCause = (unsigned int)501;
        ns1__ApplicationRegisterResponse.responseInfoHeader->errorCause = 0;
//15-Sep-2019 YR BZ#50517
#ifdef _SM_AUTHENTICATION_
        this->m_db_data_ptr->TicketTab.LogoutUsersPerApplicationIdSM(ns1__ApplicationRegisterRequest->applicationId );
#else
        this->m_db_data_ptr->TicketTab.LogoutUsersPerApplicationId(ns1__ApplicationRegisterRequest->applicationId);
#endif
      break;
    }
  }

  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::ApplicationRegister\n" 
                         << "Servers Info \"" << ns1__ApplicationRegisterResponse.responseInfoHeader->serversInfo.c_str() << "\""
                         << LOG_TERMINATOR;
  return SOAP_OK;
  //END CHANGED BY PVL_STAT
}//ApplicationRegister


// Web service operation 'ApplicationKeepAlive' (returns error code or SOAP_OK)
int CsaWebServer::ApplicationKeepAlive(ns1__ApplicationKeepAliveRequestType *ns1__ApplicationKeepAliveRequest, ns1__ApplicationReturnType &ns1__ApplicationKeepAliveResponse)
{
  csa_web_serverMT_log_d << "Invoke CsaWebServer::ApplicationKeepAlive - ";
  //Fool protector
  if(ns1__ApplicationKeepAliveRequest == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::ApplicationKeepAlive \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__ApplicationKeepAliveRequest", "ns1__ApplicationKeepAliveRequest is NULL");
  }

  //printf("  +++ time: %d, check if keeplaive. already in process report_code: [%d]  thid: [%d]\n",time(0), do_reports_area[KeepAlive_E],this->ThreadIndex);  fflush(stdout);
  if (do_reports_area[KeepAlive_E] == 1) // already process brief agent report in another thread
  {
    //12-Oct-2021 YR BZ#55296
    ns1__ApplicationKeepAliveResponse.responseInfoHeader->errorCause = (unsigned int)400;//MT SERVER BUSY
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 400. KeepAlive_E \n" << LOG_TERMINATOR;
    printf("+++ errorCause = 400. SERVER BUSY CsaWebServer::KeepAlive  \n");  fflush(stdout);
    return SOAP_OK;
  }

  char servers_info[256];
  memset(servers_info, 0, 256);     
  EosHAPrepareAllStatus(servers_info, __FILE__, __LINE__);
  AddDPToServersInfo_AppCenter(servers_info);
  //AddServerTimeToServersInfo(servers_info);

  //start connection
  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__ApplicationKeepAliveResponse.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__ApplicationKeepAliveResponse.responseInfoHeader->errorCause = (unsigned int)0;
  ns1__ApplicationKeepAliveResponse.responseInfoHeader->serviceReqId = ns1__ApplicationKeepAliveRequest->serviceReqId;
  ns1__ApplicationKeepAliveResponse.responseInfoHeader->serversInfo = servers_info;

  if(ha_status_is_active && !IsHAActive())
  {
    csa_web_serverMT_log_i << "Server was ACTIVE becomes NOT ACTIVE, Logout all users for app_id " << ns1__ApplicationKeepAliveRequest->applicationId.c_str() << LOG_TERMINATOR;
#ifdef _SM_AUTHENTICATION_
    this->m_db_data_ptr->TicketTab.LogoutUsersPerApplicationIdSM(ns1__ApplicationKeepAliveRequest->applicationId );
#else
    this->m_db_data_ptr->TicketTab.LogoutUsersPerApplicationId(ns1__ApplicationKeepAliveRequest->applicationId);
#endif
  }
  else if(!m_db_data_ptr->TicketTab.IsApplicationRegister(ns1__ApplicationKeepAliveRequest->applicationId))
  {
    csa_web_serverMT_log_i << "NOT Registered for app_id " << ns1__ApplicationKeepAliveRequest->applicationId.c_str() << LOG_TERMINATOR;
    m_db_data_ptr->TraceTab.AddAplication(ip, port, ns1__ApplicationKeepAliveRequest->applicationId);//PVL_STAT
  }
  else
  {
    csa_web_serverMT_log_d << "Server is ACTIVE" << LOG_TERMINATOR;
  }

  ha_status_is_active = IsHAActive();

  m_db_data_ptr->TicketTab.UpdateApplicationKeepAliveTime(ns1__ApplicationKeepAliveRequest->applicationId);
  //m_db_data_ptr->TraceTab.AddKeepAlive(this->ip);//PVL_STAT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__ApplicationKeepAliveResponse->responseInfoHeader->errorCause);//PVL_STAT
  csa_web_serverMT_log_d << "SOAP_OK. CsaWebServer::ApplicationKeepAlive\n" 
                         << "Servers Info \"" << ns1__ApplicationKeepAliveResponse.responseInfoHeader->serversInfo.c_str() << "\""
                         << LOG_TERMINATOR;
  return SOAP_OK;
}// END Web service operation 'ApplicationKeepAlive'


// Web service operation 'TraceInternalStat' (returns error code or SOAP_OK)
int  CsaWebServer::TraceInternalStat(ns1__StatRequestType *ns1__TraceInternalStatRequest, ns1__StatReturnType &ns1__TraceInternalStatResponse)
{
  csa_web_serverMT_log_i << "Invoke CsaWebServer::TraceInternalStat \n" << LOG_TERMINATOR;
  ns1__StatItemType *tmp;

  //Fool protector
  if(ns1__TraceInternalStatRequest == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::TraceInternalStat \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__TraceInternalStatRequest", "ns1__TraceInternalStatRequest is NULL");
  }

  ns1__TraceInternalStatResponse.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__TraceInternalStatResponse.responseInfoHeader->errorCause = (unsigned int)0;
  ns1__TraceInternalStatResponse.responseInfoHeader->serviceReqId = ns1__TraceInternalStatRequest->serviceReqId;

  for(m_db_data_ptr->TraceTab.it=m_db_data_ptr->TraceTab.tab.begin(); m_db_data_ptr->TraceTab.it!= m_db_data_ptr->TraceTab.tab.end(); ++m_db_data_ptr->TraceTab.it)
  {
    tmp = soap_new_ns1__StatItemType(this, 1);
    tmp->ipAddress = m_db_data_ptr->TraceTab.it->second->ip_addres;
    tmp->CurrentNumberOfSoapConnections = m_db_data_ptr->TraceTab.it->second->current_number_of_connections;
    tmp->applicationId = m_db_data_ptr->TraceTab.it->second->application_id;
    tmp->LastError = m_db_data_ptr->TraceTab.it->second->last_error_code;
    tmp->lastKeepAliveSecAgo = NlaCalcCsaElapsedTime(m_db_data_ptr->TraceTab.it->second->last_keep_alive);

    tmp->numberOfSoapConnectionsSinceLastStatRequest = m_db_data_ptr->TraceTab.it->second->n_last_request_conn;
    m_db_data_ptr->TraceTab.it->second->n_last_request_conn = 0;

    tmp->numOfMonitors = m_db_data_ptr->TraceTab.it->second->n_monitors;
    tmp->numOfVisors = m_db_data_ptr->TraceTab.it->second->n_visors;
    tmp->numOfWebVisors = m_db_data_ptr->TraceTab.it->second->n_web_visors;
    tmp->totalNumberOfSoapConnections = m_db_data_ptr->TraceTab.it->second->n_total_conn;
    m_net_data_ptr->STAT_vector.push_back(tmp);
    ns1__TraceInternalStatResponse.returnStatArray.push_back(tmp);
  }

  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::TraceInternalStat \n" << LOG_TERMINATOR;

  return SOAP_OK;

}// END Web service operation 'TraceInternalStat'


// Web service operation 'AgentNeedHelp' (returns error code or SOAP_OK)
int CsaWebServer::AgentNeedHelp(ns1__BriefInfoRequestType *ns1__AgentNeedHelpRequest, ns1__ReturnMatrixDataTypeBG &ns1__AgentNeedHelpResponse)
{
  AgentRecordReply            ReplyObj;
  CsaRc_t                     rc = ARC_OK;
  CsaRc_t                     rc2 = ARC_OK;
  GenId_t		              help_group_id;
  AgentRecord*                agent;
  CsaAgentPerGroupRecord_t*   agent_per_group;
  Uint_t                      token;

  csa_web_serverMT_log_i << "Invoke CsaWebServer::AgentNeedHelp \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__AgentNeedHelpRequest == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::AgentNeedHelp \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data in ns1__AgentNeedHelpRequest", "ns1__AgentNeedHelpRequest is NULL");
  }

  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT

  ns1__AgentNeedHelpResponse.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__AgentNeedHelpResponse.responseInfoHeader->serviceReqId = ns1__AgentNeedHelpRequest->requestInfoHeader->serviceReqId;
  ns1__AgentNeedHelpResponse.responseInfoHeader->errorCause = 0;

  if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__AgentNeedHelpRequest->requestInfoHeader->ticket))
  {
    ns1__AgentNeedHelpResponse.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::AgentNeedHelp \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  Csa_Setup.get(NUM_GROUPS, (int *)&help_group_id);

  for (rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getFirstLoginForGroup(help_group_id,&agent_per_group,token);
    rc2 == ARC_OK;
    rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getNextLoginForGroup(&agent_per_group,token))
  {
    // if agent logged in, add full info for agent to event
    if (agent_per_group->Logged_in)
    {
      // static info - need agent record
      rc = Csa_Agents_Tbls[TBL_ONLINE]->get(agent_per_group->Internal_id,&agent);
      if (rc == ARC_OK){
        ReplyObj.CreateNewRow(this); //Start Row
        ReplyObj.InsertAgentHELPDataToRow(this,agent,agent_per_group);
        ReplyObj.InsertRowToMatrix(this,ns1__AgentNeedHelpResponse);//End Row
      }
    }
  }

  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__AgentNeedHelpResponse->responseInfoHeader->errorCause);//PVL_STAT

  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::AgentNeedHelp \n" << LOG_TERMINATOR;

  return SOAP_OK;
}// END Web service operation 'AgentNeedHelp'


// Web service operation 'GroupHourlyInfo' (returns error code or SOAP_OK)
int CsaWebServer::GroupHourlyInfo(ns1__GenInfoRequestType *ns1__GroupHourlyInfoRequest, ns1__ReturnMatrixDataTypePerGroup &ns1__GroupHourlyInfoResponse)
{
  GroupHourlyReply  ReplyObj;
  CsaRc_t           rc = ARC_OK;
  //GroupRecord*      grec;
  unsigned int      ret_index=0;

  csa_web_serverMT_log_i << "Invoke CsaWebServer::GroupHourlyInfo (group size " << ns1__GroupHourlyInfoRequest->Id.size() << ")\n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__GroupHourlyInfoRequest == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault o Request Data. CsaWebServer::GroupHourlyInfo \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data", "ns1__GroupHourlyInfoRequest is NULL");
  }


  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__GroupHourlyInfoResponse.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__GroupHourlyInfoResponse.responseInfoHeader->serviceReqId = ns1__GroupHourlyInfoRequest->requestInfoHeader->serviceReqId;

  if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__GroupHourlyInfoRequest->requestInfoHeader->ticket))
  {
    ns1__GroupHourlyInfoResponse.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::GroupHourlyInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  if(!m_db_data_ptr->groupstab.csa_web_TryLock())	//MT
  {
    ns1__GroupHourlyInfoResponse.responseInfoHeader->errorCause = (unsigned int)700;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT//MT BUSY
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::GroupHourlyInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  //  body
  for (unsigned int i=0; i<ns1__GroupHourlyInfoRequest->Id.size(); i++)
  {
    m_db_data_ptr->limitstab.VerifyGroupInLimitTab(ns1__GroupHourlyInfoRequest->Id[i]); //PVL 28-07-2009 1002567
    ns1__GroupHourlyInfoResponse.responseInfoHeader->errorCause = (unsigned int)0;
    ReplyObj.AddNewGroupBlock(this,ns1__GroupHourlyInfoRequest->Id[i],ns1__GroupHourlyInfoResponse);
    ReplyObj.InsertDataToRows(this,ns1__GroupHourlyInfoRequest->Id[i],ns1__GroupHourlyInfoResponse,i);
  }

  m_db_data_ptr->groupstab.csa_web_UnLock();//MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__GroupHourlyInfoResponse->responseInfoHeader->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::GroupHourlyInfo \n" << LOG_TERMINATOR;
  return SOAP_OK;
}

/// Web service operation 'GroupDailyBriefInfo' (returns error code or SOAP_OK)
// Prototype int CsaWebServer::GroupQueInfo       (ns1__GenInfoRequestType *ns1__RequestGroupQueInfo,        ns1__ReturnArrayDataType *ns1__ReturnGroupQueInfo)
int CsaWebServer::GroupDailyBriefInfo(ns1__GenInfoRequestType *ns1__GroupDailyBriefInfoRequest, ns1__ReturnArrayDataType &ns1__GroupDailyBriefInfoResponse)
{
  GroupRecordReply    ReplyObj;
  CsaRc_t             rc = ARC_OK;
  GroupRecord*        grec;
  unsigned int        ret_index=0; //PVL 14-06-2010 1004180 not valid index problem

  csa_web_serverMT_log_i << "Invoke CsaWebServer::GroupDailyBriefInfo \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__GroupDailyBriefInfoRequest == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::GroupDailyBriefInfo \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data", "ns1__GroupDailyBriefInfoRequest is NULL");
  }


  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__GroupDailyBriefInfoResponse.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__GroupDailyBriefInfoResponse.responseInfoHeader->serviceReqId = ns1__GroupDailyBriefInfoRequest->requestInfoHeader->serviceReqId;

  if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__GroupDailyBriefInfoRequest->requestInfoHeader->ticket))
  {
    ns1__GroupDailyBriefInfoResponse.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::GroupDailyBriefInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  if(!m_db_data_ptr->groupstab.csa_web_TryLock())	//MT
  {
    ns1__GroupDailyBriefInfoResponse.responseInfoHeader->errorCause = (unsigned int)700;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT//MT BUSY
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::GroupDailyBriefInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  //12-Sep-2022 YR BZ#56823
  char servers_info[256];
  memset(servers_info, 0, 256);
  AddServerTimeToServersInfo(servers_info);
  ns1__GroupDailyBriefInfoResponse.responseInfoHeader->serversInfo = servers_info;

  for (unsigned int i=0; i<ns1__GroupDailyBriefInfoRequest->Id.size(); i++)
  {
    m_db_data_ptr->limitstab.VerifyGroupInLimitTab(ns1__GroupDailyBriefInfoRequest->Id[i]); //PVL 28-07-2009 1002567
    rc = Csa_Grps_Tbls[TBL_DAILY_SUM]->get(ns1__GroupDailyBriefInfoRequest->Id[i], &grec);
    if (rc == ARC_OK)
    {
      if(ReplyObj.AddNewGroupBlock(this,grec->Grp_id,ns1__GroupDailyBriefInfoResponse))
      {
        ns1__GroupDailyBriefInfoResponse.responseInfoHeader->errorCause = (unsigned int)0;
        ReplyObj.MakePerGroup_QueInfo(this,grec,ns1__GroupDailyBriefInfoResponse.returnArrayOfBlocks[ret_index]); //PVL 14-06-2010 1004180 not valid index problem
        ReplyObj.MakePerGroup_AgentsInfo(this,grec,ns1__GroupDailyBriefInfoResponse.returnArrayOfBlocks[ret_index]); //PVL 14-06-2010 1004180 not valid index problem
        ReplyObj.MakePerGroup_ShiftInfo(this,grec,ns1__GroupDailyBriefInfoResponse.returnArrayOfBlocks[ret_index]); //PVL 14-06-2010 1004180 not valid index problem
        ReplyObj.MakePerGroup_InterflInfo(this,grec,ns1__GroupDailyBriefInfoResponse.returnArrayOfBlocks[ret_index]); //PVL 14-06-2010 1004180 not valid index problem
        ReplyObj.MakePerGroup_AbandInfo(this,grec,ns1__GroupDailyBriefInfoResponse.returnArrayOfBlocks[ret_index]); //PVL 14-06-2010 1004180 not valid index problem
        ret_index++; //PVL 14-06-2010 1004180 not valid index problem
      }
    }
    else
    {
      ns1__GroupDailyBriefInfoResponse.responseInfoHeader->errorCause = (unsigned int)1;
    }
  }

  m_db_data_ptr->groupstab.csa_web_UnLock();//MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__GroupDailyBriefInfoResponse->responseInfoHeader->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK CsaWebServer::GroupDailyBriefInfo \n" << LOG_TERMINATOR;
  return SOAP_OK;
}


/// Web service operation 'GroupDailyDetailedInfo' (returns error code or SOAP_OK)
//int CsaWebServer::GroupDailyDetailedInfo(ns1__GenInfoRequestType *ns1__GroupDailyDetailedInfoRequest, ns1__ReturnArrayDataType *ns1__GroupDailyDetailedInfoResponse)
int CsaWebServer::GroupDailyDetailedInfo(ns1__GenInfoRequestType *ns1__GroupDailyBriefInfoRequest, ns1__ReturnArrayDataType &ns1__GroupDailyBriefInfoResponse)
{
  GroupRecordReply    ReplyObj;
  CsaRc_t             rc = ARC_OK;
  GroupRecord*        grec;
  unsigned int        ret_index=0; //PVL 14-06-2010 1004180 not valid index problem

  csa_web_serverMT_log_i << "Invoke CsaWebServer::GroupDailyDetailedInfo \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__GroupDailyBriefInfoRequest == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_faultNo Request Data. CsaWebServer::GroupDailyDetailedInfo \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data", "ns1__GroupDailyBriefInfoRequest is NULL");
  }


  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__GroupDailyBriefInfoResponse.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__GroupDailyBriefInfoResponse.responseInfoHeader->serviceReqId = ns1__GroupDailyBriefInfoRequest->requestInfoHeader->serviceReqId;

  if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__GroupDailyBriefInfoRequest->requestInfoHeader->ticket))
  {
    ns1__GroupDailyBriefInfoResponse.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::GroupDailyDetailedInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  if(!m_db_data_ptr->groupstab.csa_web_TryLock())	//MT
  {
    ns1__GroupDailyBriefInfoResponse.responseInfoHeader->errorCause = (unsigned int)700;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT//MT BUSY
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::GroupDailyDetailedInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  for (unsigned int i=0; i<ns1__GroupDailyBriefInfoRequest->Id.size(); i++)
  {
    m_db_data_ptr->limitstab.VerifyGroupInLimitTab(ns1__GroupDailyBriefInfoRequest->Id[i]); //PVL 28-07-2009 1002567
    rc = Csa_Grps_Tbls[TBL_DAILY_SUM]->get(ns1__GroupDailyBriefInfoRequest->Id[i], &grec);
    if (rc == ARC_OK)
    {
      if(ReplyObj.AddNewGroupBlock(this,grec->Grp_id,ns1__GroupDailyBriefInfoResponse))
      {
        ns1__GroupDailyBriefInfoResponse.responseInfoHeader->errorCause = (unsigned int)0;
        ReplyObj.MakePerGroup_QueInfo(this,grec,ns1__GroupDailyBriefInfoResponse.returnArrayOfBlocks[ret_index]); //PVL 14-06-2010 1004180 not valid index problem
        ReplyObj.MakePerGroup_AgentsInfo(this,grec,ns1__GroupDailyBriefInfoResponse.returnArrayOfBlocks[ret_index]); //PVL 14-06-2010 1004180 not valid index problem
        ReplyObj.MakePerGroup_ShiftInfo(this,grec,ns1__GroupDailyBriefInfoResponse.returnArrayOfBlocks[ret_index]); //PVL 14-06-2010 1004180 not valid index problem
        ReplyObj.MakePerGroup_InterflInfo(this,grec,ns1__GroupDailyBriefInfoResponse.returnArrayOfBlocks[ret_index]); //PVL 14-06-2010 1004180 not valid index problem
        ReplyObj.MakePerGroup_AbandInfo(this,grec,ns1__GroupDailyBriefInfoResponse.returnArrayOfBlocks[ret_index]); //PVL 14-06-2010 1004180 not valid index problem
        ret_index++; //PVL 14-06-2010 1004180 not valid index problem
      }

    }
    else
    {
      ns1__GroupDailyBriefInfoResponse.responseInfoHeader->errorCause = (unsigned int)1;
    }
  }

  m_db_data_ptr->groupstab.csa_web_UnLock();//MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__GroupDailyBriefInfoResponse->responseInfoHeader->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::GroupDailyDetailedInfo \n" << LOG_TERMINATOR;
  return SOAP_OK;
}


// Web service operation 'SuperGroupHourlyInfo' (returns error code or SOAP_OK)
int CsaWebServer::SuperGroupHourlyInfo(ns1__GenInfoRequestType *ns1__SuperGroupHourlyInfoRequest, ns1__ReturnMatrixDataTypePerSuperGroup &ns1__SuperGroupHourlyInfoResponse)
{
  SuperGroupHourlyReply  ReplyObj;
  CsaRc_t           rc = ARC_OK;
  //GroupRecord*      grec;
  unsigned int      ret_index=0;

  csa_web_serverMT_log_i << "Invoke CsaWebServer::GroupHourlyInfo (group size " << ns1__SuperGroupHourlyInfoRequest->Id.size() << ")\n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__SuperGroupHourlyInfoRequest == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault o Request Data. CsaWebServer::GroupHourlyInfo \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data", "ns1__SuperGroupHourlyInfoRequest is NULL");
  }


  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__SuperGroupHourlyInfoResponse.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__SuperGroupHourlyInfoResponse.responseInfoHeader->serviceReqId = ns1__SuperGroupHourlyInfoRequest->requestInfoHeader->serviceReqId;

  if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__SuperGroupHourlyInfoRequest->requestInfoHeader->ticket))
  {
    ns1__SuperGroupHourlyInfoResponse.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::SuperGroupHourlyInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  if(!m_db_data_ptr->supergrouptab.csa_web_TryLock())	//MT
  {
    ns1__SuperGroupHourlyInfoResponse.responseInfoHeader->errorCause = (unsigned int)700;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT//MT BUSY
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::SuperGroupHourlyInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  //  body
  for (unsigned int i=0; i<ns1__SuperGroupHourlyInfoRequest->Id.size(); i++)
  {
    m_db_data_ptr->limitstab.VerifyGroupInLimitTab(ns1__SuperGroupHourlyInfoRequest->Id[i]); //PVL 28-07-2009 1002567
    ns1__SuperGroupHourlyInfoResponse.responseInfoHeader->errorCause = (unsigned int)0;
    ReplyObj.AddNewSuperGroupBlock(this,ns1__SuperGroupHourlyInfoRequest->Id[i],ns1__SuperGroupHourlyInfoResponse);
    ReplyObj.InsertDataToRows(this,ns1__SuperGroupHourlyInfoRequest->Id[i],ns1__SuperGroupHourlyInfoResponse,i);
  }

  m_db_data_ptr->supergrouptab.csa_web_UnLock();//MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__SuperGroupHourlyInfoRequest->responseInfoHeader->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK. CsaWebServer::SuperGroupHourlyInfo \n" << LOG_TERMINATOR;
  return SOAP_OK;
}

/// Web service operation 'SuperGroupDailyBriefInfo' (returns error code or SOAP_OK)
int CsaWebServer::SuperGroupDailyBriefInfo(ns1__GenInfoRequestType *ns1__SuperGroupDailyBriefInfoRequest, ns1__ReturnArrayDataType &ns1__SuperGroupDailyBriefInfoResponse)
{
  SuperGroup_Reply    ReplyObj;
  CsaRc_t             rc = ARC_OK;
  SuperGroupRecord*   sg_rec_arr_ptr = NULL;
  //GroupRecord*        grec;
  unsigned int        ret_index=0; //PVL 14-06-2010 1004180 not valid index problem

  csa_web_serverMT_log_i << "Invoke CsaWebServer::SuperGroupDailyBriefInfo \n" << LOG_TERMINATOR;
  //Fool protector
  if(ns1__SuperGroupDailyBriefInfoRequest == NULL)
  {
    csa_web_serverMT_log_i << "soap_receiver_fault No Request Data. CsaWebServer::SuperGroupDailyBriefInfo \n" << LOG_TERMINATOR;
    return soap_receiver_fault(this, "No Request Data", "ns1__SuperGroupDailyBriefInfoRequest is NULL");
  }

  //m_db_data_ptr->TraceTab.StartConnection(this->ip);//PVL_STAT
  ns1__SuperGroupDailyBriefInfoResponse.responseInfoHeader = m_net_data_ptr->ResponseHeader;
  ns1__SuperGroupDailyBriefInfoResponse.responseInfoHeader->serviceReqId = ns1__SuperGroupDailyBriefInfoRequest->requestInfoHeader->serviceReqId;

  if(!m_db_data_ptr->TicketTab.VerifyTicket(ns1__SuperGroupDailyBriefInfoRequest->requestInfoHeader->ticket))
  {
    ns1__SuperGroupDailyBriefInfoResponse.responseInfoHeader->errorCause = 500;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,500);//PVL_STAT
    csa_web_serverMT_log_i << "errorCause = 500. CsaWebServer::SuperGroupDailyBriefInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }
  if(!m_db_data_ptr->supergrouptab.csa_web_TryLock())	//MT
  {
    ns1__SuperGroupDailyBriefInfoResponse.responseInfoHeader->errorCause = (unsigned int)700;
    //m_db_data_ptr->TraceTab.EndConnection(this->ip,700);//PVL_STAT//MT BUSY
    csa_web_serverMT_log_i << "errorCause = 700. CsaWebServer::SuperGroupDailyBriefInfo \n" << LOG_TERMINATOR;
    return SOAP_OK;
  }

  csa_web_serverMT_log_d << "ns1__SuperGroupDailyBriefInfoRequest->Id.size() " << ns1__SuperGroupDailyBriefInfoRequest->Id.size() << "\n SG Ids: ";
  //start PVL 14-06-2010 1004180 mailbox
  for (unsigned int i=0; i<ns1__SuperGroupDailyBriefInfoRequest->Id.size(); i++)
  {
    if(i > 0)
      csa_web_serverMT_log_d << ",";
    csa_web_serverMT_log_d << ns1__SuperGroupDailyBriefInfoRequest->Id[i];
  }
  csa_web_serverMT_log_d << LOG_TERMINATOR;

  csa_web_serverMT_log_d << "Start Iterating - ns1__SuperGroupDailyBriefInfoRequest->Id" << LOG_TERMINATOR;

  //for (unsigned int i=0; i<ns1__SuperGroupDailyBriefInfoRequest->Id.size(); i++)
  //{
  //  m_db_data_ptr->limitstab.VerifyGroupInLimitTab(ns1__SuperGroupDailyBriefInfoRequest->Id[i]); //PVL 28-07-2009 1002567
  //  rc = Csa_Super_Grp_Tbl[TBL_DAILY_SUM]->get(ns1__SuperGroupDailyBriefInfoRequest->Id[i], &sg_rec_arr_ptr);
  //  if (rc == ARC_OK)
  //  {
  //    ReplyObj.AddNewSuperGroupBlock(this,sg_rec_arr_ptr->getSuperGroupId(),ns1__SuperGroupDailyBriefInfoResponse);
  //    ns1__SuperGroupDailyBriefInfoResponse.responseInfoHeader->errorCause = (unsigned int)0;
  //    ReplyObj.MakePerGroup_QueInfo(this,sg_rec_arr_ptr,ns1__SuperGroupDailyBriefInfoResponse.returnArrayOfBlocks[ret_index]); //PVL 14-06-2010 1004180 not valid index problem
  //    ReplyObj.MakePerGroup_AgentsInfo(this,sg_rec_arr_ptr,ns1__SuperGroupDailyBriefInfoResponse.returnArrayOfBlocks[ret_index]); //PVL 14-06-2010 1004180 not valid index problem
  //    ReplyObj.MakePerGroup_ShiftInfo(this,sg_rec_arr_ptr,ns1__SuperGroupDailyBriefInfoResponse.returnArrayOfBlocks[ret_index]); //PVL 14-06-2010 1004180 not valid index problem
  //    ReplyObj.MakePerGroup_InterflInfo(this,sg_rec_arr_ptr,ns1__SuperGroupDailyBriefInfoResponse.returnArrayOfBlocks[ret_index]); //PVL 14-06-2010 1004180 not valid index problem
  //    ReplyObj.MakePerGroup_AbandInfo(this,sg_rec_arr_ptr,ns1__SuperGroupDailyBriefInfoResponse.returnArrayOfBlocks[ret_index]); //PVL 14-06-2010 1004180 not valid index problem

  //    ret_index++; //PVL 14-06-2010 1004180 not valid index problem
  //  }
  //  else
  //  {
  //    ns1__SuperGroupDailyBriefInfoResponse.responseInfoHeader->errorCause = (unsigned int)1;
  //  }
  //}


  sg_rec_arr_ptr = new SuperGroupRecord[ns1__SuperGroupDailyBriefInfoRequest->Id.size()];
  ns1__SuperGroupDailyBriefInfoResponse.responseInfoHeader->errorCause = (unsigned int)0;

  //12-Sep-2022 YR BZ#56823
  char servers_info[256];
  memset(servers_info, 0, 256);
  AddServerTimeToServersInfo(servers_info);
  ns1__SuperGroupDailyBriefInfoResponse.responseInfoHeader->serversInfo = servers_info;

  for( unsigned int i=0; i<ns1__SuperGroupDailyBriefInfoRequest->Id.size(); i++ )
  {
    if(ReplyObj.AddNewSuperGroupBlock(this,ns1__SuperGroupDailyBriefInfoRequest->Id[i],ns1__SuperGroupDailyBriefInfoResponse))
    {
      sg_rec_arr_ptr[i].setSuperGroupId(ns1__SuperGroupDailyBriefInfoRequest->Id[i]);
      sg_rec_arr_ptr[i].Clear();

      rc = ReplyObj.MakeSuperGroupDailyInfo(this,&sg_rec_arr_ptr[i],ns1__SuperGroupDailyBriefInfoResponse.returnArrayOfBlocks[ret_index]);
      if (rc != ARC_OK)
      {
        ReplyObj.MakeSuperGroupInfoEMPTY(this,sg_rec_arr_ptr[i].getSuperGroupId(),ns1__SuperGroupDailyBriefInfoResponse.returnArrayOfBlocks[ret_index]);  //PVL 14-06-2010 1004180
        ns1__SuperGroupDailyBriefInfoResponse.responseInfoHeader->errorCause = (unsigned int)2;
      }
      ret_index++;
    }
  }

  delete []sg_rec_arr_ptr;
  m_db_data_ptr->supergrouptab.csa_web_UnLock();//MT
  //m_db_data_ptr->TraceTab.EndConnection(this->ip,ns1__SuperGroupDailyBriefInfoResponse->responseInfoHeader->errorCause);//PVL_STAT
  csa_web_serverMT_log_i << "SOAP_OK CsaWebServer::SuperGroupDailyBriefInfo \n" << LOG_TERMINATOR;
  return SOAP_OK;
}


//9-Aug-2022 YR BZ#56641
void AddServerTimeToServersInfo(char* server_info)
{
  char        timebuf[20];
  time_t      date_time;
  struct tm*  current_time;

  /////////////////////////////////////////////
  time(&date_time);
  current_time = er_localtime(&date_time);  /* Convert time to struct tm form */
  strftime(timebuf, 30, "%H:%M:%S", current_time);

  if(strlen(server_info) > 0)
    er_sprintf(&server_info[strlen(server_info)], ",%s", timebuf);
  else
    er_sprintf(&server_info[strlen(server_info)], "%s", timebuf);
}

//18-Apr-2023 YR BZ#
void AddDPToServersInfo_AppCenter(char* server_info)
{
  er_sprintf(&server_info[strlen(server_info)], ",%s:%s", EosDPGetParamName(EOS_DP_APP_CENTER), EosDPGetStatusName(EosDPGetAACStatus(__FILE__, __LINE__)));
}

