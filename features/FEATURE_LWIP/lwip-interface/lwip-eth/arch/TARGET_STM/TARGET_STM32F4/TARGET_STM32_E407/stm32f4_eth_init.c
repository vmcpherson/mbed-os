
/*
  PA1/RMII_REF_CLK
  PA2/ETH_RMII_MDIO
  PC1/ETH_RMII_MDC
  PA7/D11/ETH_RMII_CRS_DV
  PC4/ETH_RMII_RXD0
  PC5/ETH_RMII_RXD1
  ..
  PG11/ETH_RMII_TX_EN
  PG13/ETH_RMII_TXD0
  PG14/ETH_RMII_TXD1
  PA3/ETH_RMII_MDINT
  PG6/EPHY-RST#


  TODO
  ====

  PA3 is the interrupt pin. IT is not configured, how should it be?
  

  PG6 is connected to the NRST pin of the PHY. It has a power-on reset circuit.
  Do we need to configure PG6 as a open collecter output and reset the PHY
  manually at any point?
*/




#include "stm32f4xx_hal.h"

/**
 * Override HAL Eth Init function
 */
void HAL_ETH_MspInit(ETH_HandleTypeDef* heth)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    if (heth->Instance == ETH) {

        /* Enable GPIOs clocks */
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();

        /*
          E407

          BOARD_PHY_ID                MII_LAN8710A_ID

          
          Alt function
          ============
          PA1/RMII_REF_CLK
          PA2/ETH_RMII_MDIO
          PC1/ETH_RMII_MDC
          PA7/D11/ETH_RMII_CRS_DV
          PC4/ETH_RMII_RXD0
          PC5/ETH_RMII_RXD1
          ..
          PG11/ETH_RMII_TX_EN
          PG13/ETH_RMII_TXD0
          PG14/ETH_RMII_TXD1

          Inputs - floating
          =================
          PA3/ETH_RMII_MDINT

          Inputs - pullup
          ===============
          PG6/EPHY-RST#
        */

        /* Configure PA1, PA2 and PA7
           
           PA1/RMII_REF_CLK
           PA2/ETH_RMII_MDIO
           PA7/D11/ETH_RMII_CRS_DV
        */
        GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStructure.Pull = GPIO_NOPULL; 
        GPIO_InitStructure.Alternate = GPIO_AF11_ETH;
        GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

        /* Configure PC1, PC4 and PC5
           
           PC1/ETH_RMII_MDC
           PA7/D11/ETH_RMII_CRS_DV
           PC4/ETH_RMII_RXD0
           PC5/ETH_RMII_RXD1
        */
        GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

        /* Configure PG11, PG13 and PG14

           PG11/ETH_RMII_TX_EN
           PG13/ETH_RMII_TXD0
           PG14/ETH_RMII_TXD1
         */
        GPIO_InitStructure.Pin =  GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

        /* Enable the Ethernet global Interrupt */
        HAL_NVIC_SetPriority(ETH_IRQn, 0x7, 0);
        HAL_NVIC_EnableIRQ(ETH_IRQn);
        
        /* Enable ETHERNET clock  */
        __HAL_RCC_ETH_CLK_ENABLE();
    }
}

/**
 * Override HAL Eth DeInit function
 */
void HAL_ETH_MspDeInit(ETH_HandleTypeDef* heth)
{
    if (heth->Instance == ETH) {
        /* Peripheral clock disable */
        __HAL_RCC_ETH_CLK_DISABLE();

        /** ETH GPIO Configuration

          PA1/RMII_REF_CLK
          PA2/ETH_RMII_MDIO
          PC1/ETH_RMII_MDC
          PA7/D11/ETH_RMII_CRS_DV
          PC4/ETH_RMII_RXD0
          PC5/ETH_RMII_RXD1
          ..
          PG11/ETH_RMII_TX_EN
          PG13/ETH_RMII_TXD0
          PG14/ETH_RMII_TXD1
         */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7);
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5);
        HAL_GPIO_DeInit(GPIOG, GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14);

        /* Disable the Ethernet global Interrupt */
        NVIC_DisableIRQ(ETH_IRQn);
    }
}
