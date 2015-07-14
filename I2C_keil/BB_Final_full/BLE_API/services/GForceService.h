/* 
    Name: GForceService.h
    Author: Beau Roland
    
 */

#ifndef __BLE_GFORCE_SERVICE_H__
#define __BLE_GFORCE_SERVICE_H__

#include "BLEDevice.h"

/**
* @class GForceService
* @brief BLE Service for GForce mouthguard. This BLE Service contains the acceleration and groscope data for the x,y,and z axis. <br>
*/
class GForceService {

public:
    /**
     * @brief Constructor
     *
     * @param[ref] _ble
     *               Reference to the underlying BLEDevice.
     */
    GForceService(BLEDevice &_ble, uint8_t loc) :
        ble(_ble),
        valueBytes(0,0),
        gforceData(GattCharacteristic::UUID_GFORCE_DATA_POINT_CHAR, valueBytes.getPointer(),
                valueBytes.getNumValueBytes(), GForceValueBytes::MAX_VALUE_BYTES,
                GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY),
        location(GattCharacteristic::UUID_BODY_SENSOR_LOCATION_CHAR, &loc),
        controlPoint(GattCharacteristic::UUID_GFORCE_CONTROL_POINT_CHAR, &controlPointValue) {
        setupService();
    }

    void updateData(uint16_t* buffer, uint16_t bufLen) {
        valueBytes.updateGForceData(buffer, bufLen);
        ble.updateCharacteristicValue(gforceData.getValueAttribute().getHandle(), valueBytes.getPointer(), valueBytes.getNumValueBytes());
    }
    
    void updateData(uint16_t aMag, uint16_t gMag) {
        valueBytes.updateGForceData(aMag, gMag);
        ble.updateCharacteristicValue(gforceData.getValueAttribute().getHandle(), valueBytes.getPointer(), valueBytes.getNumValueBytes());
    }
    



    /**
     * @brief Set a new 8-bit value for heart rate.
     *
     * @param[in] hrmCounter
     *                  HeartRate in bpm.
     */
     /*
    void updateData(uint16_t xAccel,uint16_t yAccel,uint16_t zAccel,uint16_t xGyro,uint16_t yGyro,uint16_t zGyro, uint8_t batteryLife) {
        valueBytes.updateGForceData(xAccel, yAccel, zAccel, xGyro, yGyro, zGyro,  batteryLife);
        ble.updateCharacteristicValue(gforceData.getValueAttribute().getHandle(), valueBytes.getPointer(), valueBytes.getNumValueBytes());
    }
    */

    /**
     * This callback allows the GForceService to receive updates to the
     * controlPoint Characteristic.
     *
     * @param[in] params
     *     Information about the characterisitc being updated.
     */
    virtual void onDataWritten(const GattCharacteristicWriteCBParams *params) {
        if (params->charHandle == controlPoint.getValueAttribute().getHandle()) {
            /* Do something here if the new value is 1; else you can override this method by
             * extending this class.
             * @NOTE: if you are extending this class, be sure to also call
             * ble.onDataWritten(this, &ExtendedHRService::onDataWritten); in
             * your constructor.
             */
        }
    }

private:
    void setupService(void) {
        static bool serviceAdded = false; /* We should only ever need to add the heart rate service once. */
        if (serviceAdded) {
            return;
        }

        GattCharacteristic *charTable[] = {&gforceData, &location ,&controlPoint};
        GattService         gforceService(GattService::UUID_GRAYMATTER_SERVICE, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));

        ble.addService(gforceService);
        serviceAdded = true;

        ble.onDataWritten(this, &GForceService::onDataWritten);
    }
    
#define NUM_VALUES_IN_FULL_PACKET_GF    9

#define X_ACCEL_OFFSET_IDX  0
#define Y_ACCEL_OFFSET_IDX  1
#define Z_ACCEL_OFFSET_IDX  2
#define X_GYRO_OFFSET_IDX   3
#define Y_GYRO_OFFSET_IDX   4
#define Z_GYRO_OFFSET_IDX   5
#define BATTERY_OFFSET_IDX  6

#define NUM_VALUES_IN_MAG_PACKET_GF        2
#define AMAG_OFFSET   0
#define GMAG_OFFSET   1

private:
    /* Private internal representation for the bytes used to work with the vaulue of the heart-rate characteristic. */
    struct GForceValueBytes {
        static const unsigned MAX_VALUE_BYTES  = 1 + 2*NUM_VALUES_IN_FULL_PACKET_GF ; /* FLAGS + up to two bytes for heart-rate */
        //static const unsigned MAG_VALUE_BYTES  = 1 + 2*NUM_VALUES_IN_MAG_PACKET_GF;
        static const unsigned FLAGS_BYTE_INDEX = 0;

        static const unsigned VALUE_FORMAT_BITNUM = 0;
        static const uint8_t  VALUE_FORMAT_FLAG   = (1 << VALUE_FORMAT_BITNUM);

        GForceValueBytes(uint16_t aMag, uint16_t gMag) {
            updateGForceData(aMag, gMag);
        }
        
        GForceValueBytes(uint8_t value){
            updateGForceData(value);    
        }
            
        void updateGForceData(uint16_t* buffer, uint16_t bufLen)
        {
            valueBytes[FLAGS_BYTE_INDEX]    |= VALUE_FORMAT_FLAG;
            for(uint16_t i=0; i<bufLen; i++){
                valueBytes[FLAGS_BYTE_INDEX + 1 + i*2] = (uint8_t)(buffer[i] & 0xFF);
                valueBytes[FLAGS_BYTE_INDEX + 2 + i*2] = (uint8_t)(buffer[i] >> 8);
            }
        }
        void updateGForceData(uint8_t value){
            valueBytes[FLAGS_BYTE_INDEX]    |= VALUE_FORMAT_FLAG;
            for(uint16_t i=1; i<MAX_VALUE_BYTES; i++){
                valueBytes[FLAGS_BYTE_INDEX + i] = (uint8_t)(value);
            }
        }
        void updateGForceData(uint16_t aMag, uint16_t gMag) {
            valueBytes[FLAGS_BYTE_INDEX]    |= VALUE_FORMAT_FLAG;
            valueBytes[FLAGS_BYTE_INDEX + 1 + AMAG_OFFSET] = (uint8_t)(aMag & 0xFF);
            valueBytes[FLAGS_BYTE_INDEX + 2 + AMAG_OFFSET] = (uint8_t)(aMag >> 8);
            valueBytes[FLAGS_BYTE_INDEX + 1 + GMAG_OFFSET*2] = (uint8_t)(gMag & 0xFF);
            valueBytes[FLAGS_BYTE_INDEX + 2 + GMAG_OFFSET*2] = (uint8_t)(gMag >> 8);
        }
        /*
        GForceValueBytes(uint16_t xAccel,uint16_t yAccel,uint16_t zAccel,uint16_t xGyro,uint16_t yGyro,uint16_t zGyro, uint8_t batteryLife) : valueBytes() {
            updateGForceData(xAccel,yAccel, zAccel, xGyro, yGyro, zGyro, batteryLife);
        }
        */
        /*
        void updateGForceData(uint16_t xAccel,uint16_t yAccel,uint16_t zAccel,uint16_t xGyro,uint16_t yGyro,uint16_t zGyro, uint8_t batteryLife) {
            valueBytes[FLAGS_BYTE_INDEX]    |= VALUE_FORMAT_FLAG;
            valueBytes[FLAGS_BYTE_INDEX + 1 + X_ACCEL_OFFSET_IDX] = (uint8_t)(xAccel & 0xFF);
            valueBytes[FLAGS_BYTE_INDEX + 2 + X_ACCEL_OFFSET_IDX] = (uint8_t)(xAccel >> 8);
            valueBytes[FLAGS_BYTE_INDEX + 1 + Y_ACCEL_OFFSET_IDX*2] = (uint8_t)(yAccel & 0xFF);
            valueBytes[FLAGS_BYTE_INDEX + 2 + Y_ACCEL_OFFSET_IDX*2] = (uint8_t)(yAccel >> 8);
            valueBytes[FLAGS_BYTE_INDEX + 1 + Z_ACCEL_OFFSET_IDX*2] = (uint8_t)(zAccel & 0xFF);
            valueBytes[FLAGS_BYTE_INDEX + 2 + Z_ACCEL_OFFSET_IDX*2] = (uint8_t)(zAccel >> 8);
            valueBytes[FLAGS_BYTE_INDEX + 1 + X_GYRO_OFFSET_IDX*2]  = (uint8_t)(xGyro & 0xFF);
            valueBytes[FLAGS_BYTE_INDEX + 2 + X_GYRO_OFFSET_IDX*2]  = (uint8_t)(xGyro >> 8);
            valueBytes[FLAGS_BYTE_INDEX + 1 + Y_GYRO_OFFSET_IDX*2]  = (uint8_t)(yGyro & 0xFF);
            valueBytes[FLAGS_BYTE_INDEX + 2 + Y_GYRO_OFFSET_IDX*2]  = (uint8_t)(yGyro >> 8);
            valueBytes[FLAGS_BYTE_INDEX + 1 + Z_GYRO_OFFSET_IDX*2]  = (uint8_t)(zGyro & 0xFF);
            valueBytes[FLAGS_BYTE_INDEX + 2 + Z_GYRO_OFFSET_IDX*2]  = (uint8_t)(zGyro >> 8);
            valueBytes[FLAGS_BYTE_INDEX + 1 + BATTERY_OFFSET_IDX*2] = (uint8_t)(batteryLife & 0xFF);
        }
        */

        uint8_t       *getPointer(void) {
            return valueBytes;
        }

        const uint8_t *getPointer(void) const {
            return valueBytes;
        }

        unsigned       getNumValueBytes(void) const {
            return 1 + ((valueBytes[FLAGS_BYTE_INDEX] & VALUE_FORMAT_FLAG) ? NUM_VALUES_IN_FULL_PACKET_GF*sizeof(uint16_t) : NUM_VALUES_IN_FULL_PACKET_GF*sizeof(uint8_t));
        }

private:
        uint8_t valueBytes[MAX_VALUE_BYTES];
    };

private:
    BLEDevice           &ble;

    GForceValueBytes  valueBytes;
    uint8_t           controlPointValue;

    GattCharacteristic                   gforceData;
    ReadOnlyGattCharacteristic<uint8_t>  location;
    WriteOnlyGattCharacteristic<uint8_t> controlPoint;
};

#endif /* #ifndef __BLE_GFORCE_SERVICE_H__*/
