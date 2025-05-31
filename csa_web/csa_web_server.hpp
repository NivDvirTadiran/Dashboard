#ifndef CSA_WEB_SERVER_H
#define CSA_WEB_SERVER_H

#include "aragent.hpp"
#include "csa_web_init.h"
#include "csa_web_limits.hpp"
#include "csa_web_main.h"
#include "csa_web_agents.hpp"
#include "csa_web_groups.hpp"
#include "csa_web_dnis.hpp"
#include "csa_web_ivrapp.hpp"
#include "csa_web_mailbox.hpp"
#include "csa_web_ticket.hpp"

#include "soapEMISwebBindingService.h"
#include <vector>

#include <map>

class CsaWebServer : public EMISwebBindingService{

//============================ Public Section ========================
public:

  ns1__ResponseInfoHeaderType *         ResponseHeader;

  vector<ns1__BlockItemType*>           BLOCK_vector;
  vector<ns1__BlockItemType*>::iterator BLOCK_iter;

  vector<ns1__returnAgentMatrixType*>           MATRIX_PER_GROUP_vector;
  vector<ns1__returnAgentMatrixType*>::iterator MATRIX_PER_GROUP_iter;

  vector<ns1__DataItemType*>            DIT_vector;
  vector<ns1__DataItemType*>::iterator  DIT_iter;

  vector<ns1__DataItemRow*>             ROW_vector;
  vector<ns1__DataItemRow*>::iterator   ROW_iter;

  vector<ns1__GroupListDataItemType*>           GroupListType_vector;
  vector<ns1__GroupListDataItemType*>::iterator GroupListType_iter;

  vector<ns1__AgentsListDataItemType*>            AgentsListType_vector;
  vector<ns1__AgentsListDataItemType*>::iterator  AgentsListType_iter;

  vector<ns1__DefaultWorkspaceItemType*>            DW_vector;
  vector<ns1__DefaultWorkspaceItemType*>::iterator  DW_iter;

  map<unsigned int,ns1__ReportListDataItemType*>            RLT_map;
  map<unsigned int,ns1__ReportListDataItemType*>::iterator  RLT_iter;

  vector<ns1__DNISListDataItemType*>            DNISL_vector;
  vector<ns1__DNISListDataItemType*>::iterator  DNISL_iter;

  vector<ns1__GenListDataItemType*>             GENL_vector;
  vector<ns1__GenListDataItemType*>::iterator   GENL_iter;

  State_colors_tab  colortab;
  Limits_tab        limitstab;
  Agents_tab        agentstab;
  Groups_tab        groupstab;
  DNIS_tab          dnistab;
  IVRapp_tab        ivrapptab;
  IVRgrpport_tab    ivrgrptab;
  MAILbox_tab       mailboxacctab;

  TICKET_tab        TicketTab;


  // ctor
  CsaWebServer(){};
  //dstr
  ~CsaWebServer(){};
  void CleanAll(void);
  void InsertGenListDataItemType(unsigned int             Id,
                                 std::string              Name,
                                 ns1__GenListReturnType * ReturnArrayData);

  bool IsAgentLoggedOnlyToAnotherGroup(unsigned int AgentId,unsigned int GroupId );

  bool IsAgentNotLoggedToGroup(unsigned int AgentId,unsigned int GroupId );

  int localLoginLogout(ns1__LoginManagerRequestType *ns1__LoginManagerRequest, ns1__GenListReturnType *ns1__LoginManagerResponse);

  int GroupDetailFullInfo(ns1__GenInfoRequestType *ns1__RequestGroupDetailFullInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupDetailFullInfo);

  int GroupQueInfo(ns1__GenInfoRequestType *ns1__RequestGroupQueInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupQueInfo);



  /// Web service operation 'GroupAgentsInfo' (returns error code or SOAP_OK)
  int GroupAgentsInfo(ns1__GenInfoRequestType *ns1__RequestGroupAgentsInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupAgentsInfo);

  /// Web service operation 'GroupAbndInfo' (returns error code or SOAP_OK)
  int GroupAbndInfo(ns1__GenInfoRequestType *ns1__RequestGroupAbndInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupAbndInfo);

  /// Web service operation 'GroupIntOvfInfo' (returns error code or SOAP_OK)
  int GroupIntOvfInfo(ns1__GenInfoRequestType *ns1__RequestGroupIntOvfInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupIntOvfInfo);

  /// Web service operation 'GroupShiftInfo' (returns error code or SOAP_OK)
  int GroupShiftInfo(ns1__GenInfoRequestType *ns1__RequestGroupShiftInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupShiftInfo);

  /// Web service operation 'GroupGeneralInfo' (returns error code or SOAP_OK)
  int GroupGeneralInfo(ns1__GenInfoRequestType *ns1__RequestGroupGeneralInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupGeneralInfo);

  /// Web service operation 'GroupPeriodInfo' (returns error code or SOAP_OK)
  int GroupPeriodInfo(ns1__GenInfoRequestType *ns1__RequestGroupPeriodInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupPeriodInfo);

  /// Web service operation 'UserLogin' (returns error code or SOAP_OK)
  int UserLogin(ns1__LoginRequestType *ns1__RequestUserLogin, ns1__LoginReturnType *ns1__ReturnUserLogin);

  /// Web service operation 'UserLogout' (returns error code or SOAP_OK)
  int UserLogout(ns1__GenRequestType *ns1__RequestUserLogout, ns1__ResponseInfoHeaderType *ns1__NoDataResponse);

  /// Web service operation 'GroupList' (returns error code or SOAP_OK)
  int GroupList(ns1__GenRequestType *ns1__RequestGroupList, ns1__GroupListReturnType *ns1__ReturnGroupList);

  /// Web service operation 'IvrPortGroupList' (returns error code or SOAP_OK)
  int IvrPortGroupList(ns1__GenRequestType *ns1__RequestIvrPortGroupList, ns1__GenListReturnType *ns1__ReturnIvrPortGroupList);

  /// Web service operation 'IvrAppList' (returns error code or SOAP_OK)
  int IvrAppList(ns1__GenRequestType *ns1__RequestIvrAppList, ns1__GenListReturnType *ns1__ReturnIvrAppList);


  int AgentsPerGroupInfo(ns1__GenInfoRequestType *ns1__RequestGroupAgentsMatrix, ns1__ReturnMatrixDataTypeBGperGroupArr *ns1__ReturnGroupAgentsMatrix);

  /// Web service operation 'AgentsList' (returns error code or SOAP_OK)
  int AgentsList(ns1__GenRequestType *ns1__RequestAgentsList, ns1__AgentsListReturnType *ns1__ReturnAgentsList);

  /// Web service operation 'BriefAgents' (returns error code or SOAP_OK)
  int BriefAgents(ns1__BriefInfoRequestType *ns1__RequestBriefAgentsMatrix, ns1__ReturnMatrixDataTypeBG *ns1__ReturnBriefAgentsMatrix);

  /// Web service operation 'DefaultWorkspace' (returns error code or SOAP_OK)
  int DefaultWorkspace(ns1__DefaultWorkspaceRequestType *ns1__RequestDefaultWorkspace, ns1__DefaultWorkspaceReturnType *ns1__ReturnDefaultWorkspace);

  /// Web service operation 'ReportListData' (returns error code or SOAP_OK)
  int ReportListData(ns1__ReportListDataRequestType *ns1__RequestReportListData, ns1__ReportListDataReturnType *ns1__ReturnReportListData);
  /// Web service operation 'VoiceDNISList' (returns error code or SOAP_OK)
  int VoiceDNISList(ns1__GenRequestType *ns1__RequestVoiceDNISList, ns1__DNISListReturnType *ns1__ReturnVoiceDNISList);

  /// Web service operation 'EmailDNISList' (returns error code or SOAP_OK)
  int EmailDNISList(ns1__GenRequestType *ns1__RequestEmailDNISList, ns1__DNISListReturnType *ns1__ReturnEmailDNISList);

  /// Web service operation 'BriefVoiceDNIS' (returns error code or SOAP_OK)
  int BriefVoiceDNIS(ns1__BriefInfoRequestType *ns1__RequestBriefVoiceDNISMatrix, ns1__ReturnMatrixDataTypeBG *ns1__ReturnBriefVoiceDNISMatrix);

  /// Web service operation 'BriefEmailDNIS' (returns error code or SOAP_OK)
  int BriefEmailDNIS(ns1__BriefInfoRequestType *ns1__RequestBriefEMailDNISMatrix, ns1__ReturnMatrixDataTypeBG *ns1__ReturnBriefEMailDNISMatrix);
  /// Web service operation 'IvrApplInfo' (returns error code or SOAP_OK)

  int IvrApplInfo(ns1__GenInfoRequestType *ns1__RequestIvrApplInfo, ns1__ReturnArrayDataType *ns1__ReturnIvrApplInfo);

  /// Web service operation 'IvrPortInfo' (returns error code or SOAP_OK)
  int IvrPortInfo(ns1__GenInfoRequestType *ns1__RequestIvrPortInfo, ns1__ReturnArrayDataType *ns1__ReturnIvrPortInfo);

  /// Web service operation 'IvrGrpInfo' (returns error code or SOAP_OK)
  int IvrGrpInfo(ns1__GenInfoRequestType *ns1__RequestIvrGrpInfo, ns1__ReturnArrayDataType *ns1__ReturnIvrGrpInfo);

  int MailboxInfo(ns1__GenInfoRequestType *ns1__RequestMailboxInfo, ns1__ReturnArrayDataType *ns1__ReturnMailboxInfo);


  int MailAccList(ns1__GenRequestType *ns1__RequestMailAccList, ns1__GenListReturnType *ns1__ReturnMailAccList);

  /// Web service operation 'CommandAgentLoginLogout' (returns error code or SOAP_OK)
  int CommandAgentLoginLogout(ns1__ACDMultiRequestType *ns1__LoginLogoutRequestAcd, ns1__ResponseInfoHeaderType *ns1__NoDataResponse);

  /// Web service operation 'CommandAgentReleaseResume' (returns error code or SOAP_OK)
  int CommandAgentReleaseResume(ns1__ACDRequestType *ns1__ReleaseResumeRequestAcd, ns1__ResponseInfoHeaderType *ns1__NoDataResponse);

  /// Web service operation 'CommandAgentStartChat' (returns error code or SOAP_OK)
  int CommandAgentStartChat(ns1__TFRequestType *ns1__StartChatRequestAcd, ns1__ChatResponseType *ns1__StartChatResponse);

  /// Web service operation 'CommandAgentStartRecord' (returns error code or SOAP_OK)
  int CommandAgentStartRecord(ns1__TFRequestType *ns1__StartRecordRequestAcd, ns1__ResponseInfoHeaderType *ns1__NoDataResponse);

  /// Web service operation 'CommandAgentTelephonyFeatures' (returns error code or SOAP_OK)
  int CommandAgentTelephonyFeatures(ns1__TFRequestType *ns1__RequestTf, ns1__ResponseInfoHeaderType *ns1__NoDataResponse);


  /// Web service operation 'SaveWorkspase' (returns error code or SOAP_OK)
  int SaveWorkspase(ns1__SaveWorkspaseRequestType *ns1__RequestSaveWorkspase, ns1__ResponseInfoHeaderType *ns1__NoDataResponse);

  /// Web service operation 'SaveReport' (returns error code or SOAP_OK)
  int SaveReport(ns1__RequestSaveReportType *ns1__RequestSaveReport, ns1__ResponseSaveReportType *ns1__ResponseSaveReport);

  /// Web service operation 'DeleteReport' (returns error code or SOAP_OK)
  int DeleteReport(ns1__RequestDeleteReportType *ns1__RequestDeleteReport, ns1__ResponseInfoHeaderType *ns1__NoDataResponse);

  /// Web service operation 'LoginManagerAgentsPerGroup' (returns error code or SOAP_OK)
  int LoginManagerAgentsPerGroup(ns1__LoginManagerRequestType *ns1__LoginManagerRequest, ns1__GenListReturnType *ns1__LoginManagerResponse);

  /// Web service operation 'LoginManagerGroupsPerAgent' (returns error code or SOAP_OK)
  int LoginManagerGroupsPerAgent(ns1__LoginManagerRequestType *ns1__LoginManagerRequest, ns1__GenListReturnType *ns1__LoginManagerResponse);


  /// Web service operation 'LoginManagerOneAgentToMultiGroups' (returns error code or SOAP_OK)
  int LoginManagerOneAgentToMultiGroups(ns1__ACDMultiRequestType *ns1__LoginLogoutRequestAcd, ns1__ResponseInfoHeaderType *ns1__NoDataResponse);

  /// Web service operation 'LoginManagerMultiAgentsToOneGroup' (returns error code or SOAP_OK)
  int LoginManagerMultiAgentsToOneGroup(ns1__ACDMultiRequestType *ns1__LoginLogoutRequestAcd, ns1__ResponseInfoHeaderType *ns1__NoDataResponse);


  /// Web service operation 'LoginManagerAvailableAgentsForGroup' (returns error code or SOAP_OK)
  int LoginManagerAvailableAgentsForGroup(ns1__LoginManagerRequestType *ns1__LoginManagerRequestAvailableAgents, ns1__GenListReturnType *ns1__LoginManagerResponse);

  /// Web service operation 'LoginManagerAvailableGroupsForAgent' (returns error code or SOAP_OK)
  int LoginManagerAvailableGroupsForAgent(ns1__LoginManagerRequestType *ns1__LoginManagerRequestAvailableGroups, ns1__GenListReturnType *ns1__LoginManagerResponse);

};

#endif
