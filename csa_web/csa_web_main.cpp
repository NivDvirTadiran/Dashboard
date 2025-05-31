#include "er_std_override.h"

#include <errno.h>
#ifdef WIN32
#include "winsock2.h"
#include "windows/soapEMISwebBindingService.h"
#include "windows/EMISwebBinding.nsmap"
#else
#include "linux/soapEMISwebBindingService.h"
#include "linux/EMISwebBinding.nsmap"
#endif

#include <bswgen.h>
#include <bswinifile.h>
#include "csa_web_main.h"
#include "csa_web_serverMT.hpp"
#include "csa_web_thmanager.hpp"

extern "C"
{
  #include <ha_defs.h>
}

#define SEC_WEB_REALTIME    "WEB-REALTIME"
#define VAL_PORT            "Port"
#define VAL_PORT_DEFAULT	9071

//==========================[ Static Variables ]======================
static int        emis_binding_port = 9071;
static bool       first_init = false;
static ThMngr_c&  ThreadManager = ThMngr_c::GetThMngr();
static BswLog_c   csa_web_main_log_d(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_DETAILED);
static BswLog_c   csa_web_main_log_i(BswMakeLogSite(FC_CSA_WEB, 1),LM_INTER_CSCI, LOGL_INFO);



//HTTP GET
int http_get(struct soap *soap)
{
  csa_web_main_log_i << "http_get Invoke - ";

  // Tomcat servlet sends HTTP GET -reguest for the wsdl file.
  FILE *fd;
  int r;
  char *s = strchr(soap->path, '?');

  if (!s || strcmp(s, "?wsdl"))
  {
    csa_web_main_log_i << "return SOAP_GET_METHOD" << LOG_TERMINATOR;
    return SOAP_GET_METHOD;
  }

  fd = er_fopen("emisweb.wsdl", "rb"); // open WSDL file to copy
  if (!fd)
  {
    csa_web_main_log_i << " open emisweb.wsdl fails (error " << errno << ") return 404" << LOG_TERMINATOR;
    return 404; // return HTTP not found error
  }
  soap->http_content = "text/xml"; // HTTP header with text/xml content

  soap_response(soap, SOAP_FILE);
  for (;;)
  {
    r = fread(soap->tmpbuf, 1, sizeof(soap->tmpbuf), fd);
    if (!r)
      break;
    if (soap_send_raw(soap, soap->tmpbuf, r))
      break; // can't send, but little we can do about that
  }

  fclose(fd);
  soap_end_send(soap);

  csa_web_main_log_i << "return SOAP_OK" << LOG_TERMINATOR;

  return SOAP_OK;
}
//------------------------------------------------------------------------------------------------

#ifdef PVL_MEM_DEBUG_
	#include <crtdbg.h>//----------------------------------DEBUG
#endif


CsaWebServer wEMISServiceInterface;

int CSA_WEB_Init(void)
{
  csa_web_main_log_i << "CSA_WEB_Init Invoke\n" << LOG_TERMINATOR;

  //24-Jan-2019 YR BZ#48668
  if(!first_init)
  {
    BswIniFile_c iniFile("../Registry.ini");
    emis_binding_port = iniFile.GetInt(SEC_WEB_REALTIME, VAL_PORT, VAL_PORT_DEFAULT);
    first_init = true;
  }

  SOCKET  m = -1;

  //5-Nov-2018 YR BZ#48254
  wEMISServiceInterface.EMISwebBindingService_init(SOAP_IO_KEEPALIVE, SOAP_IO_KEEPALIVE);
  //wEMISServiceInterface.EMISwebBindingService_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
  ThreadManager.Init();
  wEMISServiceInterface.fget = http_get;
  wEMISServiceInterface.bind_flags |= SO_REUSEADDR;

  m = wEMISServiceInterface.bind(NULL, emis_binding_port, 100);

  if (m < 0)
  {
    wEMISServiceInterface.soap_print_fault(stderr);
    csa_web_main_log_i << "CSA_WEB_Init -> errno = [" << errno << "] " << strerror(errno) << " - retry after reset \n" << LOG_TERMINATOR;
    FILE * fp = er_fopen ("emis_web.txt", "w+");
    fprintf(fp, "******* CSA_WEB_Init -> errno = [%d] %s - retry after reset\n", errno,strerror(errno));
    fclose(fp);
    er_fprintf(stderr,"CSA_WEB_Init -> errno = [%d] %s - retry after reset\n",errno,strerror(errno));

    //4-Nov-2018 YR BZ#48254
    wEMISServiceInterface.reset();

#ifndef _WIN32
    int count = 0;
    int sleeptime = 8;
    for (count; count < 10; ++count)
    {
      sleep(sleeptime);
      m = wEMISServiceInterface.bind(NULL, emis_binding_port, 100);
      if (m >= 0)
      {
        FILE * fp = er_fopen ("emis_web.txt", "w+");
        fprintf(fp, "******* CSA_WEB_Init->Socket connection successful after [%d] retries: master socket = %d\n", count+1, m);
        fclose(fp);
        er_fprintf(stderr, "CSA_WEB_Init->Socket connection successful after [%d] retries: master socket = %d\n", count+1, m);
        return 0;
      }
      er_fprintf(stderr,"++++ [%d], CSA_WEB_Init -> retry [%d]   errno = [%d] %s ++++\n",time(0),count + 1,errno,strerror(errno));
    }

#endif
    return 1;//error
  }
  else
  {
    er_fprintf(stderr, "CSA_WEB_InitSocket connection successful: master socket = %d\n", m);
    return 0;
  }
}


int CSA_WEB_mainLoop(void)
{
  csa_web_main_log_d << "CSA_WEB_mainLoop Invoke\n" << LOG_TERMINATOR;

  SOCKET  s; // operation socket
  CsaWebServer *wEMISServiceInterface_COPY;

  s = wEMISServiceInterface.accept();
  if (s < 0)
  {
    wEMISServiceInterface.soap_print_fault(stderr);
    return 1;
  }

  // Copy soap object
  wEMISServiceInterface_COPY = (CsaWebServer*)wEMISServiceInterface.copy();
  if (!wEMISServiceInterface_COPY){
    return 1;     
  }

  // Send soap object to Available Thread
  ThreadManager.ProcessSoapConnection(wEMISServiceInterface_COPY);
  return 0;
}


/*-----------------------[ IsHAActive ]--------------------------------*/
bool  IsHAActive(void)
{
  return ((EosHAGetLocalStatus(__FILE__, __LINE__) == EOS_HAS_ACTIVE_SA) || (EosHAGetLocalStatus(__FILE__, __LINE__) == EOS_HAS_ACTIVE));
}

/*-----------------------[ IsHAPassive ]-------------------------------*/
bool  IsHAPassive(void)
{
  return (EosHAGetLocalStatus(__FILE__, __LINE__) == EOS_HAS_PASSIVE);
}

/*-----------------------[ IsHAIsland ]--------------------------------*/
bool  IsHAIsland(void)
{
  return ((EosHAGetLocalStatus(__FILE__, __LINE__) == EOS_HAS_ACTIVE) && (EosHAGetOtherStatus(__FILE__, __LINE__) == EOS_HAS_UNKNOWN));
}


