/*
 * @file Obd2.cpp
 * @brief Định nghĩa các hàm để yêu cầu và giải mã dữ liệu OBD-II
 * @details File này chứa các hàm để yêu cầu dữ liệu và giải mã dữ liệu OBD-II.
 * @author phantinhphan333
 * @date 2024-11-01
 */

#include "Obd2.h"

void set_mask_filt()
{
    CAN.init_Mask(0, 0, 0x7FC);
    CAN.init_Mask(1, 0, 0x7FC);
    CAN.init_Filt(0, 0, 0x7E8);
    CAN.init_Filt(1, 0, 0x7E8);
    CAN.init_Filt(2, 0, 0x7E8);
    CAN.init_Filt(3, 0, 0x7E8);
    CAN.init_Filt(4, 0, 0x7E8);
    CAN.init_Filt(5, 0, 0x7E8);
}

static OBD2_Status_t sendPid(unsigned char __pid)
{
    unsigned char tmp[8] = {0x02, 0x01, __pid, 0, 0, 0, 0, 0}; // Tạo khung OBD-II để yêu cầu dữ liệu

    if (CAN.sendMsgBuf(CAN_ID_PID, 0, 8, tmp) == CAN_OK) // gửi khung OBD-II lên CAN bus
    {
        // In dữ liệu chuẩn bị gửi lên màn hình Serial
        Serial.print("SEND PID: 0x");
        Serial.println(__pid, HEX);
        return OBD2_OK;
    }
    else
    {
        Serial.println("SEND PID ERROR");
        return OBD2_ERR;
    }
}

OBD2_Status_t Send_task(void)
{
    static int count = 0; // Khai báo biến đếm lần lượt gửi các mã PID
    count++;
    // Kiểm tra xem đã gửi hết các PID chưa
    if (count > MAX_PID_COUNT)
    {
        count = 0; // Nếu đã gửi hết thì reset lại biến đếm
    }

    switch (count)
    {
    case 1:
        return sendPid(MODE_1_VEHICLE_SPEED); // Gửi mã PID yêu cầu giá trị tốc độ xe
    case 2:
        return sendPid(MODE_1_ENGINE_RPM); // Gửi mã PID yêu cầu giá trị tốc độ động cơ
    case 3:
        return sendPid(MODE_1_ENGINE_COOL_TEMP); // Gửi mã PID yêu cầu giá trị nhiệt độ nước làm mát
    case 4:
        return sendPid(MODE_1_THROTTLE_POSITION); // Gửi mã PID yêu cầu giá trị độ mở bướm ga
    case 5:
        return sendPid(MODE_1_FUEL_LEVEL); // Gửi mã PID yêu cầu giá trị mức nhiên liệu
    case 6:
        return sendPid(MODE_1_ENGINE_LOAD); // Gửi mã PID yêu cầu giá trị tải động cơ
    }
}

static void DecodeData(unsigned char *buf)
{
    switch (buf[2]) // buf[2]: chứa mã PID
    {
    case MODE_1_VEHICLE_SPEED:
    {
        int vehicle_speed = buf[3];
        if (Firebase.setInt(firebaseData, "/DATN/Xe 1/vehicle_speed", vehicle_speed))
            ;
        {
            Serial.println("vehicle_speed: " + String(vehicle_speed));
        }
    }
    break;
    case MODE_1_ENGINE_RPM:
    {
        int data_1 = buf[3];
        int data_2 = buf[4];
        int engine_speed = (data_1 * 256 + data_2) / 4;
        if (Firebase.setInt(firebaseData, "/DATN/Xe 1/engine_speed", engine_speed))
            ;
        //{  Serial.println("engine_speed: " + String(engine_speed));  }
    }
    break;
    case MODE_1_ENGINE_COOL_TEMP:
    {
        int engine_coolant_temperature = buf[3] - 40;
        if (Firebase.setInt(firebaseData, "/DATN/Xe 1/engine_coolant_temperature", engine_coolant_temperature))
            ;
        //{ Serial.println("engine_coolant_temperature: " + String(engine_coolant_temperature)); }
    }
    break;
    case MODE_1_THROTTLE_POSITION:
    {
        int throttle_position = buf[3] * 0.392156;
        if (Firebase.setInt(firebaseData, "/DATN/Xe 1/throttle_position", throttle_position))
            ;
        //{ Serial.println("throttle_position: " + String(throttle_position));  }
    }
    break;
    case MODE_1_FUEL_LEVEL:
    {
        int fuel_level = buf[3] * 0.392156;
        if (Firebase.setInt(firebaseData, "/DATN/Xe 1/fuel_level", fuel_level))
            ;
        //{ Serial.println("fuel_level: " + String(fuel_level)); }
    }
    break;
    case MODE_1_ENGINE_LOAD:
    {
        int engine_load = buf[3] * 0.392156;
        if (Firebase.setInt(firebaseData, "/DATN/Xe 1/engine_load", engine_load))
            ;
        //{ Serial.println("engine_load: " + String(engine_load)); }
    }
    break;
    }
}

void taskCanRecv()
{
    unsigned char len = 0;
    unsigned char buf[8];

    if (CAN_MSGAVAIL == CAN.checkReceive())
    {                              // check if get data
        CAN.readMsgBuf(&len, buf); // read data,  len: data length, buf: data buf

        Serial.println("\r\n------------------------------------------------------------------");
        Serial.print("Get Data From id: 0x");
        Serial.println(CAN.getCanId(), HEX);
        for (int i = 0; i < len; i++)
        { // print the data
            Serial.print("0x");
            Serial.print(buf[i], HEX);
            Serial.print("\t");
        }
        Serial.println();
    }
    DecodeData(buf);
}
