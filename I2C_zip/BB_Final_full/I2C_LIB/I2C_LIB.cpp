#include "I2C_LIB.h"

#define OUTPUT_READ true

I2C_LIB::I2C_LIB() : sc(USBTX, USBRX), i2c(p13, p15) {
    slaveAddr = (0x68 << 1);
}

/** Specific address constructor.
 * @param address I2C address
 * @see MPU6050_DEFAULT_ADDRESS
 * @see MPU6050_ADDRESS_AD0_LOW
 * @see MPU6050_ADDRESS_AD0_HIGH
 */
I2C_LIB::I2C_LIB(char addr) : sc(USBTX, USBRX), i2c(p13, p15) {
    slaveAddr = addr;
}

void I2C_LIB::I2C_Write ( int addr, char cmd, char value ) {
        i2c.start();
        i2c.write(addr);
        i2c.write(cmd);
        i2c.write(value);
        i2c.stop();
}

void I2C_LIB::I2C_Read ( int addr, char cmd, char *data, int length ) {
      int result;
      result = i2c.write(addr, &cmd, length, true); 
        //wait(0.1);
      
      /*#ifdef OUTPUT_READ
      sc.printf("I2C Write Result: %i\n", result);
      wait(0.1);
      #endif*/

      result = i2c.read(addr, data, length);
      
      /*#ifdef OUTPUT_READ
      sc.printf("I2C Read Result: %i\n", result);
      wait(0.1);
      sc.printf("Test: %i\n\r", data[0]);
      wait(0.1);
      #endif*/
}
