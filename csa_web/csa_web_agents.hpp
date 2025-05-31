#ifndef GSOAP_WEB_AGENTS_TAB_H
#define GSOAP_WEB_AGENTS_TAB_H
#include "acsasta.hpp"
#include "csa_web_main.h"

#include "csa_web_lock.hpp"

class Agents_tab: public CSA_WEB_LOCK_obj{

//============================ Public Section ========================
public:
  // ctor
  Agents_tab()
  { 
    tab.clear();
    LoadAgentsList();
  };
  //dstr
  ~Agents_tab(){};

  void LoadAgentsList(void);
  typedef struct AgentsInfo_s
  {
    Id_type       a_id;
    string        a_name;
    string        a_number;
  }AgentsInfo_t;

  typedef map<Id_type,AgentsInfo_t> agent_id_data_type;

  agent_id_data_type tab;  // ID, NAME
  agent_id_data_type::iterator it;


};


#endif
