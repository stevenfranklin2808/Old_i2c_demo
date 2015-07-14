#include "mbed.h"

class I2C_LIB {
    
    private:
        Serial sc;
        I2C i2c;
        
        char slaveAddr;
    
    public:
        I2C_LIB();
        I2C_LIB(char addr);
        
        void I2C_Write ( int addr, char cmd, char value );

        //void I2C_Write_Chain ( int addr, char cmd, char *value, int length );

        void I2C_Read ( int addr, char cmd, char *data, int length );

};
