/*********************************************************************************
*                            Function Definition                                  
*    Function    :                                                                
*    Parameter   :                                                              
*    Return      :                                                               
*                          ***2018/07/15-19:35:05  By Nick***               
*********************************************************************************/
#include "DataSave.h"
#include "24cxx.h" 
#include "stdio.h" 


_St_DataSave St_DataSave = 
{ 
    0  ,
    
};



int  St_Save_To_Flash( void )
{
    u8    * ipos = NULL;
    u16     temp = 0;
    
    ipos   =  (u8 *)&St_DataSave;
    
    if(NULL == ipos)   return -1;
    
    temp  =  sizeof(_St_DataSave) ;
    if(FLASH_MAX_SIZE < temp)  return -2;
    
    AT24CXX_Write(FLASH_START_ADDR,ipos,temp);
    
    return 0; 
}


int  St_Read_From_Flash( void )
{
    u8    * ipos = NULL;
    u16     temp = 0;
    
    ipos   =  (u8 *)&St_DataSave;
    
    if(NULL == ipos)   return -1;
    
    temp  =  sizeof(_St_DataSave) ;
    if(FLASH_MAX_SIZE < temp)  return -2;
    
    AT24CXX_Read(FLASH_START_ADDR,ipos,temp);
    
    return 0;
}


