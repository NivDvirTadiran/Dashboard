#include <er_std_override.h>
#include "csa_web_online_srv_th.h"

extern Ushort_t do_reports_area[];


WebNlaServeThread_c::WebNlaServeThread_c(Ushort_t id)
{
  //Todo: Add initializations here
  m_id = id;
  m_log.SetLogSite(BswMakeLogSite(FC_CSA_WEB,1));
  m_log.SetLogMask(LM_PUBLIC);
  m_log.SetLogLevel(LOGL_INFO);
  m_log << "Ctor of WebNlaServeThread_c called with id [" << m_id << "]\n" << LOG_TERMINATOR;
}


WebNlaServeThread_c::~WebNlaServeThread_c(void)
{
  //Todo: Add termination here
  DeleteWaitableObject(m_event);
}


RetCode_t WebNlaServeThread_c::CreateThread()
{
  RetCode_t rc = BswOnlineThread_c::CreateThread();

  m_log(LOGL_INFO) << "WebNlaServeThread_c::CreateThread called for thread [" << m_id << "]\n" << LOG_TERMINATOR;
  m_event = CreateNewEvent();
  //m_status = FINISHED_SUCCESSFULLY; //status will be changed on connect

  return rc;
}

void WebNlaServeThread_c::SignalEvent()
{
  m_log(LOGL_INFO) << "WebNlaServeThread_c::SignalEvent called for thread [" << m_id << "]\n" << LOG_TERMINATOR;
  BswOnlineThread_c::SignalEvent(m_event);
}


RetCode_t WebNlaServeThread_c::MyMainFunction()
{
  RetCode_t rc = BSWRC_OK;
  SrvNetData_c *net_data;
  CsaWebServer *SoapPtr;
  net_data = new SrvNetData_c();

  er_fprintf(stderr, "%d : THREAD: STARTED \n",m_manager_iface->GetIndex());

  //Do your main loop here
  while(true) //dwe
  {
    m_log(LOGL_INFO) << "WebNlaServeThread_c thread [" << m_id << "] is now WAITING for job\n" << LOG_TERMINATOR;
    WaitForObject(m_event,MAX_TIMEOUT);
    m_log(LOGL_INFO) << "WebNlaServeThread_c thread [" << m_id << "] is now WORKING on job\n" << LOG_TERMINATOR;

    //To tell the main thread that we're now working on a job:
    m_manager_iface->GetCriticalSection().Lock();

    if ((SoapPtr = m_manager_iface->GetSoap()) != NULL)
    {
      //char address[20];
      //er_sprintf(address, "%p", SoapPtr);
      //er_fprintf(stderr, "WebNlaServeThread_c::MyMainFunction() calling EMISwebBindingService_init (%s)\n",address);

      //27-Jul-2021 YR BZ#54709 - EMISwebBindingService_init is called only from here
      SoapPtr->EMISwebBindingService_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
      SoapPtr->m_net_data_ptr = net_data;
      SoapPtr->m_db_data_ptr = m_db_data;
      SoapPtr->m_net_data_ptr->CreateResponseHeader(SoapPtr);
      SoapPtr->ThreadIndex = m_manager_iface->GetIndex();
    }

    try
    {
      //printf("++++++++++time: %d before SoapPtr->serve  thread id: %d in work +++++++++++++++++++++++++++\n",time(0) , SoapPtr->ThreadIndex);
      if (SoapPtr->serve() != SOAP_OK) // process RPC request
      {
        soap_print_fault(SoapPtr, stderr); // print error
      }
      else
      {
      }
    }
    catch(...)
    {
      er_printf("!!!!! FAILED to serve connection : accepted connection from IP=%d.%d.%d.%d socket=%d \n",
                (SoapPtr->ip >> 24)&0xFF, (SoapPtr->ip >> 16)&0xFF, (SoapPtr->ip >> 8)&0xFF, SoapPtr->ip&0xFF, SoapPtr->socket);
      er_fprintf(stderr, "!!!!!! FAILED to serve connection : accepted connection from IP=%d.%d.%d.%d socket=%d \n",
                (SoapPtr->ip >> 24)&0xFF, (SoapPtr->ip >> 16)&0xFF, (SoapPtr->ip >> 8)&0xFF, SoapPtr->ip&0xFF, SoapPtr->socket);
    }

    int reportCode = SoapPtr->m_ReportCode;
    SoapPtr->m_ReportCode = 0;
    Sleep(5);
    //    printf("            ++++++++++time: %d after sleep 10 thread id: %d in work ++++++++++++++++++++++++++\n", time(0),m_manager_iface->GetIndex());


    SoapPtr->CleanAll();
    delete SoapPtr;
    SoapPtr = NULL;

    //SoapPtr->m_net_data_ptr->CleanAll(SoapPtr,m_manager_iface->GetIndex());
    //SoapPtr->destroy(); // calls soap_destroy() and soap_end()
    //soap_destroy((struct soap*)SoapPtr); // clean up class instances
    //soap_end((struct soap*)SoapPtr); // clean up everything and close socket
    //delete SoapPtr;

    if (reportCode != 0 && reportCode <= SuperGroupInfo_E && do_reports_area[reportCode] == 1) //zero report in process flag
    {
      // printf("            ++++++++++time: %d after SoapPtr->serve zero report area[%d]  thread id: %d in work ++++++++++++++++++++++++++\n", time(0),reportCode , m_manager_iface->GetIndex());
      do_reports_area[reportCode] = 0;
    }
    if (rc == BSWRC_OK)
    {
      m_manager_iface->SetStatus(InteractWithManager_c::FINISHED);
    }
    else
    {
      m_manager_iface->SetStatus(InteractWithManager_c::FINISHED);
    }

    m_manager_iface->GetCriticalSection().Unlock();
  }//end while
}

