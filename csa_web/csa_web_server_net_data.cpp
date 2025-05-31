#include "er_std_override.h"
#include "csa_web_server_net_data.hpp"
#include "csa_web_main.h"

SrvNetData_c::SrvNetData_c()
{
  //printf("START SrvNetData_c::SrvNetData_c() \r\n");
  //BLOCK_vector.clear();
  //MATRIX_PER_GROUP_vector.clear();
  //DIT_vector.clear();
  //ROW_vector.clear();
  //GroupListType_vector.clear();
  //AgentsListType_vector.clear();
  //DW_vector.clear();
  //RLT_map.clear();
  //DNISL_vector.clear();
  //GENL_vector.clear();
  //STAT_vector.clear();
  //printf("END SrvNetData_c::SrvNetData_c() \r\n");
}


//==========================[ Static Variables ]======================
static BswLog_c   csa_web_net_data_log_d(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_DETAILED);
static BswLog_c   csa_web_net_data_log_i(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_INFO);


//-----------------------------------------------------------------------
void SrvNetData_c::CreateResponseHeader(struct soap *soap)
{
  ResponseHeader = soap_new_ns1__ResponseInfoHeaderType(soap,1);
}


//-----------------------------------------------------------------------
void SrvNetData_c::CleanAll(struct soap *soap,int tid)
{
  char address[20];
  er_sprintf(address, "%p", soap);
  csa_web_net_data_log_d << "SrvNetData_c::CleanAll (Address of soap " << address << ")\n" << LOG_TERMINATOR;

  //printf("                  +++++++++++++  %d START SrvNetData_c::CleanAll  tid:[%d]  \r\n",time(0),tid);

  //if(ResponceHeader != NULL)
  //  soap_delete_ns1__ResponceInfoHeaderType( soap,ResponceHeader);

  //-------
  if(BLOCK_vector.size() > 0)
  {
    csa_web_net_data_log_d << "SrvNetData_c::CleanAll - Clear BLOCK_vector (size " << BLOCK_vector.size() << ")\n" << LOG_TERMINATOR;
    //for(BLOCK_iter=BLOCK_vector.begin(); BLOCK_iter!= BLOCK_vector.end(); ++BLOCK_iter)
    //  soap_delete_ns1__BlockItemType( soap,*BLOCK_iter);
    BLOCK_vector.clear();
  }
  //printf("                  +++++++++++++ %d AFTER BLOCK_vector tid:[%d]\r\n",time(0),tid);
  //-------

  if(MATRIX_PER_GROUP_vector.size() > 0)
  {
    csa_web_net_data_log_d << "SrvNetData_c::CleanAll - Clear MATRIX_PER_GROUP_vector (size " << MATRIX_PER_GROUP_vector.size() << ")\n" << LOG_TERMINATOR;

    for(MATRIX_PER_GROUP_iter=MATRIX_PER_GROUP_vector.begin(); MATRIX_PER_GROUP_iter!= MATRIX_PER_GROUP_vector.end(); ++MATRIX_PER_GROUP_iter)
    {
      std::vector<ns1__DataItemRow * >::iterator  it = (*MATRIX_PER_GROUP_iter)->returnMatrix.begin();
      printf("                  +++++++++++++ %d  MATRIX__vector tid:[%d] no of fileds:[%d]\r\n",time(0),tid,(*MATRIX_PER_GROUP_iter)->returnMatrix.size());
      //for(it;it != (*MATRIX_PER_GROUP_iter)->returnMatrix.end();++it)
      //{
      //  soap_delete_ns1__DataItemType(soap,*it);
      //}
      (*MATRIX_PER_GROUP_iter)->returnMatrix.clear();
      //soap_delete_ns1__returnDataRowsPerGroup( soap,*MATRIX_PER_GROUP_iter);
    }
  }

  MATRIX_PER_GROUP_vector.clear();

  //printf("                  +++++++++++++ %d AFTER MATRIX_PER_GROUP_vector tid:[%d]\r\n",time(0),tid);
  //-------

  if(DIT_vector.size() > 0)
  {
    csa_web_net_data_log_d << "SrvNetData_c::CleanAll - Clear DIT_vector (size " << DIT_vector.size() << ")\n" << LOG_TERMINATOR;
    //for(DIT_iter=DIT_vector.begin(); DIT_iter!= DIT_vector.end(); ++DIT_iter)
    //  soap_delete_ns1__DataItemType( soap,*DIT_iter);
    DIT_vector.clear();
  }

  //printf("                  +++++++++++++ %d AFTER DIT_vector tid:[%d]\r\n",time(0),tid);
  //-------

  if(ROW_vector.size() > 0)
  {
    csa_web_net_data_log_d << "SrvNetData_c::CleanAll - Clear ROW_vector (size " << ROW_vector.size() << ")\n" << LOG_TERMINATOR;

    int kkk = 0;
    for(ROW_iter=ROW_vector.begin(); ROW_iter!= ROW_vector.end(); ++ROW_iter)
    {
      std::vector<ns1__DataItemType * >::iterator it = (*ROW_iter)->returnArray.begin();
      printf("                  +++++++++++++[%d]  %d   IROW_vector tid:[%d] no of fileds:[%d]\r\n",++kkk,time(0),tid,(*ROW_iter)->returnArray.size());
      //for(it;it != (*ROW_iter)->returnArray.end();++it)
      //{
      //  ns1__DataItemType *item = (ns1__DataItemType *)(*it);
      //  if (item->bgcolor)
      //  {
      //    free (item->bgcolor);
      //  }
      //  soap_delete_ns1__DataItemType(soap,*it);
      //}
      (*ROW_iter)->returnArray.clear();
      //soap_delete_ns1__DataItemRow( soap,*ROW_iter);
    }
    ROW_vector.clear();
  }

  //printf("                  +++++++++++++ %d AFTER IROW_vector tid:[%d]\r\n",time(0),tid);
  //-------

  if(GroupListType_vector.size() > 0)
  {
    csa_web_net_data_log_d << "SrvNetData_c::CleanAll - Clear GroupListType_vector (size " << GroupListType_vector.size() << ")\n" << LOG_TERMINATOR;

    //for(GroupListType_iter=GroupListType_vector.begin(); GroupListType_iter!= GroupListType_vector.end(); ++GroupListType_iter)
    //  soap_delete_ns1__GroupListDataItemType( soap,*GroupListType_iter);
    GroupListType_vector.clear();
  }

  //printf("                  +++++++++++++ %d AFTER GroupListType_vector tid:[%d]\r\n",time(0),tid);
  //-------

  if(AgentsListType_vector.size() > 0)
  {
    csa_web_net_data_log_d << "SrvNetData_c::CleanAll - Clear AgentsListType_vector (size " << AgentsListType_vector.size() << ")\n" << LOG_TERMINATOR;

    //for(AgentsListType_iter=AgentsListType_vector.begin(); AgentsListType_iter!= AgentsListType_vector.end(); ++AgentsListType_iter)
    //  soap_delete_ns1__AgentsListDataItemType( soap,*AgentsListType_iter );
    AgentsListType_vector.clear();
  }

  //printf("                  +++++++++++++ %d AFTER AgentsListType_vector tid:[%d]\r\n",time(0),tid);
  //-------

  if(DW_vector.size() > 0)
  {
    csa_web_net_data_log_d << "SrvNetData_c::CleanAll - Clear DW_vector (size " << DW_vector.size() << ")\n" << LOG_TERMINATOR;

   //for(DW_iter=DW_vector.begin(); DW_iter!= DW_vector.end(); ++DW_iter)
	//  soap_delete_ns1__DefaultWorkspaceItemType( soap,*DW_iter);
   DW_vector.clear();
  }

  //printf("                  +++++++++++++ %d AFTER DW_vector tid:[%d]\r\n",time(0),tid);
  //-------

  if(RLT_map.size() > 0)
  {
    csa_web_net_data_log_d << "SrvNetData_c::CleanAll - Clear RLT_map (size " << RLT_map.size() << ")\n" << LOG_TERMINATOR;

    //for(RLT_iter=RLT_map.begin(); RLT_iter!= RLT_map.end(); ++RLT_iter)
    //  soap_delete_ns1__ReportListDataItemType( soap,RLT_iter->second );
    RLT_map.clear();
  }

  //printf("                  +++++++++++++ %d AFTER RLT_iter tid:[%d]\r\n",time(0),tid);
  //-------

  if(DNISL_vector.size() > 0)
  {
    csa_web_net_data_log_d << "SrvNetData_c::CleanAll - Clear DNISL_vector (size " << DNISL_vector.size() << ")\n" << LOG_TERMINATOR;

   //for(DNISL_iter=DNISL_vector.begin(); DNISL_iter!= DNISL_vector.end(); ++DNISL_iter)
   // soap_delete_ns1__DNISListDataItemType(soap,*DNISL_iter);
   DNISL_vector.clear();
  }

  //printf("                  +++++++++++++ %d AFTER DNISL_vector tid:[%d]\r\n",time(0),tid);
  //-------

  if(GENL_vector.size() > 0)
  {
    csa_web_net_data_log_d << "SrvNetData_c::CleanAll - Clear GENL_vector (size " << GENL_vector.size() << ")\n" << LOG_TERMINATOR;

    //for(GENL_iter=GENL_vector.begin(); GENL_iter!= GENL_vector.end(); ++GENL_iter)
    //  soap_delete_ns1__GenListDataItemType(soap,*GENL_iter);
    GENL_vector.clear();
  }

  //printf("                  +++++++++++++ %d AFTER GENL_vector tid:[%d]\r\n",time(0),tid);
  //-------PVL_STAT
  if(STAT_vector.size() > 0)
  {
    csa_web_net_data_log_d << "SrvNetData_c::CleanAll - Clear STAT_vector (size " << STAT_vector.size() << ")\n" << LOG_TERMINATOR;

    //for(STAT_iter=STAT_vector.begin(); STAT_iter!= STAT_vector.end(); ++STAT_iter)
    //  soap_delete_ns1__StatItemType(soap,*STAT_iter);
    STAT_vector.clear();
  }

  //printf("END SrvNetData_c::CleanAll \r\n");
  //printf("                  +++++++++++++  %d END   SrvNetData_c::CleanAll  tid:[%d]  \r\n",time(0),tid);

  if(ResponseHeader != NULL)
  {
    csa_web_net_data_log_d << "SrvNetData_c::CleanAll - delete ResponseHeader\n" << LOG_TERMINATOR;
    delete []ResponseHeader;
    ResponseHeader = NULL;
  }

}
