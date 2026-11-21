#ifndef __LAN8720_H
#define __LAN8720_H

#include "eth.h"
#include "gpio.h"

/* PHY Address */
#define LAN8720_PHY_ADDR 0x00U

/* PHY Registers */
#define LAN8720_REG_BCR 0x00U
#define LAN8720_REG_BSR 0x01U
#define LAN8720_REG_PHYID1 0x02U
#define LAN8720_REG_PHYID2 0x03U
#define LAN8720_REG_SCSR 0x1FU

/* BSR Bits */
#define LAN8720_BSR_LINK_STATUS (1U << 2)
#define LAN8720_BSR_AUTONEGO_ABILITY (1U << 3)
#define LAN8720_BSR_REMOTE_FAULT (1U << 4)
#define LAN8720_BSR_AUTONEGO_COMPLETE (1U << 5)

/* PHY Link Status */
typedef enum
{
  LAN8720_LINK_DOWN = 0,
  LAN8720_LINK_UP
} LAN8720_LinkStatus_t;

/* PHY Speed */
typedef enum
{
  LAN8720_SPEED_UNKNOWN = 0,
  LAN8720_SPEED_10M,
  LAN8720_SPEED_100M
} LAN8720_Speed_t;

/* PHY Duplex */
typedef enum
{
  LAN8720_DUPLEX_UNKNOWN = 0,
  LAN8720_DUPLEX_HALF,
  LAN8720_DUPLEX_FULL
} LAN8720_Duplex_t;

/* Basic Status Register */
typedef struct
{
  uint32_t raw;

  uint8_t link_status;
  uint8_t autoneg_ability;
  uint8_t autoneg_complete;
  uint8_t remote_fault;

} LAN8720_BasicStatus_t;

/* Special Control/Status Register */
typedef struct
{
  uint32_t raw;

  LAN8720_Speed_t speed;
  LAN8720_Duplex_t duplex;

} LAN8720_SpecialStatus_t;

/* Basic Functions */
HAL_StatusTypeDef LAN8720_ReadReg(uint32_t reg, uint32_t *value);
HAL_StatusTypeDef LAN8720_WriteReg(uint32_t reg, uint32_t value);

void LAN8720_Reset(void);

/* Status Functions */
LAN8720_BasicStatus_t LAN8720_GetBasicStatus(void);
LAN8720_SpecialStatus_t LAN8720_GetSpecialStatus(void);

#endif