#include "er_std_override.h"
#include "csa_gsoap_th_mngr.h"
#include "csa_web_trace.hpp"
#include "csa_web_commondefs.hpp"


//==========================[ Static Variables ]======================
static BswLog_c csa_web_TRACE_Map_log_i(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_INFO);
static CsaGsoapThMngr_c&  csa_gsoap_th_mngr = CsaGsoapThMngr_c::GetCsaGsoapThMngr();
//static CsaWebConsoleMonitor_c& console_mon = CsaWebConsoleMonitor_c::GetMonitor();



//===========[ TRACE_Map_c::Implemented Methods ]=====================

/*------------------------[ AddAplication ]-------------------------*/
void TRACE_Map_c::AddAplication(unsigned long ip, int port, std::string  application_id)
{
  csa_web_TRACE_Map_log_i << "TRACE_Map_c::AddAplication from Addr: " << ip << ":" << port << " with id: " << application_id.c_str() << LOG_TERMINATOR;

  unsigned short ipa[4];
  char temp_buf_ip[20];
  char temp_buf_port[10];
  trace_data_t *new_trace_data;
  //er_fprintf(stderr,"LOCK TRACE aa{\n");
  if(!csa_web_TryLock()) //MT
    return;

  new_trace_data = (trace_data_t*)new trace_data_t();

  ipa[0] =(unsigned short)((ip >> 24)&0xFF);
  ipa[1] =(unsigned short)((ip >> 16)&0xFF);
  ipa[2] =(unsigned short)((ip >> 8)&0xFF);
  ipa[3] =(unsigned short)(ip &0xFF);
  er_sprintf(temp_buf_ip, "%d.%d.%d.%d",ipa[0],ipa[1],ipa[2],ipa[3]);
  er_sprintf(temp_buf_port, "%d",port);
  new_trace_data->ip_addres = temp_buf_ip;
  new_trace_data->application_id = application_id;

  new_trace_data->current_number_of_connections = 0;
  new_trace_data->last_error_code = 0;
  new_trace_data->last_keep_alive= 0;
  new_trace_data->n_last_request_conn = 0;
  new_trace_data->n_monitors = 0;
  new_trace_data->n_total_conn = 0;
  new_trace_data->n_visors = 0;
  new_trace_data->n_web_visors = 0;
  tab[ip] = new_trace_data;
  //er_fprintf(stderr,"} UNLOCK TRACE aa\n");
  csa_web_TRACE_Map_log_i << "TRACE_Map_c - Size of application map: " << tab.size() << LOG_TERMINATOR;
  this->csa_web_UnLock();//MT
  csa_gsoap_th_mngr.ApplicationRegister(application_id,temp_buf_ip,temp_buf_port);
  return;
}


/*------------------------[ AddAplication ]-------------------------*/
bool TRACE_Map_c::RemoveAplication(std::string  application_id)
{
  csa_web_TRACE_Map_log_i << "TRACE_Map_c::RemoveAplication with id " << application_id.c_str()
                          << " (tab size before: " << tab.size() << ") - ";

  if(!csa_web_TryLock())
  {
    csa_web_TRACE_Map_log_i << " - ERROR csa_web_TryLock FAIL" << LOG_TERMINATOR;
    return false;
  }

  if(tab.size() > 0)
  {
    for(it = tab.begin(); it != tab.end(); ++it)
    {
      if(it->second->application_id == application_id)
      {
        csa_web_TRACE_Map_log_i << it->second->application_id.c_str() << " Found in tab map " << LOG_TERMINATOR;
        tab.erase(it);
        it = tab.begin();
        csa_web_UnLock();//MT
        csa_web_TRACE_Map_log_i << " - OK (tab size after: " << tab.size() << ")" << LOG_TERMINATOR;
        return true;
      }
    }
    csa_web_UnLock();//MT
    csa_web_TRACE_Map_log_i << " - NOT FOUND" << LOG_TERMINATOR;
    return true;
  }
  else
  {
    csa_web_UnLock();//MT
    csa_web_TRACE_Map_log_i << " - MAP is EMPTY" << LOG_TERMINATOR;
    return true;
  }

  csa_web_UnLock();//MT
  csa_web_TRACE_Map_log_i << " - ERROR" << LOG_TERMINATOR;
  return false;
}


/*------------------------[ ZeroAplication ]------------------------*/
void TRACE_Map_c::ZeroAplication(unsigned long ip)
{
  csa_web_TRACE_Map_log_i << "TRACE_Map_c::ZeroAplication IP: " << ip << LOG_TERMINATOR;

  //er_fprintf(stderr,"LOCK TRACE za{\n");
  if(!csa_web_TryLock()) //MT
    return;
  it =tab.find(ip);
  if(it != tab.end())
  {
    it->second->current_number_of_connections = 0;
    it->second->last_error_code = 0;
    it->second->last_keep_alive = 0;
    it->second->n_last_request_conn = 0;
    it->second->n_monitors = 0;
    it->second->n_total_conn = 0;
    it->second->n_visors = 0;
    it->second->n_web_visors = 0;
  }
  //er_fprintf(stderr,"}UNLOCK TRACE za\n");
  this->csa_web_UnLock();//MT
  return;
}


/*------------------------[ AddKeepAlive ]--------------------------*/
void TRACE_Map_c::AddKeepAlive(unsigned long ip)
{
  return;
  //er_fprintf(stderr,"LOCK TRACE ka{\n");
  if(!csa_web_TryLock()) //MT
    return;
  it = tab.find(ip);
  if(it != tab.end())
  {
    it->second->last_keep_alive = time( NULL);
  }
  //er_fprintf(stderr,"} UNLOCK TRACE ka\n");
  this->csa_web_UnLock();//MT
  return;
}


/*------------------------[ ChangeNumberByLicenceType ]-------------*/
void TRACE_Map_c::ChangeNumberByLicenceType(unsigned long ip,unsigned short  license_type, bool insert)
{
  csa_web_TRACE_Map_log_i << "TRACE_Map_c::ChangeNumberByLicenceType IP: " << ip << LOG_TERMINATOR;

  //er_fprintf(stderr,"LOCK TRACE chn{\n");
  if(!csa_web_TryLock()) //MT
    return;
  it =tab.find(ip);
  if(it != tab.end())
  {
    switch (license_type)
    {
    case FULL_VISOR:
      if(insert)
        it->second->n_visors ++;
      else
        it->second->n_visors --;
      break;
    case MONITOR_ONLY_VISOR:
      if(insert)
        it->second->n_monitors ++;
      else
        it->second->n_monitors --;
      break;
    case FULL_WEB_VISOR:
      if(insert)
        it->second->n_web_visors ++;
      else
        it->second->n_web_visors --;
      break;

    }
    //er_fprintf(stderr,"} UNLOCK TRACE chn\n");
    this->csa_web_UnLock();//MT
    return;
  }
}


/*------------------------[ StartConnection ]---------------------*/
void  TRACE_Map_c::StartConnection(unsigned long ip)
{
  return;
  //er_fprintf(stderr,"LOCK TRACE StartCon {\n");
  if(!csa_web_TryLock()) //MT
    return;
  it =tab.find(ip);
  if(it != tab.end())
  {
    if(it->second->n_total_conn == 65000)
      it->second->n_total_conn = 0;

    if(it->second->n_last_request_conn == 65000)
      it->second->n_last_request_conn = 0;

    it->second->n_last_request_conn ++;
    it->second->n_total_conn++;
    it->second->current_number_of_connections++;
    //er_fprintf(stderr,"} UNLOCK TRACE StartCon\n");
    this->csa_web_UnLock();//MT
  }
  else
  {   //er_fprintf(stderr,"} UNLOCK TRACE StartCon\n");
    this->csa_web_UnLock();//MT
    AddAplication(ip,0,"NOTHING");
  }
  return;
}


/*------------------------[ EndConnection ]-----------------------*/
void  TRACE_Map_c::EndConnection(unsigned long ip,unsigned short error_code)
{
  return;
  if(!csa_web_TryLock()) //MT
    return;
  it =tab.find(ip);
  if(it != tab.end())
  {
    it->second->current_number_of_connections--;
    if(error_code != 0)
      it->second->last_error_code;
  }
  //er_fprintf(stderr,"} End Connection \n");
  this->csa_web_UnLock();//MT
  return;
}



//void TRACE_Map_c::AddDataToArray(unsigned long ip)
//{
////ns1__StatItemType *tmp;
////
////
//// if(tab.size() > 0){
////   for(it=tab.begin();it!= tab.end(); ++it)
////   {
////     tmp = soap_new_ns1__StatItemType(soap, 1);
////
////     tmp->ipAddress = it->second->ip_addres;
////     tmp->CurrentNumberOfSoapConnections = it->second->current_number_of_connections;
////     tmp->applicationId = it->second->application_id;
////     tmp->LastError = it->second->last_error_code;
////     tmp->lastKeepAliveSecAgo = NlaCalcCsaElapsedTime(it->second->last_keep_alive);
////
////     tmp->numberOfSoapConnectionsSinceLastStatRequest = it->second->n_last_request_conn;
////     it->second->n_last_request_conn = 0;
//// 
////     tmp->numOfMonitors = it->second->n_monitors;
////     tmp->numOfVisors = it->second->n_visors;
////     tmp->numOfWebVisors = it->second->n_web_visors;
////     tmp->totalNumberOfSoapConnections = it->second->n_total_conn;
////
////     soap->m_net_data_ptr->STAT_vector.push_back(tmp);
////   }
//// }
//
//}

