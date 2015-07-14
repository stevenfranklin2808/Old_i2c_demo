#include "I2C_LIB.h"
#include "H3LIS331DL.h"

#define MPU3050_DEFAULT_ADDRESS 0x69

#define MPU3050_WHO_AM_I         0x00
#define MPU3050_PRODUCT_ID       0x01
#define MPU3050_XG_OFFS_USRH     0x0C 
#define MPU3050_XG_OFFS_USRL     0x0D 
#define MPU3050_YG_OFFS_USRH     0x0E
#define MPU3050_YG_OFFS_USRL     0x0F
#define MPU3050_ZG_OFFS_URSH     0x10
#define MPU3050_ZG_OFFS_USR      0x11
#define MPU3050_FIFO_EN          0x12
#define MPU3050_AUXVDDIO         0x13
#define MPU3050_AUX_SLV_ADDR     0x14
#define MPU3050_SMPLRT_DIV       0x15
#define MPU3050_DLPF_FS_SYNC     0x16
#define MPU3050_INT_CFG          0x17
#define MPU3050_AUX_ADDR         0x18
#define MPU3050_INT_STATUS       0x1A
#define MPU3050_TEMP_OUT_H       0x1B
#define MPU3050_TEMP_OUT_L       0x1C
#define MPU3050_GYRO_XOUT_H      0x1D
#define MPU3050_GYRO_XOUT_L      0x1E
#define MPU3050_GYRO_YOUT_H      0x1F
#define MPU3050_GYRO_YOUT_L      0x20
#define MPU3050_GYRO_ZOUT_H      0x21
#define MPU3050_GYRO_ZOUT_L      0x22
#define MPU3050_AUX_XOUT_H       0x23
#define MPU3050_AUX_XOUT_L       0x24
#define MPU3050_AUX_YOUT_H       0x25
#define MPU3050_AUX_YOUT_L       0x26
#define MPU3050_AUX_ZOUT_H       0x27
#define MPU3050_AUX_ZOUT_L       0x28
#define MPU3050_FIFO_COUNTH      0x3A
#define MPU3050_FIFO_COUNTL      0x3B
#define MPU3050_FIFO_R           0x3C
#define MPU3050_USER_CTRL        0x3D
#define MPU3050_PWR_MGM          0x3E

class MPU3050 {
    private:
        I2C_LIB i2c;
        uint8_t mpuAddr;
        uint8_t accelAddr;
        char buffer[14];
    public:
        MPU3050();
        MPU3050(uint8_t addr);
        
        void initialize();
        bool testConnection();
        
        void getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz);

};
