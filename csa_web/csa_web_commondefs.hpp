#ifndef CSA_WEB_COMMON_H
#define CSA_WEB_COMMON_H

#include "argroup.hpp"
#include "asutil.hpp"
#include "ardnis.hpp"
#include "arsupgrp.hpp"
#include <time.h>
#include <vector>

extern CsaSetup&          Csa_Setup;     
#define mymax(a,b)            (((a) > (b)) ? (a) : (b))   // taken from WINDEFS.H
extern GroupTable*        Csa_Grps_Tbls[];                // ARGROUP.HPP   ARMAIN.CPP
extern AgentTable*        Csa_Agents_Tbls[];              // ARAGENT.HPP   ARMAIN.CPP
extern SuperGroupsTable*  Csa_Super_Grp_Tbl[];              // ARSUPGRP.HPP  ARMAIN.CPP
extern CsaGeneralDNIS_Table *Csa_Dnis_Tbls[];             // ARDNIS.HPP    ARMAIN.CPP

static  time_t  csaTimeNow;


time_t NlaCalcCsaElapsedTime( time_t csa_time);
string NlaConvertTimeToString (time_t csa_time);
string NlaConvertSecondsToStringMMSS (time_t csa_time);
string Ulong2String(unsigned long ul);
void   Ulong2String(Ulong_t ul, string &ret_str);

//------------------------------------------------------------------------------------------------

#endif

