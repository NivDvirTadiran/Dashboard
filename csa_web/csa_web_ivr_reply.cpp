#include "er_std_override.h"
#include "csa_web_ivr_reply.hpp"
#include "csa_web_commondefs.hpp"
extern "C" {
#include "oeve.h"
#include "oevebsw.h"
}
//------------------------------------------------------------------------------------------------
//CHANGED  PVL 11-8-2009 1002606
CsaRc_t  IVRApplicationReply::MakeIvrAppInfo    (CsaWebServer *soap,IvrAppRecord_c	*ivrapp, ns1__BlockItemType *ReturnArrayData )
{
CsaRc_t rc = ARC_OK;

// Make ERS_ID

unsigned int inBound_RPH =0;
unsigned int inBound_avg_handle_time =0;
unsigned int inBound_avg_fully_handle_time =0;
unsigned int inBound_avg_failed_handle_time =0;
unsigned int inBound_avg_interflowed_time =0;
unsigned int outBound_avg_handle_time=0;
unsigned int outBound_avg_fully_handle_time =0;
unsigned int outBound_avg_failed_handle_time =0;
unsigned int outBound_avg_interflowed_time =0;
unsigned int outBound_RPH =0;


CsaOnlineIvrApplicationStatistics_t	*event_data = NULL;
rc = ivrapp->makeEventData(event_data);
if(rc != ARC_OK)
   return rc;
//NAME
  InsertFullDataBG(soap,NULL,"f6_3_11_1_1","IVR App. Name",soap->m_db_data_ptr->ivrapptab.GetName(ivrapp->m_app_id),ReturnArrayData); 

  InsertFullDataBG(soap,NULL,"f6_3_11_1_2","Interval Time",ivrapp->Window_length,ReturnArrayData); 
//inBound
  InsertFullDataBG(soap,NULL,"f6_3_11_2_1","Calls",event_data->stats_per_call_type[inBound].current_active_calls,ReturnArrayData); 

  InsertFullDataBG(soap,NULL,"f6_3_11_2_3","Too Long ACD",event_data->stats_per_call_type[inBound].calls_longer_than_theshold,ReturnArrayData);

  InsertFullDataBG(soap,NULL,"f6_3_11_2_5","Avg.time",NlaCalcCsaElapsedTime(event_data->stats_per_call_type[inBound].average_handling_time_of_active_calls),ReturnArrayData); 
  InsertFullDataBG(soap,NULL,"f6_3_11_2_4","Max time",NlaCalcCsaElapsedTime(event_data->stats_per_call_type[inBound].longest_handle_time_of_active_call),ReturnArrayData); 
//-----------------
  if (event_data->stats_per_call_type[inBound].num_of_accesses > 0 )
	{
	  if (ivrapp->Window_length != 0 )
         inBound_RPH = (event_data->stats_per_call_type[inBound].num_of_accesses * 3600)/ (ivrapp->Window_length  * 60);
	}
	InsertFullDataBG(soap,NULL,"f6_3_11_3_18","RPH ACD",inBound_RPH,ReturnArrayData); 
//------------------
	InsertFullDataBG(soap,NULL,"f6_3_11_3_1","Times Accessed ACD",	event_data->stats_per_call_type[inBound].num_of_accesses,ReturnArrayData); 
  InsertFullDataBG(soap,NULL,"f6_3_11_3_4","Fully Handled ACD",		event_data->stats_per_call_type[inBound].num_fully_handled,ReturnArrayData); 
  InsertFullDataBG(soap,NULL,"f6_3_11_3_5","Failed ACD",			event_data->stats_per_call_type[inBound].num_failed_handled,ReturnArrayData); 
  InsertFullDataBG(soap,NULL,"f6_3_11_3_6","Interflowed ACD",		event_data->stats_per_call_type[inBound].num_interflowed,ReturnArrayData); 
//-----------------
  if (event_data->stats_per_call_type[inBound].num_of_accesses  !=0 )
	  inBound_avg_handle_time = event_data->stats_per_call_type[inBound].total_handle_time/
                                        event_data->stats_per_call_type[inBound].num_of_accesses ;
 
  InsertFullDataBG(soap,NULL,"f6_3_11_3_10","All/access",inBound_avg_handle_time,ReturnArrayData); 
//-----------------
  InsertFullDataBG(soap,NULL,"f6_3_11_3_11","Max Handled ACD",event_data->stats_per_call_type[inBound].longest_handle_time,ReturnArrayData); 
 
    if (event_data->stats_per_call_type[inBound].num_fully_handled!=0 )
         inBound_avg_fully_handle_time = event_data->stats_per_call_type[inBound].total_handle_time/
                                           event_data->stats_per_call_type[inBound].num_fully_handled;
    InsertFullDataBG(soap,NULL,"f6_3_11_3_12","Fully handled",inBound_avg_fully_handle_time,ReturnArrayData); 

  InsertFullDataBG(soap,NULL,"f6_3_11_3_13","longest_fully_handle_time",event_data->stats_per_call_type[inBound].longest_fully_handle_time,ReturnArrayData); 
  
  if (event_data->stats_per_call_type[inBound].num_failed_handled!= 0 )
         inBound_avg_failed_handle_time = event_data->stats_per_call_type[inBound].total_failed_handle_time /
                                             event_data->stats_per_call_type[inBound].num_failed_handled;
  InsertFullDataBG(soap,NULL,"f6_3_11_3_14","Avg. Failed",inBound_avg_failed_handle_time,ReturnArrayData); 
//------------------------
  InsertFullDataBG(soap,NULL,"f6_3_11_3_15","Failed",event_data->stats_per_call_type[inBound].longest_failed_handle_time,ReturnArrayData); 

    if (event_data->stats_per_call_type[inBound].num_interflowed != 0 )
         inBound_avg_interflowed_time =event_data->stats_per_call_type[inBound].total_interflowed_time /
                                             event_data->stats_per_call_type[inBound].num_interflowed ;
    InsertFullDataBG(soap,NULL,"f6_3_11_3_16","Avg. Interflowed",inBound_avg_interflowed_time,ReturnArrayData); 
//-------------------------------------
    InsertFullDataBG(soap,NULL,"f6_3_11_3_17","Long.Interflowed",event_data->stats_per_call_type[inBound].longest_interflowed_time,ReturnArrayData);//PVL 9-06-2010 1004145
 

//------------------------------

  // Out Bound
 InsertFullDataBG(soap,NULL,"f6_3_11_2_2","Calls OACD", event_data->stats_per_call_type[outBound].current_active_calls,ReturnArrayData); 
 InsertFullDataBG(soap,NULL,"f6_3_11_2_6","Too Long OACD",event_data->stats_per_call_type[outBound].calls_longer_than_theshold,ReturnArrayData); 
 InsertFullDataBG(soap,NULL,"f6_3_11_2_8","Avg.time OACD",event_data->stats_per_call_type[outBound].average_handling_time_of_active_calls,ReturnArrayData); 
 InsertFullDataBG(soap,NULL,"f6_3_11_2_7","Max time OACD",event_data->stats_per_call_type[outBound].longest_handle_time_of_active_call,ReturnArrayData); 
  if ( event_data->stats_per_call_type[outBound].num_of_accesses > 0 )
	{
	if (ivrapp->Window_length != 0 )
         inBound_RPH =  (event_data->stats_per_call_type[outBound].num_of_accesses * 3600)/ (ivrapp->Window_length  * 60);
	}
    InsertFullDataBG(soap,NULL,"f6_3_11_3_36","RPH OACD",outBound_RPH,ReturnArrayData); 
 InsertFullDataBG(soap,NULL,"f6_3_11_3_19","Times Accessed OACD", event_data->stats_per_call_type[outBound].num_of_accesses,ReturnArrayData); 
 InsertFullDataBG(soap,NULL,"f6_3_11_3_21","Fully Handled OACD", event_data->stats_per_call_type[outBound].num_fully_handled,ReturnArrayData); 
 InsertFullDataBG(soap,NULL,"f6_3_11_3_23","Failed OACD", event_data->stats_per_call_type[outBound].num_failed_handled,ReturnArrayData); 
 InsertFullDataBG(soap,NULL,"f6_3_11_3_24","Interflowed OACD",event_data->stats_per_call_type[outBound].num_interflowed,ReturnArrayData); 

 if (event_data->stats_per_call_type[outBound].num_of_accesses !=0)
         outBound_avg_handle_time  =  event_data->stats_per_call_type[outBound].total_handle_time /
                                         event_data->stats_per_call_type[outBound].num_of_accesses;
 InsertFullDataBG(soap,NULL,"f6_3_11_3_28","All/access",outBound_avg_handle_time,ReturnArrayData); 

 InsertFullDataBG(soap,NULL,"f6_3_11_3_29","Max Handled OACD", event_data->stats_per_call_type[outBound].longest_handle_time,ReturnArrayData); 


 if (event_data->stats_per_call_type[outBound].num_fully_handled  !=0)
         outBound_avg_fully_handle_time =  event_data->stats_per_call_type[outBound].total_fully_handle_time /
                                           event_data->stats_per_call_type[outBound].num_fully_handled;
 InsertFullDataBG(soap,NULL,"f6_3_11_3_30","Fully handled",outBound_avg_fully_handle_time,ReturnArrayData); 

 InsertFullDataBG(soap,NULL,"f6_3_11_3_31","longest_fully_handle_time", event_data->stats_per_call_type[outBound].longest_fully_handle_time,ReturnArrayData); 


  if ( event_data->stats_per_call_type[outBound].num_failed_handled!=0)
          outBound_avg_failed_handle_time =  event_data->stats_per_call_type[outBound].total_failed_handle_time /
                                              event_data->stats_per_call_type[outBound].num_failed_handled;
  InsertFullDataBG(soap,NULL,"f6_3_11_3_32","Avg. Failed",outBound_avg_failed_handle_time,ReturnArrayData); 

  InsertFullDataBG(soap,NULL,"f6_3_11_3_33","Failed",event_data->stats_per_call_type[outBound].longest_failed_handle_time,ReturnArrayData); 


    if (event_data->stats_per_call_type[outBound].num_interflowed !=0)
         outBound_avg_interflowed_time =  event_data->stats_per_call_type[outBound].total_interflowed_time /
                                             event_data->stats_per_call_type[outBound].num_interflowed ;
   InsertFullDataBG(soap,NULL,"f6_3_11_3_34","Avg. Interflowed",outBound_avg_interflowed_time,ReturnArrayData); 
 
   InsertFullDataBG(soap,NULL,"f6_3_11_3_35","Long.Interflowed", event_data->stats_per_call_type[outBound].longest_interflowed_time,ReturnArrayData); 

  


return rc;
}

CsaRc_t  IVRApplicationReply::MakeIvrAppInfoEMPTY    (CsaWebServer *soap,unsigned int IvrAppId, ns1__BlockItemType *ReturnArrayData )  //PVL 14-06-2010 1004180
{
CsaRc_t                                        rc = ARC_OK;
//NAME
  InsertFullDataBG(soap,NULL,"f6_3_11_1_1","IVR App. Name",soap->m_db_data_ptr->ivrapptab.GetName(IvrAppId),ReturnArrayData); 

  InsertFullDataBG(soap,NULL,"f6_3_11_1_2","Interval Time",0,ReturnArrayData); 
//inBound
  InsertFullDataBG(soap,NULL,"f6_3_11_2_1","Calls",0,ReturnArrayData); 

  InsertFullDataBG(soap,NULL,"f6_3_11_2_3","Too Long ACD",0,ReturnArrayData);

  InsertFullDataBG(soap,NULL,"f6_3_11_2_5","Avg.time",0,ReturnArrayData); 
  InsertFullDataBG(soap,NULL,"f6_3_11_2_4","Max time",0,ReturnArrayData); 
//-----------------

InsertFullDataBG(soap,NULL,"f6_3_11_3_18","RPH ACD",0,ReturnArrayData); 
//------------------
InsertFullDataBG(soap,NULL,"f6_3_11_3_1","Times Accessed ACD",0,ReturnArrayData); 
InsertFullDataBG(soap,NULL,"f6_3_11_3_4","Fully Handled ACD",0,ReturnArrayData); 
InsertFullDataBG(soap,NULL,"f6_3_11_3_5","Failed ACD",0,ReturnArrayData); 
InsertFullDataBG(soap,NULL,"f6_3_11_3_6","Interflowed ACD",0,ReturnArrayData); 
//-----------------
InsertFullDataBG(soap,NULL,"f6_3_11_3_10","All/access",0,ReturnArrayData); 
//-----------------
InsertFullDataBG(soap,NULL,"f6_3_11_3_11","Max Handled ACD",0,ReturnArrayData); 
 
InsertFullDataBG(soap,NULL,"f6_3_11_3_12","Fully handled",0,ReturnArrayData); 

InsertFullDataBG(soap,NULL,"f6_3_11_3_13","longest_fully_handle_time",0,ReturnArrayData); 
  
InsertFullDataBG(soap,NULL,"f6_3_11_3_14","Avg. Failed",0,ReturnArrayData); 
//------------------------
InsertFullDataBG(soap,NULL,"f6_3_11_3_15","Failed",0,ReturnArrayData); 
InsertFullDataBG(soap,NULL,"f6_3_11_3_16","Avg. Interflowed",0,ReturnArrayData); 

// Out Bound
 InsertFullDataBG(soap,NULL,"f6_3_11_2_2","Calls OACD",0,ReturnArrayData); 
 InsertFullDataBG(soap,NULL,"f6_3_11_2_6","Too Long OACD",0,ReturnArrayData); 
 InsertFullDataBG(soap,NULL,"f6_3_11_2_8","Avg.time OACD",0,ReturnArrayData); 
 InsertFullDataBG(soap,NULL,"f6_3_11_2_7","Max time OACD",0,ReturnArrayData); 
 InsertFullDataBG(soap,NULL,"f6_3_11_3_36","RPH OACD",0,ReturnArrayData); 
 InsertFullDataBG(soap,NULL,"f6_3_11_3_19","Times Accessed OACD",0,ReturnArrayData); 
 InsertFullDataBG(soap,NULL,"f6_3_11_3_21","Fully Handled OACD",0,ReturnArrayData); 
 InsertFullDataBG(soap,NULL,"f6_3_11_3_23","Failed OACD",0,ReturnArrayData); 
 InsertFullDataBG(soap,NULL,"f6_3_11_3_24","Interflowed OACD",0,ReturnArrayData); 

 InsertFullDataBG(soap,NULL,"f6_3_11_3_28","All/access",0,ReturnArrayData); 

 InsertFullDataBG(soap,NULL,"f6_3_11_3_29","Max Handled OACD",0,ReturnArrayData); 

 InsertFullDataBG(soap,NULL,"f6_3_11_3_30","Fully handled",0,ReturnArrayData); 

 InsertFullDataBG(soap,NULL,"f6_3_11_3_31","longest_fully_handle_time", 0,ReturnArrayData); 

  InsertFullDataBG(soap,NULL,"f6_3_11_3_32","Avg. Failed",0,ReturnArrayData); 

  InsertFullDataBG(soap,NULL,"f6_3_11_3_33","Failed",0,ReturnArrayData); 
   InsertFullDataBG(soap,NULL,"f6_3_11_3_34","Avg. Interflowed",0,ReturnArrayData); 
 
   InsertFullDataBG(soap,NULL,"f6_3_11_3_35","Long.Interflowed",0,ReturnArrayData); 

  


return rc;
}

CsaRc_t     IVRApplicationReply::StartMonitor          (unsigned int ivrapp_id)
{ //we can send multiple messages  to ARMAIN.cpp according the same ivrapp_id ,
  // becouse of internal validation in setNlaDumpOn() ,
	 CsaRc_t                                        rc = ARC_OK;
		
    EosEventHeader_t               event_header;
    EosRetCode_t                   erc;
    NLA_CSA_001                    reg_req;
    
	event_header.dest_info = -1; //PVL 7-02-2010 
    event_header.op_code = WEB_CSA_reg_for_ivr_app_req ;
    event_header.dest_que_id = CSA_Q4;
    event_header.reply_que_id = 0;//DUMMY , not in use  //RBE_Q2;
    event_header.data_len = sizeof(reg_req);
    reg_req.entity_id = ivrapp_id;
    reg_req.refresh_interval = 1;
    reg_req.tbl_index = 0;
    erc = EosCreateAndSendEvent (NLA_TH1, event_header.dest_que_id , event_header, &reg_req, event_header.data_len);
	return rc;
}
