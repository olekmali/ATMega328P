//* Simple MPU6050 library - header file - 2021 *
#ifndef LIB_MPU6050_H_
#define LIB_MPU6050_H_

typedef struct
{
    float x;
    float y;
    float z;
} mpu_data_t;

typedef enum { mpu_OK, mpu_ERROR } mpu_status_t;

mpu_status_t mpu_init(void);
mpu_status_t mpu_get_accel(mpu_data_t* mpu_data);

#endif