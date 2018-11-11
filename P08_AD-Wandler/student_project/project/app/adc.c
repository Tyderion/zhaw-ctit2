/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module adc.
 * --
 * --
 * -- $Id: adc.c 3222 2016-04-12 12:57:24Z ruan $
 * ------------------------------------------------------------------------- */

/* standard includes */
#include <stdint.h>
#include <reg_stm32f4xx.h>

/* user includes */
#include "adc.h"
#include "reg_ctboard.h"


/* -- Macros
 * ------------------------------------------------------------------------- */

#define PERIPH_GPIOF_ENABLE (0x00000020)
#define PERIPH_ADC3_ENABLE  (0x00000400)

/* Configuring pin for ADC: PF.6 */
#define GPIOF_MODER_ANALOG (0x3 << 12)


/* ADC configuration
 * ------------------------------------------------------------------------- */

#define ADC_CCR_NORMALCLK   (0x0 << 16u)    // APB2 / 2 -> 21 MHz
#define ADC_CCR_NODMA       (0x0 << 14u)    // DMA disabled
#define ADC_CCR_DELAY       (0x0 << 8u)     // Delay 5 cycles
#define ADC_CCR_INDEPENDENT (0x0 << 0u)     // ADC independent mode

/// STUDENTS: To be programmed
#define ADC_CR2_ADON  (0x1 << 0u) // ON/ OFF Bit
#define ADC_CR2_START (0x1 << 30u)

#define ADC_SR_EOC    (0x1 << 1u)
/// END: To be programmed


/* Public function definitions
 * ------------------------------------------------------------------------- */

/*
 *  See header file
 */
void adc_init(void)
{
    /* Enable peripheral clocks */
    RCC->AHBENR[0] |= PERIPH_GPIOF_ENABLE;
    RCC->APBENR[1] |= PERIPH_ADC3_ENABLE;

    /* Configure PF.6 as input */
    GPIOF->MODER |= GPIOF_MODER_ANALOG;


    /* ADC common init */
    ADCCOM->CCR = ADC_CCR_NORMALCLK |
                  ADC_CCR_NODMA |
                  ADC_CCR_DELAY |
                  ADC_CCR_INDEPENDENT;

    /// STUDENTS: To be programmed


    ADC3->SQR[2] |= (4u << 0u);                 /* Channel 4 first in conversion sequence (SQ1). */
    ADC3->SQR[0] |= (0x0 << 20u);               /* Set L to 0 -> only 1 channel (SQ1) in sequence. */

    /// END: To be programmed
}


/*
 *  See header file
 */
uint16_t adc_get_value(adc_resolution_t resolution)
{
    uint16_t adc_value;

    /// STUDENTS: To be programmed
    ADC3->CR1 &= (0x0 << 24u);     // reset resolution
    ADC3->CR1 |= resolution;
    ADC3->CR2 |= 0x1;     // Enable ADC
    ADC3->CR2 |= ADC_CR2_START;     // Start conversion
    while (!(ADC3->SR & ADC_SR_EOC));    // Wait for end of conversion

    adc_value = ADC3->DR;
    ADC3->CR2 &= 0x0;     // Disable ADC
    /// END: To be programmed

    return adc_value;
}


/*
 *  See header file
 */
uint16_t adc_filter_value(uint16_t adc_value)
{
    uint16_t filtered_value = 0;

    /// STUDENTS: To be programmed
    const uint8_t length = 16;
    static uint16_t values[length];
    uint16_t i;
    for (i = length - 1; i > 0; i = i - 1) {
        values[i] = values[i - 1];
    }
    values[0] = adc_value;

    for (i = 0; i < length; i = i + 1) {
        filtered_value += values[i];
    }
    filtered_value /= length;
    /// END: To be programmed

    return filtered_value;
}
