//* Simple MPU6050 library - implementation file - 2023 *
#include "lib_mpu6050.h"
#include "bios_twi_master.h"

#define F_CPU 16000000UL
#include <util/delay.h>

#define MPU6050_ADDR        0x68

/* MPU6050 register address */
#define ACCEL_XOUT_H        0x3B
#define ACCEL_XOUT_L        0x3C
#define ACCEL_YOUT_H        0x3D
#define ACCEL_YOUT_L        0x3E
#define ACCEL_ZOUT_H        0x3F
#define ACCEL_ZOUT_L        0x40
#define SIGNAL_PATH_RESET   0x68
#define PWR_MGMT_1          0x6B
#define PWR_MGMT_2          0x6C


void mpu_init(void)
{
    uint8_t data1[2] = {PWR_MGMT_1, 0b10000000};        // reset device
    ret_code_t e1 = tw_master_transmit(MPU6050_ADDR, data1, sizeof(data1), false);
    _delay_ms(100);
    uint8_t data2[2] = {SIGNAL_PATH_RESET, 0b00000111}; // reset signal path
    ret_code_t e2 = tw_master_transmit(MPU6050_ADDR, data2, sizeof(data2), false);
    _delay_ms(100);
    uint8_t data3[2] = {PWR_MGMT_1, 0b00000000};        // wake up the device
    ret_code_t e3 = tw_master_transmit(MPU6050_ADDR, data3, sizeof(data3), false);
    uint8_t data4[2] = {PWR_MGMT_2, 0b00000111};        // disable gyroscopes and save 3.4 mA
    ret_code_t e4 = tw_master_transmit(MPU6050_ADDR, data4, sizeof(data4), false);
    if ( (e1!=SUCCESS) || (e2!=SUCCESS) || (e3!=SUCCESS) || (e4!=SUCCESS) ) {
        /* if desired, insert code for reporting the error */
    }
}

static void mpu_get_accel_raw(mpu_data_t* mpu_data) {
    ret_code_t error_code;
    /* 2 registers for each of accel x, y and z data */
    uint8_t data[6];

    data[0] = ACCEL_XOUT_H;
    error_code = tw_master_transmit(MPU6050_ADDR, data, 1, true);
    if (error_code!=SUCCESS) { /* if desired, insert code for reporting the error */ }
    error_code = tw_master_receive(MPU6050_ADDR, data, sizeof(data));
    if (error_code!=SUCCESS) { /* if desired, insert code for reporting the error */ }

    /* Default accel config +/- 2g */
    mpu_data->x = ( (int16_t)data[0] << 8 | data[1] ) / 16384.0;
    mpu_data->y = ( (int16_t)data[2] << 8 | data[3] ) / 16384.0;
    mpu_data->z = ( (int16_t)data[4] << 8 | data[5] ) / 16384.0;
}

void mpu_get_accel(mpu_data_t* mpu_data) {
    mpu_get_accel_raw(mpu_data);
    mpu_data->x = mpu_data->x * 9.81;
    mpu_data->y = mpu_data->y * 9.81;
    mpu_data->z = mpu_data->z * 9.81;
}
