#include "er_std_override.h"
#include "csa_web_mailbox_reply.hpp"
#include "csa_web_commondefs.hpp"

//------------------------------------------------------------------------------------------------
CsaRc_t  MAILbox_Reply::MakeMailboxInfo(CsaWebServer *soap,MailboxRecord *mailbox_rec, ns1__BlockItemType *ReturnArrayData )
{
   CsaRc_t                       rc = ARC_OK;   
   CsaOnlineMailboxStatistics_t *event_data = NULL;
   rc = mailbox_rec->makeEventData(event_data);
   if (rc == ARC_OK) {
	   InsertFullDataBG(soap,NULL,"f6_3_14_1_1","Mailbox Name",soap->m_db_data_ptr->mailboxacctab.GetName(event_data->mailbox_id),ReturnArrayData);        
	   InsertFullDataBG(soap,NULL,"f6_3_14_2_1","No.emails waiting",event_data->stats.num_waiting_emails,ReturnArrayData); 
	   InsertFullDataBG(soap,NULL,"f6_3_14_2_2","longest wait time",NlaCalcCsaElapsedTime((unsigned long)event_data->stats.longest_wait_time),ReturnArrayData); 
	   InsertFullDataBG(soap,NULL,"f6_3_14_3_1","No.emails entered",event_data->stats.num_emails_entered,ReturnArrayData); 
	   InsertFullDataBG(soap,NULL,"f6_3_14_3_2","No.emails diverted",event_data->stats.num_emails_diverted_to_agents,ReturnArrayData); 


   }

return rc;
}
//START PVL 14-06-2010 1004180
CsaRc_t  MAILbox_Reply::MakeMailboxInfoEMPTY(CsaWebServer *soap,unsigned int MailAccId, ns1__BlockItemType *ReturnArrayData )
{
	CsaRc_t                       rc = ARC_OK;   
	InsertFullDataBG(soap,NULL,"f6_3_14_1_1","Mailbox Name",soap->m_db_data_ptr->mailboxacctab.GetName(MailAccId),ReturnArrayData);        
	InsertFullDataBG(soap,NULL,"f6_3_14_2_1","No.emails waiting",0,ReturnArrayData); 
	InsertFullDataBG(soap,NULL,"f6_3_14_2_2","longest wait time",0,ReturnArrayData); 
	InsertFullDataBG(soap,NULL,"f6_3_14_3_1","No.emails entered",0,ReturnArrayData); 
	InsertFullDataBG(soap,NULL,"f6_3_14_3_2","No.emails diverted",0,ReturnArrayData); 
    return rc;
}
//END PVL 14-06-2010 1004180

void MAILbox_Reply::InsertMAILboxAccList(CsaWebServer *soap, 
		                unsigned int   Id,
						std::string    Name,
						ns1__GenListReturnType &ReturnArrayData)
{

    ns1__GenListDataItemType* tmp;
    tmp =  soap_new_ns1__GenListDataItemType(soap, 1);

    tmp->Id = Id; 
    tmp->Name = Name; 

    soap->m_net_data_ptr->GENL_vector.push_back(tmp);
    ReturnArrayData.returnArray.push_back(tmp); 
}

///////////////////////////////////////////////////////
CsaRc_t     MAILbox_Reply::StartMonitor         (unsigned int mailbox_id)
{ //we can send multiple messages  to ARMAIN.cpp according the same group_id ,
  // becouse of internal validation in setNlaDumpOn() ,
	 CsaRc_t                                        rc = ARC_OK;
		
    EosEventHeader_t               event_header;
    EosRetCode_t                   erc;
    NLA_CSA_001                    reg_req;

	event_header.dest_info = -1; //PVL 7-02-2010 
    event_header.op_code = WEB_CSA_reg_for_mailbox_req ;
    event_header.dest_que_id = CSA_Q4;
    event_header.reply_que_id = 0;//DUMMY , not in use  //RBE_Q2;
    event_header.data_len = sizeof(reg_req);
    reg_req.entity_id = mailbox_id;
    reg_req.refresh_interval = 1;
    reg_req.tbl_index = 0;
    erc = EosCreateAndSendEvent (NLA_TH1, event_header.dest_que_id , event_header, &reg_req, event_header.data_len);
	return rc;
}
