//*  i2c library use example for MPU 6050 *
#include "bios_uart0.h"
#include "bios_twi_master.h"
#include "lib_mpu6050.h"

#include <stdlib.h>

#define F_CPU 16000000UL
#include <util/delay.h>



int main(void)
{
    uart0_initialize(9600);
    tw_init(TW_FREQ_400K, true); // set I2C Frequency, enable internal pull-up
    mpu_init();
    mpu_data_t accel;

	uart0_puts("Read accelerometer data.\r\n");
    while (1)
    {
        char buffer[15]; // make sure it is big enough!
		mpu_get_accel(&accel);

        dtostrf(accel.x, 8, 2, buffer);
        uart0_puts(buffer);
        dtostrf(accel.y, 8, 2, buffer);
        uart0_puts(buffer);
        dtostrf(accel.z, 8, 2, buffer);
        uart0_puts(buffer);
        uart0_puts("\r\n");
		_delay_ms(200);
    }
}

