
#include "soft_iic.h"

  	   		   
//IO	操作脚 
//SCL
//SDA	 
//输入SDA 

#define SOFT_IIC_SDA_PORT	GPIOB
#define SOFT_IIC_SDA_PIN	GPIO_Pin_7
#define SOFT_IIC_SCL_PORT	GPIOB
#define SOFT_IIC_SCL_PIN	GPIO_Pin_6
#define SOFT_IIC_GPIO_PeriphClock_EN()  do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); }while(0)

#define SOFT_IIC_SDA_INPUT()	{GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x80000000;}
#define SOFT_IIC_SDA_OUTPUT()	{GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x30000000;}

#define SOFT_IIC_SDA_HIGH()		PBout(7) = 1
#define SOFT_IIC_SDA_LOW()		PBout(7) = 0
#define SOFT_IIC_SCL_HIGH()		PBout(6) = 1
#define SOFT_IIC_SCL_LOW()		PBout(6) = 0
#define SOFT_IIC_SDA_READ		PBin(7)
#define SOFT_IIC_DELAY_WIDE()	delay_us(1)


void IIC_Init(void)
{			
	GPIO_InitTypeDef GPIO_InitStructure;
	
	SOFT_IIC_GPIO_PeriphClock_EN();			     

	GPIO_InitStructure.GPIO_Pin = SOFT_IIC_SCL_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(SOFT_IIC_SCL_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SOFT_IIC_SDA_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(SOFT_IIC_SDA_PORT, &GPIO_InitStructure);
}
static void IIC_Start(void)
{
	SOFT_IIC_SDA_OUTPUT();     //sda set output
	SOFT_IIC_SDA_HIGH();	  	  
	SOFT_IIC_SCL_HIGH();
	SOFT_IIC_DELAY_WIDE();
	SOFT_IIC_DELAY_WIDE();
	SOFT_IIC_DELAY_WIDE();
	SOFT_IIC_DELAY_WIDE();
 	SOFT_IIC_SDA_LOW();		//START:when CLK is high,DATA change form high to low 
	SOFT_IIC_DELAY_WIDE();
	SOFT_IIC_DELAY_WIDE();
	SOFT_IIC_DELAY_WIDE();
	SOFT_IIC_DELAY_WIDE();
	SOFT_IIC_SCL_LOW();		//ready to send data or receive data
}

static void IIC_Stop(void)
{
	SOFT_IIC_SDA_OUTPUT();	//sda set output
	SOFT_IIC_SCL_LOW();
	SOFT_IIC_SDA_LOW();		//STOP:when CLK is high DATA change form low to high
	SOFT_IIC_DELAY_WIDE();
	SOFT_IIC_DELAY_WIDE();
	SOFT_IIC_DELAY_WIDE();
	SOFT_IIC_DELAY_WIDE();
	SOFT_IIC_SCL_HIGH(); 
	SOFT_IIC_SDA_HIGH();	//send stop signal
	SOFT_IIC_DELAY_WIDE();
	SOFT_IIC_DELAY_WIDE();
	SOFT_IIC_DELAY_WIDE();
	SOFT_IIC_DELAY_WIDE();							   	
}

//0: success ,1 : failed
static uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime = 0;
	
	SOFT_IIC_SDA_INPUT();      //SDA set input  
	SOFT_IIC_SDA_HIGH();
	SOFT_IIC_DELAY_WIDE();
	SOFT_IIC_SCL_HIGH();
	SOFT_IIC_DELAY_WIDE();
	while(SOFT_IIC_SDA_READ)
	{
		ucErrTime++;
		if(ucErrTime > 50){
			IIC_Stop();
			return 1;
		}
		SOFT_IIC_DELAY_WIDE();
	}
	SOFT_IIC_SCL_LOW();
	
	return 0;  
} 

// 产生应答
static void IIC_Ack(void)
{
	SOFT_IIC_SCL_LOW();
	SOFT_IIC_SDA_OUTPUT();
	SOFT_IIC_SDA_LOW();
	SOFT_IIC_DELAY_WIDE();
	SOFT_IIC_SCL_HIGH();
	SOFT_IIC_DELAY_WIDE();
	SOFT_IIC_SCL_LOW();
}
//产生非应答	
static void IIC_NAck(void)
{
	SOFT_IIC_SCL_LOW();
	SOFT_IIC_SDA_OUTPUT();
	SOFT_IIC_SDA_HIGH();
	SOFT_IIC_DELAY_WIDE();
	SOFT_IIC_SCL_HIGH();
	SOFT_IIC_DELAY_WIDE();
	SOFT_IIC_SCL_LOW();
}					 				     

static void IIC_Send_Byte(uint8_t txbyte)
{                        
	uint8_t i;

	SOFT_IIC_SDA_OUTPUT(); 	    
	SOFT_IIC_SCL_LOW();		//begin to send data
	for(i = 0; i < 8; i++){
		if(txbyte & 0x80)
			SOFT_IIC_SDA_HIGH();
		else
			SOFT_IIC_SDA_LOW();
		txbyte <<= 1; 	  
		SOFT_IIC_DELAY_WIDE();   
		SOFT_IIC_SCL_HIGH();
		SOFT_IIC_DELAY_WIDE(); 
		SOFT_IIC_SCL_LOW();	
		SOFT_IIC_DELAY_WIDE();
	}	 
} 	 
   
static uint8_t IIC_Read_Byte(uint8_t ack)
{
	uint8_t i,receive = 0;

	SOFT_IIC_SDA_INPUT();//SDA set input
	for(i = 0; i < 8; i++ ){
		SOFT_IIC_SCL_LOW(); 
		SOFT_IIC_DELAY_WIDE();
		SOFT_IIC_SCL_HIGH();
		receive <<= 1;
		if(SOFT_IIC_SDA_READ)
			receive |= 0x01;   
		SOFT_IIC_DELAY_WIDE();
	}					 
	if(ack)
		IIC_Ack(); //send ACK 
	else
		IIC_NAck();//send NACK

	return receive;
}

bool IICreadBuf(uint8_t devAddress, uint8_t memAddress, uint8_t length, uint8_t *data)
{
    uint8_t i = 0;
	uint8_t temp;
	
	IIC_Start();
	IIC_Send_Byte(devAddress);	   //send devAddress address and write
	IIC_Wait_Ack();
	if(memAddress != IICDEV_NO_MEM_ADDR){
		IIC_Send_Byte(memAddress);   // send memAddress address 
    	IIC_Wait_Ack();	
	}
	
	IIC_Start();
	IIC_Send_Byte(devAddress | 0x01); //send devAddress address and read 	
	IIC_Wait_Ack();
	
    for(i = 0; i < length; i++){ 
		 if(i != (length - 1))
		 	temp = IIC_Read_Byte(1);  //read with ack
		 else
			temp = IIC_Read_Byte(0);	//last byte with nack

		data[i] = temp;
	}
    IIC_Stop();//stop
    
    return true;
}


bool IICreadByte(uint8_t devAddress, uint8_t memAddress, uint8_t *data)
{
	return IICreadBuf(devAddress, memAddress, 1, data);
}
bool IICreadBit(uint8_t devAddress, uint8_t memAddress, uint8_t bitNum, uint8_t *data)
{
	uint8_t byte;
	bool status;

	status = IICreadBuf( devAddress, memAddress, 1, &byte);
	*data = (byte >> bitNum )& 0x01;

	return status;
}
bool IICreadBits(uint8_t devAddress, uint8_t memAddress, uint8_t bitStart, uint8_t length, uint8_t *data)
{
	uint8_t byte;
	uint8_t mask;
	bool status;

	if((status = IICreadBuf( devAddress, memAddress, 1, &byte)) == true){
		mask = (1 << length) - 1;
		byte >>= (bitStart - length + 1);
		byte &= mask;
		*data = byte;
	}
	
	return status;
}
bool IICwriteBuf(uint8_t devAddress, uint8_t memAddress, uint8_t length, uint8_t* data)
{
	uint8_t i = 0;

	IIC_Start();
	IIC_Send_Byte(devAddress);	   //send devAddress address and write
	IIC_Wait_Ack();
	if(memAddress != IICDEV_NO_MEM_ADDR){
		IIC_Send_Byte(memAddress);  // send memAddress address 
		IIC_Wait_Ack();	
	}
	for(i = 0; i < length; i++){
		IIC_Send_Byte(data[i]); 
		IIC_Wait_Ack(); 
	}
	IIC_Stop();//stop

	return true;
}

bool IICwriteByte(uint8_t devAddress, uint8_t memAddress, uint8_t data)
{
    return IICwriteBuf(devAddress, memAddress, 1, &data);
}




bool IICwriteBits(uint8_t devAddress, uint8_t memAddress, uint8_t bitStart, uint8_t length, uint8_t data)
{
    uint8_t byte;
	uint8_t mask;
	bool status;
		
    if ((status = IICwriteBuf(devAddress, memAddress, 1, &byte)) == true) {
        mask = ((1 << length) - 1) << (bitStart - length + 1);
        data <<= (bitStart - length + 1);
        data &= mask;
        byte &= ~mask;
        byte |= data;
        return IICwriteBuf(devAddress, memAddress, 1, &byte);
    } 
	
	return status;
}
bool IICwriteBit(uint8_t devAddress, uint8_t memAddress, uint8_t bitNum, uint8_t data)
{
    uint8_t byte;
    
    IICwriteBuf(devAddress, memAddress, 1, &byte);
    byte = (data != 0) ? (byte | (1 << bitNum)) : (byte & ~(1 << bitNum));
    
    return IICwriteBuf(devAddress, memAddress, 1, &byte);
}

//------------------End of File-------------

