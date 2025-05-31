#ifndef GSOAP_WEB_DNIS_TAB_H
#define GSOAP_WEB_DNIS_TAB_H
#include "acsasta.hpp"
#include "csa_web_main.h"
#include "csa_web_lock.hpp"
#include <map>


class DNIS_tab : public CSA_WEB_LOCK_obj{

//============================ Public Section ========================
public:
  // ctor
  DNIS_tab(){
    tab.clear();
    LoadDNISList();
  };

  //dstr
  ~DNIS_tab(){};

  void LoadDNISList(void);
  typedef struct DNISInfo_s
  {
    string        dnis_name;
    string        dnis_number;
    unsigned int  is_email;
  }DNISInfo_t;

  string GetName(Id_type   Id);
  string GetNumber(Id_type   Id);
  bool isDNISbyID(Id_type Id, Ulong_t Is_email_enable);
  //bool IsDNISbyNumber(char * Number,bool Is_email_enable);
  map<Id_type,DNISInfo_t> tab;  // ID, NAME
  map<Id_type,DNISInfo_t>::iterator it;

};
//---------------------------------------------------------------


#endif
