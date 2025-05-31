#include "er_std_override.h"
#include "csa_web_commondefs.hpp"

time_t   NlaCalcCsaElapsedTime( time_t csa_time)
{ if(csa_time == 0) return 0;
  if(csa_time < 0) return 0;
  return (time( NULL) - csa_time);
} /* NlaCalcCsaElapsedTime */


string NlaConvertTimeToString (time_t csa_time)
{
 string outstr;
 char          *tptr;                          
 static time_t tmp_tm;
 int i;
 char str[10];
                      
  tmp_tm = csa_time;                      
  tptr = er_ctime( &tmp_tm);
  if ( tptr != NULL) {
    for (i = 0; i < 8; i++)
      str[i] = tptr[i+11];
    str[8] = '\0';
  }
  else {
    str[0] = '\0';
  }
  outstr = str;
  return outstr;
}
//------------------------------------------------------
string NlaConvertSecondsToStringMMSS (time_t csa_time)
{
 string outstr;
 unsigned int d =0;
 unsigned int h =0;
 unsigned int m =0;
 unsigned int s =0;
 unsigned int monly =0;

 char buf[20];
  s=csa_time;
 if(s >= 86400) 
	 d = (unsigned int)(s / 86400);

 if(s - (d * 86400) >= 3600 )
  h = (unsigned int)((s - (d * 86400)) / 3600);
 else
  h = 0;

 if(s - (h * 3600) - (d * 86400) >= 60 )
    m = (unsigned int)((s - (h * 3600) - (d * 86400)) / 60);
 else
  m = 0;
 s = s - (m * 60) - (h * 3600) -  (d * 86400);
 monly = m + h*60 + d*1440;
 //er_sprintf(buf,"%d:%d:%d:%d",d,h,m,s);
 er_sprintf(buf,"%d:%d",monly,s);
 outstr = buf;
 return outstr;
}

string Ulong2String (unsigned long ul)
{
  char buf[20];
  er_sprintf(buf,"%ld",ul);
  string outstr(buf);
  //outstr = buf;
  return outstr;
}

void Ulong2String (Ulong_t ul, string &ret_str)
{
 char buf[20];
 er_sprintf(buf,"%ld",ul);
 //ret_str.clear();
 ret_str.assign(buf);
}
