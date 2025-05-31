#ifndef CSA_WEB_REPORTS_H
#define CSA_WEB_REPORTS_H

#include "csa_web_serverMT.hpp"
typedef enum
{
  NLA_REPORT_HDR,
  NLA_REPORT_INFO,
  NLA_REPORT_INFO_COUNT
}report_enum_t;


#define CSA_WEB_FIRST_REPORT_ID 10000

//#define  PUBLIC = 0,
//  PRIVATE,
//  PublicAndPrivate

typedef  struct{
    unsigned short  report_id;
    //16-May-2019 YR BZ#49775
    char report_name[REPORT_MAX_NAME_LEN];
    string report_render;
    unsigned short ppublic;
    char db_info_buffer[REPORT_MAX_DB_DATA_LEN]; 
    //char reply_buffer[REPORT_MAX_REPLY_DATA_LEN]; 
    string reply_buffer;
}report_info_t;

typedef  struct{
    short x;  //can be negative
    short y;  //can be negative
    unsigned short width;
    unsigned short height;
}geometry_t;

// defined  p:\source\include\mdbi_dbiidd.h
//typedef char          DbiDataRecord_t[512];
class ReportsReply{ 

//============================ Public Section ========================
public:

  // ctor
	ReportsReply(){
	
	};
  //dstr
	~ReportsReply(){};
	
    void InsertReportForDefaultWorkspace(CsaWebServer *soap,
									     unsigned short reportId,
									     const geometry_t &geometry,
									     ns1__DefaultWorkspaceReturnType &ReturnArrayData);

    void InsertReportToMap(CsaWebServer *soap, report_info_t* report_info );
    void InsertReportsToReply(CsaWebServer *soap,ns1__ReportListDataReturnType &ReturnArrayData);
    void SetReturnArray(ns1__ReportListDataReturnType &ReturnData)
    {
        m_ReturnArrayData = &ReturnData;
    }

    bool GetAllReportsByAgentID(CsaWebServer *soap,unsigned short agentId,ns1__ReportListDataReturnType &ReturnArrayData);

    bool GetReportInfoByRepotId(report_info_t* report_info);
    bool XcodeReportDataForWeb(report_info_t* report_info);

    bool SaveWorkspase(CsaWebServer *soap,ns1__SaveWorkspaseRequestType *Workspace);
    int SaveReport(CsaWebServer *soap,ns1__RequestSaveReportType *Report,unsigned int &SavedReportID);
    int DeleteReport(CsaWebServer *soap,ns1__RequestDeleteReportType *ns1__RequestDeleteReport);

    bool TryLoadNewDefConf(CsaWebServer *soap,unsigned int userId,ns1__DefaultWorkspaceReturnType &Response);
    int  LoadOldDefConf(CsaWebServer *soap,unsigned int userId, ns1__DefaultWorkspaceReturnType &Response);
private:
 
    ns1__ReportListDataReturnType* m_ReturnArrayData;
    report_enum_t m_report_val;
    unsigned int  m_LastReportId;
    unsigned long m_rep_info_records_to_fetch;
    unsigned long m_rep_last_record_fetched;

    bool GetOLDReportsByAgentID(CsaWebServer *soap,unsigned short agentId,ns1__ReportListDataReturnType &ReturnArrayData);
    bool GetNEWReportsByAgentID(CsaWebServer *soap,unsigned short agentId,ns1__ReportListDataReturnType &ReturnArrayData);

    bool isReportPublic(unsigned short  report_id,unsigned int UserId,bool &isReportExist);
    bool IsAllowPublic(unsigned int UserId,bool &bAllowPublic);
    bool GetLastReportNumber(void);
    //2-Oct-2019 YR BZ#50926
    bool IsReportExist(unsigned int UserId,ns1__RequestSaveReportType *Report,bool &sameName, bool &isPublic);
    //bool IsReportExist(unsigned int UserId,ns1__RequestSaveReportType *Report,unsigned int &SavedReportID);
    bool UpdateReport (unsigned int UserId,ns1__RequestSaveReportType *Report,unsigned int &SavedReportID);
    bool SaveNewReport(unsigned int UserId,ns1__RequestSaveReportType *Report,unsigned int &SavedReportID);
    bool isReportInWebTab(report_info_t* report_info);

   

    void SetRecordsToFetch(unsigned long  rec_to_fetch) { m_rep_info_records_to_fetch = rec_to_fetch;}
    void SetLastRecordFetched(unsigned long last_fetched) { m_rep_last_record_fetched = last_fetched;}
    Ulong_t      GetRecordsToFetch() { return m_rep_info_records_to_fetch;}
    Ulong_t      GetLastRecordFetched() { return m_rep_last_record_fetched;}

    int Parse_BriefAgentStatus( report_info_t* report_info,char * pch,const char *delim);//PVL 24-06-2010 1004301
    void Parse_DetailStatus( report_info_t* report_info,char * pch,const char *delim,string id_str);
    void Parse_GrpGraph( report_info_t* report_info,char * pch,const char *delim,string id_str);
    //void Parse_BriefGrpStatus( report_info_t* report_info,char * pch,const char *delim);
    int Parse_GrpAgentStatus1( report_info_t* report_info,char * pch,const char *delim,string id_str); //PVL 24-06-2010 1004301
    void Parse_DistrGraph( report_info_t* report_info,char * pch,const char *delim,string id_str);
    //void Parse_BriefDNIS( report_info_t* report_info,char * pch,const char *delim);

    int  Parse_BriefStatus( report_info_t* report_info,char * pch,const char *delim,string id_str); //PVL 24-06-2010 1004301

    //15-Aug-2019 BZ#50532
    void setFontLangInDB(ns1__SaveWorkspaseRequestType *Workspace, Ushort_t user_id);
    void getFontLangFromDB(ns1__DefaultWorkspaceReturnType &Response, Ushort_t user_id);
};

#endif
