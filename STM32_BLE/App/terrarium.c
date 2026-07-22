/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Terrarium.c
  * @author  MCD Application Team
  * @brief   Terrarium definition.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include <app_common.h>
#include "ble.h"
#include "terrarium.h"
#include "terrarium_app.h"
#include "ble_evt.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

typedef struct{
  uint16_t  TerrariumSvcHdle;				/**< Terrarium Service Handle */
  uint16_t  TemperaturaCharHdle;			/**< TEMPERATURA Characteristic Handle */
  uint16_t  UmiditaCharHdle;			/**< UMIDITA Characteristic Handle */
  uint16_t  VentolaCharHdle;			/**< VENTOLA Characteristic Handle */
/* USER CODE BEGIN Context */
  /* Place holder for Characteristic Descriptors Handle*/

/* USER CODE END Context */
}TERRARIUM_Context_t;

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private macros ------------------------------------------------------------*/
#define CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET        2
#define CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET             1
#define TEMPERATURA_SIZE        4	/* Temperatura Characteristic size */
#define UMIDITA_SIZE        4	/* Umidita Characteristic size */
#define VENTOLA_SIZE        1	/* Ventola Characteristic size */
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

static TERRARIUM_Context_t TERRARIUM_Context;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
/* USER CODE BEGIN PFD */

/* USER CODE END PFD */

/* Private functions ----------------------------------------------------------*/

/*
 * UUIDs for Terrarium service
 */
#define TERRARIUM_UUID			(0xabcd)
#define TEMPERATURA_UUID			0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#define UMIDITA_UUID			0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#define VENTOLA_UUID			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

BLE_GATT_SRV_CCCD_DECLARE(temperatura, CFG_BLE_NUM_RADIO_TASKS, BLE_GATT_SRV_CCCD_PERM_DEFAULT,
                          BLE_GATT_SRV_OP_MODIFIED_EVT_ENABLE_FLAG);
BLE_GATT_SRV_CCCD_DECLARE(umidita, CFG_BLE_NUM_RADIO_TASKS, BLE_GATT_SRV_CCCD_PERM_DEFAULT,
                          BLE_GATT_SRV_OP_MODIFIED_EVT_ENABLE_FLAG);
BLE_GATT_SRV_CCCD_DECLARE(ventola, CFG_BLE_NUM_RADIO_TASKS, BLE_GATT_SRV_CCCD_PERM_DEFAULT,
                          BLE_GATT_SRV_OP_MODIFIED_EVT_ENABLE_FLAG);

/* USER CODE BEGIN DESCRIPTORS DECLARATION */

/* USER CODE END DESCRIPTORS DECLARATION */

uint8_t temperatura_val_buffer[TEMPERATURA_SIZE];

static ble_gatt_val_buffer_def_t temperatura_val_buffer_def = {
  .op_flags = BLE_GATT_SRV_OP_MODIFIED_EVT_ENABLE_FLAG,
  .val_len = TEMPERATURA_SIZE,
  .buffer_len = sizeof(temperatura_val_buffer),
  .buffer_p = temperatura_val_buffer
};

uint8_t umidita_val_buffer[UMIDITA_SIZE];

static ble_gatt_val_buffer_def_t umidita_val_buffer_def = {
  .op_flags = BLE_GATT_SRV_OP_MODIFIED_EVT_ENABLE_FLAG,
  .val_len = UMIDITA_SIZE,
  .buffer_len = sizeof(umidita_val_buffer),
  .buffer_p = umidita_val_buffer
};

uint8_t ventola_val_buffer[VENTOLA_SIZE];

static ble_gatt_val_buffer_def_t ventola_val_buffer_def = {
  .op_flags = BLE_GATT_SRV_OP_MODIFIED_EVT_ENABLE_FLAG,
  .val_len = VENTOLA_SIZE,
  .buffer_len = sizeof(ventola_val_buffer),
  .buffer_p = ventola_val_buffer
};

/* Terrarium service TEMPERATURA (notification) UMIDITA (notification) VENTOLA (notification) characteristics definition */
static const ble_gatt_chr_def_t terrarium_chars[] = {
	{
        .properties = BLE_GATT_SRV_CHAR_PROP_READ | BLE_GATT_SRV_CHAR_PROP_NOTIFY,
        .permissions = BLE_GATT_SRV_PERM_NONE,
        .min_key_size = 0x10,
        .uuid = BLE_UUID_INIT_128(TEMPERATURA_UUID),
        .descrs = {
            .descrs_p = &BLE_GATT_SRV_CCCD_DEF_NAME(temperatura),
            .descr_count = 1U,
        },
        .val_buffer_p = &temperatura_val_buffer_def
    },
	{
        .properties = BLE_GATT_SRV_CHAR_PROP_READ | BLE_GATT_SRV_CHAR_PROP_NOTIFY,
        .permissions = BLE_GATT_SRV_PERM_NONE,
        .min_key_size = 0x10,
        .uuid = BLE_UUID_INIT_128(UMIDITA_UUID),
        .descrs = {
            .descrs_p = &BLE_GATT_SRV_CCCD_DEF_NAME(umidita),
            .descr_count = 1U,
        },
        .val_buffer_p = &umidita_val_buffer_def
    },
	{
        .properties = BLE_GATT_SRV_CHAR_PROP_READ | BLE_GATT_SRV_CHAR_PROP_NOTIFY,
        .permissions = BLE_GATT_SRV_PERM_NONE,
        .min_key_size = 0x10,
        .uuid = BLE_UUID_INIT_128(VENTOLA_UUID),
        .descrs = {
            .descrs_p = &BLE_GATT_SRV_CCCD_DEF_NAME(ventola),
            .descr_count = 1U,
        },
        .val_buffer_p = &ventola_val_buffer_def
    },
};

/* Terrarium service definition */
static const ble_gatt_srv_def_t terrarium_service = {
   .type = BLE_GATT_SRV_PRIMARY_SRV_TYPE,
   .uuid = BLE_UUID_INIT_128(TERRARIUM_UUID),
   .chrs = {
       .chrs_p = (ble_gatt_chr_def_t *)terrarium_chars,
       .chr_count = 3U,
   },
};

/* USER CODE BEGIN PF */

/* USER CODE END PF */

/**
 * @brief  Event handler
 * @param  p_Event: Address of the buffer holding the p_Event
 * @retval Ack: Return whether the p_Event has been managed or not
 */
static BLEEVT_EvtAckStatus_t TERRARIUM_EventHandler(aci_blecore_event *p_evt)
{
  BLEEVT_EvtAckStatus_t return_value = BLEEVT_NoAck;
  aci_gatt_srv_attribute_modified_event_rp0 *p_attribute_modified;
  aci_gatt_srv_read_event_rp0    *p_read;
  TERRARIUM_NotificationEvt_t notification;
  /* USER CODE BEGIN Service1_EventHandler_1 */

  /* USER CODE END Service1_EventHandler_1 */

  switch(p_evt->ecode)
  {
    case ACI_GATT_SRV_ATTRIBUTE_MODIFIED_VSEVT_CODE:
    {
      /* USER CODE BEGIN EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_BEGIN */

      /* USER CODE END EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_BEGIN */
      p_attribute_modified = (aci_gatt_srv_attribute_modified_event_rp0*)p_evt->data;
      notification.ConnectionHandle         = p_attribute_modified->Connection_Handle;
      notification.AttributeHandle          = p_attribute_modified->Attr_Handle;
      notification.DataTransfered.Length    = p_attribute_modified->Attr_Data_Length;
      notification.DataTransfered.p_Payload = p_attribute_modified->Attr_Data;
      if(p_attribute_modified->Attr_Handle == (TERRARIUM_Context.TemperaturaCharHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
      {
        return_value = BLEEVT_Ack;
        /* USER CODE BEGIN Service1_Char_1 */

        /* USER CODE END Service1_Char_1 */
        switch(p_attribute_modified->Attr_Data[0])
		{
          /* USER CODE BEGIN Service1_Char_1_attribute_modified */

          /* USER CODE END Service1_Char_1_attribute_modified */

          /* Disabled Notification management */
        case (!BLE_GATT_SRV_CCCD_NOTIFICATION):
          /* USER CODE BEGIN Service1_Char_1_Disabled_BEGIN */

          /* USER CODE END Service1_Char_1_Disabled_BEGIN */
          notification.EvtOpcode = TERRARIUM_TEMPERATURA_NOTIFY_DISABLED_EVT;
          TERRARIUM_Notification(&notification);
          /* USER CODE BEGIN Service1_Char_1_Disabled_END */

          /* USER CODE END Service1_Char_1_Disabled_END */
          break;

          /* Enabled Notification management */
        case BLE_GATT_SRV_CCCD_NOTIFICATION:
          /* USER CODE BEGIN Service1_Char_1_COMSVC_Notification_BEGIN */

          /* USER CODE END Service1_Char_1_COMSVC_Notification_BEGIN */
          notification.EvtOpcode = TERRARIUM_TEMPERATURA_NOTIFY_ENABLED_EVT;
          TERRARIUM_Notification(&notification);
          /* USER CODE BEGIN Service1_Char_1_COMSVC_Notification_END */

          /* USER CODE END Service1_Char_1_COMSVC_Notification_END */
          break;

        default:
          /* USER CODE BEGIN Service1_Char_1_default */

          /* USER CODE END Service1_Char_1_default */
          break;
        }
      }  /* if(p_attribute_modified->Attr_Handle == (TERRARIUM_Context.TemperaturaCharHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

      else if(p_attribute_modified->Attr_Handle == (TERRARIUM_Context.UmiditaCharHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
      {
        return_value = BLEEVT_Ack;
        /* USER CODE BEGIN Service1_Char_2 */

        /* USER CODE END Service1_Char_2 */
        switch(p_attribute_modified->Attr_Data[0])
		{
          /* USER CODE BEGIN Service1_Char_2_attribute_modified */

          /* USER CODE END Service1_Char_2_attribute_modified */

          /* Disabled Notification management */
        case (!BLE_GATT_SRV_CCCD_NOTIFICATION):
          /* USER CODE BEGIN Service1_Char_2_Disabled_BEGIN */

          /* USER CODE END Service1_Char_2_Disabled_BEGIN */
          notification.EvtOpcode = TERRARIUM_UMIDITA_NOTIFY_DISABLED_EVT;
          TERRARIUM_Notification(&notification);
          /* USER CODE BEGIN Service1_Char_2_Disabled_END */

          /* USER CODE END Service1_Char_2_Disabled_END */
          break;

          /* Enabled Notification management */
        case BLE_GATT_SRV_CCCD_NOTIFICATION:
          /* USER CODE BEGIN Service1_Char_2_COMSVC_Notification_BEGIN */

          /* USER CODE END Service1_Char_2_COMSVC_Notification_BEGIN */
          notification.EvtOpcode = TERRARIUM_UMIDITA_NOTIFY_ENABLED_EVT;
          TERRARIUM_Notification(&notification);
          /* USER CODE BEGIN Service1_Char_2_COMSVC_Notification_END */

          /* USER CODE END Service1_Char_2_COMSVC_Notification_END */
          break;

        default:
          /* USER CODE BEGIN Service1_Char_2_default */

          /* USER CODE END Service1_Char_2_default */
          break;
        }
      }  /* if(p_attribute_modified->Attr_Handle == (TERRARIUM_Context.UmiditaCharHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

      else if(p_attribute_modified->Attr_Handle == (TERRARIUM_Context.VentolaCharHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
      {
        return_value = BLEEVT_Ack;
        /* USER CODE BEGIN Service1_Char_3 */

        /* USER CODE END Service1_Char_3 */
        switch(p_attribute_modified->Attr_Data[0])
		{
          /* USER CODE BEGIN Service1_Char_3_attribute_modified */

          /* USER CODE END Service1_Char_3_attribute_modified */

          /* Disabled Notification management */
        case (!BLE_GATT_SRV_CCCD_NOTIFICATION):
          /* USER CODE BEGIN Service1_Char_3_Disabled_BEGIN */

          /* USER CODE END Service1_Char_3_Disabled_BEGIN */
          notification.EvtOpcode = TERRARIUM_VENTOLA_NOTIFY_DISABLED_EVT;
          TERRARIUM_Notification(&notification);
          /* USER CODE BEGIN Service1_Char_3_Disabled_END */

          /* USER CODE END Service1_Char_3_Disabled_END */
          break;

          /* Enabled Notification management */
        case BLE_GATT_SRV_CCCD_NOTIFICATION:
          /* USER CODE BEGIN Service1_Char_3_COMSVC_Notification_BEGIN */

          /* USER CODE END Service1_Char_3_COMSVC_Notification_BEGIN */
          notification.EvtOpcode = TERRARIUM_VENTOLA_NOTIFY_ENABLED_EVT;
          TERRARIUM_Notification(&notification);
          /* USER CODE BEGIN Service1_Char_3_COMSVC_Notification_END */

          /* USER CODE END Service1_Char_3_COMSVC_Notification_END */
          break;

        default:
          /* USER CODE BEGIN Service1_Char_3_default */

          /* USER CODE END Service1_Char_3_default */
          break;
        }
      }  /* if(p_attribute_modified->Attr_Handle == (TERRARIUM_Context.VentolaCharHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

      /* USER CODE BEGIN EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_END */

      /* USER CODE END EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_END */
      break;/* ACI_GATT_SRV_ATTRIBUTE_MODIFIED_VSEVT_CODE */
    }
    case ACI_GATT_SRV_READ_VSEVT_CODE :
    {
      /* USER CODE BEGIN EVT_BLUE_GATT_SRV_READ_BEGIN */

      /* USER CODE END EVT_BLUE_GATT_SRV_READ_BEGIN */
      p_read = (aci_gatt_srv_read_event_rp0*)p_evt->data;
	  if(p_read->Attribute_Handle == (TERRARIUM_Context.TemperaturaCharHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
	  {
		return_value = BLEEVT_Ack;
		/*USER CODE BEGIN Service1_Char_1_ACI_GATT_SRV_READ_VSEVT_CODE_1 */
#warning user shall call aci_gatt_srv_read_resp() function if allowed
		/*USER CODE END Service1_Char_1_ACI_GATT_SRV_READ_VSEVT_CODE_1 */

		/*USER CODE BEGIN Service1_Char_1_ACI_GATT_SRV_READ_VSEVT_CODE_2 */

		  /*USER CODE END Service1_Char_1_ACI_GATT_SRV_READ_VSEVT_CODE_2 */
	  } /* if(p_read->Attribute_Handle == (TERRARIUM_Context.TemperaturaCharHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
	  else if(p_read->Attribute_Handle == (TERRARIUM_Context.UmiditaCharHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
	  {
		return_value = BLEEVT_Ack;
		/*USER CODE BEGIN Service1_Char_2_ACI_GATT_SRV_READ_VSEVT_CODE_1 */
#warning user shall call aci_gatt_srv_read_resp() function if allowed
		/*USER CODE END Service1_Char_2_ACI_GATT_SRV_READ_VSEVT_CODE_1 */

		/*USER CODE BEGIN Service1_Char_2_ACI_GATT_SRV_READ_VSEVT_CODE_2 */

		  /*USER CODE END Service1_Char_2_ACI_GATT_SRV_READ_VSEVT_CODE_2 */
	  } /* if(p_read->Attribute_Handle == (TERRARIUM_Context.UmiditaCharHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
	  else if(p_read->Attribute_Handle == (TERRARIUM_Context.VentolaCharHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
	  {
		return_value = BLEEVT_Ack;
		/*USER CODE BEGIN Service1_Char_3_ACI_GATT_SRV_READ_VSEVT_CODE_1 */
#warning user shall call aci_gatt_srv_read_resp() function if allowed
		/*USER CODE END Service1_Char_3_ACI_GATT_SRV_READ_VSEVT_CODE_1 */

		/*USER CODE BEGIN Service1_Char_3_ACI_GATT_SRV_READ_VSEVT_CODE_2 */

		  /*USER CODE END Service1_Char_3_ACI_GATT_SRV_READ_VSEVT_CODE_2 */
	  } /* if(p_read->Attribute_Handle == (TERRARIUM_Context.VentolaCharHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/

      /* USER CODE BEGIN EVT_BLUE_GATT_SRV_READ_END */

      /* USER CODE END EVT_EVT_BLUE_GATT_SRV_READ_END */
      break;/* ACI_GATT_SRV_READ_VSEVT_CODE */
    }
    case ACI_GATT_SRV_WRITE_VSEVT_CODE:
    {
      /* USER CODE BEGIN EVT_BLUE_SRV_GATT_BEGIN */

      /* USER CODE END EVT_BLUE_SRV_GATT_BEGIN */

      /* USER CODE BEGIN EVT_BLUE_GATT_SRV_WRITE_END */

      /* USER CODE END EVT_BLUE_GATT_SRV_WRITE_END */
      break;/* ACI_GATT_SRV_WRITE_VSEVT_CODE */
    }
    case ACI_GATT_TX_POOL_AVAILABLE_VSEVT_CODE:
    {
      aci_gatt_tx_pool_available_event_rp0 *p_tx_pool_available_event;
      p_tx_pool_available_event = (aci_gatt_tx_pool_available_event_rp0 *) p_evt->data;
      UNUSED(p_tx_pool_available_event);

      /* USER CODE BEGIN ACI_GATT_TX_POOL_AVAILABLE_VSEVT_CODE */

      /* USER CODE END ACI_GATT_TX_POOL_AVAILABLE_VSEVT_CODE */
      break;/* ACI_GATT_TX_POOL_AVAILABLE_VSEVT_CODE*/
    }
    case ACI_ATT_EXCHANGE_MTU_RESP_VSEVT_CODE:
    {
      aci_att_exchange_mtu_resp_event_rp0 *p_exchange_mtu;
      p_exchange_mtu = (aci_att_exchange_mtu_resp_event_rp0 *)  p_evt->data;
      UNUSED(p_exchange_mtu);

      /* USER CODE BEGIN ACI_ATT_EXCHANGE_MTU_RESP_VSEVT_CODE */

      /* USER CODE END ACI_ATT_EXCHANGE_MTU_RESP_VSEVT_CODE */
      break;/* ACI_ATT_EXCHANGE_MTU_RESP_VSEVT_CODE */
    }
    /* USER CODE BEGIN BLECORE_EVT */

    /* USER CODE END BLECORE_EVT */
  default:
    /* USER CODE BEGIN EVT_DEFAULT */

    /* USER CODE END EVT_DEFAULT */
    break;
  }

  /* USER CODE BEGIN Service1_EventHandler_2 */

  /* USER CODE END Service1_EventHandler_2 */

  return(return_value);
}/* end TERRARIUM_EventHandler */

/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void TERRARIUM_Init(void)
{
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  UNUSED(TERRARIUM_Context);

  /* USER CODE BEGIN InitService1Svc_1 */

  /* USER CODE END InitService1Svc_1 */

  /**
   *  Register the event handler to the BLE controller
   */
  BLEEVT_RegisterGattEvtHandler(TERRARIUM_EventHandler);

  ret = aci_gatt_srv_add_service((ble_gatt_srv_def_t *)&terrarium_service);

  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail   : aci_gatt_srv_add_service command: Terrarium, error code: 0x%x \n", ret);
  }
  else
  {
    APP_DBG_MSG("  Success: aci_gatt_srv_add_service command: Terrarium \n");
  }

  TERRARIUM_Context.TerrariumSvcHdle = aci_gatt_srv_get_service_handle((ble_gatt_srv_def_t *) &terrarium_service);
  TERRARIUM_Context.TemperaturaCharHdle = aci_gatt_srv_get_char_decl_handle((ble_gatt_chr_def_t *)&terrarium_chars[0]);
  TERRARIUM_Context.UmiditaCharHdle = aci_gatt_srv_get_char_decl_handle((ble_gatt_chr_def_t *)&terrarium_chars[1]);
  TERRARIUM_Context.VentolaCharHdle = aci_gatt_srv_get_char_decl_handle((ble_gatt_chr_def_t *)&terrarium_chars[2]);

  /* USER CODE BEGIN InitService1Svc_2 */

  /* USER CODE END InitService1Svc_2 */

  if (ret != BLE_STATUS_SUCCESS)
  {
    APP_DBG_MSG("  Fail registering Terrarium handlers\n");
  }

  return;
}

/**
 * @brief  Characteristic update
 * @param  CharOpcode: Characteristic identifier
 * @param  pData: pointer to the new data to be written in the characteristic
 *
 */
tBleStatus TERRARIUM_UpdateValue(TERRARIUM_CharOpcode_t CharOpcode, TERRARIUM_Data_t *pData)
{
  tBleStatus ret = BLE_STATUS_SUCCESS;

  /* USER CODE BEGIN Service1_App_Update_Char_1 */

  /* USER CODE END Service1_App_Update_Char_1 */

  switch(CharOpcode)
  {

    default:
      break;
  }

  /* USER CODE BEGIN Service1_App_Update_Char_2 */

  /* USER CODE END Service1_App_Update_Char_2 */

  return ret;
}

/**
 * @brief  Characteristic notification
 * @param  CharOpcode: Characteristic identifier
 * @param  pData: pointer to the data to be notified to the client
 * @param  ConnectionHandle: connection handle identifying the client to be notified.
 *
 */
tBleStatus TERRARIUM_NotifyValue(TERRARIUM_CharOpcode_t CharOpcode, TERRARIUM_Data_t *pData, uint16_t ConnectionHandle)
{
  tBleStatus ret = BLE_STATUS_INVALID_PARAMS;
  /* USER CODE BEGIN Service1_App_Notify_Char_1 */

  /* USER CODE END Service1_App_Notify_Char_1 */

  switch(CharOpcode)
  {
    case TERRARIUM_TEMPERATURA:
      memcpy(temperatura_val_buffer, pData->p_Payload, MIN(pData->Length, sizeof(temperatura_val_buffer)));
      ret = aci_gatt_srv_notify(ConnectionHandle,
                                BLE_GATT_UNENHANCED_ATT_L2CAP_CID,
                                TERRARIUM_Context.TemperaturaCharHdle + 1,
                                GATT_NOTIFICATION,
                                pData->Length, /* charValueLen */
                                (uint8_t *)pData->p_Payload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_srv_notify TEMPERATURA command, error code: 0x%2X\n", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_srv_notify TEMPERATURA command\n");
      }
      /* USER CODE BEGIN Service1_Char_Value_1*/

      /* USER CODE END Service1_Char_Value_1*/
      break;

    case TERRARIUM_UMIDITA:
      memcpy(umidita_val_buffer, pData->p_Payload, MIN(pData->Length, sizeof(umidita_val_buffer)));
      ret = aci_gatt_srv_notify(ConnectionHandle,
                                BLE_GATT_UNENHANCED_ATT_L2CAP_CID,
                                TERRARIUM_Context.UmiditaCharHdle + 1,
                                GATT_NOTIFICATION,
                                pData->Length, /* charValueLen */
                                (uint8_t *)pData->p_Payload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_srv_notify UMIDITA command, error code: 0x%2X\n", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_srv_notify UMIDITA command\n");
      }
      /* USER CODE BEGIN Service1_Char_Value_2*/

      /* USER CODE END Service1_Char_Value_2*/
      break;

    case TERRARIUM_VENTOLA:
      memcpy(ventola_val_buffer, pData->p_Payload, MIN(pData->Length, sizeof(ventola_val_buffer)));
      ret = aci_gatt_srv_notify(ConnectionHandle,
                                BLE_GATT_UNENHANCED_ATT_L2CAP_CID,
                                TERRARIUM_Context.VentolaCharHdle + 1,
                                GATT_NOTIFICATION,
                                pData->Length, /* charValueLen */
                                (uint8_t *)pData->p_Payload);
      if (ret != BLE_STATUS_SUCCESS)
      {
        APP_DBG_MSG("  Fail   : aci_gatt_srv_notify VENTOLA command, error code: 0x%2X\n", ret);
      }
      else
      {
        APP_DBG_MSG("  Success: aci_gatt_srv_notify VENTOLA command\n");
      }
      /* USER CODE BEGIN Service1_Char_Value_3*/

      /* USER CODE END Service1_Char_Value_3*/
      break;

    default:
      break;
  }

  /* USER CODE BEGIN Service1_App_Notify_Char_2 */

  /* USER CODE END Service1_App_Notify_Char_2 */

  return ret;
}
