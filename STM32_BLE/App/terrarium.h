/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Terrarium.h
  * @author  MCD Application Team
  * @brief   Header for Terrarium.c
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TERRARIUM_H
#define TERRARIUM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ble_status.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported defines ----------------------------------------------------------*/
/* USER CODE BEGIN ED */

/* USER CODE END ED */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  TERRARIUM_TEMPERATURA,
  TERRARIUM_UMIDITA,
  TERRARIUM_VENTOLA,

  /* USER CODE BEGIN Service1_CharOpcode_t */

  /* USER CODE END Service1_CharOpcode_t */

  TERRARIUM_CHAROPCODE_LAST
} TERRARIUM_CharOpcode_t;

typedef enum
{
  TERRARIUM_TEMPERATURA_READ_EVT,
  TERRARIUM_TEMPERATURA_NOTIFY_ENABLED_EVT,
  TERRARIUM_TEMPERATURA_NOTIFY_DISABLED_EVT,
  TERRARIUM_UMIDITA_READ_EVT,
  TERRARIUM_UMIDITA_NOTIFY_ENABLED_EVT,
  TERRARIUM_UMIDITA_NOTIFY_DISABLED_EVT,
  TERRARIUM_VENTOLA_READ_EVT,
  TERRARIUM_VENTOLA_NOTIFY_ENABLED_EVT,
  TERRARIUM_VENTOLA_NOTIFY_DISABLED_EVT,

  /* USER CODE BEGIN Service1_OpcodeEvt_t */

  /* USER CODE END Service1_OpcodeEvt_t */

  TERRARIUM_BOOT_REQUEST_EVT
} TERRARIUM_OpcodeEvt_t;

typedef struct
{
  uint8_t *p_Payload;
  uint8_t Length;

  /* USER CODE BEGIN Service1_Data_t */

  /* USER CODE END Service1_Data_t */

} TERRARIUM_Data_t;

typedef struct
{
  TERRARIUM_OpcodeEvt_t       EvtOpcode;
  TERRARIUM_Data_t             DataTransfered;
  uint16_t                ConnectionHandle;
  uint16_t                AttributeHandle;
  uint8_t                 ServiceInstance;

  /* USER CODE BEGIN Service1_NotificationEvt_t */

  /* USER CODE END Service1_NotificationEvt_t */

} TERRARIUM_NotificationEvt_t;

/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros -----------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions ------------------------------------------------------- */
void TERRARIUM_Init(void);
void TERRARIUM_Notification(TERRARIUM_NotificationEvt_t *p_Notification);
tBleStatus TERRARIUM_UpdateValue(TERRARIUM_CharOpcode_t CharOpcode, TERRARIUM_Data_t *pData);
tBleStatus TERRARIUM_NotifyValue(TERRARIUM_CharOpcode_t CharOpcode, TERRARIUM_Data_t *pData, uint16_t ConnectionHandle);
/* USER CODE BEGIN EF */

/* USER CODE END EF */

#ifdef __cplusplus
}
#endif

#endif /*TERRARIUM_H */
