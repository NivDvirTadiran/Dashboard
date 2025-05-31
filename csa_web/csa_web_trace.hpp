#ifndef GSOAP_WEB_TRACE_H
#define GSOAP_WEB_TRACE_H
/*************************************************************************
IP address string  ,
Application id string,
Keep Alive Receved sec.  ago,
Number of  Loged Visors,
Number of  Logged WebVisors,
Number of  Logged Monitors ,
Number of  SOAP connections  served since start , will be zero after 10000 requests  
Number of  SOAP connections since last information Request
Number of SOAP connections  at progress 
Last error code , logged for 
**************************************************************************/
#include "csa_web_lock.hpp"
#include <map>


typedef struct TRACE_DATA_TYPE
{
  std::string     ip_addres;            //string IP address string  ,    
  std::string     application_id;       //application id
  time_t          last_keep_alive;      //Keep Alive Receved 
  unsigned short  n_visors;             //Number of  Loged Visors,
  unsigned short  n_web_visors;         //Number of  Logged WebVisors,
  unsigned short  n_monitors;           //Number of  Logged Monitors ,
  unsigned long   n_total_conn;         //Number of  SOAP connections  served since start , will be zero after 10000 requests  
  unsigned short  n_last_request_conn;  //Number of  SOAP connections since last information Request
  unsigned short  current_number_of_connections;//Number of SOAP connections  at progress 
  unsigned short  last_error_code;      //Last error code
}trace_data_t;


class TRACE_Map_c : public CSA_WEB_LOCK_obj
{
  public:
    TRACE_Map_c(){
      tab.clear();
    };
    ~TRACE_Map_c(){};
    void AddAplication(unsigned long ip, int port, std::string application_id);
    bool RemoveAplication(std::string application_id);
    void ZeroAplication(unsigned long ip);
    void AddKeepAlive(unsigned long ip);
    void ChangeNumberByLicenceType(unsigned long ip, unsigned short  license_type, bool insert);
    void StartConnection(unsigned long ip);
    void EndConnection(unsigned long ip, unsigned short error_code);

    //void AddDataToArray(unsigned long ip);
    //unsigned long ip;
    std::map<unsigned long,trace_data_t*> tab;  // /* IP number */, DATA
    std::map<unsigned long,trace_data_t*>::iterator it;

  private:
    enum {
      FULL_VISOR = 0,         //PVL 15-03-2010 
      MONITOR_ONLY_VISOR = 1, //PVL 15-03-2010
      FULL_WEB_VISOR = 2      //PVL SM_WEB 16-12-2009 
    } ;

};
#endif
