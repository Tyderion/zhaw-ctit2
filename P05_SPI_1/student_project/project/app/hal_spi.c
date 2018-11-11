/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur   -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences) -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Module      : SPI Library
 * -- Description :
 * --
 * -- $Id: hal_spi.c 4030 2017-04-18 11:50:56Z kjaz $
 * ------------------------------------------------------------------
 */
#include <reg_stm32f4xx.h>
#include "hal_spi.h"

#define BIT_TXE  (uint32_t)0x00000002
#define BIT_RXNE (uint32_t)0x00000001
#define BIT_BSY  (uint32_t)0x00000080

static void set_ss_pin_low(void);
static void set_ss_pin_high(void);
static void wait_10_us(void);

/*
 * according to description in header file
 */
void hal_spi_init(void)
{
    RCC->APBENR[1] |= 0x00001000;     // enable SPI clock
    RCC->AHBENR[0] |= 0x00000001;     // start clock on GPIO A
    GPIOA->OSPEEDR &= 0xFFFF00FF;     // clear P4 to P7
    GPIOA->OSPEEDR |= 0x0000FF00;     // set P4 to P7 to 100 MHz
    GPIOA->MODER &= 0xFFFF00FF;       // clear mode on P5 to P7
    // P5 to P7, P4 output mode
    GPIOA->MODER |= 0x0000A900;       // Set alternate function mode on
    // P5 to P7, P4 output mode
    GPIOA->AFR[0] &= 0x0000FFFF;      // clear alternate function
    GPIOA->AFR[0] |= 0x55550000;      // Set SPI1 alternate function

    SPI1->CR2 = 0x0000;               // set spi to default state
    SPI1->CR1 = 0x0000;               // set spi to default state

    /// STUDENTS: To be programmed

    // CR1: BIDIMODE BIDIOE CRCEN CRCNEXT DFF  RXONLY SSM SSI LSBFIRST SPE BR[2:0] MSTR CPOL CPHA
    //        0         0     0       0    0     0     1   1     0      1   11  1    ?    0    0
    //          [               0           ] [         3         ] [      7      ][        8 / C    ]
    SPI1->CR1 = 0x037C;


    /// END: To be programmed
    set_ss_pin_high();
}

/*
 * according to description in header file
 */
uint8_t hal_spi_read_write(uint8_t send_byte)
{
    uint8_t received;
    /// STUDENTS: To be programmed
    if (SPI1->SR & BIT_TXE) {
        set_ss_pin_low();
        SPI1->DR = send_byte;
        wait_10_us();
        set_ss_pin_high();
				wait_10_us();
				set_ss_pin_low();
    }

    if (SPI1->SR & BIT_RXNE) {
        received = SPI1->DR;
    }

    return received;
    /// END: To be programmed
}

/**
 * Set Slave-Select Pin (Port A, Pin 5) low
 *
 * No parameters
 *
 * No returns
 */
static void set_ss_pin_low(void)
{
    GPIOA->BSRR[1] |= 0x0010;          // Set Port A, Pin 5 low
}

/**
 * Set Slave-Select Pin (Port A, Pin 5) high
 *
 * No parameters
 *
 * No returns
 */
static void set_ss_pin_high(void)
{
    GPIOA->BSRR[0] |= 0x0010;          // Set Port A, Pin 5 high
}

/**
 * Wait for approximately 10us
 *
 * No parameters
 *
 * No returns
 */
static void wait_10_us(void)
{
    uint8_t counter = 0;
    while (counter < 160) {
        counter++;
    }
}
