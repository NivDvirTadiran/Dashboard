#ifndef GSOAP_WEB_GROUPS_TAB_H
#define GSOAP_WEB_GROUPS_TAB_H
#include "acsasta.hpp"
#include "csa_web_main.h"

#include "csa_web_lock.hpp"



typedef struct GroupsInfo_s
{
  string        grpName;
  string        grpNumber;
  unsigned int  grpEmailEnable;
}GroupsInfo_t;


class Groups_tab : public CSA_WEB_LOCK_obj{
	
//============================ Public Section ========================
public:
  // ctor
  Groups_tab()
  { 
    tab.clear();
    LoadGroupList();
  };
  //dstr
  ~Groups_tab(){};

  //20-Dec-2020 YR BZ#53247
  void LoadGroupList(Ushort_t user_id = ER_NULL_ID, Ushort_t sup_level = 0xffff);

  string GetGroupNameById(unsigned int  grpId);
  string GetGroupNumberById(unsigned int  grpId);
  bool   IsGroupExist(unsigned int grpId);
  typedef  map<Id_type,GroupsInfo_s> group_id_data_type;// ID, NAME
  group_id_data_type tab;  
  group_id_data_type::iterator it;
};
//---------------------------------------------------------------


#endif
