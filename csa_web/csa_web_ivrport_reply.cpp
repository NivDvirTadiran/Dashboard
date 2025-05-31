#include "er_std_override.h"
#include "csa_web_ivr_reply.hpp"
#include "csa_web_commondefs.hpp"

//------------------------------------------------------------------------------------------------
CsaRc_t  IVRGrpPortReply::MakeIvrGrpPortInfo(CsaWebServer *soap,CsaIvrPortRecord_c* ivr_port_rec, ns1__BlockItemType *ReturnArrayData )
{
  CsaRc_t   rc = ARC_OK;

  // Make ERS_ID

  unsigned int x = 0;
  CsaOnlineIvrPortStatistics_t  *event_data = NULL;
  string State_name;
  string State_Time;
  unsigned long  state_delta_time=0;
  unsigned int *StateColor=NULL;
  unsigned int  *TmpColorPtr = NULL;

  rc = ivr_port_rec->makeEventData(event_data);
  if (rc == ARC_OK)
  {
    InsertFullDataBG(soap,NULL,"f6_3_12_1_1","IVR Port No.",(char*)(event_data->port_dn),ReturnArrayData);

    soap->m_db_data_ptr->colortab.GetBGColorsByState(event_data->state, 0, &StateColor, State_name);
    InsertFullDataBG(soap,StateColor,"f6_3_12_2_1","State",State_name,ReturnArrayData);

    state_delta_time = NlaCalcCsaElapsedTime((unsigned long)event_data->state_begin_time);
    InsertFullDataBG(soap,NULL,"f6_3_12_2_2","State Time",state_delta_time,ReturnArrayData);
  }

  return rc;
}


//------------------------------------------------------------------------------------------------
CsaRc_t  IVRGrpPortReply::MakeIvrGrpInfo  (CsaWebServer *soap,CsaIvrGroupRecord_c* ivr_grp_rec, ns1__BlockItemType *ReturnArrayData )
{
  CsaRc_t                                        rc = ARC_OK;
  CsaOnlineIvrPortsGroupStatistics_t                     *event_data = NULL;
  rc = ivr_grp_rec->makeEventData(event_data);
  if (rc == ARC_OK)
  {
    InsertFullDataBG(soap,NULL,"f6_3_13_1_1","Group Name",  soap->m_db_data_ptr->ivrgrptab.GetName(event_data->group_id), ReturnArrayData);
    InsertFullDataBG(soap,NULL,"f6_3_13_1_2","IVR Ports",   event_data->total_num_of_ports, ReturnArrayData);
    InsertFullDataBG(soap,NULL,"f6_3_13_2_1","Idle",        event_data->num_in_idle_state,  ReturnArrayData); //PVL 20-06-2010 1004048
    InsertFullDataBG(soap,NULL,"f6_3_13_2_2","ACD",         event_data->num_in_acd_state,   ReturnArrayData);
    InsertFullDataBG(soap,NULL,"f6_3_13_2_3","O-ACD",       event_data->num_in_oacd_state,  ReturnArrayData);
    InsertFullDataBG(soap,NULL,"f6_3_13_2_4","Error",       event_data->num_in_error_state, ReturnArrayData);
  }
  return rc;
}


//------------------------------------------------------------------------------------------------
void   IVRGrpPortReply::InsertIVRGrpPortList(CsaWebServer*            soap,
                                             Id_type                  Id,
                                             std::string              Name,
                                             ns1__GenListReturnType&  ReturnArrayData)
{
  ns1__GenListDataItemType* tmp;
  tmp = soap_new_ns1__GenListDataItemType(soap, 1);

  tmp->Id = Id;
  tmp->Name = Name;

  soap->m_net_data_ptr->GENL_vector.push_back(tmp);
  ReturnArrayData.returnArray.push_back(tmp);
}


//--------------------------------------------------------------------------
CsaRc_t IVRGrpPortReply::StartMonitorGroup(unsigned int group_id)
{ //we can send multiple messages  to ARMAIN.cpp according the same group_id ,
  // becouse of internal validation in setNlaDumpOn() ,
  CsaRc_t           rc = ARC_OK;
  EosEventHeader_t  event_header;
  EosRetCode_t      erc;
  NLA_CSA_001       reg_req;

  event_header.dest_info = -1; //PVL 7-02-2010
  event_header.op_code = WEB_CSA_reg_for_ivr_group_req ;
  event_header.dest_que_id = CSA_Q4;
  event_header.reply_que_id = 0;//DUMMY , not in use  //RBE_Q2;
  event_header.data_len = sizeof(reg_req);
  reg_req.entity_id = group_id;
  reg_req.refresh_interval = 1;
  reg_req.tbl_index = 0;
  erc = EosCreateAndSendEvent (NLA_TH1, event_header.dest_que_id , event_header, &reg_req, event_header.data_len);
  return rc;
}


//------------------------------------------------------------------------------------------------
CsaRc_t   IVRGrpPortReply::StartMonitorPort(unsigned int group_id)
{ //we can send multiple messages  to ARMAIN.cpp according the same group_id ,
  // becouse of internal validation in setNlaDumpOn() ,
  CsaRc_t           rc = ARC_OK;
  EosEventHeader_t  event_header;
  EosRetCode_t      erc;
  NLA_CSA_001       reg_req;

  event_header.dest_info = -1; //PVL 7-02-2010
  event_header.op_code = WEB_CSA_reg_for_ivr_ports_req ;
  event_header.dest_que_id = CSA_Q4;
  event_header.reply_que_id = 0;//DUMMY , not in use  //RBE_Q2;
  event_header.data_len = sizeof(reg_req);
  reg_req.entity_id = group_id;
  reg_req.refresh_interval = 1;
  reg_req.tbl_index = 0;
  erc = EosCreateAndSendEvent (NLA_TH1, event_header.dest_que_id , event_header, &reg_req, event_header.data_len);
  return rc;
}
