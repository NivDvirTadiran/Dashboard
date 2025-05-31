#include "er_std_override.h"
#include "csa_web_lock.hpp"


/*------------------------[ csa_web_TryLock ]-----------------------*/
bool CSA_WEB_LOCK_obj::csa_web_TryLock(void)
{
  int n =0;
  bool  Locked = false; //MT

  //char address[20];
  //er_sprintf(address, "%p", this);

  while(n < 200) //MT
  {
    if( m_crit.Trylock() == true)
    {
      Locked = true;
      //if(n >0) er_fprintf(stderr,"TryLock - ok:%d\n",n);
      //er_fprintf(stderr,"%s: csa_web_TryLock - OK (n=%d)\n",address,n);
      return Locked;
    }
    //er_printf("*");
    Sleep(5);
    n++;
  }

  if(n>0)
    er_fprintf(stderr,"csa_web_TryLock - FAIL (n=%d)\n",n);
    //er_fprintf(stderr,"%s: csa_web_TryLock - FAIL (n=%d)\n",address,n);

  return Locked;
}


/*------------------------[ csa_web_TryLockInfinite ]---------------*/
void CSA_WEB_LOCK_obj::csa_web_TryLockInfinite(void)
{
  int n =0;
  bool  Locked = false; //MT

  //char address[20];
  //er_sprintf(address, "%p", this);

  while(!Locked) //MT
  {
    if( m_crit.Trylock() == true)
    {
      Locked = true;
      //er_fprintf(stderr,"%s: csa_web_TryLockInfinite - OK (n=%d)\n",address,n);
      //if(n>=100)
      //  er_fprintf(stderr,"TryLockInfinite (n=%d)\n",n);
      return;
    }
    Sleep(5);
    n++;
    //er_printf("%");
  }  
  er_fprintf(stderr,"csa_web_TryLockInfinite - FAIL (n=%d)\n",n);
  //er_fprintf(stderr,"%s: csa_web_TryLockInfinite - FAIL (n=%d)\n",address,n);
}


/*------------------------[ csa_web_UnLock ]------------------------*/
void CSA_WEB_LOCK_obj::csa_web_UnLock(void)
{
  //char address[20];
  //er_sprintf(address, "%p", this);
  //er_fprintf(stderr,"%s: csa_web_UnLock\n",address);
  m_crit.Unlock();//MT
}

