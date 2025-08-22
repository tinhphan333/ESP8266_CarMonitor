/*
 * @file Gps.cpp
 * @brief Định nghĩa các hàm để nhận và xử lý dữ liệu từ module GPS Neo-6M
 * @details File này chứa các hàm để nhận và xử lý dữ liệu từ module GPS Neo-6M.
 * @author phantinhphan333
 * @date 2024-11-02
 */
#include "GPS.h"

String LocationProcess(String &LocationString)
{
    // Nhận dữ liệu vị trí
    if (gps.location.isValid())
    {
        LocationString = "";
        float Latitude = gps.location.lat();  // Lấy vĩ độ
        float Longitude = gps.location.lng(); // Lấy kinh độ

        String LatitudeString = String(Latitude, 6);
        String LongitudeString = String(Longitude, 6);

        // Định dạng chuỗi vị trí sao cho phù hợp với google maps
        LocationString += "@";
        LocationString += LatitudeString;
        LocationString += ",";
        LocationString += LongitudeString;
        LocationString += ",18z";
    }
    return LocationString;
}

String DateProcess(String &DateString)
{
    // Nhận dữ liệu ngày tháng năm
    if (gps.date.isValid())
    {
        // Định dạng chuỗi ngày tháng năm
        DateString = "";
        int year = gps.date.year();   // Lấy năm
        int month = gps.date.month(); // Lấy tháng
        int date = gps.date.day();    // Lấy ngày

        DateString += String(year);
        DateString += "-";

        if (month < 10)
            DateString += '0';
        DateString += String(month);
        DateString += "-";

        if (date < 10)
            DateString += '0';
        DateString += String(date);
    }
    return DateString;
}

String TimeProcess(String &TimeString)
{
    // Nhận dữ liệu thời gian
    if (gps.time.isValid())
    {
        // Định dạng chuỗi thời gian
        TimeString = "";
        int hour = gps.time.hour() + 7; // chênh lệch múi giờ
        int minute = gps.time.minute();
        int second = gps.time.second();

        if (hour < 10)
            TimeString += '0';
        TimeString += String(hour);
        TimeString += " : ";

        if (minute < 10)
            TimeString += '0';
        TimeString += String(minute);
        TimeString += " : ";

        if (second < 10)
            TimeString += '0';
        TimeString += String(second);
    }
    return TimeString;
}
