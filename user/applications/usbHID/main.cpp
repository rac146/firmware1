/**
  ******************************************************************************
  * @file    main.cpp
  * @author  Robert Campo
  * @version V2.1.0
  * @date    19-March-2012
  * @brief   This is a modification of the original HID example provided 
  *          by ST in version 2.1.0 of their driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "application.h"
#include "main.h"

extern "C" {

#include "usbh_core.h"
#include "usbh_hid_core.h"
#include "usbh_usr.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"
#include "usb_hcd_int.h"
#include "usb_dcd_int.h"
#include "usb_dcd.h"
#include "usb_core.h"
}

int led2 = D7;
TCPClient client;
USBH_HOST USB_Host;

#define KYBRD_FIRST_COLUMN               (uint16_t)319
#define KYBRD_LAST_COLUMN                (uint16_t)7
#define KYBRD_FIRST_LINE                 (uint8_t)120
#define KYBRD_LAST_LINE                  (uint8_t)200

uint8_t  KeybrdCharXpos           = 0;
uint16_t KeybrdCharYpos           = 0;
extern  int16_t  x_loc, y_loc; 
extern __IO int16_t  prev_x, prev_y;

__IO uint32_t i = 0;
char bufferHeader[512];

void setup() {
  
    //Serial1DebugOutput debugOutput(9600, LOG_LEVEL);
	Serial1.begin(9600);
	
	//this method is disabling usb device mode through HAL
	Serial.Initialize(); 
	
	//enabling host mode
	USBH_Init(&USB_OTG_dev, 
            USB_OTG_HS_CORE_ID,
            &USB_Host,
            &HID_cb, 
            &USR_Callbacks);
  
    Serial1.println("Hello!");
}

void loop() {
	
	//call each loop
	USBH_Process(&USB_OTG_dev, &USB_Host);

}

void USBH_USR_Init_Callback()
{
	Serial1.println("USB Initialized");
}

void USBH_USR_DeviceAttached_Callback()
{
	Serial1.println("USB Device Attached");
}

void USBH_USR_UnrecoveredError_Callback()
{
	Serial1.println("USB Unrecovered Error");
}

void USBH_USR_DeviceDisconnected_Callback()
{
	Serial1.println("");
	Serial1.println("USB Device Disconnected");
}

void USBH_USR_ResetDevice_Callback()
{
	Serial1.println("USB Reset Data");
}

void USBH_USR_DeviceSpeedDetected_Callback(uint8_t DeviceSpeed) 
{
  if(DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED)
  {
    Serial1.println("High speed USB device detected");
  }  
  else if(DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED)
  {
    Serial1.println("Full speed USB device detected");
  }
  else if(DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED)
  {
    Serial1.println("Low speed USB device detected");
  }
  else
  {
    Serial1.println("Error detecting device speed");
  }
}
 
void USBH_USR_Device_DescAvailable_Callback(void *DeviceDesc)
{
	uint8_t temp[50];    
	USBH_DevDesc_TypeDef *hs;
	hs = (USBH_DevDesc_TypeDef*) DeviceDesc;  
  
	Serial1.printf((char *)temp , "VID : %04Xh\n" , (uint32_t)(*hs).idVendor); 
  
	Serial1.printf((char *)temp , "PID : %04Xh\n" , (uint32_t)(*hs).idProduct); 
}

void USBH_USR_DeviceAddressAssigned_Callback()
{
	Serial1.println("USB Device Address Assigned");
}

void USBH_USR_Configuration_DescAvailable_Callback(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
	USBH_InterfaceDesc_TypeDef *id;

	id = itfDesc;  

	if((*id).bInterfaceClass  == 0x08)
	{
		Serial1.println("MSC device detected");
	}
	else if((*id).bInterfaceClass  == 0x03)
	{
		Serial1.println("HID device detected");
	} 
}

void USBH_USR_Manufacturer_String_Callback(void *ManufacturerString)
{
	char temp[100];
	Serial1.printf(temp, "Manufacturer : %s\n", (char *)ManufacturerString);
}
 
 void USBH_USR_Product_String_Callback(void *ProductString)
{
	char temp[100];
	Serial1.printf((char *)temp, "Product : %s\n", (char *)ProductString);  
}

void USBH_USR_SerialNum_String_Callback(void *SerialNumString)
{
	uint8_t temp[100];
	Serial1.printf((char *)temp, "Serial Number : %s\n", (char *)SerialNumString);    
}

void USBH_USR_EnumerationDone_Callback()
{
	Serial1.println("Enumeration Complete");
}

void USBH_USR_DeviceNotSupported_Callback() 
{
	Serial1.println("Device Not Supported");
}

USBH_USR_Status USBH_USR_UserInput_Callback()
{
  USBH_USR_Status usbh_usr_status;

  usbh_usr_status = USBH_USR_RESP_OK;
  
  return usbh_usr_status;
}

void USBH_USR_OverCurrentDetected_Callback()
{
	Serial1.println("Over current detected");
}

void USBH_USR_DeInit_Callback() 
{
	Serial1.println("USB Deinit");
}

void USR_MOUSE_Init_Callback()
{
	Serial1.println("Mouse Initialized");
}

void USR_MOUSE_ProcessData_Callback(HID_MOUSE_Data_TypeDef *data)
{
	Serial1.printf("X:%i Y:%i", data->x, data->y);
	Serial1.println("");
}

void USR_KEYBRD_Init_Callback()
{
	Serial1.println("Keyboard Initialized. Start typing:");
}

void USR_KEYBRD_ProcessData_Callback(uint8_t data) 
{
 Serial1.printf("%c", data);
}


#ifdef USE_FULL_ASSERT
/**
* @brief  assert_failed
*         Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  File: pointer to the source file name
* @param  Line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line
  number,ex: printf("Wrong parameters value: file %s on line %d\r\n", 
  file, line) */
  
  /* Infinite loop */
  while (1)
  {}
}

#endif


/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
