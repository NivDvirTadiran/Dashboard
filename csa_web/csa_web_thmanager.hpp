#ifndef CSA_WEB_TH_MNGR_H  
#define CSA_WEB_TH_MNGR_H   

//#include <lrl_evt.h>
//#include "c2g_edb_defs.h"
//
//#include <dbi_edbmanager.h>
//#include <sea_provider.h>
//#include "c2g_sea_reply.h"
//#include "c2g_evt_id_factory.h"
//#include "c2g_db_update_manager.h"
//
//// automatic update includes
//#include <bswobjectdump.h>
//#include <bswattribdescr.h>
//#include <adupdate.h>
#include <er_queue.h>
#include <er_ptr_queue.h>
#include "csa_web_serverMT.hpp"
#include "csa_web_srv_th_interface.h"
#include <bswtimer.h>

typedef ErQueue_c<Ushort_t> csa_web_available_threads_que_t;

class  ThMngr_c 
{
  private:
    // ctor
    ThMngr_c(){};
    // dtor
    ~ThMngr_c(){};

//============================ Public Section ========================
  public:
    static ThMngr_c& GetThMngr();
    RetCode_t Load();
    RetCode_t Init();
    RetCode_t ProcessSoapConnection(CsaWebServer *m_soap);
    RetCode_t RemoveAplication(std::string application_id);


//============================ Private Section ========================
private:
  ////***********************online threads management
  csa_web_threads_map_t           m_threads_map;
  csa_web_available_threads_que_t m_available_threads_map;
  Ulong_t                         m_sp_num_threads;
  SrvDBData_c*                    m_db_data;

}; // class C2gMngr_c

#endif 
