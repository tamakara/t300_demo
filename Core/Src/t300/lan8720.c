#include "t300/lan8720.h"

/**
 * @brief Read PHY register
 */
HAL_StatusTypeDef LAN8720_ReadReg(uint32_t reg, uint32_t *value)
{
  return HAL_ETH_ReadPHYRegister(&heth, LAN8720_PHY_ADDR, reg, value);
}

/**
 * @brief Write PHY register
 */
HAL_StatusTypeDef LAN8720_WriteReg(uint32_t reg, uint32_t value)
{
  return HAL_ETH_WritePHYRegister(&heth, LAN8720_PHY_ADDR, reg, value);
}

/**
 * @brief Reset LAN8720
 */
void LAN8720_Reset(void)
{
  HAL_GPIO_WritePin(ETH_RESET_GPIO_Port, ETH_RESET_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(ETH_RESET_GPIO_Port, ETH_RESET_Pin, GPIO_PIN_SET);
}

/**
 * @brief Get PHY Basic Status Register
 */
LAN8720_BasicStatus_t LAN8720_GetBasicStatus(void)
{
  LAN8720_BasicStatus_t status = {0};

  uint32_t value = 0;

  /* First read: clear latched link status */
  if (LAN8720_ReadReg(LAN8720_REG_BSR, &value) != HAL_OK)
  {
    return status;
  }

  /* Second read: obtain current status */
  if (LAN8720_ReadReg(LAN8720_REG_BSR, &value) != HAL_OK)
  {
    return status;
  }

  status.raw = value;

  status.link_status = (value & LAN8720_BSR_LINK_STATUS) ? 1U : 0U;

  status.autoneg_ability = (value & LAN8720_BSR_AUTONEGO_ABILITY) ? 1U : 0U;

  status.autoneg_complete = (value & LAN8720_BSR_AUTONEGO_COMPLETE) ? 1U : 0U;

  status.remote_fault = (value & LAN8720_BSR_REMOTE_FAULT) ? 1U : 0U;

  return status;
}

/**
 * @brief Get PHY Special Control/Status Register
 */
LAN8720_SpecialStatus_t LAN8720_GetSpecialStatus(void)
{
  LAN8720_SpecialStatus_t status = {0};

  uint32_t value = 0;

  if (LAN8720_ReadReg(LAN8720_REG_SCSR, &value) != HAL_OK)
  {
    return status;
  }

  status.raw = value;

  /* SCSR MODE: Bit[4:2] */
  switch ((value >> 2) & 0x07U)
  {
  case 0x01:
    status.speed = LAN8720_SPEED_10M;
    status.duplex = LAN8720_DUPLEX_HALF;
    break;

  case 0x05:
    status.speed = LAN8720_SPEED_10M;
    status.duplex = LAN8720_DUPLEX_FULL;
    break;

  case 0x02:
    status.speed = LAN8720_SPEED_100M;
    status.duplex = LAN8720_DUPLEX_HALF;
    break;

  case 0x06:
    status.speed = LAN8720_SPEED_100M;
    status.duplex = LAN8720_DUPLEX_FULL;
    break;

  default:
    status.speed = LAN8720_SPEED_UNKNOWN;
    status.duplex = LAN8720_DUPLEX_UNKNOWN;
    break;
  }

  return status;
}