#ifndef CSA_WEB_IVR_REPLY_H
#define CSA_WEB_IVR_REPLY_H

#include "csa_web_serverMT.hpp"
#include "csa_res_ivr_app.h"
#include "csa_res_ivr.h"
#include "csa_web_data_interface.hpp"
class IVRApplicationReply : public DataItemType_inter{ //: public EMISwebBindingService{

//============================ Public Section ========================
public:

  // ctor
	IVRApplicationReply(){ };
  //dstr
	~IVRApplicationReply(){};

CsaRc_t     StartMonitor       (unsigned int ivrapp_id);
CsaRc_t     MakeIvrAppInfo    (CsaWebServer *soap,IvrAppRecord_c	*ivr_app_rec, ns1__BlockItemType *ReturnArrayData );
CsaRc_t     MakeIvrAppInfoEMPTY  (CsaWebServer *soap,unsigned int IvrAppId, ns1__BlockItemType *ReturnArrayData );  //PVL 14-06-2010 1004180
const static  int inBound = 0;
const static int outBound =1;
};

//--------------------------------------------------------------------
class IVRGrpPortReply : public DataItemType_inter{ //: public EMISwebBindingService{

public:

  // ctor
	IVRGrpPortReply(){ };
  //dstr
	~IVRGrpPortReply(){};
void InsertIVRGrpPortList(CsaWebServer *soap, 
		                Id_type   Id,
						std::string    Name,
						ns1__GenListReturnType &ReturnArrayData);

CsaRc_t     MakeIvrGrpPortInfo    (CsaWebServer *soap,CsaIvrPortRecord_c* ivr_port_rec, ns1__BlockItemType *ReturnArrayData );
CsaRc_t     MakeIvrGrpInfo        (CsaWebServer *soap,CsaIvrGroupRecord_c* ivr_grp_rec, ns1__BlockItemType *ReturnArrayData );
CsaRc_t     StartMonitorGroup         (unsigned int group_id);
CsaRc_t     StartMonitorPort         (unsigned int group_id);

const static  int inBound = 0;
const static int outBound =1;
};
#endif
