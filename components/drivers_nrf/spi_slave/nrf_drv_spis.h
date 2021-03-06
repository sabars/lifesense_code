/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */
 
/**@file
 * @defgroup nrf_spis SPI slave HAL and driver
 * @ingroup nrf_spi
 * @brief SPI slave API.
 * @details The SPIS HAL provides basic APIs for accessing the registers 
 * of the SPIS. The SPIS driver provides APIs on a higher level.
 **/

#ifndef SPI_SLAVE_H__
#define SPI_SLAVE_H__

#include <stdint.h>
#include "nrf.h"
#include "nrf_error.h"
#include "nrf_drv_config.h"
#include "nrf_spis.h"
#include "nrf_gpio.h"
#include "sdk_common.h"
#include "app_util_platform.h"

#if defined(NRF52)
    #define SPIS2_IRQ            SPIM2_SPIS2_SPI2_IRQn
    #define SPIS2_IRQ_HANDLER    SPIM2_SPIS2_SPI2_IRQHandler
    #define SPIS0_IRQ            SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn
    #define SPIS0_IRQ_HANDLER    SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler
    #define SPIS1_IRQ            SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn
    #define SPIS1_IRQ_HANDLER    SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler
#else
    #define SPIS0_IRQ            SPI0_TWI0_IRQn
    #define SPIS0_IRQ_HANDLER    SPI0_TWI0_IRQHandler
    #define SPIS1_IRQ            SPI1_TWI1_IRQn
    #define SPIS1_IRQ_HANDLER    SPI1_TWI1_IRQHandler
#endif

/**
 * @defgroup nrf_drv_spi_slave SPI slave driver
 * @{
 * @ingroup  nrf_spis
 *
 * @brief    Multi-instance SPI slave driver.
 */
 
#define NRF_DRV_SPIS_DEFAULT_CSN_PULLUP NRF_GPIO_PIN_NOPULL /**< Default pull-up configuration of the SPI CS. */
#define NRF_DRV_SPIS_DEFAULT_MISO_DRIVE NRF_GPIO_PIN_S0S1   /**< Default drive configuration of the SPI MISO. */
#define NRF_DRV_SPIS_DEFAULT_DEF        0xFF                /**< Default DEF character. */
#define NRF_DRV_SPIS_DEFAULT_ORC        0xFF                /**< Default ORC character. */
 
/**
* @brief This value can be provided instead of a pin number for the signals MOSI
*        and MISO to specify that the given signal is not used and therefore
*        does not need to be connected to a pin.
*/
#define NRF_DRV_SPIS_PIN_NOT_USED       0xFF

/** @brief SPIS transaction bit order definitions. */
typedef enum
{
    NRF_DRV_SPIS_BIT_ORDER_LSB_FIRST = NRF_SPIS_BIT_ORDER_LSB_FIRST, /**< Least significant bit shifted out first. */
    NRF_DRV_SPIS_BIT_ORDER_MSB_FIRST = NRF_SPIS_BIT_ORDER_MSB_FIRST  /**< Most significant bit shifted out first. */
} nrf_drv_spis_endian_t;

/** @brief SPIS mode definitions for clock polarity and phase. */
typedef enum
{
    NRF_DRV_SPIS_MODE_0 = NRF_SPIS_MODE_0,       /**< (CPOL = 0, CPHA = 0). */
    NRF_DRV_SPIS_MODE_1 = NRF_SPIS_MODE_1,       /**< (CPOL = 0, CPHA = 1). */
    NRF_DRV_SPIS_MODE_2 = NRF_SPIS_MODE_2,       /**< (CPOL = 1, CPHA = 0). */
    NRF_DRV_SPIS_MODE_3 = NRF_SPIS_MODE_3        /**< (CPOL = 1, CPHA = 1). */
} nrf_drv_spis_mode_t;

/** @brief Event callback function event definitions. */
typedef enum
{
    NRF_DRV_SPIS_BUFFERS_SET_DONE,          /**< Memory buffer set event. Memory buffers have been set successfully to the SPI slave device, and SPI transactions can be done. */
    NRF_DRV_SPIS_XFER_DONE,                 /**< SPI transaction event. SPI transaction has been completed. */  
    NRF_DRV_SPIS_EVT_TYPE_MAX                    /**< Enumeration upper bound. */      
} nrf_drv_spis_event_type_t;

/** @brief Structure containing the event context from the SPI slave driver. */
typedef struct
{
    nrf_drv_spis_event_type_t evt_type;     //!< Type of event.
    uint32_t                  rx_amount;    //!< Number of bytes received in last transaction. This parameter is only valid for @ref NRF_DRV_SPIS_XFER_DONE events.
    uint32_t                  tx_amount;    //!< Number of bytes transmitted in last transaction. This parameter is only valid for @ref NRF_DRV_SPIS_XFER_DONE events.
} nrf_drv_spis_event_t;

/** @brief SPI slave driver instance data structure. */
typedef struct
{
    NRF_SPIS_Type * p_reg;          //!< SPIS instance register.
    uint8_t         instance_id;    //!< SPIS instance ID.
    IRQn_Type       irq;            //!< IRQ of the specific instance.
} nrf_drv_spis_t;

/** @brief Macro for creating an SPI slave driver instance. */
#define NRF_DRV_SPIS_INSTANCE(id)                        \
{                                                        \
    .p_reg        = CONCAT_2(NRF_SPIS, id),              \
    .irq          = CONCAT_3(SPIS, id, _IRQ),            \
    .instance_id  = CONCAT_3(SPIS, id, _INSTANCE_INDEX), \
}

/** @brief SPI slave instance default configuration. */
#define NRF_DRV_SPIS_DEFAULT_CONFIG(id)                       \
{                                                             \
    .sck_pin      = CONCAT_3(SPIS, id, _CONFIG_SCK_PIN),      \
    .mosi_pin     = CONCAT_3(SPIS, id, _CONFIG_MOSI_PIN),     \
    .miso_pin     = CONCAT_3(SPIS, id, _CONFIG_MISO_PIN),     \
    .csn_pin      = NRF_DRV_SPIS_PIN_NOT_USED,                \
    .miso_drive   = NRF_DRV_SPIS_DEFAULT_MISO_DRIVE,          \
    .csn_pullup   = NRF_DRV_SPIS_DEFAULT_CSN_PULLUP,          \
    .orc          = NRF_DRV_SPIS_DEFAULT_ORC,                 \
    .def          = NRF_DRV_SPIS_DEFAULT_DEF,                 \
    .mode         = NRF_DRV_SPIS_MODE_0,                      \
    .bit_order    = NRF_DRV_SPIS_BIT_ORDER_MSB_FIRST,         \
    .irq_priority = CONCAT_3(SPIS, id, _CONFIG_IRQ_PRIORITY), \
}

/** @brief SPI peripheral device configuration data. */
typedef struct 
{
    uint32_t              miso_pin;            //!< SPI MISO pin (optional).
                                               /**< Set @ref NRF_DRV_SPIS_PIN_NOT_USED
                                                *   if this signal is not needed. */
    uint32_t              mosi_pin;            //!< SPI MOSI pin (optional).
                                               /**< Set @ref NRF_DRV_SPIS_PIN_NOT_USED
                                                *   if this signal is not needed. */
    uint32_t              sck_pin;             //!< SPI SCK pin.
    uint32_t              csn_pin;             //!< SPI CSN pin.
    nrf_drv_spis_mode_t   mode;                //!< SPI mode.
    nrf_drv_spis_endian_t bit_order;           //!< SPI transaction bit order.
    nrf_gpio_pin_pull_t   csn_pullup;          //!< CSN pin pull-up configuration.
    nrf_gpio_pin_drive_t  miso_drive;          //!< MISO pin drive configuration.
    uint8_t               def;                 //!< Character clocked out in case of an ignored transaction.
    uint8_t               orc;                 //!< Character clocked out after an over-read of the transmit buffer.
    uint8_t               irq_priority;        //!< Interrupt priority.
} nrf_drv_spis_config_t;


/** @brief SPI slave event callback function type.
 *
 * @param[in] event                 SPI slave driver event.  
 */
typedef void (*nrf_drv_spis_event_handler_t)(nrf_drv_spis_event_t event);

/** @brief Function for initializing the SPI slave driver instance.
 *
 * @param[in] p_instance    Pointer to the instance structure.
 * @param[in] p_config      Pointer to the structure with the initial configuration.
 *                          If NULL, the default configuration will be used.
 * @param[in] event_handler Function to be called by the SPI slave driver upon event.
 *
 * @retval NRF_SUCCESS             If the initialization was successful.
 * @retval NRF_ERROR_INVALID_PARAM If an invalid parameter is supplied.
 * @retval NRF_ERROR_BUSY          If some other peripheral with the same
 *                                 instance ID is already in use. This is 
 *                                 possible only if PERIPHERAL_RESOURCE_SHARING_ENABLED 
 *                                 is set to a value other than zero.
 */
ret_code_t nrf_drv_spis_init(nrf_drv_spis_t const * const  p_instance,
                             nrf_drv_spis_config_t const * p_config,
                             nrf_drv_spis_event_handler_t  event_handler);

/**
 * @brief Function for uninitializing the SPI slave driver instance.
 *
 * @param[in] p_instance Pointer to the instance structure.
 */
void nrf_drv_spis_uninit(nrf_drv_spis_t const * const p_instance);

/** @brief Function for preparing the SPI slave instance for a single SPI transaction.
 * 
 * This function prepares the SPI slave device to be ready for a single SPI transaction. It configures 
 * the SPI slave device to use the memory supplied with the function call in SPI transactions. 
 * 
 * When either the memory buffer configuration or the SPI transaction has been 
 * completed, the event callback function will be called with the appropriate event 
 * @ref nrf_drv_spis_event_type_t. Note that the callback function can be called before returning from 
 * this function, because it is called from the SPI slave interrupt context.
 *
 * @note This function can be called from the callback function context.
 *
 * @note Client applications must call this function after every @ref NRF_DRV_SPIS_XFER_DONE event if 
 * the SPI slave driver should be prepared for a possible new SPI transaction. 
 *
 * @note Peripherals that are using EasyDMA (for example, SPIS) require the transfer buffers
 * to be placed in the Data RAM region. Otherwise, this function will fail
 * with the error code NRF_ERROR_INVALID_ADDR.
 *
 * @param[in] p_instance            SPIS instance.
 * @param[in] p_tx_buffer           Pointer to the TX buffer.
 * @param[in] p_rx_buffer           Pointer to the RX buffer.
 * @param[in] tx_buffer_length      Length of the TX buffer in bytes.
 * @param[in] rx_buffer_length      Length of the RX buffer in bytes. 
 *
 * @retval NRF_SUCCESS              If the operation was successful.
 * @retval NRF_ERROR_NULL           If the operation failed because a NULL pointer was supplied.   
 * @retval NRF_ERROR_INVALID_STATE  If the operation failed because the SPI slave device is in an incorrect state.
 * @retval NRF_ERROR_INVALID_ADDR   If the provided buffers are not placed in the Data
 *                                  RAM region.
 * @retval NRF_ERROR_INTERNAL       If the operation failed because of an internal error.
 */
ret_code_t nrf_drv_spis_buffers_set(nrf_drv_spis_t const * const  p_instance,
                                    const uint8_t * p_tx_buffer, 
                                    uint8_t   tx_buffer_length, 
                                    uint8_t * p_rx_buffer, 
                                    uint8_t   rx_buffer_length);
//kai.ter add
ret_code_t nrf_drv_spis_tx_buffers_set(nrf_drv_spis_t const * const  p_instance,
                                    const uint8_t * p_tx_buffer,
                                    uint8_t   tx_buffer_length);
//kai.ter add
ret_code_t nrf_drv_spis_rx_buffers_set(nrf_drv_spis_t const * const  p_instance,
                                    uint8_t * p_rx_buffer,
                                    uint8_t   rx_buffer_length);


#endif // SPI_SLAVE_H__

/** @} */
