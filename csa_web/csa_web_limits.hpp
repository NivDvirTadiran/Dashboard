#ifndef GSOAP_WEB_LIMITS_H
#define GSOAP_WEB_LIMITS_H

#include "acsasta.hpp"
#include <er_ptr_map.h>
#include "csa_web_lock.hpp"

#define MAX_GROUP_NUMBER        20
#define MAX_N_OF_LIMS           480   //NLA currently, there are 16 lims for Voice. 9

typedef struct {
  const static Ulong_t f6_3_1_1_1   = 1;    ////Group name}                  //real time begins with "6"}
  const static Ulong_t f6_3_1_1_2   = 2;    // //Group Number}
  const static Ulong_t f6_3_1_1_3   = 3;    ////Time since the beginnig of the current interval}
  const static Ulong_t f6_3_1_2_1   = 4;    //LIM     ////Number of waiting calls}
  const static Ulong_t f6_3_1_2_2   = 5;    //LIM     ////number of calls waiting abouve target asa}
  const static Ulong_t f6_3_1_2_3   = 6;    //LIM     ////average wait time oconst static Ulong_t f calls in the Queue}
  const static Ulong_t f6_3_1_2_4   = 7;    //LIM     ////Longest wait time oconst static Ulong_t f a call in que}
  const static Ulong_t f6_3_1_2_5a  = 8;   //consts for external ids}
  const static Ulong_t f6_3_1_2_5b  = 9;
  const static Ulong_t f6_3_1_2_5c  = 10;
  const static Ulong_t f6_3_1_2_5d  = 11;
  const static Ulong_t f6_3_1_2_5e  = 12;
  const static Ulong_t f6_3_1_2_5f  = 13;
  const static Ulong_t f6_3_1_3_1   = 14;//LIM
  const static Ulong_t f6_3_1_3_2   = 15;//LIM
  const static Ulong_t f6_3_1_3_3   = 16;
  const static Ulong_t f6_3_1_3_4   = 17;//LIM
  const static Ulong_t f6_3_1_3_5   = 18;
  const static Ulong_t f6_3_1_3_6   = 19;//LIM
  const static Ulong_t f6_3_1_3_7   = 20;
  const static Ulong_t f6_3_1_3_8   = 21;
  const static Ulong_t f6_3_1_4_1   = 22;//LIM
  const static Ulong_t f6_3_1_4_2   = 23;
  const static Ulong_t f6_3_1_4_3   = 24;
  const static Ulong_t f6_3_1_4_3a  = 25;
  const static Ulong_t f6_3_1_4_4   = 26;//LIM
  const static Ulong_t f6_3_1_4_4a  = 27;
  const static Ulong_t f6_3_1_4_5   = 28;//LIM
  const static Ulong_t f6_3_1_4_6   = 29;
  const static Ulong_t f6_3_1_4_7   = 30; //LIM////Inter intfowed out}
  const static Ulong_t f6_3_1_4_7a  = 31;
  const static Ulong_t f6_3_1_4_8   = 32;//LIM
  const static Ulong_t f6_3_1_4_9   = 33;
  const static Ulong_t f6_3_1_4_10  = 34; ////Inter fowed in}
  const static Ulong_t f6_3_1_4_10a = 35;
  const static Ulong_t f6_3_1_4_11  = 36;//LIM
  const static Ulong_t f6_3_1_4_12  = 37;//LIM
  const static Ulong_t f6_3_1_4_13  = 38;
  const static Ulong_t f6_3_1_4_14  = 39;//LIM
  const static Ulong_t f6_3_1_4_15  = 40;
  const static Ulong_t f6_3_1_4_16  = 41;//LIM
  const static Ulong_t f6_3_2_1_1   = 42; ////agent_name}
  const static Ulong_t f6_3_2_1_2   = 43; ////agent_num}
  const static Ulong_t f6_3_2_1_3   = 44; ////ext_num}
  const static Ulong_t f6_3_2_2_1   = 45; ////state}
  const static Ulong_t f6_3_2_2_2   = 46; ////time in current state}
  const static Ulong_t f6_3_2_3_44  = 47;
  const static Ulong_t f6_3_2_3_4   = 48; ////acd_answered_calls}
  const static Ulong_t f6_3_2_2_7   = 49;
  const static Ulong_t f6_3_2_2_9   = 50;
  const static Ulong_t f6_3_2_2_8   = 51;
  const static Ulong_t f6_3_2_2_6   = 52;
  const static Ulong_t f6_3_1_4_61  = 53;
  const static Ulong_t f6_3_1_4_62  = 54;
  const static Ulong_t f6_3_1_4_64  = 55;
  const static Ulong_t f6_3_1_4_65  = 56;
  const static Ulong_t f6_3_1_4_67  = 57;
  const static Ulong_t f6_3_2_3_10  = 58; ////accumulated_wrapup_time}
  const static Ulong_t f6_3_1_4_68  = 59;
  const static Ulong_t f6_3_1_4_70  = 60;
  const static Ulong_t f6_3_1_4_71  = 61;
  const static Ulong_t f6_3_2_3_12  = 62; ////total_released_time}
  const static Ulong_t f6_3_2_3_12a = 63; ////(longInt(acc_rls_time) * 100) div longInt(acc_login_time))}
  const static Ulong_t f6_3_1_4_73  = 64;
  const static Ulong_t f6_3_1_4_74  = 65;
  const static Ulong_t f6_3_4_1_1   = 66;
  const static Ulong_t f6_3_4_2_1   = 67;
  const static Ulong_t f6_3_4_2_2   = 68;
  const static Ulong_t f6_3_4_2_3   = 69;
  const static Ulong_t f6_3_4_2_4   = 70;
  const static Ulong_t f6_3_4_3_1   = 71;
  const static Ulong_t f6_3_4_3_2   = 72;
  const static Ulong_t f6_3_4_3_3   = 73;
  const static Ulong_t f6_3_4_3_4   = 74;
  const static Ulong_t f6_3_4_3_5   = 75;
  const static Ulong_t f6_3_4_3_6   = 76;
  const static Ulong_t f6_3_4_3_7   = 77;
  const static Ulong_t f6_3_4_3_8   = 78;
  const static Ulong_t f6_3_4_4_1   = 79;
  const static Ulong_t f6_3_4_4_2   = 80;
  const static Ulong_t f6_3_4_4_3   = 81;
  const static Ulong_t f6_3_4_4_3a  = 82;
  const static Ulong_t f6_3_4_4_4   = 83;
  const static Ulong_t f6_3_4_4_4a  = 84;
  const static Ulong_t f6_3_4_4_5   = 85;
  const static Ulong_t f6_3_4_4_6   = 86;
  const static Ulong_t f6_3_4_4_7   = 87;
  const static Ulong_t f6_3_4_4_7a  = 88;
  const static Ulong_t f6_3_4_4_8   = 89;
  const static Ulong_t f6_3_4_4_9   = 90;
  const static Ulong_t f6_3_4_4_10  = 91;
  const static Ulong_t f6_3_4_4_10a = 92;
  const static Ulong_t f6_3_4_4_11  = 93;
  const static Ulong_t f6_3_4_4_12  = 94;
  const static Ulong_t f6_3_4_4_13  = 95;
  const static Ulong_t f6_3_4_4_14  = 96;
  const static Ulong_t f6_3_4_4_15  = 97;
  const static Ulong_t f6_3_4_4_16  = 98;
  const static Ulong_t f6_3_5_1_1   = 99;
  const static Ulong_t f6_3_5_1_2   = 100;
  const static Ulong_t f6_3_5_2_1   = 101;
  const static Ulong_t f6_3_5_2_2   = 102;
  const static Ulong_t f6_3_6_1_1   = 103;
  const static Ulong_t f6_3_6_1_2   = 104;
  const static Ulong_t f6_3_6_2_1   = 105;
  const static Ulong_t f6_3_7_1_1   = 106;
  const static Ulong_t f6_3_7_1_2   = 107;
  const static Ulong_t f6_3_7_2_1   = 108;
  const static Ulong_t f6_3_9_1_1   = 109;
  const static Ulong_t f6_3_9_1_2   = 110;
  const static Ulong_t f6_3_9_2_1   = 111;
  const static Ulong_t f6_3_9_2_2   = 112;
  const static Ulong_t f6_3_9_2_3   = 113;
  const static Ulong_t f6_3_9_2_4   = 114;
  const static Ulong_t f6_3_9_2_5   = 115;
  const static Ulong_t f6_3_9_2_6   = 116;
  const static Ulong_t f6_3_5_2_3   = 117;
  const static Ulong_t f6_3_1_4_17  = 118; ////rqd_agns}
  const static Ulong_t f6_3_1_5_1   = 119; ////intconst static Ulong_tfl_calls_lp}
  const static Ulong_t f6_3_1_5_2   = 120; ////tsconst static Ulong_tf_lp}
  const static Ulong_t f6_3_1_5_3   = 121; ////rqd_agns_lp}
  const static Ulong_t f6_3_1_5_4   = 122; ////avg_calls_waiting_lp}
  const static Ulong_t f6_3_1_1_4   = 123; ////interval time}
  const static Ulong_t f6_3_2_2_3   = 124; ////calls_on_hold}
  const static Ulong_t f6_3_6_2_2   = 125;
  const static Ulong_t f6_3_1_1_5   = 126; ////target_asa}
  const static Ulong_t f6_3_1_4_18  = 127; ////avg_wrapup_time}
  const static Ulong_t f6_3_1_4_19  = 128; ////avg_handle_time}
  const static Ulong_t f6_3_1_5_5   = 129; ////avg_wrapup_time_lp}
  const static Ulong_t f6_3_1_5_6   = 130; ////avg_handle_time_lp}
  const static Ulong_t f6_3_1_3_9   = 131;
  const static Ulong_t f6_3_1_3_14  = 132; ////RR: 131 //agents_on_DND}
  const static Ulong_t f6_3_1_3_10  = 133; //132 //primary_agents}
  const static Ulong_t f6_3_1_3_11  = 134; //133 //secondary_agents}
  const static Ulong_t f6_3_1_3_15  = 135; //agents on split}
  const static Ulong_t f6_3_1_3_16  = 136; //agents on wrapup +  f.wrapup}
  const static Ulong_t f6_3_1_3_13  = 137; //agents on  f.wrapup}
  const static Ulong_t f6_3_1_4_35  = 138; //avg. wrap +  f.wrap time}
  const static Ulong_t f6_3_2_1_4   = 139; //RR: 14/12/97 prim group }
  const static Ulong_t f6_3_2_1_5   = 140; //RR: 14/12/97 priority }
  const static Ulong_t f6_3_1_5_7   = 141; //abandoned calls  for last period}
  const static Ulong_t f6_3_4_3_9   = 142; //agents on dnd  for super group}
  const static Ulong_t f6_3_4_3_10  = 143; //agents on split  for super group}
  const static Ulong_t f6_3_4_3_11  = 144; //agents on wrap +  f. wrap}
  const static Ulong_t f6_3_4_4_17  = 145; //avg wrap +  f. wrap time const static Ulong_t for super group}
  const static Ulong_t f6_3_4_3_12  = 146; //agents on  f.wrap}
  const static Ulong_t f6_3_2_3_16  = 147;
  const static Ulong_t f6_3_2_3_17  = 148;
  const static Ulong_t f6_3_2_3_18  = 149;
  const static Ulong_t f6_3_2_3_19  = 150;
  const static Ulong_t f6_3_1_4_27  = 151; //tranconst static Ulong_tferred_in}
  const static Ulong_t f6_3_1_4_28  = 152; //tranconst static Ulong_tferred_out}
  const static Ulong_t f6_3_1_4_29  = 153; //delayed_calls}
  const static Ulong_t f7_3_1_0     = 154; //Interval starting at...}      // from the dbi not const static Ulong_t from CSA....}
  const static Ulong_t f7_3_1_2_8   = 155; //Total ACD incoming}          //also for hourly}
  const static Ulong_t f7_3_1_2_22  = 156; //answered calls}
  const static Ulong_t f7_3_1_2_10  = 157; //abandoned calls}
  const static Ulong_t f7_3_1_2_34  = 158; //Delayed calls}
  const static Ulong_t f7_3_1_2_16  = 159; //# records}
  const static Ulong_t f7_3_1_3_17a = 160; //avg. delay time}
  const static Ulong_t f7_3_1_3_5a  = 161; //avg. talk time}
  const static Ulong_t f7_3_1_3_10  = 162; //avg. abandond time}
  const static Ulong_t f7_3_1_3_21a = 163; //avg. wrapup time including forced wrap up}
  const static Ulong_t f7_3_1_3_6a  = 164; //avg. wrapup time with out  forced wrap up}
  const static Ulong_t f6_3_1_4_36  = 165; //Originally accepted}
  const static Ulong_t f6_3_1_4_37  = 166; //group_calls_answered}
  const static Ulong_t f6_3_1_4_38  = 167; //answered_in_another_group_calls}
  const static Ulong_t f6_3_1_4_39  = 168; //Over flowed_in}
  const static Ulong_t f6_3_1_4_40  = 169; //over flowed_in_calls_answered}
  const static Ulong_t f6_3_1_4_41  = 170; //overflowed_in_calls_answered_in_another_group}
  const static Ulong_t f6_3_1_4_42  = 171; //group_calls_answered_in_another_group}
  const static Ulong_t f6_3_1_4_43  = 172; //overflowed_in_calls_interconst static Ulong_t flowed_out}
  const static Ulong_t f6_3_1_4_44  = 173; //group_calls_interflowed_out}
  const static Ulong_t f6_3_1_4_45  = 174; //group_calls_abandoned}
  const static Ulong_t f6_3_1_4_46  = 175; //overflowed_in_calls_abandoned}
  const static Ulong_t f6_3_1_4_47  = 176; //Overflowed_out}
  const static Ulong_t f6_3_1_4_76  = 177;
  const static Ulong_t f6_3_1_4_77  = 178;
  const static Ulong_t f6_3_1_4_79  = 179;
  const static Ulong_t f6_3_1_3_17  = 180; //required number of agents}
  const static Ulong_t f6_3_1_3_18  = 181; //understafconst static Ulong_t fed number oconst static Ulong_t f agents}
  const static Ulong_t f6_3_1_3_19  = 182; //overstafconst static Ulong_t fed number oconst static Ulong_t f agents}
  const static Ulong_t f6_3_1_3_20  = 183; //min beween required and logedin numbers oconst static Ulong_t f agents}
  const static Ulong_t f6_3_1_3_21  = 184; //logged in - release}
  const static Ulong_t f6_3_1_3_18a = 185; //percent of understaconst static Ulong_t fconst static Ulong_t fed in required}
  const static Ulong_t f6_3_1_3_19a = 186; //percent of overstaconst static Ulong_t fconst static Ulong_t fed in required}
  const static Ulong_t f6_3_1_3_20a = 187; //percent of (const static Ulong_t f6_3_1_3_20) in required }
  const static Ulong_t f6_3_1_4_80  = 188;
  const static Ulong_t f6_3_1_4_81  = 189;
  const static Ulong_t f6_3_2_3_14  = 190; //NS: 16.6.98 Presented ACD Calls}
  const static Ulong_t f6_3_1_3_12  = 191; //RR: 29/07/98 forced busy agents}
  const static Ulong_t f6_3_10_1_1  = 192; //11/10/99 DNIS name}
  const static Ulong_t f6_3_10_1_2  = 193; //11/10/99 DNIS number}
  const static Ulong_t f6_3_10_1_3  = 194; //11/10/99 Time since the beginning oconst static Ulong_t f the current time int.}
  const static Ulong_t f6_3_10_1_4  = 195; //11/10/99 DNIS time interval}
  const static Ulong_t f6_3_10_1_5  = 196;//target asa}
  const static Ulong_t f6_3_10_2_1  = 197; //11/10/99 number of waiting calls}
  const static Ulong_t f6_3_10_2_2  = 198; //11/10/99 number of waiting calls above target ASA}
  const static Ulong_t f6_3_10_2_3  = 199; //11/10/99 avg. wait time of call in q}
  const static Ulong_t f6_3_10_2_4  = 200; //11/10/99 longest wait time oconst static Ulong_t f call in the q}
  const static Ulong_t f6_3_10_2_5a = 201; //11/10/99 STI1}
  const static Ulong_t f6_3_10_2_5b = 202; //11/10/99 STI2}
  const static Ulong_t f6_3_10_2_5c = 203; //11/10/99 STI3}
  const static Ulong_t f6_3_10_2_5d = 204; //11/10/99 STI4}
  const static Ulong_t f6_3_10_2_5e = 205; //11/10/99 STI5}
  const static Ulong_t f6_3_10_2_5f = 206; //11/10/99 STI6}
  const static Ulong_t f6_3_10_3_1  = 207; //11/10/99 longest acd talk time}
  const static Ulong_t f6_3_10_4_1  = 208; //11/10/99 DNIS TSconst static Ulong_t f}
  const static Ulong_t f6_3_10_4_2  = 209; //11/10/99 number of incoming DNIS calls}
  const static Ulong_t f6_3_10_4_3  = 210; //11/10/99 number of answered DNIS calls}
  const static Ulong_t f6_3_10_4_3a = 211; //11/10/99  % answered DNIS calls const static Ulong_t from all inc calls}
  const static Ulong_t f6_3_10_4_4  = 212; //11/10/99 number of abandoned calls}
  const static Ulong_t f6_3_10_4_4a = 213; //11/10/99 % abandoned calls const static Ulong_t from all inc. calls}
  const static Ulong_t f6_3_10_4_5  = 214; //11/10/99 avg. time of aband. calls}
  const static Ulong_t f6_3_10_4_6  = 215; //11/10/99 longest time of aband. calls}
  const static Ulong_t f6_3_10_4_7  = 216; //11/10/99 avg. wait time in q}
  const static Ulong_t f6_3_10_4_8  = 217; //11/10/99 longest wait time in q}
  const static Ulong_t f6_3_10_4_9  = 218; //11/10/99 avg. talk time}
  const static Ulong_t f6_3_10_4_10 = 219; //11/10/99 longest talk time}
  const static Ulong_t f6_3_10_4_11 = 220; //11/10/99 avg. answer time}
  const static Ulong_t f6_3_10_4_12 = 221; //11/10/99 avg. hold time}
  const static Ulong_t f6_3_10_4_13 = 222; //11/10/99 num of dnis call which transconst static Ulong_t ferred at least once}
  const static Ulong_t f6_3_10_4_14 = 223; //11/10/99 accomulated talk time}
  const static Ulong_t f6_3_10_4_15 = 224; //11/10/99 accomulated aband. time}
  const static Ulong_t f6_3_10_5_1  = 225; //11/10/99 tsf const static Ulong_t for previous int.}
  const static Ulong_t f6_3_10_5_2  = 226; //11/10/99 avg. call waiting const static Ulong_t for lp}
  const static Ulong_t f6_3_10_5_3  = 227; //11/10/99 aband. calls for lp}
  const static Ulong_t f6_3_10_5_4  = 228; //11/10/99 answered calls  for lp}
  const static Ulong_t f6_3_10_5_4a = 229; //11/10/99 % answered calls  from all inc calls const static Ulong_t for lp}
  const static Ulong_t f6_3_2_2_4   = 230; //Ani const static Ulong_t for agent acd call}
  const static Ulong_t f6_3_2_2_5   = 231; //dnis const static Ulong_t for agent acd call}
  const static Ulong_t f6_3_4_4_18  = 232;
  const static Ulong_t f6_3_4_4_19  = 233;
  const static Ulong_t f6_3_4_4_20  = 234;
  const static Ulong_t f6_3_4_4_21  = 235;
  const static Ulong_t f6_3_4_4_22  = 236;
  const static Ulong_t f6_3_4_4_23  = 237;
  const static Ulong_t f6_3_4_4_24  = 238;
  const static Ulong_t f6_3_4_4_25  = 239;
  const static Ulong_t f6_3_4_4_26  = 240;
  const static Ulong_t f6_3_4_4_27  = 241;
  const static Ulong_t f6_3_4_4_28  = 242;
  const static Ulong_t f6_3_4_4_29  = 243;
  const static Ulong_t f6_3_1_2_7   = 244;
  const static Ulong_t f6_3_1_3_23  = 245;
  const static Ulong_t f6_3_1_4_48  = 246;
  const static Ulong_t f6_3_1_4_50  = 247;
  const static Ulong_t f6_3_1_2_9   = 248;
  const static Ulong_t f6_3_1_2_10  = 249;
  const static Ulong_t f6_3_1_3_22  = 250;
  const static Ulong_t f6_3_1_4_49  = 251;
  const static Ulong_t f6_3_1_4_52  = 252;
  const static Ulong_t f6_3_1_4_53  = 253;
  const static Ulong_t f6_3_1_4_55  = 254;
  const static Ulong_t f6_3_1_4_56  = 255;
  const static Ulong_t f6_3_1_4_58  = 256;
  const static Ulong_t f6_3_1_4_59  = 257;
  const static Ulong_t f6_3_4_2_5   = 258;
  const static Ulong_t f6_3_4_2_6   = 259;
  const static Ulong_t f6_3_4_2_7   = 260;
  const static Ulong_t f6_3_4_3_13  = 261;
  const static Ulong_t f6_3_4_3_14  = 262;
  const static Ulong_t f7_3_1_2_1   = 263;
  const static Ulong_t f6_3_1_3_24  = 264; // Orsrvd + Oacd agents
  const static Ulong_t f6_3_1_2_8a  = 265;
  const static Ulong_t f6_3_1_2_8b  = 266;
  const static Ulong_t f6_3_1_2_8c  = 267;
  const static Ulong_t f6_3_1_2_8d  = 268;
  const static Ulong_t f6_3_1_2_8e  = 269;
  const static Ulong_t f6_3_1_2_8f  = 270;
  const static Ulong_t f6_3_12_1_1  = 271;
  const static Ulong_t f6_3_12_2_1  = 272;
  const static Ulong_t f6_3_12_2_2  = 273;
  const static Ulong_t f6_3_13_1_1  = 274;
  const static Ulong_t f6_3_13_1_2  = 275;
  const static Ulong_t f6_3_13_2_1  = 276;
  const static Ulong_t f6_3_13_2_2  = 277;
  const static Ulong_t f6_3_13_2_3  = 278;
  const static Ulong_t f6_3_13_2_4  = 279;
  const static Ulong_t f6_3_11_1_1  = 280; // Name
  const static Ulong_t f6_3_11_1_2  = 281; // Id

  //Ivr Application On Line}
  const static Ulong_t f6_3_11_2_1  = 282;
  const static Ulong_t f6_3_11_2_2  = 283;
  const static Ulong_t f6_3_11_2_3  = 284;
  const static Ulong_t f6_3_11_2_4  = 285;
  const static Ulong_t f6_3_11_2_5  = 286;
  const static Ulong_t f6_3_11_2_6  = 287;
  const static Ulong_t f6_3_11_2_7  = 288;
  const static Ulong_t f6_3_11_2_8  = 289;
  const static Ulong_t f6_3_11_3_1  = 290;
  const static Ulong_t f6_3_11_3_2  = 291;
  const static Ulong_t f6_3_11_3_3  = 292;
  const static Ulong_t f6_3_11_3_4  = 293;
  const static Ulong_t f6_3_11_3_5  = 294;
  const static Ulong_t f6_3_11_3_6  = 295;
  const static Ulong_t f6_3_11_3_7  = 296;
  const static Ulong_t f6_3_11_3_8  = 297;
  const static Ulong_t f6_3_11_3_9  = 298;
  const static Ulong_t f6_3_11_3_10 = 299;
  const static Ulong_t f6_3_11_3_11 = 300;
  const static Ulong_t f6_3_11_3_12 = 301;
  const static Ulong_t f6_3_11_3_13 = 302;
  const static Ulong_t f6_3_11_3_14 = 303;
  const static Ulong_t f6_3_11_3_15 = 304;
  const static Ulong_t f6_3_11_3_16 = 305;
  const static Ulong_t f6_3_11_3_17 = 306;
  const static Ulong_t f6_3_11_3_18 = 307;
  const static Ulong_t f6_3_11_3_19 = 308;
  const static Ulong_t f6_3_11_3_20 = 309;
  const static Ulong_t f6_3_11_3_21 = 310;
  const static Ulong_t f6_3_11_3_22 = 311;
  const static Ulong_t f6_3_11_3_23 = 312;
  const static Ulong_t f6_3_11_3_24 = 313;
  const static Ulong_t f6_3_11_3_25 = 314;
  const static Ulong_t f6_3_11_3_26 = 315;
  const static Ulong_t f6_3_11_3_27 = 316;
  const static Ulong_t f6_3_11_3_28 = 317;
  const static Ulong_t f6_3_11_3_29 = 318;
  const static Ulong_t f6_3_11_3_30 = 319;
  const static Ulong_t f6_3_11_3_31 = 320;
  const static Ulong_t f6_3_11_3_32 = 321;
  const static Ulong_t f6_3_11_3_33 = 322;
  const static Ulong_t f6_3_11_3_34 = 323;
  const static Ulong_t f6_3_11_3_35 = 324;
  const static Ulong_t f6_3_11_3_36 = 325;
  const static Ulong_t f6_3_1_2_11  = 326;
  const static Ulong_t f6_3_1_2_12  = 327;
  const static Ulong_t f6_3_1_2_13  = 328;
  const static Ulong_t f6_3_14_1_1  = 329;
  const static Ulong_t f6_3_14_1_2  = 330;
  const static Ulong_t f6_3_14_2_1  = 331;
  const static Ulong_t f6_3_14_2_2  = 332;
  const static Ulong_t f6_3_14_3_1  = 333;
  const static Ulong_t f6_3_14_3_2  = 334;
  const static Ulong_t f6_3_1_3_25  = 335; // Grp - agents on Email
  const static Ulong_t f6_3_1_4_83  = 336;
  const static Ulong_t f6_3_2_2_10  = 337;
  const static Ulong_t f6_3_1_4_88  = 338;
  const static Ulong_t f6_3_1_4_89  = 339;
  const static Ulong_t f6_3_1_4_91  = 340;
  const static Ulong_t f6_3_1_4_92  = 341;
  const static Ulong_t f6_3_1_4_94  = 342;
  const static Ulong_t f6_3_1_4_95  = 343;
  const static Ulong_t f6_3_1_4_97  = 344;
  const static Ulong_t f6_3_1_4_98  = 345;
  const static Ulong_t f6_3_1_4_100 = 346;
  const static Ulong_t f6_3_1_4_101 = 347;
  const static Ulong_t f6_3_1_4_103 = 348;
  const static Ulong_t f6_3_1_4_104 = 349;
  const static Ulong_t f6_3_1_4_105 = 350;
  const static Ulong_t f6_3_1_4_107 = 351;
  const static Ulong_t f6_3_1_4_112 = 352;
  const static Ulong_t f6_3_1_4_113 = 353;
  const static Ulong_t f6_3_2_2_11  = 354;
  const static Ulong_t f6_3_1_2_14  = 355;
  const static Ulong_t f6_3_1_2_19  = 356;
  const static Ulong_t f6_3_1_2_15  = 357;
  const static Ulong_t f6_3_1_2_16  = 358;
  const static Ulong_t f6_3_1_2_17  = 359;
  const static Ulong_t f6_3_1_2_18a = 360;
  const static Ulong_t f6_3_1_2_18b = 361;
  const static Ulong_t f6_3_1_2_18c = 362;
  const static Ulong_t f6_3_1_2_18d = 363;
  const static Ulong_t f6_3_1_2_18e = 364;
  const static Ulong_t f6_3_1_2_18f = 365;
  const static Ulong_t f6_3_1_2_20  = 366;
  const static Ulong_t f6_3_1_2_21  = 367;
  const static Ulong_t f6_3_1_2_22  = 368;
  const static Ulong_t f6_3_1_2_23a = 369;
  const static Ulong_t f6_3_1_2_23b = 370;
  const static Ulong_t f6_3_1_2_23c = 371;
  const static Ulong_t f6_3_1_2_23d = 372;
  const static Ulong_t f6_3_1_2_23e = 373;
  const static Ulong_t f6_3_1_2_23f = 374;
  const static Ulong_t f6_3_10_2_6  = 375;
  const static Ulong_t f6_3_10_2_7  = 376;
  const static Ulong_t f6_3_10_2_11 = 377;
  const static Ulong_t f6_3_10_2_12 = 378;
  const static Ulong_t f6_3_10_4_16 = 379;
  const static Ulong_t f6_3_10_4_17 = 380;
  const static Ulong_t f6_3_10_4_21 = 381;
  const static Ulong_t f6_3_10_4_26 = 382;
  const static Ulong_t f6_3_10_4_27 = 383;
  const static Ulong_t f6_3_10_4_28 = 384;
  const static Ulong_t f6_3_10_4_22 = 385;
  const static Ulong_t f6_3_10_4_29 = 386;
  const static Ulong_t f6_3_10_2_8  = 387;
  const static Ulong_t f6_3_10_2_9  = 388;
  const static Ulong_t f6_3_10_2_13 = 389;
  const static Ulong_t f6_3_10_2_14 = 390;
  const static Ulong_t f6_3_10_4_18 = 391;
  const static Ulong_t f6_3_10_4_19 = 392;
  const static Ulong_t f6_3_10_4_20 = 393;
  const static Ulong_t f6_3_10_4_23 = 394;
  const static Ulong_t f6_3_10_4_30 = 395;
  const static Ulong_t f6_3_10_4_24 = 396;
  const static Ulong_t f6_3_10_4_31 = 397;
  const static Ulong_t f6_3_10_4_25 = 398;
  const static Ulong_t f6_3_10_4_32 = 399;
  const static Ulong_t f6_3_1_4_60  = 400;
  const static Ulong_t f6_3_1_4_90  = 401;
  const static Ulong_t f6_3_1_4_63  = 402;
  const static Ulong_t f6_3_1_4_93  = 403;
  const static Ulong_t f6_3_1_4_66  = 404;
  const static Ulong_t f6_3_1_4_96  = 405;
  const static Ulong_t f6_3_1_4_69  = 406;
  const static Ulong_t f6_3_1_4_72  = 407;
  const static Ulong_t f6_3_1_4_99  = 408;
  const static Ulong_t f6_3_1_4_85  = 409;
  const static Ulong_t f6_3_1_4_109 = 410;
  const static Ulong_t f6_3_1_4_86  = 411;
  const static Ulong_t f6_3_1_4_110 = 412;
  const static Ulong_t f6_3_2_3_36  = 413;
  const static Ulong_t f6_3_2_3_40  = 414;
  const static Ulong_t f6_3_2_3_8   = 415;
  const static Ulong_t f6_3_2_3_9   = 416;
  const static Ulong_t f6_3_2_3_39  = 417;
  const static Ulong_t f6_3_2_3_34  = 418;
  const static Ulong_t f6_3_2_3_13  = 419;
  const static Ulong_t f6_3_2_3_11  = 420;
  const static Ulong_t f20_3_1_2_1  = 421;
  const static Ulong_t f20_3_1_2_2  = 422;
  const static Ulong_t f20_3_1_2_3  = 423;
  const static Ulong_t f20_3_1_2_4  = 424;
  const static Ulong_t f20_3_1_2_5a = 425;
  const static Ulong_t f20_3_1_2_5b = 426;
  const static Ulong_t f20_3_1_2_5c = 427;
  const static Ulong_t f20_3_1_2_5d = 428;
  const static Ulong_t f20_3_1_2_5e = 429;
  const static Ulong_t f20_3_1_2_5f = 430;
  const static Ulong_t f20_3_1_4_1  = 431;
  const static Ulong_t f20_3_1_4_2  = 432;
  const static Ulong_t f20_3_1_4_3  = 433;
  const static Ulong_t f20_3_1_4_4  = 434;
  const static Ulong_t f20_3_1_4_5  = 435;
  const static Ulong_t f20_3_1_4_6  = 436;
  const static Ulong_t f20_3_1_4_7  = 437;
  const static Ulong_t f20_3_1_4_8  = 438;
  const static Ulong_t f20_3_1_4_9  = 439;
  const static Ulong_t f20_3_1_4_10 = 440;
  const static Ulong_t f20_3_1_4_11 = 441;
  const static Ulong_t f20_3_1_4_12 = 442;
  const static Ulong_t f20_3_1_4_13 = 443;
  const static Ulong_t f20_3_1_4_14 = 444;
  const static Ulong_t f20_3_1_4_15 = 445;
  const static Ulong_t f20_3_1_4_16 = 446;
  const static Ulong_t f20_3_1_4_17 = 447;
  const static Ulong_t f20_3_1_4_18 = 448;
  const static Ulong_t f20_3_1_4_19 = 449;
  const static Ulong_t f20_3_1_4_20 = 450;
  const static Ulong_t f20_3_1_4_21 = 451;
  const static Ulong_t f20_3_1_4_22 = 452;
  const static Ulong_t f20_3_1_4_23 = 453;
  const static Ulong_t f20_3_10_2_1 = 454;
  const static Ulong_t f20_3_10_2_2 = 455;
  const static Ulong_t f20_3_10_2_3 = 456;
  const static Ulong_t f20_3_10_2_4 = 457;
  const static Ulong_t f20_3_10_4_1 = 458;
  const static Ulong_t f20_3_10_4_2 = 459;
  const static Ulong_t f20_3_10_4_3 = 460;
  const static Ulong_t f20_3_10_4_4 = 461;
  const static Ulong_t f20_3_10_4_5 = 462;
  const static Ulong_t f20_3_10_4_6 = 463;
  const static Ulong_t f20_3_10_4_7 = 464;
  const static Ulong_t f20_3_10_4_8 = 465;
  const static Ulong_t f20_3_10_4_9 = 466;
  const static Ulong_t f20_3_10_4_10= 467;
  const static Ulong_t f6_3_1_4_114 = 468;
  const static Ulong_t f6_3_1_4_115 = 469;
  const static Ulong_t f6_3_1_4_116 = 470;
  const static Ulong_t f6_3_1_4_117 = 471;
  const static Ulong_t f6_3_2_3_5   = 472;
  const static Ulong_t f6_3_2_3_45  = 473; //Total ACD on Hold Time
  const static Ulong_t f6_3_2_2_12  = 474; //Longest ACD on Hold Time
  const static Ulong_t f6_3_2_2_13  = 475; //Calls on Conference
  const static Ulong_t f6_3_2_2_14  = 476; //Time in Conference
} ERS_2_DBID_t;

const  typedef struct {

 } LIMIT_COLORS_t;


//-----------------------------------------------------
class Limits_tab: public CSA_WEB_LOCK_obj
{
  public:
    // ctor
    Limits_tab(){
      ERS_Limits_tab.clear();
      csa_web_TryLockInfinite(); //MT
      Init_ErsIdLookUpList();
      Init_LimitsByGroups();
      LoadLimits();
      csa_web_UnLock();//MT
    };
    //dstr
    ~Limits_tab()
    {
      UnloadLimitsAndGroups();
    };

    bool LoadLimits(void);
    bool UnloadLimitsAndGroups(void);
    bool IsTreshColorAvailable(void){return 0 ;};
    void Init_LimitsByGroups(void);


    //10-Mar-2019 YR BZ#49198 - Change Ulong_t to Ulong_t
    bool GetColorOfSTATE(Ulong_t   group_id,
                         Ulong_t   sca_state,
                         Ulong_t   db_ers_id,
                         Ulong_t   value,
                         Ulong_t** bg_color);

    //10-Mar-2019 YR BZ#49198 - Change Ulong_t to Ulong_t
    bool GetColorOfERS( Ulong_t   group_id,
                        Ulong_t   db_ers_id,
                        Ulong_t   value,
                        Ulong_t** bg_color);

    //10-Mar-2019 YR BZ#49198 - Change Ulong_t to Ulong_t
    void VerifyGroupInLimitTab(Ulong_t group_id);//PVL 28-07-2009 1002567

    void ReloadAll(void); //PVL 29-07-2009 1002487

    ERS_2_DBID_t  ersid_val;
    // Group_Limits_t ersIdMatrix[MAX_GROUP_NUMBER];
    typedef enum {
      None = 0,
      Up   = 1,
      Down = 2
    }LIMIT_TYPE_e;

    LIMIT_TYPE_e LIMIT_TYPE;

    typedef Ulong_t Group_id_t;

    typedef struct DbiColorInfo_s
    {
      string         ers_name;
      Ulong_t   db_ers_id;
      int            red_limit;
      int            yellow_limit;
      LIMIT_TYPE_e   lim_type;
      bool           inUse;
    }Group_Limits_t ;

    typedef  struct STATE_Limit_Vector_str{
      Group_Limits_t  state_vector[CSA_LAST_STATE];
    }STATE_Limit_Vector_t;

    typedef struct ERS_Limit_Vector_str{
      Group_Limits_t  ers_vector[MAX_N_OF_LIMS];
    }ERS_Limit_Vector_t;


  private:

    typedef struct DbiErsIdInfo_s
    {
      string         ers_name;
      Ulong_t   db_ers_id;
      LIMIT_TYPE_e   lim_type;
    }ers_id_tab_t ;

    ers_id_tab_t ersIdList[480];

    map <Group_id_t,STATE_Limit_Vector_t*> STATE_Limits_tab;
    map <Group_id_t,STATE_Limit_Vector_t*>::iterator STATE_Limits_it;

    map<Group_id_t,ERS_Limit_Vector_t*>   ERS_Limits_tab;
    map<Group_id_t,ERS_Limit_Vector_t*>::iterator ERS_Limits_it;

    void Init_ErsIdLookUpList(void);

    STATE_Limit_Vector_t* GetStateVector(Ulong_t group_id);
    ERS_Limit_Vector_t* GetERSVector(Ulong_t group_id);

};
#endif
