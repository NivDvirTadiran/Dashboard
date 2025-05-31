#ifdef WIN32
#include "windows/soapEMISwebBindingService.h"
#else
#include "linux/soapEMISwebBindingService.h"
#endif


int  EMISwebBindingService::GroupDetailFullInfo(ns1__GenInfoRequestType *ns1__RequestGroupDetailFullInfo, ns1__ReturnArrayDataType &ns1__ReturnGroupDetailFullInfo){return SOAP_OK;}
/// Web service operation 'GroupEntityUpdateQueInfo' (returns error code or SOAP_OK)
int EMISwebBindingService::GroupQueInfo(ns1__GenInfoRequestType *ns1__RequestGroupQueInfo, ns1__ReturnArrayDataType &ns1__ReturnGroupQueInfo){return SOAP_OK;}
/// Web service operation 'GroupEntityUpdateAgentsInfo' (returns error code or SOAP_OK)
int  EMISwebBindingService::GroupAgentsInfo(ns1__GenInfoRequestType *ns1__RequestGroupAgentsInfo, ns1__ReturnArrayDataType &ns1__ReturnGroupAgentsInfo){return SOAP_OK;}
/// Web service operation 'GroupEntityUpdateAbndInfo' (returns error code or SOAP_OK)
int  EMISwebBindingService::GroupAbndInfo(ns1__GenInfoRequestType *ns1__RequestGroupAbndInfo, ns1__ReturnArrayDataType &ns1__ReturnGroupAbndInfo){return SOAP_OK;}
/// Web service operation 'GroupEntityUpdateIntOvfInfo' (returns error code or SOAP_OK)
int  EMISwebBindingService::GroupIntOvfInfo(ns1__GenInfoRequestType *ns1__RequestGroupIntOvfInfo, ns1__ReturnArrayDataType &ns1__ReturnGroupIntOvfInfo){return SOAP_OK;}
/// Web service operation 'GroupEntityUpdateShiftInfo' (returns error code or SOAP_OK)
int  EMISwebBindingService::GroupShiftInfo(ns1__GenInfoRequestType *ns1__RequestGroupShiftInfo, ns1__ReturnArrayDataType &ns1__ReturnGroupShiftInfo){return SOAP_OK;}
/// Web service operation 'GroupEntityUpdateGeneral' (returns error code or SOAP_OK)
int  EMISwebBindingService::GroupGeneralInfo(ns1__GenInfoRequestType *ns1__RequestGroupGeneralInfo, ns1__ReturnArrayDataType &ns1__ReturnGroupGeneralInfo){return SOAP_OK;}
/// Web service operation 'GroupEntityTimerHandler' (returns error code or SOAP_OK)
int  EMISwebBindingService::GroupPeriodInfo(ns1__GenInfoRequestType *ns1__RequestGroupPeriodInfo, ns1__ReturnArrayDataType &ns1__ReturnGroupPeriodInfo){return SOAP_OK;}
//------------------------------------------------------------------------------------------------
/// Web service operation 'UserLogin' (returns error code or SOAP_OK)
int EMISwebBindingService::UserLogin(ns1__LoginRequestType *ns1__RequestUserLogin, ns1__LoginReturnType &ns1__ReturnUserLogin){return SOAP_OK;}

/// Web service operation 'UserLogout' (returns error code or SOAP_OK)
int EMISwebBindingService::UserLogout(ns1__GenRequestType *ns1__RequestUserLogout, ns1__ResponseInfoHeaderType &ns1__NoDataResponse){return SOAP_OK;}

//------------------------------------------------------------------------------------------------
/// Web service operation 'GroupListForSupervisor' (returns error code or SOAP_OK)
int EMISwebBindingService::GroupList(ns1__GenRequestType *ns1__RequestGroupList, ns1__GroupListReturnType &ns1__ReturnGroupList){return SOAP_OK;}

	/// Web service operation 'IvrPortGroupList' (returns error code or SOAP_OK)
int EMISwebBindingService::IvrPortGroupList(ns1__GenRequestType *ns1__RequestIvrPortGroupList, ns1__GenListReturnType &ns1__ReturnIvrPortGroupList){return SOAP_OK;}

	/// Web service operation 'IvrAppList' (returns error code or SOAP_OK)
int EMISwebBindingService::IvrAppList(ns1__GenRequestType *ns1__RequestIvrAppList, ns1__GenListReturnType &ns1__ReturnIvrAppList){return SOAP_OK;}

//------------------------------------------------------------------------------------------------
int EMISwebBindingService::AgentsPerGroupInfo(ns1__GenInfoRequestType *ns1__RequestGroupAgentsMatrix, ns1__ReturnMatrixDataTypePerGroup &ns1__ReturnGroupAgentsMatrix){return SOAP_OK;}
//------------------------------------------------------------------------------------------------
	/// Web service operation 'AgentsList' (returns error code or SOAP_OK)
int EMISwebBindingService::AgentsList(ns1__GenRequestType *ns1__RequestAgentsList, ns1__AgentsListReturnType &ns1__ReturnAgentsList){return SOAP_OK;}
//------------------------------------------------------------------------------------------------
	/// Web service operation 'BriefAgents' (returns error code or SOAP_OK)
int EMISwebBindingService::BriefAgents(ns1__BriefInfoRequestType *ns1__RequestBriefAgentsMatrix, ns1__ReturnMatrixDataTypeBG &ns1__ReturnBriefAgentsMatrix){return SOAP_OK;}

			/// Web service operation 'DefaultWorkspace' (returns error code or SOAP_OK)
int EMISwebBindingService::DefaultWorkspace(ns1__DefaultWorkspaceRequestType *ns1__RequestDefaultWorkspace, ns1__DefaultWorkspaceReturnType &ns1__ReturnDefaultWorkspace){return SOAP_OK;}

	/// Web service operation 'ReportListData' (returns error code or SOAP_OK)
int EMISwebBindingService::ReportListData(ns1__ReportListDataRequestType *ns1__RequestReportListData, ns1__ReportListDataReturnType &ns1__ReturnReportListData){return SOAP_OK;}

int EMISwebBindingService::VoiceDNISList(ns1__GenRequestType *ns1__RequestVoiceDNISList, ns1__DNISListReturnType &ns1__ReturnVoiceDNISList){return SOAP_OK;}

/// Web service operation 'EmailDNISList' (returns error code or SOAP_OK)
int EMISwebBindingService::EmailDNISList(ns1__GenRequestType *ns1__RequestEmailDNISList, ns1__DNISListReturnType &ns1__ReturnEmailDNISList){return SOAP_OK;}

/// Web service operation 'BriefVoiceDNIS' (returns error code or SOAP_OK)
int EMISwebBindingService::BriefVoiceDNIS(ns1__BriefInfoRequestType *ns1__RequestBriefVoiceDNISMatrix, ns1__ReturnMatrixDataTypeBG &ns1__ReturnBriefVoiceDNISMatrix){return SOAP_OK;}

/// Web service operation 'BriefEmailDNIS' (returns error code or SOAP_OK)
int EMISwebBindingService::BriefEmailDNIS(ns1__BriefInfoRequestType *ns1__RequestBriefEMailDNISMatrix, ns1__ReturnMatrixDataTypeBG &ns1__ReturnBriefEMailDNISMatrix){return SOAP_OK;}

/// Web service operation 'IvrApplInfo' (returns error code or SOAP_OK)
int EMISwebBindingService::IvrApplInfo(ns1__GenInfoRequestType *ns1__RequestIvrApplInfo, ns1__ReturnArrayDataType &ns1__ReturnIvrApplInfo){return SOAP_OK;}

	/// Web service operation 'IvrPortInfo' (returns error code or SOAP_OK)
int EMISwebBindingService::IvrPortInfo(ns1__GenInfoRequestType *ns1__RequestIvrPortInfo, ns1__ReturnArrayDataType &ns1__ReturnIvrPortInfo){return SOAP_OK;}

/// Web service operation 'IvrGrpInfo' (returns error code or SOAP_OK)
int EMISwebBindingService::IvrGrpInfo(ns1__GenInfoRequestType *ns1__RequestIvrGrpInfo, ns1__ReturnArrayDataType &ns1__ReturnIvrGrpInfo){return SOAP_OK;}

/// Web service operation 'MailboxInfo' (returns error code or SOAP_OK)
int EMISwebBindingService::MailboxInfo(ns1__GenInfoRequestType *ns1__RequestMailboxInfo, ns1__ReturnArrayDataType &ns1__ReturnMailboxInfo){return SOAP_OK;}

/// Web service operation 'MailAccList' (returns error code or SOAP_OK)
int EMISwebBindingService::MailAccList(ns1__GenRequestType *ns1__RequestMailAccList, ns1__GenListReturnType &ns1__ReturnMailAccList){return SOAP_OK;}

/// Web service operation 'SuperGroupInfo' (returns error code or SOAP_OK)
int EMISwebBindingService::SuperGroupInfo(ns1__GenInfoRequestType *ns1__RequestSuperGroupInfo, ns1__ReturnArrayDataType &ns1__ReturnSuperGroupInfo){return SOAP_OK;}

/// Web service operation 'SuperGroupList' (returns error code or SOAP_OK)
int EMISwebBindingService::SuperGroupList(ns1__GenRequestType *ns1__RequestSuperGroupList, ns1__SuperGroupListReturnType &ns1__ReturnSuperGroupList){return SOAP_OK;}


/// Web service operation 'CommandAgentLoginLogout' (returns error code or SOAP_OK)
int EMISwebBindingService::CommandAgentLoginLogout(ns1__ACDMultiRequestType *ns1__LoginLogoutRequestAcd,ns1__ResponseInfoHeaderType &ns1__NoDataResponse){return SOAP_OK;}

/// Web service operation 'CommandAgentReleaseResume' (returns error code or SOAP_OK)
int EMISwebBindingService::CommandAgentReleaseResume(ns1__ACDRequestType *ns1__ReleaseResumeRequestAcd, ns1__ResponseInfoHeaderType &ns1__NoDataResponse){return SOAP_OK;}

/// Web service operation 'CommandAgentStartChat' (returns error code or SOAP_OK)
int EMISwebBindingService::CommandAgentStartChat(ns1__TFRequestType *ns1__StartChatRequestAcd, ns1__ChatResponseType &ns1__StartChatResponse){return SOAP_OK;}

/// Web service operation 'CommandAgentStartRecord' (returns error code or SOAP_OK)
int EMISwebBindingService::CommandAgentStartRecord(ns1__TFRequestType *ns1__StartRecordRequestAcd, ns1__ResponseInfoHeaderType &ns1__NoDataResponse){return SOAP_OK;}

/// Web service operation 'CommandAgentTelephonyFeatures' (returns error code or SOAP_OK)
int EMISwebBindingService::CommandAgentTelephonyFeatures(ns1__TFRequestType *ns1__RequestTf, ns1__ResponseInfoHeaderType &ns1__NoDataResponse){return SOAP_OK;}

	/// Web service operation 'SaveWorkspase' (returns error code or SOAP_OK)
int EMISwebBindingService::SaveWorkspase(ns1__SaveWorkspaseRequestType *ns1__RequestSaveWorkspase, ns1__ResponseInfoHeaderType &ns1__NoDataResponse){return SOAP_OK;}

	/// Web service operation 'SaveReport' (returns error code or SOAP_OK)
int EMISwebBindingService::SaveReport(ns1__RequestSaveReportType *ns1__RequestSaveReport,ns1__ResponseSaveReportType &ns1__ResponseSaveReport){return SOAP_OK;}

	/// Web service operation 'DeleteReport' (returns error code or SOAP_OK)
int EMISwebBindingService::DeleteReport(ns1__RequestDeleteReportType *ns1__RequestDeleteReport, ns1__ResponseInfoHeaderType &ns1__NoDataResponse){return SOAP_OK;}

	/// Web service operation 'LoginManagerAgentsPerGroup' (returns error code or SOAP_OK)
int EMISwebBindingService::LoginManagerAgentsPerGroup(ns1__LoginManagerRequestType *ns1__LoginManagerRequest, ns1__GenListReturnType &ns1__LoginManagerResponse){return SOAP_OK;}

	/// Web service operation 'LoginManagerGroupsPerAgent' (returns error code or SOAP_OK)
int EMISwebBindingService::LoginManagerGroupsPerAgent(ns1__LoginManagerRequestType *ns1__LoginManagerRequest, ns1__GenListReturnType &ns1__LoginManagerResponse){return SOAP_OK;}


	/// Web service operation 'LoginManagerOneAgentToMultiGroups' (returns error code or SOAP_OK)
int EMISwebBindingService::LoginManagerOneAgentToMultiGroups(ns1__ACDMultiRequestType *ns1__LoginLogoutRequestAcd, ns1__ResponseInfoHeaderType &ns1__NoDataResponse){return SOAP_OK;}

	/// Web service operation 'LoginManagerMultiAgentsToOneGroup' (returns error code or SOAP_OK)
int EMISwebBindingService::LoginManagerMultiAgentsToOneGroup(ns1__ACDMultiRequestType *ns1__LoginLogoutRequestAcd, ns1__ResponseInfoHeaderType &ns1__NoDataResponse){return SOAP_OK;}


	/// Web service operation 'LoginManagerAvailableAgentsForGroup' (returns error code or SOAP_OK)
int EMISwebBindingService::LoginManagerAvailableAgentsForGroup(ns1__LoginManagerRequestType *ns1__LoginManagerRequestAvailableAgents, ns1__GenListReturnType &ns1__LoginManagerResponse){return SOAP_OK;}

	/// Web service operation 'LoginManagerAvailableGroupsForAgent' (returns error code or SOAP_OK)
int EMISwebBindingService::LoginManagerAvailableGroupsForAgent(ns1__LoginManagerRequestType *ns1__LoginManagerRequestAvailableGroups, ns1__GenListReturnType &ns1__LoginManagerResponse){return SOAP_OK;}

	/// Web service operation 'ApplicationRegister' (returns error code or SOAP_OK)
int EMISwebBindingService::ApplicationRegister(ns1__ApplicationRegisterRequestType *ns1__ApplicationRegisterRequest, ns1__ApplicationReturnType &ns1__ApplicationRegisterResponse){return SOAP_OK;}

	/// Web service operation 'ApplicationKeepAlive' (returns error code or SOAP_OK)
int EMISwebBindingService::ApplicationKeepAlive(ns1__ApplicationKeepAliveRequestType *ns1__ApplicationKeepAliveRequest, ns1__ApplicationReturnType &ns1__ApplicationKeepAliveResponse){return SOAP_OK;}

	/// Web service operation 'TraceInternalStat' (returns error code or SOAP_OK) //PVL_STAT
int  EMISwebBindingService::TraceInternalStat(ns1__StatRequestType *ns1__TraceInternalStatRequest, ns1__StatReturnType &ns1__TraceInternalStatResponse){return SOAP_OK;}//PVL_STAT
	/// Web service operation 'AgentNeedHelp' (returns error code or SOAP_OK)
int  EMISwebBindingService::AgentNeedHelp(ns1__BriefInfoRequestType *ns1__AgentNeedHelpRequest, ns1__ReturnMatrixDataTypeBG &ns1__AgentNeedHelpResponse){return SOAP_OK;}

/// Web service operation 'GroupHourlyInfo' (returns error code or SOAP_OK)
int EMISwebBindingService::GroupHourlyInfo(ns1__GenInfoRequestType *ns1__GroupHourlyInfoRequest, ns1__ReturnMatrixDataTypePerGroup &ns1__GroupHourlyInfoResponse){return SOAP_OK;}

	/// Web service operation 'GroupDailyBriefInfo' (returns error code or SOAP_OK)
int EMISwebBindingService::GroupDailyBriefInfo(ns1__GenInfoRequestType *ns1__GroupDailyBriefInfoRequest, ns1__ReturnArrayDataType &ns1__GroupDailyBriefInfoResponse){return SOAP_OK;}

	/// Web service operation 'GroupDailyDetailedInfo' (returns error code or SOAP_OK)
int EMISwebBindingService::GroupDailyDetailedInfo(ns1__GenInfoRequestType *ns1__GroupDailyDetailedInfoRequest, ns1__ReturnArrayDataType &ns1__GroupDailyDetailedInfoResponse){return SOAP_OK;}

/// Web service operation 'SuperGroupHourlyInfo' (returns error code or SOAP_OK)
int EMISwebBindingService::SuperGroupHourlyInfo(ns1__GenInfoRequestType *ns1__SuperGroupHourlyInfoRequest, ns1__ReturnMatrixDataTypePerSuperGroup &ns1__SuperGroupHourlyInfoResponse){return SOAP_OK;}

	/// Web service operation 'SuperGroupDailyBriefInfo' (returns error code or SOAP_OK)
int EMISwebBindingService::SuperGroupDailyBriefInfo(ns1__GenInfoRequestType *ns1__SuperGroupDailyBriefInfoRequest, ns1__ReturnArrayDataType &ns1__SuperGroupDailyBriefInfoResponse){return SOAP_OK;}

