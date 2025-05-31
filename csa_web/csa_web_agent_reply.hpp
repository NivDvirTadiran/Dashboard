#ifndef CSA_WEB_AGENT_REPLY_H
#define CSA_WEB_AGENT_REPLY_H
#include "csa_web_brief_agent_inf.hpp"
#include "csa_web_serverMT.hpp"


class AgentRecordReply { 

//============================ Public Section ========================
public:

  // ctor
	AgentRecordReply();
  //dstr
	~AgentRecordReply();
	

    void AddNewGroupBlock(CsaWebServer *soap,
										unsigned int groupId,
										ns1__ReturnMatrixDataTypePerGroup &ReturnData);
	void InsertAgentsList(CsaWebServer *soap,
						unsigned int   AgentId,
						std::string    AgentName,
						std::string    AgentNumber,					 
						ns1__AgentsListReturnType &ReturnArrayData);



	void CreateNewRow(CsaWebServer *soap);
	void InsertFullDataBG(CsaWebServer *soap,unsigned int * bgcolor, std::string ersid,std::string ersname,std::string value);
	
	void InsertRowToMatrix(CsaWebServer *soap,ns1__returnDataRowsPerGroup* matrix);
	void InsertRowToMatrix(CsaWebServer *soap,ns1__ReturnMatrixDataTypeBG& matrix);

	void InsertAgentDataToRow(CsaWebServer *soap,
							//  GroupRecordReply &ReplyObj,
							AgentRecord*          agent,
							CsaAgentPerGroupRecord_t*  agent_per_group,
							unsigned int groupId);
   void InsertAgentHELPDataToRow(CsaWebServer *soap,     //PVL 15-06-2010 1004086					
							AgentRecord*          agent,
							CsaAgentPerGroupRecord_t*  agent_per_group);                    

	void InsertBriefAgentDataToRow(CsaWebServer *soap,
							// AgentRecordReply &ReplyObj,
							AgentRecord*          agent,
							BriefAgentInfo &totals);
	void InsertBrief_LogOut_AgentDataToRow (CsaWebServer *soap,Id_type a_id);
private:
	ns1__DataItemRow *CurrentRow;
};



#endif
