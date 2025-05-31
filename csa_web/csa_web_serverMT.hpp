#ifndef CSA_WEB_SERVER_MT_H
#define CSA_WEB_SERVER_MT_H

#include "aragent.hpp"
#include "csa_web_server_net_data.hpp"
#include "csa_web_server_db_data.hpp"


#ifdef WIN32
#include "windows/soapEMISwebBindingService.h"
#else
#include "linux/soapEMISwebBindingService.h"
#endif

typedef enum
{
  GroupAbndInfo_E = 1,
  GroupIntOvfInfo_E,
  GroupShiftInfo_E,
  GroupGeneralInfo_E,
  GroupPeriodInfo_E,
  UserLogin_E,
  UserLogout_E,
  GroupList_E,
  IvrPortGroupList_E,
  IvrAppList_E,
  AgentsPerGroupInfo_E,
  AgentsList_E,
  BriefAgents_E,
  VoiceDNISList_E,
  EmailDNISList_E,
  BriefVoiceDNIS_E,
  BriefEmailDNIS_E,
  IvrApplInfo_E,
  IvrPortInfo_E,
  IvrGrpInfo_E,
  MailboxInfo_E,
  KeepAlive_E,
  MailAccList_E,
  SuperGroupList_E,
  SuperGroupInfo_E
} CSA_WEB_REPORTS;


static bool ha_status_is_active;


class CsaWebServer : public EMISwebBindingService{

//============================ Public Section ========================
public:

  // Constructor
  CsaWebServer();
  //Constructor with copy of another engine state
  CsaWebServer(const struct soap &soap);
  //dstr
  ~CsaWebServer();

  SrvNetData_c*   m_net_data_ptr;
  SrvDBData_c*    m_db_data_ptr;
  Ushort_t        ThreadIndex; //For DEBUG
  Ushort_t	      m_ReportCode;

  virtual CsaWebServer *copy();

  void CleanAll(void);
  void InsertGenListDataItemType(unsigned int   Id,
                                 std::string    Name,
                                 ns1__GenListReturnType &ReturnArrayData);	
  bool IsAgentLoggedOnlyToAnotherGroup(unsigned int AgentId,unsigned int GroupId );
  bool IsAgentNotLoggedToGroup(unsigned int AgentId,unsigned int GroupId );

  int localLoginLogout(ns1__LoginManagerRequestType *ns1__LoginManagerRequest, ns1__GenListReturnType &ns1__LoginManagerResponse);
  int GroupDetailFullInfo(ns1__GenInfoRequestType *ns1__RequestGroupDetailFullInfo, ns1__ReturnArrayDataType &ns1__ReturnGroupDetailFullInfo);

  int GroupQueInfo(ns1__GenInfoRequestType *ns1__RequestGroupQueInfo, ns1__ReturnArrayDataType &ns1__ReturnGroupQueInfo);


  /// Web service operation 'GroupAgentsInfo' (returns error code or SOAP_OK)
  int GroupAgentsInfo(ns1__GenInfoRequestType *ns1__RequestGroupAgentsInfo, ns1__ReturnArrayDataType &ns1__ReturnGroupAgentsInfo);

  /// Web service operation 'GroupAbndInfo' (returns error code or SOAP_OK)
  int GroupAbndInfo(ns1__GenInfoRequestType *ns1__RequestGroupAbndInfo, ns1__ReturnArrayDataType &ns1__ReturnGroupAbndInfo);

  /// Web service operation 'GroupIntOvfInfo' (returns error code or SOAP_OK)
  int GroupIntOvfInfo(ns1__GenInfoRequestType *ns1__RequestGroupIntOvfInfo, ns1__ReturnArrayDataType &ns1__ReturnGroupIntOvfInfo);

  /// Web service operation 'GroupShiftInfo' (returns error code or SOAP_OK)
  int GroupShiftInfo(ns1__GenInfoRequestType *ns1__RequestGroupShiftInfo, ns1__ReturnArrayDataType &ns1__ReturnGroupShiftInfo);

  /// Web service operation 'GroupGeneralInfo' (returns error code or SOAP_OK)
  int GroupGeneralInfo(ns1__GenInfoRequestType *ns1__RequestGroupGeneralInfo, ns1__ReturnArrayDataType &ns1__ReturnGroupGeneralInfo);

  /// Web service operation 'GroupPeriodInfo' (returns error code or SOAP_OK)
  int GroupPeriodInfo(ns1__GenInfoRequestType *ns1__RequestGroupPeriodInfo, ns1__ReturnArrayDataType &ns1__ReturnGroupPeriodInfo);

  /// Web service operation 'UserLogin' (returns error code or SOAP_OK)
  int UserLogin(ns1__LoginRequestType *ns1__RequestUserLogin, ns1__LoginReturnType &ns1__ReturnUserLogin);

  /// Web service operation 'UserLogout' (returns error code or SOAP_OK)
  int UserLogout(ns1__GenRequestType *ns1__RequestUserLogout, ns1__ResponseInfoHeaderType &ns1__NoDataResponse);

  /// Web service operation 'GroupList' (returns error code or SOAP_OK)
  int GroupList(ns1__GenRequestType *ns1__RequestGroupList, ns1__GroupListReturnType &ns1__ReturnGroupList);

  /// Web service operation 'IvrPortGroupList' (returns error code or SOAP_OK)
  int IvrPortGroupList(ns1__GenRequestType *ns1__RequestIvrPortGroupList, ns1__GenListReturnType &ns1__ReturnIvrPortGroupList);

  /// Web service operation 'IvrAppList' (returns error code or SOAP_OK)
  int IvrAppList(ns1__GenRequestType *ns1__RequestIvrAppList, ns1__GenListReturnType &ns1__ReturnIvrAppList);


  int AgentsPerGroupInfo(ns1__GenInfoRequestType *ns1__RequestGroupAgentsMatrix, ns1__ReturnMatrixDataTypePerGroup &ns1__ReturnGroupAgentsMatrix);

  /// Web service operation 'AgentsList' (returns error code or SOAP_OK)
  int AgentsList(ns1__GenRequestType *ns1__RequestAgentsList, ns1__AgentsListReturnType &ns1__ReturnAgentsList);

  /// Web service operation 'BriefAgents' (returns error code or SOAP_OK)
  int BriefAgents(ns1__BriefInfoRequestType *ns1__RequestBriefAgentsMatrix, ns1__ReturnMatrixDataTypeBG &ns1__ReturnBriefAgentsMatrix);

  /// Web service operation 'DefaultWorkspace' (returns error code or SOAP_OK)
  int DefaultWorkspace(ns1__DefaultWorkspaceRequestType *ns1__RequestDefaultWorkspace, ns1__DefaultWorkspaceReturnType &ns1__ReturnDefaultWorkspace);

  /// Web service operation 'ReportListData' (returns error code or SOAP_OK)
  int ReportListData(ns1__ReportListDataRequestType *ns1__RequestReportListData, ns1__ReportListDataReturnType &ns1__ReturnReportListData);
  /// Web service operation 'VoiceDNISList' (returns error code or SOAP_OK)
  int VoiceDNISList(ns1__GenRequestType *ns1__RequestVoiceDNISList, ns1__DNISListReturnType &ns1__ReturnVoiceDNISList);

  /// Web service operation 'EmailDNISList' (returns error code or SOAP_OK)
  int EmailDNISList(ns1__GenRequestType *ns1__RequestEmailDNISList, ns1__DNISListReturnType &ns1__ReturnEmailDNISList);

  /// Web service operation 'BriefVoiceDNIS' (returns error code or SOAP_OK)
  int BriefVoiceDNIS(ns1__BriefInfoRequestType *ns1__RequestBriefVoiceDNISMatrix, ns1__ReturnMatrixDataTypeBG &ns1__ReturnBriefVoiceDNISMatrix);

  /// Web service operation 'BriefEmailDNIS' (returns error code or SOAP_OK)
  int BriefEmailDNIS(ns1__BriefInfoRequestType *ns1__RequestBriefEMailDNISMatrix, ns1__ReturnMatrixDataTypeBG &ns1__ReturnBriefEMailDNISMatrix);
  /// Web service operation 'IvrApplInfo' (returns error code or SOAP_OK)

  int IvrApplInfo(ns1__GenInfoRequestType *ns1__RequestIvrApplInfo, ns1__ReturnArrayDataType &ns1__ReturnIvrApplInfo);

  /// Web service operation 'IvrPortInfo' (returns error code or SOAP_OK)
  int IvrPortInfo(ns1__GenInfoRequestType *ns1__RequestIvrPortInfo, ns1__ReturnArrayDataType &ns1__ReturnIvrPortInfo);

  /// Web service operation 'IvrGrpInfo' (returns error code or SOAP_OK)
  int IvrGrpInfo(ns1__GenInfoRequestType *ns1__RequestIvrGrpInfo, ns1__ReturnArrayDataType &ns1__ReturnIvrGrpInfo);

  /// Web service operation 'MailboxInfo' (returns error code or SOAP_OK)
  int MailboxInfo(ns1__GenInfoRequestType *ns1__RequestMailboxInfo, ns1__ReturnArrayDataType &ns1__ReturnMailboxInfo);

  /// Web service operation 'SuperGroupInfo' (returns error code or SOAP_OK)
  int SuperGroupInfo(ns1__GenInfoRequestType *ns1__RequestSuperGroupInfo, ns1__ReturnArrayDataType &ns1__ReturnSuperGroupInfo);

  /// Web service operation 'SuperGroupList' (returns error code or SOAP_OK)
  int SuperGroupList(ns1__GenRequestType *ns1__RequestSuperGroupList, ns1__SuperGroupListReturnType &ns1__ReturnSuperGroupList);

  int MailAccList(ns1__GenRequestType *ns1__RequestMailAccList, ns1__GenListReturnType &ns1__ReturnMailAccList);

  /// Web service operation 'CommandAgentLoginLogout' (returns error code or SOAP_OK)
  int CommandAgentLoginLogout(ns1__ACDMultiRequestType *ns1__LoginLogoutRequestAcd, ns1__ResponseInfoHeaderType &ns1__NoDataResponse);

  /// Web service operation 'CommandAgentReleaseResume' (returns error code or SOAP_OK)
  int CommandAgentReleaseResume(ns1__ACDRequestType *ns1__ReleaseResumeRequestAcd, ns1__ResponseInfoHeaderType &ns1__NoDataResponse);

  /// Web service operation 'CommandAgentStartChat' (returns error code or SOAP_OK)
  int CommandAgentStartChat(ns1__TFRequestType *ns1__StartChatRequestAcd, ns1__ChatResponseType &ns1__StartChatResponse);

  /// Web service operation 'CommandAgentStartRecord' (returns error code or SOAP_OK)
  int CommandAgentStartRecord(ns1__TFRequestType *ns1__StartRecordRequestAcd, ns1__ResponseInfoHeaderType &ns1__NoDataResponse);

  /// Web service operation 'CommandAgentTelephonyFeatures' (returns error code or SOAP_OK)
  int CommandAgentTelephonyFeatures(ns1__TFRequestType *ns1__RequestTf, ns1__ResponseInfoHeaderType &ns1__NoDataResponse);


  /// Web service operation 'SaveWorkspase' (returns error code or SOAP_OK)
  int SaveWorkspase(ns1__SaveWorkspaseRequestType *ns1__RequestSaveWorkspase, ns1__ResponseInfoHeaderType &ns1__NoDataResponse);

  /// Web service operation 'SaveReport' (returns error code or SOAP_OK)
  int SaveReport(ns1__RequestSaveReportType *ns1__RequestSaveReport, ns1__ResponseSaveReportType &ns1__ResponseSaveReport);

  /// Web service operation 'DeleteReport' (returns error code or SOAP_OK)
  int DeleteReport(ns1__RequestDeleteReportType *ns1__RequestDeleteReport, ns1__ResponseInfoHeaderType &ns1__NoDataResponse);

  /// Web service operation 'LoginManagerAgentsPerGroup' (returns error code or SOAP_OK)
  int LoginManagerAgentsPerGroup(ns1__LoginManagerRequestType *ns1__LoginManagerRequest, ns1__GenListReturnType &ns1__LoginManagerResponse);

  /// Web service operation 'LoginManagerGroupsPerAgent' (returns error code or SOAP_OK)
  int LoginManagerGroupsPerAgent(ns1__LoginManagerRequestType *ns1__LoginManagerRequest, ns1__GenListReturnType &ns1__LoginManagerResponse);


  /// Web service operation 'LoginManagerOneAgentToMultiGroups' (returns error code or SOAP_OK)
  int LoginManagerOneAgentToMultiGroups(ns1__ACDMultiRequestType *ns1__LoginLogoutRequestAcd, ns1__ResponseInfoHeaderType &ns1__NoDataResponse);

  /// Web service operation 'LoginManagerMultiAgentsToOneGroup' (returns error code or SOAP_OK)
  int LoginManagerMultiAgentsToOneGroup(ns1__ACDMultiRequestType *ns1__LoginLogoutRequestAcd, ns1__ResponseInfoHeaderType &ns1__NoDataResponse);


  /// Web service operation 'LoginManagerAvailableAgentsForGroup' (returns error code or SOAP_OK)
  int LoginManagerAvailableAgentsForGroup(ns1__LoginManagerRequestType *ns1__LoginManagerRequestAvailableAgents, ns1__GenListReturnType &ns1__LoginManagerResponse);

  /// Web service operation 'LoginManagerAvailableGroupsForAgent' (returns error code or SOAP_OK)
  int LoginManagerAvailableGroupsForAgent(ns1__LoginManagerRequestType *ns1__LoginManagerRequestAvailableGroups, ns1__GenListReturnType &ns1__LoginManagerResponse);

  /// Web service operation 'ApplicationRegister' (returns error code or SOAP_OK)
  int ApplicationRegister(ns1__ApplicationRegisterRequestType *ns1__ApplicationRegisterRequest, ns1__ApplicationReturnType &ns1__ApplicationRegisterResponse);

  /// Web service operation 'ApplicationKeepAlive' (returns error code or SOAP_OK)
  int ApplicationKeepAlive(ns1__ApplicationKeepAliveRequestType *ns1__ApplicationKeepAliveRequest, ns1__ApplicationReturnType &ns1__ApplicationKeepAliveResponse);

  /// Web service operation 'TraceInternalStat' (returns error code or SOAP_OK)//PVL_STAT
  int TraceInternalStat(ns1__StatRequestType *ns1__TraceInternalStatRequest, ns1__StatReturnType &ns1__TraceInternalStatResponse);//PVL_STAT
  /// Web service operation 'AgentNeedHelp' (returns error code or SOAP_OK)
  virtual	int AgentNeedHelp(ns1__BriefInfoRequestType *ns1__AgentNeedHelpRequest, ns1__ReturnMatrixDataTypeBG &ns1__AgentNeedHelpResponse);

  /// Web service operation 'GroupHourlyInfo' (returns error code or SOAP_OK)
  int GroupHourlyInfo(ns1__GenInfoRequestType *ns1__GroupHourlyInfoRequest, ns1__ReturnMatrixDataTypePerGroup &ns1__GroupHourlyInfoResponse);

  /// Web service operation 'GroupDailyBriefInfo' (returns error code or SOAP_OK)
  int GroupDailyBriefInfo(ns1__GenInfoRequestType *ns1__GroupDailyBriefInfoRequest, ns1__ReturnArrayDataType &ns1__GroupDailyBriefInfoResponse);

  /// Web service operation 'GroupDailyDetailedInfo' (returns error code or SOAP_OK)
  int GroupDailyDetailedInfo(ns1__GenInfoRequestType *ns1__GroupDailyDetailedInfoRequest, ns1__ReturnArrayDataType &ns1__GroupDailyDetailedInfoResponse);

  /// Web service operation 'SuperGroupHourlyInfo' (returns error code or SOAP_OK)
  int SuperGroupHourlyInfo(ns1__GenInfoRequestType *ns1__SuperGroupHourlyInfoRequest, ns1__ReturnMatrixDataTypePerSuperGroup &ns1__SuperGroupHourlyInfoResponse);

  /// Web service operation 'SuperGroupDailyBriefInfo' (returns error code or SOAP_OK)
  int SuperGroupDailyBriefInfo(ns1__GenInfoRequestType *ns1__SuperGroupDailyBriefInfoRequest, ns1__ReturnArrayDataType &ns1__SuperGroupDailyBriefInfoResponse);

};

//9-Aug-2022 YR BZ#56641
void AddServerTimeToServersInfo(char* server_info);

//18-Apr-2023 YR BZ#
void AddDPToServersInfo_AppCenter(char* server_info);

#endif

