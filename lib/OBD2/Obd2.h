/*
 * @file Obd2.h
 * @brief Khai báo các hàm để yêu cầu và giải mã dữ liệu OBD-II
 * @details File này cung cấp giao diện các hàm để yêu cầu dữ liệu và giải mã dữ liệu OBD-II.
 * @author phantinhphan333
 * @date 2024-11-01
 */

#ifndef OBD2_H
#define OBD2_H

#include <Arduino.h>
#include "Obd2_const.h"          // Thư viện chứa các hằng số OBD-II
#include <mcp_can.h>             // Thư viện MCP_CAN giao tiếp với Canbus thông qua module MCP2515
#include <Firebase_ESP_Client.h> // Thư viện để làm việc với Firebase

const int SPI_CS_PIN = 15; // Chân GPIO 15 của ESP8266 nối chân Chip select của MCP2515
MCP_CAN CAN(SPI_CS_PIN);   // Khởi tạo đối tượng CAN để giao tiếp với MCP2515

/**
 * @brief Trạng thái hàm trả về khi yêu cầu và giải mã dữ liệu OBD-II
 * @details Trạng thái này được sử dụng để xác định kết quả của các hàm gửi PID và giải mã dữ liệu OBD-II.
 */
typedef enum
{
    OBD2_OK = 0,
    OBD2_ERR = 1,
} OBD2_Status_t;

/**
 * @brief Thiết lập bộ lọc cho CAN message
 * @details Hàm này được sử dụng để thiết lập bộ lọc cho các thông điệp CAN nhận được từ MCP2515.
 */
void set_mask_filt();

/**
 * @brief Gửi lần lượt các yêu cầu PID OBD-II
 * @return Trạng thái của yêu cầu
 */
OBD2_Status_t Send_task(void);

#endif // OBD2_H
