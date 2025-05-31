#include "er_std_override.h"
#include "csa_web_server_db_data.hpp"

static BswLog_c   csa_web_serve_db_data_log_i(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_INFO);


//ctor
SrvDBData_c::SrvDBData_c()
{
  char address[20];
  er_sprintf(address, "%p", this);
  csa_web_serve_db_data_log_i << "Create new SrvDBData_c (Address " << address << ")\n" << LOG_TERMINATOR;
}


//dtor
SrvDBData_c::~SrvDBData_c()
{
  char address[20];
  er_sprintf(address, "%p", this);
  csa_web_serve_db_data_log_i << "Delete SrvDBData_c (Address " << address << ")\n" << LOG_TERMINATOR;
}


/*------------------------[ GetData ]-------------------------------*/
void SrvDBData_c::GetData(Ushort_t index) //TEST
{
  bool  Locked = true;
  while(Locked)
  {   
    if( this->m_crit.Trylock() == true)
    {
      //processing
      //
      er_printf("%d THREAD Process DATA \n",index);
      Sleep(2000);
      this->m_crit.Unlock();
      Locked = false;
    }
    else
    {
      er_printf("%d THREAD Data Busy........ \n",index);
    }
  }

  return;
}
