#include "er_std_override.h"
#include "csa_web_group_reply.hpp"
#include "csa_web_commondefs.hpp"


static BswLog_c csa_web_group_reply_log_d(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_DETAILED);
static BswLog_c csa_web_group_reply_log_i(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_INFO);


Ulong_t  GroupRecordReply::myOldestTime(Ulong_t a,Ulong_t b) //PVL 20-7-2009 1001310
{
  if(a==0)
    return b;
  if(b==0)
    return a;
  return  (a<b?a:b);
}


//------------------------------------------------------------------------------------------------
void GroupRecordReply::InsertGroupList(CsaWebServer*  soap,
                                       unsigned int   grpId,
                                       std::string    grpName,
                                       std::string    grpNumber,
                                       unsigned int   grpEmailEnable,
                                       ns1__GroupListReturnType &ReturnArrayData)
{
  ns1__GroupListDataItemType *tmp;
  tmp= soap_new_ns1__GroupListDataItemType(soap, 1);
  tmp->grpId = grpId;
  tmp->grpName = grpName;
  tmp->grpNumber = grpNumber;
  tmp->grpEmailEnable = grpEmailEnable;//PVL 13-06-2010 1004082
  //soap->m_net_data_ptr->GroupListType_vector.push_back(tmp);

  csa_web_group_reply_log_d << "GroupRecordReply::InsertGroupList <" << tmp->grpId << "," << tmp->grpName.c_str() << ">" << LOG_TERMINATOR;

  ReturnArrayData.returnArray.push_back(tmp);
}


CsaRc_t GroupRecordReply::MakeEventDataWeb_QueInfo(CsaOnlineGroupStatistics_t* ed,GroupRecord* grec)
{
  CsaRc_t     rc = ARC_OK;
  time_t      dummy = 0;
  struct tm * stm = NULL;

  //PVL 2-06-2010 1004133 bool supportedOutbound = Csa_Setup.supportOutbound();
  //PVL 2-06-2010 1004133 bool supportedChat = Csa_Setup.supportChat();
  //PVL 2-06-2010 1004133 bool supportedEmail = Csa_Setup.supportEmail();
  //PVL 2-06-2010 1004133 bool supportedDialList = Csa_Setup.supportDialList();

  CsaTime_t temp_avg_in_q, temp_max_in_q;
  CsaTime_t temp_total_q;
  CsaNlaGrpQueStat_t abnd_stat,clbk_stat,diallist_stat; // diallist_stat added by Pavel, BUG 1000460
  BswLog_c& log = UtlStaticObjectsMngr_c::getLog(2);

  //------------------
  // fields from calls now in queue
  if (grec->Queued_calls[acd_type_acd_offset].inited())
  {
    ed->queue.queued_calls = grec->Queued_calls[acd_type_acd_offset].getNum();
    // rc is always OK!!! check the function!
    rc = grec->Queued_calls[acd_type_acd_offset].getStatistics(temp_avg_in_q,
                                                               temp_max_in_q,
                                                               ed->queue.sti,
                                                               ed->queue.waiting_above_target_asa); //f6_3_1_2_2

    // soap is because it is too hard to cast the parameters from getStatistics
    if (rc == ARC_OK)
    {
      ed->queue.avg_time_in_queue = temp_avg_in_q;
      ed->queue.longest_queue_time = temp_max_in_q; //f6_3_1_2_4
    }
  }


  //-------------STI
  //CHAT
  if(supportedChat) //PVL 2-06-2010 1004133
  {
    if (grec->Queued_calls[acd_type_chat_offset].inited()) 
    {
      ed->chat_queue.queued_calls = grec->Queued_calls[acd_type_chat_offset].getNum();
      // rc is always OK!!! check the function!
      rc = grec->Queued_calls[acd_type_chat_offset].getStatistics(temp_avg_in_q,temp_max_in_q,ed->chat_queue.sti,ed->chat_queue.waiting_above_target_asa); //f6_3_1_2_15
      // soap is because it is too hard to cast the parameters from getStatistics
      if (rc == ARC_OK) {
        ed->chat_queue.avg_time_in_queue = temp_avg_in_q;//f6_3_1_2_17
        ed->chat_queue.longest_queue_time = temp_max_in_q;//f6_3_1_2_16
      }
    }
  }//PVL 2-06-2010 1004133

  //---------------f6_3_1_2_19
  //EMAIL
  if(supportedEmail) //PVL 2-06-2010 1004133
  {
    if (grec->Queued_calls[acd_type_email_offset].inited())
    {
      ed->email_queue.queued_calls = grec->Queued_calls[acd_type_email_offset].getNum();
      // rc is always OK!!! check the function!
      //f6_3_1_2_20
      rc = grec->Queued_calls[acd_type_email_offset].getStatistics(temp_avg_in_q,temp_max_in_q,ed->email_queue.sti,ed->email_queue.waiting_above_target_asa);
      // soap is because it is too hard to cast the parameters from getStatistics
      if (rc == ARC_OK) {
        ed->email_queue.avg_time_in_queue = temp_avg_in_q; //f6_3_1_2_3 + AVRG f6_3_1_2_22
        ed->email_queue.longest_queue_time = temp_max_in_q; //f6_3_1_2_21
      }
    }
  }//PVL 2-06-2010 1004133

  //--------------f6_3_1_2_7
  //DIALIST
  if(supportedDialList) //PVL 2-06-2010 1004133
  {
    if (grec->Queued_calls[acd_type_dial_list_offset].inited())
    {
      diallist_stat.queued_calls = grec->Queued_calls[acd_type_dial_list_offset].getNum(); // changed += to = by Pavel, BUG 1000460
      rc = grec->Queued_calls[acd_type_dial_list_offset].getStatistics(temp_avg_in_q,temp_max_in_q,diallist_stat.sti,diallist_stat.waiting_above_target_asa);
      if (rc == ARC_OK) {
        diallist_stat.avg_time_in_queue = temp_avg_in_q;
        diallist_stat.longest_queue_time = temp_max_in_q;
      }
    }
  }//PVL 2-06-2010 1004133

  /*---------------------------------------------------------------------
  Outbound ACD Statistics (Callback + Abandoned)
  ---------------------------------------------------------------------*/
  if (supportedOutbound)
  {
    // saves us the try/catch thing
    if (grec->Queued_calls[acd_type_callback_offset].inited())
    {
      clbk_stat.queued_calls = grec->Queued_calls[acd_type_callback_offset].getNum();
      rc = grec->Queued_calls[acd_type_callback_offset].getStatistics(temp_avg_in_q,temp_max_in_q,clbk_stat.sti,clbk_stat.waiting_above_target_asa);
      // soap is because it is too hard to cast the parameters from getStatistics!!!
      if (rc == ARC_OK) {
        clbk_stat.avg_time_in_queue = temp_avg_in_q;
        clbk_stat.longest_queue_time = temp_max_in_q;
      }
    }

    // abnd, optional
    if (grec->Queued_calls[acd_type_abandoned_offset].inited()) 
    {
      abnd_stat.queued_calls = grec->Queued_calls[acd_type_abandoned_offset].getNum();
      rc = grec->Queued_calls[acd_type_abandoned_offset].getStatistics(temp_avg_in_q,temp_max_in_q,abnd_stat.sti,abnd_stat.waiting_above_target_asa);
      // soap is because it is too hard to cast the parameters from getStatistics!!!
      if (rc == ARC_OK) {
        abnd_stat.avg_time_in_queue = temp_avg_in_q;
        abnd_stat.longest_queue_time = temp_max_in_q;
      }
    }

    //--
    // unite the outbound call type
    ed->o_queue.queued_calls = abnd_stat.queued_calls + clbk_stat.queued_calls;
    ed->o_queue.longest_queue_time = mymax(abnd_stat.longest_queue_time,clbk_stat.longest_queue_time); //f6_3_1_2_10
    ed->o_queue.waiting_above_target_asa = abnd_stat.waiting_above_target_asa + clbk_stat.waiting_above_target_asa;
    for (int i = 0 ; i < STI_NUM_BINS ; i++)
      ed->o_queue.sti[i] = abnd_stat.sti[i] + clbk_stat.sti[i];
    temp_total_q = (clbk_stat.queued_calls * clbk_stat.avg_time_in_queue) + (abnd_stat.queued_calls * abnd_stat.avg_time_in_queue);
    if ( ed->o_queue.queued_calls != 0)
      ed->o_queue.avg_time_in_queue = (Ulong_t) (temp_total_q / ed->o_queue.queued_calls); //f6_3_1_2_9

    if (grec->Queued_calls[acd_type_dummy_offset].inited())
    {
      ed->pending_outb.queued_calls = grec->Queued_calls[acd_type_dummy_offset].getNum();
      rc = grec->Queued_calls[acd_type_dummy_offset].getStatistics(temp_avg_in_q,
                                                                   temp_max_in_q,
                                                                   ed->pending_outb.sti,
                                                                   ed->pending_outb.waiting_above_target_asa);
      // soap is because it is too hard to cast the parameters from getStatistics!!!
      if (rc == ARC_OK) {
        ed->pending_outb.avg_time_in_queue = temp_avg_in_q;  //f6_3_1_2_12
        ed->pending_outb.longest_queue_time = temp_max_in_q; //f6_3_1_2_13
      }
    }
  }

  if (grec->Queued_calls[acd_type_abandoned_offset].inited())
  {
    abnd_stat.queued_calls = grec->Queued_calls[acd_type_abandoned_offset].getNum();
    rc = grec->Queued_calls[acd_type_abandoned_offset].getStatistics(temp_avg_in_q,temp_max_in_q,abnd_stat.sti,abnd_stat.waiting_above_target_asa);
    // soap is because it is too hard to cast the parameters from getStatistics!!!
    if (rc == ARC_OK) {
      abnd_stat.avg_time_in_queue = temp_avg_in_q;
      abnd_stat.longest_queue_time = temp_max_in_q;
    }
  }

  // unite the outbound call type
  ed->o_queue.queued_calls = abnd_stat.queued_calls + clbk_stat.queued_calls;

  //-----------------f6_3_1_2_2--------------------
  //1. ABOVE
  //2. ABOVE
  //------------f6_3_1_2_9----------------------------------


  if (supportedDialList)
  {
    // saves us the try/catch thing

    // else log << func_name << " Dial List supported, but Outbound Dial List Connected_calls array ptr is set to NULL in group " << (char*)Grp_dn <<LOG_TERMINATOR;

    if (grec->Queued_calls[acd_type_dial_list_offset].inited())
    {
      diallist_stat.queued_calls = grec->Queued_calls[acd_type_dial_list_offset].getNum(); // changed += to = by Pavel, BUG 1000460
      rc = grec->Queued_calls[acd_type_dial_list_offset].getStatistics(temp_avg_in_q,temp_max_in_q,diallist_stat.sti,diallist_stat.waiting_above_target_asa);
      if (rc == ARC_OK) {
        diallist_stat.avg_time_in_queue = temp_avg_in_q;
        diallist_stat.longest_queue_time = temp_max_in_q;
      }
    }

    ed->o_queue.longest_queue_time = mymax(ed->o_queue.longest_queue_time,diallist_stat.longest_queue_time);
    ed->o_queue.waiting_above_target_asa += diallist_stat.waiting_above_target_asa;
    Uhuge_t temp_total_q, a = ed->o_queue.queued_calls,
                          b = ed->o_queue.avg_time_in_queue,
                          c = diallist_stat.queued_calls,
                          d = diallist_stat.avg_time_in_queue;

    for (int i = 0 ; i < STI_NUM_BINS ; i++)
      ed->o_queue.sti[i] += diallist_stat.sti[i];
    temp_total_q = (a * b) + (c * d);
    // do soap after we reconstruct total queue time
    ed->o_queue.queued_calls += diallist_stat.queued_calls;
    if ( ed->o_queue.queued_calls != 0)
      ed->o_queue.avg_time_in_queue = (Ulong_t) (temp_total_q / ed->o_queue.queued_calls);
  }

  if (supportedEmail)
  {
    if (grec->Queued_calls[acd_type_email_offset].inited())
    {
      ed->email_queue.queued_calls = grec->Queued_calls[acd_type_email_offset].getNum();
      // rc is always OK!!! check the function!
      rc = grec->Queued_calls[acd_type_email_offset].getStatistics(temp_avg_in_q,temp_max_in_q,ed->email_queue.sti,ed->email_queue.waiting_above_target_asa);
      // soap is because it is too hard to cast the parameters from getStatistics
      if (rc == ARC_OK) {
        ed->email_queue.avg_time_in_queue = temp_avg_in_q;
        ed->email_queue.longest_queue_time = temp_max_in_q;
      }
    }
  }
  ///
  return rc;
}


CsaRc_t GroupRecordReply::MakePerGroup_QueInfo  (CsaWebServer *soap,GroupRecord*  grec, ns1__BlockItemType *ReturnArrayData )
{
  CsaRc_t                     rc = ARC_OK;
  CsaOnlineGroupStatistics_t  GroupStatistics;
  CsaOnlineGroupStatistics_t* ed;
  csaTimeNow = time( NULL);

  memset(&GroupStatistics, 0, sizeof(CsaOnlineGroupStatistics_t));

  rc =  MakeEventDataWeb_QueInfo(&GroupStatistics,grec);
  int i=0;

  ed = &GroupStatistics;
  //header

  //array
  unsigned long avg_chat_voice;
  unsigned long  longest_queue_time;
  unsigned long  chat_longest_queue_time;

  unsigned int  *TmpColorPtr = NULL;

  InsertFullData(soap,"f6_3_1_1_2","Group No",soap->m_db_data_ptr->groupstab.GetGroupNumberById(grec->Grp_id),ReturnArrayData);

  //----------------
  //ERS : 6.3.1.2.1 - queued_calls
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_2_1,
    ed->queue.queued_calls,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_2_1","Q.Calls", ed->queue.queued_calls,ReturnArrayData);
  //----------------
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_2_14,
    ed->chat_queue.queued_calls,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_2_14","Chat.Q.Calls",ed->chat_queue.queued_calls,ReturnArrayData);
  //----------------
  //20-Mar-2019 YR BZ#49198
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_2_1, //f20_3_1_2_1
    ed->queue.queued_calls + ed->chat_queue.queued_calls,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f20_3_1_2_1","Q+.Calls", ed->queue.queued_calls + ed->chat_queue.queued_calls,ReturnArrayData);
  //----------------
  InsertFullData(soap,"f6_3_1_2_19","Emails In Q", ed->email_queue.queued_calls,ReturnArrayData);
  InsertFullData(soap,"f6_3_1_2_7","O.Q.Calls", ed->o_queue.queued_calls,ReturnArrayData);
  InsertFullData(soap,"f6_3_1_2_11","Pend.O.Q.Calls", ed->pending_outb.queued_calls,ReturnArrayData);
  //-----------
  //ERS: 6.3.1.2.2 - waiting_above_target_asa
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_2_2,
    ed->queue.waiting_above_target_asa,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_2_2","waiting_above_target_asa", ed->queue.waiting_above_target_asa,ReturnArrayData);
  //----------
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_2_15,
    ed->chat_queue.waiting_above_target_asa,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_2_15","chat_queue.waiting_above_target_asa", ed->chat_queue.waiting_above_target_asa,ReturnArrayData);
  //-------------
  //20-Mar-2019 YR BZ#49198
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_2_2, //20_3_1_2_2
    ed->queue.waiting_above_target_asa + ed->chat_queue.waiting_above_target_asa,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f20_3_1_2_2","Above T.ASA", ed->queue.waiting_above_target_asa + ed->chat_queue.waiting_above_target_asa,ReturnArrayData);
  //---------
  InsertFullData(soap,"f6_3_1_2_20","email_queue.waiting_above_target_asa", ed->email_queue.waiting_above_target_asa,ReturnArrayData);
  //-----------
  //ERS: 6.3.1.2.3 - avg_wait_in_que
  if (ed->queue.avg_time_in_queue > 0 ){
    soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_2_3,
      (Ulong_t)NlaCalcCsaElapsedTime(ed->queue.avg_time_in_queue),&TmpColorPtr);
    InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_2_3","Avg.In.Q", (Ulong_t)NlaCalcCsaElapsedTime(ed->queue.avg_time_in_queue),ReturnArrayData);
  }
  else
    InsertFullData(soap,"f6_3_1_2_3","Avg.In.Q", 0,ReturnArrayData);
  //--------------
  if( ed->chat_queue.avg_time_in_queue> 0 ){
    soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_2_17,
      (Ulong_t)NlaCalcCsaElapsedTime(ed->chat_queue.avg_time_in_queue),&TmpColorPtr);
    InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_2_17","chat_queue.avg_time_in_queue", (Ulong_t)NlaCalcCsaElapsedTime(ed->chat_queue.avg_time_in_queue),ReturnArrayData);
  }
  else
    InsertFullData(soap,"f6_3_1_2_17","chat_queue.avg_time_in_que", 0,ReturnArrayData);
  //--------------------
  if (ed->queue.queued_calls + ed->chat_queue.queued_calls > 0)
    avg_chat_voice =(unsigned int)(  ((NlaCalcCsaElapsedTime(ed->queue.avg_time_in_queue) * ed->queue.queued_calls) / (ed->queue.queued_calls+ed->chat_queue.queued_calls))
    + ((NlaCalcCsaElapsedTime(ed->chat_queue.avg_time_in_queue ) * ed->chat_queue.queued_calls) / (ed->queue.queued_calls+ed->chat_queue.queued_calls)));
  else
    avg_chat_voice = 0;
  //20-Mar-2019 YR BZ#49198
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_2_3,//f20_3_1_2_3
    avg_chat_voice,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f20_3_1_2_3","Avg.time", avg_chat_voice,ReturnArrayData);
  //-----------------
  if (ed->email_queue.avg_time_in_queue > 0)
    InsertFullData(soap,"f6_3_1_2_22","Email Avg.inQ.", (Ulong_t)NlaCalcCsaElapsedTime(ed->email_queue.avg_time_in_queue),ReturnArrayData);
  else
    InsertFullData(soap,"f6_3_1_2_22","Email Avg.inQ.", 0,ReturnArrayData);
  //--------------
  if (ed->o_queue.avg_time_in_queue > 0 )
    InsertFullData(soap,"f6_3_1_2_9","Avg.time in OutQ.", (Ulong_t)NlaCalcCsaElapsedTime(ed->o_queue.avg_time_in_queue),ReturnArrayData);
  else
    InsertFullData(soap,"f6_3_1_2_9","Avg.time in OutQ.", 0,ReturnArrayData);
  //----------------
  if (ed->pending_outb.avg_time_in_queue > 0)
    InsertFullData(soap,"f6_3_1_2_12","Avg.time In.Q", (Ulong_t)NlaCalcCsaElapsedTime(ed->pending_outb.avg_time_in_queue),ReturnArrayData);
  else
    InsertFullData(soap,"f6_3_1_2_12","Avg.time In.Q", 0,ReturnArrayData);
  //----------------
  if (ed->queue.longest_queue_time > 0)  {
    longest_queue_time = ed->queue.longest_queue_time;
    soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_2_4,
      (Ulong_t)NlaCalcCsaElapsedTime(longest_queue_time),&TmpColorPtr);
    InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_2_4","Max In.Q.", (Ulong_t)NlaCalcCsaElapsedTime(longest_queue_time),ReturnArrayData);
  }
  else
  {
    longest_queue_time =0;
    InsertFullData(soap,"f6_3_1_2_4","Max In.Q.", longest_queue_time,ReturnArrayData);
  }
  //--------------
  if (ed->chat_queue.longest_queue_time > 0 ){
    chat_longest_queue_time = ed->chat_queue.longest_queue_time ;
    InsertFullData(soap,"f6_3_1_2_16","CHAT Q. Max time", (Ulong_t)NlaCalcCsaElapsedTime(chat_longest_queue_time),ReturnArrayData);
  }
  else {
    chat_longest_queue_time = 0;
    InsertFullData(soap,"f6_3_1_2_16","CHAT Q. Max time", chat_longest_queue_time,ReturnArrayData);
  }
  //---------------
  InsertFullData(soap,"f20_3_1_2_4","Q.Max time",(Ulong_t)NlaCalcCsaElapsedTime( myOldestTime(longest_queue_time,chat_longest_queue_time)),ReturnArrayData); //PVL 6-06-2010 1002584//GetBiggerCsaTimeValue
  //---------------
  if (ed->email_queue.longest_queue_time >0)
    InsertFullData(soap,"f6_3_1_2_21","EMAIL Q Max time", (Ulong_t)NlaCalcCsaElapsedTime(ed->email_queue.longest_queue_time),ReturnArrayData);
  else
    InsertFullData(soap,"f6_3_1_2_21","EMAIL Q. Max time", 0,ReturnArrayData);
  //-------------
  if (ed->o_queue.longest_queue_time > 0)
    InsertFullData(soap,"f6_3_1_2_10","O.Q. Max time", (Ulong_t)NlaCalcCsaElapsedTime(ed->o_queue.longest_queue_time),ReturnArrayData);
  else
    InsertFullData(soap,"f6_3_1_2_10","O.Q.Max time", 0,ReturnArrayData);
  //-----------------
  if (ed->pending_outb.longest_queue_time >0)
    InsertFullData(soap,"f6_3_1_2_13","P.OutQ Max time", (Ulong_t)NlaCalcCsaElapsedTime(ed->pending_outb.longest_queue_time),ReturnArrayData);
  else
    InsertFullData(soap,"f6_3_1_2_13","P.OutQ Max time", 0,ReturnArrayData);
  //-------------------
  //ERS : 6.3.1.2.5 - acd calls according to sti}
  InsertFullData(soap,"f6_3_1_2_5a","ST1", ed->queue.sti[0],ReturnArrayData);
  InsertFullData(soap,"f6_3_1_2_18a","CHAT.ST1", ed->chat_queue.sti[0],ReturnArrayData);
  InsertFullData(soap,"f20_3_1_2_5a","ST1+", ed->queue.sti[0] + ed->chat_queue.sti[0],ReturnArrayData);

  InsertFullData(soap,"f6_3_1_2_5b","ST2",ed->queue.sti[1],ReturnArrayData);
  InsertFullData(soap,"f6_3_1_2_18b","CHAT.ST2", ed->chat_queue.sti[1],ReturnArrayData);
  InsertFullData(soap,"f20_3_1_2_5b","ST2+", ed->queue.sti[1]+ed->chat_queue.sti[1],ReturnArrayData);

  InsertFullData(soap,"f6_3_1_2_5c","ST3",ed->queue.sti[2],ReturnArrayData);
  InsertFullData(soap,"f6_3_1_2_18c","CHAT.ST3", ed->chat_queue.sti[2],ReturnArrayData);
  InsertFullData(soap,"f20_3_1_2_5c","ST3+", ed->queue.sti[2]+ed->chat_queue.sti[2],ReturnArrayData);

  InsertFullData(soap,"f6_3_1_2_5d","ST4", ed->queue.sti[3],ReturnArrayData);
  InsertFullData(soap,"f6_3_1_2_18d","CHAT.ST4",ed->chat_queue.sti[3],ReturnArrayData);
  InsertFullData(soap,"f20_3_1_2_5d","ST4+", ed->queue.sti[3]+ed->chat_queue.sti[3],ReturnArrayData);

  InsertFullData(soap,"f6_3_1_2_5e","ST5", ed->queue.sti[4],ReturnArrayData);
  InsertFullData(soap,"f6_3_1_2_18e","CHAT.ST5", ed->chat_queue.sti[4],ReturnArrayData);
  InsertFullData(soap,"f20_3_1_2_5e","ST5+", ed->queue.sti[4]+ed->chat_queue.sti[4],ReturnArrayData);

  InsertFullData(soap,"f6_3_1_2_5f","ST6",ed->queue.sti[5],ReturnArrayData);
  InsertFullData(soap,"f6_3_1_2_18f","CHAT.ST6", ed->chat_queue.sti[5],ReturnArrayData);
  InsertFullData(soap,"f20_3_1_2_5f","ST6+", ed->queue.sti[5]+ed->chat_queue.sti[5],ReturnArrayData);

  InsertFullData(soap,"f6_3_1_2_8a","OUT.ST1", ed->o_queue.sti[0],ReturnArrayData);
  InsertFullData(soap,"f6_3_1_2_8b","OUT.ST2", ed->o_queue.sti[1],ReturnArrayData);
  InsertFullData(soap,"f6_3_1_2_8c","OUT.ST3", ed->o_queue.sti[2],ReturnArrayData);
  InsertFullData(soap,"f6_3_1_2_8d","OUT.ST4", ed->o_queue.sti[3],ReturnArrayData);
  InsertFullData(soap,"f6_3_1_2_8e","OUT.ST5", ed->o_queue.sti[4],ReturnArrayData);
  InsertFullData(soap,"f6_3_1_2_8f","OUT.ST6", ed->o_queue.sti[5],ReturnArrayData);

  InsertFullData(soap,"f6_3_1_2_23a","EMAIL.ST1", ed->email_queue.sti[0],ReturnArrayData);
  InsertFullData(soap,"f6_3_1_2_23b","EMAIL.ST2", ed->email_queue.sti[1],ReturnArrayData);
  InsertFullData(soap,"f6_3_1_2_23c","EMAIL.ST3", ed->email_queue.sti[2],ReturnArrayData);
  InsertFullData(soap,"f6_3_1_2_23d","EMAIL.ST4", ed->email_queue.sti[3],ReturnArrayData);
  InsertFullData(soap,"f6_3_1_2_23e","EMAIL.ST5", ed->email_queue.sti[4],ReturnArrayData);
  InsertFullData(soap,"f6_3_1_2_23f","EMAIL.ST6", ed->email_queue.sti[5],ReturnArrayData);

  return rc;
}


CsaRc_t GroupRecordReply::MakePerGroup_ShiftInfo  (CsaWebServer *soap,GroupRecord*  grec, ns1__BlockItemType *ReturnArrayData )
{
  CsaRc_t       rc = ARC_OK;
  unsigned int* TmpColorPtr = NULL;

  //PVL 2-06-2010 1004133 bool supportedOutbound = Csa_Setup.supportOutbound();
  //PVL 2-06-2010 1004133 bool supportedChat =     Csa_Setup.supportChat();
  //PVL 2-06-2010 1004133 bool supportedEmail =    Csa_Setup.supportEmail();
  //PVL 2-06-2010 1004133 bool supportedDialList = Csa_Setup.supportDialList();

  unsigned int answered_calls = 0;
  unsigned int accepted_calls = 0;
  unsigned int chat_answered_calls = 0;
  unsigned int mail_answered_calls = 0;
  unsigned int group_calls_answered = 0;
  unsigned int chat_direct_answered = 0;
  unsigned int answered_in_another_group_calls = 0;
  unsigned int chat_answered_in_another_group_calls = 0;
  unsigned int mail_answered_in_another_group_calls = 0;
  unsigned int group_calls_answered_in_another_group = 0;
  unsigned int chat_direct_answered_in_another_group = 0;
  unsigned int queued_current_interval = 0;
  unsigned int avg_wait_que_on_cur_int = 0;
  unsigned int chat_queued_current_interval = 0;
  unsigned int chat_avg_wait_que_on_cur_int = 0;
  unsigned int avg_chat_voice_q = 0;
  unsigned int mail_queued_current_interval = 0;
  unsigned int mail_avg_wait_que_on_cur_int = 0;
  unsigned int longest_que_time_on_cur_int = 0;
  unsigned int chat_longest_que_time_on_cur_int = 0;
  unsigned int mail_longest_que_time_on_cur_int = 0;
  unsigned int mail_avg_interaction_time = 0;
  unsigned int avg_acd_talk_time = 0;
  unsigned int chat_avg_interaction_time = 0;
  unsigned int avg_chat_voice = 0;
  unsigned int avg_wrapup_time = 0;
  unsigned int avg_wrap_f_wrap_time = 0;
  unsigned int avg_handle_time =0;
  unsigned int longest_acd_talk_time = 0;
  unsigned int avg_answer_time = 0;
  unsigned int transferred_in = 0;
  unsigned int transferred_out = 0;
  unsigned int delayed_calls = 0;
  unsigned int tsf_denominator = 0;
  unsigned int new_tsf = 0;
  time_t beginning_time;
  time_t interval_time;
  unsigned int RPH;

  accepted_calls = grec->Acd[acd_type_acd_offset].calls_requesting_callback.get() +
                   grec->Acd[acd_type_acd_offset].group_calls_answered_per_time_slot.get() +
                   grec->Acd[acd_type_acd_offset].group_calls_abandoned_per_time_slot.get() +
                   grec->Acd[acd_type_acd_offset].group_calls_answered_in_another_group.get() +
                   grec->Acd[acd_type_acd_offset].group_calls_interflowed_out.get() +
                   grec->Acd[acd_type_acd_offset].overflowed_in_calls_answered_per_time_slot.get() +
                   grec->Acd[acd_type_acd_offset].overflowed_in_calls_abandoned_per_time_slot.get() +
                   grec->Acd[acd_type_acd_offset].overflowed_in_calls_answered_in_another_group.get()+
                   grec->Acd[acd_type_acd_offset].overflowed_in_calls_interflowed_out.get();

  // InsertFullDataBG(soap,NULL,"ERSID","NAME",VALUE,ReturnArrayData);
  //  tsf_denominator := 0;

  //  {ERS : 6.3.1.4.3 & 6.3.1.4.3a - # answered_calls + % of accepted_call}
  answered_calls = grec->Acd[acd_type_acd_offset].group_calls_answered_per_time_slot.get() +
                   grec->Acd[acd_type_acd_offset].overflowed_in_calls_answered_per_time_slot.get();
  InsertFullDataBG(soap,NULL,"f6_3_1_4_3","answered_calls",answered_calls,ReturnArrayData);

  if (accepted_calls > 0 )
  {
    soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_3a,
                                                  (unsigned int)((answered_calls * 100 ) /accepted_calls),&TmpColorPtr);
    InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_3a","f6_3_1_4_3a",(unsigned int)((answered_calls * 100 ) /accepted_calls),ReturnArrayData);
  }
  else
    InsertFullDataBG(soap,NULL,"f6_3_1_4_3a","f6_3_1_4_3a",0,ReturnArrayData);


  // {ERS : 6.3.1.4.37 - group_calls_answered}
  group_calls_answered = grec->Acd[acd_type_acd_offset].group_calls_answered_per_time_slot.get() ;
  InsertFullDataBG(soap,NULL,"f6_3_1_4_37","group_calls_answered",group_calls_answered,ReturnArrayData);


  //{ERS : 6.3.1.4.38 - answered_in_another_group_calls}
  answered_in_another_group_calls = grec->Acd[acd_type_acd_offset].answered_in_another_group_calls.get();
  InsertFullDataBG(soap,NULL,"f6_3_1_4_38","answered_in_another_group_calls",answered_in_another_group_calls,ReturnArrayData);


  //{ERS : 6.3.1.4.42 - group_calls_answered_in_another_group}

  group_calls_answered_in_another_group = grec->Acd[acd_type_acd_offset].group_calls_answered_in_another_group.get();
  InsertFullDataBG(soap,NULL,"f6_3_1_4_42","group_calls_answered_in_another_group",group_calls_answered_in_another_group,ReturnArrayData);


  //{ERS : 6.3.1.4.1 - tsf for current interval}
  //{3 variations for tsf calculate}
  //31-Jan-2021 YR BZ#52984
  int  tsf_form_var = Csa_Setup.getTsfFormula();
  //int  tsf_form_var = TSF_FORM_VAR1; //TSF_FORM_VAR3;

  switch( tsf_form_var)
  {
    case TSF_FORM_VAR1:
      tsf_denominator = answered_calls;
    break;
    case TSF_FORM_VAR2:
      tsf_denominator = answered_calls + grec->Acd[acd_type_acd_offset].abandoned_after_target_asa.get();
    break;
    case TSF_FORM_VAR3:
      tsf_denominator = accepted_calls;
    break;
    default:
      tsf_denominator = accepted_calls;
    break;
  }

  if (tsf_denominator > 0 )
  {
    new_tsf = (unsigned int)((grec->Acd[acd_type_acd_offset].calls_answered_before_target_asa.get() * 100) /
              tsf_denominator);
    if (new_tsf > 100)
      new_tsf = 100;
  }
  else
    new_tsf = 100;

   soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_1,
                                                 new_tsf,&TmpColorPtr);
   InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_1","TSF",new_tsf,ReturnArrayData);

   beginning_time = grec->Current_shift_start_time;
   //data_list.InsertString (f6_3_1_1_3, '00:00');

   interval_time = grec->Window_length ;
   InsertFullDataBG(soap,NULL,"f6_3_1_1_4","Interval Time",interval_time,ReturnArrayData);

   // {ERS : 6.3.1.4.11 - rph}
   // {Debug('RPH: answered_calls: ' + IntToStr (acd.answered_calls)
   //  + '   begining_time:  ' + IntToStr(NlaCalcCsaElapsedTime(begining_time)));}
   //{  new_rph := NlaCalcCsaElapsedTime(begining_time);}
   if ( (NlaCalcCsaElapsedTime(beginning_time) > 0) &
        (NlaCalcCsaElapsedTime(beginning_time) < interval_time))
   {
     RPH = (unsigned int)((answered_calls * 3600) / NlaCalcCsaElapsedTime (beginning_time));
   }
   else 
   {
     if ((NlaCalcCsaElapsedTime(beginning_time) > 0) &
         (interval_time > 0) )
       RPH = (unsigned int)(((answered_calls) * 60) / interval_time );
     else
       RPH = 0;
   }
   //ersIdList[36].lim_type := Down;
   soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_11,
                                                 RPH,&TmpColorPtr);
   InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_11","RPH",RPH,ReturnArrayData);
   //{ERS : 6.3.1.4.12 - avg_wait_que_on_cur_int}

   queued_current_interval = grec->Acd[acd_type_acd_offset].queued_calls.get();
   if (grec->Acd[acd_type_acd_offset].queued_calls.get() > 0 )
     avg_wait_que_on_cur_int = (unsigned int)(grec->Acd[acd_type_acd_offset].total_queue_time.get()/
                               grec->Acd[acd_type_acd_offset].queued_calls.get());
   else
     avg_wait_que_on_cur_int = 0;
   soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_12,
                                                 avg_wait_que_on_cur_int,&TmpColorPtr);
   InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_12","avg_wait_que_on_cur_int",avg_wait_que_on_cur_int,ReturnArrayData);



   //{ERS : 6.3.1.4.13 - longest_que_time_on_cur_int}

   longest_que_time_on_cur_int = grec->Acd[acd_type_acd_offset].longest_queue_time.get();
   soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_13,
                                                 longest_que_time_on_cur_int,&TmpColorPtr);
   InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_13","longest_que_time_on_cur_int",longest_que_time_on_cur_int,ReturnArrayData);


   //{ERS : 6.3.1.4.14 - avg_acd_talk_time }
   if( answered_calls > 0 )
     avg_acd_talk_time = (unsigned int)( grec->Acd[acd_type_acd_offset].total_talk_time.get() /answered_calls);
   else
     avg_acd_talk_time = 0;
   soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_14,
                                                 avg_acd_talk_time,&TmpColorPtr);
   InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_14","avg_acd_talk_time",avg_acd_talk_time,ReturnArrayData);


   // {ERS : 6.3.1.4.18 - avg_wrapup_time }
   if (answered_calls > 0 )
   {
     avg_wrapup_time = (unsigned int)( grec->Acd[acd_type_acd_offset].total_wrap_up_time.get() /answered_calls);
     avg_wrap_f_wrap_time = (unsigned int)( (grec->Acd[acd_type_acd_offset].total_wrap_up_time.get() +
                                             grec->Acd[acd_type_acd_offset].total_forced_wrap_up_time.get())/
                                             answered_calls);
   }
   else
   {
     avg_wrapup_time = 0;
     avg_wrap_f_wrap_time = 0;
   }
   InsertFullDataBG(soap,NULL,"f6_3_1_4_18","avg_wrapup_time",avg_wrapup_time,ReturnArrayData);
   //{executone: 6.3.1.4.35}
   InsertFullDataBG(soap,NULL,"f6_3_1_4_35","avg_wrap_f_wrap_time",avg_wrap_f_wrap_time,ReturnArrayData);

   // {ERS : 6.3.1.4.19 - avg_handle_time }
   if (answered_calls > 0 )
     avg_handle_time = (unsigned int)( (grec->Acd[acd_type_acd_offset].total_talk_time.get() +
                                        grec->Acd[acd_type_acd_offset].total_wrap_up_time.get()) / answered_calls);
   else
     avg_handle_time = 0;
   InsertFullDataBG(soap,NULL,"f6_3_1_4_19","avg_handle_time",avg_handle_time,ReturnArrayData);


   //   {ERS : 6.3.1.4.15  - longest_acd_talk_time}
   longest_acd_talk_time = grec->Acd[acd_type_acd_offset].longest_talk_time.get();
   InsertFullDataBG(soap,NULL,"f6_3_1_4_15","longest_acd_talk_time",longest_acd_talk_time,ReturnArrayData);

   // {ERS : 6.1.3.4.16 - avg_answer_time}
   if (answered_calls > 0)
     avg_answer_time = (unsigned int)(grec->Acd[acd_type_acd_offset].total_wait_till_answer_time.get()/ answered_calls);
   else
     avg_answer_time = 0;
   soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_16,
                                                 avg_answer_time,&TmpColorPtr);
   InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_16","ASA",avg_answer_time,ReturnArrayData);

   //  {ERS : 6.3.1.4.27 }
   transferred_in = grec->Acd[acd_type_acd_offset].transferred_in_calls.get();
   InsertFullDataBG(soap,NULL,"f6_3_1_4_27","transferred_in",transferred_in,ReturnArrayData);

   //   {ERS : 6.3.1.4.28 }
   transferred_out = grec->Acd[acd_type_acd_offset].transferred_out_calls.get() ;
   soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_28,
                                                 transferred_out,&TmpColorPtr);
   InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_28","transferred_out",transferred_out,ReturnArrayData);

   //   {ERS : 6.3.1.4.29 }
   delayed_calls = grec->Acd[acd_type_acd_offset].delayed_calls.get();
   soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_29,
                                                 delayed_calls,&TmpColorPtr);
   InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_29","delayed_calls",delayed_calls,ReturnArrayData);
   /*---------------------------------------------------------------------
							Chat ACD Statistics
    ---------------------------------------------------------------------*/
   if (supportedChat)
   {
     //f6_3_1_4_63  CHAT_

     chat_answered_calls = grec->Acd[acd_type_chat_offset].overflowed_in_calls_answered_per_time_slot.get() +
                           grec->Acd[acd_type_chat_offset].group_calls_answered_per_time_slot.get();
     InsertFullDataBG(soap,NULL,"f6_3_1_4_63","Chat Answered by Group",chat_answered_calls,ReturnArrayData);
     //f6_3_1_4_64 CHAT_
     chat_direct_answered = grec->Acd[acd_type_chat_offset].group_calls_answered_per_time_slot.get();
     InsertFullDataBG(soap,NULL,"f6_3_1_4_64","chat_direct_answered",chat_direct_answered,ReturnArrayData);

     // f6_3_1_4_66 CHAT_
     chat_answered_in_another_group_calls = grec->Acd[acd_type_chat_offset].group_calls_answered_in_another_group.get() +
                                            grec->Acd[acd_type_chat_offset].overflowed_in_calls_answered_in_another_group.get();
     InsertFullDataBG(soap,NULL,"f6_3_1_4_66","chat_answered_in_another_group_calls",chat_answered_in_another_group_calls,ReturnArrayData);

     //f6_3_1_4_67 CHAT_
     chat_direct_answered_in_another_group =   grec->Acd[acd_type_chat_offset].group_calls_answered_in_another_group.get();
     soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_67,
                                                   chat_direct_answered_in_another_group,&TmpColorPtr);
     InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_67","chat_direct_answered_in_another_group",chat_direct_answered_in_another_group,ReturnArrayData);

     //f6_3_1_4_85 CHAT_
     chat_queued_current_interval = grec->Acd[acd_type_chat_offset].queued_calls.get();
     if( chat_queued_current_interval > 0)
       chat_avg_wait_que_on_cur_int = (unsigned int)((grec->Acd[acd_type_chat_offset].total_wait_time_of_interflowed_calls.get() +
                                                      grec->Acd[acd_type_chat_offset].total_wait_time_of_abandoned_calls.get() +
                                                      grec->Acd[acd_type_chat_offset].total_queue_time.get() )/
                                                      chat_queued_current_interval);
     else
       chat_avg_wait_que_on_cur_int = 0;
     soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_85,
                                                   chat_avg_wait_que_on_cur_int,&TmpColorPtr);
     InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_85","chat_avg_wait_que_on_cur_int",chat_avg_wait_que_on_cur_int,ReturnArrayData);
     //----
     //CHAT_
     chat_longest_que_time_on_cur_int = grec->Acd[acd_type_chat_offset].longest_queue_time.get();
     InsertFullDataBG(soap,NULL,"f6_3_1_4_86","chat_longest_que_time_on_cur_int",chat_longest_que_time_on_cur_int,ReturnArrayData);
     //f6_3_1_4_88 CHAT_
     if (chat_answered_calls > 0 )
       chat_avg_interaction_time =(unsigned int)(  grec->Acd[acd_type_chat_offset].total_talk_time.get() /chat_answered_calls);
     else
       chat_avg_interaction_time = 0;
     soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_88,
                                                   chat_avg_interaction_time,&TmpColorPtr);
     InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_88","chat_avg_interaction_time",chat_avg_interaction_time,ReturnArrayData);
   }
   //f20_3_1_4_22 CHAT_
   if (chat_queued_current_interval+queued_current_interval > 0)
     avg_chat_voice_q = (unsigned int)( ((avg_wait_que_on_cur_int * queued_current_interval) +
     (chat_avg_wait_que_on_cur_int * chat_queued_current_interval)) /
     (queued_current_interval+chat_queued_current_interval));
   else
     avg_chat_voice_q = 0;
   //----
   //20-Mar-2019 YR BZ#49198
   soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_12, //20_3_1_4_22
                                                 avg_chat_voice_q,&TmpColorPtr);
   InsertFullDataBG(soap,TmpColorPtr,"f20_3_1_4_22","avg_chat_voice_q",avg_chat_voice_q,ReturnArrayData);

   /*---------------------------------------------------------------------
						E-MAil ACD Statistics
    ---------------------------------------------------------------------*/
   if (supportedEmail)
   {
     // f6_3_1_4_93 EMAIL_
     mail_answered_calls = grec->Acd[acd_type_email_offset].overflowed_in_calls_answered_per_time_slot.get() +
                           grec->Acd[acd_type_email_offset].group_calls_answered_per_time_slot.get();
    InsertFullDataBG(soap,NULL,"f6_3_1_4_93","Emails Answered",mail_answered_calls,ReturnArrayData);

    //f6_3_1_4_96 EMAIL_
    mail_answered_in_another_group_calls = grec->Acd[acd_type_email_offset].group_calls_answered_in_another_group.get() +
                                           grec->Acd[acd_type_email_offset].overflowed_in_calls_answered_in_another_group.get();
    InsertFullDataBG(soap,NULL,"f6_3_1_4_96","Emails Answered by Another Group",mail_answered_in_another_group_calls,ReturnArrayData);

    //EMAIL_
    mail_queued_current_interval = grec->Acd[acd_type_email_offset].queued_calls.get();
    if (mail_queued_current_interval > 0 )
      mail_avg_wait_que_on_cur_int = (unsigned int)(( grec->Acd[acd_type_email_offset].total_wait_time_of_interflowed_calls.get() +
                                                   grec->Acd[acd_type_email_offset].total_queue_time.get() ) /
                                                    mail_queued_current_interval);
    else
      mail_avg_wait_que_on_cur_int = 0;
    InsertFullDataBG(soap,NULL,"f6_3_1_4_109","In Queue",mail_avg_wait_que_on_cur_int,ReturnArrayData);


    //f6_3_1_4_110 EMAIL_
    mail_longest_que_time_on_cur_int = grec->Acd[acd_type_email_offset].longest_queue_time.get();
    InsertFullDataBG(soap,NULL,"f6_3_1_4_110","In Queue",mail_longest_que_time_on_cur_int,ReturnArrayData);

    //f6_3_1_4_112 EMAIL_
    if (mail_answered_calls > 0)
      mail_avg_interaction_time = (unsigned int)( grec->Acd[acd_type_email_offset].total_talk_time.get() /
                                   mail_answered_calls);
    else
      mail_avg_interaction_time = 0;
    soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_112,
                                                  mail_avg_interaction_time,&TmpColorPtr);
    InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_112","Talk",mail_avg_interaction_time,ReturnArrayData);
  }

  //AFTER
  if(chat_longest_que_time_on_cur_int > longest_que_time_on_cur_int)
    InsertFullDataBG(soap,NULL,"f20_3_1_4_23","In Queue",chat_longest_que_time_on_cur_int,ReturnArrayData);
  else
    InsertFullDataBG(soap,NULL,"f20_3_1_4_23","In Queue",longest_que_time_on_cur_int,ReturnArrayData);
  // AFTER
  if (answered_calls+chat_answered_calls > 0)
    avg_chat_voice = (unsigned int)( ((avg_acd_talk_time * answered_calls) +
	                  (chat_avg_interaction_time * chat_answered_calls)) /
                       (answered_calls+chat_answered_calls));
  else
    avg_chat_voice = 0;

  //20-Mar-2019 YR BZ#49198
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_14, //20_3_1_4_20
                                                avg_chat_voice,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f20_3_1_4_20","Talk",avg_chat_voice,ReturnArrayData);
  //AFTER
  //f20_3_1_4_4  CHAT_
  InsertFullDataBG(soap,NULL,"f20_3_1_4_4","Answered",chat_answered_calls+answered_calls,ReturnArrayData);
  //f20_3_1_4_5
  InsertFullDataBG(soap,NULL,"f20_3_1_4_5","Answered DC",chat_direct_answered+group_calls_answered,ReturnArrayData);
  //f20_3_1_4_7
  InsertFullDataBG(soap,NULL,"f20_3_1_4_7","Answered In Another Group",chat_answered_in_another_group_calls+answered_in_another_group_calls,ReturnArrayData);
  //f20_3_1_4_8
  InsertFullDataBG(soap,NULL,"f20_3_1_4_8","chat_direct_answered_in_another_group+group_calls_answered_in_another_group",chat_direct_answered_in_another_group+group_calls_answered_in_another_group,ReturnArrayData);
  return rc;
}


CsaRc_t GroupRecordReply::MakePerGroup_AbandInfo  (CsaWebServer *soap,GroupRecord*  grec, ns1__BlockItemType *ReturnArrayData )
{
  CsaRc_t        rc = ARC_OK;
  unsigned int*  TmpColorPtr = NULL;

  //bool supportedOutbound ;
  bool supportedChat ;
  //bool supportedEmail ;
  //bool supportedDialList;

  unsigned int accepted_calls=0;
  unsigned int abandoned_calls=0;
  unsigned int longest_wait_of_abnd=0;
  unsigned int group_calls_abandoned=0;
  unsigned int ovf_in_calls_abandoned=0;
  unsigned int avg_wait_of_abnd=0;
  unsigned int chat_abandoned_calls =0;
  unsigned int chat_avg_wait_of_abnd=0;
  unsigned int avg_chat_voice=0;
  unsigned int chat_longest_wait_time_of_abandoned=0;
  unsigned int chat_direct_abandoned=0;
  unsigned int chat_overflowed_in_abandoned=0;
  accepted_calls = grec->Acd[acd_type_acd_offset].accepted_calls.get();
  abandoned_calls = grec->Acd[acd_type_acd_offset].abandoned_calls.get();

  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_4,
                                                abandoned_calls,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_4","abandoned_calls",abandoned_calls,ReturnArrayData);


  if (accepted_calls > 0) // LIMIT 27 UP
  {
    soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,
                                                  soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_4a,
                                                  (unsigned int)((abandoned_calls * 100)/accepted_calls),
                                                  &TmpColorPtr);
    InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_4a","abandoned_calls ???",(unsigned int)((abandoned_calls * 100)/accepted_calls),ReturnArrayData);
  }
  else
    InsertFullDataBG(soap,NULL,"f6_3_1_4_4a","abandoned_calls ???",0,ReturnArrayData);

  // {ERS : 6.3.1.4.6 - longest_wait_of_abnd_calls}   // LIMIT 27 NONE
  longest_wait_of_abnd = grec->Acd[acd_type_acd_offset].longest_wait_time_of_abandoned_calls.get();

  InsertFullDataBG(soap,NULL,"f6_3_1_4_6","longest_wait_of_abnd",longest_wait_of_abnd,ReturnArrayData);

  //174 NONE
  group_calls_abandoned = grec->Acd[acd_type_acd_offset].group_calls_abandoned_per_time_slot.get();
  InsertFullDataBG(soap,NULL,"f6_3_1_4_45","group_calls_abandoned",group_calls_abandoned,ReturnArrayData);

  //175 NONE
  ovf_in_calls_abandoned = grec->Acd[acd_type_acd_offset].overflowed_in_calls_abandoned_per_time_slot.get();
  InsertFullDataBG(soap,NULL,"f6_3_1_4_46","ovf_in_calls_abandoned",ovf_in_calls_abandoned,ReturnArrayData);

  //f6_3_1_4_5  //LIMIT 28 UP
  if(abandoned_calls > 0)
  {
    avg_wait_of_abnd = (unsigned int)(  grec->Acd[acd_type_acd_offset].total_wait_time_of_abandoned_calls.get()/abandoned_calls);
    soap->m_db_data_ptr->limitstab.GetColorOfERS(grec->Grp_id,
                                                 soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_5,
                                                 avg_wait_of_abnd,
                                                 &TmpColorPtr);
    InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_5","avg_wait_of_abnd",avg_wait_of_abnd,ReturnArrayData);
  }
  else
  {
    avg_wait_of_abnd = 0;
    InsertFullDataBG(soap,NULL,"f6_3_1_4_5","avg_wait_of_abnd",0,ReturnArrayData);
  }

  supportedChat = Csa_Setup.supportChat();
  if (supportedChat)
  {
    chat_abandoned_calls = grec->Acd[acd_type_chat_offset].group_calls_abandoned_per_time_slot.get()+
                           grec->Acd[acd_type_chat_offset].overflowed_in_calls_abandoned_per_time_slot.get();
    //[406].lim_type := Up;

    soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,
                                                  soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_69,
                                                  chat_abandoned_calls,
                                                  &TmpColorPtr);
    InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_69","chat_abandoned_calls",chat_abandoned_calls,ReturnArrayData);
    //----
    soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,
                                                  26,       //[440].lim_type := Up; // [26].lim_type := Up;
                                                  chat_abandoned_calls + abandoned_calls,
                                                  &TmpColorPtr);


   //f6_3_1_4_79
   if (chat_abandoned_calls > 0)
   {
     chat_avg_wait_of_abnd = (unsigned int)( grec->Acd[acd_type_chat_offset].total_wait_time_of_abandoned_calls.get()/
                                             chat_abandoned_calls);
     soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_79,
                                                   chat_avg_wait_of_abnd,&TmpColorPtr);
     InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_79","chat_avg_wait_of_abnd",chat_avg_wait_of_abnd,ReturnArrayData);
   }
   else
   {
     chat_avg_wait_of_abnd = 0;
     InsertFullDataBG(soap,NULL,"f6_3_1_4_79","chat_avg_wait_of_abnd",0,ReturnArrayData);
   }

   //NONE  ersIdList[188].ers_name := 'f6_3_1_4_80'; {percent of (f6_3_1_3_20) in required }
   chat_longest_wait_time_of_abandoned = grec->Acd[acd_type_chat_offset].longest_wait_time_of_abandoned_calls.get();
   InsertFullDataBG(soap,NULL,"f6_3_1_4_80","chat_longest_wait_time_of_abandoned",chat_longest_wait_time_of_abandoned,ReturnArrayData);

   //ersIdList[60].lim_type := Up; NOT in DB
   chat_direct_abandoned = grec->Acd[acd_type_chat_offset].group_calls_abandoned_per_time_slot.get();
   soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_70,
                                                 chat_direct_abandoned,&TmpColorPtr);
   InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_70","chat_direct_abandoned",chat_direct_abandoned,ReturnArrayData);

   chat_overflowed_in_abandoned = grec->Acd[acd_type_chat_offset].overflowed_in_calls_abandoned_per_time_slot.get();
   InsertFullDataBG(soap,NULL,"f6_3_1_4_71","chat_overflowed_in_abandoned",chat_overflowed_in_abandoned,ReturnArrayData);

  //------------------------
  }//END CHAT


  //f20_3_1_4_18 // ersIdList[448].lim_type := Up;        //inherits ersIdList[28].ers_id := f6_3_1_4_5;
  if (chat_abandoned_calls+abandoned_calls > 0 )
  {
    avg_chat_voice =(unsigned int)( ((chat_avg_wait_of_abnd * chat_abandoned_calls) +
                                     (avg_wait_of_abnd * abandoned_calls)) /
                                     (chat_abandoned_calls + abandoned_calls));
    //20-Mar-2019 YR BZ#49198
    soap->m_db_data_ptr->limitstab.GetColorOfERS(grec->Grp_id,
                                                 soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_5, //f20_3_1_4_18 // ersIdList[448].lim_type := Up; inherits ersIdList[28].ers_id := f6_3_1_4_5;
                                                 avg_chat_voice,
                                                 &TmpColorPtr);
    InsertFullDataBG(soap,TmpColorPtr,"f20_3_1_4_18","avg_chat_voice",avg_chat_voice,ReturnArrayData);
  }
  else
  {
    avg_chat_voice = 0;
    InsertFullDataBG(soap,NULL,"f20_3_1_4_18","avg_chat_voice",0,ReturnArrayData);
  }

  //NONE
  if(chat_longest_wait_time_of_abandoned > longest_wait_of_abnd)
    InsertFullDataBG(soap,NULL,"f20_3_1_4_19","longest_wait_time_of_abandoned",chat_longest_wait_time_of_abandoned,ReturnArrayData);
  else
    InsertFullDataBG(soap,NULL,"f20_3_1_4_19","longest_wait_time_of_abandoned",longest_wait_of_abnd,ReturnArrayData);


  //AFTER
  //20-Mar-2019 YR BZ#49198
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_4,//f20_3_1_4_10
                                                chat_abandoned_calls + abandoned_calls,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f20_3_1_4_10","Abnd.",chat_abandoned_calls + abandoned_calls,ReturnArrayData);
  //
  InsertFullDataBG(soap,NULL,"f20_3_1_4_11","Abnd DC",chat_direct_abandoned+group_calls_abandoned,ReturnArrayData);
  InsertFullDataBG(soap,NULL,"f20_3_1_4_12","chat_overflowed_in_abandoned+ovf_in_calls_abandoned",chat_overflowed_in_abandoned+ovf_in_calls_abandoned,ReturnArrayData);
  return rc;
}


//-------------------------------------------------------------
CsaRc_t      GroupRecordReply::MakePerGroup_InterflInfo  (CsaWebServer *soap,GroupRecord*  grec, ns1__BlockItemType *ReturnArrayData )
{
  CsaRc_t       rc = ARC_OK;
  unsigned int* TmpColorPtr = NULL;

  unsigned int intfl_out = 0;
  unsigned int accepted_calls = 0;
  unsigned int answered_calls = 0;
  unsigned int prz_intrfl_calls = 0;
  unsigned int chat_intfl_out = 0;
  unsigned int mail_intfl_out = 0;
  unsigned int avg_wait_of_intfl_from_grp = 0;
  unsigned int mail_avg_wait_of_intfl_from_grp = 0;
  unsigned int chat_avg_wait_of_intfl_from_grp = 0;
  unsigned int  avg_chat_voice = 0;
  unsigned int  longest_wait_time_of_intfl = 0;
  unsigned int  chat_longest_wait_time_of_interflowed_out = 0;
  unsigned int  intfl_in = 0;
  unsigned int  prz_ovfl_in = 0;
  unsigned int  group_calls_intf_out = 0;
  unsigned int  chat_direct_interflowed_out = 0;
  unsigned int  ovrfl_out = 0;
  unsigned int  ovrfl_in = 0;
  unsigned int  chat_overflowed_in_accepted = 0;
  unsigned int  ovrfl_in_calls_answred = 0;
  unsigned int  chat_overflowed_in_answered = 0;
  unsigned int  ovrfl_in_calls_answred_in_another_group = 0;
  unsigned int  chat_overflowed_in_answered_in_another_group = 0;
  unsigned int  ovrfl_in_calls_intfl_out = 0;
  unsigned int  chat_overflowed_in_interflowed_out = 0;
  unsigned int  handled_o_acd_calls = 0;
  unsigned int  cmpn_hung_up_calls = 0;
  unsigned int  cmpn_no_cust_calls = 0;
  unsigned int  cmpn_unatt_calls = 0;
  unsigned int  oacd_dialed_calls = 0;
  unsigned int  answered_o_acd_calls = 0;
  unsigned int  failed_o_acd_calls = 0;
  unsigned int  o_acd_calls_through_queue = 0;
  unsigned int  o_acd_total_queue_time = 0;
  unsigned int  o_acd_longest_queue_time = 0;
  unsigned int  o_acd_total_talk_time = 0;
  unsigned int  o_acd_total_wait_time_of_failed_calls = 0;
  unsigned int  o_acd_longest_wait_time_of_failed_calls = 0;
  unsigned int  o_acd_longest_talk_time = 0;
  unsigned int  avg_talk_time_o_acd_calls = 0;
  unsigned int  avg_wait_time_failed_o_acd_calls = 0;
  unsigned int  avg_wait_time_o_acd_calls_in_queue = 0;

  unsigned int target_asa = 0;
  unsigned int chat_accepted_calls = 0;
  unsigned int mail_accepted_calls = 0;
  unsigned int originally_accepted = 0;
  unsigned int chat_direct_accepted = 0;
  CsaTime_t avarage_time, max_time, max_time2 = 0;
  Ulong_t dummy2;
  GenSti_t  sti;
  unsigned int lng_talk_time_o_acd_calls;


  //10-Mar-2021 YR BZ#53632
  accepted_calls = grec->Acd[acd_type_acd_offset].accepted_calls.get();
  //accepted_calls = grec->Acd[acd_type_acd_offset].calls_requesting_callback.get() +
  //                 grec->Acd[acd_type_acd_offset].group_calls_answered_per_time_slot.get() +
  //                 grec->Acd[acd_type_acd_offset].group_calls_abandoned_per_time_slot.get() +
  //                 grec->Acd[acd_type_acd_offset].group_calls_answered_in_another_group.get() +
  //                 grec->Acd[acd_type_acd_offset].group_calls_interflowed_out.get() +
  //                 grec->Acd[acd_type_acd_offset].overflowed_in_calls_answered_per_time_slot.get() +
  //                 grec->Acd[acd_type_acd_offset].overflowed_in_calls_abandoned_per_time_slot.get() +
  //                 grec->Acd[acd_type_acd_offset].overflowed_in_calls_answered_in_another_group.get() +
  //                 grec->Acd[acd_type_acd_offset].overflowed_in_calls_interflowed_out.get();

  answered_calls = grec->Acd[acd_type_acd_offset].group_calls_answered_per_time_slot.get() +
                   grec->Acd[acd_type_acd_offset].overflowed_in_calls_answered_per_time_slot.get();


  /*---------------------------------------------------------------------
  Inbound ACD Statistics
  ---------------------------------------------------------------------*/
  // fields from calls now connected

  max_time =0; //!!!!!!!!!!!!!!!!

  //  {ERS: 6.3.1.4.2 - accepted_calls}
  InsertFullDataBG(soap,NULL,"f6_3_1_4_2","Accepted",accepted_calls,ReturnArrayData);


  // {ERS : 6.3.1.4.7 & 6.3.1.4.7a - intfl_out}

  intfl_out = grec->Acd[acd_type_acd_offset].group_calls_interflowed_out.get() +
              grec->Acd[acd_type_acd_offset].overflowed_in_calls_interflowed_out.get();
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_7,
                                                intfl_out,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_7","intfl_out",intfl_out,ReturnArrayData);

  if( accepted_calls > 0 )
    prz_intrfl_calls = (unsigned int)((intfl_out * 100) / accepted_calls );
  else
    prz_intrfl_calls =0;

  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_7a,
                                                prz_intrfl_calls,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_7a","% Intrfl calls",prz_intrfl_calls,ReturnArrayData);


  //{ERS : 6.3.1.4.8 - avg_wait_of_intfl_from_grp}
  if (intfl_out > 0 )
    avg_wait_of_intfl_from_grp =(unsigned int)(grec->Acd[acd_type_acd_offset].total_wait_time_of_interflowed_calls.get()
    /intfl_out);
  else
    avg_wait_of_intfl_from_grp = 0;
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_8,
                                                avg_wait_of_intfl_from_grp,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_8","avg_wait_of_intfl_from_grp",avg_wait_of_intfl_from_grp,ReturnArrayData);

  target_asa  = grec->Target_asa;
  InsertFullDataBG(soap,NULL,"f6_3_1_1_5","TASA",target_asa,ReturnArrayData);


  //{ERS : 6.3.1.4.9 - longest_wait_time_of_intfl}
  longest_wait_time_of_intfl = grec->Acd[acd_type_acd_offset].longest_wait_time_of_interflowed_calls.get();
  InsertFullDataBG(soap,NULL,"f6_3_1_4_9","longest_wait_time_of_intfl",longest_wait_time_of_intfl,ReturnArrayData);


  //{ERS : 6.3.1.4.10 & 6.3.1.4.10a - intfl_in and % of accepted_calls}
  intfl_in = grec->Acd[acd_type_acd_offset].interflowed_in_calls.get();
  InsertFullDataBG(soap,NULL,"f6_3_1_4_10","intfl_in",intfl_in,ReturnArrayData);
  if (accepted_calls > 0)
    prz_ovfl_in = (unsigned int)((intfl_in *100)/accepted_calls );
  else
    prz_ovfl_in = 0;
  InsertFullDataBG(soap,NULL,"f6_3_1_4_10a","% ovfl in",prz_ovfl_in,ReturnArrayData);


  //{ERS : f6.3.1.4.44 group_calls_interflowed_out}

  group_calls_intf_out = grec->Acd[acd_type_acd_offset].group_calls_interflowed_out.get();
  InsertFullDataBG(soap,NULL,"f6_3_1_4_44","group_calls_intf_out",group_calls_intf_out,ReturnArrayData);

  // {ERS : f6.3.1.4.47 overflowed_out}
  ovrfl_out = grec->Acd[acd_type_acd_offset].overflowed_out_calls.get();
  InsertFullDataBG(soap,NULL,"f6_3_1_4_47","ovrfl_out",ovrfl_out,ReturnArrayData);

  //{ERS : f6.3.1.4.39 overflowed_in}
  ovrfl_in = grec->Acd[acd_type_acd_offset].overflowed_in_calls.get();
  InsertFullDataBG(soap,NULL,"f6_3_1_4_39","ovrfl_in",ovrfl_in,ReturnArrayData);


  //{ERS : f6.3.1.4.40 overflowed_in_calls_answred}
  ovrfl_in_calls_answred = grec->Acd[acd_type_acd_offset].overflowed_in_calls_answered_per_time_slot.get();
  InsertFullDataBG(soap,NULL,"f6_3_1_4_40","ovrfl_in_calls_answred",ovrfl_in_calls_answred,ReturnArrayData);


  //{ERS : f6.3.1.4.41 overflowed_in_calls_answered_in_another_group}
  ovrfl_in_calls_answred_in_another_group = grec->Acd[acd_type_acd_offset].overflowed_in_calls_answered_in_another_group.get();
  InsertFullDataBG(soap,NULL,"f6_3_1_4_41","ovrfl_in_calls_answred_in_another_group",ovrfl_in_calls_answred_in_another_group,ReturnArrayData);


  //{ERS : f6.3.1.4.43 overflowed_in_calls_interflowed_out}
  ovrfl_in_calls_intfl_out = grec->Acd[acd_type_acd_offset].overflowed_in_calls_interflowed_out.get();
  InsertFullDataBG(soap,NULL,"f6_3_1_4_43","ovrfl_in_calls_intfl_out",ovrfl_in_calls_intfl_out,ReturnArrayData);

  //   {ERS: 6.3.1.4.2 - originally_accepted_calls}
  originally_accepted = grec->Acd[acd_type_acd_offset].originally_accepted_calls.get();
  InsertFullDataBG(soap,NULL,"f6_3_1_4_36","Accepted",originally_accepted,ReturnArrayData);


/*---------------------------------------------------------------------
						E-MAil ACD Statistics
---------------------------------------------------------------------*/
  //PVL 2-06-2010 1004133 bool supportedEmail =    Csa_Setup.supportEmail();
  if (supportedEmail)
  {
    //MAIL_
    mail_accepted_calls = grec->Acd[acd_type_email_offset].originally_accepted_calls.get()+
                          grec->Acd[acd_type_email_offset].overflowed_in_calls.get();
    InsertFullDataBG(soap,NULL,"f6_3_1_4_90","Emails Accepted",mail_accepted_calls,ReturnArrayData);


    //EMAIL_
    mail_intfl_out = grec->Acd[acd_type_email_offset].group_calls_interflowed_out.get() +
                     grec->Acd[acd_type_email_offset].overflowed_in_calls_interflowed_out.get();
    InsertFullDataBG(soap,NULL,"f6_3_1_4_99","Emails intfl",mail_intfl_out,ReturnArrayData);

    //EMAIL_
    if( mail_intfl_out > 0 )
      mail_avg_wait_of_intfl_from_grp = (unsigned int)(grec->Acd[acd_type_email_offset].total_wait_time_of_interflowed_calls.get()/mail_intfl_out);
    else
      mail_avg_wait_of_intfl_from_grp = 0;

    soap->m_db_data_ptr->limitstab.GetColorOfERS(grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_76,
                                                 mail_avg_wait_of_intfl_from_grp,&TmpColorPtr);
    InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_76","mail_avg_wait_of_intfl_from_grp",mail_avg_wait_of_intfl_from_grp,ReturnArrayData);
  }

/*---------------------------------------------------------------------
							Chat ACD Statistics
---------------------------------------------------------------------*/
  //PVL 2-06-2010 1004133 bool supportedChat =     Csa_Setup.supportChat();
  if (supportedChat)
  {
    // CHAT_
    chat_accepted_calls = grec->Acd[acd_type_chat_offset].originally_accepted_calls.get()+
		                  grec->Acd[acd_type_chat_offset].overflowed_in_calls.get();
    InsertFullDataBG(soap,NULL,"f6_3_1_4_60","Accepted",chat_accepted_calls,ReturnArrayData);
    //CHAT
    chat_direct_accepted = grec->Acd[acd_type_chat_offset].originally_accepted_calls.get();
    InsertFullDataBG(soap,NULL,"f6_3_1_4_61","Accepted",chat_direct_accepted,ReturnArrayData);

    //CHAT_
    if( chat_intfl_out > 0)
      chat_avg_wait_of_intfl_from_grp = (unsigned int)(grec->Acd[acd_type_chat_offset].total_wait_time_of_interflowed_calls.get()
                                          /chat_intfl_out);
      else
        chat_avg_wait_of_intfl_from_grp = 0;
    InsertFullDataBG(soap,NULL,"f6_3_1_4_103","chat_avg_wait_of_intfl_from_grp",chat_avg_wait_of_intfl_from_grp,ReturnArrayData);

    //CHAT_
    chat_intfl_out = grec->Acd[acd_type_chat_offset].group_calls_interflowed_out.get() +
		             grec->Acd[acd_type_chat_offset].overflowed_in_calls_interflowed_out.get();
	soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_72,
                                                  chat_intfl_out,&TmpColorPtr);
    InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_72","chat_intfl_out",chat_intfl_out,ReturnArrayData);
    //CHAT_
    chat_longest_wait_time_of_interflowed_out = grec->Acd[acd_type_chat_offset].longest_wait_time_of_interflowed_calls.get();
    InsertFullDataBG(soap,NULL,"f6_3_1_4_77","chat_longest_wait_time_of_interflowed_out",chat_longest_wait_time_of_interflowed_out,ReturnArrayData);
    //CHAT_
    chat_direct_interflowed_out =   grec->Acd[acd_type_chat_offset].group_calls_interflowed_out.get();
    soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_73,
                                                  chat_direct_interflowed_out,&TmpColorPtr);
    InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_73","chat_direct_interflowed_out",chat_direct_interflowed_out,ReturnArrayData);
    //CHAT_
    chat_overflowed_in_accepted =grec->Acd[acd_type_chat_offset].overflowed_in_calls.get();
    InsertFullDataBG(soap,NULL,"f6_3_1_4_62","chat_overflowed_in_accepted",chat_overflowed_in_accepted,ReturnArrayData);
    //CHAT
    chat_overflowed_in_answered = grec->Acd[acd_type_chat_offset].overflowed_in_calls_answered_per_time_slot.get();
    soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_65,
                                                  chat_overflowed_in_answered,&TmpColorPtr);
    InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_65","chat_overflowed_in_answered",chat_overflowed_in_answered,ReturnArrayData);

    //CHAT_
    chat_overflowed_in_answered_in_another_group =  grec-> Acd[acd_type_chat_offset].overflowed_in_calls_answered_in_another_group.get();
    soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_68,
                                                  chat_overflowed_in_answered_in_another_group,&TmpColorPtr);
    InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_68","chat_overflowed_in_answered_in_another_group",chat_overflowed_in_answered_in_another_group,ReturnArrayData);
    //CHAT
    chat_overflowed_in_interflowed_out =   grec->Acd[acd_type_chat_offset].overflowed_in_calls_interflowed_out.get();
    InsertFullDataBG(soap,NULL,"f6_3_1_4_74","chat_overflowed_in_interflowed_out",chat_overflowed_in_interflowed_out,ReturnArrayData);

  }
/*---------------------------------------------------------------------
			Outbound ACD Statistics (Callback + Abandoned)
---------------------------------------------------------------------*/
  //PVL 2-06-2010 1004133 bool supportedOutbound = Csa_Setup.supportOutbound();
  if (supportedOutbound)
  {
    if (!grec->nullConnectedTable(acd_type_callback_offset)) {
      grec->Connected_calls[acd_type_callback_offset]->getStatistics(avarage_time,max_time,sti,dummy2);
    }
    if (!grec->nullConnectedTable(acd_type_abandoned_offset)) {
      grec->Connected_calls[acd_type_abandoned_offset]->getStatistics(avarage_time,max_time2,sti,dummy2);
    }
    o_acd_longest_talk_time = myOldestTime(max_time,max_time2);

    //4-Jul-2023 YR BZ#58021
    //if (!grec->nullConnectedTable(acd_type_dial_list_offset)) {
    //  grec->Connected_calls[acd_type_dial_list_offset]->getStatistics(avarage_time,max_time,sti,dummy2);
    //}
    o_acd_longest_talk_time = myOldestTime(max_time,o_acd_longest_talk_time);

    CsaTime_t time_now = CsaTimTimeNow();
    CsaTime_t time_tmp;
    time_tmp   = mymax(grec->Acd[acd_type_callback_offset].longest_talk_time.get(),
      grec->Acd[acd_type_abandoned_offset].longest_talk_time.get());
    o_acd_longest_talk_time = 0;
    if(time_tmp != 0)
      o_acd_longest_talk_time = time_now - time_tmp;


    handled_o_acd_calls = grec->Acd[acd_type_callback_offset].accepted_calls.get() +
                          grec->Acd[acd_type_abandoned_offset].accepted_calls.get();

    soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_48,
                                                  handled_o_acd_calls,&TmpColorPtr);
    InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_48","handled_o_acd_calls",handled_o_acd_calls,ReturnArrayData);

    answered_o_acd_calls = grec->Acd[acd_type_callback_offset].group_calls_answered_per_time_slot.get() +
                           grec->Acd[acd_type_abandoned_offset].group_calls_answered_per_time_slot.get() +
                           grec->Acd[acd_type_callback_offset].overflowed_in_calls_answered_per_time_slot.get() +
                           grec->Acd[acd_type_abandoned_offset].overflowed_in_calls_answered_per_time_slot.get();

    failed_o_acd_calls = grec->Acd[acd_type_callback_offset].abandoned_calls.get() +
                         grec->Acd[acd_type_abandoned_offset].abandoned_calls.get() +
                         grec->Acd[acd_type_callback_offset].blocked_calls.get() +
                         grec->Acd[acd_type_abandoned_offset].blocked_calls.get() +
                         grec->Acd[acd_type_callback_offset].group_calls_interflowed_out.get() +
                         grec->Acd[acd_type_callback_offset].overflowed_in_calls_interflowed_out.get() +
                         grec->Acd[acd_type_abandoned_offset].group_calls_interflowed_out.get() +
                         grec->Acd[acd_type_abandoned_offset].overflowed_in_calls_interflowed_out.get();

    o_acd_calls_through_queue = grec->Acd[acd_type_callback_offset].queued_calls.get() +
                                grec->Acd[acd_type_abandoned_offset].queued_calls.get();

    o_acd_total_queue_time = grec->Acd[acd_type_callback_offset].total_queue_time.get() +
                             grec->Acd[acd_type_abandoned_offset].total_queue_time.get();

    o_acd_longest_queue_time = mymax(grec->Acd[acd_type_callback_offset].longest_queue_time.get(),
                                     grec->Acd[acd_type_callback_offset].longest_queue_time.get());

    o_acd_total_talk_time = grec->Acd[acd_type_callback_offset].total_talk_time.get() +
                            grec->Acd[acd_type_abandoned_offset].total_talk_time.get();

    o_acd_total_wait_time_of_failed_calls = grec->Acd[acd_type_callback_offset].total_wait_time_of_abandoned_calls.get() +
                                            grec->Acd[acd_type_abandoned_offset].total_wait_time_of_abandoned_calls.get() +
                                            grec->Acd[acd_type_callback_offset].total_wait_time_of_interflowed_calls.get() +
                                            grec->Acd[acd_type_abandoned_offset].total_wait_time_of_interflowed_calls.get();
    o_acd_longest_wait_time_of_failed_calls = mymax(grec->Acd[acd_type_callback_offset].longest_wait_time_of_abandoned_calls.get(),
                                                    grec->Acd[acd_type_abandoned_offset].longest_wait_time_of_abandoned_calls.get());
    o_acd_longest_wait_time_of_failed_calls = (UINT) mymax(o_acd_longest_wait_time_of_failed_calls,
                                                           grec->Acd[acd_type_callback_offset].longest_wait_time_of_interflowed_calls.get());
    o_acd_longest_wait_time_of_failed_calls = (UINT) mymax(o_acd_longest_wait_time_of_failed_calls,
                                                           grec->Acd[acd_type_abandoned_offset].longest_wait_time_of_interflowed_calls.get());

  }
  /*---------------------------------------------------------------------
						Dial List ACD Statistics
---------------------------------------------------------------------*/
  //PVL 2-06-2010 1004133 bool supportedDialList = Csa_Setup.supportDialList();
  if (supportedDialList)
  {
    if (!grec->nullConnectedTable(acd_type_dial_list_offset))
    {
      grec->Connected_calls[acd_type_dial_list_offset]->getStatistics(avarage_time,max_time,sti,dummy2);
      o_acd_longest_talk_time = myOldestTime(o_acd_longest_talk_time,max_time);
    }
    o_acd_longest_talk_time	= myOldestTime(o_acd_longest_talk_time,grec->Acd[acd_type_dial_list_offset].longest_talk_time.get());
    //DIAL
    cmpn_hung_up_calls = grec->Acd[acd_type_dial_list_offset].outb_completed_calls_hung_at_ivr.get();
    InsertFullDataBG(soap,NULL,"f6_3_1_4_115","cmpn_hung_up_calls",cmpn_hung_up_calls,ReturnArrayData);

    //DIAL
    cmpn_no_cust_calls = grec->Acd[acd_type_dial_list_offset].outb_failed_to_complete_in_ivr.get();
    InsertFullDataBG(soap,NULL,"f6_3_1_4_116","cmpn_no_cust_calls",cmpn_no_cust_calls,ReturnArrayData);
    //DIAL
    cmpn_unatt_calls = grec->Acd[acd_type_dial_list_offset].outb_unattended.get();
    InsertFullDataBG(soap,NULL,"f6_3_1_4_117","cmpn_unatt_calls",cmpn_unatt_calls,ReturnArrayData);

    //DIAL
    answered_o_acd_calls += grec->Acd[acd_type_dial_list_offset].group_calls_answered_per_time_slot.get() +
                            grec->Acd[acd_type_dial_list_offset].overflowed_in_calls_answered_per_time_slot.get();

    //DIAL
    failed_o_acd_calls += grec->Acd[acd_type_dial_list_offset].abandoned_calls.get() +
                          grec->Acd[acd_type_dial_list_offset].blocked_calls.get() +
                          grec->Acd[acd_type_dial_list_offset].group_calls_interflowed_out.get() +
                          grec->Acd[acd_type_dial_list_offset].overflowed_in_calls_interflowed_out.get();

    oacd_dialed_calls = answered_o_acd_calls + cmpn_hung_up_calls + cmpn_no_cust_calls;
    InsertFullDataBG(soap,NULL,"f6_3_1_4_114","oacd_dialed_calls",oacd_dialed_calls,ReturnArrayData);

    //DL
    handled_o_acd_calls						+= grec->Acd[acd_type_dial_list_offset].accepted_calls.get();
    o_acd_calls_through_queue               += grec->Acd[acd_type_dial_list_offset].queued_calls.get();
    o_acd_total_queue_time                  += grec->Acd[acd_type_dial_list_offset].total_queue_time.get();
    o_acd_longest_queue_time                = mymax(o_acd_longest_queue_time,grec->Acd[acd_type_dial_list_offset].longest_queue_time.get());
    o_acd_total_talk_time					+= grec->Acd[acd_type_dial_list_offset].total_talk_time.get();
    o_acd_total_wait_time_of_failed_calls	+= grec->Acd[acd_type_dial_list_offset].total_wait_time_of_abandoned_calls.get() +
                                               grec->Acd[acd_type_dial_list_offset].total_wait_time_of_interflowed_calls.get();
    o_acd_longest_wait_time_of_failed_calls	= (UINT) mymax(o_acd_longest_wait_time_of_failed_calls,
                                                           grec->Acd[acd_type_dial_list_offset].longest_wait_time_of_abandoned_calls.get());
    o_acd_longest_wait_time_of_failed_calls = (UINT) mymax(o_acd_longest_wait_time_of_failed_calls,
                                                           grec->Acd[acd_type_dial_list_offset].longest_wait_time_of_interflowed_calls.get());

  }//end of DL
  ///----------------- AFTER--------------------
  //AFTER
  InsertFullDataBG(soap,NULL,"f20_3_1_4_1","Accepted",chat_accepted_calls+accepted_calls,ReturnArrayData);

  //AFTER
  InsertFullDataBG(soap,NULL,"f20_3_1_4_2","Accepted",originally_accepted+chat_direct_accepted,ReturnArrayData);

  //AFTER
  InsertFullDataBG(soap,NULL,"f20_3_1_4_3","ovrfl_in + chat_overflowed_in_accepted",ovrfl_in + chat_overflowed_in_accepted,ReturnArrayData);

  //AFTER
  //20-Mar-2019 YR BZ#49198
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_7,//20_3_1_4_13
    chat_intfl_out +intfl_out,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f20_3_1_4_13","intfl. out",chat_intfl_out +intfl_out,ReturnArrayData);

  //AFTER
  if(chat_intfl_out+intfl_out  > 0 )
    avg_chat_voice = (unsigned int)(((chat_avg_wait_of_intfl_from_grp * chat_intfl_out) + (avg_wait_of_intfl_from_grp * intfl_out))
                                      /(chat_intfl_out+intfl_out) );
  else
    avg_chat_voice = 0;
  InsertFullDataBG(soap,NULL,"f20_3_1_4_16","avg_chat_voice",avg_chat_voice,ReturnArrayData);

  //AFTER
  if(chat_longest_wait_time_of_interflowed_out > longest_wait_time_of_intfl)
    InsertFullDataBG(soap,NULL,"f20_3_1_4_17","Interflowed",chat_longest_wait_time_of_interflowed_out,ReturnArrayData);
   else
     InsertFullDataBG(soap,NULL,"f20_3_1_4_17","Interflowed",longest_wait_time_of_intfl,ReturnArrayData);
  //AFTER
  InsertFullDataBG(soap,TmpColorPtr,"f20_3_1_4_14","DC Interflowed Out",chat_direct_interflowed_out + group_calls_intf_out,ReturnArrayData);
  //AFTER
  InsertFullDataBG(soap,NULL,"f20_3_1_4_6","Overflowed In Answered by Group",chat_overflowed_in_answered + ovrfl_in_calls_answred,ReturnArrayData);
  //AFTER
  InsertFullDataBG(soap,NULL,"f20_3_1_4_9","Overflowed In Answered by Another Group",chat_overflowed_in_answered_in_another_group + ovrfl_in_calls_answred_in_another_group,ReturnArrayData);
  //AFTER
  InsertFullDataBG(soap,NULL,"f20_3_1_4_15","Overflowed In, Interflowed Out",ovrfl_in_calls_intfl_out + chat_overflowed_in_interflowed_out,ReturnArrayData);
  //AFTER
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_49,
                                                answered_o_acd_calls,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_49","answered_o_acd_calls",answered_o_acd_calls,ReturnArrayData);
  //AFTER
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_50,
                                                failed_o_acd_calls,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_50","failed_o_acd_calls",failed_o_acd_calls,ReturnArrayData);
  //AFTER   //f6_3_1_4_52
  if (o_acd_calls_through_queue > 0 )
    avg_wait_time_o_acd_calls_in_queue =(unsigned int)(o_acd_total_queue_time/o_acd_calls_through_queue);
  else
    avg_wait_time_o_acd_calls_in_queue = 0;
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_52,
                                                avg_wait_time_o_acd_calls_in_queue,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_52","avg_wait_time_o_acd_calls_in_queue",avg_wait_time_o_acd_calls_in_queue,ReturnArrayData);

  //AFTER f6_3_1_4_53
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_53,
                                                o_acd_longest_queue_time,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_53","o_acd_longest_queue_time",o_acd_longest_queue_time,ReturnArrayData);


  //f6_3_1_4_55
  //AFTER
  if (failed_o_acd_calls > 0)
    avg_wait_time_failed_o_acd_calls = (unsigned int)(o_acd_total_wait_time_of_failed_calls / failed_o_acd_calls);
  else
    avg_wait_time_failed_o_acd_calls = 0;
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_55,
                                                avg_wait_time_failed_o_acd_calls,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_55","avg_wait_time_failed_o_acd_calls",avg_wait_time_failed_o_acd_calls,ReturnArrayData);

  //AFTER f6_3_1_4_56
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_56,
                                                o_acd_longest_wait_time_of_failed_calls,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_56","o_acd_longest_wait_time_of_failed_calls",o_acd_longest_wait_time_of_failed_calls,ReturnArrayData);

  //AFTER f6_3_1_4_58
  if (handled_o_acd_calls > 0)
    avg_talk_time_o_acd_calls = (unsigned int)(o_acd_total_talk_time /handled_o_acd_calls);
  else
    avg_talk_time_o_acd_calls = 0;
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_58,
                                                avg_talk_time_o_acd_calls,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_58","avg_talk_time_o_acd_calls",avg_talk_time_o_acd_calls,ReturnArrayData);


  //AFTER f6_3_1_4_59
  lng_talk_time_o_acd_calls = NlaCalcCsaElapsedTime(o_acd_longest_talk_time);
  if (o_acd_total_talk_time > 0)
    lng_talk_time_o_acd_calls = NlaCalcCsaElapsedTime(o_acd_longest_talk_time);
  else
    lng_talk_time_o_acd_calls = 0;
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_59,
                                                lng_talk_time_o_acd_calls,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_59","lng_talk_time_o_acd_calls",lng_talk_time_o_acd_calls,ReturnArrayData);

  return rc;
}


CsaRc_t  GroupRecordReply::MakePerGroup_AgentsInfo(CsaWebServer *soap,GroupRecord*  grec, ns1__BlockItemType *ReturnArrayData )
{
  CsaRc_t       rc = ARC_OK;
  unsigned int* TmpColorPtr = NULL;
  CsaTime_t avarage_time, max_time,max_time2;
  Ulong_t dummy2 = 0;
  bool supportedOutbound ;
  bool supportedChat ;
  bool supportedEmail ;
  bool supportedDialList;

  //19-Jun-2019 YR BZ#50083
  Ulong_t longest_acd_call_talk_time = 0;
  Ulong_t longest_OACD_call_talk_time = 0;
  Ulong_t chat_longest_contact_time = 0;
  Ulong_t longest_acd_talk_time = 0;

  unsigned int outbound_acd_num_of_agents = 0;
  unsigned int outbound_rsvd_num_of_agents = 0;
  Ulong_t over_time_talking_agents = 0;
  GenSti_t  sti;

  csaTimeNow = time( NULL);
  supportedOutbound = Csa_Setup.supportOutbound();
  supportedChat = Csa_Setup.supportChat();
  supportedEmail = Csa_Setup.supportEmail();
  supportedDialList = Csa_Setup.supportDialList();

  longest_acd_talk_time =grec->Acd[acd_type_acd_offset].longest_talk_time.get();

  //{ERS:  6.3.1.3.8 - longest_acd_call_talk_time}

  // fields from calls now connected
  if (!grec->nullConnectedTable(acd_type_acd_offset))
  {
    grec->Connected_calls[acd_type_acd_offset]->getStatistics(avarage_time,max_time,sti,over_time_talking_agents);
    longest_acd_call_talk_time = max_time;
  }

  //{ERS : 6.3.1.3.10 - primary_agents, secondary }
  InsertFullData(soap,"f6_3_1_3_10","primary_agents",grec->Current_agents_state.primary_agents.get(),ReturnArrayData);
  //-----------

  //ERS:  f6_3_1_3_1 - num_of_agents
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_3_1,
                                                grec->Current_agents_state.number_of_agents.get(),&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_3_1","Logged in",grec->Current_agents_state.number_of_agents.get(),ReturnArrayData);

  //    //ERS : f6_3_1_3_11- agents_on_release}
  InsertFullData(soap,"f6_3_1_3_11","num_of_agents - primary_agents",
                 grec->Current_agents_state.number_of_agents.get() -
                 grec->Current_agents_state.primary_agents.get(),
                 ReturnArrayData);

  //    //ERS : 6.3.1.3.2 - agents_on_release}
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_3_2,
                                                grec->Current_agents_state.agents_on_release.get(),&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_3_2","Release",grec->Current_agents_state.agents_on_release.get(),ReturnArrayData);

  //    //ERS :f6_3_1_3_21, on_phones_num_agents}
  InsertFullData(soap,"f6_3_1_3_21","on_phones_num_agents",
                 grec->Current_agents_state.number_of_agents.get() - grec->Current_agents_state.agents_on_release.get(),ReturnArrayData);

  //    //ERS : 6.3.1.3.3 - agents_on_wrap_up}
  InsertFullData(soap,"f6_3_1_3_3","WrapUp",grec->Current_agents_state.agents_on_wrap_up.get(),ReturnArrayData);

  //    //ERS : 6.3.1.3.4 - idle_agents}
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_3_4,
    grec->Current_agents_state.idle_agents.get(),&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_3_4","Idle",grec->Current_agents_state.idle_agents.get(),ReturnArrayData);

  //      {ERS : 6.3.1.3.9 - busy_agents}
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_3_9,
                                                grec->Current_agents_state.busy_agents.get(),&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_3_9","Busy",grec->Current_agents_state.busy_agents.get(),ReturnArrayData);

  //    //ERS : 6.3.1.3.5 - ACD_agents}
  InsertFullData(soap,"f6_3_1_3_5","ACD",grec->Current_agents_state.acd_talking_agents.get(),ReturnArrayData);

  //    //ERS : 6.3.1.3.6 - non_acd_busy_agents}
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_3_6,
                                                grec->Current_agents_state.non_acd_busy_agents.get(),&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_3_6","Non ACD",grec->Current_agents_state.non_acd_busy_agents.get(),ReturnArrayData);

  // //     {ERS : 6.3.1.3.14 - agents_on_dnd}
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_3_14,
    grec->Current_agents_state.agents_on_dnd.get(),&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_3_14","agents_on_dnd",grec->Current_agents_state.agents_on_dnd.get(),ReturnArrayData);

  //      //{ERS : f6_3_1_3_12 - agents_on_f_busy}
  InsertFullData(soap,"f6_3_1_3_12","agents_on_f_busy",grec->Current_agents_state.agents_on_forced_busy.get(),ReturnArrayData);

  //      //{ERS : 6.3.1.3.15 - agents_on_split}
  InsertFullData(soap,"f6_3_1_3_15","Split",grec->Current_agents_state.agents_on_split.get(),ReturnArrayData);

  //    //    {ERS : 6.3.1.3.13 - agents_on_f_wrap}
  InsertFullData(soap,"f6_3_1_3_13","agents_on_f_wrap",grec->Current_agents_state.agents_on_forced_wrap_up.get(),ReturnArrayData);

  //    //{ERS: 6.3.1.3.16 - agents wrap + f. wrap}
  InsertFullData(soap,"f6_3_1_3_16","agents wrap + f. wrap",
    grec->Current_agents_state.agents_on_wrap_up.get() +
    grec->Current_agents_state.agents_on_forced_wrap_up.get(),ReturnArrayData);

  //f6_3_1_3_22 outbound_rsvd_num_of_agents
  outbound_rsvd_num_of_agents = grec->Current_agents_state.agents_on_outbound_reserved.get();
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_3_22,
                                                outbound_rsvd_num_of_agents,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_3_22","O-Rsrvd",outbound_rsvd_num_of_agents,ReturnArrayData);

  //f6_3_1_3_23, outbound_acd_num_of_agents
  outbound_acd_num_of_agents = grec->Current_agents_state.agents_on_outbound.get();
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_3_23,
                                                outbound_acd_num_of_agents,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_3_23","O-ACD",outbound_acd_num_of_agents,ReturnArrayData);

  //f6_3_1_3_24 out_rsvd_and_out_acd_num_of_agents
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_3_24,
                                                outbound_rsvd_num_of_agents + outbound_acd_num_of_agents,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_3_24","OutBound+Rsrv",outbound_rsvd_num_of_agents + outbound_acd_num_of_agents,ReturnArrayData);
  //------------------------------------
  if(supportedOutbound)
  {
    if (!grec->nullConnectedTable(acd_type_callback_offset))
    {
      rc =grec->Connected_calls[acd_type_callback_offset]->getStatistics(avarage_time,max_time,sti,dummy2);
      if (rc == ARC_OK)
      {
        longest_acd_call_talk_time = myOldestTime(max_time,longest_acd_call_talk_time);
        over_time_talking_agents += dummy2;
      }
    }
    if (!grec->nullConnectedTable(acd_type_abandoned_offset)) //CHANGED to acd_type_abandoned_offset
    {
      rc= grec->Connected_calls[acd_type_abandoned_offset]->getStatistics(avarage_time,max_time2,sti,dummy2);
      if (rc == ARC_OK)
      {
        longest_acd_call_talk_time = myOldestTime(max_time2,longest_acd_call_talk_time); //NEW
        over_time_talking_agents += dummy2; //NEW
      }
    }
    longest_OACD_call_talk_time = myOldestTime(max_time,max_time2);
  }

  //------------------------------------
  if(supportedDialList)
  {
    if (!grec->nullConnectedTable(acd_type_dial_list_offset))
    {
      rc =grec->Connected_calls[acd_type_dial_list_offset]->getStatistics(avarage_time,max_time,sti,dummy2);
      if (rc == ARC_OK)
      {
        longest_OACD_call_talk_time = myOldestTime(max_time,longest_OACD_call_talk_time);
        longest_acd_call_talk_time = myOldestTime(max_time,longest_acd_call_talk_time);
        over_time_talking_agents += dummy2;
      }
    }
  }
  //------------------------------------
  if (supportedChat)
  {
    if (!grec->nullConnectedTable(acd_type_chat_offset))
    {
      rc = grec->Connected_calls[acd_type_chat_offset]->getStatistics(avarage_time,max_time,sti,dummy2);
      if (rc == ARC_OK)
      {
        longest_acd_call_talk_time = myOldestTime(max_time,longest_acd_call_talk_time);
        over_time_talking_agents += dummy2;
      }
    }
    //f6_3_1_4_89
    chat_longest_contact_time = grec->Acd[acd_type_chat_offset].longest_talk_time.get();
    soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_89,
                                                  chat_longest_contact_time,&TmpColorPtr);
    InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_89","chat_longest_contact_time",chat_longest_contact_time,ReturnArrayData);

    //f6_3_1_4_81
    //NOT SUPPORTED	 InsertFullData(soap,"f6_3_1_4_81","chat_total_wait_time_of_answered",grec->Acd[acd_type_chat_offset].total_wait_till_answer_time.get(),ReturnArrayData);
    //f6_3_1_4_83	chat_longest_wait_time_of_answered
    //NOT SUPPORTED	 InsertFullData(soap,"f6_3_1_4_83","chat_longest_wait_time_of_answered",grec->Acd[acd_type_chat_offset].longest_wait_till_answer_time.get(),ReturnArrayData);
  }
  //f20_3_1_4_21
  if(chat_longest_contact_time > longest_acd_talk_time)
    InsertFullData(soap,"f20_3_1_4_21","longest_contact_time",chat_longest_contact_time,ReturnArrayData);
  else
    InsertFullData(soap,"f20_3_1_4_21","longest_contact_time",longest_acd_talk_time,ReturnArrayData);


  //--------------------------------------
  if (supportedEmail)
  {
    if (!grec->nullConnectedTable(acd_type_email_offset))
    {
      rc = grec->Connected_calls[acd_type_email_offset]->getStatistics(avarage_time,max_time,sti,dummy2);
      if (rc == ARC_OK)
      {
        longest_acd_call_talk_time = myOldestTime(max_time,longest_acd_call_talk_time);
        over_time_talking_agents += dummy2;
      }
    }
    //f6_3_1_3_25
    soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_3_25,
                                                  grec->Current_agents_state.agents_on_email.get(),&TmpColorPtr);
    InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_3_25","agents_on_email",grec->Current_agents_state.agents_on_email.get(),ReturnArrayData);
    //f6_3_1_4_91
    InsertFullData(soap,"f6_3_1_4_91","email_direct_accepted",grec->Acd[acd_type_email_offset].originally_accepted_calls.get(),ReturnArrayData);
    //f6_3_1_4_92
    InsertFullData(soap,"f6_3_1_4_92","email_overflowed_in_accepted",grec->Acd[acd_type_email_offset].overflowed_in_calls.get(),ReturnArrayData);
    //f6_3_1_4_94
    InsertFullData(soap,"f6_3_1_4_94","email_direct_answered",grec->Acd[acd_type_email_offset].group_calls_answered_per_time_slot.get(),ReturnArrayData);
    //f6_3_1_4_95
    soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_95,
                                                  grec->Acd[acd_type_email_offset].overflowed_in_calls_answered_per_time_slot.get(),&TmpColorPtr);
    InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_95","email_overflowed_in_answered",grec->Acd[acd_type_email_offset].overflowed_in_calls_answered_per_time_slot.get(),ReturnArrayData);
    //f6_3_1_4_97
    soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_97,
                                                  grec->Acd[acd_type_email_offset].group_calls_answered_in_another_group.get(),&TmpColorPtr);
    InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_97","email_direct_answered_in_another_group",grec->Acd[acd_type_email_offset].group_calls_answered_in_another_group.get(),ReturnArrayData);
    //f6_3_1_4_98
    InsertFullData(soap,"f6_3_1_4_98","email_overflowed_in_answered_in_another_group",grec->Acd[acd_type_email_offset].overflowed_in_calls_answered_in_another_group.get(),ReturnArrayData);
    //f6_3_1_4_100
    soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_4_100,
                                                  grec->Acd[acd_type_email_offset].group_calls_interflowed_out.get(),&TmpColorPtr);
    InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_4_100","email_direct_interflowed_out",grec->Acd[acd_type_email_offset].group_calls_interflowed_out.get(),ReturnArrayData);
    //f6_3_1_4_101
    InsertFullData(soap,"f6_3_1_4_101","email_overflowed_in_interflowed_out",grec->Acd[acd_type_email_offset].overflowed_in_calls_interflowed_out.get(),ReturnArrayData);
    //f6_3_1_4_104
    InsertFullData(soap,"f6_3_1_4_104","email_longest_wait_time_of_interflowed_out",grec->Acd[acd_type_email_offset].longest_wait_time_of_interflowed_calls.get(),ReturnArrayData);
    //f6_3_1_4_105
    //NOT IN USE  InsertFullData(soap,"f6_3_1_4_105","email_total_wait_time_of_answered",grec->Acd[acd_type_email_offset].total_wait_till_answer_time.get(),ReturnArrayData);
    //f6_3_1_4_107
    //NOT IN USE  InsertFullData(soap,"f6_3_1_4_107","longest_wait_time_of_answered",grec->Acd[acd_type_email_offset].longest_wait_till_answer_time.get(),ReturnArrayData);
    //f6_3_1_4_113
    InsertFullData(soap,"f6_3_1_4_113","email_longest_interaction_time",grec->Acd[acd_type_email_offset].longest_talk_time.get(),ReturnArrayData);
  }

  if (longest_acd_call_talk_time > 0)
  {
    longest_acd_call_talk_time = NlaCalcCsaElapsedTime(longest_acd_call_talk_time);
  }
  else
  {
    longest_acd_call_talk_time = 0;
  }
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_3_8,
    longest_acd_call_talk_time,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_3_8","longest_acd_call_talk_time",longest_acd_call_talk_time,ReturnArrayData);


  //    //    {ERS : 6.3.1.3.13 - agents_on_f_wrap}
  soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_1_3_7,
                                                over_time_talking_agents,&TmpColorPtr);
  InsertFullDataBG(soap,TmpColorPtr,"f6_3_1_3_7","over_time_talking_agents",over_time_talking_agents,ReturnArrayData);

  return rc;
}


