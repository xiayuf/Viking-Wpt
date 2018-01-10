//==============================================================================
// gateways.h
//==============================================================================


//==============================================================================
#ifndef GATEWAYS_H
#define GATEWAYS_H
//==============================================================================


//==============================================================================
int HUD_Init(void);
void HUD_Redraw(float x, int y);
void PreV_CalcRefdef(struct ref_params_s *pparams);
void PostV_CalcRefdef(struct ref_params_s *pparams);
void CL_CreateMove(float frametime, struct usercmd_s *cmd, int active);
void HUD_PlayerMove (struct playermove_s *a, int b);
void HUD_PostRunCmd(struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed);
void HUD_TempEntUpdate(double* pframetime, double* pclient_time, double* pcl_gravity, TEMPENTITY*** pppTempEntFree, TEMPENTITY*** pppTempEntActive, int( **pCallback_AddVisibleEntity )( cl_entity_t *pEntity ), void( **pCallback_TempEntPlaySound )( TEMPENTITY *pTemp, float damp ) );
void HUD_AddEntity(int type, struct cl_entity_s *ent, const char *modelname);
void HUD_Key_Event(int eventcode, int keynum, const char *pszCurrentBinding);
void HUD_UpdateClientData(client_data_t *cdata, float flTime);
//==============================================================================


//==============================================================================
#endif
//==============================================================================