/*
 * BL.c
 *
 *  Created on: Dec 1, 2024
 *      Author: workstation
 */

#include <stdint.h>
#include "BL.h"
#include "main.h"
#include "BL_prv.h"

extern CRC_HandleTypeDef hcrc;
extern UART_HandleTypeDef huart2;



static uint8_t u8VerifyCRC(uint8_t* copy_puDataArr,uint8_t copy_u8Length,uint32_t copy_u32HostCRC)
{
	uint8_t Local_u8Iterator, Local_u8CRCStatus;
	uint32_t Local_u32AccCRC ,Local_u32Temp;

	for(Local_u8Iterator=0; Local_u8Iterator<copy_u8Length; Local_u8Iterator++)
	{
		Local_u32Temp=copy_puDataArr[Local_u8Iterator];
		Local_u32AccCRC=HAL_CRC_Accumulate(&hcrc, &Local_u32Temp,1 );


	}
	//reset crc calcuation unit
	__HAL_CRC_DR_RESET(&hcrc);
	if(Local_u32AccCRC==copy_u32HostCRC)
	{
		Local_u8CRCStatus= CRC_SUCCESS;
	}
	else
	{
		Local_u8CRCStatus= CRC_FAIL;
	}


	return Local_u8CRCStatus;


}


static void voidSendAck(uint8_t copy_u8ReplyLength)
{
	uint8_t Local_u8AckBuffer[2]={BL_ACK,copy_u8ReplyLength};
	HAL_UART_Transmit(&huart2, Local_u8AckBuffer, 2, HAL_MAX_DELAY);

}
static void voidSendNAck(void)
{
	uint8_t Local_u8NAck=BL_NACK;
	HAL_UART_Transmit(&huart2, & Local_u8NAck, 1, HAL_MAX_DELAY);

}

void BL_voidHandleGetVerCmd(uint8_t* copypu8CmdPacket)
{
	uint8_t Local_u8BLVersion, Local_u8CRCStatus, Local_u8CmdLength;
	uint32_t Local_u32HostCRC;

	Local_u8CmdLength=copypu8CmdPacket[0]+1;  //the first byte already includes the length to follow
	Local_u32HostCRC=*((uint32_t*)(copypu8CmdPacket /* address of the first byte of the packet*/+ Local_u8CmdLength/*moves the pointer to the end of the packet*/ - 4)); //I need to get the CRC value from the address it's located at
    Local_u8CRCStatus = u8VerifyCRC(copypu8CmdPacket, Local_u8CmdLength-4, Local_u32HostCRC);
	if(Local_u8CRCStatus==CRC_SUCCESS)
	{
		voidSendAck(1u); //this bootloader version has length to follow of size 1 byte
		Local_u8BLVersion=BL_VERSION;
		HAL_UART_Transmit(&huart2, &Local_u8BLVersion, 1, HAL_MAX_DELAY);
	}
	else
	{
		voidSendNAck();
	}



}
void BL_voidHandleGetHelpCmd(uint8_t* copypu8CmdPacket)
{
	    uint8_t Local_u8CRCStatus, Local_u8CmdLength;
		uint32_t Local_u32HostCRC;

		Local_u8CmdLength=copypu8CmdPacket[0]+1;  //the first byte already includes the length to follow
		Local_u32HostCRC=*((uint32_t*)(copypu8CmdPacket /* address of the first byte of the packet*/+ Local_u8CmdLength/*moves the pointer to the end of the packet*/ - 4)); //I need to get the CRC value from the address it's located at
	    Local_u8CRCStatus = u8VerifyCRC(copypu8CmdPacket, Local_u8CmdLength-4, Local_u32HostCRC);
		if(Local_u8CRCStatus==CRC_SUCCESS)
		{
			uint8_t Local_pu8BLCommands[]=
			{
					BL_GET_VER           ,
					BL_GET_HELP          ,
					BL_GET_CID           ,
					BL_GET_RDP_STATUS    ,
					BL_GO_TO_ADDRESS     ,
					BL_FLASH_ERASE       ,
					BL_MEM_WRITE         ,
					BL_EN_RW_PROTECT     ,
					BL_MEM_READ          ,
					BL_READ_SECOR_STATUS ,
					BL_OTP_READ          ,
					BL_DIS_RW_PROTECT
			};
			voidSendAck(sizeof(Local_pu8BLCommands)); //this bootloader version has length to follow of size 1 byte
			HAL_UART_Transmit(&huart2, Local_pu8BLCommands, sizeof(Local_pu8BLCommands), HAL_MAX_DELAY);

		}
		else
		{
			voidSendNAck();
		}

}
void BL_voidHandleGetCIDCmd(uint8_t* copypu8CmdPacket)
{
	uint8_t Local_u8CRCStatus, Local_u8CmdLength;
			uint32_t Local_u32HostCRC;

			Local_u8CmdLength=copypu8CmdPacket[0]+1;  //the first byte already includes the length to follow
			Local_u32HostCRC=*((uint32_t*)(copypu8CmdPacket /* address of the first byte of the packet*/+ Local_u8CmdLength/*moves the pointer to the end of the packet*/ - 4)); //I need to get the CRC value from the address it's located at
		    Local_u8CRCStatus = u8VerifyCRC(copypu8CmdPacket, Local_u8CmdLength-4, Local_u32HostCRC);
			if(Local_u8CRCStatus==CRC_SUCCESS)
			{
				uint16_t Local_u16DeviceID = (DBGMCU_IDCODE & 0x0fff); //to only get the first 12 bits
				voidSendAck(2u);
				HAL_UART_Transmit(&huart2, (uint8_t*)&Local_u16DeviceID, 2, HAL_MAX_DELAY);

			}
			else
			{

				voidSendNAck();
			}

}
void BL_voidHandleGetRDPStatusCmd(uint8_t* copypu8CmdPacket)
{
	uint8_t Local_u8CRCStatus, Local_u8CmdLength;
			uint32_t Local_u32HostCRC;

			Local_u8CmdLength=copypu8CmdPacket[0]+1;  //the first byte already includes the length to follow
			Local_u32HostCRC=*((uint32_t*)(copypu8CmdPacket /* address of the first byte of the packet*/+ Local_u8CmdLength/*moves the pointer to the end of the packet*/ - 4)); //I need to get the CRC value from the address it's located at
		    Local_u8CRCStatus = u8VerifyCRC(copypu8CmdPacket, Local_u8CmdLength-4, Local_u32HostCRC);
			if(Local_u8CRCStatus==CRC_SUCCESS)
			{
				uint8_t Local_u8RDPStatus=(uint8_t)((RDP_USER_OPTION_WORD >> 8) & 0Xff);
				voidSendAck(1u);
				HAL_UART_Transmit(&huart2, &Local_u8RDPStatus, 1, HAL_MAX_DELAY);

			}
			else
			{
				voidSendNAck();
			}

}
void
BL_voidHandleGoToAddressCmd(uint8_t* copypu8CmdPacket)
{
	uint8_t Local_u8CRCStatus, Local_u8CmdLength;
			uint32_t Local_u32HostCRC;

			Local_u8CmdLength=copypu8CmdPacket[0]+1;  //the first byte already includes the length to follow
			Local_u32HostCRC=*((uint32_t*)(copypu8CmdPacket /* address of the first byte of the packet*/+ Local_u8CmdLength/*moves the pointer to the end of the packet*/ - 4)); //I need to get the CRC value from the address it's located at
		    Local_u8CRCStatus = u8VerifyCRC(copypu8CmdPacket, Local_u8CmdLength-4, Local_u32HostCRC);
			if(Local_u8CRCStatus==CRC_SUCCESS)
			{
				uint32_t Local_u32Address;
				voidSendAck(1u);
				Local_u32Address=*((uint32_t*)&copypu8CmdPacket[2]); //&copypu8CmdPacket[2]->means a pointer to the address of index 2 of type uint8
			    uint8_t Local_u8ValidAddressStatus = u8DataValidate(Local_u32Address);

			    HAL_UART_Transmit(&huart2, &Local_u8ValidAddressStatus  , 1, HAL_MAX_DELAY);

			    if(Local_u8ValidAddressStatus == VALID_ADDRESS)
			    {


			    	void(*Local_pvPtrFunc)(void)=NULL;
			    	Local_u32Address |= 1 ; //to make t-bit =1
			    	Local_pvPtrFunc=(void*)Local_u32Address;
			    	Local_pvPtrFunc();


			    }
			    else
			    {
			    }


			}



			else
			{
				voidSendNAck();
			}

}

static uint8_t u8DataValidate (uint32_t Copy_u32Address)
{
	//Address is valid if it's within SRAM or flash
	uint8_t Local_u8AddressStatus;
	if((Copy_u32Address >= FLASH_BASE) && (Copy_u32Address <= FLASH_END))
	{
		Local_u8AddressStatus=VALID_ADDRESS;
	}
	else if((Copy_u32Address >= SRAM1_BASE) && (Copy_u32Address <= SRAM1_BASE+(128*1024)))
    {
		Local_u8AddressStatus=VALID_ADDRESS;

	}
	else
	{
		Local_u8AddressStatus=INVALID_ADDRESS;
	}



	return Local_u8AddressStatus;

}


void BL_voidHandleFlashEraseCmd(uint8_t* copypu8CmdPacket)
{
	uint8_t Local_u8CRCStatus, Local_u8CmdLength,Local_u8EraseStatus ;
			uint32_t Local_u32HostCRC;

			Local_u8CmdLength=copypu8CmdPacket[0]+1;  //the first byte already includes the length to follow
			Local_u32HostCRC=*((uint32_t*)(copypu8CmdPacket /* address of the first byte of the packet*/+ Local_u8CmdLength/*moves the pointer to the end of the packet*/ - 4)); //I need to get the CRC value from the address it's located at
		    Local_u8CRCStatus = u8VerifyCRC(copypu8CmdPacket, Local_u8CmdLength-4, Local_u32HostCRC);
			if(Local_u8CRCStatus==CRC_SUCCESS)
			{
				voidSendAck(1u); //this bootloader version has length to follow of size 1 byte
				HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
				Local_u8EraseStatus=u8ExecuteFlashErase(copypu8CmdPacket[2], copypu8CmdPacket[3]);
				HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
				HAL_UART_Transmit(&huart2, &Local_u8EraseStatus, 1, HAL_MAX_DELAY);



			}
			else
			{
				voidSendNAck();
			}

}

static uint8_t u8ExecuteFlashErase(uint8_t Copy_u8SectorNumber,uint8_t Copy_u8NumberofSectors)
{
	 HAL_StatusTypeDef Local_ErrorStatus = HAL_OK;
	 if((Copy_u8NumberofSectors > 8) && (Copy_u8SectorNumber != 0xff))
	 {
		 Local_ErrorStatus=HAL_ERROR;
	 }
	 else if((Copy_u8SectorNumber > 7) && (Copy_u8SectorNumber != 0xff) )
	 {
		 Local_ErrorStatus=HAL_ERROR;
	 }
	 else
	 {
		 FLASH_EraseInitTypeDef Local_MyErase;
		 uint32_t Local_u32SectorError;

		 if(Copy_u8SectorNumber==0xff)
		 {
			 //mass erase id required
			 Local_MyErase.TypeErase = FLASH_TYPEERASE_MASSERASE;
		 }
		 else
		 {

			 //sector erase id required
			 uint8_t Local_u8RemainingSectors = 8 - Copy_u8SectorNumber;
			 if(Copy_u8NumberofSectors>Local_u8RemainingSectors)
			 {
				 Copy_u8NumberofSectors=Local_u8RemainingSectors;
			 }
			 else
			 {
				 //do nothing

			 }
			 Local_MyErase.TypeErase = FLASH_TYPEERASE_SECTORS;
			 Local_MyErase.Sector=Copy_u8SectorNumber;
			 Local_MyErase.NbSectors=Copy_u8NumberofSectors;

		 }
		 Local_MyErase.VoltageRange=FLASH_VOLTAGE_RANGE_3;
		 Local_MyErase.Banks=FLASH_BANK_1;

		 //Unlock flash before erasing
		 HAL_FLASH_Unlock();
		 Local_ErrorStatus=HAL_FLASHEx_Erase(&Local_MyErase, &Local_u32SectorError);
		 HAL_FLASH_Lock();



	 }



	 return Local_ErrorStatus;

}
void BL_voidHandleMemWriteCmd(uint8_t* copypu8CmdPacket)
{
	uint8_t Local_u8CRCStatus, Local_u8CmdLength;
			uint32_t Local_u32HostCRC;

			Local_u8CmdLength=copypu8CmdPacket[0]+1;  //the first byte already includes the length to follow
			Local_u32HostCRC=*((uint32_t*)(copypu8CmdPacket /* address of the first byte of the packet*/+ Local_u8CmdLength/*moves the pointer to the end of the packet*/ - 4)); //I need to get the CRC value from the address it's located at
		    Local_u8CRCStatus = u8VerifyCRC(copypu8CmdPacket, Local_u8CmdLength-4, Local_u32HostCRC);
			if(Local_u8CRCStatus==CRC_SUCCESS)
			{
				uint32_t Local_u32WritingStatus;
				uint32_t Local_u32Address = *((uint32_t*)&copypu8CmdPacket[2]);  //extract the address from the packet

				uint8_t Localu8AddressStatus = u8DataValidate(Copy_u32Address);
				if(Localu8AddressStatus==VALID_ADDRESS)
				{
					uint8_t Local_u8PayloadLength = copypu8CmdPacket[6];  //payload length field takes only one byte


				}
				else
				{
					Local_u32WritingStatus==WRITING_ERROR;

				}
				HAL_UART_Transmit(&huart2, &Local_u32WritingStatus, 1, HAL_MAX_DELAY);
				voidSendAck(1u); //this bootloader version has length to follow of size 1 byte

			}
			else
			{
				voidSendNAck();
			}

}

static uint8_t u8ExecuteMemWrite(uint8_t* copy_pu8DataArr, uint32_t copu_u32StartAddress,uint8_t copy_u8Length )
{

}
void BL_voidHandleEnRWProtectCmd(uint8_t* copypu8CmdPacket)
{
	uint8_t Local_u8CRCStatus, Local_u8CmdLength;
			uint32_t Local_u32HostCRC;

			Local_u8CmdLength=copypu8CmdPacket[0]+1;  //the first byte already includes the length to follow
			Local_u32HostCRC=*((uint32_t*)(copypu8CmdPacket /* address of the first byte of the packet*/+ Local_u8CmdLength/*moves the pointer to the end of the packet*/ - 4)); //I need to get the CRC value from the address it's located at
		    Local_u8CRCStatus = u8VerifyCRC(copypu8CmdPacket, Local_u8CmdLength-4, Local_u32HostCRC);
			if(Local_u8CRCStatus==CRC_SUCCESS)
			{
				voidSendAck(1u); //this bootloader version has length to follow of size 1 byte

			}
			else
			{
				voidSendNAck();
			}

}
void BL_voidHandleMemReadCmd(uint8_t* copypu8CmdPacket)
{
	uint8_t Local_u8CRCStatus, Local_u8CmdLength;
			uint32_t Local_u32HostCRC;

			Local_u8CmdLength=copypu8CmdPacket[0]+1;  //the first byte already includes the length to follow
			Local_u32HostCRC=*((uint32_t*)(copypu8CmdPacket /* address of the first byte of the packet*/+ Local_u8CmdLength/*moves the pointer to the end of the packet*/ - 4)); //I need to get the CRC value from the address it's located at
		    Local_u8CRCStatus = u8VerifyCRC(copypu8CmdPacket, Local_u8CmdLength-4, Local_u32HostCRC);
			if(Local_u8CRCStatus==CRC_SUCCESS)
			{
				voidSendAck(1u); //this bootloader version has length to follow of size 1 byte

			}
			else
			{
				voidSendNAck();
			}

}
void BL_voidHandleReadSectorStatusCmd(uint8_t* copypu8CmdPacket)
{
	uint8_t Local_u8CRCStatus, Local_u8CmdLength;
			uint32_t Local_u32HostCRC;

			Local_u8CmdLength=copypu8CmdPacket[0]+1;  //the first byte already includes the length to follow
			Local_u32HostCRC=*((uint32_t*)(copypu8CmdPacket /* address of the first byte of the packet*/+ Local_u8CmdLength/*moves the pointer to the end of the packet*/ - 4)); //I need to get the CRC value from the address it's located at
		    Local_u8CRCStatus = u8VerifyCRC(copypu8CmdPacket, Local_u8CmdLength-4, Local_u32HostCRC);
			if(Local_u8CRCStatus==CRC_SUCCESS)
			{
				voidSendAck(1u); //this bootloader version has length to follow of size 1 byte

			}
			else
			{
				voidSendNAck();
			}

}
void BL_voidHandleOTPReadCmd(uint8_t* copypu8CmdPacket)
{
	uint8_t Local_u8CRCStatus, Local_u8CmdLength;
			uint32_t Local_u32HostCRC;

			Local_u8CmdLength=copypu8CmdPacket[0]+1;  //the first byte already includes the length to follow
			Local_u32HostCRC=*((uint32_t*)(copypu8CmdPacket /* address of the first byte of the packet*/+ Local_u8CmdLength/*moves the pointer to the end of the packet*/ - 4)); //I need to get the CRC value from the address it's located at
		    Local_u8CRCStatus = u8VerifyCRC(copypu8CmdPacket, Local_u8CmdLength-4, Local_u32HostCRC);
			if(Local_u8CRCStatus==CRC_SUCCESS)
			{
				voidSendAck(1u); //this bootloader version has length to follow of size 1 byte

			}
			else
			{
				voidSendNAck();
			}

}
void BL_voidHandleDisRWProtectCmd(uint8_t* copypu8CmdPacket)
{

	uint8_t Local_u8CRCStatus, Local_u8CmdLength;
			uint32_t Local_u32HostCRC;

			Local_u8CmdLength=copypu8CmdPacket[0]+1;  //the first byte already includes the length to follow
			Local_u32HostCRC=*((uint32_t*)(copypu8CmdPacket /* address of the first byte of the packet*/+ Local_u8CmdLength/*moves the pointer to the end of the packet*/ - 4)); //I need to get the CRC value from the address it's located at
		    Local_u8CRCStatus = u8VerifyCRC(copypu8CmdPacket, Local_u8CmdLength-4, Local_u32HostCRC);
			if(Local_u8CRCStatus==CRC_SUCCESS)
			{
				voidSendAck(1u); //this bootloader version has length to follow of size 1 byte

			}
			else
			{
				voidSendNAck();
			}
}














