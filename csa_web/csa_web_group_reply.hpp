#ifndef CSA_WEB_GROUP_REPLY_H
#define CSA_WEB_GROUP_REPLY_H

#include "csa_web_serverMT.hpp"
#include "csa_web_data_interface.hpp"
#include "argroup.hpp"

class GroupRecordReply : public DataItemType_inter
{ 
//============================ Public Section ========================
  public:

    // ctor
    GroupRecordReply()
    {
      supportedOutbound = Csa_Setup.supportOutbound();//PVL 2-06-2010 1004133 
      supportedChat =     Csa_Setup.supportChat();//PVL 2-06-2010 1004133 
      supportedEmail =    Csa_Setup.supportEmail();//PVL 2-06-2010 1004133 
      supportedDialList = Csa_Setup.supportDialList();//PVL 2-06-2010 1004133 
      acd_type_acd_offset = getOffSetOfAcdCallTypes(CL_CALL_TYPE_ACD);
      if(supportedEmail)//PVL 2-06-2010 1004133 
        acd_type_email_offset = getOffSetOfAcdCallTypes(CL_CALL_TYPE_EMAIL);
      if(supportedChat)//PVL 2-06-2010 1004133 
        acd_type_chat_offset = getOffSetOfAcdCallTypes(CL_CALL_TYPE_CHAT);
      if(supportedOutbound)//PVL 2-06-2010 1004133 
      {
        acd_type_callback_offset = getOffSetOfAcdCallTypes(CL_CALL_TYPE_CALLBACK);
        acd_type_abandoned_offset = getOffSetOfAcdCallTypes(CL_CALL_TYPE_ABANDONED);
      }//PVL 2-06-2010 1004133 
      acd_type_dummy_offset = getOffSetOfAcdCallTypes(CL_CALL_TYPE_DUMMY);
      if(supportedDialList)//PVL 2-06-2010 1004133 
        acd_type_dial_list_offset = getOffSetOfAcdCallTypes(CL_CALL_TYPE_DIAL_LIST);
    };

    //dstr
    ~GroupRecordReply(){};


    void InsertGroupList(CsaWebServer *soap,
                         unsigned int   GrpId,
                         std::string    GrpName,
                         std::string    GrpNumber,
                         unsigned int   GrpEmailEnable,
                         ns1__GroupListReturnType &ReturnArrayData);

    CsaRc_t     MakeEventDataWeb_QueInfo( CsaOnlineGroupStatistics_t* ed, GroupRecord* grec );
    CsaRc_t     MakePerGroup_QueInfo    (CsaWebServer *soap,GroupRecord*  grec, ns1__BlockItemType* ReturnArrayData );
    CsaRc_t     MakePerGroup_ShiftInfo  (CsaWebServer *soap,GroupRecord*  grec, ns1__BlockItemType* ReturnArrayData );
    CsaRc_t     MakePerGroup_AbandInfo  (CsaWebServer *soap,GroupRecord*  grec, ns1__BlockItemType* ReturnArrayData );
    CsaRc_t     MakePerGroup_InterflInfo(CsaWebServer *soap,GroupRecord*  grec, ns1__BlockItemType* ReturnArrayData );
    CsaRc_t     MakePerGroup_AgentsInfo (CsaWebServer *soap,GroupRecord*  grec, ns1__BlockItemType* ReturnArrayData );

    int acd_type_acd_offset ;      
    int acd_type_email_offset;  
    int acd_type_chat_offset;     
    int acd_type_callback_offset;
    int acd_type_abandoned_offset;
    int acd_type_dummy_offset;
    int acd_type_dial_list_offset;

    bool supportedOutbound ;//PVL 2-06-2010 1004133 
    bool supportedChat;//PVL 2-06-2010 1004133 
    bool supportedEmail;//PVL 2-06-2010 1004133 
    bool supportedDialList;//PVL 2-06-2010 1004133 
    Ulong_t  myOldestTime(Ulong_t a,Ulong_t b);
};
#endif
