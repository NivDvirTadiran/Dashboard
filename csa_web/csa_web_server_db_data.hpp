#ifndef CSA_WEB_SERVER_DB_DATA_MT_H
#define CSA_WEB_SERVER_DB_DATA_MT_H
#include "acsasta.hpp"
#ifdef WIN32
#include "windows/soapEMISwebBindingService.h"
#else
#include "linux/soapEMISwebBindingService.h"
#endif


#include "csa_web_init.h"

#include "csa_web_limits.hpp"
#include "csa_web_main.h"
#include "csa_web_agents.hpp"
#include "csa_web_groups.hpp"
#include "csa_web_dnis.hpp"
#include "csa_web_ivrapp.hpp"
#include "csa_web_mailbox.hpp"
#include "csa_web_sg.h"
#include "csa_web_ticket.hpp"
#include "csa_web_trace.hpp" //PVL_STAT
#include <bswcritsection.h>

class SrvDBData_c
{
  public:
    SrvDBData_c();
    ~SrvDBData_c();

    void GetData(Ushort_t index); //TEST

    State_colors_tab colortab; //NO need for lock, initalisation at beggining
    Limits_tab       limitstab; //MT
    Agents_tab       agentstab; //MT
    Groups_tab       groupstab; //MT
    DNIS_tab         dnistab; //MT
    IVRapp_tab       ivrapptab; //MT
    IVRgrpport_tab   ivrgrptab; //MT
    MAILbox_tab      mailboxacctab;//MT
    SuperGroups_tab  supergrouptab;//MT

    TICKET_tab       TicketTab; //MT
    TRACE_Map_c      TraceTab;  //PVL_STAT
  private:
    BswCritSection_c      m_crit;
};
#endif

