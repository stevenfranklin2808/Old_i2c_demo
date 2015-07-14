#include "mbed.h"
#include "H3LIS331DL.h"
#include "BLEDevice.h"
#include "HeartRateService.h"
#include "BatteryService.h"
#include "GForceService.h"
#include "DeviceInformationService.h"
#include "math.h"
#include "I2C_LIB.h"
//#include "MPU3050.h"


#define VAL_X_AXIS  186
#define VAL_Y_AXIS  204 
#define VAL_Z_AXIS  459

#define UPDATE_PARAMS_FOR_LONGER_CONNECTION_INTERVAL 0

BLEDevice  ble;
DigitalOut led1(LED1);
Ticker ticker;
H3LIS331DL h3lis(p13, p15);
Serial pc(USBTX, USBRX); // tx, rx
//MPU3050 mpu;
I2C_LIB i2c;
double accelAddr = (0x18<<1);
double *aa = &accelAddr;
int ctr = 0;


int16_t ax, ay, az;
int16_t gx, gy, gz;
char buffer[6];
double accel[3] = {0.0, 0.0, 0.0};
uint16_t magg = 0;
uint16_t maga = 0;
#define NUM_VALUES_IN_PACKET_MAIN    8
uint16_t sensorInfo[NUM_VALUES_IN_PACKET_MAIN];
        
int16_t twosCConvert(int16_t c);        
        

const static char     DEVICE_NAME[]        = "G-Force";
static const uint16_t uuid16_list[]        = {GattService::UUID_HEART_RATE_SERVICE,
                                              GattService::UUID_DEVICE_INFORMATION_SERVICE};
static volatile bool  triggerSensorPolling = false;

void disconnectionCallback(Gap::Handle_t handle, Gap::DisconnectionReason_t reason)
{
    ble.startAdvertising(); // restart advertising
}

void periodicCallback(void)
{
    led1 = !led1; /* Do blinky on LED1 while we're waiting for BLE events */

    /* Note that the periodicCallback() executes in interrupt context, so it is safer to do
     * heavy-weight sensor polling from the main thread. */
    triggerSensorPolling = true;
}
    
int main() {
    float temp1,temp2,temp3;
    led1 = 1;
    ticker.attach(periodicCallback, 1); // blink LED every second

    ble.init();
    ble.onDisconnection(disconnectionCallback);

    /* Setup primary service. */
    //uint8_t hrmCounter = 1; // init HRM to 100bps
    //HeartRateService hrService(ble, hrmCounter, HeartRateService::LOCATION_FINGER);
    GForceService gForceService(ble, HeartRateService::LOCATION_FINGER);

    /* Setup auxiliary service. */
    DeviceInformationService deviceInfo(ble, "ARM", "Model1", "SN1", "hw-rev1", "fw-rev1", "soft-rev1");

    /* Setup advertising. */
    ble.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
    ble.accumulateAdvertisingPayload(GapAdvertisingData::GENERIC_HEART_RATE_SENSOR);
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    ble.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.setAdvertisingInterval(2000);
    ble.startAdvertising();


    //mpu.initialize();
    h3lis.init();
    //h3lis.importPara(VAL_X_AXIS,VAL_Y_AXIS,VAL_Z_AXIS);
    
    while(true) {
        //if (triggerSensorPolling && ble.getGapState().connected) {
          //  triggerSensorPolling = false;
            for(uint16_t a = 0; a < NUM_VALUES_IN_PACKET_MAIN;)
            {
                //i2c.I2C_Read((0x18 << 1),0x28,buffer,1);
                i2c.I2C_Read((0x18 << 1),0x29,buffer+1,1);
                //i2c.I2C_Read((0x18 << 1),0x2A,buffer+2,1);
                i2c.I2C_Read((0x18 << 1),0x2B,buffer+3,1);
                //i2c.I2C_Read((0x18 << 1),0x2C,buffer+4,1);
                i2c.I2C_Read((0x18 << 1),0x2D,buffer+5,1);
                ax = twosCConvert((int16_t)buffer[1]); // +- 100g
                ay = twosCConvert((int16_t)buffer[3]);
                az = twosCConvert((int16_t)buffer[5]);
                sensorInfo[a++] =  80 * abs(sqrt((float)(ax*ax+ay*ay+az*az)));
                //sensorInfo[a++] = 0;
                
               /* i2c.I2C_Read((0x68 << 1),0x1D,buffer,1);
                //i2c.I2C_Read((0x68 << 1),0x1E,buffer+1,1);
                i2c.I2C_Read((0x68 << 1),0x1F,buffer+2,1);
                //i2c.I2C_Read((0x68 << 1),0x20,buffer+3,1);
                i2c.I2C_Read((0x68 << 1),0x21,buffer+4,1);
                //i2c.I2C_Read((0x68 << 1),0x22,buffer+5,1);
                gx = buffer[0];
                gy = buffer[2];
                gz = buffer[4];
                sensorInfo[a++] = 7 * abs(sqrt((float)(gx*gx+gy*gy+gz*gz))-360.624451);*/
                //sensorInfo[a] = magg;
                //pc.printf("gx: %i\ngy: %i\ngz: %i\n\n", gx, gy, gz);
                
                
                
                //pc.printf("ax: %d\nay: %d\naz: %d\nmag A: %f\n\n", ax, ay, az, maga);
                wait_ms(20);
            }
            //ctr++;
         
           // pc.printf("L, H: %i, %i\n\n", buffer[0], buffer[1]);
            //gx = gy = gz = 0;
            //gForceService.updateData(ax,ay,az,gx,gy,gz,0);
            //gForceService.updateData(sensorInfo[0],sensorInfo[1]);
            gForceService.updateData(sensorInfo,NUM_VALUES_IN_PACKET);
            wait_ms(5);
        //}
        //else {
            //ble.waitForEvent();    
        //}    
    }
}

int16_t twosCConvert(int16_t c){
    if (c >= 128)
        c -= 256;
    return c;
}
