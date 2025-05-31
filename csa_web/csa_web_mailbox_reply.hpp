#ifndef CSA_WEB_MAILBOX_REPLY_H
#define CSA_WEB_MAILBOX_REPLY_H

#include "csa_web_serverMT.hpp"
#include "csa_res_email.h"
#include "csa_web_data_interface.hpp"
class MAILbox_Reply : public DataItemType_inter{ //: public EMISwebBindingService{

//============================ Public Section ========================
public:

    // ctor
	MAILbox_Reply(){ };
    //dstr
    ~MAILbox_Reply(){};
    void InsertMAILboxAccList(CsaWebServer *soap, Id_type Id,std::string Name,ns1__GenListReturnType &ReturnArrayData);
    CsaRc_t StartMonitor         (unsigned int mailbox_id);
    CsaRc_t MakeMailboxInfo    (CsaWebServer *soap,MailboxRecord *mailbox_rec, ns1__BlockItemType *ReturnArrayData );
    CsaRc_t MAILbox_Reply::MakeMailboxInfoEMPTY(CsaWebServer *soap,unsigned int MailAccId, ns1__BlockItemType *ReturnArrayData );//PVL 14-06-2010 1004180
    const static int inBound = 0;
    const static int outBound =1;
};
#endif
