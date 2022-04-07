#include "FDC2214.h"
#include "delay.h"

/*
使用方法:(需要配合myiic.c文件)
FDC2214_Init();//初始化
float value;//定义一个值
value=FDC_Result(CH0);//将测到的电容值保存在value
*/
st_data_typedef          st_data_structure       = {0};
st_status_typedef        st_status_structure     = {0};
st_freq_cap_typedef      st_freq_cap_structure   = {0};

/******define  device init parameter ********/
st_fdc_init_typedef  st_fdc_init_structure =
{
    .Channel_Mode                 = Channel_Mode_Mutio ,
    .Channel_Single_Option        = Channel_Option_Ch0,//Channel_Option_Single_typedef
    .Channel_Muti_Option          = Channel_Option_Ch0_1_2_3,//Channel_Option_Muti_typedef
    
    /***config CLK   *****/
    .Clk_Src                      = Clk_Src_Externa ,//Clk_Src_typedef  40MHz
    
    .Clk_Fref_Divider_Ch0         = 1,//between 0 and 511  //21151506 27394466
    .Clk_Fref_Divider_Ch1         = 1,
    .Clk_Fref_Divider_Ch2         = 1,
    .Clk_Fref_Divider_Ch3         = 1,
    
    .Clk_Fin_Sel_Ch0              = Clk_FirstMode,//Clk_Fin_Sel_typedef
    .Clk_Fin_Sel_Ch1              = Clk_FirstMode,
    .Clk_Fin_Sel_Ch2              = Clk_FirstMode,
    .Clk_Fin_Sel_Ch3              = Clk_FirstMode,
    
    /*** config time ****/
    .Time_SettleCount_Ch0         = 0x0400,//between 0 and 65535
    .Time_SettleCount_Ch1         = 0x0400,
    .Time_SettleCount_Ch2         = 0x0400,
    .Time_SettleCount_Ch3         = 0x0400,
    
    .Time_RCount_Ch0              = 0xffff,//between 0 and 65535
    .Time_RCount_Ch1              = 0xffff,
    .Time_RCount_Ch2              = 0xffff,
    .Time_RCount_Ch3              = 0xffff,   
    
    /*** config current ***********/
    .Current_Activation_Mode     = Current_Activation_Mode_Low_Power,//Current_Activation_Mode_typedef
    
    .Current_High_Mode           = Current_High_Mode_Normal,//Current_High_Mode_typedef
    
    .Current_Driver_Ch0          = Current_1_354,//Current_Driver_typedef
    .Current_Driver_Ch1          = Current_1_354,
    .Current_Driver_Ch2          = Current_1_354,
    .Current_Driver_Ch3          = Current_1_354,
    
    /*** config Filter**************/
    .Filter_Input_Deglitch       = Filter_Input_Deglitch_10MHz,//Filter_Input_Deglitch_typedef 
    
    /**** config ERROR_CONFIG****************/
    .Wd_Err2out                  =  Error_Config_Disable_Watchdog,//Error_Config_typedef
    .Ah_Warn2out                 =  Error_Config_Disable_Amp_High,
    .Al_Warn2out                 =  Error_Config_Disable_Amp_Low,
    .Wd_Err2int                  =  Error_Config_Disable_Int_Wachdog,
    .Drdy_2int                   =  Error_Config_Enable_Int_DataRead,
};

/******************To save register parameter of device init**********************************/
st_register_typedef st_register_structure  = 
{
    .DATA_MSB_CH0          = 0x0000 ,
    .DATA_LSB_CH0          = 0x0000 ,
    .DATA_MSB_CH1          = 0x0000 ,
    .DATA_LSB_CH1          = 0x0000 ,
    .DATA_MSB_CH2          = 0x0000 ,
    .DATA_LSB_CH2          = 0x0000 ,
    .DATA_MSB_CH3          = 0x0000 ,
    .DATA_LSB_CH3          = 0x0000 ,
    
    .RCOUNT_CH0            = 0x0080 ,
    .RCOUNT_CH1            = 0x0080 ,
    .RCOUNT_CH2            = 0x0080 ,
    .RCOUNT_CH3            = 0x0080 ,
    
    .SETTLECOUNT_CH0       = 0x0000 ,
    .SETTLECOUNT_CH1       = 0x0000 ,
    .SETTLECOUNT_CH2       = 0x0000 ,
    .SETTLECOUNT_CH3       = 0x0000 ,
    
    .CLOCK_DIVIDERS_CH0    = 0x0000 ,
    .CLOCK_DIVIDERS_CH1    = 0x0000 ,
    .CLOCK_DIVIDERS_CH2    = 0x0000 ,
    .CLOCK_DIVIDERS_CH3    = 0x0000 ,
    
    .STATUS                = 0x0000 ,
    .ERROR_CONFIG          = 0x0000 ,
    .CONFIG                = 0x2801 ,
    .MUX_CONFIG            = 0x020f ,
    .RESET_DEV             = 0x0000 ,
    
    .DRIVE_CURRENT_CH0     = 0x0000 ,
    .DRIVE_CURRENT_CH1     = 0x0000 ,
    .DRIVE_CURRENT_CH2     = 0x0000 ,
    .DRIVE_CURRENT_CH3     = 0x0000 ,
    
    .MANUFACTURER_ID       = 0x5449 ,
    .DEVICE_ID             = 0x3055 ,
};

static fdc_u16 FDC_Read_Register(fdc_u8 addr)
{
    fdc_u16 temp = 0;
    
    fdc_iic_start();
    
    fdc_iic_send_byte(FDC_I2C_WADDR);
    fdc_iic_wait_ack();
    
    fdc_iic_send_byte(addr);
    fdc_iic_wait_ack();
    
    fdc_iic_start();
    
    fdc_iic_send_byte(FDC_I2C_RADDR);
    fdc_iic_wait_ack();
    
    temp = (fdc_iic_read_byte(1)) << 8;//ACK
    
    temp = (temp | fdc_iic_read_byte(0));//NACK
    
    fdc_iic_stop();
    
    return temp;
}

static fdc_u16 FDC_Write_Register(fdc_u8 addr,fdc_u16 value)
{
    fdc_u8  temp = 0;
    
    fdc_iic_start();
    
    fdc_iic_send_byte(FDC_I2C_WADDR);
    fdc_iic_wait_ack();
    
    fdc_iic_send_byte(addr);
    fdc_iic_wait_ack();
    
    temp = value>>8;
    fdc_iic_send_byte(temp);
    fdc_iic_wait_ack();
    
    temp = value & 0x00ff;
    fdc_iic_send_byte(temp);
    fdc_iic_wait_ack();
    
    fdc_iic_stop();
    
    return value;
}

int FDC_Cmd_SleepMode(fdc_u8 status)
{
    fdc_u16  temp = 0;
    temp = FDC_Read_Register(FDC_CONFIG);
    
    if(!status) 
    {
        temp = temp & (~FDC_SLEEP_MODE_EN);  //[13] = 0;
    }
    else if(status)
    {
        temp = temp | FDC_SLEEP_MODE_EN;     //[13] = 1;
    }
    
    FDC_Write_Register(FDC_CONFIG,temp);  
    return 0;   
}

int FDC_Cmd_Int(fdc_u8 status)
{
    fdc_u16  temp = 0;
    temp = FDC_Read_Register(FDC_CONFIG);
    
    if(!status) 
    {
        temp = temp | FDC_INTB_DIS;     //[7] = 1;
    }
    else if(status)
    {
        temp = temp & (~FDC_INTB_DIS);  //[7] = 0;
    }
    
    FDC_Write_Register(FDC_CONFIG,temp);   
    return 0;   
}

int FDC_Check_Exist(void)
{
    if(0x5449 == FDC_Read_Register(FDC_MANUFACTURER_ID) && \
       0x3055 == FDC_Read_Register(FDC_DEVICE_ID) )
    {
        return 0;
    }
    else 
    {
        return -1;
    }
}

int FDC_Reset(void)
{
    FDC_Write_Register(FDC_RESET_DEV,0x8000);
    return 0;
}

int FDC_Init(st_fdc_init_typedef * fdc_init_structure,st_register_typedef *st_register_structure)
{
    FDC_IIC_INIT();
    FDC_SET_ADDR();
    FDC_SET_SDPIN();  
    FDC_Cmd_SleepMode(1);  //进入睡眠模式更改配置才有效 
    st_register_structure->RCOUNT_CH0 = FDC_Write_Register(FDC_RCOUNT_CH0,fdc_init_structure->Time_RCount_Ch0);
    st_register_structure->RCOUNT_CH1 = FDC_Write_Register(FDC_RCOUNT_CH1,fdc_init_structure->Time_RCount_Ch1);
    st_register_structure->RCOUNT_CH2 = FDC_Write_Register(FDC_RCOUNT_CH2,fdc_init_structure->Time_RCount_Ch2);
    st_register_structure->RCOUNT_CH3 = FDC_Write_Register(FDC_RCOUNT_CH3,fdc_init_structure->Time_RCount_Ch3);
    
    st_register_structure->SETTLECOUNT_CH0 = FDC_Write_Register(FDC_SETTLECOUNT_CH0,fdc_init_structure->Time_SettleCount_Ch0);
    st_register_structure->SETTLECOUNT_CH1 = FDC_Write_Register(FDC_SETTLECOUNT_CH1,fdc_init_structure->Time_SettleCount_Ch1);
    st_register_structure->SETTLECOUNT_CH2 = FDC_Write_Register(FDC_SETTLECOUNT_CH2,fdc_init_structure->Time_SettleCount_Ch2);
    st_register_structure->SETTLECOUNT_CH3 = FDC_Write_Register(FDC_SETTLECOUNT_CH3,fdc_init_structure->Time_SettleCount_Ch3);
    
    if( !(IS_Clk_Fin_Sel_typedef(fdc_init_structure->Clk_Fin_Sel_Ch0) && \
          IS_Clk_Fin_Sel_typedef(fdc_init_structure->Clk_Fin_Sel_Ch1) && \
          IS_Clk_Fin_Sel_typedef(fdc_init_structure->Clk_Fin_Sel_Ch2) && \
          IS_Clk_Fin_Sel_typedef(fdc_init_structure->Clk_Fin_Sel_Ch3) ))
    {
        return -1;
    }
    
    st_register_structure->CLOCK_DIVIDERS_CH0 = FDC_Write_Register(FDC_CLOCK_DIVIDERS_CH0,fdc_init_structure->Clk_Fin_Sel_Ch0 | \
                                                                  fdc_init_structure->Clk_Fref_Divider_Ch0);
    st_register_structure->CLOCK_DIVIDERS_CH1 = FDC_Write_Register(FDC_CLOCK_DIVIDERS_CH1,fdc_init_structure->Clk_Fin_Sel_Ch1 | \
                                                                  fdc_init_structure->Clk_Fref_Divider_Ch1);
    st_register_structure->CLOCK_DIVIDERS_CH2 = FDC_Write_Register(FDC_CLOCK_DIVIDERS_CH2,fdc_init_structure->Clk_Fin_Sel_Ch2 | \
                                                                  fdc_init_structure->Clk_Fref_Divider_Ch2);
    st_register_structure->CLOCK_DIVIDERS_CH3 = FDC_Write_Register(FDC_CLOCK_DIVIDERS_CH3,fdc_init_structure->Clk_Fin_Sel_Ch3 | \
                                                                  fdc_init_structure->Clk_Fref_Divider_Ch3);
    
    if( !(IS_Error_Config_typedef(fdc_init_structure->Wd_Err2out) && \
          IS_Error_Config_typedef(fdc_init_structure->Ah_Warn2out) && \
          IS_Error_Config_typedef(fdc_init_structure->Al_Warn2out) && \
          IS_Error_Config_typedef(fdc_init_structure->Wd_Err2int) && \
          IS_Error_Config_typedef(fdc_init_structure->Drdy_2int) ))
    {
        return -2;
    }
    
    st_register_structure->ERROR_CONFIG = FDC_Write_Register(FDC_STATUS_CONFIG,fdc_init_structure->Wd_Err2out  | \
                                                             fdc_init_structure->Ah_Warn2out | \
                                                             fdc_init_structure->Al_Warn2out | \
                                                             fdc_init_structure->Wd_Err2int  | \
                                                             fdc_init_structure->Drdy_2int);
    
    if( !IS_Channel_Option_Single_typedef(fdc_init_structure->Channel_Single_Option) )
    {
        return -3;
    }
    if( !IS_Current_Activation_Mode_typedef(fdc_init_structure->Current_Activation_Mode) )
    {
        return -4;
    }
    if( !IS_Current_High_Mode_typedef(fdc_init_structure->Current_High_Mode) )
    {
        return -5;
    }
    if( !IS_Clk_Src_typedef(fdc_init_structure->Clk_Src) )
    {
        return -6;
    }
    
    st_register_structure->CONFIG = FDC_Write_Register(FDC_CONFIG,fdc_init_structure->Channel_Single_Option | \
                                                      fdc_init_structure->Current_Activation_Mode | \
                                                      fdc_init_structure->Current_High_Mode  | \
                                                      fdc_init_structure->Clk_Src   |  \
                                                      0x2000 /*| 0x0080*/ | 0x1401 /*Sleep mode and disable intb and [12] [10] [0] set to 1*/);
    
    if( !IS_Channel_Mode_typedef(fdc_init_structure->Channel_Mode) )
    {
        return -7;
    }
    if( !IS_Channel_Option_Muti_typedef(fdc_init_structure->Channel_Muti_Option) )
    {
        return -8;
    }
    if( !IS_Filter_Input_Deglitch_typedef(fdc_init_structure->Filter_Input_Deglitch)  )
    {
        return -9;
    }
    
    st_register_structure->MUX_CONFIG =  FDC_Write_Register(FDC_MUX_CONFIG,fdc_init_structure->Channel_Mode | \
                                                          fdc_init_structure->Channel_Muti_Option   |   \
                                                          fdc_init_structure->Filter_Input_Deglitch |   \
                                                          0x0208  /*  [12:3] must be set to 0 0010 0000 1 */);
        
    if( !(IS_Current_Driver_typedef(fdc_init_structure->Current_Driver_Ch0) && \
          IS_Current_Driver_typedef(fdc_init_structure->Current_Driver_Ch1) && \
          IS_Current_Driver_typedef(fdc_init_structure->Current_Driver_Ch2) && \
          IS_Current_Driver_typedef(fdc_init_structure->Current_Driver_Ch3) ))
    {
        return -10;
    }
    
    st_register_structure->DRIVE_CURRENT_CH0 =  FDC_Write_Register(FDC_DRIVE_CURRENT_CH0,fdc_init_structure->Current_Driver_Ch0);
    st_register_structure->DRIVE_CURRENT_CH1 =  FDC_Write_Register(FDC_DRIVE_CURRENT_CH1,fdc_init_structure->Current_Driver_Ch1);
    st_register_structure->DRIVE_CURRENT_CH2 =  FDC_Write_Register(FDC_DRIVE_CURRENT_CH2,fdc_init_structure->Current_Driver_Ch2);
    st_register_structure->DRIVE_CURRENT_CH3 =  FDC_Write_Register(FDC_DRIVE_CURRENT_CH3,fdc_init_structure->Current_Driver_Ch3);
    
    return 0;
}

int FDC_Get_Data(st_data_typedef * st_data_structure,Channeln_typedef fdc_chn)
{
    fdc_u16 temp_lsb = 0,temp_msb  = 0;
    
    if(fdc_chn & 0x01)
    {
        temp_lsb = temp_msb = 0;
        temp_msb = FDC_Read_Register( FDC_DATA_MSB_CH0 );
        temp_lsb = FDC_Read_Register( FDC_DATA_LSB_CH0 );
        st_data_structure->Flag_WD0 = ( temp_msb & FDC_CHn_ERR_WD  ) >> 13;
        st_data_structure->Flag_AW0 = ( temp_msb & FDC_CHn_ERR_AW  ) >> 12;
        
        st_data_structure->Data_Ch0 = ((temp_msb & FDC_MSB_DATAn)<<16) | temp_lsb ;
    }
    
    if(fdc_chn & 0x02)
    {
        temp_lsb = temp_msb = 0;
        temp_msb = FDC_Read_Register( FDC_DATA_MSB_CH1 );
        temp_lsb = FDC_Read_Register( FDC_DATA_LSB_CH1 );
        st_data_structure->Flag_WD1 = ( temp_msb & FDC_CHn_ERR_WD  ) >> 13;
        st_data_structure->Flag_AW1 = ( temp_msb & FDC_CHn_ERR_AW  ) >> 12;
        
        st_data_structure->Data_Ch1 = ((temp_msb & FDC_MSB_DATAn)<<16) | temp_lsb ;
    }

    if(fdc_chn & 0x04)
    {
        temp_lsb = temp_msb = 0;
        temp_msb = FDC_Read_Register( FDC_DATA_MSB_CH2 );
        temp_lsb = FDC_Read_Register( FDC_DATA_LSB_CH2 );
        st_data_structure->Flag_WD2 = ( temp_msb & FDC_CHn_ERR_WD  ) >> 13;
        st_data_structure->Flag_AW2 = ( temp_msb & FDC_CHn_ERR_AW  ) >> 12;
        
        st_data_structure->Data_Ch2 = ((temp_msb & FDC_MSB_DATAn)<<16) | temp_lsb ;
    }

    if(fdc_chn & 0x08)
    {    
        temp_lsb = temp_msb = 0;
        temp_msb = FDC_Read_Register( FDC_DATA_MSB_CH3 );
        temp_lsb = FDC_Read_Register( FDC_DATA_LSB_CH3 );
        st_data_structure->Flag_WD3 = ( temp_msb & FDC_CHn_ERR_WD  ) >> 13;
        st_data_structure->Flag_AW3 = ( temp_msb & FDC_CHn_ERR_AW  ) >> 12;
        
        st_data_structure->Data_Ch3 = ((temp_msb & FDC_MSB_DATAn)<<16) | temp_lsb ;
    }    
    return 0;
}

int FDC_Get_Status(st_status_typedef * st_status_structure)
{
    fdc_u16 temp  = 0;
    
    temp = FDC_Read_Register( FDC_STATUS );
    st_status_structure->STATUS_ERR_CHAN       = (temp & FDC_ERR_CHAN  ) >> 14;
    st_status_structure->STATUS_ERR_WD         = (temp & FDC_ERR_WD    ) >> 11;
    st_status_structure->STATUS_ERR_AHW        = (temp & FDC_ERR_AHW   ) >> 10;
    st_status_structure->STATUS_ERR_ALW        = (temp & FDC_ERR_ALW   ) >> 9;
    st_status_structure->STATUS_DRDY           = (temp & FDC_DRDY      ) >> 6;
    st_status_structure->STATUS_CH0_UNREADCONV = (temp & FDC_CH0_UNREADCONV ) >> 3;
    st_status_structure->STATUS_CH1_UNREADCONV = (temp & FDC_CH1_UNREADCONV ) >> 2;
    st_status_structure->STATUS_CH2_UNREADCONV = (temp & FDC_CH2_UNREADCONV ) >> 1;
    st_status_structure->STATUS_CH3_UNREADCONV = (temp & FDC_CH3_UNREADCONV ) >> 0;
    
    return 0;
}

int FDC_Reflash_RegisterPara(st_register_typedef * st_register_structure)
{
    st_register_structure->DATA_MSB_CH0 = FDC_Read_Register(FDC_DATA_MSB_CH0);
    st_register_structure->DATA_LSB_CH0 = FDC_Read_Register(FDC_DATA_LSB_CH0);
    st_register_structure->DATA_MSB_CH1 = FDC_Read_Register(FDC_DATA_MSB_CH1);
    st_register_structure->DATA_LSB_CH1 = FDC_Read_Register(FDC_DATA_LSB_CH1);
    st_register_structure->DATA_MSB_CH2 = FDC_Read_Register(FDC_DATA_MSB_CH2);
    st_register_structure->DATA_LSB_CH2 = FDC_Read_Register(FDC_DATA_LSB_CH2);
    st_register_structure->DATA_MSB_CH3 = FDC_Read_Register(FDC_DATA_MSB_CH3);
    st_register_structure->DATA_LSB_CH3 = FDC_Read_Register(FDC_DATA_LSB_CH3);
    
    st_register_structure->RCOUNT_CH0   = FDC_Read_Register(FDC_RCOUNT_CH0);
    st_register_structure->RCOUNT_CH1   = FDC_Read_Register(FDC_RCOUNT_CH1);
    st_register_structure->RCOUNT_CH2   = FDC_Read_Register(FDC_RCOUNT_CH2);
    st_register_structure->RCOUNT_CH3   = FDC_Read_Register(FDC_RCOUNT_CH3);

    st_register_structure->SETTLECOUNT_CH0 = FDC_Read_Register(FDC_SETTLECOUNT_CH0);
    st_register_structure->SETTLECOUNT_CH1 = FDC_Read_Register(FDC_SETTLECOUNT_CH1);
    st_register_structure->SETTLECOUNT_CH2 = FDC_Read_Register(FDC_SETTLECOUNT_CH2);
    st_register_structure->SETTLECOUNT_CH3 = FDC_Read_Register(FDC_SETTLECOUNT_CH3);

    st_register_structure->CLOCK_DIVIDERS_CH0 = FDC_Read_Register(FDC_CLOCK_DIVIDERS_CH0);
    st_register_structure->CLOCK_DIVIDERS_CH1 = FDC_Read_Register(FDC_CLOCK_DIVIDERS_CH1);   
    st_register_structure->CLOCK_DIVIDERS_CH2 = FDC_Read_Register(FDC_CLOCK_DIVIDERS_CH2);
    st_register_structure->CLOCK_DIVIDERS_CH3 = FDC_Read_Register(FDC_CLOCK_DIVIDERS_CH3);

    st_register_structure->STATUS             = FDC_Read_Register(FDC_STATUS);
    st_register_structure->ERROR_CONFIG       = FDC_Read_Register(FDC_STATUS_CONFIG);
    st_register_structure->CONFIG             = FDC_Read_Register(FDC_CONFIG);
    st_register_structure->MUX_CONFIG         = FDC_Read_Register(FDC_MUX_CONFIG);
    st_register_structure->RESET_DEV          = FDC_Read_Register(FDC_RESET_DEV);
    
    st_register_structure->DRIVE_CURRENT_CH0  = FDC_Read_Register(FDC_DRIVE_CURRENT_CH0);
    st_register_structure->DRIVE_CURRENT_CH1  = FDC_Read_Register(FDC_DRIVE_CURRENT_CH1);
    st_register_structure->DRIVE_CURRENT_CH2  = FDC_Read_Register(FDC_DRIVE_CURRENT_CH2);
    st_register_structure->DRIVE_CURRENT_CH3  = FDC_Read_Register(FDC_DRIVE_CURRENT_CH3);
    
    st_register_structure->MANUFACTURER_ID    = FDC_Read_Register(FDC_MANUFACTURER_ID);
    st_register_structure->DEVICE_ID          = FDC_Read_Register(FDC_DEVICE_ID);
    
    return 0;
}

#define SQUARE_2PI    39.47841761466705

int FDC_Calc_Cap(st_data_typedef * st_data_structure,st_register_typedef * st_register_structure, \
                 st_freq_cap_typedef * st_freq_cap_structure)
{
    fdc_u16  temp_u16_1  =  0;
    fdc_u16  temp_u16_2  =  0;
    
    if(!(st_data_structure->Flag_AW0) && !(st_data_structure->Flag_WD0))
    {
        temp_u16_1 = (st_register_structure->CLOCK_DIVIDERS_CH0 & FDC_CHn_FIN_SEL) >> 12;//1
        temp_u16_2 = (st_register_structure->CLOCK_DIVIDERS_CH0 & FDC_CHn_FREF_DIVIDER);//1
        
		st_freq_cap_structure->freq_ch0 =  ( temp_u16_1 * External_Src * st_data_structure->Data_Ch0 ) \
                                        /  ( temp_u16_2 * 268435456 * 1.0) ;//MHz
		
        /*st_freq_cap_structure->freq_ch0 =  ( 5.0* st_data_structure->Data_Ch0 ) \
                                        /   33554432*1.0 ;//MHz
		*/
        st_freq_cap_structure->Cap_total_Ch0 = 1000000.0 / (SQUARE_2PI * PARA_SENSOR_L *  \
                                        (st_freq_cap_structure->freq_ch0) * (st_freq_cap_structure->freq_ch0));
		// st_freq_cap_structure->Cap_total_Ch0 = 63376395124000000/(st_data_structure->Data_Ch0*st_data_structure->Data_Ch0*1.0);
		
        st_freq_cap_structure->Cap_Sensor_Ch0 = st_freq_cap_structure->Cap_total_Ch0 - PARA_SENSOR_C ;     
    }
    
    temp_u16_1  =  0;
    temp_u16_2  =  0;
    
    if(!(st_data_structure->Flag_AW1) && !(st_data_structure->Flag_WD1))
    {
        temp_u16_1 = (st_register_structure->CLOCK_DIVIDERS_CH1 & FDC_CHn_FIN_SEL) >> 12;
        temp_u16_2 = (st_register_structure->CLOCK_DIVIDERS_CH1 & FDC_CHn_FREF_DIVIDER);
        
        st_freq_cap_structure->freq_ch1 =  ( temp_u16_1 * External_Src * st_data_structure->Data_Ch1 ) \
                                        /  ( temp_u16_2 * 268435456 * 1.0) ;//MHz
        
        st_freq_cap_structure->Cap_total_Ch1 = 1000000.0 / (SQUARE_2PI * PARA_SENSOR_L *  \
                                        (st_freq_cap_structure->freq_ch1) * (st_freq_cap_structure->freq_ch1));
        st_freq_cap_structure->Cap_Sensor_Ch1 = st_freq_cap_structure->Cap_total_Ch1 - PARA_SENSOR_C;      
    }

    temp_u16_1  =  0;
    temp_u16_2  =  0;
    
    if(!(st_data_structure->Flag_AW2) && !(st_data_structure->Flag_WD2))
    {
        temp_u16_1 = (st_register_structure->CLOCK_DIVIDERS_CH2 & FDC_CHn_FIN_SEL) >> 12;
        temp_u16_2 = (st_register_structure->CLOCK_DIVIDERS_CH2 & FDC_CHn_FREF_DIVIDER);
        
        st_freq_cap_structure->freq_ch2 =  ( temp_u16_1 * External_Src * st_data_structure->Data_Ch2 ) \
                                        /  ( temp_u16_2 * 268435456 * 1.0) ;//MHz
        
        st_freq_cap_structure->Cap_total_Ch2 = 1000000.0 / (SQUARE_2PI * PARA_SENSOR_L *  \
                                        (st_freq_cap_structure->freq_ch2) * (st_freq_cap_structure->freq_ch2));

        st_freq_cap_structure->Cap_Sensor_Ch2 = st_freq_cap_structure->Cap_total_Ch2 - PARA_SENSOR_C ;

    }
    
    temp_u16_1  =  0;
    temp_u16_2  =  0;

    if(!(st_data_structure->Flag_AW3) && !(st_data_structure->Flag_WD3))
    {
        temp_u16_1 = (st_register_structure->CLOCK_DIVIDERS_CH3 & FDC_CHn_FIN_SEL) >> 12;
        temp_u16_2 = (st_register_structure->CLOCK_DIVIDERS_CH3 & FDC_CHn_FREF_DIVIDER);
        
        st_freq_cap_structure->freq_ch3 =  ( temp_u16_1 * External_Src * st_data_structure->Data_Ch3 ) \
                                        /  ( temp_u16_2 * 268435456 * 1.0) ;//MHz
        
        st_freq_cap_structure->Cap_total_Ch3 = 1000000.0 / (SQUARE_2PI * PARA_SENSOR_L *  \
                                        (st_freq_cap_structure->freq_ch3) * (st_freq_cap_structure->freq_ch3));
  
        st_freq_cap_structure->Cap_Sensor_Ch3 = st_freq_cap_structure->Cap_total_Ch3 - PARA_SENSOR_C  ;       
    }    
    
    
    return 0;
}

void FDC2214_Init(void)
{
	while(FDC_Init(&st_fdc_init_structure,&st_register_structure)!=0){};
    FDC_Reflash_RegisterPara(&st_register_structure);
    FDC_Cmd_SleepMode(0);	
	delay_ms(50); 
}

float FDC_Result(u8 ch)
{
	fdc_u16  temp_u16_1  =  0;
    fdc_u16  temp_u16_2  =  0;
	FDC_Get_Data(&st_data_structure, ch );    
	switch(ch)
	{
		case CH0:
		{
		temp_u16_1 = (st_register_structure.CLOCK_DIVIDERS_CH0 & FDC_CHn_FIN_SEL) >> 12;//1
        temp_u16_2 = (st_register_structure.CLOCK_DIVIDERS_CH0 & FDC_CHn_FREF_DIVIDER);//1
        
		st_freq_cap_structure.freq_ch0 =  ( temp_u16_1 * External_Src * st_data_structure.Data_Ch0 ) \
                                        /  ( temp_u16_2 * 268435456 * 1.0) ;//MHz

        st_freq_cap_structure.Cap_total_Ch0 = 1000000.0 / (SQUARE_2PI * PARA_SENSOR_L *  \
                                        (st_freq_cap_structure.freq_ch0) * (st_freq_cap_structure.freq_ch0));

        st_freq_cap_structure.Cap_Sensor_Ch0 = st_freq_cap_structure.Cap_total_Ch0 - PARA_SENSOR_C ;    
		return st_freq_cap_structure.Cap_Sensor_Ch0;			
		}break;		

case CH1:
		{
		temp_u16_1 = (st_register_structure.CLOCK_DIVIDERS_CH1 & FDC_CHn_FIN_SEL) >> 12;//1
        temp_u16_2 = (st_register_structure.CLOCK_DIVIDERS_CH1 & FDC_CHn_FREF_DIVIDER);//1
        
		st_freq_cap_structure.freq_ch1 =  ( temp_u16_1 * External_Src * st_data_structure.Data_Ch1 ) \
                                        /  ( temp_u16_2 * 268435456 * 1.0) ;//MHz

        st_freq_cap_structure.Cap_total_Ch1 = 1000000.0 / (SQUARE_2PI * PARA_SENSOR_L *  \
                                        (st_freq_cap_structure.freq_ch1) * (st_freq_cap_structure.freq_ch1));

        st_freq_cap_structure.Cap_Sensor_Ch1 = st_freq_cap_structure.Cap_total_Ch1 - PARA_SENSOR_C ; 
		return st_freq_cap_structure.Cap_Sensor_Ch1;			
		}break;		

case CH2:
		{
		temp_u16_1 = (st_register_structure.CLOCK_DIVIDERS_CH2 & FDC_CHn_FIN_SEL) >> 12;//1
        temp_u16_2 = (st_register_structure.CLOCK_DIVIDERS_CH2 & FDC_CHn_FREF_DIVIDER);//1
        
		st_freq_cap_structure.freq_ch2 =  ( temp_u16_1 * External_Src * st_data_structure.Data_Ch2 ) \
                                        /  ( temp_u16_2 * 268435456 * 1.0) ;//MHz

        st_freq_cap_structure.Cap_total_Ch2 = 1000000.0 / (SQUARE_2PI * PARA_SENSOR_L *  \
                                        (st_freq_cap_structure.freq_ch2) * (st_freq_cap_structure.freq_ch2));

        st_freq_cap_structure.Cap_Sensor_Ch2 = st_freq_cap_structure.Cap_total_Ch2 - PARA_SENSOR_C ;   
		return st_freq_cap_structure.Cap_Sensor_Ch2;			
		}break;		

case CH3:
		{
		temp_u16_1 = (st_register_structure.CLOCK_DIVIDERS_CH3 & FDC_CHn_FIN_SEL) >> 12;//1
        temp_u16_2 = (st_register_structure.CLOCK_DIVIDERS_CH3 & FDC_CHn_FREF_DIVIDER);//1
        
		st_freq_cap_structure.freq_ch3 =  ( temp_u16_1 * External_Src * st_data_structure.Data_Ch3 ) \
                                        /  ( temp_u16_2 * 268435456 * 1.0) ;//MHz

        st_freq_cap_structure.Cap_total_Ch3 = 1000000.0 / (SQUARE_2PI * PARA_SENSOR_L *  \
                                        (st_freq_cap_structure.freq_ch3) * (st_freq_cap_structure.freq_ch3));

        st_freq_cap_structure.Cap_Sensor_Ch3 = st_freq_cap_structure.Cap_total_Ch3 - PARA_SENSOR_C ;    
		return st_freq_cap_structure.Cap_Sensor_Ch3;
		}break;				
	}
}

