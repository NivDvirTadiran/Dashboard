#ifndef GSOAP_WEB_IVRAPP_TAB_H
#define GSOAP_WEB_IVRAPP_TAB_H
#include "acsasta.hpp"
#include "csa_web_main.h"
#include "csa_web_lock.hpp"

#include <map>
class IVRapp_tab: public CSA_WEB_LOCK_obj{
	
//============================ Public Section ========================
public:
  // ctor
	IVRapp_tab()
	 { 
		 tab.clear();
		 LoadList();
		 isMonitored= false;
     };
  //dstr
	~IVRapp_tab(){};
	
    void LoadList(void);
	string GetName(unsigned int Id);
	map<Id_type,string> tab;  // ID, NAME
     map<Id_type,string>::iterator it;
	bool isMonitor(void);
private:	
	bool isMonitored;
};
class IVRgrpport_tab : public CSA_WEB_LOCK_obj{
	
//============================ Public Section ========================
public:
  // ctor
	IVRgrpport_tab()
	 { 
		 tab.clear();
		 LoadList();
		 isMonitored= false;
     };
  //dstr
	~IVRgrpport_tab(){};
	
    void LoadList(void);
	string GetName(unsigned int Id);
	map<Id_type,string> tab;  // ID, NAME
     map<Id_type,string>::iterator it;
	bool isMonitor(void);

private:	
	bool isMonitored;
	
};
#endif
	
    
