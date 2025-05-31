#ifndef CSA_WEB_SG_REPLY_H
#define CSA_WEB_SG_REPLY_H

#include "csa_web_serverMT.hpp"
//#include "csa_res_sgrp.h"
#include "arsupgrp.hpp"
#include "csa_web_data_interface.hpp"


class SuperGroup_Reply : public DataItemType_inter{ //: public EMISwebBindingService{

//============================ Public Section ========================
public:

  // ctor
  SuperGroup_Reply(){};
  //dstr
  ~SuperGroup_Reply(){};
  void InsertSuperGroupList(CsaWebServer *soap, Id_type Id,std::string Name,vector<Id_type> group_list, ns1__SuperGroupListReturnType &ReturnArrayData);
  CsaRc_t StartMonitor(unsigned int supergroup_id);
  CsaRc_t MakeSuperGroupInfo(CsaWebServer *soap,SuperGroupRecord *supergroup_rec, ns1__BlockItemType *ReturnArrayData );
  CsaRc_t MakeSuperGroupDailyInfo(CsaWebServer *soap,SuperGroupRecord *supergroup_rec, ns1__BlockItemType *ReturnArrayData );
  CsaRc_t MakeSuperGroupInfo_OnlineStat(CsaWebServer *soap,SuperGroupRecord *supergroup_rec, ns1__BlockItemType *ReturnArrayData );
  CsaRc_t MakeSuperGroupInfo_AcdStat(CsaWebServer *soap,SuperGroupRecord *supergroup_rec, ns1__BlockItemType *ReturnArrayData );
  CsaRc_t MakeSuperGroupInfo_AgentsAvailability(CsaWebServer *soap,SuperGroupRecord *supergroup_rec, ns1__BlockItemType *ReturnArrayData );
  CsaRc_t MakeSuperGroupInfoEMPTY(CsaWebServer *soap,unsigned int supergroup_id, ns1__BlockItemType *ReturnArrayData );
};


#endif
