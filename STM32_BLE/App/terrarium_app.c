/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Terrarium_app.c
  * @author  MCD Application Team
  * @brief   Terrarium_app application definition.
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
#include "main.h"
#include "app_common.h"
#include "app_ble.h"
#include "ble.h"
#include "terrarium_app.h"
#include "terrarium.h"
#include "stm32_seq.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

typedef enum
{
  Temperatura_NOTIFICATION_OFF,
  Temperatura_NOTIFICATION_ON,
  Umidita_NOTIFICATION_OFF,
  Umidita_NOTIFICATION_ON,
  Ventola_NOTIFICATION_OFF,
  Ventola_NOTIFICATION_ON,
  /* USER CODE BEGIN Service1_APP_SendInformation_t */

  /* USER CODE END Service1_APP_SendInformation_t */
  TERRARIUM_APP_SENDINFORMATION_LAST
} TERRARIUM_APP_SendInformation_t;

typedef struct
{
  TERRARIUM_APP_SendInformation_t     Temperatura_Notification_Status;
  TERRARIUM_APP_SendInformation_t     Umidita_Notification_Status;
  TERRARIUM_APP_SendInformation_t     Ventola_Notification_Status;
  /* USER CODE BEGIN Service1_APP_Context_t */

  /* USER CODE END Service1_APP_Context_t */
  uint16_t              ConnectionHandle;
} TERRARIUM_APP_Context_t;

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
static TERRARIUM_APP_Context_t TERRARIUM_APP_Context;

uint8_t a_TERRARIUM_UpdateCharData[247];

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void TERRARIUM_Temperatura_SendNotification(void);
static void TERRARIUM_Umidita_SendNotification(void);
static void TERRARIUM_Ventola_SendNotification(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void TERRARIUM_Notification(TERRARIUM_NotificationEvt_t *p_Notification)
{
  /* USER CODE BEGIN Service1_Notification_1 */

  /* USER CODE END Service1_Notification_1 */
  switch(p_Notification->EvtOpcode)
  {
    /* USER CODE BEGIN Service1_Notification_Service1_EvtOpcode */

    /* USER CODE END Service1_Notification_Service1_EvtOpcode */

    case TERRARIUM_TEMPERATURA_READ_EVT:
      /* USER CODE BEGIN Service1Char1_READ_EVT */

      /* USER CODE END Service1Char1_READ_EVT */
      break;

    case TERRARIUM_TEMPERATURA_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN Service1Char1_NOTIFY_ENABLED_EVT */
      TERRARIUM_APP_Context.Temperatura_Notification_Status = Temperatura_NOTIFICATION_ON;
      /* USER CODE END Service1Char1_NOTIFY_ENABLED_EVT */
      break;

    case TERRARIUM_TEMPERATURA_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN Service1Char1_NOTIFY_DISABLED_EVT */
      TERRARIUM_APP_Context.Temperatura_Notification_Status = Temperatura_NOTIFICATION_ON;
      /* USER CODE END Service1Char1_NOTIFY_DISABLED_EVT */
      break;

    case TERRARIUM_UMIDITA_READ_EVT:
      /* USER CODE BEGIN Service1Char2_READ_EVT */

      /* USER CODE END Service1Char2_READ_EVT */
      break;

    case TERRARIUM_UMIDITA_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN Service1Char2_NOTIFY_ENABLED_EVT */
      TERRARIUM_APP_Context.Umidita_Notification_Status = Umidita_NOTIFICATION_ON;
      /* USER CODE END Service1Char2_NOTIFY_ENABLED_EVT */
      break;

    case TERRARIUM_UMIDITA_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN Service1Char2_NOTIFY_DISABLED_EVT */
      TERRARIUM_APP_Context.Umidita_Notification_Status = Umidita_NOTIFICATION_OFF;
      /* USER CODE END Service1Char2_NOTIFY_DISABLED_EVT */
      break;

    case TERRARIUM_VENTOLA_READ_EVT:
      /* USER CODE BEGIN Service1Char3_READ_EVT */

      /* USER CODE END Service1Char3_READ_EVT */
      break;

    case TERRARIUM_VENTOLA_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN Service1Char3_NOTIFY_ENABLED_EVT */
      TERRARIUM_APP_Context.Ventola_Notification_Status = Ventola_NOTIFICATION_ON;
      /* USER CODE END Service1Char3_NOTIFY_ENABLED_EVT */
      break;

    case TERRARIUM_VENTOLA_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN Service1Char3_NOTIFY_DISABLED_EVT */
      TERRARIUM_APP_Context.Ventola_Notification_Status = Ventola_NOTIFICATION_OFF;
      /* USER CODE END Service1Char3_NOTIFY_DISABLED_EVT */
      break;

    default:
      /* USER CODE BEGIN Service1_Notification_default */

      /* USER CODE END Service1_Notification_default */
      break;
  }
  /* USER CODE BEGIN Service1_Notification_2 */

  /* USER CODE END Service1_Notification_2 */
  return;
}

void TERRARIUM_APP_EvtRx(TERRARIUM_APP_ConnHandleNotEvt_t *p_Notification)
{
  /* USER CODE BEGIN Service1_APP_EvtRx_1 */

  /* USER CODE END Service1_APP_EvtRx_1 */

  switch(p_Notification->EvtOpcode)
  {
    /* USER CODE BEGIN Service1_APP_EvtRx_Service1_EvtOpcode */

    /* USER CODE END Service1_APP_EvtRx_Service1_EvtOpcode */
    case TERRARIUM_CONN_HANDLE_EVT :
      TERRARIUM_APP_Context.ConnectionHandle = p_Notification->ConnectionHandle;
      /* USER CODE BEGIN Service1_APP_CENTR_CONN_HANDLE_EVT */

      /* USER CODE END Service1_APP_CENTR_CONN_HANDLE_EVT */
      break;
    case TERRARIUM_DISCON_HANDLE_EVT :
      TERRARIUM_APP_Context.ConnectionHandle = 0xFFFF;
      /* USER CODE BEGIN Service1_APP_DISCON_HANDLE_EVT */

      /* USER CODE END Service1_APP_DISCON_HANDLE_EVT */
      break;

    default:
      /* USER CODE BEGIN Service1_APP_EvtRx_default */

      /* USER CODE END Service1_APP_EvtRx_default */
      break;
  }

  /* USER CODE BEGIN Service1_APP_EvtRx_2 */

  /* USER CODE END Service1_APP_EvtRx_2 */

  return;
}

void TERRARIUM_APP_Init(void)
{
  TERRARIUM_APP_Context.ConnectionHandle = 0xFFFF;
  TERRARIUM_Init();

  /* USER CODE BEGIN Service1_APP_Init */

  /* USER CODE END Service1_APP_Init */
  return;
}

/* USER CODE BEGIN FD */

// funzioni custom che aggiornano i valori e li mandano come notifica

void Notifica_Temperatura_BLE(float temp_gradi)
{
  if (TERRARIUM_APP_Context.Temperatura_Notification_Status == Temperatura_NOTIFICATION_ON &&
      TERRARIUM_APP_Context.ConnectionHandle != 0xFFFF) // 0xFFFF valore default stato disconnesso
  {
    uint16_t temp_raw = (uint16_t)(temp_gradi * 10); // possiamo solo mandare array di byte
    TERRARIUM_Data_t data; // definita da cubeMX nell'header
    data.p_Payload = (uint8_t*)&temp_raw;
    data.Length = sizeof(temp_raw);

    TERRARIUM_NotifyValue(TERRARIUM_TEMPERATURA, &data, TERRARIUM_APP_Context.ConnectionHandle);
  }
}

void Notifica_Umidita_BLE(float umidita_percent)
{
  if (TERRARIUM_APP_Context.Umidita_Notification_Status == Umidita_NOTIFICATION_ON &&
      TERRARIUM_APP_Context.ConnectionHandle != 0xFFFF)
  {
    uint16_t umidita_raw = (uint16_t)(umidita_percent * 10);
    TERRARIUM_Data_t data;
    data.p_Payload = (uint8_t*)&umidita_raw;
    data.Length = sizeof(umidita_raw);

    TERRARIUM_NotifyValue(TERRARIUM_UMIDITA, &data, TERRARIUM_APP_Context.ConnectionHandle);
  }
}

void Notifica_Ventola_BLE(uint8_t stato_ventola)
{
  if (TERRARIUM_APP_Context.Ventola_Notification_Status == Ventola_NOTIFICATION_ON &&
      TERRARIUM_APP_Context.ConnectionHandle != 0xFFFF)
  {
    TERRARIUM_Data_t data;
    data.p_Payload = &stato_ventola;
    data.Length = sizeof(stato_ventola);

    TERRARIUM_NotifyValue(TERRARIUM_VENTOLA, &data, TERRARIUM_APP_Context.ConnectionHandle);
  }
}

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
__USED void TERRARIUM_Temperatura_SendNotification(void) /* Property Notification */
{
  TERRARIUM_APP_SendInformation_t notification_on_off = Temperatura_NOTIFICATION_OFF;
  TERRARIUM_Data_t terrarium_notification_data;

  terrarium_notification_data.p_Payload = (uint8_t*)a_TERRARIUM_UpdateCharData;
  terrarium_notification_data.Length = 0;

  /* USER CODE BEGIN Service1Char1_NS_1*/
  notification_on_off = TERRARIUM_APP_Context.Temperatura_Notification_Status;
  /* USER CODE END Service1Char1_NS_1*/

  if (notification_on_off != Temperatura_NOTIFICATION_OFF && TERRARIUM_APP_Context.ConnectionHandle != 0xFFFF)
  {
    TERRARIUM_NotifyValue(TERRARIUM_TEMPERATURA, &terrarium_notification_data, TERRARIUM_APP_Context.ConnectionHandle);
  }

  /* USER CODE BEGIN Service1Char1_NS_Last*/

  /* USER CODE END Service1Char1_NS_Last*/

  return;
}

__USED void TERRARIUM_Umidita_SendNotification(void) /* Property Notification */
{
  TERRARIUM_APP_SendInformation_t notification_on_off = Umidita_NOTIFICATION_OFF;
  TERRARIUM_Data_t terrarium_notification_data;

  terrarium_notification_data.p_Payload = (uint8_t*)a_TERRARIUM_UpdateCharData;
  terrarium_notification_data.Length = 0;

  /* USER CODE BEGIN Service1Char2_NS_1*/
  notification_on_off = TERRARIUM_APP_Context.Umidita_Notification_Status;
  /* USER CODE END Service1Char2_NS_1*/

  if (notification_on_off != Umidita_NOTIFICATION_OFF && TERRARIUM_APP_Context.ConnectionHandle != 0xFFFF)
  {
    TERRARIUM_NotifyValue(TERRARIUM_UMIDITA, &terrarium_notification_data, TERRARIUM_APP_Context.ConnectionHandle);
  }

  /* USER CODE BEGIN Service1Char2_NS_Last*/

  /* USER CODE END Service1Char2_NS_Last*/

  return;
}

__USED void TERRARIUM_Ventola_SendNotification(void) /* Property Notification */
{
  TERRARIUM_APP_SendInformation_t notification_on_off = Ventola_NOTIFICATION_OFF;
  TERRARIUM_Data_t terrarium_notification_data;

  terrarium_notification_data.p_Payload = (uint8_t*)a_TERRARIUM_UpdateCharData;
  terrarium_notification_data.Length = 0;

  /* USER CODE BEGIN Service1Char3_NS_1*/
  notification_on_off = TERRARIUM_APP_Context.Ventola_Notification_Status;
  /* USER CODE END Service1Char3_NS_1*/

  if (notification_on_off != Ventola_NOTIFICATION_OFF && TERRARIUM_APP_Context.ConnectionHandle != 0xFFFF)
  {
    TERRARIUM_NotifyValue(TERRARIUM_VENTOLA, &terrarium_notification_data, TERRARIUM_APP_Context.ConnectionHandle);
  }

  /* USER CODE BEGIN Service1Char3_NS_Last*/

  /* USER CODE END Service1Char3_NS_Last*/

  return;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/

/* USER CODE END FD_LOCAL_FUNCTIONS*/
