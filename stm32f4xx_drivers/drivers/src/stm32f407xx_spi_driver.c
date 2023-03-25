#include "stm32f407xx_spi_driver.h"

static void spi_txe_interrupt_handle(SPI_Handle_t SPIHandle);
static void spi_rxne_interrupt_handle(SPI_Handle_t SPIHandle);
static void spi_ovr_err_interrupt_handle(SPI_Handle_t SPIHandle);

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
void SPI_PeriClockControl(uint8_t SPIx, uint8_t State) {
	if (SPIx == SPI1) {
		RCC_APB2_PER_CLK_CTRL(12, ENABLE);
	} else if (SPIx == SPI2) {
		RCC_APB1_PER_CLK_CTRL(14, ENABLE);
	} else if (SPIx == SPI3) {
		RCC_APB1_PER_CLK_CTRL(15, ENABLE);
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
	void SPI_Init(SPI_Handle_t SPIHandle) {

		//peripheral clock enable

		SPI_PeriClockControl(SPIHandle.SPIx, ENABLE);

		//first lets configure the SPI_CR1 register

		uint32_t tempreg = 0;

		//1. configure the device mode
		MODIFY_BIT_N(SPI_ptr(SPIHandle.SPIx)->CR1, SPI_CR1_MSTR,
				SPIHandle.SPIConfig.SPI_DeviceMode);

		//2. Configure the bus config
		if (SPIHandle.SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD) {
			//bidi mode should be cleared
			CLR_BIT_N(SPI_ptr(SPIHandle.SPIx)->CR1,SPI_CR1_BIDIMODE);
		} else if (SPIHandle.SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD) {
			//bidi mode should be set
			SET_BIT_N(SPI_ptr(SPIHandle.SPIx)->CR1, SPI_CR1_BIDIMODE);
		} else if (SPIHandle.SPIConfig.SPI_BusConfig
				== SPI_BUS_CONFIG_SIMPLEX_RXONLY) {
			//BIDI mode should be cleared
			CLR_BIT_N(SPI_ptr(SPIHandle.SPIx)->CR1,SPI_CR1_BIDIMODE);
			//RXONLY bit must be set
			SET_BIT_N(SPI_ptr(SPIHandle.SPIx)->CR1, SPI_CR1_RXONLY);
		}

		// 3. Configure the spi serial clock speed (baud rate)
		MODIFY_KBITS(SPI_ptr(SPIHandle.SPIx)->CR1, 3, SPI_CR1_BR,
				SPIHandle.SPIConfig.SPI_SclkSpeed);

		//4.  Configure the DFF
		MODIFY_BIT_N(SPI_ptr(SPIHandle.SPIx)->CR1, SPI_CR1_DFF,
				SPIHandle.SPIConfig.SPI_DFF);

		//5. configure the CPOL
		MODIFY_BIT_N(SPI_ptr(SPIHandle.SPIx)->CR1, SPI_CR1_CPOL,
				SPIHandle.SPIConfig.SPI_CPOL);

		//6 . configure the CPHA
		MODIFY_BIT_N(SPI_ptr(SPIHandle.SPIx)->CR1, SPI_CR1_CPHA,
				SPIHandle.SPIConfig.SPI_CPHA);

		MODIFY_BIT_N(SPI_ptr(SPIHandle.SPIx)->CR1, SPI_CR1_SSM,
				SPIHandle.SPIConfig.SPI_SSM);
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
	void SPI_DeInit(uint8_t SPIx) {
		//todo
	}

	uint8_t SPI_GetFlagStatus(uint8_t SPIx, uint32_t FlagName) {
		if (SPIx_ptr(SPIx)->SR & FlagName) {
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
	void SPI_SendData(uint8_t SPIx, uint8_t *pTxBuffer, uint32_t Len) {
		while (Len > 0) {
			//1. wait until TXE is set
			while (!GET_BIT_N(SPIx_ptr(SPIx)->SR, SPI_SR_TXE))
				;

			//2. check the DFF bit in CR1

			if (GET_BIT_N(SPIx_ptr(SPIx)->CR1, SPI_CR1_DFF)) {
				//16 bit DFF
				//1. load the data in to the DR
				SPIx_ptr(SPIx)->DR = *((uint16_t*) pTxBuffer);
				Len--;
				Len--;
				(uint16_t*) pTxBuffer++;
			} else {
				//8 bit DFF
				SPIx_ptr(SPIx)->DR = *pTxBuffer;
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
	void SPI_ReceiveData(uint8_t SPIx, uint8_t *pRxBuffer, uint32_t Len) {
		while (Len > 0) {
			//1. wait until RXNE is set

			while (!GET_BIT_N(SPIx_ptr(SPIx)->SR, SPI_SR_RXNE))
				;

			//2. check the DFF bit in CR1
			if (GET_BIT_N(SPIx_ptr(SPIx)->CR1, SPI_CR1_DFF)) {
				//16 bit DFF
				//1. load the data from DR to Rxbuffer address
				*((uint16_t*) pRxBuffer) = SPIx_ptr(SPIx)->DR;
				Len--;
				Len--;
				(uint16_t*) pRxBuffer++;
			} else {
				//8 bit DFF
				*(pRxBuffer) = SPIx_ptr(SPIx)->DR;
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
	void SPI_PeripheralControl(uint8_t SPIx, uint8_t State) {
		MODIFY_BIT_N(SPIx_ptr(SPIx)->CR1, SPI_CR1_SPE, State);
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
	void SPI_SSIConfig(uint8_t SPIx, uint8_t State) {
		MODIFY_BIT_N(SPIx_ptr(SPIx)->CR1, SPI_CR1_SSI, State);
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
	void SPI_SSOEConfig(uint8_t SPIx, uint8_t State) {
		MODIFY_BIT_N(SPIx_ptr(SPIx)->CR2, SPI_CR2_SSOE, State);
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
	void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi) {
		NVIC_INTR_CTRL(IRQNumber, State);
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
	void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority) {
		NVIC_INTR_PR_CTRL(IRQNumber, IRQPriority);
	}

	uint8_t SPI_SendDataIT(SPI_Handle_t SPIHandle, uint8_t *pTxBuffer,
			uint32_t Len) {
		uint8_t state = SPIHandle.TxState;

		if (state != SPI_BUSY_IN_TX) {
			//1 . Save the Tx buffer address and Len information in some global variables
			SPIHandle.pTxBuffer = pTxBuffer;
			SPIHandle.TxLen = Len;
			//2.  Mark the SPI state as busy in transmission so that
			//    no other code can take over same SPI peripheral until transmission is over
			SPIHandle.TxState = SPI_BUSY_IN_TX;

			//3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
			SET_BIT_N(SPIx_ptr(SPIHandle.SPIx)->CR2,SPI_CR2_TXEIE);

		}

		return state;
	}

	uint8_t SPI_ReceiveDataIT(SPI_Handle_t SPIHandle, uint8_t *pRxBuffer,
			uint32_t Len) {
		uint8_t state = SPIHandle.RxState;

		if (state != SPI_BUSY_IN_RX) {
			//1 . Save the Rx buffer address and Len information in some global variables
			SPIHandle.pRxBuffer = pRxBuffer;
			SPIHandle.RxLen = Len;
			//2.  Mark the SPI state as busy in reception so that
			//    no other code can take over same SPI peripheral until reception is over
			SPIHandle.RxState = SPI_BUSY_IN_RX;

			//3. Enable the RXNEIE control bit to get interrupt whenever RXNEIE flag is set in SR
			SET_BIT_N(SPIx_ptr(SPIHandle.SPIx)->CR2,SPI_CR2_RXNEIE);

		}

		return state;

	}

	void SPI_IRQHandling(SPI_Handle_t SPIHandle) {

		uint8_t temp1, temp2;
		//first lets check for TXE
		temp1 = SPIx_ptr(SPIHandle.SPIx)->SR & (1 << SPI_SR_TXE);
		temp2 = SPIx_ptr(SPIHandle.SPIx)->CR2 & (1 << SPI_CR2_TXEIE);

		if (temp1 && temp2) {
			//handle TXE
			spi_txe_interrupt_handle(SPIHandle);
		}

		// check for RXNE
		temp1 = SPIx_ptr(SPIHandle.SPIx)->SR & (1 << SPI_SR_RXNE);
		temp2 = SPIx_ptr(SPIHandle.SPIx)->CR2 & (1 << SPI_CR2_RXNEIE);

		if (temp1 && temp2) {
			//handle RXNE
			spi_rxne_interrupt_handle(SPIHandle);
		}

		// check for ovr flag
		temp1 = SPIx_ptr(SPIHandle.SPIx)->SR & (1 << SPI_SR_OVR);
		temp2 = SPIx_ptr(SPIHandle.SPIx)->CR2 & (1 << SPI_CR2_ERRIE);

		if (temp1 && temp2) {
			//handle ovr error
			spi_ovr_err_interrupt_handle(SPIHandle);
		}

	}

//some helper function implementations

	static void spi_txe_interrupt_handle(SPI_Handle_t SPIHandle) {
		// check the DFF bit in CR1
		if (GET_BIT_N(SPIx_ptr(SPIHandle.SPIx)->CR1 ,SPI_CR1_DFF)) {
			//16 bit DFF
			//1. load the data in to the DR
			SPIx_ptr(SPIHandle.SPIx)->DR = *((uint16_t*) SPIHandle.pTxBuffer);
			SPIHandle.TxLen--;
			SPIHandle.TxLen--;
			(uint16_t*) SPIHandle.pTxBuffer++;
		} else {
			//8 bit DFF
			SPIx_ptr(SPIHandle.SPIx)->DR = *SPIHandle.pTxBuffer;
			SPIHandle.TxLen--;
			SPIHandle.pTxBuffer++;
		}

		if (!SPIHandle.TxLen) {
			//TxLen is zero , so close the spi transmission and inform the application that
			//TX is over.

			//this prevents interrupts from setting up of TXE flag
			SPI_CloseTransmisson(SPIHandle);
			SPI_ApplicationEventCallback(SPIHandle, SPI_EVENT_TX_CMPLT);
		}

	}

	static void spi_rxne_interrupt_handle(SPI_Handle_t SPIHandle) {
		//do rxing as per the dff
		if (GET_BIT_N(SPIx_ptr(SPIHandle.SPIx)->CR1,SPI_CR1_DFF)) {
			//16 bit
			*((uint16_t*) SPIHandle.pRxBuffer) =
					(uint16_t) SPIx_ptr(SPIHandle.SPIx)->DR;
			SPIHandle.RxLen -= 2;
			SPIHandle.pRxBuffer++;
			SPIHandle.pRxBuffer++;

		} else {
			//8 bit
			*(SPIHandle.pRxBuffer) = (uint8_t) SPIx_ptr(SPIHandle.SPIx)->DR;
			SPIHandle.RxLen--;
			SPIHandle.pRxBuffer++;
		}

		if (!SPIHandle.RxLen) {
			//reception is complete
			SPI_CloseReception(SPIHandle);
			SPI_ApplicationEventCallback(SPIHandle, SPI_EVENT_RX_CMPLT);
		}

	}

	static void spi_ovr_err_interrupt_handle(SPI_Handle_t SPIHandle) {

		//1. clear the ovr flag
		if (SPIHandle.TxState != SPI_BUSY_IN_TX) {
			SPIx_ptr(SPIHandle.SPIx)->DR;
			SPIx_ptr(SPIHandle.SPIx)->SR;
		}
		//2. inform the application
		SPI_ApplicationEventCallback(SPIHandle, SPI_EVENT_OVR_ERR);

	}

	void SPI_CloseTransmisson(SPI_Handle_t SPIHandle) {
		CLR_BIT_N(SPIx_ptr(SPIHandle.SPIx)->CR2,SPI_CR2_TXEIE);
		SPIHandle.pTxBuffer = NULL;
		SPIHandle.TxLen = 0;
		SPIHandle.TxState = SPI_READY;

	}

	void SPI_CloseReception(SPI_Handle_t SPIHandle) {
		CLR_BIT_N(SPIx_ptr(SPIHandle.SPIx)->CR2,SPI_CR2_RXNEIE);
		SPIHandle.pRxBuffer = NULL;
		SPIHandle.RxLen = 0;
		SPIHandle.RxState = SPI_READY;

	}

	void SPI_ClearOVRFlag(uint8_t SPIx) {
		SPIx_ptr(SPIx)->DR;
		SPIx_ptr(SPIx)->SR;
	}

	__weak void SPI_ApplicationEventCallback(SPI_Handle_t SPIHandle,
			uint8_t AppEv) {

		//This is a weak implementation . the user application may override this function.
	}

