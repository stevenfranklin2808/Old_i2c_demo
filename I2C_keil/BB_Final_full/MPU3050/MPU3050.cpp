#include "MPU3050.h"

MPU3050::MPU3050() {
    mpuAddr = MPU3050_DEFAULT_ADDRESS;
    accelAddr = H3LIS331DL_MEMS_I2C_ADDRESS;
}


MPU3050::MPU3050(uint8_t address) {
    mpuAddr = address;
    accelAddr = H3LIS331DL_MEMS_I2C_ADDRESS;
}

void MPU3050::initialize ( ) {
    
    //i2c.I2C_Write(mpuAddr, 0x14, (0x18 << 1));
    //i2c.I2C_Write(mpuAddr, 0x18, (0x18 << 1));  
    
    char value[5];
    value[0] = 0x27;
    i2c.I2C_Write(accelAddr, H3LIS331DL_CTRL_REG1, value[0]);
    i2c.I2C_Write(mpuAddr, 0x3D, 0x28);

    
    #ifdef OUTPUT_SERIAL
    i2c.I2C_Read(paddr, 0x1C, data, 1);
    sc.printf("Written Result: %i\nTemperature: %i\n\n\n", data[0], data[1]);
    #endif

    #ifdef OUTPUT_SERIAL 
    data[0] = data[1] = 0;
    i2c.I2C_Read(paddr, H3LIS331DL_WHO_AM_I, data, 1);
    sc.printf("Peripheral Accelerometer Who Am I Test\nI Am: %x\n", data[0]);
    #endif
}


bool MPU3050::testConnection() {
    //uint8_t deviceId = getDeviceID();
    mpuAddr = 0x34;
    return (mpuAddr == 0x34);
}

void MPU3050::getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz) {
    //i2c.I2C_Write_Chain(mpuAddr, MPU3050_GYRO_XOUT_H, buffer, 12);
    *ax = (((int16_t)buffer[0]) << 8) | buffer[1];
    *ay = (((int16_t)buffer[2]) << 8) | buffer[3];
    *az = (((int16_t)buffer[4]) << 8) | buffer[5];
    *gx = (((int16_t)buffer[8]) << 8) | buffer[9];
    *gy = (((int16_t)buffer[10]) << 8) | buffer[11];
    *gz = (((int16_t)buffer[12]) << 8) | buffer[13];
}

/*
uint8_t MPU3050::getAuxVDDIOLevel()
{
    i2Cdev.readBit(devAddr, MPU3050_RA_YG_OFFS_TC, MPU3050_TC_PWR_MODE_BIT, buffer);
    return buffer[0];
}

void MPU3050::setAuxVDDIOLevel(uint8_t level)
{
    i2Cdev.writeBit(devAddr, MPU3050_RA_YG_OFFS_TC, MPU3050_TC_PWR_MODE_BIT, level);
}

uint8_t MPU3050::getRate()
{
    i2Cdev.readByte(devAddr, MPU3050_RA_SMPLRT_DIV, buffer);
    return buffer[0];
}

 
void MPU3050::setRate(uint8_t rate)
{
    i2Cdev.writeByte(devAddr, MPU3050_RA_SMPLRT_DIV, rate);
}

uint8_t MPU3050::getExternalFrameSync()
{
    i2Cdev.readBits(devAddr, MPU3050_RA_CONFIG, MPU3050_CFG_EXT_SYNC_SET_BIT, MPU3050_CFG_EXT_SYNC_SET_LENGTH, buffer);
    return buffer[0];
}


void MPU3050::setExternalFrameSync(uint8_t sync)
{
    i2Cdev.writeBits(devAddr, MPU3050_RA_CONFIG, MPU3050_CFG_EXT_SYNC_SET_BIT, MPU3050_CFG_EXT_SYNC_SET_LENGTH, sync);
}
uint8_t MPU3050::getFullScaleGyroRange()
{
    i2Cdev.readBits(devAddr, MPU3050_RA_GYRO_CONFIG, MPU3050_GCONFIG_FS_SEL_BIT, MPU3050_GCONFIG_FS_SEL_LENGTH, buffer);
    return buffer[0];
}

void MPU3050::setFullScaleGyroRange(uint8_t range)
{
    i2Cdev.writeBits(devAddr, MPU3050_RA_GYRO_CONFIG, MPU3050_GCONFIG_FS_SEL_BIT, MPU3050_GCONFIG_FS_SEL_LENGTH, range);
}


bool MPU3050::getAccelXSelfTest()
{
    i2Cdev.readBit(devAddr, MPU3050_RA_ACCEL_CONFIG, MPU3050_ACONFIG_XA_ST_BIT, buffer);
    return buffer[0];
}

void MPU3050::setAccelXSelfTest(bool enabled)
{
    i2Cdev.writeBit(devAddr, MPU3050_RA_ACCEL_CONFIG, MPU3050_ACONFIG_XA_ST_BIT, enabled);
}
bool MPU3050::getAccelYSelfTest()
{
    i2Cdev.readBit(devAddr, MPU3050_RA_ACCEL_CONFIG, MPU3050_ACONFIG_YA_ST_BIT, buffer);
    return buffer[0];
}

void MPU3050::setAccelYSelfTest(bool enabled)
{
    i2Cdev.writeBit(devAddr, MPU3050_RA_ACCEL_CONFIG, MPU3050_ACONFIG_YA_ST_BIT, enabled);
}

bool MPU3050::getAccelZSelfTest()
{
    i2Cdev.readBit(devAddr, MPU3050_RA_ACCEL_CONFIG, MPU3050_ACONFIG_ZA_ST_BIT, buffer);
    return buffer[0];
}

void MPU3050::setAccelZSelfTest(bool enabled)
{
    i2Cdev.writeBit(devAddr, MPU3050_RA_ACCEL_CONFIG, MPU3050_ACONFIG_ZA_ST_BIT, enabled);
}

uint8_t MPU3050::getFullScaleAccelRange()
{
    i2Cdev.readBits(devAddr, MPU3050_RA_ACCEL_CONFIG, MPU3050_ACONFIG_AFS_SEL_BIT, MPU3050_ACONFIG_AFS_SEL_LENGTH, buffer);
    return buffer[0];
}

void MPU3050::setFullScaleAccelRange(uint8_t range)
{
    i2Cdev.writeBits(devAddr, MPU3050_RA_ACCEL_CONFIG, MPU3050_ACONFIG_AFS_SEL_BIT, MPU3050_ACONFIG_AFS_SEL_LENGTH, range);
}
*/
