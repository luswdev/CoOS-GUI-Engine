/**
 *******************************************************************************
 * @file       topwin.h
 * @version    V0.0.1   
 * @date       2019.5.24
 * @brief      The top window management for gui engine.	
 *******************************************************************************
 */ 

#ifndef _COGUI_TOPWIN_H
#define _COGUI_TOPWIN_H

/*---------------------------- Function Define -------------------------------*/
cogui_topwin_t *GetTopWin(S32 x, S32 y);
cogui_topwin_t *GetTopWinFocus(void);

#endif /* _COGUI_TOPWIN_H */