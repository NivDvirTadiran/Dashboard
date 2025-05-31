
#include <proj_gen.h>
#include <bswlog.h>

#include "csa_web_sm_syn.hpp"

RetCode_t CsaWebSMsyn_c::LoginWebVisorSync(const char * name,const char * password,const char * session_id,const char * ip_address,const char * server_ip_address,RetCode_t &executionResult,Ulong_t &client_Id,Ushort_t &sup_level)
{
  return BSWRC_OK;
}

RetCode_t CsaWebSMsyn_c::LogoutWebVisorSync(Ulong_t client_Id)
{
  return BSWRC_OK;
}
