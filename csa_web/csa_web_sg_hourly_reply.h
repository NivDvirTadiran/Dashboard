#ifndef CSA_WEB_SG_HOURLY_REPLY_H
#define CSA_WEB_SG_HOURLY_REPLY_H
//#include "csa_web_brief_agent_inf.hpp"
#include "csa_web_serverMT.hpp"

//#define DB_NUMBER_FORMAT "Number"
//#define DB_TIME_FORMAT   "Char(10)"
//#define FIRST_COLUMN_FORMAT "Char(6)"
//
//#define NLA_NUMBER_FORMAT 'l'
//#define NLA_TIME_FORMAT   't'
//#define NLA_STRING_FORMAT 's'

class SuperGroupHourlyReply { 

//============================ Public Section ========================
public:

  // ctor
  SuperGroupHourlyReply(){ };
  //dstr
  ~SuperGroupHourlyReply(){};

  void AddNewSuperGroupBlock(CsaWebServer *soap,
                             unsigned int sgroupId,
                             ns1__ReturnMatrixDataTypePerSuperGroup &ReturnData);
  /*
  void InsertAgentsList(CsaWebServer *soap,
                        unsigned int   AgentId,
                        std::string    AgentName,
                        std::string    AgentNumber,					 
                        ns1__AgentsListReturnType *ReturnArrayData);
  */
  void CreateNewRow(CsaWebServer *soap);
  void InsertFullDataBG(CsaWebServer *soap,unsigned int * bgcolor, std::string ersid,std::string ersname,std::string value);
  void InsertRowToMatrix(CsaWebServer *soap,ns1__returnDataRowsPerSuperGroup* matrix);
	
  void InsertDataToRow(CsaWebServer *soap,unsigned int sgroupId);
  RetCode_t InsertDataToRows(CsaWebServer *                           soap,
                             unsigned int                             sgroupId,
                             ns1__ReturnMatrixDataTypePerSuperGroup&  ReturnData,
                             unsigned int                             blockInd);

  //RetCode_t CreateSqlSelects(CsaWebServer *soap, unsigned int sgroupId);
  unsigned char GetNlaFieldFormat( char *arg_sql_format);
  void          SetDateTodayAndTimeNow(EosOpCode_t op_code,           //input
                                       Uint_t      nla_int,           //input
                                       Ulong_t     server_time,       //input
                                       char        *date_today,       //output
                                       Uint_t      *time_now_rounded /*output*/,
                                       Uint_t      *time_from);
   
private:
  ns1__DataItemRow *CurrentRow;
};



#endif
