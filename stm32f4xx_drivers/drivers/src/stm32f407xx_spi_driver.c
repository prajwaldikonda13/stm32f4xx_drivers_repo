/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: Feb 9, 2019
 *      Author: admin
 */

#include "stm32f407xx_spi_driver.h"

static void  spi_txe_interrupt_handle(SPI_Handle_t SPIHandle);
static void  spi_rxne_interrupt_handle(SPI_Handle_t SPIHandle);
static void  spi_ovr_err_interrupt_handle(SPI_Handle_t SPIHandle);

/*********************************************************************
 * @fn      		  - SPI_PeriClockControl
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void SPI_PeriClockControl(uint8_t SPIx, uint8_t EnorDi)
{

	if(EnorDi == ENABLE)
	{
		if(SPIx == 0)
		{
			SPI1_PCLK_EN();
		}else if (SPIx == 0)
		{
			SPI2_PCLK_EN();
		}else if (SPIx == 0)
		{
			SPI3_PCLK_EN();
		}
	}
	else
	{
		//TODO
	}
}


/*********************************************************************
 * @fn      		  - SPI_Init
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void SPI_Init(SPI_Handle_t SPIHandle)
{

	//peripheral clock enable

	SPI_PeriClockControl(SPIHandle.SPIx, ENABLE);

	//first lets configure the SPI_CR1 register

	uint32_t tempreg = 0;

	//1. configure the device mode
	tempreg |= SPIHandle.SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR ;

	//2. Configure the bus config
	if(SPIHandle.SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		//bidi mode should be cleared
		tempreg &= ~( 1 << SPI_CR1_BIDIMODE);

	}else if (SPIHandle.SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		//bidi mode should be set
		tempreg |= ( 1 << SPI_CR1_BIDIMODE);
	}else if (SPIHandle.SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		//BIDI mode should be cleared
		tempreg &= ~( 1 << SPI_CR1_BIDIMODE);
		//RXONLY bit must be set
		tempreg |= ( 1 << SPI_CR1_RXONLY);
	}

	// 3. Configure the spi serial clock speed (baud rate)
	tempreg |= SPIHandle.SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;

	//4.  Configure the DFF
	tempreg |= SPIHandle.SPIConfig.SPI_DFF << SPI_CR1_DFF;

	//5. configure the CPOL
	tempreg |= SPIHandle.SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

	//6 . configure the CPHA
	tempreg |= SPIHandle.SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	tempreg |= SPIHandle.SPIConfig.SPI_SSM << SPI_CR1_SSM;

	SPIx_ptr(SPIHandle.SPIx)->CR1 = tempreg;

}

/*********************************************************************
 * @fn      		  - SPI_DeInit
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void SPI_DeInit(uint8_t SPIx)
{
 //todo
}

uint8_t SPI_GetFlagStatus(uint8_t SPIx , uint32_t FlagName)
{
	if(SPIx_ptr(SPIx)->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/*********************************************************************
 * @fn      		  - SPI_SendData
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              - This is blocking call

 */
void SPI_SendData(uint8_t SPIx,uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		//1. wait until TXE is set
		while(SPI_GetFlagStatus(SPIx,SPI_TXE_FLAG)  == FLAG_RESET );

		//2. check the DFF bit in CR1
		if( (SPIx_ptr(SPIx)->CR1 & ( 1 << SPI_CR1_DFF) ) )
		{
			//16 bit DFF
			//1. load the data in to the DR
			SPIx_ptr(SPIx)->DR =   *((uint16_t*)pTxBuffer);
			Len--;
			Len--;
			(uint16_t*)pTxBuffer++;
		}else
		{
			//8 bit DFF
			SPIx_ptr(SPIx)->DR =   *pTxBuffer;
			Len--;
			pTxBuffer++;
		}
	}

}

/*********************************************************************
 * @fn      		  - SPI_ReceiveData
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void SPI_ReceiveData(uint8_t SPIx, uint8_t *pRxBuffer, uint32_t Len)
{
	while(Len > 0)
		{
			//1. wait until RXNE is set
			while(SPI_GetFlagStatus(SPIx,SPI_RXNE_FLAG)  == (uint8_t)FLAG_RESET );

			//2. check the DFF bit in CR1
			if( (SPIx_ptr(SPIx)->CR1 & ( 1 << SPI_CR1_DFF) ) )
			{
				//16 bit DFF
				//1. load the data from DR to Rxbuffer address
				 *((uint16_t*)pRxBuffer) = SPIx_ptr(SPIx)->DR ;
				Len--;
				Len--;
				(uint16_t*)pRxBuffer++;
			}else
			{
				//8 bit DFF
				*(pRxBuffer) = SPIx_ptr(SPIx)->DR ;
				Len--;
				pRxBuffer++;
			}
		}

}


/*********************************************************************
 * @fn      		  - SPI_PeripheralControl
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void SPI_PeripheralControl(uint8_t SPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		SPIx_ptr(SPIx)->CR1 |=  (1 << SPI_CR1_SPE);
	}else
	{
		SPIx_ptr(SPIx)->CR1 &=  ~(1 << SPI_CR1_SPE);
	}


}


/*********************************************************************
 * @fn      		  - SPI_SSIConfig
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void  SPI_SSIConfig(uint8_t SPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		SPIx_ptr(SPIx)->CR1 |=  (1 << SPI_CR1_SSI);
	}else
	{
		SPIx_ptr(SPIx)->CR1 &=  ~(1 << SPI_CR1_SSI);
	}


}


/*********************************************************************
 * @fn      		  - SPI_SSOEConfig
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void  SPI_SSOEConfig(uint8_t SPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		SPIx_ptr(SPIx)->CR2 |=  (1 << SPI_CR2_SSOE);
	}else
	{
		SPIx_ptr(SPIx)->CR2 &=  ~(1 << SPI_CR2_SSOE);
	}


}



/*********************************************************************
 * @fn      		  - SPI_IRQInterruptConfig
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{

//	if(EnorDi == ENABLE)
//	{
//		if(IRQNumber <= 31)
//		{
//			//program ISER0 register
//			*NVIC_ISER0 |= ( 1 << IRQNumber );
//
//		}else if(IRQNumber > 31 && IRQNumber < 64 ) //32 to 63
//		{
//			//program ISER1 register
//			*NVIC_ISER1 |= ( 1 << (IRQNumber % 32) );
//		}
//		else if(IRQNumber >= 64 && IRQNumber < 96 )
//		{
//			//program ISER2 register //64 to 95
//			*NVIC_ISER3 |= ( 1 << (IRQNumber % 64) );
//		}
//	}else
//	{
//		if(IRQNumber <= 31)
//		{
//			//program ICER0 register
//			*NVIC_ICER0 |= ( 1 << IRQNumber );
//		}else if(IRQNumber > 31 && IRQNumber < 64 )
//		{
//			//program ICER1 register
//			*NVIC_ICER1 |= ( 1 << (IRQNumber % 32) );
//		}
//		else if(IRQNumber >= 6 && IRQNumber < 96 )
//		{
//			//program ICER2 register
//			*NVIC_ICER3 |= ( 1 << (IRQNumber % 64) );
//		}
//	}

}


/*********************************************************************
 * @fn      		  - SPI_IRQPriorityConfig
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	//1. first lets find out the ipr register
//	uint8_t iprx = IRQNumber / 4;
//	uint8_t iprx_section  = IRQNumber %4 ;
//
//	uint8_t shift_amount = ( 8 * iprx_section) + ( 8 - NO_PR_BITS_IMPLEMENTED) ;
//
//	*(  NVIC_PR_BASE_ADDR + iprx ) |=  ( IRQPriority << shift_amount );

}


uint8_t SPI_SendDataIT(SPI_Handle_t SPIHandle,uint8_t *pTxBuffer, uint32_t Len)
{
	uint8_t state = SPIHandle.TxState;

	if(state != SPI_BUSY_IN_TX)
	{
		//1 . Save the Tx buffer address and Len information in some global variables
		SPIHandle.pTxBuffer = pTxBuffer;
		SPIHandle.TxLen = Len;
		//2.  Mark the SPI state as busy in transmission so that
		//    no other code can take over same SPI peripheral until transmission is over
		SPIHandle.TxState = SPI_BUSY_IN_TX;

		//3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
		SPIx_ptr(SPIHandle.SPIx)->CR2 |= ( 1 << SPI_CR2_TXEIE );

	}


	return state;
}


uint8_t SPI_ReceiveDataIT(SPI_Handle_t SPIHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	uint8_t state = SPIHandle.RxState;

	if(state != SPI_BUSY_IN_RX)
	{
		//1 . Save the Rx buffer address and Len information in some global variables
		SPIHandle.pRxBuffer = pRxBuffer;
		SPIHandle.RxLen = Len;
		//2.  Mark the SPI state as busy in reception so that
		//    no other code can take over same SPI peripheral until reception is over
		SPIHandle.RxState = SPI_BUSY_IN_RX;

		//3. Enable the RXNEIE control bit to get interrupt whenever RXNEIE flag is set in SR
		SPIx_ptr(SPIHandle.SPIx)->CR2 |= ( 1 << SPI_CR2_RXNEIE );

	}


	return state;

}





void SPI_IRQHandling(SPI_Handle_t SPIHandle)
{

	uint8_t temp1 , temp2;
	//first lets check for TXE
	temp1 = SPIx_ptr(SPIHandle.SPIx)->SR & ( 1 << SPI_SR_TXE);
	temp2 = SPIx_ptr(SPIHandle.SPIx)->CR2 & ( 1 << SPI_CR2_TXEIE);

	if( temp1 && temp2)
	{
		//handle TXE
		spi_txe_interrupt_handle(SPIHandle);
	}

	// check for RXNE
	temp1 = SPIx_ptr(SPIHandle.SPIx)->SR & ( 1 << SPI_SR_RXNE);
	temp2 = SPIx_ptr(SPIHandle.SPIx)->CR2 & ( 1 << SPI_CR2_RXNEIE);

	if( temp1 && temp2)
	{
		//handle RXNE
		spi_rxne_interrupt_handle(SPIHandle);
	}

	// check for ovr flag
	temp1 = SPIx_ptr(SPIHandle.SPIx)->SR & ( 1 << SPI_SR_OVR);
	temp2 = SPIx_ptr(SPIHandle.SPIx)->CR2 & ( 1 << SPI_CR2_ERRIE);

	if( temp1 && temp2)
	{
		//handle ovr error
		spi_ovr_err_interrupt_handle(SPIHandle);
	}


}


//some helper function implementations

static void  spi_txe_interrupt_handle(SPI_Handle_t SPIHandle)
{
	// check the DFF bit in CR1
	if( (SPIx_ptr(SPIHandle.SPIx)->CR1 & ( 1 << SPI_CR1_DFF) ) )
	{
		//16 bit DFF
		//1. load the data in to the DR
		SPIx_ptr(SPIHandle.SPIx)->DR =   *((uint16_t*)SPIHandle.pTxBuffer);
		SPIHandle.TxLen--;
		SPIHandle.TxLen--;
		(uint16_t*)SPIHandle.pTxBuffer++;
	}else
	{
		//8 bit DFF
		SPIx_ptr(SPIHandle.SPIx)->DR =   *SPIHandle.pTxBuffer;
		SPIHandle.TxLen--;
		SPIHandle.pTxBuffer++;
	}

	if(! SPIHandle.TxLen)
	{
		//TxLen is zero , so close the spi transmission and inform the application that
		//TX is over.

		//this prevents interrupts from setting up of TXE flag
		SPI_CloseTransmisson(SPIHandle);
		SPI_ApplicationEventCallback(SPIHandle,SPI_EVENT_TX_CMPLT);
	}

}


static void  spi_rxne_interrupt_handle(SPI_Handle_t SPIHandle)
{
	//do rxing as per the dff
	if(SPIx_ptr(SPIHandle.SPIx)->CR1 & ( 1 << 11))
	{
		//16 bit
		*((uint16_t*)SPIHandle.pRxBuffer) = (uint16_t)SPIx_ptr(SPIHandle.SPIx)->DR;
		SPIHandle.RxLen -= 2;
		SPIHandle.pRxBuffer++;
		SPIHandle.pRxBuffer++;

	}else
	{
		//8 bit
		*(SPIHandle.pRxBuffer) = (uint8_t) SPIx_ptr(SPIHandle.SPIx)->DR;
		SPIHandle.RxLen--;
		SPIHandle.pRxBuffer++;
	}

	if(! SPIHandle.RxLen)
	{
		//reception is complete
		SPI_CloseReception(SPIHandle);
		SPI_ApplicationEventCallback(SPIHandle,SPI_EVENT_RX_CMPLT);
	}

}


static void  spi_ovr_err_interrupt_handle(SPI_Handle_t SPIHandle)
{

	uint8_t temp;
	//1. clear the ovr flag
	if(SPIHandle.TxState != SPI_BUSY_IN_TX)
	{
		temp = SPIx_ptr(SPIHandle.SPIx)->DR;
		temp = SPIx_ptr(SPIHandle.SPIx)->SR;
	}
	(void)temp;
	//2. inform the application
	SPI_ApplicationEventCallback(SPIHandle,SPI_EVENT_OVR_ERR);

}


void SPI_CloseTransmisson(SPI_Handle_t SPIHandle)
{
	SPIx_ptr(SPIHandle.SPIx)->CR2 &= ~( 1 << SPI_CR2_TXEIE);
	SPIHandle.pTxBuffer = NULL;
	SPIHandle.TxLen = 0;
	SPIHandle.TxState = SPI_READY;

}

void SPI_CloseReception(SPI_Handle_t SPIHandle)
{
	SPIx_ptr(SPIHandle.SPIx)->CR2 &= ~( 1 << SPI_CR2_RXNEIE);
	SPIHandle.pRxBuffer = NULL;
	SPIHandle.RxLen = 0;
	SPIHandle.RxState = SPI_READY;

}



void SPI_ClearOVRFlag(uint8_t SPIx)
{
	uint8_t temp;
	temp = SPIx_ptr(SPIx)->DR;
	temp = SPIx_ptr(SPIx)->SR;
	(void)temp;

}



__weak void SPI_ApplicationEventCallback(SPI_Handle_t SPIHandle,uint8_t AppEv)
{

	//This is a weak implementation . the user application may override this function.
}







