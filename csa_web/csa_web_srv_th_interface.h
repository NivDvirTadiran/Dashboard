#ifndef GSOAP_CONN_INFO_S 
#define GSOAP_CONN_INFO_S 
#include <er_ptr_map.h>
#include <bswcritsection.h>

#include "csa_web_serverMT.hpp"

class WebNlaServeThread_c;
class InteractWithManager_c
{
public:
	enum {
		WORKING,
		FINISHED		
	};
    ~InteractWithManager_c();

    InteractWithManager_c(Ushort_t          index,
		                     WebNlaServeThread_c* thread);

	void SetStatus(Ushort_t status) {m_status=status;}
	void SetIndex(Ushort_t index) {m_index=index;}
	void SetThread(WebNlaServeThread_c* thread) {m_thread = thread;}
    void SetSoap(CsaWebServer* soap_obj) {m_soap=soap_obj;}
	BswCritSection_c&     GetCriticalSection(void) {return m_crit;}
	Ushort_t              GetStatus(void)          {return m_status;}
	Ushort_t              GetIndex(void)           {return m_index;}
	WebNlaServeThread_c*  GetThread(void)          {return m_thread;}
	CsaWebServer* GetSoap(void)           {return m_soap;}


private:
	BswCritSection_c      m_crit;
	Ushort_t              m_index;
	Ushort_t              m_status; //one of WORKING_ON_JOB,FINISHED_FAILED or FINISHED_SUCCESSFULY
	WebNlaServeThread_c*     m_thread;
	CsaWebServer *m_soap;
};
typedef ErPtrMap_c<Ushort_t, InteractWithManager_c> csa_web_threads_map_t;



#endif
