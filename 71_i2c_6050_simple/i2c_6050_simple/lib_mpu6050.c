//* Simple MPU6050 library - implementaiton file - 2021 *
#include "lib_mpu6050.h"
#include "bios_twi_master.h"

#define MPU6050_ADDR	0x68

/* MPU6050 register address */
#define ACCEL_XOUT_H	0x3B
#define ACCEL_XOUT_L	0x3C
#define ACCEL_YOUT_H	0x3D
#define ACCEL_YOUT_L	0x3E
#define ACCEL_ZOUT_H	0x3F
#define ACCEL_ZOUT_L	0x40
#define PWR_MGMT_1		0x6B


void mpu_init(void)
{
    // Write 0 to PWR_MGMT_1 reg to wakeup MPU
    uint8_t data[2] = {PWR_MGMT_1, 0};
    ret_code_t error_code = tw_master_transmit(MPU6050_ADDR, data, sizeof(data), false);
    if (error_code!=SUCCESS) { /* if desired, insert code for reporting the error */ }
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
