

#include "stm32f407xx_usart_driver.h"

/*********************************************************************
 * @fn      		  - USART_SetBaudRate
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
void USART_SetBaudRate(uint8_t USARTx, uint32_t BaudRate)
{

	//Variable to hold the APB clock
	uint32_t PCLKx;

	uint32_t usartdiv;

	//variables to hold Mantissa and Fraction values
	uint32_t M_part,F_part;

  uint32_t tempreg=0;

  //Get the value of APB bus clock in to the variable PCLKx
  if(USARTx == USART1 || USARTx == USART6)
  {
	   //USART1 and USART6 are hanging on APB2 bus
	   PCLKx = RCC_GetPCLK2Value();
  }else
  {
	   PCLKx = RCC_GetPCLK1Value();
  }

  //Check for OVER8 configuration bit
  if(USARTx_ptr(USARTx)->CR1 & (1 << USART_CR1_OVER8))
  {
	   //OVER8 = 1 , over sampling by 8
	   usartdiv = ((25 * PCLKx) / (2 *BaudRate));
  }else
  {
	   //over sampling by 16
	   usartdiv = ((25 * PCLKx) / (4 *BaudRate));
  }

  //Calculate the Mantissa part
  M_part = usartdiv/100;

  //Place the Mantissa part in appropriate bit position . refer USART_BRR
  tempreg |= M_part << 4;

  //Extract the fraction part
  F_part = (usartdiv - (M_part * 100));

  //Calculate the final fractional
  if(USARTx_ptr(USARTx)->CR1 & ( 1 << USART_CR1_OVER8))
   {
	  //OVER8 = 1 , over sampling by 8
	  F_part = ((( F_part * 8)+ 50) / 100)& ((uint8_t)0x07);

   }else
   {
	   //over sampling by 16
	   F_part = ((( F_part * 16)+ 50) / 100) & ((uint8_t)0x0F);

   }

  //Place the fractional part in appropriate bit position . refer USART_BRR
  tempreg |= F_part;

  //copy the value of tempreg in to BRR register
  USARTx_ptr(USARTx)->BRR = tempreg;
}

/*********************************************************************
 * @fn      		  - USART_Init
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
void USART_Init(USART_Handle_t USARTHandle)
{

	//Temporary variable
	uint32_t tempreg=0;

/******************************** Configuration of CR1******************************************/

	//Implement the code to enable the Clock for given USART peripheral
	 USART_PeriClockControl(USARTHandle.USARTx,ENABLE);

	//Enable USART Tx and Rx engines according to the USART_Mode configuration item
	if ( USARTHandle.USART_Config.USART_Mode == USART_MODE_ONLY_RX)
	{
		//Implement the code to enable the Receiver bit field
		tempreg|= (1 << USART_CR1_RE);
	}else if (USARTHandle.USART_Config.USART_Mode == USART_MODE_ONLY_TX)
	{
		//Implement the code to enable the Transmitter bit field
		tempreg |= ( 1 << USART_CR1_TE );

	}else if (USARTHandle.USART_Config.USART_Mode == USART_MODE_TXRX)
	{
		//Implement the code to enable the both Transmitter and Receiver bit fields
		tempreg |= ( ( 1 << USART_CR1_RE) | ( 1 << USART_CR1_TE) );
	}

    //Implement the code to configure the Word length configuration item
	tempreg |= USARTHandle.USART_Config.USART_WordLength << USART_CR1_M ;


    //Configuration of parity control bit fields
	if ( USARTHandle.USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN)
	{
		//Implement the code to enable the parity control
		tempreg |= ( 1 << USART_CR1_PCE);

		//Implement the code to enable EVEN parity
		//Not required because by default EVEN parity will be selected once you enable the parity control

	}else if (USARTHandle.USART_Config.USART_ParityControl == USART_PARITY_EN_ODD )
	{
		//Implement the code to enable the parity control
	    tempreg |= ( 1 << USART_CR1_PCE);

	    //Implement the code to enable ODD parity
	    tempreg |= ( 1 << USART_CR1_PS);

	}

   //Program the CR1 register
	USARTx_ptr(USARTHandle.USARTx)->CR1 = tempreg;

/******************************** Configuration of CR2******************************************/

	tempreg=0;

	//Implement the code to configure the number of stop bits inserted during USART frame transmission
	tempreg |= USARTHandle.USART_Config.USART_NoOfStopBits << USART_CR2_STOP;

	//Program the CR2 register
	USARTx_ptr(USARTHandle.USARTx)->CR2 = tempreg;

/******************************** Configuration of CR3******************************************/

	tempreg=0;

	//Configuration of USART hardware flow control
	if ( USARTHandle.USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS)
	{
		//Implement the code to enable CTS flow control
		tempreg |= ( 1 << USART_CR3_CTSE);


	}else if (USARTHandle.USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS)
	{
		//Implement the code to enable RTS flow control
		tempreg |= ( 1 << USART_CR3_RTSE);

	}else if (USARTHandle.USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS)
	{
		//Implement the code to enable both CTS and RTS Flow control
		tempreg |= ( 1 << USART_CR3_CTSE);
		tempreg |= ( 1 << USART_CR3_RTSE);
	}


	USARTx_ptr(USARTHandle.USARTx)->CR3 = tempreg;

/******************************** Configuration of BRR(Baudrate register)******************************************/

	//Implement the code to configure the baud rate
	//We will cover this in the lecture. No action required here
	USART_SetBaudRate(USARTHandle.USARTx,USARTHandle.USART_Config.USART_Baud);

}




/*********************************************************************
 * @fn      		  - USART_EnableOrDisable
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
void USART_PeripheralControl(uint8_t USARTx, uint8_t Cmd)
{
	if(Cmd == ENABLE)
	{
		USARTx_ptr(USARTx)->CR1 |= (1 << 13);
	}else
	{
		USARTx_ptr(USARTx)->CR1 &= ~(1 << 13);
	}

}



/*********************************************************************
 * @fn      		  - I2C_PeriClockControl
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
void USART_PeriClockControl(uint8_t USARTx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(USARTx == USART1)
		{
			USART1_PCCK_EN();
		}else if (USARTx == USART2)
		{
			USART2_PCCK_EN();
		}else if (USARTx == USART3)
		{
			USART3_PCCK_EN();
		}
		else if (USARTx == UART4)
		{
			UART4_PCCK_EN();
		}
	}
	else
	{
		//TODO
	}

}
/*********************************************************************
 * @fn      		  - USART_GetFlagStatus
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
uint8_t USART_GetFlagStatus(uint8_t USARTx, uint8_t StatusFlagName)
{
    if(USARTx_ptr(USARTx)->SR & StatusFlagName)
    {
    	return SET;
    }

   return RESET;
}

/*********************************************************************
 * @fn      		  - USART_SendData
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              - Resolve all the TODOs

 */
void USART_SendData(USART_Handle_t USARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{

	uint16_t *pdata;

   //Loop over until "Len" number of bytes are transferred
	for(uint32_t i = 0 ; i < Len; i++)
	{
		//Implement the code to wait until TXE flag is set in the SR
		while(! USART_GetFlagStatus(USARTHandle.USARTx,USART_FLAG_TXE));

		//Check the USART_WordLength item for 9BIT or 8BIT in a frame
		if(USARTHandle.USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
		{
			//if 9BIT load the DR with 2bytes masking  the bits other than first 9 bits
			pdata = (uint16_t*) pTxBuffer;
			USARTx_ptr(USARTHandle.USARTx)->DR = (*pdata & (uint16_t)0x01FF);

			//check for USART_ParityControl
			if(USARTHandle.USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				//No parity is used in this transfer , so 9bits of user data will be sent
				//Implement the code to increment pTxBuffer twice
				pTxBuffer++;
				pTxBuffer++;
			}
			else
			{
				//Parity bit is used in this transfer . so 8bits of user data will be sent
				//The 9th bit will be replaced by parity bit by the hardware
				pTxBuffer++;
			}
		}
		else
		{
			//This is 8bit data transfer
			USARTx_ptr(USARTHandle.USARTx)->DR = (*pTxBuffer  & (uint8_t)0xFF);

			//Implement the code to increment the buffer address
			pTxBuffer++;
		}
	}

	//Implement the code to wait till TC flag is set in the SR
	while( ! USART_GetFlagStatus(USARTHandle.USARTx,USART_FLAG_TC));
}


/*********************************************************************
 * @fn      		  - USART_ReceiveData
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

void USART_ReceiveData(USART_Handle_t USARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
   //Loop over until "Len" number of bytes are transferred
	for(uint32_t i = 0 ; i < Len; i++)
	{
		//Implement the code to wait until RXNE flag is set in the SR
		while(! USART_GetFlagStatus(USARTHandle.USARTx,USART_FLAG_RXNE));

		//Check the USART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
		if(USARTHandle.USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
		{
			//We are going to receive 9bit data in a frame

			//Now, check are we using USART_ParityControl control or not
			if(USARTHandle.USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				//No parity is used , so all 9bits will be of user data

				//read only first 9 bits so mask the DR with 0x01FF
				*((uint16_t*) pRxBuffer) = (USARTx_ptr(USARTHandle.USARTx)->DR  & (uint16_t)0x01FF);

				//Now increment the pRxBuffer two times
				pRxBuffer++;
				pRxBuffer++;
			}
			else
			{
				//Parity is used, so 8bits will be of user data and 1 bit is parity
				 *pRxBuffer = (USARTx_ptr(USARTHandle.USARTx)->DR  & (uint8_t)0xFF);
				 pRxBuffer++;
			}
		}
		else
		{
			//We are going to receive 8bit data in a frame

			//Now, check are we using USART_ParityControl control or not
			if(USARTHandle.USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				//No parity is used , so all 8bits will be of user data

				//read 8 bits from DR
				 *pRxBuffer = (uint8_t) (USARTx_ptr(USARTHandle.USARTx)->DR  & (uint8_t)0xFF);
			}

			else
			{
				//Parity is used, so , 7 bits will be of user data and 1 bit is parity

				//read only 7 bits , hence mask the DR with 0X7F
				 *pRxBuffer = (uint8_t) (USARTx_ptr(USARTHandle.USARTx)->DR  & (uint8_t)0x7F);

			}

			//Now , increment the pRxBuffer
			pRxBuffer++;
		}
	}

}

/*********************************************************************
 * @fn      		  - USART_SendDataWithIT
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
uint8_t USART_SendDataIT(USART_Handle_t USARTHandle,uint8_t *pTxBuffer, uint32_t Len)
{
	uint8_t txstate = USARTHandle.TxBusyState;

	if(txstate != USART_BUSY_IN_TX)
	{
		USARTHandle.TxLen = Len;
		USARTHandle.pTxBuffer = pTxBuffer;
		USARTHandle.TxBusyState = USART_BUSY_IN_TX;

		//Implement the code to enable interrupt for TXE
		USARTx_ptr(USARTHandle.USARTx)->CR1 |= ( 1 << USART_CR1_TXEIE);


		//Implement the code to enable interrupt for TC
		USARTx_ptr(USARTHandle.USARTx)->CR1 |= ( 1 << USART_CR1_TCIE);


	}

	return txstate;
}


/*********************************************************************
 * @fn      		  - USART_ReceiveDataWithIT
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
uint8_t USART_ReceiveDataIT(USART_Handle_t USARTHandle,uint8_t *pRxBuffer, uint32_t Len)
{
	uint8_t rxstate = USARTHandle.RxBusyState;

	if(rxstate != USART_BUSY_IN_RX)
	{
		USARTHandle.RxLen = Len;
		USARTHandle.pRxBuffer = pRxBuffer;
		USARTHandle.RxBusyState = USART_BUSY_IN_RX;

		(void)USARTx_ptr(USARTHandle.USARTx)->DR;

		//Implement the code to enable interrupt for RXNE
		USARTx_ptr(USARTHandle.USARTx)->CR1 |= ( 1 << USART_CR1_RXNEIE);

	}

	return rxstate;
}


/*********************************************************************
 * @fn      		  - USART_ClearFlag
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              - Applicable to only USART_CTS_FLAG , USART_LBD_FLAG
 * USART_TC_FLAG,USART_TC_FLAG flags
 *

 */

void USART_ClearFlag(uint8_t USARTx, uint16_t StatusFlagName)
{
	USARTx_ptr(USARTx)->SR &= ~( StatusFlagName);

}

/*********************************************************************
 * @fn      		  - USART_IRQInterruptConfig
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
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
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
 * @fn      		  - USART_IRQPriorityConfig
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
void USART_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	//1. first lets find out the ipr register
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section  = IRQNumber %4 ;

//	uint8_t shift_amount = ( 8 * iprx_section) + ( 8 - NO_PR_BITS_IMPLEMENTED) ;
//
//	*(  NVIC_PR_BASE_ADDR + iprx ) |=  ( IRQPriority << shift_amount );

}

/*********************************************************************
 * @fn      		  - USART_IRQHandler
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
void USART_IRQHandling(USART_Handle_t USARTHandle)
{

	uint32_t temp1 , temp2, temp3;

	uint16_t *pdata;

/*************************Check for TC flag ********************************************/

    //Implement the code to check the state of TC bit in the SR
	temp1 = USARTx_ptr(USARTHandle.USARTx)->SR & ( 1 << USART_SR_TC);

	 //Implement the code to check the state of TCEIE bit
	temp2 = USARTx_ptr(USARTHandle.USARTx)->CR1 & ( 1 << USART_CR1_TCIE);

	if(temp1 && temp2 )
	{
		//this interrupt is because of TC

		//close transmission and call application callback if TxLen is zero
		if ( USARTHandle.TxBusyState == USART_BUSY_IN_TX)
		{
			//Check the TxLen . If it is zero then close the data transmission
			if(! USARTHandle.TxLen )
			{
				//Implement the code to clear the TC flag
				USARTx_ptr(USARTHandle.USARTx)->SR &= ~( 1 << USART_SR_TC);

				//Implement the code to clear the TCIE control bit

				//Reset the application state
				USARTHandle.TxBusyState = USART_READY;

				//Reset Buffer address to NULL
				USARTHandle.pTxBuffer = NULL;

				//Reset the length to zero
				USARTHandle.TxLen = 0;

				//Call the application call back with event USART_EVENT_TX_CMPLT
				USART_ApplicationEventCallback(USARTHandle,USART_EVENT_TX_CMPLT);
			}
		}
	}

/*************************Check for TXE flag ********************************************/

	//Implement the code to check the state of TXE bit in the SR
	temp1 = USARTx_ptr(USARTHandle.USARTx)->SR & ( 1 << USART_SR_TXE);

	//Implement the code to check the state of TXEIE bit in CR1
	temp2 = USARTx_ptr(USARTHandle.USARTx)->CR1 & ( 1 << USART_CR1_TXEIE);


	if(temp1 && temp2 )
	{
		//this interrupt is because of TXE

		if(USARTHandle.TxBusyState == USART_BUSY_IN_TX)
		{
			//Keep sending data until Txlen reaches to zero
			if(USARTHandle.TxLen > 0)
			{
				//Check the USART_WordLength item for 9BIT or 8BIT in a frame
				if(USARTHandle.USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
				{
					//if 9BIT load the DR with 2bytes masking  the bits other than first 9 bits
					pdata = (uint16_t*) USARTHandle.pTxBuffer;
					USARTx_ptr(USARTHandle.USARTx)->DR = (*pdata & (uint16_t)0x01FF);

					//check for USART_ParityControl
					if(USARTHandle.USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
					{
						//No parity is used in this transfer , so 9bits of user data will be sent
						//Implement the code to increment pTxBuffer twice
						USARTHandle.pTxBuffer++;
						USARTHandle.pTxBuffer++;
						USARTHandle.TxLen-=2;
					}
					else
					{
						//Parity bit is used in this transfer . so 8bits of user data will be sent
						//The 9th bit will be replaced by parity bit by the hardware
						USARTHandle.pTxBuffer++;
						USARTHandle.TxLen-=1;
					}
				}
				else
				{
					//This is 8bit data transfer
					USARTx_ptr(USARTHandle.USARTx)->DR = (*USARTHandle.pTxBuffer  & (uint8_t)0xFF);

					//Implement the code to increment the buffer address
					USARTHandle.pTxBuffer++;
					USARTHandle.TxLen-=1;
				}

			}
			if (USARTHandle.TxLen == 0 )
			{
				//TxLen is zero
				//Implement the code to clear the TXEIE bit (disable interrupt for TXE flag )
				USARTx_ptr(USARTHandle.USARTx)->CR1 &= ~( 1 << USART_CR1_TXEIE);
			}
		}
	}

/*************************Check for RXNE flag ********************************************/

	temp1 = USARTx_ptr(USARTHandle.USARTx)->SR & ( 1 << USART_SR_RXNE);
	temp2 = USARTx_ptr(USARTHandle.USARTx)->CR1 & ( 1 << USART_CR1_RXNEIE);


	if(temp1 && temp2 )
	{
		//this interrupt is because of rxne
		if(USARTHandle.RxBusyState == USART_BUSY_IN_RX)
		{
			if(USARTHandle.RxLen > 0)
			{
				//Check the USART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
				if(USARTHandle.USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
				{
					//We are going to receive 9bit data in a frame

					//Now, check are we using USART_ParityControl control or not
					if(USARTHandle.USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
					{
						//No parity is used , so all 9bits will be of user data

						//read only first 9 bits so mask the DR with 0x01FF
						*((uint16_t*) USARTHandle.pRxBuffer) = (USARTx_ptr(USARTHandle.USARTx)->DR  & (uint16_t)0x01FF);

						//Now increment the pRxBuffer two times
						USARTHandle.pRxBuffer++;
						USARTHandle.pRxBuffer++;
						USARTHandle.RxLen-=2;
					}
					else
					{
						//Parity is used, so 8bits will be of user data and 1 bit is parity
						 USARTHandle.pRxBuffer = (USARTx_ptr(USARTHandle.USARTx)->DR  & (uint8_t)0xFF);
						 USARTHandle.pRxBuffer++;
						 USARTHandle.RxLen-=1;
					}
				}
				else
				{
					//We are going to receive 8bit data in a frame

					//Now, check are we using USART_ParityControl control or not
					if(USARTHandle.USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
					{
						//No parity is used , so all 8bits will be of user data

						//read 8 bits from DR
						 USARTHandle.pRxBuffer = (uint8_t) (USARTx_ptr(USARTHandle.USARTx)->DR  & (uint8_t)0xFF);

					}

					else
					{
						//Parity is used, so , 7 bits will be of user data and 1 bit is parity

						//read only 7 bits , hence mask the DR with 0X7F
						 USARTHandle.pRxBuffer = (uint8_t) (USARTx_ptr(USARTHandle.USARTx)->DR  & (uint8_t)0x7F);

					}

					//Now , increment the pRxBuffer
					USARTHandle.pRxBuffer++;
					 USARTHandle.RxLen-=1;
				}


			}//if of >0

			if(! USARTHandle.RxLen)
			{
				//disable the rxne
				USARTx_ptr(USARTHandle.USARTx)->CR1 &= ~( 1 << USART_CR1_RXNEIE );
				USARTHandle.RxBusyState = USART_READY;
				USART_ApplicationEventCallback(USARTHandle,USART_EVENT_RX_CMPLT);
			}
		}
	}


/*************************Check for CTS flag ********************************************/
//Note : CTS feature is not applicable for UART4 and UART5

	//Implement the code to check the status of CTS bit in the SR
	temp1 = USARTx_ptr(USARTHandle.USARTx)->SR & ( 1 << USART_SR_CTS);

	//Implement the code to check the state of CTSE bit in CR1
	temp2 = USARTx_ptr(USARTHandle.USARTx)->CR3 & ( 1 << USART_CR3_CTSE);

	//Implement the code to check the state of CTSIE bit in CR3 (This bit is not available for UART4 & UART5.)
	temp3 = USARTx_ptr(USARTHandle.USARTx)->CR3 & ( 1 << USART_CR3_CTSIE);


	if(temp1  && temp2 )
	{
		//Implement the code to clear the CTS flag in SR
		USARTx_ptr(USARTHandle.USARTx)->SR &=  ~( 1 << USART_SR_CTS);

		//this interrupt is because of cts
		USART_ApplicationEventCallback(USARTHandle,USART_EVENT_CTS);
	}

/*************************Check for IDLE detection flag ********************************************/

	//Implement the code to check the status of IDLE flag bit in the SR
	temp1 = USARTx_ptr(USARTHandle.USARTx)->SR & ( 1 << USART_SR_IDLE);

	//Implement the code to check the state of IDLEIE bit in CR1
	temp2 = USARTx_ptr(USARTHandle.USARTx)->CR1 & ( 1 << USART_CR1_IDLEIE);


	if(temp1 && temp2)
	{
		//Implement the code to clear the IDLE flag. Refer to the RM to understand the clear sequence
		temp1 = USARTx_ptr(USARTHandle.USARTx)->SR &= ~( 1 << USART_SR_IDLE);

		//this interrupt is because of idle
		USART_ApplicationEventCallback(USARTHandle,USART_EVENT_IDLE);
	}

/*************************Check for Overrun detection flag ********************************************/

	//Implement the code to check the status of ORE flag  in the SR
	temp1 = USARTx_ptr(USARTHandle.USARTx)->SR & USART_SR_ORE;

	//Implement the code to check the status of RXNEIE  bit in the CR1
	temp2 = USARTx_ptr(USARTHandle.USARTx)->CR1 & USART_CR1_RXNEIE;


	if(temp1  && temp2 )
	{
		//Need not to clear the ORE flag here, instead give an api for the application to clear the ORE flag .

		//this interrupt is because of Overrun error
		USART_ApplicationEventCallback(USARTHandle,USART_ERR_ORE);
	}



/*************************Check for Error Flag ********************************************/

//Noise Flag, Overrun error and Framing Error in multibuffer communication
//We dont discuss multibuffer communication in this course. please refer to the RM
//The blow code will get executed in only if multibuffer mode is used.

	temp2 =  USARTx_ptr(USARTHandle.USARTx)->CR3 & ( 1 << USART_CR3_EIE) ;

	if(temp2 )
	{
		temp1 = USARTx_ptr(USARTHandle.USARTx)->SR;
		if(temp1 & ( 1 << USART_SR_FE))
		{
			/*
				This bit is set by hardware when a de-synchronization, excessive noise or a break character
				is detected. It is cleared by a software sequence (an read to the USART_SR register
				followed by a read to the USART_DR register).
			*/
			USART_ApplicationEventCallback(USARTHandle,USART_ERR_FE);
		}

		if(temp1 & ( 1 << USART_SR_NE) )
		{
			/*
				This bit is set by hardware when noise is detected on a received frame. It is cleared by a
				software sequence (an read to the USART_SR register followed by a read to the
				USART_DR register).
			*/
			USART_ApplicationEventCallback(USARTHandle,USART_ERR_NE);
		}

		if(temp1 & ( 1 << USART_SR_ORE) )
		{
			USART_ApplicationEventCallback(USARTHandle,USART_ERR_ORE);
		}
	}


}



/*********************************************************************
 * @fn      		  - USART_ApplicationEventCallback
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
__weak void USART_ApplicationEventCallback(USART_Handle_t USARTHandle,uint8_t event)
{

}
