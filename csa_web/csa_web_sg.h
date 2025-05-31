#ifndef GSOAP_WEB_SUPERGROUP_TAB_H
#define GSOAP_WEB_SUPERGROUP_TAB_H
#include "acsasta.hpp"
#include "csa_web_main.h"
#include "csa_web_lock.hpp"
#include "csa_web_groups.hpp"

#include <map>

class SuperGroups_tab : public CSA_WEB_LOCK_obj{

//============================ Public Section ========================
public:

  class SGroupsInfo_c
  {
  public:
    void setSGroupId(Ulong_t  sgrpId){
      sgrpId = sgrpId;
    }
    void setSGroupName(string sg_name){
      sgrpName = sg_name;
    }
    Ulong_t getSGroupId(){
      return sgrpId;
    }
    string getSGroupName(){
      return sgrpName;
    }

    void ClearGroupList(){
      tab.clear();
    }
    void InsertGroup(Ulong_t grp_id){
      tab.push_back(grp_id);
    }
    vector<Id_type> getGroupList(){
      return tab;
    }

  private:
    Ulong_t     sgrpId;
    string      sgrpName;
    typedef  vector<Id_type> group_id_data_type;// ID, NAME
    //typedef  map<Id_type,GroupsInfo_t> group_id_data_type;// ID, NAME
    group_id_data_type tab;  
    group_id_data_type::iterator it;
  };

  // ctor
  SuperGroups_tab(){ 
    tab.clear();
    LoadList();
    isMonitored= false;
  };

  //dstr
  ~SuperGroups_tab(){};

  //20-Dec-2020 YR BZ#53247
  void    LoadList(Ushort_t user_id = ER_NULL_ID, Ushort_t sup_level = 0xffff);
  void    LoadGroups(Ulong_t Id);
  string  GetName(Ulong_t Id);
  //26-Oct-2021 YR BZ#55354
  bool    IsSuperGroupExist(Ulong_t Id);
  bool    isMonitor(void);

  typedef  map<Id_type,SGroupsInfo_c> sgroup_id_data_type;// ID, NAME
  sgroup_id_data_type tab;  
  sgroup_id_data_type::iterator it;
private:
  bool isMonitored;
};

#endif
