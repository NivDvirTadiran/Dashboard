/*
 * File name : csa_web_sm_syn.hpp
 * Author    : Pavel Dobrohotov
 * Created   : 16/12/09
 * ==========: 
 * Abstract  : adi_dbi_replies class definition
 */

#ifndef GSOAP_WEB_SM_SYN_H
#define GSOAP_WEB_SM_SYN_H 

_USING_BSW

//==========================[ Interface Dependencies ]================
#include "sm_serviceiface.h"

/*-----------------------[ AdiDbiReply_c ]---------------------------*/
class CsaWebSMsyn_c : public IsmService_c  
{
public:
virtual RetCode_t Connected(IsmServiceReply_c * reply_ptr, NetStnId_t stationId){return BSWRC_OK;} 
virtual RetCode_t DisConnected(IsmServiceReply_c * reply_ptr, NetStnId_t stationId){return BSWRC_OK;} 
virtual RetCode_t ConfirmETAS(IsmServiceReply_c * reply_ptr,
						const char * version,
						const char * agent_Id,		//agent_Id -> 'a_number' in dbs agent table
						const char * passWord,
						const char *ext,
						const char *email_account,
						const char *email_password){return BSWRC_OK;} 
virtual RetCode_t ETASStilAlive(IsmServiceReply_c * reply_ptr, Ulong_t client_Id){return BSWRC_OK;} 
virtual RetCode_t ETASOffLine(Ulong_t client_Id){return BSWRC_OK;} 
virtual RetCode_t RegisterForClientStateNotifications(IsmServiceReply_c * reply_ptr, Ulong_t client_Id){return BSWRC_OK;} 
virtual RetCode_t UnRegisterForClientStateNotifications(IsmServiceReply_c * reply_ptr, Ulong_t client_Id){return BSWRC_OK;}   
virtual  RetCode_t SaveETASIniCfg(IsmServiceReply_c * reply_ptr, BswStream_c&  stream){return BSWRC_OK;} 
virtual  RetCode_t ConfirmVisor(IsmServiceReply_c * reply_ptr, const char * name, const char * password){return BSWRC_OK;} 
//******** WEB NLA interface *************************
virtual  RetCode_t LoginWebVisorSync(const char * name,const char * password,const char * session_id,const char * ip_address,const char * server_ip_address,RetCode_t &executionResult,Ulong_t &client_Id,Ushort_t &sup_level);
virtual  RetCode_t LogoutWebVisorSync(Ulong_t client_Id);
}; 



#endif	// GSOAP_WEB_SM_SYN_H
