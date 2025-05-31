#ifndef CSA_WEB_SERVER_NET_DATA_MT_H
#define CSA_WEB_SERVER_NET_DATA_MT_H
#include "acsasta.hpp"
#include "csa_web_main.h"
#ifdef WIN32
#include "windows/soapEMISwebBindingService.h"
#else
#include "linux/soapEMISwebBindingService.h"
#endif

class SrvNetData_c {
public:

//============================ Public Section ========================
 ns1__ResponseInfoHeaderType *ResponseHeader;

 vector<ns1__BlockItemType*> BLOCK_vector;
 vector<ns1__BlockItemType*>::iterator BLOCK_iter;

 vector<ns1__returnDataRowsPerGroup*> MATRIX_PER_GROUP_vector;
 vector<ns1__returnDataRowsPerGroup*>::iterator MATRIX_PER_GROUP_iter;

 vector<ns1__DataItemType*> DIT_vector;
 vector<ns1__DataItemType*>::iterator DIT_iter;

 vector<ns1__DataItemRow*> ROW_vector;
 vector<ns1__DataItemRow*>::iterator ROW_iter;

 vector<ns1__GroupListDataItemType*> GroupListType_vector;
 vector<ns1__GroupListDataItemType*>::iterator GroupListType_iter;

 vector<ns1__AgentsListDataItemType*> AgentsListType_vector;
 vector<ns1__AgentsListDataItemType*>::iterator AgentsListType_iter;

 vector<ns1__DefaultWorkspaceItemType*> DW_vector;
 vector<ns1__DefaultWorkspaceItemType*>::iterator  DW_iter;

 map<unsigned int,ns1__ReportListDataItemType* >RLT_map;
 map<unsigned int,ns1__ReportListDataItemType* >::iterator RLT_iter;

 vector<ns1__DNISListDataItemType*> DNISL_vector;
 vector<ns1__DNISListDataItemType*>::iterator  DNISL_iter;

 vector<ns1__GenListDataItemType*> GENL_vector;
 vector<ns1__GenListDataItemType*>::iterator  GENL_iter;

 vector<ns1__SuperGroupListDataItemType*> SGroupL_vector;
 vector<ns1__SuperGroupListDataItemType*>::iterator  SGroupL_iter;

 vector<ns1__StatItemType*> STAT_vector; //PVL_STAT
 vector<ns1__StatItemType*>::iterator  STAT_iter; //PVL_STAT

 SrvNetData_c(); //PVL
~SrvNetData_c(){};
void CreateResponseHeader(struct soap *soap);
 void CleanAll(struct soap *soap, int tid);
};

#endif
