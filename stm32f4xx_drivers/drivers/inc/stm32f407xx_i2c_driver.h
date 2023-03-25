
#ifndef INC_STM32F407XX_I2C_DRIVER_H_
#define INC_STM32F407XX_I2C_DRIVER_H_

#include "stm32f407xx.h"
#include"stm32f407xx_rcc_driver.h"
#include"stm32f407xx_nvic_driver.h"

#define I2Cx_ptr(I2Cx)			((I2C_RegDef_t*)BASE_ADDR_LST[I2Cx])


/*
 * Configuration structure for I2Cx peripheral
 */
typedef struct
{
	uint32_t I2C_SCLSpeed;
	uint8_t  I2C_DeviceAddress;
	uint8_t  I2C_AckControl;
	uint8_t  I2C_FMDutyCycle;

}I2C_Config_t;

/*
 *Handle structure for I2Cx peripheral
 */
typedef struct
{
	uint8_t 	I2Cx;
	I2C_Config_t 	I2C_Config;
	uint8_t 		*pTxBuffer; /* !< To store the app. Tx buffer address > */
	uint8_t 		*pRxBuffer;	/* !< To store the app. Rx buffer address > */
	uint32_t 		TxLen;		/* !< To store Tx len > */
	uint32_t 		RxLen;		/* !< To store Tx len > */
	uint8_t 		TxRxState;	/* !< To store Communication state > */
	uint8_t 		DevAddr;	/* !< To store slave/device address > */
    uint32_t        RxSize;		/* !< To store Rx size  > */
    uint8_t         Sr;			/* !< To store repeated start value  > */
}I2C_Handle_t;




#define I2C_CR1_ACK				10
#define I2C_OAR_ADDR			1



/*
 * I2C application states
 */
#define I2C_READY 					0
#define I2C_BUSY_IN_RX 				1
#define I2C_BUSY_IN_TX 				2

/*
 * @I2C_SCLSpeed
 */
#define I2C_SCL_SPEED_SM 	100000
#define I2C_SCL_SPEED_FM4K 	400000
#define I2C_SCL_SPEED_FM2K  200000


/*
 * @I2C_AckControl
 */
#define I2C_ACK_ENABLE        1
#define I2C_ACK_DISABLE       0

#define I2C_CCR_F_S				15
#define I2C_CCR_DUTY			14

/*
 * @I2C_FMDutyCycle
 */
#define I2C_FM_DUTY_2        0
#define I2C_FM_DUTY_16_9     1


/*
 * I2C related status flags definitions
 */
#define I2C_FLAG_TXE   		( 1 << I2C_SR1_TXE)
#define I2C_FLAG_RXNE   	( 1 << I2C_SR1_RXNE)
#define I2C_FLAG_SB			( 1 << I2C_SR1_SB)
#define I2C_FLAG_OVR  		( 1 << I2C_SR1_OVR)
#define I2C_FLAG_AF   		( 1 << I2C_SR1_AF)
#define I2C_FLAG_ARLO 		( 1 << I2C_SR1_ARLO)
#define I2C_FLAG_BERR 		( 1 << I2C_SR1_BERR)
#define I2C_FLAG_STOPF 		( 1 << I2C_SR1_STOPF)
#define I2C_FLAG_ADD10 		( 1 << I2C_SR1_ADD10)
#define I2C_FLAG_BTF  		( 1 << I2C_SR1_BTF)
#define I2C_FLAG_ADDR 		( 1 << I2C_SR1_ADDR)
#define I2C_FLAG_TIMEOUT 	( 1 << I2C_SR1_TIMEOUT)

#define I2C_DISABLE_SR  	RESET
#define I2C_ENABLE_SR   	SET


/*
 * I2C application events macros
 */
#define I2C_EV_TX_CMPLT  	 	0
#define I2C_EV_RX_CMPLT  	 	1
#define I2C_EV_STOP       		2
#define I2C_ERROR_BERR 	 		3
#define I2C_ERROR_ARLO  		4
#define I2C_ERROR_AF    		5
#define I2C_ERROR_OVR   		6
#define I2C_ERROR_TIMEOUT 		7
#define I2C_EV_DATA_REQ         8
#define I2C_EV_DATA_RCV         9

/******************************************************************************************
 *								APIs supported by this driver
 *		 For more information about the APIs check the function definitions
 ******************************************************************************************/
/*
 * Peripheral Clock setup
 */
void I2C_PeriClockControl(uint8_t I2Cx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void I2C_Init(I2C_Handle_t I2CHandle);
void I2C_DeInit(uint8_t I2Cx);


/*
 * Data Send and Receive
 */
void I2C_MasterSendData(I2C_Handle_t I2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr);
void I2C_MasterReceiveData(I2C_Handle_t I2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr);
uint8_t I2C_MasterSendDataIT(I2C_Handle_t I2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr);
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t I2CHandle,uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr);

void I2C_CloseReceiveData(I2C_Handle_t I2CHandle);
void I2C_CloseSendData(I2C_Handle_t I2CHandle);


void I2C_SlaveSendData(uint8_t I2C,uint8_t data);
uint8_t I2C_SlaveReceiveData(uint8_t I2Cx);

/*
 * IRQ Configuration and ISR handling
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void I2C_EV_IRQHandling(I2C_Handle_t I2CHandle);
void I2C_ER_IRQHandling(I2C_Handle_t I2CHandle);


/*
 * Other Peripheral Control APIs
 */
void I2C_PeripheralControl(uint8_t I2Cx, uint8_t Status);
uint8_t I2C_GetFlagStatus(uint8_t I2Cx , uint32_t FlagName);
void I2C_ManageAcking(uint8_t I2Cx, uint8_t Status);
void I2C_GenerateStopCondition(uint8_t I2Cx);


static void  I2C_GenerateStartCondition(uint8_t I2Cx);
static void I2C_ExecuteAddressPhase(uint8_t I2Cx, uint8_t SlaveAddr,uint8_t RnW);
static void I2C_ADDR_MstrClear(I2C_Handle_t I2CHandle);
static void I2C_ADDR_SlvClear(I2C_Handle_t I2CHandle);




static void ConfigureCCRVal(I2C_Handle_t I2CHandle);
static void ConfigureTRISEVal(I2C_Handle_t I2CHandle);



static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t I2CHandle );
static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t I2CHandle );

void I2C_SlaveEnableDisableCallbackEvents(uint8_t I2Cx,uint8_t EnorDi);
static uint8_t GetLSBSetBit(uint32_t Val);
/*
 * Application callback
 */
void I2C_ApplicationEventCallback(I2C_Handle_t I2CHandle,uint8_t AppEv);

#endif
