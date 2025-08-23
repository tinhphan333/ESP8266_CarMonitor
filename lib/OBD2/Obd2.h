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
#include "Obd2_const.h" // Thư viện chứa các hằng số OBD-II
#include <mcp_can.h>    // Thư viện MCP_CAN giao tiếp với Canbus thông qua module MCP2515
#include <Firebase.h>   // Thư viện để làm việc với Firebase

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
 * @brief Gửi lần lượt các yêu cầu PID OBD-II
 * @param CAN Đối tượng CAN để giao tiếp với bus CAN
 * @return OBD2_OK nếu thành công, OBD2_ERR nếu có lỗi
 */
OBD2_Status_t Send_task(MCP_CAN &CAN);

/**
 * @brief Nhận và xử lý dữ liệu từ CAN bus và upload lên Firebase
 * @param firebaseData Đối tượng FirebaseData để upload dữ liệu lên Firebase
 * @return OBD2_OK nếu thành công, OBD2_ERR nếu có lỗi
 */
OBD2_Status_t Receive_task(FirebaseData &firebaseData, MCP_CAN &CAN);

#endif // OBD2_H
