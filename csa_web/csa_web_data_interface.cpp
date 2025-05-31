#include "er_std_override.h"
#include "csa_web_data_interface.hpp"


static BswLog_c csa_web_data_interface_log_d(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_DETAILED);
static BswLog_c csa_web_data_interface_log_i(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_INFO);
static BswLog_c csa_web_data_interface_log_e(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_ERROR);


//-------------------------------------------------------------------------------------------
// Insert Data to Reply Array
//long
void DataItemType_inter::InsertFullData(CsaWebServer *soap,std::string ersid,std::string ersname,Ulong_t value,ns1__BlockItemType *ReturnArrayData)
{
  ns1__DataItemType *  tmp;
  tmp = soap_new_ns1__DataItemType(soap, 1);
  tmp->ersid = ersid;
  tmp->ersname = ersname;
  //tmp->value = Ulong2String((unsigned long) value);
  Ulong2String(value,tmp->value);
  //soap->m_net_data_ptr->DIT_vector.push_back(tmp);
  ReturnArrayData->returnArray.push_back(tmp);
}


//long
void DataItemType_inter::InsertFullDataBG(CsaWebServer *soap,unsigned int * bgcolor_ptr, std::string ersid,std::string ersname,Ulong_t value,ns1__BlockItemType *ReturnArrayData)
{
  ns1__DataItemType *  tmp;
  tmp = soap_new_ns1__DataItemType(soap, 1);
  tmp->ersid = ersid;
  tmp->ersname = ersname;
  //tmp->value = Ulong2String((unsigned long) value);
  Ulong2String(value,tmp->value);
  if(bgcolor_ptr!=NULL){
    tmp->bgcolor = bgcolor_ptr;
  }
  //soap->m_net_data_ptr->DIT_vector.push_back(tmp);
  ReturnArrayData->returnArray.push_back(tmp);
}


//string
void DataItemType_inter::InsertFullData(CsaWebServer *soap,std::string ersid,std::string ersname,std::string value,ns1__BlockItemType *ReturnArrayData)
{
  ns1__DataItemType *  tmp;
  tmp = soap_new_ns1__DataItemType(soap, 1);
  tmp->ersid = ersid;
  tmp->ersname = ersname;
  tmp->value = value;
  //soap->m_net_data_ptr->DIT_vector.push_back(tmp);
  ReturnArrayData->returnArray.push_back(tmp);
}


//string
void DataItemType_inter::InsertFullDataBG(CsaWebServer *soap,unsigned int * bgcolor_ptr, std::string ersid,std::string ersname,std::string value,ns1__BlockItemType *ReturnArrayData)
{
  ns1__DataItemType *  tmp;
  tmp = soap_new_ns1__DataItemType(soap, 1);
  tmp->ersid = ersid;
  tmp->ersname = ersname;
  tmp->value = value;
  if(bgcolor_ptr!=NULL){
    tmp->bgcolor = bgcolor_ptr;
  }
  //soap->m_net_data_ptr->DIT_vector.push_back(tmp);
  ReturnArrayData->returnArray.push_back(tmp);
}


///
bool DataItemType_inter::AddNewGroupBlock(CsaWebServer *soap,unsigned int groupId, ns1__ReturnArrayDataType &ReturnData)
{
  //26-Oct-2021 YR BZ#55354
  if(soap->m_db_data_ptr->groupstab.IsGroupExist(groupId))
  {
    ns1__BlockItemType*   tmp;
    tmp = soap_new_ns1__BlockItemType(soap, 1);
    tmp->Id = groupId;
    tmp->Name = soap->m_db_data_ptr->groupstab.GetGroupNameById(groupId);
    //soap->m_net_data_ptr->BLOCK_vector.push_back(tmp);
    csa_web_data_interface_log_d << "DataItemType_inter::AddNewGroupBlock <" << tmp->Id << "," << tmp->Name.c_str() << ">" << LOG_TERMINATOR;
    ReturnData.returnArrayOfBlocks.push_back(tmp);
    return true;
  }
  else
    csa_web_data_interface_log_d << "DataItemType_inter::AddNewGroupBlock <" << groupId << "> - NOT ADDED (not in list)" << LOG_TERMINATOR;
  return false;
}


//IVR APPlicatio
void DataItemType_inter::AddNewIvrAppBlock(CsaWebServer *soap,unsigned int IvrAppId,ns1__ReturnArrayDataType &ReturnData)
{
  ns1__BlockItemType*   tmp;
  tmp = soap_new_ns1__BlockItemType(soap, 1);
  tmp->Id = IvrAppId;
  tmp->Name = soap->m_db_data_ptr->ivrapptab.GetName(IvrAppId);
  //soap->m_net_data_ptr->BLOCK_vector.push_back(tmp);
  ReturnData.returnArrayOfBlocks.push_back(tmp);
}


//IVR GRPlicatio
void DataItemType_inter::AddNewIvrGrpBlock(CsaWebServer *soap,unsigned int IvrGrpId,ns1__ReturnArrayDataType &ReturnData)
{
  ns1__BlockItemType*   tmp;
  tmp = soap_new_ns1__BlockItemType(soap, 1);
  tmp->Id = IvrGrpId;
  tmp->Name = soap->m_db_data_ptr->ivrgrptab.GetName(IvrGrpId);
  //soap->m_net_data_ptr->BLOCK_vector.push_back(tmp);
  ReturnData.returnArrayOfBlocks.push_back(tmp);
}


//MAILBOX GRPlicatio
void DataItemType_inter::AddNewMailAccBlock(CsaWebServer *soap,unsigned int MailAccId,ns1__ReturnArrayDataType &ReturnData)
{
  ns1__BlockItemType*   tmp;
  tmp = soap_new_ns1__BlockItemType(soap, 1);
  tmp->Id = MailAccId;
  tmp->Name = soap->m_db_data_ptr->mailboxacctab.GetName(MailAccId);
  //soap->m_net_data_ptr->BLOCK_vector.push_back(tmp);
  ReturnData.returnArrayOfBlocks.push_back(tmp);
}

///
bool DataItemType_inter::AddNewSuperGroupBlock(CsaWebServer *soap,unsigned int SuperGroupId, ns1__ReturnArrayDataType &ReturnData)
{
  //26-Oct-2021 YR BZ#55354
  if(soap->m_db_data_ptr->supergrouptab.IsSuperGroupExist(SuperGroupId))
  {
    ns1__BlockItemType*   tmp;
    tmp = soap_new_ns1__BlockItemType(soap, 1);
    tmp->Id = SuperGroupId;
    tmp->Name = soap->m_db_data_ptr->supergrouptab.GetName(SuperGroupId);
    //soap->m_net_data_ptr->BLOCK_vector.push_back(tmp);
    ReturnData.returnArrayOfBlocks.push_back(tmp);
    csa_web_data_interface_log_d << "DataItemType_inter::AddNewSuperGroupBlock <" << tmp->Id << "," << tmp->Name.c_str() << "> - Num of blocks after " << ReturnData.returnArrayOfBlocks.size() << LOG_TERMINATOR;
    return true;
  }
  else
    csa_web_data_interface_log_d << "DataItemType_inter::AddNewSuperGroupBlock <" << SuperGroupId << "> - NOT ADDED (not in list)" << LOG_TERMINATOR;
  return false;
}


