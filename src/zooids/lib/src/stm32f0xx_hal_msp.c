/**
  ******************************************************************************
  * @file    Templates/Src/stm32f0xx_hal_msp.c
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    26-June-2015
  * @brief   HAL MSP module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bbzmain.h"
#include "config.h"

/** @addtogroup STM32F0xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static DMA_HandleTypeDef hdma_tx;
static DMA_HandleTypeDef hdma_rx;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief I2C MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for transmission request by peripheral
  *           - NVIC configuration for DMA interrupt request enable
  * @param hi2c: I2C handle pointer
  * @retval None
  */
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
  static DMA_HandleTypeDef hdma_tx;
  static DMA_HandleTypeDef hdma_rx;

  GPIO_InitTypeDef  GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  I2Cx_SCL_GPIO_CLK_ENABLE();
  I2Cx_SDA_GPIO_CLK_ENABLE();
  /* Enable I2C2 clock */
  I2Cx_CLK_ENABLE();
  /* Enable DMA1 clock */
  DMAx_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* I2C TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = I2Cx_SCL_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = I2Cx_SCL_AF;

  HAL_GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStruct);

  /* I2C RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = I2Cx_SDA_PIN;
  GPIO_InitStruct.Alternate = I2Cx_SDA_AF;

  HAL_GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the DMA ##################################################*/
  /* Configure the DMA handler for Transmission process */
  hdma_tx.Instance                 = I2Cx_TX_DMA_INSTANCE;

  hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  hdma_tx.Init.Mode                = DMA_NORMAL;
  hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;

  HAL_DMA_Init(&hdma_tx);

  /* Associate the initialized DMA handle to the the I2C handle */
  __HAL_LINKDMA(hi2c, hdmatx, hdma_tx);

  /* Configure the DMA handler for Transmission process */
  hdma_rx.Instance                 = I2Cx_RX_DMA_INSTANCE;

  hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  hdma_rx.Init.Mode                = DMA_NORMAL;
  hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;

  HAL_DMA_Init(&hdma_rx);

  /* Associate the initialized DMA handle to the the I2C handle */
  __HAL_LINKDMA(hi2c, hdmarx, hdma_rx);

  /*##-4- Configure the NVIC for DMA #########################################*/
  /* NVIC configuration for DMA transfer complete interrupt (I2C1_TX) */
  HAL_NVIC_SetPriority(I2Cx_DMA_TX_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(I2Cx_DMA_TX_IRQn);

  /* NVIC configuration for DMA transfer complete interrupt (I2C1_RX) */
  HAL_NVIC_SetPriority(I2Cx_DMA_RX_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(I2Cx_DMA_RX_IRQn);
}

/**
  * @brief I2C MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param hi2c: I2C handle pointer
  * @retval None
  */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{

  static DMA_HandleTypeDef hdma_tx;
  static DMA_HandleTypeDef hdma_rx;

  /*##-1- Reset peripherals ##################################################*/
  I2Cx_FORCE_RESET();
  I2Cx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* Configure I2C Tx as alternate function  */
  HAL_GPIO_DeInit(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_PIN);
  /* Configure I2C Rx as alternate function  */
  HAL_GPIO_DeInit(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_PIN);

  /*##-3- Disable the DMA ####################################################*/
  /* De-Initialize the DMA associated to transmission process */
  HAL_DMA_DeInit(&hdma_tx);
  /* De-Initialize the DMA associated to reception process */
  HAL_DMA_DeInit(&hdma_rx);

  /*##-4- Disable the NVIC for DMA ###########################################*/
  HAL_NVIC_DisableIRQ(I2Cx_DMA_TX_IRQn);
  HAL_NVIC_DisableIRQ(I2Cx_DMA_RX_IRQn);
}


void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    if (hspi->Instance == SPIx)
    {
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* Enable GPIO TX/RX clock */

        SPIx_SCK_GPIO_CLK_ENABLE();
        SPIx_MISO_GPIO_CLK_ENABLE();
        SPIx_MOSI_GPIO_CLK_ENABLE();
        SPIx_CSN_GPIO_CLK_ENABLE();
        SPIx_CE_GPIO_CLK_ENABLE();
        /* Enable SPI clock */
        SPIx_CLK_ENABLE();
        /* Enable DMA1 clock */
        DMAx_CLK_ENABLE();

        /*##-2- Configure peripheral GPIO ##########################################*/
        /* SPI SCK GPIO pin configuration  */
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pin = SPIx_SCK_PIN;
        GPIO_InitStruct.Alternate = SPIx_SCK_AF;
        HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

        /* SPI MISO GPIO pin configuration  */
        GPIO_InitStruct.Pin = SPIx_MISO_PIN;
        GPIO_InitStruct.Alternate = SPIx_MISO_AF;
        HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);

        /* SPI MOSI GPIO pin configuration  */
        GPIO_InitStruct.Pin = SPIx_MOSI_PIN;
        GPIO_InitStruct.Alternate = SPIx_MOSI_AF;
        HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);

        /* SPI CE GPIO pin configuration  */
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pin = SPIx_CE_PIN;
        HAL_GPIO_Init(SPIx_CE_GPIO_PORT, &GPIO_InitStruct);

        /* SPI CSN GPIO pin configuration  */
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pin = SPIx_CSN_PIN;
        HAL_GPIO_Init(SPIx_CSN_GPIO_PORT, &GPIO_InitStruct);

        /*  RADIO_IRQn pin configuration  */
        GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Pin = RADIO_IRQ_PIN;
        HAL_GPIO_Init(RADIO_IRQ_GPIO_PORT, &GPIO_InitStruct);

        /* Enable and set EXTI line Interrupt to the lowest priority */
        HAL_NVIC_SetPriority(RADIO_IRQn, RADIO_IRQ_PRIORITY, 0);
        HAL_NVIC_EnableIRQ(RADIO_IRQn);

        /*##-3- Configure the DMA ##################################################*/
        /* Configure the DMA handler for Transmission process */
        hdma_tx.Instance = SPIx_TX_DMA_STREAM;
        hdma_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_tx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_tx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        hdma_tx.Init.Mode = DMA_NORMAL;
        hdma_tx.Init.Priority = DMA_PRIORITY_LOW;

        HAL_DMA_Init(&hdma_tx);

        /* Associate the initialized DMA handle to the the SPI handle */
        __HAL_LINKDMA(hspi, hdmatx, hdma_tx);

        /* Configure the DMA handler for Transmission process */
        hdma_rx.Instance = SPIx_RX_DMA_STREAM;

        hdma_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_tx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        hdma_rx.Init.Mode = DMA_NORMAL;
        hdma_rx.Init.Priority = DMA_PRIORITY_HIGH;

        HAL_DMA_Init(&hdma_rx);

        /* Associate the initialized DMA handle to the the SPI handle */
        __HAL_LINKDMA(hspi, hdmarx, hdma_rx);

        /*##-4- Configure the NVIC for DMA #########################################*/
        /* NVIC configuration for DMA transfer complete interrupt (SPI3_TX) */
        HAL_NVIC_SetPriority(SPIx_DMA_TX_IRQn, 1, 1);
        HAL_NVIC_EnableIRQ(SPIx_DMA_TX_IRQn);

        /* NVIC configuration for DMA transfer complete interrupt (SPI3_RX) */
        HAL_NVIC_SetPriority(SPIx_DMA_RX_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(SPIx_DMA_RX_IRQn);
    }
}

/**
  * @brief SPI MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO configuration to its default state
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPIx)
    {
        /*##-1- Reset peripherals ##################################################*/
        SPIx_FORCE_RESET();
        SPIx_RELEASE_RESET();

        /*##-2- Disable peripherals and GPIO Clocks ################################*/
        /* Configure SPI SCK as alternate function  */
        HAL_GPIO_DeInit(SPIx_SCK_GPIO_PORT, SPIx_SCK_PIN);
        /* Configure SPI MISO as alternate function  */
        HAL_GPIO_DeInit(SPIx_MISO_GPIO_PORT, SPIx_MISO_PIN);
        /* Configure SPI MOSI as alternate function  */
        HAL_GPIO_DeInit(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_PIN);

        /*##-3- Disable the DMA ####################################################*/
        /* De-Initialize the DMA associated to transmission process */
        HAL_DMA_DeInit(&hdma_tx);
        /* De-Initialize the DMA associated to reception process */
        HAL_DMA_DeInit(&hdma_rx);

        /*##-4- Disable the NVIC for DMA ###########################################*/
        HAL_NVIC_DisableIRQ(SPIx_DMA_TX_IRQn);
        HAL_NVIC_DisableIRQ(SPIx_DMA_RX_IRQn);
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx Peripheral clock enable */
  MICRO_TIM_CLK_ENABLE();
  
  /*##-2- Configure the NVIC for TIMx ########################################*/
  /* Set the TIMx priority */
  HAL_NVIC_SetPriority(MICRO_IRQn, MICRO_IRQ_PRIORITY, 0);

  /* Enable the TIMx global Interrupt */
  HAL_NVIC_EnableIRQ(MICRO_IRQn);
}


/**
* @brief  ADC MSP Init
* @param  hadc : ADC handle
* @retval None
*/
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    static DMA_HandleTypeDef DmaHandle;

    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* ADC1 Periph clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();
    /* Enable DMA1 clock */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /*##- 3- Configure DMA #####################################################*/

    /*********************** Configure DMA parameters ***************************/
    DmaHandle.Instance = DMA1_Channel1;
    DmaHandle.Init.Direction = DMA_PERIPH_TO_MEMORY;
    DmaHandle.Init.PeriphInc = DMA_PINC_DISABLE;
    DmaHandle.Init.MemInc = DMA_MINC_ENABLE;
    DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    DmaHandle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    DmaHandle.Init.Mode = DMA_CIRCULAR;
    DmaHandle.Init.Priority = DMA_PRIORITY_LOW;

    /* Deinitialize  & Initialize the DMA for new transfer */
    HAL_DMA_DeInit(&DmaHandle);
    HAL_DMA_Init(&DmaHandle);

    /* Associate the DMA handle */
    __HAL_LINKDMA(hadc, DMA_Handle, DmaHandle);

    /* NVIC configuration for DMA Input data interrupt */
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}