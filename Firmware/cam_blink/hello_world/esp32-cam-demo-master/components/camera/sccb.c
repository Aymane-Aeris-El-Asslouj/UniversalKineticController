/*
 * This file is part of the OpenMV project.
 * Copyright (c) 2013/2014 Ibrahim Abdelkader <i.abdalkader@gmail.com>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * SCCB (I2C like) driver.
 *
 */
#include <stdbool.h>
#include "wiring.h"
#include "sccb.h"
#include "twi.h"
#include <stdio.h>

#define SCCB_FREQ   (100000) // We don't need fast I2C. 100KHz is fine here.
#define TIMEOUT     (1000) /* Can't be sure when I2C routines return. Interrupts
while polling hardware may result in unknown delays. */


uint8_t SCCB_Read(uint8_t slv_addr, uint8_t reg)
{
    uint8_t data=0;

    __disable_irq();
    int rc = twi_writeTo(slv_addr, &reg, 1, true);
    if (rc != 0) {
        data = 0xff;
    }
    else {
        rc = twi_readFrom(slv_addr, &data, 1, true);
        if (rc != 0) {
            data=0xFF;
        }
    }
    __enable_irq();
    if (rc != 0) {
        printf("SCCB_Read [%02x] failed rc=%d\n", reg, rc);
    }
    return data;
}

uint8_t SCCB_Write(uint8_t slv_addr, uint8_t reg, uint8_t data)
{
    uint8_t ret=0;
    uint8_t buf[] = {reg, data};

    __disable_irq();
    if(twi_writeTo(slv_addr, buf, 2, true) != 0) {
        ret=0xFF;
    }
    __enable_irq();
    if (ret != 0) {
        printf("SCCB_Write [%02x]=%02x failed\n", reg, data);
    }
    return ret;
}
