#ifndef CSA_WEB_DNIS_REPLY_H
#define CSA_WEB_DNIS_REPLY_H

#include "csa_web_serverMT.hpp"
#include "ardnis.hpp"

#define EMAIL_DNIS 1
#define NOT_EMAIL_DNIS 0
class DNISRecordReply { 

//============================ Public Section ========================
public:

  // ctor
  DNISRecordReply(){ };
  //dstr
  ~DNISRecordReply(){};

  unsigned int GetHandleDNISList(CsaWebServer *soap,   //PVL_STAT
                                 ns1__GenRequestType *RequestData,
                                 ns1__DNISListReturnType &ReturnArrayData,
                                 bool is_email);

  void InsertDNISList(CsaWebServer *soap, 
                      unsigned int   dnis_Id,
                      std::string    dnis_Name,
                      std::string    dnis_Number,
                      bool           is_email,//PVL 13-06-2010 1004079
                      ns1__DNISListReturnType &ReturnArrayData);


  void CreateNewRow(CsaWebServer *soap);
  void InsertFullDataBG(CsaWebServer *soap,unsigned int * bgcolor, std::string ersid,std::string ersname,std::string value);
  void InsertRowToMatrix(CsaWebServer *soap,ns1__ReturnMatrixDataTypeBG& matrix);

  void InsertBriefVoiceDNISDataToRow(CsaWebServer*    soap,
                                     CsaDNIS_Record*  drec);
  void InsertBriefEmailDNISDataToRow(CsaWebServer*    soap,
                                     CsaDNIS_Record*  drec );
  bool IsDNIS(CsaWebServer*   soap,
              CsaDNIS_Record* drec,
              unsigned int    Is_email_enable)
  {
    return soap->m_db_data_ptr->dnistab.isDNISbyID(drec->Id,Is_email_enable);
  }

private:
  ns1__DataItemRow *CurrentRow;
};



#endif
