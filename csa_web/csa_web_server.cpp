#include "csa_web_server.hpp"
#include "csa_web_commondefs.hpp"
#include "csa_web_group_reply.hpp"
#include "csa_web_agent_reply.hpp"
#include "csa_web_report.h"
#include "csa_web_dnis_reply.hpp"
#include "csa_web_ivr_reply.hpp"
#include "csa_web_mailbox_reply.hpp"
#include "csa_web_ticket.hpp"

void CsaWebServer::CleanAll(void)
{

 if(ResponceHeader != NULL)
    soap_delete_ns1__ResponceInfoHeaderType(this,ResponceHeader);
 //-------
 
 if(BLOCK_vector.size() > 0){
   for(BLOCK_iter=BLOCK_vector.begin(); BLOCK_iter!= BLOCK_vector.end(); ++BLOCK_iter) 
	  soap_delete_ns1__BlockItemType(this,*BLOCK_iter);
   BLOCK_vector.clear();
 }
 //-------
  if(MATRIX_PER_GROUP_vector.size() > 0){
   for(MATRIX_PER_GROUP_iter=MATRIX_PER_GROUP_vector.begin(); MATRIX_PER_GROUP_iter!= MATRIX_PER_GROUP_vector.end(); ++MATRIX_PER_GROUP_iter) 
	  soap_delete_ns1__returnAgentMatrixType(this,*MATRIX_PER_GROUP_iter);
   MATRIX_PER_GROUP_vector.clear();
 }
 //-------
 if(DIT_vector.size() > 0){
   for(DIT_iter=DIT_vector.begin(); DIT_iter!= DIT_vector.end(); ++DIT_iter) 
	  soap_delete_ns1__DataItemType(this,*DIT_iter);
   DIT_vector.clear();
 }
 //-------
 if(ROW_vector.size() > 0){
   for(ROW_iter=ROW_vector.begin(); ROW_iter!= ROW_vector.end(); ++ROW_iter) 
	  soap_delete_ns1__DataItemRow(this,*ROW_iter);
   ROW_vector.clear();
 }

 //-------
  if(GroupListType_vector.size() > 0){
   for(GroupListType_iter=GroupListType_vector.begin(); GroupListType_iter!= GroupListType_vector.end(); ++GroupListType_iter) 
	  soap_delete_ns1__GroupListDataItemType(this,*GroupListType_iter);
   GroupListType_vector.clear();
 }
 //-------
if(AgentsListType_vector.size() > 0){
   for(AgentsListType_iter=NetData->AgentsListType_vector.begin(); AgentsListType_iter!= AgentsListType_vector.end(); ++AgentsListType_iter) 
	   soap_delete_ns1__AgentsListDataItemType(this,*AgentsListType_iter );
   AgentsListType_vector.clear();
 }

 //-------
  if(DW_vector.size() > 0){
   for(DW_iter=DW_vector.begin(); DW_iter!= DW_vector.end(); ++DW_iter) 
	  soap_delete_ns1__DefaultWorkspaceItemType(this,*DW_iter);
   DW_vector.clear();
 }
 //-------
if(RLT_map.size() > 0){
   for(RLT_iter=RLT_map.begin(); RLT_iter!= RLT_map.end(); ++RLT_iter) 
	   soap_delete_ns1__ReportListDataItemType(this,RLT_iter->second );
   RLT_map.clear();
 }
 //-------
  if(DNISL_vector.size() > 0){
   for(DNISL_iter=DNISL_vector.begin(); DNISL_iter!= DNISL_vector.end(); ++DNISL_iter) 
	  soap_delete_ns1__DNISListDataItemType(this,*DNISL_iter);
   DNISL_vector.clear();
 }
 //-------
  if(GENL_vector.size() > 0){
   for(GENL_iter=GENL_vector.begin(); GENL_iter!= GENL_vector.end(); ++GENL_iter) 
	  soap_delete_ns1__GenListDataItemType(this,*GENL_iter);
   GENL_vector.clear();
 }

}
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
void CsaWebServer::InsertGenListDataItemType(
		                unsigned int   Id,
						std::string    Name,
						ns1__GenListReturnType *ReturnArrayData)
{

ns1__GenListDataItemType* tmp;
tmp =  soap_new_ns1__GenListDataItemType(this, 1);

tmp->Id = Id; 
tmp->Name = Name; 

this->GENL_vector.push_back(tmp);
ReturnArrayData->returnArray.push_back(tmp); 
}
//------------------------------------------------------------------------------------------------
int  CsaWebServer::GroupDetailFullInfo(ns1__GenInfoRequestType *ns1__RequestGroupDetailFullInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupDetailFullInfo)
{
unsigned int  *TmpColorPtr = NULL;
GroupRecordReply ReplyObj;
 CsaRc_t                                        rc = ARC_OK;
 GroupRecord*                                   grec;
//Fool protector
 if(ns1__RequestGroupDetailFullInfo == NULL)
    return soap_receiver_fault(this, "No Request Data", "ns1__RequestGroupDetailFullInfo is NULL");

 //header
ns1__ReturnGroupDetailFullInfo->responceInfoHeader = ResponceHeader;

ns1__ReturnGroupDetailFullInfo->responceInfoHeader->errorCause = (unsigned int)0;
ns1__ReturnGroupDetailFullInfo->responceInfoHeader->serviceReqId = 
ns1__RequestGroupDetailFullInfo->requestInfoHeader->serviceReqId;

if(!TicketTab.VerifyTicket(ns1__RequestGroupDetailFullInfo->requestInfoHeader->ticket))
	{
		ns1__ReturnGroupDetailFullInfo->responceInfoHeader->errorCause = 500;
		return SOAP_OK;
	}

for (unsigned int i=0; i<ns1__RequestGroupDetailFullInfo->Id.size(); i++)
	{
		limitstab.VerifyGroupInLimitTab(ns1__RequestGroupDetailFullInfo->Id[i]);//PVL 28-07-2009 1002567
		rc = Csa_Grps_Tbls[TBL_ONLINE]->get(ns1__RequestGroupDetailFullInfo->Id[i], &grec);


//array
ReplyObj.AddNewGroupBlock(this,grec->Grp_id,ns1__ReturnGroupDetailFullInfo);

 //      //{ERS : 6.3.1.3.10 - primary_agents, secondary }
ReplyObj.InsertFullData(this,"f6_3_1_3_10","primary_agents",grec->Current_agents_state.primary_agents.get(),ns1__ReturnGroupDetailFullInfo->returnArrayOfBlocks[i]);

//ERS:  f6_3_1_3_1 - num_of_agents
limitstab.GetColorOfERS( grec->Grp_id,limitstab.ersid_val.f6_3_1_3_1,
				        grec->Current_agents_state.number_of_agents.get(),&TmpColorPtr);
ReplyObj.InsertFullDataBG(this,TmpColorPtr,"f6_3_1_3_1","Logged in",grec->Current_agents_state.number_of_agents.get(),ns1__ReturnGroupDetailFullInfo->returnArrayOfBlocks[i]);

//    //ERS : 6.3.1.3.2 - agents_on_release}
limitstab.GetColorOfERS( grec->Grp_id,limitstab.ersid_val.f6_3_1_3_2,
				        grec->Current_agents_state.agents_on_release.get(),&TmpColorPtr);
ReplyObj.InsertFullDataBG(this,TmpColorPtr,"f6_3_1_3_2","Release",grec->Current_agents_state.agents_on_release.get(),ns1__ReturnGroupDetailFullInfo->returnArrayOfBlocks[i]);

//    //ERS : 6.3.1.3.3 - agents_on_wrap_up}
ReplyObj.InsertFullData(this,"f6_3_1_3_3","Wrap-up",grec->Current_agents_state.agents_on_wrap_up.get(),ns1__ReturnGroupDetailFullInfo->returnArrayOfBlocks[i]);

//    //ERS : 6.3.1.3.4 - idle_agents}
limitstab.GetColorOfERS( grec->Grp_id,limitstab.ersid_val.f6_3_1_3_4,
				       grec->Current_agents_state.idle_agents.get(),&TmpColorPtr);
ReplyObj.InsertFullDataBG(this,TmpColorPtr,"f6_3_1_3_4","Idle",grec->Current_agents_state.idle_agents.get(),ns1__ReturnGroupDetailFullInfo->returnArrayOfBlocks[i]);

 //    //ERS : 6.3.1.3.5 - acd_busy_agents}
ReplyObj.InsertFullData(this,"f6_3_1_3_5","ACD",grec->Current_agents_state.busy_agents.get(),ns1__ReturnGroupDetailFullInfo->returnArrayOfBlocks[i]);

 //    //ERS : 6.3.1.3.6 - non_acd_busy_agents}
limitstab.GetColorOfERS( grec->Grp_id,limitstab.ersid_val.f6_3_1_3_6,
				      grec->Current_agents_state.non_acd_busy_agents.get(),&TmpColorPtr);
ReplyObj.InsertFullDataBG(this,TmpColorPtr,"f6_3_1_3_6","Non-ACD",grec->Current_agents_state.non_acd_busy_agents.get(),ns1__ReturnGroupDetailFullInfo->returnArrayOfBlocks[i]);
 // if (non_acd_busy_agents != grp_stat.agents.non_acd_busy_agents){
 //      non_acd_busy_agents = grp_stat.agents.non_acd_busy_agents;
 //      ReplyObj.InsertFullData(this,"f6_3_1_3_6, non_acd_busy_agents);
 // };

 

 // //     {ERS : 6.3.1.3.14 - agents_on_dnd}
 limitstab.GetColorOfERS( grec->Grp_id,limitstab.ersid_val.f6_3_1_3_14,
				               grec->Current_agents_state.agents_on_dnd.get(),&TmpColorPtr);
 ReplyObj.InsertFullDataBG(this,TmpColorPtr,"f6_3_1_3_14","agents_on_dnd",grec->Current_agents_state.agents_on_dnd.get(),ns1__ReturnGroupDetailFullInfo->returnArrayOfBlocks[i]);

 //      //{ERS : 6.3.1.3.15 - agents_on_split}
 ReplyObj.InsertFullData(this,"f6_3_1_3_15","agents_on_split",grec->Current_agents_state.agents_on_split.get(),ns1__ReturnGroupDetailFullInfo->returnArrayOfBlocks[i]);


 //    //    {ERS : 6.3.1.3.13 - agents_on_f_wrap}
 ReplyObj.InsertFullData(this,"f6_3_1_3_13","agents_on_f_wrap",grec->Current_agents_state.agents_on_forced_wrap_up.get(),ns1__ReturnGroupDetailFullInfo->returnArrayOfBlocks[i]);


 //    //{ERS: 6.3.1.3.16 - agents wrap + f. wrap}
 ReplyObj.InsertFullData(this,"f6_3_1_3_16","num_of_agents",
	 grec->Current_agents_state.agents_on_wrap_up.get() +
	 grec->Current_agents_state.agents_on_forced_wrap_up.get(),ns1__ReturnGroupDetailFullInfo->returnArrayOfBlocks[i]);
 // if ((wrap_changed == TRUE) || (f_wrap_changed == TRUE))
 //          ReplyObj.InsertFullData(this,"f6_3_1_3_16, agents_on_wrap_up + agents_on_f_wrap);
}
return SOAP_OK;
}

/// Web service operation 'GroupEntityUpdateQueInfo' (returns error code or SOAP_OK)
int CsaWebServer::GroupQueInfo(ns1__GenInfoRequestType *ns1__RequestGroupQueInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupQueInfo)
	{
	GroupRecordReply ReplyObj;
	CsaRc_t                                        rc = ARC_OK;
	GroupRecord*                                   grec;
	//Fool protector
    if(ns1__RequestGroupQueInfo == NULL)
       return soap_receiver_fault(this, "No Request Data", "ns1__RequestGroupQueInfo is NULL");

	ns1__ReturnGroupQueInfo->responceInfoHeader = ResponceHeader;
	ns1__ReturnGroupQueInfo->responceInfoHeader->serviceReqId = 
		ns1__RequestGroupQueInfo->requestInfoHeader->serviceReqId;
	
	if(!TicketTab.VerifyTicket(ns1__RequestGroupQueInfo->requestInfoHeader->ticket))
	{
		ns1__ReturnGroupQueInfo->responceInfoHeader->errorCause = 500;
		return SOAP_OK;
	}

	for (unsigned int i=0; i<ns1__RequestGroupQueInfo->Id.size(); i++)
	{
	    limitstab.VerifyGroupInLimitTab(ns1__RequestGroupQueInfo->Id[i]); //PVL 28-07-2009 1002567
		rc = Csa_Grps_Tbls[TBL_ONLINE]->get(ns1__RequestGroupQueInfo->Id[i], &grec);
		if (rc == ARC_OK) {
			ReplyObj.AddNewGroupBlock(this,grec->Grp_id,ns1__ReturnGroupQueInfo);
	 		ns1__ReturnGroupQueInfo->responceInfoHeader->errorCause = (unsigned int)0;
			ReplyObj.MakePerGroup_QueInfo(this,grec,ns1__ReturnGroupQueInfo->returnArrayOfBlocks[i]);
		}
		else
		{
    		ns1__ReturnGroupQueInfo->responceInfoHeader->errorCause = (unsigned int)1;
		}
	}

	return SOAP_OK;
	}

		/// Web service operation 'UserLogin' (returns error code or SOAP_OK)
int CsaWebServer::UserLogin(ns1__LoginRequestType *ns1__RequestUserLogin, ns1__LoginReturnType *ns1__ReturnUserLogin)
{  
   	//Fool protector
    if(ns1__RequestUserLogin == NULL)
       return soap_receiver_fault(this, "No Request Data", "ns1__RequestUserLogin is NULL");

	sup_data_t *new_sup_data = NULL;
	
	ns1__ReturnUserLogin->responceInfoHeader = ResponceHeader;
	ns1__ReturnUserLogin->responceInfoHeader->errorCause = 0;
	ns1__ReturnUserLogin->responceInfoHeader->serviceReqId = 
		ns1__RequestUserLogin->serviceReqId;
    ns1__ReturnUserLogin->loginEnable = FALSE;
	if(this->TicketTab.GetTicket(ns1__RequestUserLogin->name, ns1__RequestUserLogin->pass, &new_sup_data)){
		if(new_sup_data != NULL){
		  ns1__ReturnUserLogin->ticket= new_sup_data->ticket; 
		  ns1__ReturnUserLogin->loginEnable = TRUE;
		  limitstab.ReloadAll(); //PVL 29-07-2009 1002487
		}
	}

	return SOAP_OK;
  }
/// Web service operation 'UserLogout' (returns error code or SOAP_OK)
int CsaWebServer::UserLogout(ns1__GenRequestType *ns1__RequestUserLogout, ns1__ResponceInfoHeaderType *ns1__NoDataResponce)
{
//Fool protector
 if(ns1__RequestUserLogout == NULL)
    return soap_receiver_fault(this, "No Request Data in ns1__RequestUserLogout", "ns1__RequestGroupDetailFullInfo is NULL");

	ns1__NoDataResponce->serviceReqId =ns1__RequestUserLogout->requestInfoHeader->serviceReqId;
	
	
	if(this->TicketTab.ReturnTicket(ns1__RequestUserLogout->requestInfoHeader->ticket))
        ns1__NoDataResponce->errorCause = 0;
	else
		ns1__NoDataResponce->errorCause = 1;

	return SOAP_OK;
}

	/// Web service operation 'GroupListForSupervisor' (returns error code or SOAP_OK)
int CsaWebServer::GroupList(ns1__GenRequestType *ns1__RequestGroupList, ns1__GroupListReturnType *ns1__ReturnGroupList)
{      
	GroupRecordReply ReplyObj;
	DBManager_c		&db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
	RetCode_t		bswrc = BSWRC_OK,executionResult;
	Statement_c	st;
	ResultSet_c rs;
	bool			dbirc = true, no_records = true;
		char sql_statement[MAX_SQL_QUERY_SIZE];
 //Fool protector
 if(ns1__RequestGroupList == NULL)
    return soap_receiver_fault(this, "No Request Data in ns1__RequestGroupList", "ns1__RequestGroupList is NULL");

		this->groupstab.LoadGroupList();

    unsigned int               grpId ;
    char                       grpName[51] ;
    char                       grpNumber[51];
    unsigned int               grpEmailEnable;        
	unsigned int               user_is_owner;
	char                       nla_priv[5];
	ns1__ReturnGroupList->responceInfoHeader = ResponceHeader;
	ns1__ReturnGroupList->responceInfoHeader->serviceReqId = ns1__RequestGroupList->requestInfoHeader->serviceReqId;
	ns1__ReturnGroupList->responceInfoHeader->errorCause = 0;

	sup_data_t *tmp_sup_data;
	if(!TicketTab.VerifyTicketGetData(ns1__RequestGroupList->requestInfoHeader->ticket,
		                    &tmp_sup_data))
	{
		ns1__ReturnGroupList->responceInfoHeader->errorCause = 500;
		return SOAP_OK;
	}

	if((tmp_sup_data->sup_level== 1)|| // supervisor 
        (tmp_sup_data->sup_level == 3)) //Entity Administrator //PVL, 1-12-2008 1001249
        sprintf(sql_statement,"select group_id,g_name, g_number,email_enabled from grp");
        else
			sprintf(sql_statement,"select grp.group_id,grp.g_name,grp.g_number,grp.email_enabled,grp_sup.nla_priv from grp,grp_sup where grp_sup.group_id=grp.group_id and grp_sup.g_type = 'g' and grp_sup.user_id=%d",
			        tmp_sup_data->user_id);
       
		st.SetQuery(sql_statement);
		bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);
			
		printf("ServiceReqId - %d\n",ns1__RequestGroupList->requestInfoHeader->serviceReqId);
		ns1__ReturnGroupList->responceInfoHeader = ResponceHeader;
		ns1__ReturnGroupList->responceInfoHeader->serviceReqId =
			              ns1__RequestGroupList->requestInfoHeader->serviceReqId;
	

//		InsertGroupList(this,1,"SupportGroup","10",0,ns1__ReturnGroupListForSupervisor);
//		InsertGroupList(this,2,"SupportGroup2Email","2",1,ns1__ReturnGroupListForSupervisor);
		Ushort_t total_records_num = rs.GetRecordCount(); 
		Ushort_t left_records_num = rs.GetRecordCount(); 
		Ushort_t first_record_index;
		if (total_records_num == 0)
		{
			//NOTHING
		}
		else
		{
			while(left_records_num > 0)
			{ 
			//save the first record index 
			first_record_index = total_records_num - left_records_num;		    						
			//start to fill buffer with NlaGroupEntityInfo structures
			while(rs.Next() && bswrc == BSWRC_OK)
			{ 
				if (!(rs.GetUlongByName("group_id",(Ulong_t &)grpId))            ||
					!(rs.GetStringByName("g_name",grpName))                      ||
					!(rs.GetStringByName("g_number",(char *)grpNumber))        ||
					!(rs.GetUlongByName("email_enabled",(Ulong_t &)grpEmailEnable)))
				{
					//do nothing. get next result set
					//GetLog() << " AdiMngr_c::HandleAgentGroupListReply - opcode: Capp_Dbi_get_lst_grp_entities_req_op_code\n"
					//	<< " Can't get record from result set\n"<< LOG_TERMINATOR;
					left_records_num--;
				}
				else
				{		
					user_is_owner = (Flag_t)false;
					if((tmp_sup_data->sup_level == 1)|| // supervisor 
						(tmp_sup_data->sup_level == 3)) //Entity Administrator //PVL, 8-02-2009 1001249
						user_is_owner = (Flag_t)true;
			
					else 
					{
					rs.GetStringByName("nla_priv",(char *)nla_priv);
					if(nla_priv[0] == 't')
						user_is_owner = (Flag_t)true;
					}					
				ReplyObj.InsertGroupList(this,grpId,grpName,grpNumber,grpEmailEnable,ns1__ReturnGroupList);			
				left_records_num--;
				//if there are more info records to insert 				
				if (left_records_num == 0)
				break;
				}
			}
          
		}
	}
			
			return SOAP_OK;
}

//----------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
int CsaWebServer::GroupAgentsInfo(ns1__GenInfoRequestType *ns1__RequestGroupAgentsInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupAgentsInfo)
{  
	GroupRecordReply ReplyObj;
	CsaRc_t                                        rc = ARC_OK;
	GroupRecord*                                   grec;
 //Fool protector
 if(ns1__RequestGroupAgentsInfo == NULL)
    return soap_receiver_fault(this, "No Request Data in ns1__RequestGroupAgentsInfo", "ns1__RequestGroupAgentsInfo is NULL");

	ns1__ReturnGroupAgentsInfo->responceInfoHeader = ResponceHeader;
	ns1__ReturnGroupAgentsInfo->responceInfoHeader->serviceReqId = 
		ns1__RequestGroupAgentsInfo->requestInfoHeader->serviceReqId;
	
	if(!TicketTab.VerifyTicket(ns1__RequestGroupAgentsInfo->requestInfoHeader->ticket))
	{
		ns1__ReturnGroupAgentsInfo->responceInfoHeader->errorCause = 500;
		return SOAP_OK;
	}


   for (unsigned int i=0; i<ns1__RequestGroupAgentsInfo->Id.size(); i++)
	{
		limitstab.VerifyGroupInLimitTab(ns1__RequestGroupAgentsInfo->Id[i]); //PVL 28-07-2009 1002567
		rc = Csa_Grps_Tbls[TBL_ONLINE]->get(ns1__RequestGroupAgentsInfo->Id[i], &grec);
		
		if(rc == ARC_OK){
		  ReplyObj.AddNewGroupBlock(this,grec->Grp_id,ns1__ReturnGroupAgentsInfo);
		  ns1__ReturnGroupAgentsInfo->responceInfoHeader->errorCause = (unsigned int)0;
		  ReplyObj.MakePerGroup_AgentsInfo(this,grec,ns1__ReturnGroupAgentsInfo->returnArrayOfBlocks[i]);
		}
		else {
		ns1__ReturnGroupAgentsInfo->responceInfoHeader->errorCause = (unsigned int)1;
		}
	}
 return SOAP_OK;
}
//--------------------------------------------
CsaWebServer::AgentsPerGroupInfo(ns1__GenInfoRequestType *ns1__RequestGroupAgentsMatrix, ns1__ReturnMatrixDataTypeBGperGroupArr *ns1__ReturnGroupAgentsMatrix)
{
  AgentRecordReply ReplyObj;
  AgentRecord*                                   agent;
  CsaAgentPerGroupRecord_t*                      agent_per_group;
  Uint_t                                         token;
  
  CsaRc_t rc;
  CsaRc_t rc2;
 
  //Fool protector
    if(ns1__RequestGroupAgentsMatrix == NULL)
       return soap_receiver_fault(this, "No Request Data", "ns1__RequestGroupAgentsMatrix is NULL");

 

	ns1__ReturnGroupAgentsMatrix->responceInfoHeader = ResponceHeader;
	ns1__ReturnGroupAgentsMatrix->responceInfoHeader->errorCause = (unsigned int)1;
	ns1__ReturnGroupAgentsMatrix->responceInfoHeader->serviceReqId = 
	ns1__RequestGroupAgentsMatrix->requestInfoHeader->serviceReqId;

	if(!TicketTab.VerifyTicket(ns1__RequestGroupAgentsMatrix->requestInfoHeader->ticket))
	{
		ns1__ReturnGroupAgentsMatrix->responceInfoHeader->errorCause = 500;
		return SOAP_OK;
	}
	for (unsigned int i=0; i<ns1__RequestGroupAgentsMatrix->Id.size(); i++)
	{
		limitstab.VerifyGroupInLimitTab(ns1__RequestGroupAgentsMatrix->Id[i]); //PVL 28-07-2009 1002567	
		ns1__ReturnGroupAgentsMatrix->responceInfoHeader->errorCause = (unsigned int)0;
		ReplyObj.AddNewGroupBlock(this,
			             ns1__RequestGroupAgentsMatrix->Id[i],
			             ns1__ReturnGroupAgentsMatrix);
	for (rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getFirstLoginForGroup(
													ns1__RequestGroupAgentsMatrix->Id[i],
													&agent_per_group,
													token);
			rc2 == ARC_OK;
			rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getNextLoginForGroup(
													&agent_per_group,
													token)) 
		{
	
				// if agent logged in, add full info for agent to event
			if (agent_per_group->Logged_in) 
			{
				   limitstab.VerifyGroupInLimitTab(agent_per_group->Group_id); //PVL 28-07-2009 1002567
					// static info - need agent record
					ReplyObj.CreateNewRow(this); //Start Row
					rc = Csa_Agents_Tbls[TBL_ONLINE]->get(agent_per_group->Internal_id,&agent);
					if (rc == ARC_OK)
					{
					ReplyObj.InsertAgentDataToRow(this,agent,agent_per_group,ns1__RequestGroupAgentsMatrix->Id[i]);
					// At the END , inser Row 
					ReplyObj.InsertRowToMatrix(this,ns1__ReturnGroupAgentsMatrix->returnArrayOfGroupsForAgent[i]);//End Row			 
					}
			}

		}
	}
   
	
	return SOAP_OK;
}





	/// Web service operation 'AgentsList' (returns error code or SOAP_OK)
int CsaWebServer::AgentsList(ns1__GenRequestType *ns1__RequestAgentsList, ns1__AgentsListReturnType *ns1__ReturnAgentsList)
{
	AgentRecordReply ReplyObj;

	//Fool protector
	if(ns1__RequestAgentsList == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestAgentsList", "ns1__RequestAgentsList is NULL");

	if(!TicketTab.VerifyTicket(ns1__RequestAgentsList->requestInfoHeader->ticket))
	{
		ns1__ReturnAgentsList->responceInfoHeader->errorCause = 500;
		return SOAP_OK;
	}
  

   ns1__ReturnAgentsList->responceInfoHeader = ResponceHeader;
   ns1__ReturnAgentsList->responceInfoHeader->serviceReqId = ns1__RequestAgentsList->requestInfoHeader->serviceReqId;
   ns1__ReturnAgentsList->responceInfoHeader->errorCause = 0;
	
   agentstab.LoadAgentsList();								
		
  if(agentstab.tab.size() >0)
	for(agentstab.it=agentstab.tab.begin(); agentstab.it!= agentstab.tab.end(); ++agentstab.it) 
	{	   
	  ReplyObj.InsertAgentsList(this,
		  (*agentstab.it).second.a_id,//a_id,
		  (*agentstab.it).second.a_name,//a_name,
		  (*agentstab.it).second.a_number,//a_number,
		  ns1__ReturnAgentsList);		
	}
	return SOAP_OK;
}
//------------------------------------------------------------------------------------------------
	/// Web service operation 'BriefAgents' (returns error code or SOAP_OK)
int CsaWebServer::BriefAgents(ns1__BriefInfoRequestType *ns1__RequestBriefAgentsMatrix, ns1__ReturnMatrixDataTypeBG *ns1__ReturnBriefAgentsMatrix)
{
  AgentRecordReply                               ReplyObj;
  AgentRecord*                                   agent = NULL;
  CsaAgentPerGroupRecord_t*                      agent_per_group = NULL;
  Uint_t                                         token;
 
  CsaRc_t rc;
  CsaRc_t rc2;
 	//Fool protector
    if(ns1__RequestBriefAgentsMatrix == NULL)
       return soap_receiver_fault(this, "No Request Data", "ns1__RequestBriefAgentsMatrix is NULL");


  ns1__ReturnBriefAgentsMatrix->responceInfoHeader = ResponceHeader;
  ns1__ReturnBriefAgentsMatrix->responceInfoHeader->errorCause = (unsigned int)1;
  ns1__ReturnBriefAgentsMatrix->responceInfoHeader->serviceReqId = 
	  ns1__RequestBriefAgentsMatrix->requestInfoHeader->serviceReqId;

	if(!TicketTab.VerifyTicket(ns1__RequestBriefAgentsMatrix->requestInfoHeader->ticket))
	{
		ns1__ReturnBriefAgentsMatrix->responceInfoHeader->errorCause = 500;
		return SOAP_OK;
	}

//  for (unsigned int i=0; i<agentstab.tab.size(); i++) //cycle through Agents records
	Id_type               a_id ;
if(agentstab.tab.size() >0)
	for(agentstab.it=agentstab.tab.begin(); agentstab.it!= agentstab.tab.end(); ++agentstab.it) 
	{
        BriefAgentInfo TotalsAgentInfo;		
		a_id = (*agentstab.it).second.a_id;
		rc = Csa_Agents_Tbls[TBL_ONLINE]->getByDataBaseId( a_id,&agent);
		if (rc == ARC_OK){
			// cycle through login records
			for (
				rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getFirstLoginForAgent(agent->Internal_id, &agent_per_group, token);
				rc2 == ARC_OK;
				rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getNextLoginForAgent(&agent_per_group, token)
				)
				{
				ns1__ReturnBriefAgentsMatrix->responceInfoHeader->errorCause  = (unsigned int)0;
						// if agent logged in, add full info for agent to event
					if (agent_per_group->Logged_in) 
					{
							// static info - need agent record						
						
					}
                       //Collect Data over All Groups for Agent
					    TotalsAgentInfo.SetAcdData(agent,agent_per_group);
				} //End of All groups per Agent 
				// Build Brief Data
				ReplyObj.CreateNewRow(this); //Start Row
				ReplyObj.InsertBriefAgentDataToRow(this,agent,TotalsAgentInfo);
				// At the END , inser Row 
				ReplyObj.InsertRowToMatrix(this,ns1__ReturnBriefAgentsMatrix);//End Row			

		} // If agent FOUND
		else 
		{ //Agent not Found , so show LogOut 
		    ns1__ReturnBriefAgentsMatrix->responceInfoHeader->errorCause  = (unsigned int)0;
			ReplyObj.CreateNewRow(this); //Start Row
			ReplyObj.InsertBrief_LogOut_AgentDataToRow (this,a_id);
			ReplyObj.InsertRowToMatrix(this,ns1__ReturnBriefAgentsMatrix);//End Row	
		}

		
	}

	return SOAP_OK;
}



int CsaWebServer::GroupAbndInfo(ns1__GenInfoRequestType *ns1__RequestGroupAbndInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupAbndInfo)
{
    GroupRecordReply ReplyObj;
	CsaRc_t                                        rc = ARC_OK;
	GroupRecord*                                   grec;
  	//Fool protector
	if(ns1__RequestGroupAbndInfo == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestGroupAbndInfo", "ns1__RequestGroupAbndInfo is NULL");

   
	
	ns1__ReturnGroupAbndInfo->responceInfoHeader = ResponceHeader;
	ns1__ReturnGroupAbndInfo->responceInfoHeader->serviceReqId = 
		ns1__RequestGroupAbndInfo->requestInfoHeader->serviceReqId;
   	
	if(!TicketTab.VerifyTicket(ns1__RequestGroupAbndInfo->requestInfoHeader->ticket))
	{
		ns1__ReturnGroupAbndInfo->responceInfoHeader->errorCause = 500;
		return SOAP_OK;
	}

 	for (unsigned int i=0; i<ns1__RequestGroupAbndInfo->Id.size(); i++)
	{
		limitstab.VerifyGroupInLimitTab(ns1__RequestGroupAbndInfo->Id[i]); //PVL 28-07-2009 1002567
		rc = Csa_Grps_Tbls[TBL_ONLINE]->get(ns1__RequestGroupAbndInfo->Id[i], &grec);
		if (rc== ARC_OK)
		{
			ReplyObj.AddNewGroupBlock(this,grec->Grp_id,ns1__ReturnGroupAbndInfo);
			ns1__ReturnGroupAbndInfo->responceInfoHeader->errorCause = (unsigned int)0;
			ReplyObj.MakePerGroup_AbandInfo (this,grec,ns1__ReturnGroupAbndInfo->returnArrayOfBlocks[i]);
		}
		else
		{
			ns1__ReturnGroupAbndInfo->responceInfoHeader->errorCause = (unsigned int)1;
		}
	}
	

	return SOAP_OK;
}

 /// Web service operation 'GroupEntityUpdateShiftInfo' (returns error code or SOAP_OK)
int  CsaWebServer::GroupShiftInfo(ns1__GenInfoRequestType *ns1__RequestGroupShiftInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupShiftInfo)
{
	GroupRecordReply ReplyObj;
	CsaRc_t                                        rc = ARC_OK;
	GroupRecord*                                   grec;
   	//Fool protector
	if(ns1__RequestGroupShiftInfo == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestGroupShiftInfo", "ns1__RequestGroupShiftInfo is NULL");
  
	ns1__ReturnGroupShiftInfo->responceInfoHeader = ResponceHeader;
	ns1__ReturnGroupShiftInfo->responceInfoHeader->errorCause = (unsigned int)0;
	ns1__ReturnGroupShiftInfo->responceInfoHeader->serviceReqId = 
		ns1__RequestGroupShiftInfo->requestInfoHeader->serviceReqId;
	
	if(!TicketTab.VerifyTicket(ns1__RequestGroupShiftInfo->requestInfoHeader->ticket))
	{
		ns1__ReturnGroupShiftInfo->responceInfoHeader->errorCause = 500;
		return SOAP_OK;
	}

  	for (unsigned int i=0; i<ns1__RequestGroupShiftInfo->Id.size(); i++)
	{
		limitstab.VerifyGroupInLimitTab(ns1__RequestGroupShiftInfo->Id[i]); //PVL 28-07-2009 1002567
		rc = Csa_Grps_Tbls[TBL_ONLINE]->get(ns1__RequestGroupShiftInfo->Id[i], &grec);
		if (rc== ARC_OK )
		{
			ReplyObj.AddNewGroupBlock(this,grec->Grp_id,ns1__ReturnGroupShiftInfo);
			ns1__ReturnGroupShiftInfo->responceInfoHeader->errorCause = (unsigned int)0;
			ReplyObj.MakePerGroup_ShiftInfo (this,grec,ns1__ReturnGroupShiftInfo->returnArrayOfBlocks[i]);
		}
		else
		{
			ns1__ReturnGroupShiftInfo->responceInfoHeader->errorCause = (unsigned int)1;
		}
	}
	return SOAP_OK;
}
//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
int CsaWebServer::GroupIntOvfInfo(ns1__GenInfoRequestType *ns1__RequestGroupIntOvfInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupIntOvfInfo)
{
		GroupRecordReply ReplyObj;
	CsaRc_t                                        rc = ARC_OK;
	GroupRecord*                                   grec;
   	//Fool protector
	if(ns1__RequestGroupIntOvfInfo == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestGroupIntOvfInfo", "ns1__RequestGroupIntOvfInfo is NULL");

    ns1__ReturnGroupIntOvfInfo->responceInfoHeader = ResponceHeader;
	ns1__ReturnGroupIntOvfInfo->responceInfoHeader->errorCause = (unsigned int)0;
	ns1__ReturnGroupIntOvfInfo->responceInfoHeader->serviceReqId = 
		ns1__RequestGroupIntOvfInfo->requestInfoHeader->serviceReqId;
	
	if(!TicketTab.VerifyTicket(ns1__RequestGroupIntOvfInfo->requestInfoHeader->ticket))
	{
		ns1__ReturnGroupIntOvfInfo->responceInfoHeader->errorCause = 500;
		return SOAP_OK;
	}

	for (unsigned int i=0; i<ns1__RequestGroupIntOvfInfo->Id.size(); i++)
	{
		limitstab.VerifyGroupInLimitTab(ns1__RequestGroupIntOvfInfo->Id[i]);//PVL 28-07-2009 1002567
		rc = Csa_Grps_Tbls[TBL_ONLINE]->get(ns1__RequestGroupIntOvfInfo->Id[i], &grec);
       if (rc== ARC_OK)
		{
			ReplyObj.AddNewGroupBlock(this,grec->Grp_id,ns1__ReturnGroupIntOvfInfo);
			ns1__ReturnGroupIntOvfInfo->responceInfoHeader->errorCause = (unsigned int)0;
			ReplyObj.MakePerGroup_InterflInfo (this,grec,ns1__ReturnGroupIntOvfInfo->returnArrayOfBlocks[i]);
		}
	else
	{
		ns1__ReturnGroupIntOvfInfo->responceInfoHeader->errorCause = (unsigned int)1;
	}
	}
	return SOAP_OK;
}
//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
/// Web service operation 'GroupGeneralInfo' (returns error code or SOAP_OK)
int CsaWebServer::GroupGeneralInfo(ns1__GenInfoRequestType *ns1__RequestGroupGeneralInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupGeneralInfo)
{
	GroupRecordReply ReplyObj;
	CsaRc_t                                        rc = ARC_OK;
	GroupRecord*                                   grec;
    unsigned int  *TmpColorPtr = NULL;
   	
	//Fool protector
	if(ns1__RequestGroupGeneralInfo == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestGroupGeneralInfo", "ns1__RequestGroupGeneralInfo is NULL");

    int acd_type_acd_offset =	getOffSetOfAcdCallTypes(CL_CALL_TYPE_ACD);
	int acd_type_email_offset	= 0;//getOffSetOfAcdCallTypes(CL_CALL_TYPE_EMAIL);
	int acd_type_chat_offset = 0;//	=	getOffSetOfAcdCallTypes(CL_CALL_TYPE_CHAT);
	int acd_type_dial_list_offset = 0;

	bool supportedOutbound = Csa_Setup.supportOutbound();
	bool supportedChat =     Csa_Setup.supportChat();
	bool supportedEmail =    Csa_Setup.supportEmail();
	bool supportedDialList = Csa_Setup.supportDialList();
    
   // unsigned int 
    ns1__ReturnGroupGeneralInfo->responceInfoHeader = ResponceHeader;
	ns1__ReturnGroupGeneralInfo->responceInfoHeader->errorCause = (unsigned int)0;
	ns1__ReturnGroupGeneralInfo->responceInfoHeader->serviceReqId = 
		ns1__RequestGroupGeneralInfo->requestInfoHeader->serviceReqId;

	if(!TicketTab.VerifyTicket(ns1__RequestGroupGeneralInfo->requestInfoHeader->ticket))
	{
		ns1__ReturnGroupGeneralInfo->responceInfoHeader->errorCause = 500;
		return SOAP_OK;
	}
    for (unsigned int i=0; i<ns1__RequestGroupGeneralInfo->Id.size(); i++)
	{
		limitstab.VerifyGroupInLimitTab(ns1__RequestGroupGeneralInfo->Id[i]);//PVL 28-07-2009 1002567
		rc = Csa_Grps_Tbls[TBL_ONLINE]->get(ns1__RequestGroupGeneralInfo->Id[i], &grec);
		if (rc == ARC_OK) {
			ReplyObj.AddNewGroupBlock(this,grec->Grp_id,ns1__ReturnGroupGeneralInfo);
	 		ns1__ReturnGroupGeneralInfo->responceInfoHeader->errorCause = (unsigned int)0;
			ReplyObj.MakePerGroup_QueInfo(this,grec,ns1__ReturnGroupGeneralInfo->returnArrayOfBlocks[i]);
		}
		else
		{
    		ns1__ReturnGroupGeneralInfo->responceInfoHeader->errorCause = (unsigned int)1;
		}
	}

    
    	
	
	return SOAP_OK;
}


//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
/// Web service operation 'GroupPeriodInfo' (returns error code or SOAP_OK)
	int CsaWebServer::GroupPeriodInfo(ns1__GenInfoRequestType *ns1__RequestGroupPeriodInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupPeriodInfo)
	{
	GroupRecordReply ReplyObj;
	CsaRc_t                                        rc = ARC_OK;
	GroupRecord*                                   grec;
	//Fool protector
	if(ns1__RequestGroupPeriodInfo == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestGroupPeriodInfo", "ns1__RequestGroupPeriodInfo is NULL");

    ns1__ReturnGroupPeriodInfo->responceInfoHeader = ResponceHeader; 
	ns1__ReturnGroupPeriodInfo->responceInfoHeader->errorCause = (unsigned int)0;
	ns1__ReturnGroupPeriodInfo->responceInfoHeader->serviceReqId = 
		ns1__RequestGroupPeriodInfo->requestInfoHeader->serviceReqId;
	
	if(!TicketTab.VerifyTicket(ns1__RequestGroupPeriodInfo->requestInfoHeader->ticket))
	{
		ns1__ReturnGroupPeriodInfo->responceInfoHeader->errorCause = 500;
		return SOAP_OK;
	}
	for (unsigned int i=0; i<ns1__RequestGroupPeriodInfo->Id.size(); i++)
	{	
		limitstab.VerifyGroupInLimitTab(ns1__RequestGroupPeriodInfo->Id[i]);//PVL 28-07-2009 1002567
		rc = Csa_Grps_Tbls[TBL_ONLINE]->get(ns1__RequestGroupPeriodInfo->Id[i], &grec);
		if (rc == ARC_OK) {
		 ReplyObj.AddNewGroupBlock(this,grec->Grp_id,ns1__ReturnGroupPeriodInfo);
	 		ns1__ReturnGroupPeriodInfo->responceInfoHeader->errorCause = (unsigned int)0;
		 //ReplyObj.MakePerGroup_QueInfo (this,grec,ns1__ReturnGroupPeriodInfo->returnArrayOfBlocks[i]);
		 ReplyObj.MakePerGroup_ShiftInfo (this,grec,ns1__ReturnGroupPeriodInfo->returnArrayOfBlocks[i]);
		 ReplyObj.MakePerGroup_AbandInfo (this,grec,ns1__ReturnGroupPeriodInfo->returnArrayOfBlocks[i]);
		 ReplyObj.MakePerGroup_InterflInfo (this,grec,ns1__ReturnGroupPeriodInfo->returnArrayOfBlocks[i]);
		}
		else
		{
    		ns1__ReturnGroupPeriodInfo->responceInfoHeader->errorCause = (unsigned int)1;
		}
	}
	return SOAP_OK;
	}

//--------------------------------------------------------------------------------------------------------------
			/// Web service operation 'DefaultWorkspace' (returns error code or SOAP_OK)
int CsaWebServer::DefaultWorkspace(ns1__DefaultWorkspaceRequestType *ns1__RequestDefaultWorkspace, ns1__DefaultWorkspaceReturnType *ns1__ReturnDefaultWorkspace)
{      
   ReportsReply ReplyObj;

   //Fool protector
	if(ns1__RequestDefaultWorkspace == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestDefaultWorkspace", "ns1__RequestDefaultWorkspace is NULL");


   ns1__ReturnDefaultWorkspace->responceInfoHeader = ResponceHeader;
		ns1__ReturnDefaultWorkspace->responceInfoHeader->serviceReqId =
		       ns1__RequestDefaultWorkspace->requestInfoHeader->serviceReqId;

	ns1__ReturnDefaultWorkspace->responceInfoHeader->errorCause = (unsigned int)0;

   	sup_data_t *tmp_sup_data;
	if(!TicketTab.VerifyTicketGetData(ns1__RequestDefaultWorkspace->requestInfoHeader->ticket,
		                    &tmp_sup_data))
	{
		ns1__ReturnDefaultWorkspace->responceInfoHeader->errorCause = 500;
		return SOAP_OK;
	}

	if(ReplyObj.TryLoadNewDefConf(this,tmp_sup_data->user_id,ns1__ReturnDefaultWorkspace))
     return SOAP_OK;
   else
     ReplyObj.LoadOldDefConf(this,tmp_sup_data->user_id,ns1__ReturnDefaultWorkspace);   
  return SOAP_OK;
}

	/// Web service operation 'ReportListData' (returns error code or SOAP_OK)
int CsaWebServer::ReportListData(ns1__ReportListDataRequestType *ns1__RequestReportListData, ns1__ReportListDataReturnType *ns1__ReturnReportListData)
{
	ReportsReply ReplyObj;
   //Fool protector
	if(ns1__RequestReportListData == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestReportListData", "ns1__RequestReportListData is NULL");

	ReplyObj.SetReturnArray(ns1__ReturnReportListData);
    ns1__ReturnReportListData->responceInfoHeader = ResponceHeader;
	ns1__ReturnReportListData->responceInfoHeader->errorCause = (unsigned int)0;
	ns1__ReturnReportListData->responceInfoHeader->serviceReqId = 
		ns1__RequestReportListData->requestInfoHeader->serviceReqId;
	
	sup_data_t *tmp_sup_data;
	if(!TicketTab.VerifyTicketGetData(ns1__RequestReportListData->requestInfoHeader->ticket,
		                    &tmp_sup_data))
	{
		ns1__ReturnReportListData->responceInfoHeader->errorCause = 500;
		return SOAP_OK;
	}
	//Get Reports for agentId
	if( ReplyObj.GetAllReportsByAgentID(this,tmp_sup_data->user_id,ns1__ReturnReportListData))
       ns1__ReturnReportListData->responceInfoHeader->errorCause = (unsigned int)1;
	
	return SOAP_OK;
}
//--------------------------------------------------------------------------------------------------------------
/// Web service operation 'VoiceDNISList' (returns error code or SOAP_OK)
int CsaWebServer::VoiceDNISList(ns1__GenRequestType *ns1__RequestVoiceDNISList, ns1__DNISListReturnType *ns1__ReturnVoiceDNISList)
{
	DNISRecordReply ReplyObj;
   //Fool protector
	if(ns1__RequestVoiceDNISList == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestVoiceDNISList", "ns1__RequestVoiceDNISList is NULL");

	ReplyObj.GetHandleDNISList(this,ns1__RequestVoiceDNISList,ns1__ReturnVoiceDNISList,false);	//Voice
	return SOAP_OK;
}
//--------------------------------------------------------------------------------------------------------------
/// Web service operation 'EmailDNISList' (returns error code or SOAP_OK)
int CsaWebServer::EmailDNISList(ns1__GenRequestType *ns1__RequestEmailDNISList, ns1__DNISListReturnType *ns1__ReturnEmailDNISList)
{
	DNISRecordReply ReplyObj;
   //Fool protector
	if(ns1__RequestEmailDNISList == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestEmailDNISList", "ns1__RequestEmailDNISList is NULL");

	ReplyObj.GetHandleDNISList(this,ns1__RequestEmailDNISList,ns1__ReturnEmailDNISList,true); //Email	
	return SOAP_OK;
}
//--------------------------------------------------------------------------------------------------------------
/// Web service operation 'BriefVoiceDNIS' (returns error code or SOAP_OK)
int CsaWebServer::BriefVoiceDNIS(ns1__BriefInfoRequestType *ns1__RequestBriefVoiceDNISMatrix, ns1__ReturnMatrixDataTypeBG *ns1__ReturnBriefVoiceDNISMatrix)
{
	CsaRc_t         rc = ARC_OK;
	DNISRecordReply ReplyObj;
	CsaDNIS_Record* drec;
   //Fool protector
	if(ns1__RequestBriefVoiceDNISMatrix == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestBriefVoiceDNISMatrix", "ns1__RequestBriefVoiceDNISMatrix is NULL");

	ns1__ReturnBriefVoiceDNISMatrix->responceInfoHeader = ResponceHeader;
	ns1__ReturnBriefVoiceDNISMatrix->responceInfoHeader->errorCause =0;
	ns1__ReturnBriefVoiceDNISMatrix->responceInfoHeader->serviceReqId = 
		ns1__RequestBriefVoiceDNISMatrix->requestInfoHeader->serviceReqId;
	if(!TicketTab.VerifyTicket(ns1__RequestBriefVoiceDNISMatrix->requestInfoHeader->ticket))
	{
		ns1__ReturnBriefVoiceDNISMatrix->responceInfoHeader->errorCause = 500;
		return SOAP_OK;
	}

	// cycle through records
	rc = Csa_Dnis_Tbls[TBL_ONLINE]->getFirstDNIS(&drec);
	if (rc == ARC_OK) {
	while ( drec ) {		
			if(	drec->Id != 99999){
				if(ReplyObj.IsDNIS(this,drec,NOT_EMAIL_DNIS))
				{
					ReplyObj.CreateNewRow(this); //Start Row
					ReplyObj.InsertBriefVoiceDNISDataToRow(this,drec);
					ReplyObj.InsertRowToMatrix(this,ns1__ReturnBriefVoiceDNISMatrix);//End Row	
				}	
				Csa_Dnis_Tbls[TBL_ONLINE]->getNextDNIS(&drec);
			}
			else {
                 return SOAP_OK;
			}			
		}
	}
	else 
	{
       ns1__ReturnBriefVoiceDNISMatrix->responceInfoHeader->errorCause =(unsigned int)1;
	}
	return SOAP_OK;
}
//--------------------------------------------------------------------------------------------------------------
/// Web service operation 'BriefEmailDNIS' (returns error code or SOAP_OK)
int CsaWebServer::BriefEmailDNIS(ns1__BriefInfoRequestType *ns1__RequestBriefEMailDNISMatrix, ns1__ReturnMatrixDataTypeBG *ns1__ReturnBriefEMailDNISMatrix)
{
    CsaRc_t         rc = ARC_OK;
    DNISRecordReply ReplyObj;
	CsaDNIS_Record* drec;
   //Fool protector
	if(ns1__RequestBriefEMailDNISMatrix == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestBriefEMailDNISMatrix", "ns1__RequestBriefEMailDNISMatrix is NULL");

	ns1__ReturnBriefEMailDNISMatrix->responceInfoHeader = ResponceHeader;
	ns1__ReturnBriefEMailDNISMatrix->responceInfoHeader->errorCause =0;
	ns1__ReturnBriefEMailDNISMatrix->responceInfoHeader->serviceReqId =
		ns1__RequestBriefEMailDNISMatrix->requestInfoHeader->serviceReqId;
	
	if(!TicketTab.VerifyTicket(ns1__RequestBriefEMailDNISMatrix->requestInfoHeader->ticket))
	{
		ns1__ReturnBriefEMailDNISMatrix->responceInfoHeader->errorCause = 500;
		return SOAP_OK;
	}

	// cycle through records
	rc= Csa_Dnis_Tbls[TBL_ONLINE]->getFirstDNIS(&drec);
	if (rc == ARC_OK) {
		while ( drec ) {		
			if(	drec->Id != 99999){
				if(ReplyObj.IsDNIS(this,drec,EMAIL_DNIS)){
					ReplyObj.CreateNewRow(this); //Start Row
					ReplyObj.InsertBriefEmailDNISDataToRow(this,drec);
					ReplyObj.InsertRowToMatrix(this,ns1__ReturnBriefEMailDNISMatrix);//End Row	
					}
				Csa_Dnis_Tbls[TBL_ONLINE]->getNextDNIS(&drec);
			}
			else{
				return SOAP_OK;
			}			
		}
	}
	else
	{
         ns1__ReturnBriefEMailDNISMatrix->responceInfoHeader->errorCause = (unsigned int)1;
	}
	return SOAP_OK;
}

//------------------------------------------------------------------------------------
/// Web service operation 'IvrApplInfo' (returns error code or SOAP_OK)
int CsaWebServer::IvrApplInfo(ns1__GenInfoRequestType *ns1__RequestIvrApplInfo, ns1__ReturnArrayDataType *ns1__ReturnIvrApplInfo)
{
    IVRApplicationReply ReplyObj;
	CsaRc_t                                        rc = ARC_OK;
	unsigned int ivrapp_id;  // PVL_WEB 3-8-2009
	IvrAppRecord_c	*ivrapp;
   //Fool protector
	if(ns1__RequestIvrApplInfo == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestIvrApplInfo", "ns1__RequestIvrApplInfo is NULL");

    ns1__ReturnIvrApplInfo->responceInfoHeader = ResponceHeader; 
	ns1__ReturnIvrApplInfo->responceInfoHeader->errorCause = (unsigned int)0;
	ns1__ReturnIvrApplInfo->responceInfoHeader->serviceReqId = 
		ns1__RequestIvrApplInfo->requestInfoHeader->serviceReqId;
	
	if(!TicketTab.VerifyTicket(ns1__RequestIvrApplInfo->requestInfoHeader->ticket))
	{
		ns1__ReturnIvrApplInfo->responceInfoHeader->errorCause = 500;
		return SOAP_OK;
	}

	

	for (unsigned int i=0; i<ns1__RequestIvrApplInfo->Id.size(); i++)
	{
		
		ivrapp_id = ns1__RequestIvrApplInfo->Id[i];// PVL_WEB 3-8-2009
        ReplyObj.StartMonitor(ivrapp_id);          // PVL_WEB 3-8-2009

		rc = IvrAppMngr_c::GetTable().Get(ns1__RequestIvrApplInfo->Id[i],ivrapp);
		if (rc == ARC_OK) {
			ReplyObj.AddNewIvrAppBlock(this,ivrapp->m_app_id,ns1__ReturnIvrApplInfo);
	 		ns1__ReturnIvrApplInfo->responceInfoHeader->errorCause = (unsigned int)0;
		
		  rc=ReplyObj.MakeIvrAppInfo (this,ivrapp,ns1__ReturnIvrApplInfo->returnArrayOfBlocks[i]);
		  if (rc != ARC_OK) 
			ns1__ReturnIvrApplInfo->responceInfoHeader->errorCause = (unsigned int)2;
		
		}
		else
		{
    		ns1__ReturnIvrApplInfo->responceInfoHeader->errorCause = (unsigned int)1;
		}
	}
return SOAP_OK;
}
//------------------------------------------------------------------------------------
/// Web service operation 'IvrPortInfo' (returns error code or SOAP_OK)
int CsaWebServer::IvrPortInfo(ns1__GenInfoRequestType *ns1__RequestIvrPortInfo, ns1__ReturnArrayDataType *ns1__ReturnIvrPortInfo)
{     IVRGrpPortReply ReplyObj;
	  CsaRc_t                       rc = ARC_OK;
      CsaIvrPortRecord_c			*prec;
   //Fool protector
	if(ns1__RequestIvrPortInfo == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestIvrPortInfo", "ns1__RequestIvrPortInfo is NULL");
	 

      ns1__ReturnIvrPortInfo->responceInfoHeader = ResponceHeader; 
	  ns1__ReturnIvrPortInfo->responceInfoHeader->errorCause = (unsigned int)1;
      ns1__ReturnIvrPortInfo->responceInfoHeader->serviceReqId = 
		  ns1__RequestIvrPortInfo->requestInfoHeader->serviceReqId;
	
	if(!TicketTab.VerifyTicket(ns1__RequestIvrPortInfo->requestInfoHeader->ticket))
	{
		ns1__ReturnIvrPortInfo->responceInfoHeader->errorCause = 500;
		return SOAP_OK;
	}

	 

	  for (unsigned int i=0; i<ns1__RequestIvrPortInfo->Id.size(); i++){
		ReplyObj.StartMonitorPort(ns1__RequestIvrPortInfo->Id[i]); //PVL_WEB 3-8-2009 ivr_group_id
	    ReplyObj.AddNewIvrGrpBlock(this,ns1__RequestIvrPortInfo->Id[i],ns1__ReturnIvrPortInfo);
		for (rc = MainIvrTable_c::GetMainIvrTable()->getFirstForGroupOnline(ns1__RequestIvrPortInfo->Id[i],&prec);
			 rc == ARC_OK;
			 rc = MainIvrTable_c::GetMainIvrTable()->getNextForGroupOnline(&prec)) {				  			
				 ns1__ReturnIvrPortInfo->responceInfoHeader->errorCause = (unsigned int)0;
				 ReplyObj.MakeIvrGrpPortInfo(this,prec,ns1__ReturnIvrPortInfo->returnArrayOfBlocks[i]);

			}
	}
return SOAP_OK;
}
int CsaWebServer::IvrGrpInfo(ns1__GenInfoRequestType *ns1__RequestIvrGrpInfo, ns1__ReturnArrayDataType *ns1__ReturnIvrGrpInfo)
{
      IVRGrpPortReply ReplyObj;
	  CsaRc_t                       rc = ARC_OK;   
	  CsaIvrGroupRecord_c *ivr_grp_rec = NULL;

   //Fool protector
	if(ns1__RequestIvrGrpInfo == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestIvrGrpInfo", "ns1__RequestIvrGrpInfo is NULL");



      ns1__ReturnIvrGrpInfo->responceInfoHeader = ResponceHeader; 
	  ns1__ReturnIvrGrpInfo->responceInfoHeader->errorCause = (unsigned int)1;
      ns1__ReturnIvrGrpInfo->responceInfoHeader->serviceReqId = 
		  ns1__RequestIvrGrpInfo->requestInfoHeader->serviceReqId;
	
	  if(!TicketTab.VerifyTicket(ns1__RequestIvrGrpInfo->requestInfoHeader->ticket))
		{
			ns1__ReturnIvrGrpInfo->responceInfoHeader->errorCause = 500;
			return SOAP_OK;
		}
	 

	  for (unsigned int i=0; i<ns1__RequestIvrGrpInfo->Id.size(); i++){
	   ReplyObj.StartMonitorGroup(ns1__RequestIvrGrpInfo->Id[i]); //PVL_WEB 3-8-2009 ivr_group_id
       rc = MainIvrTable_c::GetMainIvrTable()->GetOnlineGroupsTable()->get(ns1__RequestIvrGrpInfo->Id[i],&ivr_grp_rec);
	   if (rc == ARC_OK){
		   ns1__ReturnIvrGrpInfo->responceInfoHeader->errorCause = (unsigned int)0;
		   ReplyObj.AddNewIvrGrpBlock(this,ivr_grp_rec->m_ivr_port_grp_id,ns1__ReturnIvrGrpInfo);
           ReplyObj.MakeIvrGrpInfo(this,ivr_grp_rec,ns1__ReturnIvrGrpInfo->returnArrayOfBlocks[i]);
	   }
	     
	  }
	return SOAP_OK;
}




//-------------------------------------------------------------------------------------------------
// Web service operation 'IvrPortGroupList' (returns error code or SOAP_OK)
int CsaWebServer::IvrPortGroupList(ns1__GenRequestType *ns1__RequestIvrPortGroupList, ns1__GenListReturnType *ns1__ReturnIvrPortGroupList)
{
	IVRGrpPortReply ReplyObj;
   //Fool protector
	if(ns1__RequestIvrPortGroupList == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestIvrPortGroupList", "ns1__RequestIvrPortGroupList is NULL");

    
    ns1__ReturnIvrPortGroupList->responceInfoHeader = ResponceHeader;
	ns1__ReturnIvrPortGroupList->responceInfoHeader->errorCause = 1;
	ns1__ReturnIvrPortGroupList->responceInfoHeader->serviceReqId =
	                      	ns1__RequestIvrPortGroupList->requestInfoHeader->serviceReqId;

	if(!TicketTab.VerifyTicket(ns1__RequestIvrPortGroupList->requestInfoHeader->ticket))
	{
		ns1__ReturnIvrPortGroupList->responceInfoHeader->errorCause = 500;
		return SOAP_OK;
	}
	
	this->ivrgrptab.LoadList();
	
	for ( this->ivrgrptab.it=this->ivrgrptab.tab.begin() ; 
		   this->ivrgrptab.it != this->ivrgrptab.tab.end(); 
		   this->ivrgrptab.it++ )
	 {
		 ns1__ReturnIvrPortGroupList->responceInfoHeader->errorCause = 0;
		 ReplyObj.InsertIVRGrpPortList(this,
			              this->ivrgrptab.it->first,
						  this->ivrgrptab.it->second,
			              ns1__ReturnIvrPortGroupList);	
	 }

return SOAP_OK;
}
//-------------------------------------------------------------------------------------------------
// Web service operation 'IvrAppList' (returns error code or SOAP_OK)
int CsaWebServer::IvrAppList(ns1__GenRequestType *ns1__RequestIvrAppList, ns1__GenListReturnType *ns1__ReturnIvrAppList)
{
	IVRApplicationReply ReplyObj;
   //Fool protector
	if(ns1__RequestIvrAppList == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestIvrAppList", "ns1__RequestIvrAppList is NULL");

    ns1__ReturnIvrAppList->responceInfoHeader = ResponceHeader;
	ns1__ReturnIvrAppList->responceInfoHeader->errorCause = 1;
	ns1__ReturnIvrAppList->responceInfoHeader->serviceReqId =
	                      	ns1__RequestIvrAppList->requestInfoHeader->serviceReqId;

	if(!TicketTab.VerifyTicket(ns1__RequestIvrAppList->requestInfoHeader->ticket))
	{
		ns1__ReturnIvrAppList->responceInfoHeader->errorCause = 500;
		return SOAP_OK;
	}
	this->ivrapptab.LoadList();

	for ( this->ivrapptab.it=this->ivrapptab.tab.begin() ; 
		   this->ivrapptab.it != this->ivrapptab.tab.end(); 
		   this->ivrapptab.it++ )
	 {
		 ns1__ReturnIvrAppList->responceInfoHeader->errorCause = 0;
		   InsertGenListDataItemType(
			              this->ivrapptab.it->first,
						  this->ivrapptab.it->second,
			              ns1__ReturnIvrAppList);	
	 }

return SOAP_OK;
}
//////////////////
int CsaWebServer::MailboxInfo(ns1__GenInfoRequestType *ns1__RequestMailboxInfo, ns1__ReturnArrayDataType *ns1__ReturnMailboxInfo)
{
	  MAILbox_Reply ReplyObj;
	  CsaRc_t                       rc = ARC_OK;   
	  MailboxRecord *mailbox_rec = NULL;
   //Fool protector
	if(ns1__RequestMailboxInfo == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestMailboxInfo", "ns1__RequestMailboxInfo is NULL");
	

      ns1__ReturnMailboxInfo->responceInfoHeader = ResponceHeader; 
	  ns1__ReturnMailboxInfo->responceInfoHeader->errorCause = (unsigned int)1;
      ns1__ReturnMailboxInfo->responceInfoHeader->serviceReqId =
		  ns1__RequestMailboxInfo->requestInfoHeader->serviceReqId;
	
		if(!TicketTab.VerifyTicket(ns1__RequestMailboxInfo->requestInfoHeader->ticket))
		{
			ns1__ReturnMailboxInfo->responceInfoHeader->errorCause = 500;
			return SOAP_OK;
		}
	  
	  for (unsigned int i=0; i<ns1__RequestMailboxInfo->Id.size(); i++){
	   ReplyObj.StartMonitor(ns1__RequestMailboxInfo->Id[i]);//PVL_WEB 3-8-2009 mailbox_id
       rc = MainMailboxTable::getMailboxTable().GetMailbox(MailboxesTable::MB_TBL_ONLINE,ns1__RequestMailboxInfo->Id[i],&mailbox_rec);
	   if (rc == ARC_OK){
		   ns1__ReturnMailboxInfo->responceInfoHeader->errorCause = (unsigned int)0;
		   ReplyObj.AddNewMailAccBlock(this,mailbox_rec->Mailbox_id,ns1__ReturnMailboxInfo);
		   ReplyObj.MakeMailboxInfo(this,mailbox_rec,ns1__ReturnMailboxInfo->returnArrayOfBlocks[i]);
	   }
	     
	  }

		
return SOAP_OK;
}
/// Web service operation 'MailAccList' (returns error code or SOAP_OK)
int CsaWebServer::MailAccList(ns1__GenRequestType *ns1__RequestMailAccList, ns1__GenListReturnType *ns1__ReturnMailAccList)
{
	MAILbox_Reply ReplyObj;

   //Fool protector
	if(ns1__RequestMailAccList == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestMailAccList", "ns1__RequestMailAccList is NULL");


    ns1__ReturnMailAccList->responceInfoHeader = ResponceHeader;
	ns1__ReturnMailAccList->responceInfoHeader->errorCause = 1;
	ns1__ReturnMailAccList->responceInfoHeader->serviceReqId =
	                      	ns1__RequestMailAccList->requestInfoHeader->serviceReqId;

	if(!TicketTab.VerifyTicket(ns1__RequestMailAccList->requestInfoHeader->ticket))
		{
			ns1__ReturnMailAccList->responceInfoHeader->errorCause = 500;
			return SOAP_OK;
		}	
	
	this->mailboxacctab.LoadList();
	
	for ( this->mailboxacctab.it=this->mailboxacctab.tab.begin() ; 
		   this->mailboxacctab.it != this->mailboxacctab.tab.end(); 
		   this->mailboxacctab.it++ )
	 {
		 ns1__ReturnMailAccList->responceInfoHeader->errorCause = 0;
		 ReplyObj.InsertMAILboxAccList(this,
										this->mailboxacctab.it->first,
										this->mailboxacctab.it->second,
										ns1__ReturnMailAccList);	
	 }

	return SOAP_OK;
}

/// Web service operation 'CommandAgentLoginLogout' (returns error code or SOAP_OK)
int CsaWebServer::CommandAgentLoginLogout(ns1__ACDMultiRequestType *ns1__LoginLogoutRequestAcd, ns1__ResponceInfoHeaderType *ns1__NoDataResponce)
{
   //Fool protector
	if(ns1__LoginLogoutRequestAcd == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__LoginLogoutRequestAcd", "ns1__LoginLogoutRequestAcd is NULL");

	return LoginManagerOneAgentToMultiGroups(ns1__LoginLogoutRequestAcd,ns1__NoDataResponce);
 
}

/// Web service operation 'CommandAgentReleaseResume' (returns error code or SOAP_OK)
int CsaWebServer::CommandAgentReleaseResume(ns1__ACDRequestType *ns1__ReleaseResumeRequestAcd, ns1__ResponceInfoHeaderType *ns1__NoDataResponce)
{   
    CsaRc_t                       rc = ARC_OK; 
	   //Fool protector
	if(ns1__ReleaseResumeRequestAcd == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__ReleaseResumeRequestAcd", "ns1__ReleaseResumeRequestAcd is NULL");

	ns1__NoDataResponce->errorCause = 0;
	ns1__NoDataResponce->serviceReqId =    ns1__ReleaseResumeRequestAcd->requestInfoHeader->serviceReqId;

	if(!this->TicketTab.VerifyTicket(ns1__ReleaseResumeRequestAcd->requestInfoHeader->ticket))
	{
		ns1__NoDataResponce->errorCause =500; 
		return SOAP_OK;
	}
	if(ns1__ReleaseResumeRequestAcd->Operation == "RELEASE")
	{
		NlaCsaAgentReleaseReq_t notify_data;
		notify_data.agent_db_id = ns1__ReleaseResumeRequestAcd->AgentId;
		notify_data.group_id = ns1__ReleaseResumeRequestAcd->GroupId;
		
		CsaUtilSendEvent(CSA_TH3,CSA_release_request,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
		return SOAP_OK;
	}
	if(ns1__ReleaseResumeRequestAcd->Operation == "RESUME")
	{
		NlaCsaAgentResumeReq_t notify_data;
		notify_data.agent_db_id = ns1__ReleaseResumeRequestAcd->AgentId;
		notify_data.group_id = ns1__ReleaseResumeRequestAcd->GroupId;

		CsaUtilSendEvent(CSA_TH3,CSA_resume_request,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
		return SOAP_OK;
	}
	else 
       ns1__NoDataResponce->errorCause = 1;
	return SOAP_OK;}

/// Web service operation 'CommandAgentStartChat' (returns error code or SOAP_OK)
int CsaWebServer::CommandAgentStartChat(ns1__TFRequestType *ns1__StartChatRequestAcd, ns1__ChatResponceType *ns1__StartChatResponce)
{    
    CsaRc_t                       rc = ARC_OK; 
   //Fool protector
	if(ns1__StartChatRequestAcd == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__StartChatRequestAcd", "ns1__StartChatRequestAcd is NULL");

	ns1__StartChatResponce->errorCause = 0;
	ns1__StartChatResponce->serviceReqId =    ns1__StartChatRequestAcd->requestInfoHeader->serviceReqId;
	
	
	sup_data_t *tmp_sup_data;
	
	if(!this->TicketTab.VerifyTicketGetData(ns1__StartChatRequestAcd->requestInfoHeader->ticket,
		                    &tmp_sup_data))
	{
		ns1__StartChatResponce->errorCause =500; 
		return SOAP_OK;
	}
    if(ns1__StartChatRequestAcd->Operation == "START_CHAT")
	{
		NlaCsaAgentChatReq_t notify_data;
		notify_data.agent_db_id = ns1__StartChatRequestAcd->AgentId;
		notify_data.group_id = tmp_sup_data->agent_id; //sup user id  
		
		char    temp[20] = "";	
		ns1__StartChatResponce->sessionId += "admin";
		er_itoa(tmp_sup_data->agent_id,temp,10);
		ns1__StartChatResponce->sessionId +=temp; 
		ns1__StartChatResponce->sessionId += "_";
		er_itoa(ns1__StartChatRequestAcd->AgentId,temp,10);
		ns1__StartChatResponce->sessionId +=temp; 
		 

		CsaUtilSendEvent(CSA_TH3,CSA_chat_request,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
		return SOAP_OK;
	}
	else 
       ns1__StartChatResponce->errorCause = 1;
	return SOAP_OK;
}

/// Web service operation 'CommandAgentStartRecord' (returns error code or SOAP_OK)
int CsaWebServer::CommandAgentStartRecord(ns1__TFRequestType *ns1__StartRecordRequestAcd, ns1__ResponceInfoHeaderType *ns1__NoDataResponce)
{    
    CsaRc_t                       rc = ARC_OK; 
   //Fool protector
	if(ns1__StartRecordRequestAcd == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__StartRecordRequestAcd", "ns1__StartRecordRequestAcd is NULL");

	ns1__NoDataResponce->errorCause = 0;
	ns1__NoDataResponce->serviceReqId =    ns1__StartRecordRequestAcd->requestInfoHeader->serviceReqId;
	sup_data_t *tmp_sup_data;
	if(!this->TicketTab.VerifyTicketGetData(ns1__StartRecordRequestAcd->requestInfoHeader->ticket, &tmp_sup_data))
	{
		ns1__NoDataResponce->errorCause =500; 
		return SOAP_OK;
	}

/* V6 ONLY

	if(ns1__StartRecordRequestAcd->Operation == "START_RECORD")
	{
		NlaCsaAgentLogoutReq_t notify_data;
		notify_data.agent_db_id = ns1__RequestAcd->AgentId; //Agent to record
				
		CsaUtilSendEvent(CSA_TH3,CSA_startrecord_request,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
		return SOAP_OK;
	}
	else 
       ns1__NoDataResponce->errorCause = 1;
V6*/
	return SOAP_OK;
}

/// Web service operation 'CommandAgentTelephonyFeatures' (returns error code or SOAP_OK)
int CsaWebServer::CommandAgentTelephonyFeatures(ns1__TFRequestType *ns1__RequestTf, ns1__ResponceInfoHeaderType *ns1__NoDataResponce)
{    
    CsaRc_t                       rc = ARC_OK; 
   //Fool protector
	if(ns1__RequestTf == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestTf", "ns1__RequestTf is NULL");

	ns1__NoDataResponce->errorCause = 0;
	ns1__NoDataResponce->serviceReqId =    ns1__RequestTf->requestInfoHeader->serviceReqId;
	sup_data_t *tmp_sup_data;
	if(!this->TicketTab.VerifyTicketGetData(ns1__RequestTf->requestInfoHeader->ticket, &tmp_sup_data))
	{
		ns1__NoDataResponce->errorCause =500; 
		return SOAP_OK;
	}
/* V6 ONLY
    AgentRecord*                                   agent = NULL;	
	rc = Csa_Agents_Tbls[TBL_ONLINE]->getByDataBaseId(AgentId,&agent);
    if (rc == ARC_OK){
            notify_data.etas_agent_ext = agent->Device;
			}
    else {
		ns1__NoDataResponce->errorCause =501; 
		return SOAP_OK;
	}

    if(ns1__RequestTf->Operation == "BARGE_IN")
	{
		NlaCsaAgentBargeInReq_t notify_data;
		
		notify_data.dest_agent_db_id = ns1__RequestTf->AgentId;
		
		
		CsaUtilSendEvent(CSA_TH3,CSA_agent_bargein_req,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
		return SOAP_OK;
	}
	if(ns1__RequestTf->Operation == "WHISPER")
	{
		NlaCsaAgentWhisperReq_t notify_data;
		notify_data.dest_agent_db_id = ns1__RequestTf->AgentId;
				
		CsaUtilSendEvent(CSA_TH3,CSA_agent_whisper_req,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
		return SOAP_OK;
	}
	if(ns1__RequestTf->Operation == "SILENT_MONITOR")
	{
		NlaCsaAgentSilentmonitorReq_t notify_data;
		notify_data.dest_agent_db_id = ns1__RequestTf->AgentId;
				
		CsaUtilSendEvent(CSA_TH3,CSA_agent_silentmonitor_req;,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
		return SOAP_OK;
	}
	else 
       ns1__NoDataResponce->errorCause = 1;
   V6*/
	return SOAP_OK;
}

/// Web service operation 'SaveWorkspase' (returns error code or SOAP_OK)
int CsaWebServer::SaveWorkspase(ns1__SaveWorkspaseRequestType *ns1__RequestSaveWorkspase, ns1__ResponceInfoHeaderType *ns1__NoDataResponce)
{
	ReportsReply ReplyObj;
    CsaRc_t                       rc = ARC_OK; 
   //Fool protector
	if(ns1__RequestSaveWorkspase == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestSaveWorkspase", "ns1__RequestSaveWorkspase is NULL");

	ns1__NoDataResponce->errorCause = 0;
	ns1__NoDataResponce->serviceReqId =    ns1__RequestSaveWorkspase->requestInfoHeader->serviceReqId;

	if(!this->TicketTab.VerifyTicket(ns1__RequestSaveWorkspase->requestInfoHeader->ticket))
	{
		ns1__NoDataResponce->errorCause =500; 
		return SOAP_OK;
	}
    
	if(ReplyObj.SaveWorkspase(this,ns1__RequestSaveWorkspase)){
          ns1__NoDataResponce->errorCause = 1;
	}
	return SOAP_OK;
}

/// Web service operation 'SaveReport' (returns error code or SOAP_OK)
int CsaWebServer::SaveReport(ns1__RequestSaveReportType *ns1__RequestSaveReport, ns1__ResponceSaveReportType *ns1__ResponceSaveReport)
{
	ReportsReply ReplyObj;
	CsaRc_t                       rc = ARC_OK; 
	int res;
   //Fool protector
	if(ns1__RequestSaveReport == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestSaveReport", "ns1__RequestSaveReport is NULL");

	ns1__ResponceSaveReport->errorCause = 0;
	ns1__ResponceSaveReport->serviceReqId =    ns1__RequestSaveReport->requestInfoHeader->serviceReqId;
	if(!this->TicketTab.VerifyTicket(ns1__RequestSaveReport->requestInfoHeader->ticket))
	{
		ns1__ResponceSaveReport->errorCause =500; 
		return SOAP_OK;
	}
	res= ReplyObj.SaveReport(this,ns1__RequestSaveReport,ns1__ResponceSaveReport->reportId);
	if(res > 0){
          ns1__ResponceSaveReport->errorCause = res;
	}
	return SOAP_OK;
}

/// Web service operation 'DeleteReport' (returns error code or SOAP_OK)
int CsaWebServer::DeleteReport(ns1__RequestDeleteReportType *ns1__RequestDeleteReport, ns1__ResponceInfoHeaderType *ns1__NoDataResponce)
{
	ReportsReply ReplyObj;
	CsaRc_t                       rc = ARC_OK; 
   //Fool protector
	if(ns1__RequestDeleteReport == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__RequestDeleteReport", "ns1__RequestDeleteReport is NULL");

   ns1__NoDataResponce->errorCause = 0;
   ns1__NoDataResponce->serviceReqId =    ns1__RequestDeleteReport->requestInfoHeader->serviceReqId;

   if(!this->TicketTab.VerifyTicket(ns1__RequestDeleteReport->requestInfoHeader->ticket))
	{
		ns1__NoDataResponce->errorCause =500; 
		return SOAP_OK;
	}

	if(ReplyObj.DeleteReport(this,ns1__RequestDeleteReport)){
          ns1__NoDataResponce->errorCause = 1;
	}
	return SOAP_OK;
}

/// Web service operation 'LoginManagerAgentsPerGroup' (returns error code or SOAP_OK)
int CsaWebServer::LoginManagerAgentsPerGroup(ns1__LoginManagerRequestType *ns1__LoginManagerRequest, ns1__GenListReturnType *ns1__LoginManagerResponse)
{

	AgentRecord*                                   agent = NULL; 
	CsaAgentPerGroupRecord_t*                      agent_per_group = NULL;
	Uint_t                                         token;
	CsaRc_t rc;
	CsaRc_t rc2;
	GenId_t						group_id;
   //Fool protector
	if(ns1__LoginManagerRequest == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__LoginManagerRequest", "ns1__LoginManagerRequest is NULL");

	ns1__LoginManagerResponse->responceInfoHeader = ResponceHeader;
	ns1__LoginManagerResponse->responceInfoHeader->errorCause = (unsigned int)103; //AGENTS not found
	ns1__LoginManagerResponse->responceInfoHeader->serviceReqId = ns1__LoginManagerRequest->requestInfoHeader->serviceReqId;

	if(!this->TicketTab.VerifyTicket(ns1__LoginManagerRequest->requestInfoHeader->ticket))
	{
		ns1__LoginManagerResponse->responceInfoHeader->errorCause =500; 
		return SOAP_OK;
	}
	group_id = ns1__LoginManagerRequest->Id;
	//OPTIONAL , DELETE FOR PERFORMANCE 
	this->groupstab.LoadGroupList();
    
	if (this->groupstab.IsGroupExist(group_id)){
		// cycle through login records  by GroupID
		for (rc = Csa_Agents_Tbls[TBL_ONLINE]->getFirstLoginForGroup( group_id, //Group_ID
			                                                         &agent_per_group,token) ;
				 rc == ARC_OK;
				 rc = Csa_Agents_Tbls[TBL_ONLINE]->getNextLoginForGroup(&agent_per_group,token) )
			{
			ns1__LoginManagerResponse->responceInfoHeader->errorCause = (unsigned int)0;
					// if agent logged in, add full info for agent to event
				if (agent_per_group->Logged_in) 
				{
				  rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getByDataBaseId(agent_per_group->Agent_id,&agent);
			      // put Record of Group per Agent
					if (rc2 == ARC_OK){
						InsertGenListDataItemType(	agent_per_group->Agent_id,
													agent->Name,
													ns1__LoginManagerResponse);
					}

				}
				
			} //End of All groups per Agent 
			

	} // If agent FOUND
	else 
	{ //Agent not Found , so show LogOut 
	   
		ns1__LoginManagerResponse->responceInfoHeader->errorCause = (unsigned int)102; //Group NOT FOUND  
		 
	}	
	return SOAP_OK;
}

/// Web service operation 'LoginManagerGroupsPerAgent' (returns error code or SOAP_OK)
int CsaWebServer::LoginManagerGroupsPerAgent(ns1__LoginManagerRequestType *ns1__LoginManagerRequest, ns1__GenListReturnType *ns1__LoginManagerResponse)
{

	AgentRecord*                                   agent = NULL;
	CsaAgentPerGroupRecord_t*                      agent_per_group = NULL;
	Uint_t                                         token;
	CsaRc_t rc;
	CsaRc_t rc2;
   //Fool protector
	if(ns1__LoginManagerRequest == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__LoginManagerRequest", "ns1__LoginManagerRequest is NULL");

	ns1__LoginManagerResponse->responceInfoHeader = ResponceHeader;
	ns1__LoginManagerResponse->responceInfoHeader->errorCause = (unsigned int)102;
	ns1__LoginManagerResponse->responceInfoHeader->serviceReqId = ns1__LoginManagerRequest->requestInfoHeader->serviceReqId;
	
	if(!this->TicketTab.VerifyTicket(ns1__LoginManagerRequest->requestInfoHeader->ticket))
	{
		ns1__LoginManagerResponse->responceInfoHeader->errorCause =500; 
		return SOAP_OK;
	}
	//OPTIONAL , DELETE FOR PERFORMANCE 
	this->groupstab.LoadGroupList();

	rc = Csa_Agents_Tbls[TBL_ONLINE]->getByDataBaseId(ns1__LoginManagerRequest->Id,&agent);
	if (rc == ARC_OK){
		// cycle through login records
		for (
			rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getFirstLoginForAgent(agent->Internal_id, &agent_per_group, token);
			rc2 == ARC_OK;
			rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getNextLoginForAgent(&agent_per_group, token)
			)
			{
			ns1__LoginManagerResponse->responceInfoHeader->errorCause = (unsigned int)0;
					// if agent logged in, add full info for agent to event
				if (agent_per_group->Logged_in) 
				{
			      // put Record of Group per Agent
					InsertGenListDataItemType(	agent_per_group->Group_id,
								this->groupstab.GetGroupNameById(agent_per_group->Group_id),
								ns1__LoginManagerResponse);

				}
				
			} //End of All groups per Agent 
			

	} // If agent FOUND
	else 
	{ //Agent not Found , so show LogOut 
	   
		ns1__LoginManagerResponse->responceInfoHeader->errorCause = (unsigned int)103; //AGENT NOT FOUND  
		 
	}
	return SOAP_OK;
}

	/// Web service operation 'LoginManagerOneAgentToMultiGroups' (returns error code or SOAP_OK)
int CsaWebServer::LoginManagerOneAgentToMultiGroups(ns1__ACDMultiRequestType *ns1__LoginLogoutRequestAcd, ns1__ResponceInfoHeaderType *ns1__NoDataResponce)
{
	CsaRc_t                       rc = ARC_OK; 
   //Fool protector
	if(ns1__LoginLogoutRequestAcd == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__LoginLogoutRequestAcd", "ns1__LoginLogoutRequestAcd is NULL");

	ns1__NoDataResponce->errorCause = 0;
	ns1__NoDataResponce->serviceReqId =    ns1__LoginLogoutRequestAcd->requestInfoHeader->serviceReqId;

	
	if(!this->TicketTab.VerifyTicket(ns1__LoginLogoutRequestAcd->requestInfoHeader->ticket))
	{
		ns1__NoDataResponce->errorCause =500; 
		return SOAP_OK;
	}
	if(ns1__LoginLogoutRequestAcd->Operation == "LOGIN")
	{	
		NlaCsaAgentLoginReq_t notify_data ;
		for (unsigned int i=0; i<ns1__LoginLogoutRequestAcd->multiId.size(); i++)
		{
			notify_data.agent_db_id =ns1__LoginLogoutRequestAcd->Id ;
			notify_data.group_id = ns1__LoginLogoutRequestAcd->multiId[i];
			CsaUtilSendEvent(CSA_TH3,CSA_logged_in_request,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
		}
		return SOAP_OK;
       
	}
	if(ns1__LoginLogoutRequestAcd->Operation == "LOGOUT")
	{
		NlaCsaAgentLogoutReq_t notify_data;
		for (unsigned int i=0; i<ns1__LoginLogoutRequestAcd->multiId.size(); i++)
		{
			notify_data.agent_db_id = ns1__LoginLogoutRequestAcd->Id;
			notify_data.group_id = ns1__LoginLogoutRequestAcd->multiId[0];    
			CsaUtilSendEvent(CSA_TH3,CSA_logged_out_request,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
		}
		return SOAP_OK;
	}
	else 
       ns1__NoDataResponce->errorCause = 1;
	return SOAP_OK;
}

	/// Web service operation 'LoginManagerMultiAgentsToOneGroup' (returns error code or SOAP_OK)
int CsaWebServer::LoginManagerMultiAgentsToOneGroup(ns1__ACDMultiRequestType *ns1__LoginLogoutRequestAcd, ns1__ResponceInfoHeaderType *ns1__NoDataResponce)
{
	CsaRc_t                       rc = ARC_OK; 
   //Fool protector
	if(ns1__LoginLogoutRequestAcd == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__LoginLogoutRequestAcd", "ns1__LoginLogoutRequestAcd is NULL");

	ns1__NoDataResponce->errorCause = 0;
	ns1__NoDataResponce->serviceReqId =    ns1__LoginLogoutRequestAcd->requestInfoHeader->serviceReqId;

	
	if(!this->TicketTab.VerifyTicket(ns1__LoginLogoutRequestAcd->requestInfoHeader->ticket))
	{
		ns1__NoDataResponce->errorCause =500; 
		return SOAP_OK;
	}
	if(ns1__LoginLogoutRequestAcd->Operation == "LOGIN")
	{	
		NlaCsaAgentLoginReq_t notify_data ;
		for (unsigned int i=0; i<ns1__LoginLogoutRequestAcd->multiId.size(); i++)
		{
			notify_data.agent_db_id =ns1__LoginLogoutRequestAcd->multiId[i] ;
			notify_data.group_id =   ns1__LoginLogoutRequestAcd->Id ;
			CsaUtilSendEvent(CSA_TH3,CSA_logged_in_request,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
		}
		return SOAP_OK;
       
	}
	if(ns1__LoginLogoutRequestAcd->Operation == "LOGOUT")
	{
		NlaCsaAgentLogoutReq_t notify_data;
		for (unsigned int i=0; i<ns1__LoginLogoutRequestAcd->multiId.size(); i++)
		{
			notify_data.agent_db_id = ns1__LoginLogoutRequestAcd->multiId[0];
			notify_data.group_id = ns1__LoginLogoutRequestAcd->Id ;    
			CsaUtilSendEvent(CSA_TH3,CSA_logged_out_request,CSA_Q3,(char*)&notify_data, sizeof(notify_data));
		}
		return SOAP_OK;
	}
	else 
       ns1__NoDataResponce->errorCause = 1;
	return SOAP_OK;
}


	/// Web service operation 'LoginManagerAvailableAgentsForGroup' (returns error code or SOAP_OK)
int CsaWebServer::LoginManagerAvailableAgentsForGroup(ns1__LoginManagerRequestType *ns1__LoginManagerRequestAvailableAgents, ns1__GenListReturnType *ns1__LoginManagerResponse)
{   
	//Fool protector
	if(ns1__LoginManagerRequestAvailableAgents == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__LoginManagerRequestAvailableAgents", "ns1__LoginManagerRequestAvailableAgents is NULL");

	ns1__LoginManagerResponse->responceInfoHeader = ResponceHeader;
	ns1__LoginManagerResponse->responceInfoHeader->errorCause = (unsigned int)102;
	ns1__LoginManagerResponse->responceInfoHeader->serviceReqId = ns1__LoginManagerRequestAvailableAgents->requestInfoHeader->serviceReqId;
// Test Ticket
	if(!this->TicketTab.VerifyTicket(ns1__LoginManagerRequestAvailableAgents->requestInfoHeader->ticket))
	{
		ns1__LoginManagerResponse->responceInfoHeader->errorCause =500; 
		return SOAP_OK;
	}

if(agentstab.tab.size() >0)
	for(agentstab.it=agentstab.tab.begin(); agentstab.it!= agentstab.tab.end(); ++agentstab.it) 
	{
	//Test , is Agent Logged in Another Group
		if(IsAgentLoggedOnlyToAnotherGroup((*agentstab.it).second.a_id, //Agent Id
						                ns1__LoginManagerRequestAvailableAgents->Id //Group Id
										))
		{
			ns1__LoginManagerResponse->responceInfoHeader->errorCause  = 0;
			InsertGenListDataItemType(	(*agentstab.it).second.a_id,
										(*agentstab.it).second.a_name,
										ns1__LoginManagerResponse);
		}
	}

	return SOAP_OK;
}
//////////////////////////////////////////////////////////////////////////////
bool CsaWebServer::IsAgentLoggedOnlyToAnotherGroup(unsigned int AgentId,unsigned int GroupId )
{
	AgentRecord*                                   agent = NULL;
	CsaAgentPerGroupRecord_t*                      agent_per_group = NULL;
	Uint_t                                         token;
	bool FoundInTheGroup =false;
	bool FoundInAnotherGroup= false;
	CsaRc_t rc;
	CsaRc_t rc2;

        BriefAgentInfo TotalsAgentInfo;		
		rc = Csa_Agents_Tbls[TBL_ONLINE]->getByDataBaseId(AgentId,&agent);
		if (rc == ARC_OK){
			// cycle through login records
			for (
				rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getFirstLoginForAgent(agent->Internal_id, &agent_per_group, token);
				rc2 == ARC_OK;
				rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getNextLoginForAgent(&agent_per_group, token)
				)					   
				if (agent_per_group->Logged_in) 
				{ // if agent logged in, Test GroupId 
					if(GroupId == agent_per_group->Group_id  )
						FoundInTheGroup = true;
					else
						FoundInAnotherGroup = true;			
				}                       
				
		} // If agent FOUND
		else 
		{ //Agent not Found ,
		return false;
		}
	if(!FoundInTheGroup && FoundInAnotherGroup)
		return true;
	else
		return false;

}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool CsaWebServer::IsAgentNotLoggedToGroup(unsigned int AgentId,unsigned int GroupId )
{
	AgentRecord*                                   agent = NULL;
	CsaAgentPerGroupRecord_t*                      agent_per_group = NULL;
	Uint_t                                         token;
	bool FoundInTheGroup =false;
	bool FoundInAnotherGroup= false;
	CsaRc_t rc;
	CsaRc_t rc2;

        BriefAgentInfo TotalsAgentInfo;		
		rc = Csa_Agents_Tbls[TBL_ONLINE]->getByDataBaseId(AgentId,&agent);
		if (rc == ARC_OK){
			// cycle through login records
			for (
				rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getFirstLoginForAgent(agent->Internal_id, &agent_per_group, token);
				rc2 == ARC_OK;
				rc2 = Csa_Agents_Tbls[TBL_ONLINE]->getNextLoginForAgent(&agent_per_group, token)
				)					   
				if (agent_per_group->Logged_in) 
				{ // if agent logged in, Test GroupId 
					if(GroupId == agent_per_group->Group_id  )
						FoundInTheGroup = true;
					else
						FoundInAnotherGroup = true;			
				}                       
				
		} // If agent FOUND
		else 
		{ //Agent not Found ,
		return true;
		}
	if(FoundInTheGroup)
		return false;
	else
		return true;
		

}

//////////////////////////////////////////////////////////////////////////////
	/// Web service operation 'LoginManagerAvailableGroupsForAgent' (returns error code or SOAP_OK)
int CsaWebServer::LoginManagerAvailableGroupsForAgent(ns1__LoginManagerRequestType *ns1__LoginManagerRequestAvailableGroups, ns1__GenListReturnType *ns1__LoginManagerResponse)
{
	
   //Fool protector
	if(ns1__LoginManagerRequestAvailableGroups == NULL)
		return soap_receiver_fault(this, "No Request Data in ns1__LoginManagerRequestAvailableGroups", "ns1__LoginManagerRequestAvailableGroups is NULL");


	ns1__LoginManagerResponse->responceInfoHeader = ResponceHeader;
	ns1__LoginManagerResponse->responceInfoHeader->errorCause = (unsigned int)102;
	ns1__LoginManagerResponse->responceInfoHeader->serviceReqId = ns1__LoginManagerRequestAvailableGroups->requestInfoHeader->serviceReqId;
// Test Ticket
	if(!this->TicketTab.VerifyTicket(ns1__LoginManagerRequestAvailableGroups->requestInfoHeader->ticket))
	{
		ns1__LoginManagerResponse->responceInfoHeader->errorCause =500; 
		return SOAP_OK;
	}
	

	if(this->groupstab.tab.size() > 0)
		for(groupstab.it=groupstab.tab.begin(); groupstab.it!= groupstab.tab.end(); ++groupstab.it) 
			if(IsAgentNotLoggedToGroup(ns1__LoginManagerRequestAvailableGroups->Id, ////Agent Id
									   (*groupstab.it).first                        //Group Id
											))
			{
				ns1__LoginManagerResponse->responceInfoHeader->errorCause  = 0;
				InsertGenListDataItemType(	(*groupstab.it).first, //id
											(*groupstab.it).second.grpName,//name
											ns1__LoginManagerResponse);
			}



	return SOAP_OK;
}
