#ifndef WEB_ONLINE_SERVER_THREAD_H
#define WEB_ONLINE_SERVER_THREAD_H

//*******************************************
#include <bswcritsection.h>
#include <bswonlinethread.h>
#include "csa_web_srv_th_interface.h"

class InteractWithManager_c;

class WebNlaServeThread_c : public BswOnlineThread_c
{
  public:
    //inherited from BswOnlineThread_c
    WebNlaServeThread_c(Ushort_t id);
    ~WebNlaServeThread_c();
    RetCode_t   CreateThread();
    RetCode_t   MyMainFunction();

    void        SetParameters(InteractWithManager_c* main_thread_iface, SrvDBData_c  *db_data) 
    {
      m_manager_iface = main_thread_iface;
      m_db_data = db_data;
    }

    void SignalEvent();
    //p  RetCode_t ConnectToEdb(void);

  private:
    //Those two members are used to interact with the main thread. m_event is used as an sync object. 
    //m_main_thread_iface contains critical section object and also queue to holds the data needed to write
    //to db.
    InteractWithManager_c* m_manager_iface;
    Ushort_t               m_event;
    SrvDBData_c           *m_db_data;

    Ushort_t              m_id; // id of thread
    BswLog_c              m_log;

};

#endif
