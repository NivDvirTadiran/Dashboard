#include "er_std_override.h"
#include "csa_web_thmanager.hpp"
#include "csa_web_online_srv_th.h"
#include "csa_web_serverMT.hpp"


//==========================[ Static Variables ]======================
static BswLog_c   csa_web_th_mngr_log_i(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_INFO);
static BswLog_c   csa_web_th_mngr_log_d(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_DETAILED);


//-------------------------------- ThMngr_c thread management ----------------------------------------------
ThMngr_c& ThMngr_c::GetThMngr() //this class is a singleton
{
  static ThMngr_c manager_instance;

  return manager_instance;
}


/*===========================================================================================================
  	Method Name:        ThMngr_c::OnIdle()
  	Description:
  	Authors:
============================================================================================================*/
extern Ushort_t do_reports_area[];

RetCode_t ThMngr_c::ProcessSoapConnection(CsaWebServer *m_soap)
{
  RetCode_t               rc = BSWRC_OK;
  Ushort_t                thread_index = 0xFFFF;
  InteractWithManager_c*  iface_obj_ptr = NULL;
  //Ushort_t avail_thread_index;
  InteractWithManager_c*  iface_obj= NULL;

  char address[20];
  er_sprintf(address, "%p", m_soap);

  csa_web_th_mngr_log_d << "ThMngr_c::ProcessSoapConnection() Invoke (soap address " << address << ")\n" << LOG_TERMINATOR;

  //  struct linger  dont_linger;
  //
  //    dont_linger.l_onoff = 0;
  //    dont_linger.l_linger = 0;
  //    setsockopt(m_soap->socket, SOL_SOCKET, SO_LINGER,(char *)&dont_linger, sizeof(dont_linger));
  //
  //  m_soap->linger_time = 100;
  //  m_soap->accept_flags = SO_LINGER;
  //look for online threads that finished their previous job

  while( BSWRC_OK == rc)
  {
    m_threads_map.ResetGetNext();
    iface_obj_ptr = m_threads_map.GetNext(1, &thread_index);
    // Refresh Available Threads
    while (iface_obj_ptr!=NULL)
    {
      if (iface_obj_ptr->GetCriticalSection().Trylock()==true)
      {
        //WORKING_ON_JOB is not included here on purpose.
        if (iface_obj_ptr->GetStatus() == InteractWithManager_c::FINISHED)
        {                                                                                                //###                                                                                                //###
          iface_obj_ptr->SetSoap(m_soap);
          iface_obj_ptr->SetStatus(InteractWithManager_c::WORKING);
          //printf("++++++++++time: %d, onlIDXId: %d, SoapPtr: %x\n",time(0), iface_obj_ptr->GetIndex() ,m_soap);
          iface_obj_ptr->GetThread()->SignalEvent();
          iface_obj_ptr->GetCriticalSection().Unlock();
          return BSWRC_OK;
          break;
        }
        iface_obj_ptr->GetCriticalSection().Unlock();
      }
      iface_obj_ptr = m_threads_map.GetNext(1,&thread_index);
    }//inner while look for free online thread

    //take out one of the available on line threads.
    //if(m_available_threads_map.GetSize() == 0)
    //{
    Sleep(50);//not found free online thread before check again
    //   continue;
    //}
  }

  //   rc = m_available_threads_map.Pop(avail_thread_index);
  //	if(BSWRC_FAIL == rc )
  //	{
  //		rc = BSWRC_OK;
  //		continue;
  //	}
  //	else
  //	{
  //		//the thread is available so ?there is no need to synch?
  //		iface_obj = m_threads_map.Get(avail_thread_index);
  //		iface_obj->SetSoap(m_soap);
  //
  //		iface_obj->SetStatus(InteractWithManager_c::WORKING);
  //		printf("++++++++++time: %d, onlIDXId: %d, available size: %d, SoapPtr: %x\n",time(0), avail_thread_index, m_available_threads_map.GetSize(),m_soap);
  //		iface_obj->GetThread()->SignalEvent();
  //		break; //exit first while loop
  //	}
  //  }

  return rc;
}


//================ INIT ========================
RetCode_t ThMngr_c::Init()
{
  RetCode_t     rc = BSWRC_OK;
  RetCode_t     executionResult = BSWRC_OK;
  Ulong_t num_of_threads;

  num_of_threads = 10;// NUM_OF_THREADS;

  csa_web_th_mngr_log_i << "ThMngr_c::Init() Invoke, num_of_threads = " << num_of_threads << "\n" << LOG_TERMINATOR;

  memset (&do_reports_area[0],0, sizeof(Ushort_t) * (SuperGroupInfo_E + 1));

  //threads management
  WebNlaServeThread_c* thread;
  BswThreadNoQueParams_s* params;
  InteractWithManager_c* iface_obj_ptr;

  m_db_data  = new SrvDBData_c(); // single instance of Entity Tables, Colors and Ticket Tabs;

  FILE * fp = er_fopen ("emis_web.txt", "w+");
  fprintf(fp, "******* ThMngr_c::Init -> num_of_threads : %d\r\n", num_of_threads);
  fclose(fp);

  for (int i=0; i < (int)num_of_threads; i++)
  {
    thread = new WebNlaServeThread_c(i);
    if (thread)
    {
      params = thread->GetThreadParams();
      params->thread_ptr = thread;
      params->priority = 1;
      params->thread_type = SYNC_THREAD_NO_SUPERVISION;
      params->cyclic_timer = 0;
      rc = thread->CreateThread();

      iface_obj_ptr =  new InteractWithManager_c(i, thread);
      iface_obj_ptr->SetIndex(i);
      thread->SetParameters(iface_obj_ptr,m_db_data);
    }

    if (iface_obj_ptr) {
      m_threads_map.Insert(i, iface_obj_ptr);
      //m_available_threads_map.Push(i);
    }
    thread->ResumeThread();
  }
  return rc;
}


//================ INIT ========================
RetCode_t ThMngr_c::RemoveAplication(std::string application_id)
{
  RetCode_t     rc = BSWRC_OK;

  csa_web_th_mngr_log_i << "ThMngr_c::RemoveAplication " << application_id.c_str() << LOG_TERMINATOR;

  if( m_db_data->TraceTab.RemoveAplication(application_id) )
  {
    m_db_data->TicketTab.LogoutUsersPerApplicationIdSM(application_id);
    //1-May-2023 YR BZ#57777
    //m_db_data->TicketTab.RemoveAplication(application_id);
  }
  else
  {
    rc = BSWRC_FAIL;
  }


  return rc;
}
