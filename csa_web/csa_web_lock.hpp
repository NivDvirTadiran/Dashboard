#ifndef GSOAP_WEB_LOCK_H
#define GSOAP_WEB_LOCK_H

#include "acsasta.hpp"
#include "csa_web_main.h"
#include <bswcritsection.h>


class CSA_WEB_LOCK_obj
{
//============================ Public Section ========================
public:
  // ctor
  CSA_WEB_LOCK_obj(){};
  //dstr
  ~CSA_WEB_LOCK_obj(){};

  bool csa_web_TryLock(void);
  void csa_web_TryLockInfinite(void);
  void csa_web_UnLock(void);

private:
  BswCritSection_c      m_crit;
};
#endif

