#include "er_std_override.h"
#include "csa_web_srv_th_interface.h"

InteractWithManager_c::InteractWithManager_c(Ushort_t          index,
                                             WebNlaServeThread_c* thread) : 
m_index(index), 
m_status(InteractWithManager_c::FINISHED),
m_thread(thread)
{
 
}

InteractWithManager_c::~InteractWithManager_c()
{

}
