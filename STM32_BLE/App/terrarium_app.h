/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Terrarium_app.h
  * @author  MCD Application Team
  * @brief   Header for Terrarium_app.c
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
#ifndef TERRARIUM_APP_H
#define TERRARIUM_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  TERRARIUM_CONN_HANDLE_EVT,
  TERRARIUM_DISCON_HANDLE_EVT,

  /* USER CODE BEGIN Service1_OpcodeNotificationEvt_t */

  /* USER CODE END Service1_OpcodeNotificationEvt_t */

  TERRARIUM_LAST_EVT,
} TERRARIUM_APP_OpcodeNotificationEvt_t;

typedef struct
{
  TERRARIUM_APP_OpcodeNotificationEvt_t          EvtOpcode;
  uint16_t                                 ConnectionHandle;

  /* USER CODE BEGIN TERRARIUM_APP_ConnHandleNotEvt_t */

  /* USER CODE END TERRARIUM_APP_ConnHandleNotEvt_t */
} TERRARIUM_APP_ConnHandleNotEvt_t;
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
void Notifica_Temperatura_BLE(float temp_gradi);
void Notifica_Umidita_BLE(float umidita_percent);
void Notifica_Ventola_BLE(uint8_t stato_ventola);
/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros -----------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions ------------------------------------------------------- */
void TERRARIUM_APP_Init(void);
void TERRARIUM_APP_EvtRx(TERRARIUM_APP_ConnHandleNotEvt_t *p_Notification);
/* USER CODE BEGIN EF */

/* USER CODE END EF */

#ifdef __cplusplus
}
#endif

#endif /*TERRARIUM_APP_H */
