/*********************************************************************************
*                            Function Definition                                  
*    Function    :                                                                
*    Parameter   :                                                              
*    Return      :                                                               
*                          ***2018/07/15-19:36:11  By Nick***               
*********************************************************************************/

#ifndef _DATASAVE_H_
#define _DATASAVE_H_

#include "stm32f4xx.h"

#pragma pack(push)
#pragma pack(1)	//°´×Ö½Ú¶ÔÆë

typedef struct __St_DataSave
{
    
    float  CH0_Adi_Senser_Cal   ;
    float  CH1_Adi_Senser_Cal   ;
    float  CH2_Adi_Senser_Cal   ;
    float  CH3_Adi_Senser_Cal   ;
    
    
}_St_DataSave;


#pragma pack(pop)

extern  _St_DataSave St_DataSave;

#define   FLASH_MAX_SIZE     127
#define   FLASH_START_ADDR   0


extern int  St_Save_To_Flash( void )  ;
extern int  St_Read_From_Flash( void );

#endif

