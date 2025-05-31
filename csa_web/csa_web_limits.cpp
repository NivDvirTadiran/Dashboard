#include "er_std_override.h"
#include "csa_web_main.h"
#include "csa_web_limits.hpp"



static BswLog_c csa_web_limits_log_d(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_DETAILED);
static BswLog_c csa_web_limits_log_i(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_INFO);


Ulong_t  WHITE_COLOR  =0xffffff;
Ulong_t *WHITE_COLOR_PTR = &WHITE_COLOR;
Ulong_t  RED_COLOR =  0xff0000;
Ulong_t *RED_COLOR_PTR = &RED_COLOR;
Ulong_t  YELLOW_COLOR =0xffff00;
Ulong_t *YELLOW_COLOR_PTR = &YELLOW_COLOR;

void Limits_tab::Init_ErsIdLookUpList(void)
{
  ersIdList[1].ers_name = "f6_3_1_1_1";
  ersIdList[1].db_ers_id = ersid_val.f6_3_1_1_1;
  ersIdList[1].lim_type = None;
  ersIdList[2].ers_name = "f6_3_1_1_2";
  ersIdList[2].db_ers_id = ersid_val.f6_3_1_1_2;
  ersIdList[2].lim_type = None;
  ersIdList[3].ers_name = "f6_3_1_1_3";
  ersIdList[3].db_ers_id = ersid_val.f6_3_1_1_3;
  ersIdList[3].lim_type = None;
  ersIdList[4].ers_name = "f6_3_1_2_1";
  ersIdList[4].db_ers_id = ersid_val.f6_3_1_2_1;
  ersIdList[4].lim_type = Up;
  ersIdList[5].ers_name = "f6_3_1_2_2";
  ersIdList[5].db_ers_id = ersid_val.f6_3_1_2_2;
  ersIdList[5].lim_type = Up;
  ersIdList[6].ers_name = "f6_3_1_2_3";
  ersIdList[6].db_ers_id = ersid_val.f6_3_1_2_3;
  ersIdList[6].lim_type = Up;
  ersIdList[7].ers_name = "f6_3_1_2_4";
  ersIdList[7].db_ers_id = ersid_val.f6_3_1_2_4;
  ersIdList[7].lim_type = Up;
  ersIdList[8].ers_name = "f6_3_1_2_5a";
  ersIdList[8].db_ers_id = ersid_val.f6_3_1_2_5a;
  ersIdList[8].lim_type = None;
  ersIdList[9].ers_name = "f6_3_1_2_5b";
  ersIdList[9].db_ers_id = ersid_val.f6_3_1_2_5b;
  ersIdList[9].lim_type = None;
  ersIdList[10].ers_name = "f6_3_1_2_5c";
  ersIdList[10].db_ers_id = ersid_val.f6_3_1_2_5c;
  ersIdList[10].lim_type = None;
  ersIdList[11].ers_name = "f6_3_1_2_5d";
  ersIdList[11].db_ers_id = ersid_val.f6_3_1_2_5d;
  ersIdList[11].lim_type = None;
  ersIdList[12].ers_name = "f6_3_1_2_5e";
  ersIdList[12].db_ers_id = ersid_val.f6_3_1_2_5e;
  ersIdList[12].lim_type = None;
  ersIdList[13].ers_name = "f6_3_1_2_5f";
  ersIdList[13].db_ers_id = ersid_val.f6_3_1_2_5f;
  ersIdList[13].lim_type = None;
  ersIdList[14].ers_name = "f6_3_1_3_1";
  ersIdList[14].db_ers_id = ersid_val.f6_3_1_3_1;
  ersIdList[14].lim_type = Down;  //PVL, 1000675
  ersIdList[15].ers_name = "f6_3_1_3_2";
  ersIdList[15].db_ers_id = ersid_val.f6_3_1_3_2;
  ersIdList[15].lim_type = Up;
  ersIdList[16].ers_name = "f6_3_1_3_3";
  ersIdList[16].db_ers_id = ersid_val.f6_3_1_3_3;
  ersIdList[16].lim_type = None;
  ersIdList[17].ers_name = "f6_3_1_3_4";
  ersIdList[17].db_ers_id = ersid_val.f6_3_1_3_4;
  ersIdList[17].lim_type = Down;
  ersIdList[18].ers_name = "f6_3_1_3_5";
  ersIdList[18].db_ers_id = ersid_val.f6_3_1_3_5;
  ersIdList[18].lim_type = None;
  ersIdList[19].ers_name = "f6_3_1_3_6";
  ersIdList[19].db_ers_id = ersid_val.f6_3_1_3_6;
  ersIdList[19].lim_type = Up;
  ersIdList[20].ers_name = "f6_3_1_3_7";
  ersIdList[20].db_ers_id = ersid_val.f6_3_1_3_7;
  ersIdList[20].lim_type = Up;
  ersIdList[21].ers_name = "f6_3_1_3_8";
  ersIdList[21].db_ers_id = ersid_val.f6_3_1_3_8;
  ersIdList[21].lim_type = Up;
  ersIdList[22].ers_name = "f6_3_1_4_1";
  ersIdList[22].db_ers_id = ersid_val.f6_3_1_4_1;
  ersIdList[22].lim_type = Down;
  ersIdList[23].ers_name = "f6_3_1_4_2";
  ersIdList[23].db_ers_id = ersid_val.f6_3_1_4_2;
  ersIdList[23].lim_type = None;
  ersIdList[24].ers_name = "f6_3_1_4_3";
  ersIdList[24].db_ers_id = ersid_val.f6_3_1_4_3;
  ersIdList[24].lim_type = None;
  ersIdList[25].ers_name = "f6_3_1_4_3a";
  ersIdList[25].db_ers_id = ersid_val.f6_3_1_4_3a;
  ersIdList[25].lim_type = Down;
  ersIdList[26].ers_name = "f6_3_1_4_4";
  ersIdList[26].db_ers_id = ersid_val.f6_3_1_4_4;
  ersIdList[26].lim_type = Up;
  ersIdList[27].ers_name = "f6_3_1_4_4a";
  ersIdList[27].db_ers_id = ersid_val.f6_3_1_4_4a;
  ersIdList[27].lim_type = Up;
  ersIdList[28].ers_name = "f6_3_1_4_5";
  ersIdList[28].db_ers_id = ersid_val.f6_3_1_4_5;
  ersIdList[28].lim_type = Up;
  ersIdList[29].ers_name = "f6_3_1_4_6";
  ersIdList[29].db_ers_id = ersid_val.f6_3_1_4_6;
  ersIdList[29].lim_type = None;
  ersIdList[30].ers_name = "f6_3_1_4_7";
  ersIdList[30].db_ers_id = ersid_val.f6_3_1_4_7;
  ersIdList[30].lim_type = Up;
  ersIdList[31].ers_name = "f6_3_1_4_7a";
  ersIdList[31].db_ers_id = ersid_val.f6_3_1_4_7a;
  ersIdList[31].lim_type = Up;
  ersIdList[32].ers_name = "f6_3_1_4_8";
  ersIdList[32].db_ers_id = ersid_val.f6_3_1_4_8;
  ersIdList[32].lim_type = Up;
  ersIdList[33].ers_name = "f6_3_1_4_9";
  ersIdList[33].db_ers_id = ersid_val.f6_3_1_4_9;
  ersIdList[33].lim_type = None;
  ersIdList[34].ers_name = "f6_3_1_4_10";
  ersIdList[34].db_ers_id = ersid_val.f6_3_1_4_10;
  ersIdList[34].lim_type = None;
  ersIdList[35].ers_name = "f6_3_1_4_10a";
  ersIdList[35].db_ers_id = ersid_val.f6_3_1_4_10a;
  ersIdList[35].lim_type = None;
  ersIdList[36].ers_name = "f6_3_1_4_11";
  ersIdList[36].db_ers_id = ersid_val.f6_3_1_4_11;
  ersIdList[36].lim_type = Down;  //PVL 26.08.2008 bug1000735
  ersIdList[37].ers_name = "f6_3_1_4_12";
  ersIdList[37].db_ers_id = ersid_val.f6_3_1_4_12;
  ersIdList[37].lim_type = Up;
  ersIdList[38].ers_name = "f6_3_1_4_13";
  ersIdList[38].db_ers_id = ersid_val.f6_3_1_4_13;
  ersIdList[38].lim_type = Up;
  ersIdList[39].ers_name = "f6_3_1_4_14";
  ersIdList[39].db_ers_id = ersid_val.f6_3_1_4_14;
  ersIdList[39].lim_type = Up;
  ersIdList[40].ers_name = "f6_3_1_4_15";
  ersIdList[40].db_ers_id = ersid_val.f6_3_1_4_15;
  ersIdList[40].lim_type = None;
  ersIdList[41].ers_name = "f6_3_1_4_16";
  ersIdList[41].db_ers_id = ersid_val.f6_3_1_4_16;
  ersIdList[41].lim_type = Up;
  ersIdList[42].ers_name = "f6_3_2_1_1";
  ersIdList[42].db_ers_id  = ersid_val.f6_3_2_1_1;
  ersIdList[42].lim_type = None;
  ersIdList[43].ers_name = "f6_3_2_1_2";
  ersIdList[43].db_ers_id  = ersid_val.f6_3_2_1_2;
  ersIdList[43].lim_type = None;
  ersIdList[44].ers_name = "f6_3_2_1_3";
  ersIdList[44].db_ers_id  = ersid_val.f6_3_2_1_3;
  ersIdList[44].lim_type = None;
  ersIdList[45].ers_name = "f6_3_2_2_1";
  ersIdList[45].db_ers_id  = ersid_val.f6_3_2_2_1;
  ersIdList[45].lim_type = None;
  ersIdList[46].ers_name = "f6_3_2_2_2";
  ersIdList[46].db_ers_id = ersid_val.f6_3_2_2_2;
  ersIdList[46].lim_type = Up;
  ersIdList[47].ers_name = "f6_3_2_3_44";
  ersIdList[47].db_ers_id  = ersid_val.f6_3_2_3_44;
  ersIdList[47].lim_type = None;
  ersIdList[48].ers_name = "f6_3_2_3_4";
  ersIdList[48].db_ers_id  = ersid_val.f6_3_2_3_4;
  ersIdList[48].lim_type = None;
  ersIdList[49].ers_name = "f6_3_2_2_7";
  ersIdList[49].db_ers_id = ersid_val.f6_3_2_2_7;
  ersIdList[49].lim_type = None;
  ersIdList[50].ers_name = "f6_3_2_2_9";
  ersIdList[50].db_ers_id = ersid_val.f6_3_2_2_9;
  ersIdList[50].lim_type = None;
  ersIdList[51].ers_name = "f6_3_2_2_8";
  ersIdList[51].db_ers_id = ersid_val.f6_3_2_2_8;
  ersIdList[51].lim_type = None;
  ersIdList[52].ers_name = "f6_3_2_2_6";
  ersIdList[52].db_ers_id = ersid_val.f6_3_2_2_6;
  ersIdList[52].lim_type = None;
  ersIdList[53].ers_name = "f6_3_1_4_61";
  ersIdList[53].db_ers_id = ersid_val.f6_3_1_4_61;
  ersIdList[53].lim_type = None;
  ersIdList[54].ers_name = "f6_3_1_4_62";
  ersIdList[54].db_ers_id = ersid_val.f6_3_1_4_62;
  ersIdList[54].lim_type = None;
  ersIdList[55].ers_name = "f6_3_1_4_64";
  ersIdList[55].db_ers_id = ersid_val.f6_3_1_4_64;
  ersIdList[55].lim_type = None;
  ersIdList[56].ers_name = "f6_3_1_4_65";
  ersIdList[56].db_ers_id = ersid_val.f6_3_1_4_65;
  ersIdList[56].lim_type = Up;
  ersIdList[57].ers_name = "f6_3_1_4_67";
  ersIdList[57].db_ers_id = ersid_val.f6_3_1_4_67;
  ersIdList[57].lim_type = Up;
  ersIdList[58].ers_name = "f6_3_2_3_10";
  ersIdList[58].db_ers_id = ersid_val.f6_3_2_3_10;
  ersIdList[58].lim_type = None;
  ersIdList[59].ers_name = "f6_3_1_4_68";
  ersIdList[59].db_ers_id = ersid_val.f6_3_1_4_68;
  ersIdList[59].lim_type = Up;
  ersIdList[60].ers_name = "f6_3_1_4_70";
  ersIdList[60].db_ers_id = ersid_val.f6_3_1_4_70;
  ersIdList[60].lim_type = Up;
  ersIdList[61].ers_name = "f6_3_1_4_71";
  ersIdList[61].db_ers_id = ersid_val.f6_3_1_4_71;
  ersIdList[61].lim_type = None;
  ersIdList[62].ers_name = "f6_3_2_3_12";
  ersIdList[62].db_ers_id = ersid_val.f6_3_2_3_12;
  ersIdList[62].lim_type = Up;
  ersIdList[63].ers_name = "f6_3_2_3_12a";
  ersIdList[63].db_ers_id = ersid_val.f6_3_2_3_12a;
  ersIdList[63].lim_type = None;
  ersIdList[64].ers_name = "f6_3_1_4_73";
  ersIdList[64].db_ers_id = ersid_val.f6_3_1_4_73;
  ersIdList[64].lim_type = Up;
  ersIdList[65].ers_name = "f6_3_1_4_74";
  ersIdList[65].db_ers_id = ersid_val.f6_3_1_4_74;
  ersIdList[65].lim_type = None;
  ersIdList[66].ers_name = "f6_3_4_1_1";
  ersIdList[66].db_ers_id = ersid_val.f6_3_4_1_1;
  ersIdList[66].lim_type = None;
  ersIdList[67].ers_name = "f6_3_4_2_1";
  ersIdList[67].db_ers_id = ersid_val.f6_3_4_2_1;
  ersIdList[67].lim_type = Up;
  ersIdList[68].ers_name = "f6_3_4_2_2";
  ersIdList[68].db_ers_id = ersid_val.f6_3_4_2_2;
  ersIdList[68].lim_type = Up;
  ersIdList[69].ers_name = "f6_3_4_2_3";
  ersIdList[69].db_ers_id = ersid_val.f6_3_4_2_3;
  ersIdList[69].lim_type = Up;
  ersIdList[70].ers_name = "f6_3_4_2_4";
  ersIdList[70].db_ers_id = ersid_val.f6_3_4_2_4;
  ersIdList[70].lim_type = Up;
  ersIdList[71].ers_name = "f6_3_4_3_1";
  ersIdList[71].db_ers_id = ersid_val.f6_3_4_3_1;
  ersIdList[71].lim_type = None;
  ersIdList[72].ers_name = "f6_3_4_3_2";
  ersIdList[72].db_ers_id = ersid_val.f6_3_4_3_2;
  ersIdList[72].lim_type = Up;
  ersIdList[73].ers_name = "f6_3_4_3_3";
  ersIdList[73].db_ers_id = ersid_val.f6_3_4_3_3;
  ersIdList[73].lim_type = None;
  ersIdList[74].ers_name = "f6_3_4_3_4";
  ersIdList[74].db_ers_id = ersid_val.f6_3_4_3_4;
  ersIdList[74].lim_type = None;
  ersIdList[75].ers_name = "f6_3_4_3_5";
  ersIdList[75].db_ers_id = ersid_val.f6_3_4_3_5;
  ersIdList[75].lim_type = None;
  ersIdList[76].ers_name = "f6_3_4_3_6";
  ersIdList[76].db_ers_id = ersid_val.f6_3_4_3_6;
  ersIdList[76].lim_type = Up;
  ersIdList[77].ers_name = "f6_3_4_3_7";
  ersIdList[77].db_ers_id = ersid_val.f6_3_4_3_7;
  ersIdList[77].lim_type = Up;
  ersIdList[78].ers_name = "f6_3_4_3_8";
  ersIdList[78].db_ers_id = ersid_val.f6_3_4_3_8;
  ersIdList[78].lim_type = None;
  ersIdList[79].ers_name = "f6_3_4_4_1";
  ersIdList[79].db_ers_id = ersid_val.f6_3_4_4_1;
  ersIdList[79].lim_type = None;
  ersIdList[80].ers_name = "f6_3_4_4_2";
  ersIdList[80].db_ers_id = ersid_val.f6_3_4_4_2;
  ersIdList[80].lim_type = None;
  ersIdList[81].ers_name = "f6_3_4_4_3";
  ersIdList[81].db_ers_id = ersid_val.f6_3_4_4_3;
  ersIdList[81].lim_type = None;
  ersIdList[82].ers_name = "f6_3_4_4_3a";
  ersIdList[82].db_ers_id = ersid_val.f6_3_4_4_3a;
  ersIdList[82].lim_type = Down;
  ersIdList[83].ers_name = "f6_3_4_4_4";
  ersIdList[83].db_ers_id = ersid_val.f6_3_4_4_4;
  ersIdList[83].lim_type = None;
  ersIdList[84].ers_name = "f6_3_4_4_4a";
  ersIdList[84].db_ers_id = ersid_val.f6_3_4_4_4a;
  ersIdList[84].lim_type = Up;
  ersIdList[85].ers_name = "f6_3_4_4_5";
  ersIdList[85].db_ers_id = ersid_val.f6_3_4_4_5;
  ersIdList[85].lim_type = None;
  ersIdList[86].ers_name = "f6_3_4_4_6";
  ersIdList[86].db_ers_id = ersid_val.f6_3_4_4_6;
  ersIdList[86].lim_type = None;
  ersIdList[87].ers_name = "f6_3_4_4_7";
  ersIdList[87].db_ers_id = ersid_val.f6_3_4_4_7;
  ersIdList[87].lim_type = None;
  ersIdList[88].ers_name = "f6_3_4_4_7a";
  ersIdList[88].db_ers_id = ersid_val.f6_3_4_4_7a;
  ersIdList[88].lim_type = None;
  ersIdList[89].ers_name = "f6_3_4_4_8";
  ersIdList[89].db_ers_id = ersid_val.f6_3_4_4_8;
  ersIdList[89].lim_type = None;
  ersIdList[90].ers_name = "f6_3_4_4_9";
  ersIdList[90] .db_ers_id = ersid_val.f6_3_4_4_9;
  ersIdList[90].lim_type = None;
  ersIdList[91].ers_name = "f6_3_4_4_10";
  ersIdList[91].db_ers_id = ersid_val.f6_3_4_4_10;
  ersIdList[91].lim_type = None;
  ersIdList[92].ers_name = "f6_3_4_4_10a";
  ersIdList[92].db_ers_id = ersid_val.f6_3_4_4_10a;
  ersIdList[92].lim_type = None;
  ersIdList[93].ers_name = "f6_3_4_4_11";
  ersIdList[93].db_ers_id = ersid_val.f6_3_4_4_11;
  ersIdList[93].lim_type = Down;
  ersIdList[94].ers_name = "f6_3_4_4_12";
  ersIdList[94].db_ers_id = ersid_val.f6_3_4_4_12;
  ersIdList[94].lim_type = None;
  ersIdList[95].ers_name = "f6_3_4_4_13";
  ersIdList[95].db_ers_id = ersid_val.f6_3_4_4_13;
  ersIdList[95].lim_type = None;
  ersIdList[96].ers_name = "f6_3_4_4_14";
  ersIdList[96].db_ers_id = ersid_val.f6_3_4_4_14;
  ersIdList[96].lim_type = None;
  ersIdList[97].ers_name = "f6_3_4_4_15";
  ersIdList[97].db_ers_id = ersid_val.f6_3_4_4_15;
  ersIdList[97].lim_type = None;
  ersIdList[98].ers_name = "f6_3_4_4_16";
  ersIdList[98].db_ers_id = ersid_val.f6_3_4_4_16;
  ersIdList[98].lim_type = Up;
  ersIdList[99].ers_name = "f6_3_5_1_1";
  ersIdList[99].db_ers_id = ersid_val.f6_3_5_1_1;
  ersIdList[99].lim_type = None;
  ersIdList[100].ers_name = "f6_3_5_1_2";
  ersIdList[100].db_ers_id = ersid_val.f6_3_5_1_2;
  ersIdList[100].lim_type = None;
  ersIdList[101].ers_name = "f6_3_5_2_1";
  ersIdList[101].db_ers_id = ersid_val.f6_3_5_2_1;
  ersIdList[101].lim_type = None;
  ersIdList[102].ers_name = "f6_3_5_2_2";
  ersIdList[102].db_ers_id = ersid_val.f6_3_5_2_2;
  ersIdList[102].lim_type = Down;
  ersIdList[103].ers_name = "f6_3_6_1_1";
  ersIdList[103].db_ers_id = ersid_val.f6_3_6_1_1;
  ersIdList[103].lim_type = None;
  ersIdList[104].ers_name = "f6_3_6_1_2";
  ersIdList[104].db_ers_id = ersid_val.f6_3_6_1_2;
  ersIdList[104].lim_type = None;
  ersIdList[105].ers_name = "f6_3_6_2_1";
  ersIdList[105].db_ers_id = ersid_val.f6_3_6_2_1;
  ersIdList[105].lim_type = None;
  ersIdList[106].ers_name = "f6_3_7_1_1";
  ersIdList[106].db_ers_id = ersid_val.f6_3_7_1_1;
  ersIdList[106].lim_type = None;
  ersIdList[107].ers_name = "f6_3_7_1_2";
  ersIdList[107].db_ers_id = ersid_val.f6_3_7_1_2;
  ersIdList[107].lim_type = None;
  ersIdList[108].ers_name = "f6_3_7_2_1";
  ersIdList[108].db_ers_id = ersid_val.f6_3_7_2_1;
  ersIdList[108].lim_type = None;
  ersIdList[109].ers_name = "f6_3_9_1_1";
  ersIdList[109].db_ers_id = ersid_val.f6_3_9_1_1;
  ersIdList[109].lim_type = None;
  ersIdList[110].ers_name = "f6_3_9_1_2";
  ersIdList[110].db_ers_id = ersid_val.f6_3_9_1_2;
  ersIdList[110].lim_type = None;
  ersIdList[111].ers_name = "f6_3_9_2_1";
  ersIdList[111].db_ers_id = ersid_val.f6_3_9_2_1;
  ersIdList[111].lim_type = None;
  ersIdList[112].ers_name = "f6_3_9_2_2";
  ersIdList[112].db_ers_id = ersid_val.f6_3_9_2_2;
  ersIdList[112].lim_type = None;
  ersIdList[113].ers_name = "f6_3_9_2_3";
  ersIdList[113].db_ers_id = ersid_val.f6_3_9_2_3;
  ersIdList[113].lim_type = None;
  ersIdList[114].ers_name = "f6_3_9_2_4";
  ersIdList[114].db_ers_id = ersid_val.f6_3_9_2_4;
  ersIdList[114].lim_type = None;
  ersIdList[115].ers_name = "f6_3_9_2_5";
  ersIdList[115].db_ers_id = ersid_val.f6_3_9_2_5;
  ersIdList[115].lim_type = None;
  ersIdList[116].ers_name = "f6_3_9_2_6";
  ersIdList[116].db_ers_id = ersid_val.f6_3_9_2_6;
  ersIdList[116].lim_type = None;
  ersIdList[117].ers_name = "f6_3_5_2_3";
  ersIdList[117].db_ers_id = ersid_val.f6_3_5_2_3;
  ersIdList[117].lim_type = Up;
  ersIdList[118].ers_name = "f6_3_1_4_17"; //{rqd_agns}
  ersIdList[118].db_ers_id = ersid_val.f6_3_1_4_17;
  ersIdList[118].lim_type = None;
  ersIdList[119].ers_name = "f6_3_1_5_1"; //{intfl_calls_lp}
  ersIdList[119].db_ers_id = ersid_val.f6_3_1_5_1;
  ersIdList[119].lim_type = None;
  ersIdList[120].ers_name = "f6_3_1_5_2"; //{tsf_lp}
  ersIdList[120].db_ers_id = ersid_val.f6_3_1_5_2;
  ersIdList[120].lim_type = None;
  ersIdList[121].ers_name = "f6_3_1_5_3"; //{rqd_agns_lp}
  ersIdList[121].db_ers_id = ersid_val.f6_3_1_5_3;
  ersIdList[121].lim_type = None;
  ersIdList[122].ers_name = "f6_3_1_5_4"; //{avg_num_wait_calls}
  ersIdList[122].db_ers_id = ersid_val.f6_3_1_5_4;
  ersIdList[122].lim_type = None;
  ersIdList[123].ers_name = "f6_3_1_1_4"; //{interval_time}
  ersIdList[123].db_ers_id = ersid_val.f6_3_1_1_4;
  ersIdList[123].lim_type = None;
  ersIdList[124].ers_name = "f6_3_2_2_3";
  ersIdList[124].db_ers_id = ersid_val.f6_3_2_2_3;
  ersIdList[124].lim_type = None;
  ersIdList[125].ers_name = "f6_3_6_2_2";
  ersIdList[125].db_ers_id = ersid_val.f6_3_6_2_2;
  ersIdList[125].lim_type = None;
  ersIdList[126].ers_name = "f6_3_1_1_5"; //{target_asa}
  ersIdList[126].db_ers_id = ersid_val.f6_3_1_1_5;
  ersIdList[126].lim_type = None;
  ersIdList[127].ers_name = "f6_3_1_4_18"; //{avg_wrapup_time}
  ersIdList[127].db_ers_id = ersid_val.f6_3_1_4_18;
  ersIdList[127].lim_type = None;
  ersIdList[128].ers_name = "f6_3_1_4_19"; //{avg_handle_time}
  ersIdList[128].db_ers_id = ersid_val.f6_3_1_4_19;
  ersIdList[128].lim_type = None;
  ersIdList[129].ers_name = "f6_3_1_5_5"; ////{avg_wrapup_time_lp}
  ersIdList[129].db_ers_id = ersid_val.f6_3_1_5_5;
  ersIdList[129].lim_type = None;
  ersIdList[130].ers_name = "f6_3_1_5_6"; ////{avg_handle_time_lp}
  ersIdList[130].db_ers_id = ersid_val.f6_3_1_5_6;
  ersIdList[130].lim_type = None;
  ersIdList[131].ers_name = "f6_3_1_3_9";
  ersIdList[131].db_ers_id = ersid_val.f6_3_1_3_9;
  ersIdList[131].lim_type = Up;
  ersIdList[132].ers_name = "f6_3_1_3_14";// //{agents on dnd}
  ersIdList[132].db_ers_id = ersid_val.f6_3_1_3_14;
  ersIdList[132].lim_type = Up;
  ersIdList[133].ers_name = "f6_3_1_3_10";// //{primary agents}
  ersIdList[133].db_ers_id = ersid_val.f6_3_1_3_10;
  ersIdList[133].lim_type = None;
  ersIdList[134].ers_name = "f6_3_1_3_11"; ////{secondary agents}
  ersIdList[134].db_ers_id = ersid_val.f6_3_1_3_11;
  ersIdList[134].lim_type = None;
  ersIdList[135].ers_name = "f6_3_1_3_15"; ////{agents on split}
  ersIdList[135].db_ers_id = ersid_val.f6_3_1_3_15;
  ersIdList[135].lim_type = None;
  ersIdList[136].ers_name = "f6_3_1_3_16"; ////{agents on wrapup + f.wrapup}
  ersIdList[136].db_ers_id = ersid_val.f6_3_1_3_16;
  ersIdList[136].lim_type = None;
  ersIdList[137].ers_name = "f6_3_1_3_13"; //{agents on f.wrapup}
  ersIdList[137].db_ers_id = ersid_val.f6_3_1_3_13;
  ersIdList[137].lim_type = None;
  ersIdList[138].ers_name = "f6_3_1_4_35"; //{avg. wrap + f.wrap time}
  ersIdList[138].db_ers_id = ersid_val.f6_3_1_4_35;
  ersIdList[138].lim_type = None;
  ersIdList[139].ers_name = "f6_3_2_1_4";
  ersIdList[139].db_ers_id  = ersid_val.f6_3_2_1_4;
  ersIdList[139].lim_type = None;
  ersIdList[140].ers_name = "f6_3_2_1_5";
  ersIdList[140].db_ers_id  = ersid_val.f6_3_2_1_5;
  ersIdList[140].lim_type = None;
  ersIdList[141].ers_name = "f6_3_1_5_7"; //{avg. wrap + f.wrap time}
  ersIdList[141].db_ers_id = ersid_val.f6_3_1_5_7;
  ersIdList[141].lim_type = Up;
  ersIdList[142].ers_name = "f6_3_4_3_9"; //{RR: 15/12/97 agents on dnd}
  ersIdList[142].db_ers_id = ersid_val.f6_3_4_3_9;
  ersIdList[142].lim_type = Up;
  ersIdList[143].ers_name = "f6_3_4_3_10"; //{agents on split}
  ersIdList[143].db_ers_id = ersid_val.f6_3_4_3_10;
  ersIdList[143].lim_type = None;
  ersIdList[144].ers_name = "f6_3_4_3_11"; //{agents on wrap + f.wrap}
  ersIdList[144].db_ers_id = ersid_val.f6_3_4_3_11;
  ersIdList[144].lim_type = None;
  ersIdList[145].ers_name = "f6_3_4_4_17"; //{avg wrap + f.wrap time}
  ersIdList[145].db_ers_id = ersid_val.f6_3_4_4_17;
  ersIdList[145].lim_type = None;
  ersIdList[146].ers_name = "f6_3_4_3_12"; //{agents on f.wrap}
  ersIdList[146].db_ers_id = ersid_val.f6_3_4_3_12;
  ersIdList[146].lim_type = None;
  ersIdList[147].ers_name = "f6_3_2_3_16";
  ersIdList[147].db_ers_id  = ersid_val.f6_3_2_3_16;
  ersIdList[147].lim_type = None;
  ersIdList[148].ers_name = "f6_3_2_3_17";
  ersIdList[148].db_ers_id  = ersid_val.f6_3_2_3_17;
  ersIdList[148].lim_type = None;
  ersIdList[149].ers_name = "f6_3_2_3_18";
  ersIdList[149].db_ers_id  = ersid_val.f6_3_2_3_18;
  ersIdList[149].lim_type = None;
  ersIdList[150].ers_name = "f6_3_2_3_19";
  ersIdList[150].db_ers_id  = ersid_val.f6_3_2_3_19;
  ersIdList[150].lim_type = None;
  ersIdList[151].ers_name = "f6_3_1_4_27"; //{transferrd_in}
  ersIdList[151].db_ers_id = ersid_val.f6_3_1_4_27;
  ersIdList[151].lim_type = None;
  ersIdList[152].ers_name = "f6_3_1_4_28"; //{transferred_out}
  ersIdList[152].db_ers_id = ersid_val.f6_3_1_4_28;
  ersIdList[152].lim_type = Up;
  ersIdList[153].ers_name = "f6_3_1_4_29"; //{delayed calls}
  ersIdList[153].db_ers_id = ersid_val.f6_3_1_4_29;
  ersIdList[153].lim_type = Up;
  ersIdList[154].ers_name = "7_3_1_0"; //{interval start}
  ersIdList[154].db_ers_id = ersid_val.f7_3_1_0;
  ersIdList[154].lim_type = None;
  ersIdList[155].ers_name = "7_3_1_2_8"; //{total incoming calls}
  ersIdList[155].db_ers_id = ersid_val.f7_3_1_2_8;
  ersIdList[155].lim_type = None;
  ersIdList[156].ers_name = "7_3_1_2_22"; //{answered calls}
  ersIdList[156].db_ers_id = ersid_val.f7_3_1_2_22;
  ersIdList[156].lim_type = None;
  ersIdList[157].ers_name = "7_3_1_2_10"; //{abandoned calls}
  ersIdList[157].db_ers_id = ersid_val.f7_3_1_2_10;
  ersIdList[157].lim_type = None;
  ersIdList[158].ers_name = "7_3_1_2_34"; //{delayed calls}
  ersIdList[158].db_ers_id = ersid_val.f7_3_1_2_34;
  ersIdList[158].lim_type = None;
  ersIdList[159].ers_name = "7_3_1_2_16"; //{num recordings}
  ersIdList[159].db_ers_id = ersid_val.f7_3_1_2_16;
  ersIdList[159].lim_type = None;
  ersIdList[160].ers_name = "7_3_1_3_17a"; //{avg. delay time}
  ersIdList[160].db_ers_id = ersid_val.f7_3_1_3_17a;
  ersIdList[160].lim_type = None;
  ersIdList[161].ers_name = "7_3_1_3_5a"; //{avg. talk time}
  ersIdList[161].db_ers_id = ersid_val.f7_3_1_3_5a;
  ersIdList[161].lim_type = None;
  ersIdList[162].ers_name = "7_3_1_3_10"; //{avg. aband. time}
  ersIdList[162].db_ers_id = ersid_val.f7_3_1_3_10;
  ersIdList[162].lim_type = None;
  ersIdList[163].ers_name = "7_3_1_3_21a"; //{avg. wrap. time}
  ersIdList[163].db_ers_id = ersid_val.f7_3_1_3_21a;
  ersIdList[163].lim_type = None;
  ersIdList[164].ers_name = "7_3_1_3_6a"; //{avg. wrap. time}
  ersIdList[164].db_ers_id = ersid_val.f7_3_1_3_6a;
  ersIdList[164].lim_type = None;
  ersIdList[165].ers_name = "f6_3_1_4_36"; //{Originally accepted}
  ersIdList[165].db_ers_id = ersid_val.f6_3_1_4_36;
  ersIdList[165].lim_type = None;
  ersIdList[166].ers_name = "f6_3_1_4_37"; //{group_calls_answered}
  ersIdList[166].db_ers_id = ersid_val.f6_3_1_4_37;
  ersIdList[166].lim_type = None;
  ersIdList[167].ers_name = "f6_3_1_4_38"; //{answered_in_another_group_calls}
  ersIdList[167].db_ers_id = ersid_val.f6_3_1_4_38;
  ersIdList[167].lim_type = None;
  ersIdList[168].ers_name = "f6_3_1_4_39"; //{Overflowed_in}
  ersIdList[168].db_ers_id = ersid_val.f6_3_1_4_39;
  ersIdList[168].lim_type = None;
  ersIdList[169].ers_name = "f6_3_1_4_40";  //{overflowed_in_calls_answered}
  ersIdList[169].db_ers_id = ersid_val.f6_3_1_4_40;
  ersIdList[169].lim_type = None;
  ersIdList[170].ers_name = "f6_3_1_4_41"; //{overflowed_in_calls_answered_in_another_group}
  ersIdList[170].db_ers_id = ersid_val.f6_3_1_4_41;
  ersIdList[170].lim_type = None;
  ersIdList[171].ers_name = "f6_3_1_4_42"; //{group_calls_answered_in_another_group}
  ersIdList[171].db_ers_id = ersid_val.f6_3_1_4_42;
  ersIdList[171].lim_type = None;
  ersIdList[172].ers_name = "f6_3_1_4_43"; //{overflowed_in_calls_interflowed_out}
  ersIdList[172].db_ers_id = ersid_val.f6_3_1_4_43;
  ersIdList[172].lim_type = None;
  ersIdList[173].ers_name = "f6_3_1_4_44"; //{group_calls_interflowed_out}
  ersIdList[173].db_ers_id = ersid_val.f6_3_1_4_44;
  ersIdList[173].lim_type = None;
  ersIdList[174].ers_name = "f6_3_1_4_45"; //{group_calls_abandoned}
  ersIdList[174].db_ers_id = ersid_val.f6_3_1_4_45;
  ersIdList[174].lim_type = None;
  ersIdList[175].ers_name = "f6_3_1_4_46"; //{overflowed_in_calls_abandoned}
  ersIdList[175].db_ers_id = ersid_val.f6_3_1_4_46;
  ersIdList[175].lim_type = None;
  ersIdList[176].ers_name = "f6_3_1_4_47"; //{Overflowed_out}
  ersIdList[176].db_ers_id = ersid_val.f6_3_1_4_47;
  ersIdList[176].lim_type = None;
  ersIdList[177].ers_name = "f6_3_1_4_76"; //{Overflowed_out}
  ersIdList[177].db_ers_id = ersid_val.f6_3_1_4_76;
  ersIdList[177].lim_type = Up;       //inherits 6_3_1_4_8
  ersIdList[178].ers_name = "f6_3_1_4_77"; //{Overflowed_out}
  ersIdList[178].db_ers_id = ersid_val.f6_3_1_4_77;
  ersIdList[178].lim_type = None;
  ersIdList[179].ers_name = "f6_3_1_4_79"; //{Overflowed_out}
  ersIdList[179].db_ers_id = ersid_val.f6_3_1_4_79;
  ersIdList[179].lim_type = Up;       //inherits 6_3_1_4_5
  ersIdList[180].ers_name = "f6_3_1_3_17"; //{required number of agents}
  ersIdList[180].db_ers_id = ersid_val.f6_3_1_3_17;
  ersIdList[180].lim_type = None;
  ersIdList[181].ers_name = "f6_3_1_3_18"; //{understaffed number of agents}
  ersIdList[181].db_ers_id = ersid_val.f6_3_1_3_18;
  ersIdList[181].lim_type = None;
  ersIdList[182].ers_name = "f6_3_1_3_19"; //{understaffed number of agents}
  ersIdList[182].db_ers_id = ersid_val.f6_3_1_3_19;
  ersIdList[182].lim_type = None;
  ersIdList[183].ers_name = "f6_3_1_3_20"; //{min beween required and logedin numbers of agents}
  ersIdList[183].db_ers_id = ersid_val.f6_3_1_3_20;
  ersIdList[183].lim_type = None;
  ersIdList[184].ers_name = "f6_3_1_3_21"; //{(logged in -release) number of agents "ON PHONES"}
  ersIdList[184].db_ers_id = ersid_val.f6_3_1_3_21;
  ersIdList[184].lim_type = None;
  ersIdList[185].ers_name = "f6_3_1_3_18a"; //{percent of understaffed in required}
  ersIdList[185].db_ers_id = ersid_val.f6_3_1_3_18a;
  ersIdList[185].lim_type = None;
  ersIdList[186].ers_name = "f6_3_1_3_19a"; //{percent of overstaffed in required}
  ersIdList[186].db_ers_id = ersid_val.f6_3_1_3_19a;
  ersIdList[186].lim_type = None;
  ersIdList[187].ers_name = "f6_3_1_3_20a"; //{percent of (f6_3_1_3_20) in required }
  ersIdList[187].db_ers_id = ersid_val.f6_3_1_3_20a;
  ersIdList[187].lim_type = None;
  ersIdList[188].ers_name = "f6_3_1_4_80"; //{percent of (f6_3_1_3_20) in required }
  ersIdList[188].db_ers_id = ersid_val.f6_3_1_4_80;
  ersIdList[188].lim_type = None;
  ersIdList[189].ers_name = "f6_3_1_4_81"; //{percent of (f6_3_1_3_20) in required }
  ersIdList[189].db_ers_id = ersid_val.f6_3_1_4_81;
  ersIdList[189].lim_type = None;
  ersIdList[190].ers_name = "f6_3_2_3_14";
  ersIdList[190].db_ers_id  = ersid_val.f6_3_2_3_14;
  ersIdList[190].lim_type = None;
  ersIdList[191].ers_name = "f6_3_1_3_12";
  ersIdList[191].db_ers_id  = ersid_val.f6_3_1_3_12;
  ersIdList[191].lim_type = None;
  ersIdList[192].ers_name = "f6_3_10_1_1"; //{11/10/99 DNIS name}
  ersIdList[192].db_ers_id = ersid_val.f6_3_10_1_1; //{11/10/99 DNIS name}
  ersIdList[192].lim_type = None; //{11/10/99 DNIS name}
  ersIdList[193].ers_name = "f6_3_10_1_2";
  ersIdList[193].db_ers_id = ersid_val.f6_3_10_1_2 ; //{11/10/99 DNIS nnum}
  ersIdList[193].lim_type = None;
  ersIdList[194].ers_name = "f6_3_10_1_3"; //{11/10/99 time since curr. time int.}
  ersIdList[194].db_ers_id = ersid_val.f6_3_10_1_3;
  ersIdList[194].lim_type = None;
  ersIdList[195].ers_name = "f6_3_10_1_4";
  ersIdList[195].db_ers_id = ersid_val.f6_3_10_1_4; //{}
  ersIdList[195].lim_type = None; //{time interval}
  ersIdList[196].ers_name = "f6_3_10_1_5";
  ersIdList[196].db_ers_id = ersid_val.f6_3_10_1_5;//{}
  ersIdList[196].lim_type = None; //{target asa}
  ersIdList[197].ers_name = "f6_3_10_2_1"; //{number waiting calls}
  ersIdList[197].db_ers_id = ersid_val.f6_3_10_2_1;
  ersIdList[197].lim_type = Up;
  ersIdList[198].ers_name = "f6_3_10_2_2";//{number of waiting calls above target ASA}
  ersIdList[198].db_ers_id = ersid_val.f6_3_10_2_2;
  ersIdList[198].lim_type = Up;
  ersIdList[199].ers_name = "f6_3_10_2_3";//{avg. wait time of call in q}
  ersIdList[199].db_ers_id = ersid_val.f6_3_10_2_3;
  ersIdList[199].lim_type = Up;
  ersIdList[200].ers_name = "f6_3_10_2_4";//{11/10/99 longest wait time of call in the q}
  ersIdList[200].db_ers_id = ersid_val.f6_3_10_2_4;
  ersIdList[200].lim_type = Up;
  ersIdList[201].ers_name = "f6_3_10_2_5a";//{sti1}
  ersIdList[201].db_ers_id = ersid_val.f6_3_10_2_5a;
  ersIdList[201].lim_type = None;
  ersIdList[202].ers_name = "f6_3_10_2_5b";//{sti2}
  ersIdList[202].db_ers_id = ersid_val.f6_3_10_2_5b;
  ersIdList[202].lim_type = None;
  ersIdList[203].ers_name = "f6_3_10_2_5c";//{sti3}
  ersIdList[203].db_ers_id = ersid_val.f6_3_10_2_5c;
  ersIdList[203].lim_type = None;
  ersIdList[204].ers_name = "f6_3_10_2_5d";//{sti4}
  ersIdList[204].db_ers_id = ersid_val.f6_3_10_2_5d;
  ersIdList[204].lim_type = None;
  ersIdList[205].ers_name = "f6_3_10_2_5e";//{sti5}
  ersIdList[205].db_ers_id = ersid_val.f6_3_10_2_5e;
  ersIdList[205].lim_type = None;
  ersIdList[206].ers_name = "f6_3_10_2_5f";//{sti6}
  ersIdList[206].db_ers_id = ersid_val.f6_3_10_2_5f;
  ersIdList[206].lim_type = None;
  ersIdList[207].ers_name = "f6_3_10_3_1";//{longest acd talk time}
  ersIdList[207].db_ers_id = ersid_val.f6_3_10_3_1;
  ersIdList[207].lim_type = Up;
  ersIdList[208].ers_name = "f6_3_10_4_1";//{tsf}
  ersIdList[208].db_ers_id = ersid_val.f6_3_10_4_1;
  ersIdList[208].lim_type = Up;
  ersIdList[209].ers_name = "f6_3_10_4_2";//{inc. calls}
  ersIdList[209].db_ers_id = ersid_val.f6_3_10_4_2;
  ersIdList[209].lim_type = Up;
  ersIdList[210].ers_name = "f6_3_10_4_3";//{answered}
  ersIdList[210].db_ers_id = ersid_val.f6_3_10_4_3;
  ersIdList[210].lim_type = Up;
  ersIdList[211].ers_name = "f6_3_10_4_3a";//{% answered}
  ersIdList[211].db_ers_id = ersid_val.f6_3_10_4_3a;
  ersIdList[211].lim_type = Up;
  ersIdList[212].ers_name = "f6_3_10_4_4";//{aband}
  ersIdList[212].db_ers_id = ersid_val.f6_3_10_4_4;
  ersIdList[212].lim_type = Up;
  ersIdList[213].ers_name = "f6_3_10_4_4a";//{%aband}
  ersIdList[213].db_ers_id = ersid_val.f6_3_10_4_4a;
  ersIdList[213].lim_type = Up;
  ersIdList[214].ers_name = "f6_3_10_4_5";//{avg. aband time}
  ersIdList[214].db_ers_id = ersid_val.f6_3_10_4_5;
  ersIdList[214].lim_type = Up;
  ersIdList[215].ers_name = "f6_3_10_4_6";//{londest aband time}
  ersIdList[215].db_ers_id = ersid_val.f6_3_10_4_6;
  ersIdList[215].lim_type = Up;
  ersIdList[216].ers_name = "f6_3_10_4_7";//{avg. queue time}
  ersIdList[216].db_ers_id = ersid_val.f6_3_10_4_7;
  ersIdList[216].lim_type = Up;
  ersIdList[217].ers_name = "f6_3_10_4_8";//{longest queue time}
  ersIdList[217].db_ers_id = ersid_val.f6_3_10_4_8;
  ersIdList[217].lim_type = Up;
  ersIdList[218].ers_name = "f6_3_10_4_9";//{avg. talk time}
  ersIdList[218].db_ers_id = ersid_val.f6_3_10_4_9;
  ersIdList[218].lim_type = Up;
  ersIdList[219].ers_name = "f6_3_10_4_10";//{longest talk time}
  ersIdList[219].db_ers_id = ersid_val.f6_3_10_4_10;
  ersIdList[219].lim_type = Up;
  ersIdList[220].ers_name = "f6_3_10_4_11";//{avg. answered time}
  ersIdList[220].db_ers_id = ersid_val.f6_3_10_4_11;
  ersIdList[220].lim_type = Up;
  ersIdList[221].ers_name = "f6_3_10_4_12";//{avg. hold time}
  ersIdList[221].db_ers_id = ersid_val.f6_3_10_4_12;
  ersIdList[221].lim_type = Up;
  ersIdList[222].ers_name = "f6_3_10_4_13";//{# of calls which were transferred at least once}
  ersIdList[222].db_ers_id = ersid_val.f6_3_10_4_13;
  ersIdList[222].lim_type = Up;
  ersIdList[223].ers_name = "f6_3_10_4_14";//{acc. talk time}
  ersIdList[223].db_ers_id = ersid_val.f6_3_10_4_14;
  ersIdList[223].lim_type = Up;
  ersIdList[224].ers_name = "f6_3_10_4_15";//{acc. aband time}
  ersIdList[224].db_ers_id = ersid_val.f6_3_10_4_15;
  ersIdList[224].lim_type = Up;
  ersIdList[225].ers_name = "f6_3_10_5_1";//{tsf lp}
  ersIdList[225].db_ers_id = ersid_val.f6_3_10_5_1;
  ersIdList[225].lim_type = Up;
  ersIdList[226].ers_name = "f6_3_10_5_2";//{avg. call wait on lp}
  ersIdList[226].db_ers_id = ersid_val.f6_3_10_5_2;
  ersIdList[226].lim_type = Up;
  ersIdList[227].ers_name = "f6_3_10_5_3";//{aband calls for lp}
  ersIdList[227].db_ers_id = ersid_val.f6_3_10_5_3;
  ersIdList[227].lim_type = Up;
  ersIdList[228].ers_name = "f6_3_10_5_4";//{answered call for lp}
  ersIdList[228].db_ers_id = ersid_val.f6_3_10_5_4;
  ersIdList[228].lim_type = Up;
  ersIdList[229].ers_name = "f6_3_10_5_4a";//{% answered calls from all inc calls for lp}
  ersIdList[229].db_ers_id = ersid_val.f6_3_10_5_4a;
  ersIdList[229].lim_type = Up;
  ersIdList[230].ers_name = "f6_3_2_2_4";//{ani for acd call to agent}
  ersIdList[230].db_ers_id = ersid_val.f6_3_2_2_4;
  ersIdList[230].lim_type = None;
  ersIdList[231].ers_name = "f6_3_2_2_5";//{dnis for acd call to agent}
  ersIdList[231].db_ers_id = ersid_val.f6_3_2_2_5;
  ersIdList[231].lim_type = None;
  ersIdList[232].ers_name = "f6_3_4_4_18";
  ersIdList[232].db_ers_id = ersid_val.f6_3_4_4_18;
  ersIdList[232].lim_type = None;
  ersIdList[233].ers_name = "f6_3_4_4_19";
  ersIdList[233].db_ers_id = ersid_val.f6_3_4_4_19;
  ersIdList[233].lim_type = None;
  ersIdList[234].ers_name = "f6_3_4_4_20";
  ersIdList[234].db_ers_id = ersid_val.f6_3_4_4_20;
  ersIdList[234].lim_type = None;
  ersIdList[235].ers_name = "f6_3_4_4_21";
  ersIdList[235].db_ers_id = ersid_val.f6_3_4_4_21;
  ersIdList[235].lim_type = None;
  ersIdList[236].ers_name = "f6_3_4_4_22";
  ersIdList[236].db_ers_id = ersid_val.f6_3_4_4_22;
  ersIdList[236].lim_type = None;
  ersIdList[237].ers_name = "f6_3_4_4_23";
  ersIdList[237].db_ers_id = ersid_val.f6_3_4_4_23;
  ersIdList[237].lim_type = None;
  ersIdList[238].ers_name = "f6_3_4_4_24";
  ersIdList[238].db_ers_id = ersid_val.f6_3_4_4_24;
  ersIdList[238].lim_type = None;
  ersIdList[239].ers_name = "f6_3_4_4_25";
  ersIdList[239].db_ers_id = ersid_val.f6_3_4_4_25;
  ersIdList[239].lim_type = None;
  ersIdList[240].ers_name = "f6_3_4_4_26";
  ersIdList[240].db_ers_id = ersid_val.f6_3_4_4_26;
  ersIdList[240].lim_type = None;
  ersIdList[241].ers_name = "f6_3_4_4_27";
  ersIdList[241].db_ers_id = ersid_val.f6_3_4_4_27;
  ersIdList[241].lim_type = None;
  ersIdList[242].ers_name = "f6_3_4_4_28";
  ersIdList[242].db_ers_id = ersid_val.f6_3_4_4_28;
  ersIdList[242].lim_type = None;
  ersIdList[243].ers_name = "f6_3_4_4_29";
  ersIdList[243].db_ers_id = ersid_val.f6_3_4_4_29;
  ersIdList[243].lim_type = None;
  ersIdList[244].ers_name = "f6_3_1_2_7";
  ersIdList[244].db_ers_id = ersid_val.f6_3_1_2_7;
  ersIdList[244].lim_type = None;
  ersIdList[245].ers_name = "f6_3_1_3_23";
  ersIdList[245].db_ers_id = ersid_val.f6_3_1_3_23;
  ersIdList[245].lim_type = Up;
  ersIdList[246].ers_name = "f6_3_1_4_48";
  ersIdList[246].db_ers_id = ersid_val.f6_3_1_4_48;
  ersIdList[246].lim_type = Up;
  ersIdList[247].ers_name = "f6_3_1_4_50";
  ersIdList[247].db_ers_id = ersid_val.f6_3_1_4_50;
  ersIdList[247].lim_type = Up;
  ersIdList[248].ers_name = "f6_3_1_2_9";
  ersIdList[248].db_ers_id = ersid_val.f6_3_1_2_9;
  ersIdList[248].lim_type = None;
  ersIdList[249].ers_name = "f6_3_1_2_10";
  ersIdList[249].db_ers_id = ersid_val.f6_3_1_2_10;
  ersIdList[249].lim_type = None;
  ersIdList[250].ers_name = "f6_3_1_3_22";
  ersIdList[250].db_ers_id = ersid_val.f6_3_1_3_22;
  ersIdList[250].lim_type = Up;
  ersIdList[251].ers_name = "f6_3_1_4_49";
  ersIdList[251].db_ers_id = ersid_val.f6_3_1_4_49;
  ersIdList[251].lim_type = Up;
  ersIdList[252].ers_name = "f6_3_1_4_52";
  ersIdList[252].db_ers_id = ersid_val.f6_3_1_4_52;
  ersIdList[252].lim_type = Up;
  ersIdList[253].ers_name = "f6_3_1_4_53";
  ersIdList[253].db_ers_id = ersid_val.f6_3_1_4_53;
  ersIdList[253].lim_type = Up;
  ersIdList[254].ers_name = "f6_3_1_4_55";
  ersIdList[254].db_ers_id = ersid_val.f6_3_1_4_55;
  ersIdList[254].lim_type = Up;
  ersIdList[255].ers_name = "f6_3_1_4_56";
  ersIdList[255].db_ers_id = ersid_val.f6_3_1_4_56;
  ersIdList[255].lim_type = Up;
  ersIdList[256].ers_name = "f6_3_1_4_58";
  ersIdList[256].db_ers_id = ersid_val.f6_3_1_4_58;
  ersIdList[256].lim_type = Up;
  ersIdList[257].ers_name = "f6_3_1_4_59";
  ersIdList[257].db_ers_id = ersid_val.f6_3_1_4_59;
  ersIdList[257].lim_type = Up;
  ersIdList[258].ers_name = "f6_3_4_2_5";
  ersIdList[258].db_ers_id = ersid_val.f6_3_4_2_5;
  ersIdList[258].lim_type = Up;
  ersIdList[259].ers_name = "f6_3_4_2_6";
  ersIdList[259].db_ers_id = ersid_val.f6_3_4_2_6;
  ersIdList[259].lim_type = Up;
  ersIdList[260].ers_name = "f6_3_4_2_7";
  ersIdList[260].db_ers_id = ersid_val.f6_3_4_2_7;
  ersIdList[260].lim_type = Up;
  ersIdList[261].ers_name = "f6_3_4_3_13";
  ersIdList[261].db_ers_id = ersid_val.f6_3_4_3_13;
  ersIdList[261].lim_type = None;
  ersIdList[262].ers_name = "f6_3_4_3_14";
  ersIdList[262].db_ers_id = ersid_val.f6_3_4_3_14;
  ersIdList[262].lim_type = None;
  ersIdList[263].ers_name = "f7_3_1_2_1"; //{interval start}
  ersIdList[263].db_ers_id = ersid_val.f7_3_1_2_1;
  ersIdList[263].lim_type = None;
  ersIdList[264].ers_name = "f6_3_1_3_24";
  ersIdList[264].db_ers_id = ersid_val.f6_3_1_3_24;
  ersIdList[264].lim_type = Up;
  ersIdList[265].ers_name = "f6_3_1_2_8a";
  ersIdList[265].db_ers_id = ersid_val.f6_3_1_2_8a;
  ersIdList[265].lim_type = None;
  ersIdList[266].ers_name = "f6_3_1_2_8b";
  ersIdList[266].db_ers_id = ersid_val.f6_3_1_2_8b;
  ersIdList[266].lim_type = None;
  ersIdList[267].ers_name = "f6_3_1_2_8c";
  ersIdList[267].db_ers_id = ersid_val.f6_3_1_2_8c;
  ersIdList[267].lim_type = None;
  ersIdList[268].ers_name = "f6_3_1_2_8d";
  ersIdList[268].db_ers_id = ersid_val.f6_3_1_2_8d;
  ersIdList[268].lim_type = None;
  ersIdList[269].ers_name = "f6_3_1_2_8e";
  ersIdList[269].db_ers_id = ersid_val.f6_3_1_2_8e;
  ersIdList[269].lim_type = None;
  ersIdList[270].ers_name = "f6_3_1_2_8f";
  ersIdList[270].db_ers_id = ersid_val.f6_3_1_2_8f;
  ersIdList[270].lim_type = None;
  ersIdList[271].ers_name = "f6_3_12_1_1";
  ersIdList[271].db_ers_id = ersid_val.f6_3_12_1_1;
  ersIdList[271].lim_type = None;
  ersIdList[272].ers_name = "f6_3_12_2_1";
  ersIdList[272].db_ers_id = ersid_val.f6_3_12_2_1;
  ersIdList[272].lim_type = None;
  ersIdList[273].ers_name = "f6_3_12_2_2";
  ersIdList[273].db_ers_id = ersid_val.f6_3_12_2_2;
  ersIdList[273].lim_type = None;
  ersIdList[274].ers_name = "f6_3_13_1_1";
  ersIdList[274].db_ers_id = ersid_val.f6_3_13_1_1;
  ersIdList[274].lim_type = None;
  ersIdList[275].ers_name = "f6_3_13_1_2";
  ersIdList[275].db_ers_id = ersid_val.f6_3_13_1_2;
  ersIdList[275].lim_type = None;
  ersIdList[276].ers_name = "f6_3_13_2_1";
  ersIdList[276].db_ers_id = ersid_val.f6_3_13_2_1;
  ersIdList[276].lim_type = None;
  ersIdList[277].ers_name = "f6_3_13_2_2";
  ersIdList[277].db_ers_id = ersid_val.f6_3_13_2_2;
  ersIdList[277].lim_type = None;
  ersIdList[278].ers_name = "f6_3_13_2_3";
  ersIdList[278].db_ers_id = ersid_val.f6_3_13_2_3;
  ersIdList[278].lim_type = None;
  ersIdList[279].ers_name = "f6_3_13_2_4";
  ersIdList[279].db_ers_id = ersid_val.f6_3_13_2_4;
  ersIdList[279].lim_type = None;
  ersIdList[280].ers_name = "f6_3_11_1_1";
  ersIdList[280].db_ers_id = ersid_val.f6_3_11_1_1;
  ersIdList[280].lim_type = None;
  ersIdList[281].ers_name = "f6_3_11_1_2";
  ersIdList[281].db_ers_id = ersid_val.f6_3_11_1_2;
  ersIdList[281].lim_type = None;
  ersIdList[282].ers_name = "f6_3_11_2_1";
  ersIdList[282].db_ers_id = ersid_val.f6_3_11_2_1;
  ersIdList[282].lim_type = None;
  ersIdList[283].ers_name = "f6_3_11_2_2";
  ersIdList[283].db_ers_id = ersid_val.f6_3_11_2_2;
  ersIdList[283].lim_type = None;
  ersIdList[284].ers_name = "f6_3_11_2_3";
  ersIdList[284].db_ers_id = ersid_val.f6_3_11_2_3;
  ersIdList[284].lim_type = None;
  ersIdList[285].ers_name = "f6_3_11_2_4";
  ersIdList[285].db_ers_id = ersid_val.f6_3_11_2_4;
  ersIdList[285].lim_type = None;
  ersIdList[286].ers_name = "f6_3_11_2_5";
  ersIdList[286].db_ers_id = ersid_val.f6_3_11_2_5;
  ersIdList[286].lim_type = None;
  ersIdList[287].ers_name = "f6_3_11_2_6";
  ersIdList[287].db_ers_id = ersid_val.f6_3_11_2_6;
  ersIdList[287].lim_type = None;
  ersIdList[288].ers_name = "f6_3_11_2_7";
  ersIdList[288].db_ers_id = ersid_val.f6_3_11_2_7;
  ersIdList[288].lim_type = None;
  ersIdList[289].ers_name = "f6_3_11_2_8";
  ersIdList[289].db_ers_id = ersid_val.f6_3_11_2_8;
  ersIdList[289].lim_type = None;
  ersIdList[290].ers_name = "f6_3_11_3_1";
  ersIdList[290].db_ers_id = ersid_val.f6_3_11_3_1;
  ersIdList[290].lim_type = None;
  ersIdList[291].ers_name = "f6_3_11_3_2";
  ersIdList[291].db_ers_id = ersid_val.f6_3_11_3_2;
  ersIdList[291].lim_type = None;
  ersIdList[292].ers_name = "f6_3_11_3_3";
  ersIdList[292].db_ers_id = ersid_val.f6_3_11_3_3;
  ersIdList[292].lim_type = None;
  ersIdList[293].ers_name = "f6_3_11_3_4";
  ersIdList[293].db_ers_id = ersid_val.f6_3_11_3_4;
  ersIdList[293].lim_type = None;
  ersIdList[294].ers_name = "f6_3_11_3_5";
  ersIdList[294].db_ers_id = ersid_val.f6_3_11_3_5;
  ersIdList[294].lim_type = None;
  ersIdList[295].ers_name = "f6_3_11_3_6";
  ersIdList[295].db_ers_id = ersid_val.f6_3_11_3_6;
  ersIdList[295].lim_type = None;
  ersIdList[296].ers_name = "f6_3_11_3_7";
  ersIdList[296].db_ers_id = ersid_val.f6_3_11_3_7;
  ersIdList[296].lim_type = None;
  ersIdList[297].ers_name = "f6_3_11_3_8";
  ersIdList[297].db_ers_id = ersid_val.f6_3_11_3_8;
  ersIdList[297].lim_type = None;
  ersIdList[298].ers_name = "f6_3_11_3_9";
  ersIdList[298].db_ers_id = ersid_val.f6_3_11_3_9;
  ersIdList[298].lim_type = None;
  ersIdList[299].ers_name = "f6_3_11_3_10";
  ersIdList[299].db_ers_id = ersid_val.f6_3_11_3_10;
  ersIdList[299].lim_type = None;
  ersIdList[300].ers_name = "f6_3_11_3_11";
  ersIdList[300].db_ers_id = ersid_val.f6_3_11_3_11;
  ersIdList[300].lim_type = None;
  ersIdList[301].ers_name = "f6_3_11_3_12";
  ersIdList[301].db_ers_id = ersid_val.f6_3_11_3_12;
  ersIdList[301].lim_type = None;
  ersIdList[302].ers_name = "f6_3_11_3_13";
  ersIdList[302].db_ers_id = ersid_val.f6_3_11_3_13;
  ersIdList[302].lim_type = None;
  ersIdList[303].ers_name = "f6_3_11_3_14";
  ersIdList[303].db_ers_id = ersid_val.f6_3_11_3_14;
  ersIdList[303].lim_type = None;
  ersIdList[304].ers_name = "f6_3_11_3_15";
  ersIdList[304].db_ers_id = ersid_val.f6_3_11_3_15;
  ersIdList[304].lim_type = None;
  ersIdList[305].ers_name = "f6_3_11_3_16";
  ersIdList[305].db_ers_id = ersid_val.f6_3_11_3_16;
  ersIdList[305].lim_type = None;
  ersIdList[306].ers_name = "f6_3_11_3_17";
  ersIdList[306].db_ers_id = ersid_val.f6_3_11_3_17;
  ersIdList[306].lim_type = None;
  ersIdList[307].ers_name = "f6_3_11_3_18";
  ersIdList[307].db_ers_id = ersid_val.f6_3_11_3_18;
  ersIdList[307].lim_type = None;
  ersIdList[308].ers_name = "f6_3_11_3_19";
  ersIdList[308].db_ers_id = ersid_val.f6_3_11_3_19;
  ersIdList[308].lim_type = None;
  ersIdList[309].ers_name = "f6_3_11_3_20";
  ersIdList[309].db_ers_id = ersid_val.f6_3_11_3_20;
  ersIdList[309].lim_type = None;
  ersIdList[310].ers_name = "f6_3_11_3_21";
  ersIdList[310].db_ers_id = ersid_val.f6_3_11_3_21;
  ersIdList[310].lim_type = None;
  ersIdList[311].ers_name = "f6_3_11_3_22";
  ersIdList[311].db_ers_id = ersid_val.f6_3_11_3_22;
  ersIdList[311].lim_type = None;
  ersIdList[312].ers_name = "f6_3_11_3_23";
  ersIdList[312].db_ers_id = ersid_val.f6_3_11_3_23;
  ersIdList[312].lim_type = None;
  ersIdList[313].ers_name = "f6_3_11_3_24";
  ersIdList[313].db_ers_id = ersid_val.f6_3_11_3_24;
  ersIdList[313].lim_type = None;
  ersIdList[314].ers_name = "f6_3_11_3_25";
  ersIdList[314].db_ers_id = ersid_val.f6_3_11_3_25;
  ersIdList[314].lim_type = None;
  ersIdList[315].ers_name = "f6_3_11_3_26";
  ersIdList[315].db_ers_id = ersid_val.f6_3_11_3_26;
  ersIdList[315].lim_type = None;
  ersIdList[316].ers_name = "f6_3_11_3_27";
  ersIdList[316].db_ers_id = ersid_val.f6_3_11_3_27;
  ersIdList[316].lim_type = None;
  ersIdList[317].ers_name = "f6_3_11_3_28";
  ersIdList[317].db_ers_id = ersid_val.f6_3_11_3_28;
  ersIdList[317].lim_type = None;
  ersIdList[318].ers_name = "f6_3_11_3_29";
  ersIdList[318].db_ers_id = ersid_val.f6_3_11_3_29;
  ersIdList[318].lim_type = None;
  ersIdList[319].ers_name = "f6_3_11_3_30";
  ersIdList[319].db_ers_id = ersid_val.f6_3_11_3_30;
  ersIdList[319].lim_type = None;
  ersIdList[320].ers_name = "f6_3_11_3_31";
  ersIdList[320].db_ers_id = ersid_val.f6_3_11_3_31;
  ersIdList[320].lim_type = None;
  ersIdList[321].ers_name = "f6_3_11_3_32";
  ersIdList[321].db_ers_id = ersid_val.f6_3_11_3_32;
  ersIdList[321].lim_type = None;
  ersIdList[322].ers_name = "f6_3_11_3_33";
  ersIdList[322].db_ers_id = ersid_val.f6_3_11_3_33;
  ersIdList[322].lim_type = None;
  ersIdList[323].ers_name = "f6_3_11_3_34";
  ersIdList[323].db_ers_id = ersid_val.f6_3_11_3_34;
  ersIdList[323].lim_type = None;
  ersIdList[324].ers_name = "f6_3_11_3_35";
  ersIdList[324].db_ers_id = ersid_val.f6_3_11_3_35;
  ersIdList[324].lim_type = None;
  ersIdList[325].ers_name = "f6_3_11_3_36";
  ersIdList[325].db_ers_id = ersid_val.f6_3_11_3_36;
  ersIdList[325].lim_type = None;
  ersIdList[326].ers_name = "f6_3_1_2_11";
  ersIdList[326].db_ers_id = ersid_val.f6_3_1_2_11;
  ersIdList[326].lim_type = None;
  ersIdList[327].ers_name = "f6_3_1_2_12";
  ersIdList[327].db_ers_id = ersid_val.f6_3_1_2_12;
  ersIdList[327].lim_type = None;
  ersIdList[328].ers_name = "f6_3_1_2_13";
  ersIdList[328].db_ers_id = ersid_val.f6_3_1_2_13;
  ersIdList[328].lim_type = None;
  ersIdList[329].ers_name = "f6_3_14_1_1";
  ersIdList[329].db_ers_id = ersid_val.f6_3_14_1_1;
  ersIdList[329].lim_type = None;
  ersIdList[330].ers_name = "f6_3_14_1_2";
  ersIdList[330].db_ers_id = ersid_val.f6_3_14_1_2;
  ersIdList[330].lim_type = None;
  ersIdList[331].ers_name = "f6_3_14_2_1";
  ersIdList[331].db_ers_id = ersid_val.f6_3_14_2_1;
  ersIdList[331].lim_type = None;
  ersIdList[332].ers_name = "f6_3_14_2_2";
  ersIdList[332].db_ers_id = ersid_val.f6_3_14_2_2;
  ersIdList[332].lim_type = None;
  ersIdList[333].ers_name = "f6_3_14_3_1";
  ersIdList[333].db_ers_id = ersid_val.f6_3_14_3_1;
  ersIdList[333].lim_type = None;
  ersIdList[334].ers_name = "f6_3_14_3_2";
  ersIdList[334].db_ers_id = ersid_val.f6_3_14_3_2;
  ersIdList[334].lim_type = None;
  ersIdList[335].ers_name = "f6_3_1_3_25";
  ersIdList[335].db_ers_id = ersid_val.f6_3_1_3_25;
  ersIdList[335].lim_type = Up;
  ersIdList[336].ers_name = "f6_3_1_4_83";//not supported
  ersIdList[336].db_ers_id = ersid_val.f6_3_1_4_83;
  ersIdList[336].lim_type = Up;
  ersIdList[337].ers_name = "f6_3_2_2_10";
  ersIdList[337].db_ers_id = ersid_val.f6_3_2_2_10;
  ersIdList[337].lim_type = None;
  ersIdList[338].ers_name = "f6_3_1_4_88";
  ersIdList[338].db_ers_id = ersid_val.f6_3_1_4_88;
  ersIdList[338].lim_type = Up;       //inherits 6_3_1_4_14
  ersIdList[339].ers_name = "f6_3_1_4_89";
  ersIdList[339].db_ers_id = ersid_val.f6_3_1_4_89;
  ersIdList[339].lim_type = Up;
  ersIdList[340].ers_name = "f6_3_1_4_91";
  ersIdList[340].db_ers_id = ersid_val.f6_3_1_4_91;
  ersIdList[340].lim_type = None;
  ersIdList[341].ers_name = "f6_3_1_4_92";
  ersIdList[341].db_ers_id = ersid_val.f6_3_1_4_92;
  ersIdList[341].lim_type = None;
  ersIdList[342].ers_name = "f6_3_1_4_94";
  ersIdList[342].db_ers_id = ersid_val.f6_3_1_4_94;
  ersIdList[342].lim_type = None;
  ersIdList[343].ers_name = "f6_3_1_4_95";
  ersIdList[343].db_ers_id = ersid_val.f6_3_1_4_95;
  ersIdList[343].lim_type = Up;
  ersIdList[344].ers_name = "f6_3_1_4_97";
  ersIdList[344].db_ers_id = ersid_val.f6_3_1_4_97;
  ersIdList[344].lim_type = Up;
  ersIdList[345].ers_name = "f6_3_1_4_98";
  ersIdList[345].db_ers_id = ersid_val.f6_3_1_4_98;
  ersIdList[345].lim_type = None;
  ersIdList[346].ers_name = "f6_3_1_4_100";
  ersIdList[346].db_ers_id = ersid_val.f6_3_1_4_100;
  ersIdList[346].lim_type = Up;
  ersIdList[347].ers_name = "f6_3_1_4_101";
  ersIdList[347].db_ers_id = ersid_val.f6_3_1_4_101;
  ersIdList[347].lim_type = None;
  ersIdList[348].ers_name = "f6_3_1_4_103";
  ersIdList[348].db_ers_id = ersid_val.f6_3_1_4_103;
  ersIdList[348].lim_type = None;
  ersIdList[349].ers_name = "f6_3_1_4_104";
  ersIdList[349].db_ers_id = ersid_val.f6_3_1_4_104;
  ersIdList[349].lim_type = None;
  ersIdList[350].ers_name = "f6_3_1_4_105";
  ersIdList[350].db_ers_id = ersid_val.f6_3_1_4_105;
  ersIdList[350].lim_type = None;
  ersIdList[351].ers_name = "f6_3_1_4_107";
  ersIdList[351].db_ers_id = ersid_val.f6_3_1_4_107;
  ersIdList[351].lim_type = None;
  ersIdList[352].ers_name = "f6_3_1_4_112";
  ersIdList[352].db_ers_id = ersid_val.f6_3_1_4_112;
  ersIdList[352].lim_type = Up;
  ersIdList[353].ers_name = "f6_3_1_4_113";
  ersIdList[353].db_ers_id = ersid_val.f6_3_1_4_113;
  ersIdList[353].lim_type = None;
  ersIdList[354].ers_name = "f6_3_2_2_11";
  ersIdList[354].db_ers_id = ersid_val.f6_3_2_2_11;
  ersIdList[354].lim_type = None;
  ersIdList[355].ers_name = "f6_3_1_2_14";
  ersIdList[355].db_ers_id = ersid_val.f6_3_1_2_14;
  ersIdList[355].lim_type = Up;  //inherits 6_3_1_2_1
  ersIdList[356].ers_name = "f6_3_1_2_19";
  ersIdList[356].db_ers_id = ersid_val.f6_3_1_2_19;
  ersIdList[356].lim_type = None;
  ersIdList[357].ers_name = "f6_3_1_2_15";
  ersIdList[357].db_ers_id = ersid_val.f6_3_1_2_15;
  ersIdList[357].lim_type = Up;  //inherits 6_3_1_2_2
  ersIdList[358].ers_name = "f6_3_1_2_16";
  ersIdList[358].db_ers_id = ersid_val.f6_3_1_2_16;
  ersIdList[358].lim_type = None;
  ersIdList[359].ers_name = "f6_3_1_2_17";
  ersIdList[359].db_ers_id = ersid_val.f6_3_1_2_17;
  ersIdList[359].lim_type = Up;  //inherits 6_3_1_2_3
  ersIdList[360].ers_name = "f6_3_1_2_18a";
  ersIdList[360].db_ers_id = ersid_val.f6_3_1_2_18a;
  ersIdList[360].lim_type = None;
  ersIdList[361].ers_name = "f6_3_1_2_18b";
  ersIdList[361].db_ers_id = ersid_val.f6_3_1_2_18b;
  ersIdList[361].lim_type = None;
  ersIdList[362].ers_name = "f6_3_1_2_18c";
  ersIdList[362].db_ers_id = ersid_val.f6_3_1_2_18c;
  ersIdList[362].lim_type = None;
  ersIdList[363].ers_name = "f6_3_1_2_18d";
  ersIdList[363].db_ers_id = ersid_val.f6_3_1_2_18d;
  ersIdList[363].lim_type = None;
  ersIdList[364].ers_name = "f6_3_1_2_18e";
  ersIdList[364].db_ers_id = ersid_val.f6_3_1_2_18e;
  ersIdList[364].lim_type = None;
  ersIdList[365].ers_name = "f6_3_1_2_18f";
  ersIdList[365].db_ers_id = ersid_val.f6_3_1_2_18f;
  ersIdList[365].lim_type = None;
  ersIdList[366].ers_name = "f6_3_1_2_20";
  ersIdList[366].db_ers_id = ersid_val.f6_3_1_2_20;
  ersIdList[366].lim_type = None;
  ersIdList[367].ers_name = "f6_3_1_2_21";
  ersIdList[367].db_ers_id = ersid_val.f6_3_1_2_21;
  ersIdList[367].lim_type = None;
  ersIdList[368].ers_name = "f6_3_1_2_22";
  ersIdList[368].db_ers_id = ersid_val.f6_3_1_2_22;
  ersIdList[368].lim_type = None;
  ersIdList[369].ers_name = "f6_3_1_2_23a";
  ersIdList[369].db_ers_id = ersid_val.f6_3_1_2_23a;
  ersIdList[369].lim_type = None;
  ersIdList[370].ers_name = "f6_3_1_2_23b";
  ersIdList[370].db_ers_id = ersid_val.f6_3_1_2_23b;
  ersIdList[370].lim_type = None;
  ersIdList[371].ers_name = "f6_3_1_2_23c";
  ersIdList[371].db_ers_id = ersid_val.f6_3_1_2_23c;
  ersIdList[371].lim_type = None;
  ersIdList[372].ers_name = "f6_3_1_2_23d";
  ersIdList[372].db_ers_id = ersid_val.f6_3_1_2_23d;
  ersIdList[372].lim_type = None;
  ersIdList[373].ers_name = "f6_3_1_2_23e";
  ersIdList[373].db_ers_id = ersid_val.f6_3_1_2_23e;
  ersIdList[373].lim_type = None;
  ersIdList[374].ers_name = "f6_3_1_2_23f";
  ersIdList[374].db_ers_id = ersid_val.f6_3_1_2_23f;
  ersIdList[374].lim_type = None;
  ersIdList[375].ers_name = "f6_3_10_2_6";
  ersIdList[375].db_ers_id = ersid_val.f6_3_10_2_6;
  ersIdList[375].lim_type = None;
  ersIdList[376].ers_name = "f6_3_10_2_7";
  ersIdList[376].db_ers_id = ersid_val.f6_3_10_2_7;
  ersIdList[376].lim_type = None;
  ersIdList[377].ers_name = "f6_3_10_2_11";
  ersIdList[377].db_ers_id = ersid_val.f6_3_10_2_11;
  ersIdList[377].lim_type = None;
  ersIdList[378].ers_name = "f6_3_10_2_12";
  ersIdList[378].db_ers_id = ersid_val.f6_3_10_2_12;
  ersIdList[378].lim_type = None;
  ersIdList[379].ers_name = "f6_3_10_4_16";
  ersIdList[379].db_ers_id = ersid_val.f6_3_10_4_16;
  ersIdList[379].lim_type = None;
  ersIdList[380].ers_name = "f6_3_10_4_17";
  ersIdList[380].db_ers_id = ersid_val.f6_3_10_4_17;
  ersIdList[380].lim_type = None;
  ersIdList[381].ers_name = "f6_3_10_4_21";
  ersIdList[381].db_ers_id = ersid_val.f6_3_10_4_21;
  ersIdList[381].lim_type = None;
  ersIdList[382].ers_name = "f6_3_10_4_26";
  ersIdList[382].db_ers_id = ersid_val.f6_3_10_4_26;
  ersIdList[382].lim_type = None;
  ersIdList[383].ers_name = "f6_3_10_4_27";
  ersIdList[383].db_ers_id = ersid_val.f6_3_10_4_27;
  ersIdList[383].lim_type = None;
  ersIdList[384].ers_name = "f6_3_10_4_28";
  ersIdList[384].db_ers_id = ersid_val.f6_3_10_4_28;
  ersIdList[384].lim_type = None;
  ersIdList[385].ers_name = "f6_3_10_4_22";
  ersIdList[385].db_ers_id = ersid_val.f6_3_10_4_22;
  ersIdList[385].lim_type = None;
  ersIdList[386].ers_name = "f6_3_10_4_29";
  ersIdList[386].db_ers_id = ersid_val.f6_3_10_4_29;
  ersIdList[386].lim_type = None;
  ersIdList[387].ers_name = "f6_3_10_2_8";
  ersIdList[387].db_ers_id = ersid_val.f6_3_10_2_8;
  ersIdList[387].lim_type = None;
  ersIdList[388].ers_name = "f6_3_10_2_9";
  ersIdList[388].db_ers_id = ersid_val.f6_3_10_2_9;
  ersIdList[388].lim_type = None;
  ersIdList[389].ers_name = "f6_3_10_2_13";
  ersIdList[389].db_ers_id = ersid_val.f6_3_10_2_13;
  ersIdList[389].lim_type = None;
  ersIdList[390].ers_name = "f6_3_10_2_14";
  ersIdList[390].db_ers_id = ersid_val.f6_3_10_2_14;
  ersIdList[390].lim_type = None;
  ersIdList[391].ers_name = "f6_3_10_4_18";
  ersIdList[391].db_ers_id = ersid_val.f6_3_10_4_18;
  ersIdList[391].lim_type = None;
  ersIdList[392].ers_name = "f6_3_10_4_19";
  ersIdList[392].db_ers_id = ersid_val.f6_3_10_4_19;
  ersIdList[392].lim_type = None;
  ersIdList[393].ers_name = "f6_3_10_4_20";
  ersIdList[393].db_ers_id = ersid_val.f6_3_10_4_20;
  ersIdList[393].lim_type = None;
  ersIdList[394].ers_name = "f6_3_10_4_23";
  ersIdList[394].db_ers_id = ersid_val.f6_3_10_4_23;
  ersIdList[394].lim_type = None;
  ersIdList[395].ers_name = "f6_3_10_4_30";
  ersIdList[395].db_ers_id = ersid_val.f6_3_10_4_30;
  ersIdList[395].lim_type = None;
  ersIdList[396].ers_name = "f6_3_10_4_24";
  ersIdList[396].db_ers_id = ersid_val.f6_3_10_4_24;
  ersIdList[396].lim_type = None;
  ersIdList[397].ers_name = "f6_3_10_4_31";
  ersIdList[397].db_ers_id = ersid_val.f6_3_10_4_31;
  ersIdList[397].lim_type = None;
  ersIdList[398].ers_name = "f6_3_10_4_25";
  ersIdList[398].db_ers_id = ersid_val.f6_3_10_4_25;
  ersIdList[398].lim_type = None;
  ersIdList[399].ers_name = "f6_3_10_4_32";
  ersIdList[399].db_ers_id = ersid_val.f6_3_10_4_32;
  ersIdList[399].lim_type = None;
  ersIdList[400].ers_name = "f6_3_1_4_60";
  ersIdList[400].db_ers_id = ersid_val.f6_3_1_4_60;
  ersIdList[400].lim_type = None;
  ersIdList[401].ers_name = "f6_3_1_4_90";
  ersIdList[401].db_ers_id = ersid_val.f6_3_1_4_90;
  ersIdList[401].lim_type = None;
  ersIdList[402].ers_name = "f6_3_1_4_63";
  ersIdList[402].db_ers_id = ersid_val.f6_3_1_4_63;
  ersIdList[402].lim_type = None;
  ersIdList[403].ers_name = "f6_3_1_4_93";
  ersIdList[403].db_ers_id = ersid_val.f6_3_1_4_93;
  ersIdList[403].lim_type = None;
  ersIdList[404].ers_name = "f6_3_1_4_66";
  ersIdList[404].db_ers_id = ersid_val.f6_3_1_4_66;
  ersIdList[404].lim_type = None;
  ersIdList[405].ers_name = "f6_3_1_4_96";
  ersIdList[405].db_ers_id = ersid_val.f6_3_1_4_96;
  ersIdList[405].lim_type = None;
  ersIdList[406].ers_name = "f6_3_1_4_69";
  ersIdList[406].db_ers_id = ersid_val.f6_3_1_4_69;
  ersIdList[406].lim_type = Up;       //inherits 6_3_1_4_4
  ersIdList[407].ers_name = "f6_3_1_4_72";
  ersIdList[407].db_ers_id = ersid_val.f6_3_1_4_72;
  ersIdList[407].lim_type = Up;       //inherits 6_3_1_4_7
  ersIdList[408].ers_name = "f6_3_1_4_99";
  ersIdList[408].db_ers_id = ersid_val.f6_3_1_4_99;
  ersIdList[408].lim_type = None;
  ersIdList[409].ers_name = "f6_3_1_4_85";
  ersIdList[409].db_ers_id = ersid_val.f6_3_1_4_85;
  ersIdList[409].lim_type = Up;       //inherits 6_3_1_4_12
  ersIdList[410].ers_name = "f6_3_1_4_109";
  ersIdList[410].db_ers_id = ersid_val.f6_3_1_4_109;
  ersIdList[410].lim_type = None;
  ersIdList[411].ers_name = "f6_3_1_4_86";
  ersIdList[411].db_ers_id = ersid_val.f6_3_1_4_86;
  ersIdList[411].lim_type = None;
  ersIdList[412].ers_name = "f6_3_1_4_110";
  ersIdList[412].db_ers_id = ersid_val.f6_3_1_4_110;
  ersIdList[412].lim_type = None;
  ersIdList[413].ers_name = "f6_3_2_3_36";
  ersIdList[413].db_ers_id = ersid_val.f6_3_2_3_36;
  ersIdList[413].lim_type = None;
  ersIdList[414].ers_name = "f6_3_2_3_40";
  ersIdList[414].db_ers_id = ersid_val.f6_3_2_3_40;
  ersIdList[414].lim_type = None;
  ersIdList[415].ers_name = "f6_3_2_3_8";
  ersIdList[415].db_ers_id = ersid_val.f6_3_2_3_8;
  ersIdList[415].lim_type = None;
  ersIdList[416].ers_name = "f6_3_2_3_9";
  ersIdList[416].db_ers_id = ersid_val.f6_3_2_3_9;
  ersIdList[416].lim_type = None;
  ersIdList[417].ers_name = "f6_3_2_3_39";
  ersIdList[417].db_ers_id = ersid_val.f6_3_2_3_39;
  ersIdList[417].lim_type = None;
  ersIdList[418].ers_name = "f6_3_2_3_34";
  ersIdList[418].db_ers_id = ersid_val.f6_3_2_3_34;
  ersIdList[418].lim_type = None;
  ersIdList[419].ers_name = "f6_3_2_3_13";
  ersIdList[419].db_ers_id = ersid_val.f6_3_2_3_13;
  ersIdList[419].lim_type = None;
  ersIdList[420].ers_name = "f6_3_2_3_11";
  ersIdList[420].db_ers_id = ersid_val.f6_3_2_3_11;
  ersIdList[420].lim_type = None;
  ersIdList[421].ers_name = "f20_3_1_2_1";
  ersIdList[421].db_ers_id = ersid_val.f20_3_1_2_1;
  ersIdList[421].lim_type = Up;       //inherits 6_3_1_2_1
  ersIdList[422].ers_name = "f20_3_1_2_2";
  ersIdList[422].db_ers_id = ersid_val.f20_3_1_2_2;
  ersIdList[422].lim_type = Up;       //inherits 6_3_1_2_2
  ersIdList[423].ers_name = "f20_3_1_2_3";
  ersIdList[423].db_ers_id = ersid_val.f20_3_1_2_3;
  ersIdList[423].lim_type = None;
  ersIdList[424].ers_name = "f20_3_1_2_4";
  ersIdList[424].db_ers_id = ersid_val.f20_3_1_2_4;
  ersIdList[424].lim_type = Up;      //inherits 6_3_1_2_3
  ersIdList[425].ers_name = "f20_3_1_2_5a";
  ersIdList[425].db_ers_id = ersid_val.f20_3_1_2_5a;
  ersIdList[425].lim_type = None;
  ersIdList[426].ers_name = "f20_3_1_2_5b";
  ersIdList[426].db_ers_id = ersid_val.f20_3_1_2_5b;
  ersIdList[426].lim_type = None;
  ersIdList[427].ers_name = "f20_3_1_2_5c";
  ersIdList[427].db_ers_id = ersid_val.f20_3_1_2_5c;
  ersIdList[427].lim_type = None;
  ersIdList[428].ers_name = "f20_3_1_2_5d";
  ersIdList[428].db_ers_id = ersid_val.f20_3_1_2_5d;
  ersIdList[428].lim_type = None;
  ersIdList[429].ers_name = "f20_3_1_2_5e";
  ersIdList[429].db_ers_id = ersid_val.f20_3_1_2_5e;
  ersIdList[429].lim_type = None;
  ersIdList[430].ers_name = "f20_3_1_2_5f";
  ersIdList[430].db_ers_id = ersid_val.f20_3_1_2_5f;
  ersIdList[430].lim_type = None;
  ersIdList[431].ers_name = "f20_3_1_4_1";
  ersIdList[431].db_ers_id = ersid_val.f20_3_1_4_1;
  ersIdList[431].lim_type = None;
  ersIdList[432].ers_name = "f20_3_1_4_2";
  ersIdList[432].db_ers_id = ersid_val.f20_3_1_4_2;
  ersIdList[432].lim_type = None;
  ersIdList[433].ers_name = "f20_3_1_4_3";
  ersIdList[433].db_ers_id = ersid_val.f20_3_1_4_3;
  ersIdList[433].lim_type = None;
  ersIdList[434].ers_name = "f20_3_1_4_4";
  ersIdList[434].db_ers_id = ersid_val.f20_3_1_4_4;
  ersIdList[434].lim_type = None;
  ersIdList[435].ers_name = "f20_3_1_4_5";
  ersIdList[435].db_ers_id = ersid_val.f20_3_1_4_5;
  ersIdList[435].lim_type = None;
  ersIdList[436].ers_name = "f20_3_1_4_6";
  ersIdList[436].db_ers_id = ersid_val.f20_3_1_4_6;
  ersIdList[436].lim_type = None;
  ersIdList[437].ers_name = "f20_3_1_4_7";
  ersIdList[437].db_ers_id = ersid_val.f20_3_1_4_7;
  ersIdList[437].lim_type = None;
  ersIdList[438].ers_name = "f20_3_1_4_8";
  ersIdList[438].db_ers_id = ersid_val.f20_3_1_4_8;
  ersIdList[438].lim_type = None;
  ersIdList[439].ers_name = "f20_3_1_4_9";
  ersIdList[439].db_ers_id = ersid_val.f20_3_1_4_9;
  ersIdList[439].lim_type = None;
  ersIdList[440].ers_name = "f20_3_1_4_10";
  ersIdList[440].db_ers_id = ersid_val.f20_3_1_4_10;
  ersIdList[440].lim_type = Up;        //inherits 6_3_1_4_4
  ersIdList[441].ers_name = "f20_3_1_4_11";
  ersIdList[441].db_ers_id = ersid_val.f20_3_1_4_11;
  ersIdList[441].lim_type = None;
  ersIdList[442].ers_name = "f20_3_1_4_12";
  ersIdList[442].db_ers_id = ersid_val.f20_3_1_4_12;
  ersIdList[442].lim_type = None;
  ersIdList[443].ers_name = "f20_3_1_4_13";
  ersIdList[443].db_ers_id = ersid_val.f20_3_1_4_13;
  ersIdList[443].lim_type = Up;        //inherits 6_3_1_4_7
  ersIdList[444].ers_name = "f20_3_1_4_14";
  ersIdList[444].db_ers_id = ersid_val.f20_3_1_4_14;
  ersIdList[444].lim_type = None;
  ersIdList[445].ers_name = "f20_3_1_4_15";
  ersIdList[445].db_ers_id = ersid_val.f20_3_1_4_15;
  ersIdList[445].lim_type = None;
  ersIdList[446].ers_name = "f20_3_1_4_16";
  ersIdList[446].db_ers_id = ersid_val.f20_3_1_4_16;
  ersIdList[446].lim_type = None;
  ersIdList[447].ers_name = "f20_3_1_4_17";
  ersIdList[447].db_ers_id = ersid_val.f20_3_1_4_17;
  ersIdList[447].lim_type = None;
  ersIdList[448].ers_name = "f20_3_1_4_18";
  ersIdList[448].db_ers_id = ersid_val.f20_3_1_4_18;
  ersIdList[448].lim_type = Up;        //inherits 6_3_1_4_5
  ersIdList[449].ers_name = "f20_3_1_4_19";
  ersIdList[449].db_ers_id = ersid_val.f20_3_1_4_19;
  ersIdList[449].lim_type = None;
  ersIdList[450].ers_name = "f20_3_1_4_20";
  ersIdList[450].db_ers_id = ersid_val.f20_3_1_4_20;
  ersIdList[450].lim_type = Up;        //inherits 6_3_1_4_14
  ersIdList[451].ers_name = "f20_3_1_4_21";
  ersIdList[451].db_ers_id = ersid_val.f20_3_1_4_21;
  ersIdList[451].lim_type = None;
  ersIdList[452].ers_name = "f20_3_1_4_22";
  ersIdList[452].db_ers_id = ersid_val.f20_3_1_4_22;
  ersIdList[452].lim_type = Up;        //inherits 6_3_1_4_12
  ersIdList[453].ers_name = "f20_3_1_4_23";
  ersIdList[453].db_ers_id = ersid_val.f20_3_1_4_23;
  ersIdList[453].lim_type = None;
  ersIdList[454].ers_name = "f20_3_10_2_1";
  ersIdList[454].db_ers_id = ersid_val.f20_3_10_2_1;
  ersIdList[454].lim_type = None;
  ersIdList[455].ers_name = "f20_3_10_2_2";
  ersIdList[455].db_ers_id = ersid_val.f20_3_10_2_2;
  ersIdList[455].lim_type = None;
  ersIdList[456].ers_name = "f20_3_10_2_3";
  ersIdList[456].db_ers_id = ersid_val.f20_3_10_2_3;
  ersIdList[456].lim_type = None;
  ersIdList[457].ers_name = "f20_3_10_2_4";
  ersIdList[457].db_ers_id = ersid_val.f20_3_10_2_4;
  ersIdList[457].lim_type = None;
  ersIdList[458].ers_name = "f20_3_10_4_1";
  ersIdList[458].db_ers_id = ersid_val.f20_3_10_4_1;
  ersIdList[458].lim_type = None;
  ersIdList[459].ers_name = "f20_3_10_4_2";
  ersIdList[459].db_ers_id = ersid_val.f20_3_10_4_2;
  ersIdList[459].lim_type = None;
  ersIdList[460].ers_name = "f20_3_10_4_3";
  ersIdList[460].db_ers_id = ersid_val.f20_3_10_4_3;
  ersIdList[460].lim_type = None;
  ersIdList[461].ers_name = "f20_3_10_4_4";
  ersIdList[461].db_ers_id = ersid_val.f20_3_10_4_4;
  ersIdList[461].lim_type = None;
  ersIdList[462].ers_name = "f20_3_10_4_5";
  ersIdList[462].db_ers_id = ersid_val.f20_3_10_4_5;
  ersIdList[462].lim_type = None;
  ersIdList[463].ers_name = "f20_3_10_4_6";
  ersIdList[463].db_ers_id = ersid_val.f20_3_10_4_6;
  ersIdList[463].lim_type = None;
  ersIdList[464].ers_name = "f20_3_10_4_7";
  ersIdList[464].db_ers_id = ersid_val.f20_3_10_4_7;
  ersIdList[464].lim_type = None;
  ersIdList[465].ers_name = "f20_3_10_4_8";
  ersIdList[465].db_ers_id = ersid_val.f20_3_10_4_8;
  ersIdList[465].lim_type = None;
  ersIdList[466].ers_name = "f20_3_10_4_9";
  ersIdList[466].db_ers_id = ersid_val.f20_3_10_4_9;
  ersIdList[466].lim_type = None;
  ersIdList[467].ers_name = "f20_3_10_4_10";
  ersIdList[467].db_ers_id = ersid_val.f20_3_10_4_10;
  ersIdList[467].lim_type = None;
  ersIdList[468].ers_name = "f6_3_1_4_114";
  ersIdList[468].db_ers_id = ersid_val.f6_3_1_4_114;
  ersIdList[468].lim_type = None;
  ersIdList[469].ers_name = "f6_3_1_4_115";
  ersIdList[469].db_ers_id = ersid_val.f6_3_1_4_115;
  ersIdList[469].lim_type = None;
  ersIdList[470].ers_name = "f6_3_1_4_116";
  ersIdList[470].db_ers_id = ersid_val.f6_3_1_4_116;
  ersIdList[470].lim_type = None;
  ersIdList[471].ers_name = "f6_3_1_4_117";
  ersIdList[471].db_ers_id = ersid_val.f6_3_1_4_117;
  ersIdList[471].lim_type = None;
  ersIdList[472].ers_name = "f6_3_2_3_5";
  ersIdList[472].db_ers_id = ersid_val.f6_3_2_3_5;
  ersIdList[472].lim_type = None;
  ersIdList[473].ers_name = "f6_3_2_3_45";
  ersIdList[473].db_ers_id = ersid_val.f6_3_2_3_45;
  ersIdList[473].lim_type = None;
  ersIdList[474].ers_name = "f6_3_2_2_12";
  ersIdList[474].db_ers_id = ersid_val.f6_3_2_2_12;
  ersIdList[474].lim_type = None;
  ersIdList[475].ers_name = "f6_3_2_2_13";
  ersIdList[475].db_ers_id = ersid_val.f6_3_2_2_13;
  ersIdList[475].lim_type = None;
  ersIdList[476].ers_name = "f6_3_2_2_14";
  ersIdList[476].db_ers_id = ersid_val.f6_3_2_2_14;
  ersIdList[476].lim_type = None;
}
//-------------------------------------------------------------------



bool Limits_tab::LoadLimits(void)
{
  DBManager_c&  db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t     bswrc = BSWRC_OK,executionResult;
  Statement_c   st;
  ResultSet_c   rs;
  bool          dbirc = true, no_records = true;
  char sql_statement[MAX_SQL_QUERY_SIZE];

  Ushort_t total_records_num ; 
  Ushort_t left_records_num; 

  STATE_Limit_Vector_t       *StateVector;
  ERS_Limit_Vector_t         *ErsVector;

  csa_web_limits_log_d << "Limits_tab::LoadLimits" << LOG_TERMINATOR;

  Ulong_t group_id;
  Ulong_t ers_id;
  Ulong_t red_val;
  Ulong_t yel_val;

  er_sprintf(sql_statement,"select group_id, ers_id, red_val, yellow_val from grp_red_yel");
  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);

  total_records_num = rs.GetRecordCount();
  left_records_num = rs.GetRecordCount();

  if(total_records_num==0 || bswrc!= BSWRC_OK )
    return 1;

  while(rs.Next() && bswrc == BSWRC_OK)
  { 
    if( !(rs.GetUlongByName("group_id",(Ulong_t &)group_id))  ||
        !(rs.GetUlongByName("ers_id",(Ulong_t &)ers_id))      ||
        !(rs.GetUlongByName("red_val",(Ulong_t &)red_val))    ||
        !(rs.GetUlongByName("yellow_val",(Ulong_t &)yel_val)) )
    {		
      //do nothing. get next result set		
      left_records_num--;
    }
    else
    {
      //process color.info
      /*if(ersIdMatrix[group_id][ers_id] != NULL)
      {*/
      if(ers_id >= 1000 )
      {
        ers_id = ers_id - 1000; // normalize ers_id 
        if(ers_id < 480)
        {
          StateVector = GetStateVector(group_id);
          if(StateVector != NULL)
          {
            //char address[20];
            //er_sprintf(address, "%p", StateVector);
            //csa_web_limits_log_d << "LoadLimits of Group " << group_id << " (StateVector address " << address << ")"
            //                     << "ers_id " << ers_id << ", red_val " << red_val << ", yel_val " << yel_val
            //                     << LOG_TERMINATOR;

            StateVector->state_vector[ers_id].red_limit =  (int)red_val;
            StateVector->state_vector[ers_id].yellow_limit =(int)yel_val;  
            StateVector->state_vector[ers_id].inUse = true;
          }
        }
      }
      else
      {
        if(ers_id < 480)
        {
          ErsVector = GetERSVector(group_id);
          if(ErsVector!= NULL)
          {
            //char address[20];
            //er_sprintf(address, "%p", ErsVector);
            //csa_web_limits_log_d << "LoadLimits of Group " << group_id << " (ErsVector address " << address << ")"
            //                     << "ers_id " << ers_id << ", red_val " << red_val << ", yel_val " << yel_val
            //                     << LOG_TERMINATOR;

            ErsVector->ers_vector[ers_id].red_limit = (int)red_val;
            ErsVector->ers_vector[ers_id].yellow_limit = (int)  yel_val;   
            ErsVector->ers_vector[ers_id].inUse = true;
          }
        }
      }

      //}
      left_records_num--;
      if (left_records_num == 0)
        break;
    }	
  }
  return 0;
}


//-----------------------------------------------------------------
bool Limits_tab::UnloadLimitsAndGroups(void)
{
 if(STATE_Limits_tab.size() > 0){
   for(STATE_Limits_it=STATE_Limits_tab.begin(); STATE_Limits_it!= STATE_Limits_tab.end(); ++STATE_Limits_it) 
	   delete STATE_Limits_it->second;
   STATE_Limits_tab.clear();
 }

  if(ERS_Limits_tab.size() > 0){
   for(ERS_Limits_it=ERS_Limits_tab.begin(); ERS_Limits_it!= ERS_Limits_tab.end(); ++ERS_Limits_it) 
	   delete ERS_Limits_it->second;
   ERS_Limits_tab.clear();
 }
 return 0;
}


// PUBLIC 
void Limits_tab::Init_LimitsByGroups(void)
{
  DBManager_c&  db_mngr = UtlStaticObjectsMngr_c::getDBMngr();
  RetCode_t     bswrc = BSWRC_OK,executionResult;
  Statement_c   st;
  ResultSet_c   rs;
  bool          dbirc = true, no_records = true;
  char          sql_statement[MAX_SQL_QUERY_SIZE];

  Ulong_t  grpId ;
  char          grpName[51] ;
  char          grpNumber[51];
  Ulong_t  grpEmailEnable; 

  STATE_Limit_Vector_t* StateVector;
  ERS_Limit_Vector_t*   ErsVector;

  csa_web_limits_log_d << "Limits_tab::Init_LimitsByGroups" << LOG_TERMINATOR;

  er_sprintf(sql_statement,"select group_id,g_name, g_number,email_enabled from grp");

  st.SetQuery(sql_statement);
  bswrc = db_mngr.ExecuteSelectSyn(st, rs, executionResult);

  Ushort_t total_records_num = rs.GetRecordCount();
  Ushort_t left_records_num = rs.GetRecordCount();

  if (total_records_num == 0)
  {
    //NOTHING
  }
  else
  {
    while(left_records_num > 0)
    {
      //start to fill buffer with NlaGroupEntityInfo structures
      while(rs.Next() && bswrc == BSWRC_OK)
      {
        if (!(rs.GetUlongByName("group_id",(Ulong_t &)grpId))            ||
            !(rs.GetStringByName("g_name",grpName))                      ||
            !(rs.GetStringByName("g_number",(char *)grpNumber))        ||
            !(rs.GetUlongByName("email_enabled",(Ulong_t &)grpEmailEnable)))
        {
          //do nothing. get next result set
          left_records_num--;
        }
        else
        {
          StateVector = new STATE_Limit_Vector_t;
          STATE_Limits_tab[grpId]= StateVector;

          //char address[20];
          //er_sprintf(address, "%p", StateVector);
          //csa_web_limits_log_d << "Create StateVector of group " << grpName << " (address " << address << ")" << LOG_TERMINATOR;

          for(int j=0;j<CSA_LAST_STATE;j++)
            StateVector->state_vector[j].inUse = false;

          ErsVector = new ERS_Limit_Vector_t;
          ERS_Limits_tab[grpId]=ErsVector;

          for(int j=0;j<MAX_N_OF_LIMS;j++)
            ErsVector->ers_vector[j].inUse = false;

          left_records_num--;
          //if there are more info records to insert
          if (left_records_num == 0)
            break;
        }
      }
    }
  }
}


//----------------------------------------------------------------------------------------------
bool Limits_tab::GetColorOfSTATE (Ulong_t    group_id,
                                  Ulong_t    sca_state,
                                  Ulong_t    db_ers_id,
                                  Ulong_t    value,
                                  Ulong_t**  bg_color)
{
  STATE_Limit_Vector_t    *StateVector;

  *bg_color = WHITE_COLOR_PTR;

  if(!this->csa_web_TryLock())	//MT
  {
    return 1; //return White
  }

  if(group_id < MAX_GROUP_NUMBER)
  {
    if (ersIdList[db_ers_id].lim_type == None)
    {
      *bg_color = WHITE_COLOR_PTR;
    } 
    else
    {
      StateVector = GetStateVector(group_id);
      if(StateVector == NULL)
      {
        csa_web_UnLock();
        csa_web_limits_log_d << "LoadLimits of Group " << group_id << " got NULL StateVector" << LOG_TERMINATOR;
        return 1;
      }

      //char address[20];
      //er_sprintf(address, "%p", StateVector);
      //csa_web_limits_log_d << "Limits_tab::GetColorOfSTATE " << (Ulong_t)sca_state << " for Group " << group_id << " (StateVector address " << address << ") is ";

      //PVL 27-05-2010  1004103  if(!StateVector->vector[db_ers_id].inUse){
      if(!StateVector->state_vector[sca_state].inUse)  //PVL 27-05-2010  1004103
      {
        //csa_web_limits_log_d << "Not in USE" << LOG_TERMINATOR;
        csa_web_UnLock();
        return 0;
      }

      //csa_web_limits_log_d << "in USE" << LOG_TERMINATOR;

      if (ersIdList[db_ers_id].lim_type == Up)
      {
        if ((int) value >= StateVector->state_vector[sca_state].red_limit){
          *bg_color = RED_COLOR_PTR;
        }
        else
        {
          if ((int) value >= StateVector->state_vector[sca_state].yellow_limit){
            *bg_color = YELLOW_COLOR_PTR;
          }
          else{ 
            *bg_color = WHITE_COLOR_PTR;
          }
        }
      }//UP
      if (ersIdList[db_ers_id].lim_type == Down)
      {
        if ((int) value <= StateVector->state_vector[sca_state].red_limit) {
          *bg_color = RED_COLOR_PTR;
        }
        else
        {
          if ((int) value <= StateVector->state_vector[sca_state].yellow_limit) {
            *bg_color = YELLOW_COLOR_PTR;
          }
          else{
            *bg_color = WHITE_COLOR_PTR;
          }
        }
      }//DOWN
    } // NOT white
    csa_web_UnLock();
    return 1;
  }//group
  csa_web_UnLock();
  return 0;
}


//-------------------------------------------------------------------
bool Limits_tab::GetColorOfERS(Ulong_t   group_id,
                               Ulong_t   db_ers_id,
                               Ulong_t   value,
                               Ulong_t** bg_color)
{
  ERS_Limit_Vector_t         *ErsVector;
  *bg_color = WHITE_COLOR_PTR;

  if(!this->csa_web_TryLock())	//MT
  {
    return 1; //return White
  }
  if (ersIdList[db_ers_id].lim_type == None){
    *bg_color = WHITE_COLOR_PTR;
  } 
  else
  {
    ErsVector = GetERSVector(group_id);
    if(ErsVector == NULL){
      csa_web_UnLock();
      return 1;
    }
    if(!ErsVector->ers_vector[db_ers_id].inUse){
      csa_web_UnLock();
      return 0;
    }
    if (ersIdList[db_ers_id].lim_type == Up)
    {
      if ((int)value >= ErsVector->ers_vector[db_ers_id].red_limit){
        *bg_color = RED_COLOR_PTR;
      }
      else
      {
        if ((int)value >= ErsVector->ers_vector[db_ers_id].yellow_limit){
          *bg_color = YELLOW_COLOR_PTR;
        }
        else{
          *bg_color =WHITE_COLOR_PTR;
        }
      }
    }//UP
    if (ersIdList[db_ers_id].lim_type == Down)
    {
      if ((int)value <= ErsVector->ers_vector[db_ers_id].red_limit) {
        *bg_color = RED_COLOR_PTR;
      }
      else
      {
        if ((int)value <= ErsVector->ers_vector[db_ers_id].yellow_limit) {
          *bg_color = YELLOW_COLOR_PTR;
        }
        else{
          *bg_color = WHITE_COLOR_PTR;
        }
      }
    }//DOWN	
  } //not white
  csa_web_UnLock();
  return 1;
//  return 0;
}


//PVL 28-07-2009 1002567
void Limits_tab::VerifyGroupInLimitTab(Ulong_t group_id)
{
  ERS_Limit_Vector_t    *ErsVector;
  this->csa_web_TryLockInfinite();	//MT  
  ErsVector = GetERSVector(group_id);
  if(ErsVector == NULL)
  {
    //Unload Limits
    UnloadLimitsAndGroups();
    //Reload Limits
    Init_LimitsByGroups();
    LoadLimits();
  }
  this->csa_web_UnLock();
}


//PVL 28-07-2009 1002567
void Limits_tab::ReloadAll(void) //PVL 29-07-2009 1002487
{      
  csa_web_TryLockInfinite();
  //Unload Limits
  UnloadLimitsAndGroups();
  //Reload Limits
  Init_LimitsByGroups();
  LoadLimits();
  csa_web_UnLock();
}


Limits_tab::STATE_Limit_Vector_t* Limits_tab::GetStateVector(Ulong_t group_id)
{ 
  if(STATE_Limits_tab.find(group_id) != STATE_Limits_tab.end())
    return STATE_Limits_tab.find(group_id)->second;
  else
    return NULL;
}


Limits_tab::ERS_Limit_Vector_t* Limits_tab::GetERSVector(Ulong_t group_id)
{ 
  if(ERS_Limits_tab.find(group_id) != ERS_Limits_tab.end())
    return ERS_Limits_tab.find(group_id)->second;
  else
    return NULL;
}
