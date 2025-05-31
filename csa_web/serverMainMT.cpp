#include "soapEMISwebBindingService.h"
#include "EMISwebBinding.nsmap"
//-------------------------------------------------------------------------------------------
unsigned long NumberOfItems;
ns1__DataItemType*  DIT[100];
ns1__GroupListDataItemType* GLT[100];

void CreateData()
{
	NumberOfItems =0;
}


void DeleteAll(soap *soap)
	{
	for(int i=0;i<NumberOfItems;i++)
		soap_delete_ns1__DataItemType(soap,DIT[i]);
	};

//HTTP GET
int http_get(struct soap *soap)
{
	FILE *fd;
	int r;
   char *s = strchr(soap->path, '?');
   if (!s || strcmp(s, "?wsdl"))
      return SOAP_GET_METHOD;
   fd = fopen("emisweb.wsdl", "rb"); // open WSDL file to copy
   if (!fd)
      return 404; // return HTTP not found error
   soap->http_content = "text/xml"; // HTTP header with text/xml content
   soap_response(soap, SOAP_FILE);
   for (;;)
   {
      r = fread(soap->tmpbuf, 1, sizeof(soap->tmpbuf), fd);
      if (!r)
         break;
      if (soap_send_raw(soap, soap->tmpbuf, r))
         break; // can't send, but little we can do about that
   }
   fclose(fd);
   soap_end_send(soap);
   return SOAP_OK;
}
//------------------------------------
int main()
{
   EMISwebBindingService wEMISServiceInterface;
   int m, s; // master and slave sockets
   soap_init(&wEMISServiceInterface);

   wEMISServiceInterface.fget = http_get;
   //m = wEMISServiceInterface.bind("192.168.10.14", 9071, 100);
     m = wEMISServiceInterface.bind(NULL, 9071, 100);
   if (m < 0)
      soap_print_fault(&wEMISServiceInterface, stderr);
   else
   {
      fprintf(stderr, "Socket connection successful: master socket = %d\n", m);
      for (int i = 1; ; i++)
	  {
         s = wEMISServiceInterface.accept();
         if (s < 0)
         {
            soap_print_fault(&wEMISServiceInterface, stderr);
            break;
         }
		 CreateData();
         fprintf(stderr, "%d: accepted connection from IP=%d.%d.%d.%d socket=%d \n", i,
            (wEMISServiceInterface.ip >> 24)&0xFF, (wEMISServiceInterface.ip >> 16)&0xFF, (wEMISServiceInterface.ip >> 8)&0xFF, wEMISServiceInterface.ip&0xFF, s);
		 if (wEMISServiceInterface.serve() != SOAP_OK) // process RPC request
            soap_print_fault(&wEMISServiceInterface, stderr); // print error
         fprintf(stderr, "request served\n");
		 DeleteAll(&wEMISServiceInterface);
         soap_destroy(&wEMISServiceInterface); // clean up class instances
         soap_end(&wEMISServiceInterface); // clean up everything and close socket
      }
   }
   soap_done(&wEMISServiceInterface); // close master socket and detach environment
}

//



// Inser Data to Reply Array
void InsertFullData(soap *soap,std::string ersid,std::string ersname,ULONG64 value,ns1__ReturnArrayDataType *ReturnArrayData)
{

DIT[NumberOfItems] = soap_new_ns1__DataItemType(soap, 1);
DIT[NumberOfItems]->ersid = ersid;
DIT[NumberOfItems]->ersname =ersname;
DIT[NumberOfItems]->value = value;
ReturnArrayData->returnArray.push_back(DIT[NumberOfItems]);
NumberOfItems++;
}

// Inser Data to Reply Array
void InsertGroupList(soap *soap,
					 unsigned int   GrpId,
					 std::string    GrpName,
					 std::string    GrpNumber,
					 unsigned int   GrpEmailEnable,
					 ns1__GroupListReturnType *ReturnArrayData)
{

GLT[NumberOfItems] = soap_new_ns1__GroupListDataItemType(soap, 1);
GLT[NumberOfItems]->grpId = GrpId;
GLT[NumberOfItems]->grpName =GrpName;
GLT[NumberOfItems]->grpNumber = GrpNumber;
ReturnArrayData->returnArray.push_back(GLT[NumberOfItems]);
NumberOfItems++;
}


int EMISwebBindingService::GroupDetailFullInfo(ns1__GroupInfoRequestType *ns1__RequestGroupDetailFullInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupDetailFullInfo)
{
	//printf("ServiceReqId - %d\n",ns1__RequestGroupDetailFullInfo->serviceReqId);

char buffer[65];

ns1__ReturnGroupDetailFullInfo->errorCause = (unsigned int)0;
ns1__ReturnGroupDetailFullInfo->serviceReqId = ns1__RequestGroupDetailFullInfo->serviceReqId;
ns1__ReturnGroupDetailFullInfo->methodName = "Group Info";
CreateData();

InsertFullData(this,"f1_1_1_1_1","primary_agents1",1,ns1__ReturnGroupDetailFullInfo);
InsertFullData(this,"f2_2_2_2_2","primary_agents2",2,ns1__ReturnGroupDetailFullInfo);
InsertFullData(this,"f3_3_3_3_3","primary_agents3",3,ns1__ReturnGroupDetailFullInfo);
InsertFullData(this,"f4_4_4_4_4","primary_agents4",4,ns1__ReturnGroupDetailFullInfo);


  return SOAP_OK;
}
int EMISwebBindingService::GroupQueInfo(ns1__GroupInfoRequestType *ns1__RequestGroupQueInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupQueInfo)
{
	//printf("ServiceReqId - %d\n",ns1__RequestGroupQueInfo->serviceReqId);

char buffer[65];

ns1__ReturnGroupQueInfo->errorCause = (unsigned int)0;
ns1__ReturnGroupQueInfo->serviceReqId = ns1__RequestGroupQueInfo->serviceReqId;
ns1__ReturnGroupQueInfo->methodName = "QueInfo";
CreateData();

InsertFullData(this,"f10_1_1_1_1","primary_agents10",10,ns1__ReturnGroupQueInfo);
InsertFullData(this,"f20_2_2_2_2","primary_agents20",20,ns1__ReturnGroupQueInfo);
InsertFullData(this,"f30_3_3_3_3","primary_agents30",30,ns1__ReturnGroupQueInfo);
InsertFullData(this,"f40_4_4_4_4","primary_agents40",40,ns1__ReturnGroupQueInfo);


  return SOAP_OK;
}


/// Web service operation 'GroupEntityUpdateAgentsInfo' (returns error code or SOAP_OK)
int EMISwebBindingService::GroupAgentsInfo(ns1__GroupInfoRequestType *ns1__RequestGroupAgentsInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupAgentsInfo)

{ 	//printf("ServiceReqId - %d\n",ns1__RequestGroupAgentsInfo->serviceReqId);

char buffer[65];

ns1__ReturnGroupAgentsInfo->errorCause = (unsigned int)0;
ns1__ReturnGroupAgentsInfo->serviceReqId = ns1__RequestGroupAgentsInfo->serviceReqId;
ns1__ReturnGroupAgentsInfo->methodName = "GroupEntityUpdateAgentsInfo";
CreateData();

InsertFullData(this,"f10_1_1_1_1","primary_agents10",10,ns1__ReturnGroupAgentsInfo);
InsertFullData(this,"f20_2_2_2_2","primary_agents20",20,ns1__ReturnGroupAgentsInfo);
InsertFullData(this,"f30_3_3_3_3","primary_agents30",30,ns1__ReturnGroupAgentsInfo);
InsertFullData(this,"f40_4_4_4_4","primary_agents40",40,ns1__ReturnGroupAgentsInfo);
DeleteAll(this);
return SOAP_OK;
}

	/// Web service operation 'GroupAbndInfo' (returns error code or SOAP_OK)
int 	EMISwebBindingService::GroupAbndInfo(ns1__GroupInfoRequestType *ns1__RequestGroupAbndInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupAbndInfo){return SOAP_OK;}

	/// Web service operation 'GroupIntOvfInfo' (returns error code or SOAP_OK)
int 	EMISwebBindingService::GroupIntOvfInfo(ns1__GroupInfoRequestType *ns1__RequestGroupIntOvfInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupIntOvfInfo){return SOAP_OK;}

	/// Web service operation 'GroupShiftInfo' (returns error code or SOAP_OK)
int 	EMISwebBindingService::GroupShiftInfo(ns1__GroupInfoRequestType *ns1__RequestGroupShiftInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupShiftInfo){return SOAP_OK;}

	/// Web service operation 'GroupGeneralInfo' (returns error code or SOAP_OK)
int 	EMISwebBindingService::GroupGeneralInfo(ns1__GroupInfoRequestType *ns1__RequestGroupGeneralInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupGeneralInfo){return SOAP_OK;}

	/// Web service operation 'GroupPeriodInfo' (returns error code or SOAP_OK)
int 	EMISwebBindingService::GroupPeriodInfo(ns1__GroupInfoRequestType *ns1__RequestGroupPeriodInfo, ns1__ReturnArrayDataType *ns1__ReturnGroupPeriodInfo){return SOAP_OK;}

int 	EMISwebBindingService::UserLogin(ns1__LoginRequestType *ns1__RequestUserLogin, ns1__LoginReturnType *ns1__ReturnUserLogin)
		{
			//printf("ServiceReqId - %d  User: %s Pass: %s \n",
			//	                        ns1__RequestUserLogin->serviceReqId,
			//							ns1__RequestUserLogin->name.c_str(),
			//							ns1__RequestUserLogin->pass.c_str());

			char buffer[65];
			ns1__ReturnUserLogin->serviceReqId =  ns1__RequestUserLogin->serviceReqId;
			ns1__ReturnUserLogin->level =3;
			ns1__ReturnUserLogin->userId = 1;
			ns1__ReturnUserLogin->agentId =   1;
			ns1__ReturnUserLogin->loginEnable =1;


			return SOAP_OK;}

	/// Web service operation 'GroupListForSupervisor' (returns error code or SOAP_OK)
int 	EMISwebBindingService::GroupList(ns1__GroupListRequestType *ns1__RequestGroupList, ns1__GroupListReturnType *ns1__ReturnGroupList)
		{
		 //printf("ServiceReqId - %d\n",ns1__RequestGroupList->serviceReqId);

		char buffer[65];

		ns1__ReturnGroupList->serviceReqId = ns1__RequestGroupList->serviceReqId;
		CreateData();

		InsertGroupList(this,1,"SupportGroup","10",0,ns1__ReturnGroupList);
		InsertGroupList(this,2,"SupportGroup2Email","2",1,ns1__ReturnGroupList);


			return SOAP_OK;}
 ns1__DataItemTypeStr*  DITS[5];
 ns1__DataItemRow* Row[5];
 ULONG64 bgcolor;
int EMISwebBindingService::AgentsPerGroupInfo(ns1__GroupInfoRequestType *ns1__RequestGroupAgentsMatrix, ns1__ReturnMatrixDataTypeBG *ns1__ReturnGroupAgentsMatrix)
		{

		Row[0] = soap_new_ns1__DataItemRow(this, 1);

		 for(int i=0;i<3;i++){
			 DITS[i] = soap_new_ns1__DataItemTypeStr(this, 1);
			 DITS[i]->ersid ="f6_3_2_1_1";
			 DITS[i]->ersname ="Agent Name";
			 bgcolor = 0xff000;
			// DITS[i]->bgcolor = &bgcolor ;
			 Row[0]->returnArray.push_back(DITS[i]);
		 }
		 ns1__ReturnGroupAgentsMatrix->returnMatrix.push_back(Row[0]);
		return SOAP_OK;
		}

		int EMISwebBindingService::AgentsList(ns1__AgentsListRequestType *ns1__RequestAgentsList, ns1__AgentsListReturnType *ns1__ReturnAgentsList)
		{	return SOAP_OK;}

	/// Web service operation 'BriefAgents' (returns error code or SOAP_OK)
		int EMISwebBindingService::BriefAgents(ns1__BriefAgentInfoRequestType *ns1__RequestBriefAgentsMatrix, ns1__ReturnMatrixDataTypeBG *ns1__ReturnBriefAgentsMatrix)
	{	return SOAP_OK;}


	/// Web service operation 'DefaultWorkspace' (returns error code or SOAP_OK)
		int EMISwebBindingService::DefaultWorkspace(ns1__DefaultWorkspaceRequestType *ns1__RequestDefaultWorkspace, ns1__DefaultWorkspaceReturnType *ns1__ReturnDefaultWorkspace)
			{	return SOAP_OK;}

	/// Web service operation 'ReportListData' (returns error code or SOAP_OK)
		int EMISwebBindingService::ReportListData(ns1__ReportListDataRequestType *ns1__RequestReportListData, ns1__ReportListDataReturnType *ns1__ReturnReportListData)
			{	return SOAP_OK;}

/// Web service operation 'VoiceDNISList' (returns error code or SOAP_OK)
		int EMISwebBindingService::VoiceDNISList(ns1__DNISListRequestType *ns1__RequestVoiceDNISList, ns1__DNISListReturnType *ns1__ReturnVoiceDNISList)
			{	return SOAP_OK;}

	/// Web service operation 'EmailDNISList' (returns error code or SOAP_OK)
			int EMISwebBindingService::EmailDNISList(ns1__DNISListRequestType *ns1__RequestEmailDNISList, ns1__DNISListReturnType *ns1__ReturnEmailDNISList)
			{	return SOAP_OK;}

	/// Web service operation 'BriefVoiceDNIS' (returns error code or SOAP_OK)
		int EMISwebBindingService::BriefVoiceDNIS(ns1__BriefAgentInfoRequestType *ns1__RequestBriefVoiceDNISMatrix, ns1__ReturnMatrixDataTypeBG *ns1__ReturnBriefVoiceDNISMatrix)
			{	return SOAP_OK;}

	/// Web service operation 'BriefEmailDNIS' (returns error code or SOAP_OK)
		int EMISwebBindingService::BriefEmailDNIS(ns1__BriefAgentInfoRequestType *ns1__RequestBriefEMailDNISMatrix, ns1__ReturnMatrixDataTypeBG *ns1__ReturnBriefEMailDNISMatrix)
			{	return SOAP_OK;}
