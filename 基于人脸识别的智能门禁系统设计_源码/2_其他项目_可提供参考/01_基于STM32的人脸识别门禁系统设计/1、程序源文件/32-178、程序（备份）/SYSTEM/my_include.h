/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.0
//修改日期:2020/06/06
//程序功能：将工程中所有用到的头文件（库文件除外）包含在此头文件中
************************************************/
#ifndef __MY_INCLUDE_H
#define __MY_INCLUDE_H

/******************************包含头文件*******************************************/
#include "my_sysdef.h"
#ifndef  USE_RT_THREAD
#include "my_systick.h"
#endif
#include "delay.h"
#include "mygpio.h"
#include "usart.h"
////////////////////
//#include "my_24cxx.h"
//#include "my_adc.h"
//#include "my_adxl345.h"
//#include "my_alarm.h"
//#include "my_am2301.h"
//#include "my_angle.h"
//#include "APDS9960.h"
//#include "my_bh1750.h"
//#include "my_bmp180.h"
//#include "my_capture.h"
//#include "my_ccs811.h"
//#include "my_dht11.h"
//#include "my_dma.h"
//#include "my_ds18b20.h"
//#include "my_esp8266.h"
//#include "my_exti.h"
#include "my_face.h"
//#include "my_fileutils.h"
//#include "my_flowsensor.h"
//#include "my_fpm10a.h"
//#include "my_gps.h"
//#include "my_gy25.h"
//#include "my_hc595.h"
//#include "my_heartrate.h"
//#include "my_hx711.h"
//#include "my_iic.h"
//#include "my_iic_slave.h"
//#include "my_ir_uart.h"
//#include "my_jr6001.h"
#include "my_key.h"
//#include "my_key_matrix.h"
//#include "my_lcd1602.h"
//#include "my_lcd12864.h"
//#include "my_lcd.h"
#include "my_lcd_spi.h"
#include "my_lcd_spi_gui.h"
//#include "my_lcd_spi_touch.h"
#include "my_led.h"
//#include "my_ledblink.h"
//#include "my_matrixled.h"
//#include "my_max301xx_uart.h"
#include "my_menu.h"
//#include "my_oled.h"
//#include "my_pm25_uart.h"
#include "my_pwm.h"
//#include "my_rc522.h"
//#include "my_rtc.h"
//#include "my_sdcard_sdio.h"
//#include "my_segmentled.h"
//#include "my_serial_msg.h"
#include "my_sg90.h"
//#include "my_sim800a.h"
//#include "my_soundfreq.h"
//#include "my_spi_h.h"
//#include "my_stepmotor.h"
//#include "my_stmflash.h"
//#include "my_timer.h"
//#include "my_tof10120.h"
#include "my_uart_msg.h"
//#include "my_ultwave.h"
//#include "my_w25qxx.h" 
//#include "my_wt588d.h"


#endif
