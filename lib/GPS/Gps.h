/*
 * @file Gps.h
 * @brief Khai báo các hàm để nhận và xử lý dữ liệu từ module GPS Neo-6M
 * @details File này cung cấp giao diện các hàm để nhận và xử lý dữ liệu từ module GPS Neo-6M.
 * @author phantinhphan333
 * @date 2024-11-02
 */
#ifndef GPS_H
#define GPS_H

#include <TinyGPSPlus.h>    // Khai báo các hàm tương tác với module Neo-6M
#include <SoftwareSerial.h> // Khai báo các hàm để giao tiếp với module GPS qua cổng Serial

/**
 * @brief Xử lý chuỗi vị trí
 * @details Hàm này được sử dụng để xử lý chuỗi vị trí từ dữ liệu GPS
 * @param gps Đối tượng TinyGPSPlus chứa dữ liệu GPS
 * @param LocationString Chuỗi vị trí cần xử lý
 * @return Chuỗi vị trí đã được xử lý
 */
String LocationProcess(TinyGPSPlus &gps, String &LocationString);

/**
 * @brief Xử lý chuỗi vị trí
 * @details Hàm này được sử dụng để xử lý chuỗi vị trí từ dữ liệu GPS
 * @param gps Đối tượng TinyGPSPlus chứa dữ liệu GPS
 * @param LocationString Chuỗi vị trí cần xử lý
 * @return Chuỗi vị trí đã được xử lý
 */
String DateProcess(TinyGPSPlus &gps, String &DateString);

/**
 * @brief Xử lý chuỗi thời gian
 * @details Hàm này được sử dụng để xử lý chuỗi thời gian từ dữ liệu GPS
 * @param gps Đối tượng TinyGPSPlus chứa dữ liệu GPS
 * @param TimeString Chuỗi thời gian cần xử lý
 * @return Chuỗi thời gian đã được xử lý
 */
String TimeProcess(TinyGPSPlus &gps, String &TimeString);

#endif // GPS_H