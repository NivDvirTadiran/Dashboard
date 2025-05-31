#  CSA_WEB Makefile
ERSRC = $(ACC_MAIN)
OBJDIR = ./.objdir

CCSRC = \
./linux/stdsoap2.cpp \
./linux/soapEMISwebBindingService.cpp \
./linux/soapC.cpp \
csa_web_trace.cpp \
csa_web_lock.cpp \
csa_web_ticket.cpp \
csa_web_thmanager.cpp \
csa_web_srv_th_interface.cpp \
csa_web_sm_syn.cpp \
csa_web_serverMT.cpp \
csa_web_server_net_data.cpp \
csa_web_server_db_data.cpp \
csa_web_report.cpp \
csa_web_online_srv_th.cpp \
csa_web_main.cpp \
csa_web_mailbox_reply.cpp \
csa_web_mailbox.cpp \
csa_web_sg_reply.cpp \
csa_web_sg.cpp \
csa_web_limits.cpp \
csa_web_ivrport_reply.cpp \
csa_web_ivrport.cpp \
csa_web_ivrgrp_reply.cpp \
csa_web_ivrapp.cpp \
csa_web_ivr_reply.cpp \
csa_web_init.cpp \
csa_web_groups.cpp \
csa_web_group_reply.cpp \
csa_web_dnis_reply.cpp \
csa_web_dnis.cpp \
csa_web_data_interface.cpp \
csa_web_commondefs.cpp \
csa_web_brief_agent_inf.cpp \
csa_web_agents.cpp \
csa_web_agent_reply.cpp \
csa_web_sg_hourly_reply.cpp \
csa_web_group_hourly_reply.cpp


#
#CCOBJS = $(CCOBJ1) ${cppSOURCE:.cpp=.o}
#
CCOBJS = \
./linux/stdsoap2.o \
./linux/soapEMISwebBindingService.o \
./linux/soapC.o \
csa_web_trace.o \
csa_web_lock.o \
csa_web_ticket.o \
csa_web_thmanager.o \
csa_web_srv_th_interface.o \
csa_web_sm_syn.o \
csa_web_serverMT.o \
csa_web_server_net_data.o \
csa_web_server_db_data.o \
csa_web_report.o \
csa_web_online_srv_th.o \
csa_web_main.o \
csa_web_mailbox_reply.o \
csa_web_mailbox.o \
csa_web_sg_reply.o \
csa_web_sg.o \
csa_web_limits.o \
csa_web_ivrport_reply.o \
csa_web_ivrport.o \
csa_web_ivrgrp_reply.o \
csa_web_ivrapp.o \
csa_web_ivr_reply.o \
csa_web_init.o \
csa_web_groups.o \
csa_web_group_reply.o \
csa_web_dnis_reply.o \
csa_web_dnis.o \
csa_web_data_interface.o \
csa_web_commondefs.o \
csa_web_brief_agent_inf.o \
csa_web_agents.o \
csa_web_agent_reply.o \
csa_web_sg_hourly_reply.o \
csa_web_group_hourly_reply.o


#
# Sol remove it it do nothig ./api/bswdllmain.o \

include $(ERSRC)/MakeUnix/MakeBase
# MOREINCLUDES = -I $(ERSRC)/source/gen/timezone \
				 -I $(ERSRC)/source/emis/csa_web/linux
MORELIBS = -l eosevexlator -l r_con_mon_client -l con_alarms -l csa_res -l gen_utils



TARGET =  $(LIB_DIR)/libcsa_web.so

all :  $(TARGET)
$(TARGET) : $(CCOBJS)
	rm -f $(TARGET)
	$(SOLD)  -o $(TARGET) $(MORELIBS) $(CCOBJS) $(EasyRunLib) $(LINUX_LIBS) 

clean:
	/bin/rm -rf $(OBJ_DIR)  $(TARGET) $(CCOBJS)
make.depend:
	rm -f make.depend
	$(CXX) -MM $(CCSRC)  $(MOREINCLUDES) > make.depend

depend:
	$(CXX) -MM $(CCSRC)  $(MOREINCLUDES)  > make.depend

$(CCOBJS):
	@echo "---------------------------------------------------------------------------------"
	@echo $@
	@echo "---------------------------------------------------------------------------------"
	$(CXX) -o $@  $(MOREINCLUDES) -D WITH_PURE_VIRTUAL -fpermissive  ${@:.o=.cpp}

# DO NOT DELETE
sinclude ./make.depend
