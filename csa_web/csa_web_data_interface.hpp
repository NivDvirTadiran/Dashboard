#ifndef CSA_WEB_DATA_ITEM_TYPE_INT_H
#define CSA_WEB_DATA_ITEM_TYPE_INT_H
#include "csa_web_serverMT.hpp"
#include "csa_web_commondefs.hpp"
#include <vector>



class DataItemType_inter{

//============================ Public Section ========================
public:
//long
void InsertFullData(CsaWebServer *soap, std::string ersid,std::string ersname,Ulong_t value,ns1__BlockItemType *ReturnArrayData);
void InsertFullDataBG(CsaWebServer *soap,unsigned int * bgcolor, std::string ersid,std::string ersname,Ulong_t value,ns1__BlockItemType *ReturnArrayData);
//string
void InsertFullData(CsaWebServer *soap, std::string ersid,std::string ersname,std::string value,ns1__BlockItemType *ReturnArrayData);
void InsertFullDataBG(CsaWebServer *soap,unsigned int * bgcolor, std::string ersid,std::string ersname,std::string value,ns1__BlockItemType *ReturnArrayData);
//Block
bool AddNewGroupBlock(CsaWebServer *soap,unsigned int groupId,ns1__ReturnArrayDataType &ReturnData);
void AddNewIvrAppBlock(CsaWebServer *soap,unsigned int IvrAppId,ns1__ReturnArrayDataType &ReturnData);	
void AddNewIvrGrpBlock(CsaWebServer *soap,unsigned int IvrGrpId,ns1__ReturnArrayDataType &ReturnData);
void AddNewMailAccBlock(CsaWebServer *soap,unsigned int MailAccId,ns1__ReturnArrayDataType &ReturnData);
bool AddNewSuperGroupBlock(CsaWebServer *soap,unsigned int SuperGroupId,ns1__ReturnArrayDataType &ReturnData);
};
#endif
