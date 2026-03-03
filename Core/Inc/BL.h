#ifndef INC_BL_H_
#define INC_BL_H_

#define BL_GET_VER              0X51
#define BL_GET_HELP             0X52
#define BL_GET_CID              0X53
#define BL_GET_RDP_STATUS       0X54
#define BL_GO_TO_ADDRESS        0X55
#define BL_FLASH_ERASE          0X56
#define BL_MEM_WRITE            0X57
#define BL_EN_RW_PROTECT        0X58
#define BL_MEM_READ             0X59
#define BL_READ_SECOR_STATUS    0X5A
#define BL_OTP_READ             0X5B
#define BL_DIS_RW_PROTECT       0X5C

#define BL_ACK                  0XA5
#define BL_NACK                 0X7F

void BL_voidHandleGetVerCmd(uint8_t* copypu8CmdPacket);
void BL_voidHandleGetHelpCmd(uint8_t* copypu8CmdPacket);
void BL_voidHandleGetCIDCmd(uint8_t* copypu8CmdPacket);
void BL_voidHandleGetRDPStatusCmd(uint8_t* copypu8CmdPacket);
void BL_voidHandleGoToAddressCmd(uint8_t* copypu8CmdPacket);
void BL_voidHandleFlashEraseCmd(uint8_t* copypu8CmdPacket);
void BL_voidHandleMemWriteCmd(uint8_t* copypu8CmdPacket);
void BL_voidHandleEnRWProtectCmd(uint8_t* copypu8CmdPacket);
void BL_voidHandleMemReadCmd(uint8_t* copypu8CmdPacket);
void BL_voidHandleReadSectorStatusCmd(uint8_t* copypu8CmdPacket);
void BL_voidHandleOTPReadCmd(uint8_t* copypu8CmdPacket);
void BL_voidHandleDisRWProtectCmd(uint8_t* copypu8CmdPacket);


#endif /* INC_BL_H_ */
