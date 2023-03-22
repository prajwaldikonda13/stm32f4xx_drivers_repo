#include "stm32f407xx_i2c_driver.h"


static void  I2C_GenerateStartCondition(uint8_t I2Cx);
static void I2C_ExecuteAddressPhaseWrite(uint8_t I2Cx, uint8_t SlaveAddr);
static void I2C_ExecuteAddressPhaseRead(uint8_t I2Cx, uint8_t SlaveAddr);
static void I2C_ClearADDRFlag(I2C_Handle_t I2CHandle);

static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t I2CHandle );
static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t I2CHandle );

static void I2C_GenerateStartCondition(uint8_t I2Cx)
{
	SET_BIT_N(I2Cx_ptr(I2Cx)->CR1,I2C_CR1_START);
}



static void I2C_ExecuteAddressPhaseWrite(uint8_t I2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1;
	SlaveAddr &= ~(1); //SlaveAddr is Slave address + r/nw bit=0
	I2Cx_ptr(I2Cx)->DR = SlaveAddr;
}


static void I2C_ExecuteAddressPhaseRead(uint8_t I2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1;
	SlaveAddr |= 1; //SlaveAddr is Slave address + r/nw bit=1
	I2Cx_ptr(I2Cx)->DR = SlaveAddr;
}


static void I2C_ClearADDRFlag(I2C_Handle_t I2CHandle )
{
	uint32_t dummy_read;
	//check for device mode
	if(I2Cx_ptr(I2CHandle.I2Cx)->SR2 & ( 1 << I2C_SR2_MSL))
	{
		//device is in master mode
		if(I2CHandle.TxRxState == I2C_BUSY_IN_RX)
		{
			if(I2CHandle.RxSize  == 1)
			{
				//first disable the ack
				I2C_ManageAcking(I2Cx_ptr(I2CHandle.I2Cx),DISABLE);

				//clear the ADDR flag ( read SR1 , read SR2)
				dummy_read = I2Cx_ptr(I2CHandle.I2Cx)->SR1;
				dummy_read = I2Cx_ptr(I2CHandle.I2Cx)->SR2;
				(void)dummy_read;
			}

		}
		else
		{
			//clear the ADDR flag ( read SR1 , read SR2)
			dummy_read = I2Cx_ptr(I2CHandle.I2Cx)->SR1;
			dummy_read = I2Cx_ptr(I2CHandle.I2Cx)->SR2;
			(void)dummy_read;

		}

	}
	else
	{
		//device is in slave mode
		//clear the ADDR flag ( read SR1 , read SR2)
		dummy_read = I2Cx_ptr(I2CHandle.I2Cx)->SR1;
		dummy_read = I2Cx_ptr(I2CHandle.I2Cx)->SR2;
		(void)dummy_read;
	}


}


 void I2C_GenerateStopCondition(uint8_t I2Cx)
{
	 I2Cx_ptr(I2Cx)->CR1 |= ( 1 << I2C_CR1_STOP);
}


 void I2C_SlaveEnableDisableCallbackEvents(uint8_t I2Cx,uint8_t EnorDi)
 {
	 if(EnorDi == ENABLE)
	 {
		 I2Cx_ptr(I2Cx)->CR2 |= ( 1 << I2C_CR2_ITEVTEN);
		 I2Cx_ptr(I2Cx)->CR2 |= ( 1 << I2C_CR2_ITBUFEN);
		 I2Cx_ptr(I2Cx)->CR2 |= ( 1 << I2C_CR2_ITERREN);
	 }else
	 {
		 I2Cx_ptr(I2Cx)->CR2 &= ~( 1 << I2C_CR2_ITEVTEN);
		 I2Cx_ptr(I2Cx)->CR2 &= ~( 1 << I2C_CR2_ITBUFEN);
		 I2Cx_ptr(I2Cx)->CR2 &= ~( 1 << I2C_CR2_ITERREN);
	 }

 }

/*********************************************************************
 * @fn      		  - I2C_PeripheralControl
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
void I2C_PeripheralControl(uint8_t I2Cx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		I2Cx_ptr(I2Cx)->CR1 |= (1 << I2C_CR1_PE);
		//pI2cBaseAddress->CR1 |= I2C_CR1_PE_Bit_Mask;
	}else
	{
		I2Cx_ptr(I2Cx)->CR1 &= ~(1 << 0);
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
void I2C_PeriClockControl(uint8_t I2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(I2Cx == I2C1)
		{
			I2C1_PCLK_EN();
		}else if (I2Cx == I2C2)
		{
			I2C2_PCLK_EN();
		}else if (I2Cx == I2C3)
		{
			I2C3_PCLK_EN();
		}
	}
	else
	{
		//TODO
	}

}




/*********************************************************************
 * @fn      		  - I2C_Init
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
void I2C_Init(I2C_Handle_t I2CHandle)
{
	uint32_t tempreg = 0 ;

	//enable the clock for the i2cx peripheral
	I2C_PeriClockControl(I2Cx_ptr(I2CHandle.I2Cx),ENABLE);

	//ack control bit
	tempreg |= I2CHandle.I2C_Config.I2C_AckControl << 10;
	I2Cx_ptr(I2CHandle.I2Cx)->CR1 = tempreg;

	//configure the FREQ field of CR2
	tempreg = 0;
	tempreg |= RCC_GetPCLK1Value() /1000000U ;
	I2Cx_ptr(I2CHandle.I2Cx)->CR2 =  (tempreg & 0x3F);

   //program the device own address
	tempreg = 0;
	tempreg |= I2CHandle.I2C_Config.I2C_DeviceAddress << 1;
	tempreg |= ( 1 << 14);
	I2Cx_ptr(I2CHandle.I2Cx)->OAR1 = tempreg;

	//CCR calculations
	uint16_t ccr_value = 0;
	tempreg = 0;
	if(I2CHandle.I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		//mode is standard mode
		ccr_value = (RCC_GetPCLK1Value() / ( 2 * I2CHandle.I2C_Config.I2C_SCLSpeed ) );
		tempreg |= (ccr_value & 0xFFF);
	}else
	{
		//mode is fast mode
		tempreg |= ( 1 << 15);
		tempreg |= (I2CHandle.I2C_Config.I2C_FMDutyCycle << 14);
		if(I2CHandle.I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2)
		{
			ccr_value = (RCC_GetPCLK1Value() / ( 3 * I2CHandle.I2C_Config.I2C_SCLSpeed ) );
		}else
		{
			ccr_value = (RCC_GetPCLK1Value() / ( 25 * I2CHandle.I2C_Config.I2C_SCLSpeed ) );
		}
		tempreg |= (ccr_value & 0xFFF);
	}
	I2Cx_ptr(I2CHandle.I2Cx)->CCR = tempreg;

	//TRISE Configuration
	if(I2CHandle.I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		//mode is standard mode

		tempreg = (RCC_GetPCLK1Value() /1000000U) + 1 ;

	}else
	{
		//mode is fast mode
		tempreg = ( (RCC_GetPCLK1Value() * 300) / 1000000000U ) + 1;

	}

	I2Cx_ptr(I2CHandle.I2Cx)->TRISE = (tempreg & 0x3F);

}


/*********************************************************************
 * @fn      		  - I2C_DeInit
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
void I2C_DeInit(uint8_t I2Cx)
{

}


uint8_t I2C_GetFlagStatus(uint8_t I2Cx , uint32_t FlagName)
{
	if(I2Cx_ptr(I2Cx)->SR1 & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}



void I2C_MasterSendData(I2C_Handle_t I2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr)
{
	// 1. Generate the START condition
	I2C_GenerateStartCondition(I2Cx_ptr(I2CHandle.I2Cx));

	//2. confirm that start generation is completed by checking the SB flag in the SR1
	//   Note: Until SB is cleared SCL will be stretched (pulled to LOW)
	while( !  I2C_GetFlagStatus(I2Cx_ptr(I2CHandle.I2Cx),I2C_FLAG_SB)   );

	//3. Send the address of the slave with r/nw bit set to w(0) (total 8 bits )
	I2C_ExecuteAddressPhaseWrite(I2Cx_ptr(I2CHandle.I2Cx),SlaveAddr);

	//4. Confirm that address phase is completed by checking the ADDR flag in teh SR1
	while( !  I2C_GetFlagStatus(I2Cx_ptr(I2CHandle.I2Cx),I2C_FLAG_ADDR)   );

	//5. clear the ADDR flag according to its software sequence
	//   Note: Until ADDR is cleared SCL will be stretched (pulled to LOW)
	I2C_ClearADDRFlag(I2CHandle);

	//6. send the data until len becomes 0

	while(Len > 0)
	{
		while(! I2C_GetFlagStatus(I2Cx_ptr(I2CHandle.I2Cx),I2C_FLAG_TXE) ); //Wait till TXE is set
		I2Cx_ptr(I2CHandle.I2Cx)->DR = *pTxbuffer;
		pTxbuffer++;
		Len--;
	}

	//7. when Len becomes zero wait for TXE=1 and BTF=1 before generating the STOP condition
	//   Note: TXE=1 , BTF=1 , means that both SR and DR are empty and next transmission should begin
	//   when BTF=1 SCL will be stretched (pulled to LOW)

	while(! I2C_GetFlagStatus(I2Cx_ptr(I2CHandle.I2Cx),I2C_FLAG_TXE) );

	while(! I2C_GetFlagStatus(I2Cx_ptr(I2CHandle.I2Cx),I2C_FLAG_BTF) );


	//8. Generate STOP condition and master need not to wait for the completion of stop condition.
	//   Note: generating STOP, automatically clears the BTF
	if(Sr == I2C_DISABLE_SR )
		I2C_GenerateStopCondition(I2Cx_ptr(I2CHandle.I2Cx));

}


void I2C_MasterReceiveData(I2C_Handle_t I2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr)
{

	//1. Generate the START condition
	I2C_GenerateStartCondition(I2Cx_ptr(I2CHandle.I2Cx));

	//2. confirm that start generation is completed by checking the SB flag in the SR1
	//   Note: Until SB is cleared SCL will be stretched (pulled to LOW)
	while( !  I2C_GetFlagStatus(I2Cx_ptr(I2CHandle.I2Cx),I2C_FLAG_SB)   );

	//3. Send the address of the slave with r/nw bit set to R(1) (total 8 bits )
	I2C_ExecuteAddressPhaseRead(I2Cx_ptr(I2CHandle.I2Cx),SlaveAddr);

	//4. wait until address phase is completed by checking the ADDR flag in teh SR1
	while( !  I2C_GetFlagStatus(I2Cx_ptr(I2CHandle.I2Cx),I2C_FLAG_ADDR)   );


	//procedure to read only 1 byte from slave
	if(Len == 1)
	{
		//Disable Acking
		I2C_ManageAcking(I2Cx_ptr(I2CHandle.I2Cx),I2C_ACK_DISABLE);


		//clear the ADDR flag
		I2C_ClearADDRFlag(I2CHandle);

		//wait until  RXNE becomes 1
		while(! I2C_GetFlagStatus(I2Cx_ptr(I2CHandle.I2Cx),I2C_FLAG_RXNE) );

		//generate STOP condition
		if(Sr == I2C_DISABLE_SR )
			I2C_GenerateStopCondition(I2Cx_ptr(I2CHandle.I2Cx));

		//read data in to buffer
		*pRxBuffer = I2Cx_ptr(I2CHandle.I2Cx)->DR;

	}


    //procedure to read data from slave when Len > 1
	if(Len > 1)
	{
		//clear the ADDR flag
		I2C_ClearADDRFlag(I2CHandle);

		//read the data until Len becomes zero
		for ( uint32_t i = Len ; i > 0 ; i--)
		{
			//wait until RXNE becomes 1
			while(! I2C_GetFlagStatus(I2Cx_ptr(I2CHandle.I2Cx),I2C_FLAG_RXNE) );

			if(i == 2) //if last 2 bytes are remaining
			{
				//Disable Acking
				I2C_ManageAcking(I2Cx_ptr(I2CHandle.I2Cx),I2C_ACK_DISABLE);

				//generate STOP condition
				if(Sr == I2C_DISABLE_SR )
					I2C_GenerateStopCondition(I2Cx_ptr(I2CHandle.I2Cx));

			}

			//read the data from data register in to buffer
			*pRxBuffer = I2Cx_ptr(I2CHandle.I2Cx)->DR;

			//increment the buffer address
			pRxBuffer++;

		}

	}

	//re-enable ACKing
	if(I2CHandle.I2C_Config.I2C_AckControl == I2C_ACK_ENABLE)
	{
		I2C_ManageAcking(I2Cx_ptr(I2CHandle.I2Cx),I2C_ACK_ENABLE);
	}

}


void I2C_ManageAcking(uint8_t I2Cx, uint8_t EnorDi)
{
	if(EnorDi == I2C_ACK_ENABLE)
	{
		//enable the ack
		I2Cx_ptr(I2Cx)->CR1 |= ( 1 << I2C_CR1_ACK);
	}else
	{
		//disable the ack
		I2Cx_ptr(I2Cx)->CR1 &= ~( 1 << I2C_CR1_ACK);
	}
}

/*********************************************************************
 * @fn      		  - I2C_IRQInterruptConfig
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
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
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
 * @fn      		  - I2C_IRQPriorityConfig
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
void I2C_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	//1. first lets find out the ipr register
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section  = IRQNumber %4 ;

//	uint8_t shift_amount = ( 8 * iprx_section) + ( 8 - NO_PR_BITS_IMPLEMENTED) ;
//
//	*(  NVIC_PR_BASE_ADDR + iprx ) |=  ( IRQPriority << shift_amount );

}

/*********************************************************************
 * @fn      		  - I2C_MasterSendDataIT
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
uint8_t I2C_MasterSendDataIT(I2C_Handle_t I2CHandle,uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr)
{
	uint8_t busystate = I2CHandle.TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		I2CHandle.pTxBuffer = pTxBuffer;
		I2CHandle.TxLen = Len;
		I2CHandle.TxRxState = I2C_BUSY_IN_TX;
		I2CHandle.DevAddr = SlaveAddr;
		I2CHandle.Sr = Sr;

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(I2Cx_ptr(I2CHandle.I2Cx));

		//Implement the code to enable ITBUFEN Control Bit
		I2Cx_ptr(I2CHandle.I2Cx)->CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		I2Cx_ptr(I2CHandle.I2Cx)->CR2 |= ( 1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		I2Cx_ptr(I2CHandle.I2Cx)->CR2 |= ( 1 << I2C_CR2_ITERREN);

	}

	return busystate;
}


/*********************************************************************
 * @fn      		  - I2C_MasterReceiveDataIT
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
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t I2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr)
{
	uint8_t busystate = I2CHandle.TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		I2CHandle.pRxBuffer = pRxBuffer;
		I2CHandle.RxLen = Len;
		I2CHandle.TxRxState = I2C_BUSY_IN_RX;
		I2CHandle.RxSize = Len;
		I2CHandle.DevAddr = SlaveAddr;
		I2CHandle.Sr = Sr;

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(I2Cx_ptr(I2CHandle.I2Cx));

		//Implement the code to enable ITBUFEN Control Bit
		I2Cx_ptr(I2CHandle.I2Cx)->CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		I2Cx_ptr(I2CHandle.I2Cx)->CR2 |= ( 1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		I2Cx_ptr(I2CHandle.I2Cx)->CR2 |= ( 1 << I2C_CR2_ITERREN);
	}

	return busystate;
}

static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t I2CHandle )
{

	if(I2CHandle.TxLen > 0)
	{
		//1. load the data in to DR
		I2Cx_ptr(I2CHandle.I2Cx)->DR = *(I2CHandle.pTxBuffer);

		//2. decrement the TxLen
		I2CHandle.TxLen--;

		//3. Increment the buffer address
		I2CHandle.pTxBuffer++;

	}

}

static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t I2CHandle )
{
	//We have to do the data reception
	if(I2CHandle.RxSize == 1)
	{
		*I2CHandle.pRxBuffer = I2Cx_ptr(I2CHandle.I2Cx)->DR;
		I2CHandle.RxLen--;

	}


	if(I2CHandle.RxSize > 1)
	{
		if(I2CHandle.RxLen == 2)
		{
			//clear the ack bit
			I2C_ManageAcking(I2Cx_ptr(I2CHandle.I2Cx),DISABLE);
		}

			//read DR
			*I2CHandle.pRxBuffer = I2Cx_ptr(I2CHandle.I2Cx)->DR;
			I2CHandle.pRxBuffer++;
			I2CHandle.RxLen--;
	}

	if(I2CHandle.RxLen == 0 )
	{
		//close the I2C data reception and notify the application

		//1. generate the stop condition
		if(I2CHandle.Sr == I2C_DISABLE_SR)
			I2C_GenerateStopCondition(I2Cx_ptr(I2CHandle.I2Cx));

		//2 . Close the I2C rx
		I2C_CloseReceiveData(I2CHandle);

		//3. Notify the application
		I2C_ApplicationEventCallback(I2CHandle,I2C_EV_RX_CMPLT);
	}
}


void I2C_CloseReceiveData(I2C_Handle_t I2CHandle)
{
	//Implement the code to disable ITBUFEN Control Bit
	I2Cx_ptr(I2CHandle.I2Cx)->CR2 &= ~( 1 << I2C_CR2_ITBUFEN);

	//Implement the code to disable ITEVFEN Control Bit
	I2Cx_ptr(I2CHandle.I2Cx)->CR2 &= ~( 1 << I2C_CR2_ITEVTEN);

	I2CHandle.TxRxState = I2C_READY;
	I2CHandle.pRxBuffer = NULL;
	I2CHandle.RxLen = 0;
	I2CHandle.RxSize = 0;

	if(I2CHandle.I2C_Config.I2C_AckControl == I2C_ACK_ENABLE)
	{
		I2C_ManageAcking(I2Cx_ptr(I2CHandle.I2Cx),ENABLE);
	}

}

void I2C_CloseSendData(I2C_Handle_t I2CHandle)
{
	//Implement the code to disable ITBUFEN Control Bit
	I2Cx_ptr(I2CHandle.I2Cx)->CR2 &= ~( 1 << I2C_CR2_ITBUFEN);

	//Implement the code to disable ITEVFEN Control Bit
	I2Cx_ptr(I2CHandle.I2Cx)->CR2 &= ~( 1 << I2C_CR2_ITEVTEN);


	I2CHandle.TxRxState = I2C_READY;
	I2CHandle.pTxBuffer = NULL;
	I2CHandle.TxLen = 0;
}


void I2C_SlaveSendData(uint8_t I2Cx,uint8_t data)
{
	I2Cx_ptr(I2Cx)->DR = data;
}

uint8_t I2C_SlaveReceiveData(uint8_t I2Cx)
{
    return (uint8_t) I2Cx_ptr(I2Cx)->DR;
}



void I2C_EV_IRQHandling(I2C_Handle_t I2CHandle)
{
	//Interrupt handling for both master and slave mode of a device

	uint32_t temp1, temp2, temp3;

	temp1   = I2Cx_ptr(I2CHandle.I2Cx)->CR2 & ( 1 << I2C_CR2_ITEVTEN) ;
	temp2   = I2Cx_ptr(I2CHandle.I2Cx)->CR2 & ( 1 << I2C_CR2_ITBUFEN) ;

	temp3  = I2Cx_ptr(I2CHandle.I2Cx)->SR1 & ( 1 << I2C_SR1_SB);
	//1. Handle For interrupt generated by SB event
	//	Note : SB flag is only applicable in Master mode
	if(temp1 && temp3)
	{
		//The interrupt is generated because of SB event
		//This block will not be executed in slave mode because for slave SB is always zero
		//In this block lets executed the address phase
		if(I2CHandle.TxRxState == I2C_BUSY_IN_TX)
		{
			I2C_ExecuteAddressPhaseWrite(I2Cx_ptr(I2CHandle.I2Cx),I2CHandle.DevAddr);
		}else if (I2CHandle.TxRxState == I2C_BUSY_IN_RX )
		{
			I2C_ExecuteAddressPhaseRead(I2Cx_ptr(I2CHandle.I2Cx),I2CHandle.DevAddr);
		}
	}

	temp3  = I2Cx_ptr(I2CHandle.I2Cx)->SR1 & ( 1 << I2C_SR1_ADDR);
	//2. Handle For interrupt generated by ADDR event
	//Note : When master mode : Address is sent
	//		 When Slave mode   : Address matched with own address
	if(temp1 && temp3)
	{
		// interrupt is generated because of ADDR event
		I2C_ClearADDRFlag(I2CHandle);
	}

	temp3  = I2Cx_ptr(I2CHandle.I2Cx)->SR1 & ( 1 << I2C_SR1_BTF);
	//3. Handle For interrupt generated by BTF(Byte Transfer Finished) event
	if(temp1 && temp3)
	{
		//BTF flag is set
		if(I2CHandle.TxRxState == I2C_BUSY_IN_TX)
		{
			//make sure that TXE is also set .
			if(I2Cx_ptr(I2CHandle.I2Cx)->SR1 & ( 1 << I2C_SR1_TXE) )
			{
				//BTF, TXE = 1
				if(I2CHandle.TxLen == 0 )
				{
					//1. generate the STOP condition
					if(I2CHandle.Sr == I2C_DISABLE_SR)
						I2C_GenerateStopCondition(I2Cx_ptr(I2CHandle.I2Cx));

					//2. reset all the member elements of the handle structure.
					I2C_CloseSendData(I2CHandle);

					//3. notify the application about transmission complete
					I2C_ApplicationEventCallback(I2CHandle,I2C_EV_TX_CMPLT);

				}
			}

		}else if (I2CHandle.TxRxState == I2C_BUSY_IN_RX )
		{
			;
		}
	}

	temp3  = I2Cx_ptr(I2CHandle.I2Cx)->SR1 & ( 1 << I2C_SR1_STOPF);
	//4. Handle For interrupt generated by STOPF event
	// Note : Stop detection flag is applicable only slave mode . For master this flag will never be set
	//The below code block will not be executed by the master since STOPF will not set in master mode
	if(temp1 && temp3)
	{
		//STOF flag is set
		//Clear the STOPF ( i.e 1) read SR1 2) Write to CR1 )

		I2Cx_ptr(I2CHandle.I2Cx)->CR1 |= 0x0000;

		//Notify the application that STOP is detected
		I2C_ApplicationEventCallback(I2CHandle,I2C_EV_STOP);
	}


	temp3  = I2Cx_ptr(I2CHandle.I2Cx)->SR1 & ( 1 << I2C_SR1_TXE);
	//5. Handle For interrupt generated by TXE event
	if(temp1 && temp2 && temp3)
	{
		//Check for device mode
		if(I2Cx_ptr(I2CHandle.I2Cx)->SR2 & ( 1 << I2C_SR2_MSL))
		{
			//TXE flag is set
			//We have to do the data transmission
			if(I2CHandle.TxRxState == I2C_BUSY_IN_TX)
			{
				I2C_MasterHandleTXEInterrupt(I2CHandle);
			}
		}else
		{
			//slave
			//make sure that the slave is really in transmitter mode
		    if(I2Cx_ptr(I2CHandle.I2Cx)->SR2 & ( 1 << I2C_SR2_TRA))
		    {
		    	I2C_ApplicationEventCallback(I2CHandle,I2C_EV_DATA_REQ);
		    }
		}
	}

	temp3  = I2Cx_ptr(I2CHandle.I2Cx)->SR1 & ( 1 << I2C_SR1_RXNE);
	//6. Handle For interrupt generated by RXNE event
	if(temp1 && temp2 && temp3)
	{
		//check device mode .
		if(I2Cx_ptr(I2CHandle.I2Cx)->SR2 & ( 1 << I2C_SR2_MSL))
		{
			//The device is master

			//RXNE flag is set
			if(I2CHandle.TxRxState == I2C_BUSY_IN_RX)
			{
				I2C_MasterHandleRXNEInterrupt(I2CHandle);

			}

		}else
		{
			//slave
			//make sure that the slave is really in receiver mode
			if(!(I2Cx_ptr(I2CHandle.I2Cx)->SR2 & ( 1 << I2C_SR2_TRA)))
			{
				I2C_ApplicationEventCallback(I2CHandle,I2C_EV_DATA_RCV);
			}
		}
	}
}



/*********************************************************************
 * @fn      		  - I2C_ER_IRQHandling
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              - Complete the code also define these macros in the driver
						header file
						#define I2C_ERROR_BERR  3
						#define I2C_ERROR_ARLO  4
						#define I2C_ERROR_AF    5
						#define I2C_ERROR_OVR   6
						#define I2C_ERROR_TIMEOUT 7

 */

void I2C_ER_IRQHandling(I2C_Handle_t I2CHandle)
{

	uint32_t temp1,temp2;

    //Know the status of  ITERREN control bit in the CR2
	temp2 = (I2Cx_ptr(I2CHandle.I2Cx)->CR2) & ( 1 << I2C_CR2_ITERREN);


/***********************Check for Bus error************************************/
	temp1 = (I2Cx_ptr(I2CHandle.I2Cx)->SR1) & ( 1<< I2C_SR1_BERR);
	if(temp1  && temp2 )
	{
		//This is Bus error

		//Implement the code to clear the buss error flag
		I2Cx_ptr(I2CHandle.I2Cx)->SR1 &= ~( 1 << I2C_SR1_BERR);

		//Implement the code to notify the application about the error
	   I2C_ApplicationEventCallback(I2CHandle,I2C_ERROR_BERR);
	}

/***********************Check for arbitration lost error************************************/
	temp1 = (I2Cx_ptr(I2CHandle.I2Cx)->SR1) & ( 1 << I2C_SR1_ARLO );
	if(temp1  && temp2)
	{
		//This is arbitration lost error

		//Implement the code to clear the arbitration lost error flag
		I2Cx_ptr(I2CHandle.I2Cx)->SR1 &= ~( 1 << I2C_SR1_ARLO);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(I2CHandle,I2C_ERROR_ARLO);

	}

/***********************Check for ACK failure  error************************************/

	temp1 = (I2Cx_ptr(I2CHandle.I2Cx)->SR1) & ( 1 << I2C_SR1_AF);
	if(temp1  && temp2)
	{
		//This is ACK failure error

	    //Implement the code to clear the ACK failure error flag
		I2Cx_ptr(I2CHandle.I2Cx)->SR1 &= ~( 1 << I2C_SR1_AF);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(I2CHandle,I2C_ERROR_AF);
	}

/***********************Check for Overrun/underrun error************************************/
	temp1 = (I2Cx_ptr(I2CHandle.I2Cx)->SR1) & ( 1 << I2C_SR1_OVR);
	if(temp1  && temp2)
	{
		//This is Overrun/underrun

	    //Implement the code to clear the Overrun/underrun error flag
		I2Cx_ptr(I2CHandle.I2Cx)->SR1 &= ~( 1 << I2C_SR1_OVR);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(I2CHandle,I2C_ERROR_OVR);
	}

/***********************Check for Time out error************************************/
	temp1 = (I2Cx_ptr(I2CHandle.I2Cx)->SR1) & ( 1 << I2C_SR1_TIMEOUT);
	if(temp1  && temp2)
	{
		//This is Time out error

	    //Implement the code to clear the Time out error flag
		I2Cx_ptr(I2CHandle.I2Cx)->SR1 &= ~( 1 << I2C_SR1_TIMEOUT);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(I2CHandle,I2C_ERROR_TIMEOUT);
	}

}





