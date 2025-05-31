#include "er_std_override.h"
#include "csa_web_dnis_reply.hpp"
#include "csa_web_commondefs.hpp"


unsigned int  DNISRecordReply::GetHandleDNISList(CsaWebServer*            soap, //PVL_STAT
                                                 ns1__GenRequestType*     RequestData,
                                                 ns1__DNISListReturnType& ReturnArrayData,
                                                 bool                     is_email)
{
  ReturnArrayData.responseInfoHeader = soap->m_net_data_ptr->ResponseHeader;
  ReturnArrayData.responseInfoHeader->errorCause = 1;
  ReturnArrayData.responseInfoHeader->serviceReqId = RequestData->requestInfoHeader->serviceReqId;

  if(!soap->m_db_data_ptr->TicketTab.VerifyTicket(RequestData->requestInfoHeader->ticket))
    ReturnArrayData.responseInfoHeader->errorCause = 500;

  if(!soap->m_db_data_ptr->dnistab.csa_web_TryLock()) //MT
  {
    ReturnArrayData.responseInfoHeader->errorCause = (unsigned int)700;//MT SERVER BUSY
    return  ReturnArrayData.responseInfoHeader->errorCause ; //PVL_STAT
  }

  soap->m_db_data_ptr->dnistab.LoadDNISList();

  for( soap->m_db_data_ptr->dnistab.it=soap->m_db_data_ptr->dnistab.tab.begin() ;
       soap->m_db_data_ptr->dnistab.it != soap->m_db_data_ptr->dnistab.tab.end();
       soap->m_db_data_ptr->dnistab.it++ )
  {
    ReturnArrayData.responseInfoHeader->errorCause = 0;
    if(soap->m_db_data_ptr->dnistab.it->second.is_email == is_email)
      InsertDNISList(soap,
                     soap->m_db_data_ptr->dnistab.it->first,
                     soap->m_db_data_ptr->dnistab.it->second.dnis_name,
                     soap->m_db_data_ptr->dnistab.it->second.dnis_number,
                     is_email, //PVL 13-06-2010 1004079
                     ReturnArrayData);
  }

  soap->m_db_data_ptr->dnistab.csa_web_UnLock();//MT
  return  ReturnArrayData.responseInfoHeader->errorCause ;//PVL_STAT
}


//------------------------------------------------------------------------------------------------
void DNISRecordReply::InsertDNISList(CsaWebServer*  soap,
                                     unsigned int   dnis_Id,
                                     std::string    dnis_Name,
                                     std::string    dnis_Number,
                                     bool           is_email,//PVL 13-06-2010 1004079
                                     ns1__DNISListReturnType &ReturnArrayData)
{
  ns1__DNISListDataItemType* tmp;
  tmp = soap_new_ns1__DNISListDataItemType(soap, 1);

  tmp->dnisId = dnis_Id;
  tmp->dnisName = dnis_Name;
  tmp->dnisNumber = dnis_Number;
  tmp->dnisType = is_email ;//PVL 13-06-2010 1004079
  //soap->m_net_data_ptr->DNISL_vector.push_back(tmp);
  ReturnArrayData.returnArray.push_back(tmp);
}


//------------------------------------------------------------------------------------------------
void DNISRecordReply::CreateNewRow(CsaWebServer *soap)
{
  CurrentRow = soap_new_ns1__DataItemRow(soap, 1);
}


//------------------------------------------------------------------------------------------------
void DNISRecordReply::InsertFullDataBG(CsaWebServer*  soap,
                                       unsigned int*  bgcolor_ptr,
                                       std::string    ersid,
                                       std::string    ersname,
                                       std::string    value)
{
  ns1__DataItemType* tmp;

  tmp = soap_new_ns1__DataItemType(soap, 1);
  tmp->ersid = ersid;
  tmp->ersname =ersname;
  tmp->value = value;
  if(bgcolor_ptr!=NULL){
    tmp->bgcolor = bgcolor_ptr;
  }


  CurrentRow->returnArray.push_back(tmp);
  //soap->m_net_data_ptr->DIT_vector.push_back(tmp);
}


//------------------------------------------------------------------------------------------------
void DNISRecordReply::InsertRowToMatrix(CsaWebServer *soap,ns1__ReturnMatrixDataTypeBG& matrix)
{
  matrix.returnMatrix.push_back(CurrentRow);
  //soap->m_net_data_ptr->ROW_vector.push_back(CurrentRow);
}
//--------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------
void DNISRecordReply::InsertBriefVoiceDNISDataToRow(CsaWebServer *  soap,
                                                    CsaDNIS_Record* drec)
{
  int acd_type_acd_offset = getOffSetOfAcdCallTypes(CL_CALL_TYPE_ACD);

  GenSti_t  q_sti;
  GenSti_t  chat_queue_sti;
  CsaTime_t t1, t2;
  CsaRc_t   rc = ARC_OK;
  Ulong_t   voice_longest_que_time = 0;// PVL 6-07-2010  1004286
  Ulong_t   queued_calls = 0;
  Ulong_t   waiting_above_target_asa = 0;
  Ulong_t   avg_time_in_queue = 0;
  Ulong_t   longest_queue_time = 0;
  Ulong_t   chat_queue_queued_calls = 0;
  Ulong_t   chat_queue_waiting_above_target_asa = 0;
  Ulong_t   chat_queue_avg_time_in_queue = 0;
  Ulong_t   chat_queue_longest_queue_time = 0;
  Ulong_t   chat_longest_que_time = 0;
  Ulong_t   chat_accepted = 0;
  Ulong_t   accepted_calls = 0;
  Ulong_t   chat_answered = 0;
  Ulong_t   answered_calls = 0;
  Ulong_t   chat_abandoned = 0;
  Ulong_t   abandoned_calls = 0;
  Ulong_t   abandoned_after_target_asa = 0;
  Ulong_t   calls_answered_before_target_asa = 0;
  Ulong_t   calculated_tsf = 0;
  Ulong_t   chat_queued_current_interval = 0;
  Ulong_t   queued_current_interval = 0;
  Ulong_t   avg_chat_voice = 0;
  Ulong_t   avg_wait_que_on_cur_int = 0;
  Ulong_t   avg_chat_wait_que_on_cur_int = 0;
  Ulong_t   avg_chat_voice_abnd = 0;
  Ulong_t   chat_avg_wait_of_abnd = 0;
  Ulong_t   avg_wait_of_abnd = 0;
  Ulong_t   chat_longest_wait_of_abnd = 0;
  Ulong_t   longest_wait_of_abnd = 0;
  Ulong_t   chat_abandoned_after_tasa = 0; //1004286
  Ulong_t   chat_answered_before_tasa = 0; //1004286

  // fields from calls now in queue
  //VOICE
  voice_longest_que_time = drec->Acd[acd_type_acd_offset].longest_queue_time.get();// PVL 6-07-2010  1004286
  if (drec->Queued_calls[acd_type_acd_offset] != NULL)
  {
    queued_calls = drec->Queued_calls[acd_type_acd_offset]->getNum();
    rc = drec->Queued_calls[acd_type_acd_offset]->getStatistics(t1, t2, q_sti, waiting_above_target_asa);
    if (rc == ARC_OK)
    {
      avg_time_in_queue  = t1;
      longest_queue_time = t2;
    }
  }

  //CHAT
  if ((rc == ARC_OK) && Csa_Setup.supportChat())
  {
    int acd_type_chat_offset = getOffSetOfAcdCallTypes(CL_CALL_TYPE_CHAT);
    chat_longest_que_time = drec->Acd[acd_type_chat_offset].longest_queue_time.get();
    chat_accepted = drec->Acd[acd_type_chat_offset].accepted_calls.get();
    chat_answered = drec->Acd[acd_type_chat_offset].answered_per_time_slot.get();
    chat_abandoned = drec->Acd[acd_type_chat_offset].answered_per_time_slot.get();
    chat_queued_current_interval = drec->Acd[acd_type_chat_offset].queued_calls.get();
    chat_avg_wait_of_abnd = drec->Acd[acd_type_chat_offset].total_wait_time_of_abandoned_calls.get();
    chat_longest_wait_of_abnd = drec->Acd[acd_type_chat_offset].longest_wait_time_of_abandoned_calls.get();
    if (chat_queued_current_interval > 0 )
       avg_chat_wait_que_on_cur_int = (unsigned long )( drec->Acd[acd_type_chat_offset].total_queue_time.get() /
                                       chat_queued_current_interval);
    chat_abandoned_after_tasa = drec->Acd[acd_type_chat_offset].abandoned_after_target_asa.get(); //1004286
    chat_answered_before_tasa = drec->Acd[acd_type_chat_offset].calls_answered_before_target_asa.get(); //1004286

    // fields from calls now in queue
    if (drec->Queued_calls[acd_type_chat_offset] != NULL)
    {
      chat_queue_queued_calls = drec->Queued_calls[acd_type_chat_offset]->getNum();
      rc = drec->Queued_calls[acd_type_chat_offset]->getStatistics(t1, t2, chat_queue_sti, chat_queue_waiting_above_target_asa);
      if (rc == ARC_OK)
      {
        chat_queue_avg_time_in_queue = t1;
        chat_queue_longest_queue_time = t2;
      }
    }
  }
  InsertFullDataBG(soap,NULL,"f6_3_10_1_0","DNIS Id",Ulong2String(drec->Id));
  //f6_3_10_1_1,1,"DNIS","Name",0,95,l,0,l
  InsertFullDataBG(soap,NULL,"f6_3_10_1_1","DNIS Name",soap->m_db_data_ptr->dnistab.GetName(drec->Id));
  //f6_3_10_1_2,0,"DNIS","No.",0,57,c,0,c
  InsertFullDataBG(soap,NULL,"f6_3_10_1_2","DNIS No.",soap->m_db_data_ptr->dnistab.GetNumber(drec->Id));
  //f20_3_10_2_2,2,"Above","T.ASA",0,50,c,0,c
  InsertFullDataBG(soap,NULL,"f20_3_10_2_2","Above T.ASA",Ulong2String(waiting_above_target_asa + chat_queue_waiting_above_target_asa));
  //f20_3_10_2_1,1,"Calls","In Q",0,40,c,0,c
  InsertFullDataBG(soap,NULL,"f20_3_10_2_1","Calls In Q",Ulong2String(chat_queue_queued_calls + queued_calls));
  //f20_3_10_2_4,2,"Max","In Q",0,40,c,0,c

  if( (chat_queue_longest_queue_time >0 && longest_queue_time == 0) ||
      ((longest_queue_time > 0) && (chat_queue_longest_queue_time > 0) && (chat_queue_longest_queue_time < longest_queue_time)) )
  {
    InsertFullDataBG(soap,NULL,"f20_3_10_2_4","Max Time In Q",Ulong2String(NlaCalcCsaElapsedTime(chat_queue_longest_queue_time)));// PVL 6-07-2010  1004286
  }
  else
    InsertFullDataBG(soap,NULL,"f20_3_10_2_4","Max Time In Q", Ulong2String(NlaCalcCsaElapsedTime(longest_queue_time)));

  //f20_3_10_4_1,1,"Calls","ACD",0,40,c,0,c
  accepted_calls = drec->Acd[acd_type_acd_offset].accepted_calls.get();
  InsertFullDataBG(soap,NULL,"f20_3_10_4_1","Calls ACD",Ulong2String(accepted_calls +chat_accepted ));

  //f20_3_10_4_2,1,"Calls","Ansd",0,40,0,c
  answered_calls = drec->Acd[acd_type_acd_offset].answered_per_time_slot.get();
  calls_answered_before_target_asa = drec-> Acd[acd_type_acd_offset].calls_answered_before_target_asa.get();
  InsertFullDataBG(soap,NULL,"f20_3_10_4_2","Calls Ansd",Ulong2String(answered_calls+ chat_answered));
  //f20_3_10_4_3,1,"Calls","Abnd",0,40,c,0,c
  abandoned_calls = drec->Acd[acd_type_acd_offset].abandoned_calls.get() +
                    drec->Acd[acd_type_acd_offset].abandoned_while_mandatory.get();
  abandoned_after_target_asa = drec->Acd[acd_type_acd_offset].abandoned_after_target_asa.get();
  InsertFullDataBG(soap,NULL,"f20_3_10_4_3","Calls Abnd",Ulong2String(abandoned_calls+chat_abandoned));

  //f6_3_10_4_1,1,"TSF"," ",0,40,c,0,c
  int tsf_formula = Csa_Setup.getTsfFormula();
  int epic_pbx_type = 0;
  unsigned long tsf_denominator = 0;
  unsigned long tsf_numerator = 0;
  unsigned long tsf = 0;
  Csa_Setup.get(EPIC_PBX_TYPE, &epic_pbx_type);

  switch(tsf_formula)
  {
    case 1: // Answered
      tsf_denominator = answered_calls + chat_answered;  //1004286
    break;
    case 2: //
      tsf_denominator = answered_calls + chat_answered + abandoned_after_target_asa + chat_abandoned_after_tasa;  //1004286
    break;
    case 3: // sync up with DNIS
      if (epic_pbx_type == EPIC_PBX_TYPE_SHORETEL)
        tsf_denominator = accepted_calls - abandoned_calls + abandoned_after_target_asa;
      else
        tsf_denominator = accepted_calls + chat_accepted; //1004286
    break;
    default:
      tsf_denominator = accepted_calls + chat_accepted; //1004286
    break;
  }

  if( tsf_denominator > 0 )
  {
    tsf_numerator = (calls_answered_before_target_asa + chat_answered_before_tasa)* 100; //1004286
    tsf = (unsigned long )(( tsf_numerator/tsf_denominator) + 0.5);

    if( tsf > 100 )
      calculated_tsf = 100;
    else
      calculated_tsf = tsf;
  }
  else
  {
    //  if calls abandoned or a default state
    calculated_tsf = 100;
  }

  //soap->m_db_data_ptr->limitstab.GetColorOfERS( grec->Grp_id,soap->m_db_data_ptr->limitstab.ersid_val.f6_3_10_4_1,
  //                                              tsf,&TmpColorPtr);
  //InsertFullDataBG(soap,TmpColorPtr,"f6_3_10_4_1","TSF",Ulong2String(tsf));
  InsertFullDataBG(soap,NULL,"f6_3_10_4_1","TSF",Ulong2String(tsf));

  //f20_3_10_4_4,2,"Avg. Wait","Abnd",0,70,c,0,c
  if( abandoned_calls>0 )
    avg_wait_of_abnd = (unsigned long )( drec->Acd[acd_type_acd_offset].total_wait_time_of_abandoned_calls.get() / abandoned_calls);

  if( (abandoned_calls + chat_abandoned) > 0 )
    avg_chat_voice_abnd = (unsigned long )((avg_wait_of_abnd + // PVL 6-07-2010  1004286
                           chat_avg_wait_of_abnd ) /           // PVL 6-07-2010  1004286
                          (abandoned_calls+chat_abandoned));   // PVL 6-07-2010  1004286
  else
    avg_chat_voice_abnd = 0;
  InsertFullDataBG(soap,NULL,"f20_3_10_4_4","Avg. Wait Abnd",Ulong2String(avg_chat_voice_abnd));

  //f20_3_10_4_5,2,"Max Wait","Abnd",0,70,c,0,c
  longest_wait_of_abnd = drec->Acd[acd_type_acd_offset].longest_wait_time_of_abandoned_calls.get();
  if(chat_longest_wait_of_abnd > longest_wait_of_abnd)
    InsertFullDataBG(soap,NULL,"f20_3_10_4_5","Max Wait Abnd",Ulong2String(chat_longest_wait_of_abnd));
  else
    InsertFullDataBG(soap,NULL,"f20_3_10_4_5","Max Wait Abnd",Ulong2String(longest_wait_of_abnd));

  //f20_3_10_4_6,1,"Avg. Q","Time",0,60,c,0,c
  queued_current_interval = drec->Acd[acd_type_acd_offset].queued_calls.get();
  if( queued_current_interval > 0 )
    avg_wait_que_on_cur_int = (unsigned long )(drec->Acd[acd_type_acd_offset].total_queue_time.get() /
                               queued_current_interval);
  //data_list.InsertTime (f6_3_10_4_7, avg_wait_que_on_cur_int);
  if( (queued_current_interval + chat_queued_current_interval) > 0 )
    avg_chat_voice = (unsigned long )(((avg_wait_que_on_cur_int * queued_current_interval) +
                     (avg_chat_wait_que_on_cur_int * chat_queued_current_interval)) /
                     (queued_current_interval+chat_queued_current_interval));
  else
    avg_chat_voice = 0;

  InsertFullDataBG(soap,NULL,"f20_3_10_4_6","Avg. Q Time",Ulong2String(avg_chat_voice));

  //f20_3_10_4_7,2,"Max Q","Time",0,60,c,0,c

  if(voice_longest_que_time > chat_longest_que_time)
    InsertFullDataBG(soap,NULL,"f20_3_10_4_7","Max Q Time",Ulong2String(voice_longest_que_time));    // PVL 6-07-2010  1004286
  else
    InsertFullDataBG(soap,NULL,"f20_3_10_4_7","Max Q Time",Ulong2String(chat_longest_que_time)); // PVL 6-07-2010  1004286

}//InsertBriefVoiceDNISDataToRow


//----------------------------------------------------------------------
void DNISRecordReply::InsertBriefEmailDNISDataToRow(CsaWebServer*   soap,
                                                    CsaDNIS_Record* drec)
{
  Ulong_t  queued_calls = 0;
  Ulong_t  total_queued_calls = 0; //PVL 7-06-2010 1004158
  Ulong_t  waiting_above_target_asa = 0;
  Ulong_t  avg_time_in_queue = 0;
  Ulong_t  longest_queue_time = 0;
  Ulong_t  longest_queue_time_total = 0;//PVL 7-06-2010 1004158
  Ulong_t  accepted = 0;
  Ulong_t  answered = 0;
  Ulong_t  total_wait_time_of_answered = 0;
  Ulong_t  total_queue_time = 0;
  Ulong_t  total_interaction_time = 0;
  Ulong_t  longest_interaction_time = 0;
  Ulong_t  avg_mail_wait_que_on_cur_int = 0;
  Ulong_t  calls_answered_before_target_asa = 0;
  Ulong_t  calculated_tsf = 0;

  GenSti_t  email_queue_sti;
  CsaTime_t t1, t2;
  CsaRc_t   rc = ARC_OK;

  if ((rc == ARC_OK) && Csa_Setup.supportEmail())
  {
    int acd_type_email_offset = getOffSetOfAcdCallTypes(CL_CALL_TYPE_EMAIL);

    // fields from calls now in queue
    if (drec->Queued_calls[acd_type_email_offset] != NULL)
    {
      queued_calls = drec->Queued_calls[acd_type_email_offset]->getNum();
      rc = drec->Queued_calls[acd_type_email_offset]->getStatistics(t1, t2, email_queue_sti, waiting_above_target_asa);
      if (rc == ARC_OK)
      {
        avg_time_in_queue  = t1;
        longest_queue_time = t2;
      }
    }

    accepted                        = drec->Acd[acd_type_email_offset].accepted_calls.get();
    answered                        = drec->Acd[acd_type_email_offset].answered_per_time_slot.get();
    total_wait_time_of_answered     = drec->Acd[acd_type_email_offset].total_wait_till_answer_time.get();
    total_wait_time_of_answered     = drec->Acd[acd_type_email_offset].longest_wait_till_answer_time.get();
    total_queued_calls              = drec->Acd[acd_type_email_offset].queued_calls.get();//PVL 7-06-2010 1004158
    total_queue_time                = drec->Acd[acd_type_email_offset].total_queue_time.get();
    longest_queue_time_total        = drec->Acd[acd_type_email_offset].longest_queue_time.get();//PVL 7-06-2010 1004158
    total_interaction_time          = drec->Acd[acd_type_email_offset].total_talk_time.get();
    longest_interaction_time        = drec->Acd[acd_type_email_offset].longest_talk_time.get();
    calls_answered_before_target_asa = drec-> Acd[acd_type_email_offset].calls_answered_before_target_asa.get();
  }

  InsertFullDataBG(soap,NULL,"f6_3_10_1_0","DNIS Id",Ulong2String(drec->Id));
  //f6_3_10_1_1,1,"DNIS","Name",0,95,l,0,l
  InsertFullDataBG(soap,NULL,"f6_3_10_1_1","DNIS Name",soap->m_db_data_ptr->dnistab.GetName(drec->Id));
  //f6_3_10_1_2,0,"DNIS","No.",0,57,c,0,c
  InsertFullDataBG(soap,NULL,"f6_3_10_1_2","DNIS No.",soap->m_db_data_ptr->dnistab.GetNumber(drec->Id));

  //f6_3_10_2_12,2,"Above","T.ASA",0,50,c,0,c
  InsertFullDataBG(soap,NULL,"f6_3_10_2_12","Above T.ASA",Ulong2String(waiting_above_target_asa));

  //f6_3_10_2_11,1,"Calls","In Q",0,40,c,0,c
  InsertFullDataBG(soap,NULL,"f6_3_10_2_11","Calls In Q",Ulong2String(queued_calls));

  //f6_3_10_4_26,1,"Calls","ACD",0,40,c,0,c
  InsertFullDataBG(soap,NULL,"f6_3_10_4_26","Calls ACD",Ulong2String(accepted));

  //f6_3_10_4_27,1,"Calls","Ansd",0,40,0,c
  InsertFullDataBG(soap,NULL,"f6_3_10_4_27","Calls Ansd",Ulong2String(answered));

  //f6_3_10_4_28,1,"Avg. Q","Time",0,60,c,0,c
  if (queued_calls > 0 )
    avg_mail_wait_que_on_cur_int = (unsigned long )( total_queue_time / total_queued_calls); //PVL 7-06-2010 1004158
  else
    avg_mail_wait_que_on_cur_int =0;
  InsertFullDataBG(soap,NULL,"f6_3_10_4_28","Avg. Q Time",Ulong2String(avg_mail_wait_que_on_cur_int));

  //f6_3_10_4_29,2,"Max Q","Time",0,60,c,0,c
  InsertFullDataBG(soap,NULL,"f6_3_10_4_29","Max Q Time",Ulong2String(longest_queue_time_total)); //PVL 7-06-2010 1004158

  //f6_3_10_4_1,1,"TSF"," ",0,40,c,0,c
  int tsf_formula = Csa_Setup.getTsfFormula();
  int epic_pbx_type = 0;
  unsigned long tsf_denominator = 0;
  unsigned long tsf_numerator = 0;
  unsigned long tsf = 0;
  Csa_Setup.get(EPIC_PBX_TYPE, &epic_pbx_type);

  switch(tsf_formula)
  {
    case 1: // Answered
      tsf_denominator = answered;
    break;
    case 2: //
      tsf_denominator = answered;
    break;
    case 3: // sync up with DNIS
      tsf_denominator = accepted;
    break;
    default:
      tsf_denominator = accepted;
    break;
  }

  if( tsf_denominator > 0 )
  {
    tsf_numerator = calls_answered_before_target_asa * 100; //1004286
    tsf = (unsigned long )(( tsf_numerator/tsf_denominator) + 0.5);

    if( tsf > 100 )
      calculated_tsf = 100;
    else
      calculated_tsf = tsf;
  }
  else
  {
    //  if calls abandoned or a default state
    calculated_tsf = 100;
  }

  //f6_3_10_4_29,2,"TSF","Time",0,60,c,0,c
  InsertFullDataBG(soap,NULL,"f6_3_10_4_1","TSF",Ulong2String(tsf));

}//InsertBriefEmailDNISDataToRow
