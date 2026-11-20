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
  
    mpu_status_t status;
    do {
        status = mpu_init();
		if ( mpu_OK != status ) {
            uart0_puts("Please connect the MPU6050 Accelerometer.\r\n");
            _delay_ms(5000U);
        }
	} while ( mpu_OK != status );
    uart0_puts("Accelerometer initialized.\r\n");

    uart0_puts("Read accelerometer data.\r\n");
    mpu_data_t accel;
    while (1)
    {
        char buffer[15]; // make sure it is big enough!
        status = mpu_get_accel(&accel);
        if ( status == mpu_OK) {
            dtostrf(accel.x, 8, 2, buffer);
            uart0_puts(buffer);
            dtostrf(accel.y, 8, 2, buffer);
            uart0_puts(buffer);
            dtostrf(accel.z, 8, 2, buffer);
            uart0_puts(buffer);
            uart0_puts("\r\n");
            _delay_ms(200);        
		} else {
            uart0_puts("Please connect the MPU6050 Accelerometer and restart.\r\n");
            while( 1 )
                ;
		}
    }
    return(0);
}

