
#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "usbh_core.h"
#include "usb_conf.h"
#include "usbh_hid_mouse.h"

 void USBH_USR_Init_Callback();
 void USBH_USR_DeviceAttached_Callback();
 void USBH_USR_UnrecoveredError_Callback();
 void USBH_USR_DeviceDisconnected_Callback();
 void USBH_USR_ResetDevice_Callback();
 void USBH_USR_DeviceSpeedDetected_Callback(uint8_t DeviceSpeed);
 void USBH_USR_Device_DescAvailable_Callback(void *DeviceDesc);
 void USBH_USR_DeviceAddressAssigned_Callback();
 void USBH_USR_Configuration_DescAvailable_Callback(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc);
 void USBH_USR_Manufacturer_String_Callback(void *ManufacturerString);
 void USBH_USR_Product_String_Callback(void *ProductString);
 void USBH_USR_SerialNum_String_Callback(void *SerialNumString);
 void USBH_USR_EnumerationDone_Callback();
 void USBH_USR_DeviceNotSupported_Callback();
 USBH_USR_Status USBH_USR_UserInput_Callback();
 void USBH_USR_OverCurrentDetected_Callback();
 void USBH_USR_DeInit_Callback();
 
 void USR_MOUSE_Init_Callback();
 void USR_MOUSE_ProcessData_Callback(HID_MOUSE_Data_TypeDef *data);
 void USR_KEYBRD_Init_Callback();
 void USR_KEYBRD_ProcessData_Callback(uint8_t data);
 
 #ifdef __cplusplus
}
#endif

#endif