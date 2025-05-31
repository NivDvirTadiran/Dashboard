#ifndef GSOAP_WEB_INIT_H
#define GSOAP_WEB_INIT_H
#include "acsasta.hpp"


class State_colors_tab
{
//============================ Public Section ========================
public:
  // ctor
  State_colors_tab();

  //dstr
  ~State_colors_tab(){};

  bool LoadColors(void);	
  CsaStaAgentState_t Get_CSA_state_by_ColorId(unsigned long color_id,string &state_name);
  int GetBGColorsByState(int     state,        //IN
                         int     in_conf,
                         Ulong_t **color_ptr,    //OUT
                         string& state_name);  //OUT

  bool IsStateColorAvailable(){return isAvailable; };	
  bool Get_Color_By_CSA_state(int state, Ulong_t &bg_color, Ulong_t &fr_color);

private:
  typedef struct DbiColorInfo_s
  {
    CsaStaAgentState_t state;
    string             state_name;
    Ulong_t            bg_color;
    Ulong_t            fg_color;
  }DbiColorInfo_t;
 
  bool            isAvailable;
  DbiColorInfo_s  tab[50];
  unsigned short  local_record_index;
    
};
//---------------------------------------------------------------


#endif
