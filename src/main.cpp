#include <TinyGPSPlus.h>         // Khai báo các hàm tương tác với module Neo-6M
#include <SoftwareSerial.h>      // Khai báo các hàm để giao tiếp với module GPS qua cổng Serial
#include <Obd2.h>                // Khai báo các hàm để yêu cầu và giải mã dữ liệu CAN thông qua chuẩn OBD-II
#include <ESP8266WiFi.h>         // Thư viện để kết nối WiFi
#include <Firebase_ESP_Client.h> // Thư viện để làm việc với Firebase
//------------------------------------------------------------------------------

// Khai báo thông tin kết nối WiFi và Firebase
#define WIFI_SSID "phantinh_ne"
#define WIFI_PASSWORD "12345678"
#define FIREBASE_HOST "datn-car-supervisor-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "BOV1iatWLkigeV6rPQrqo0fDDoSHKOw8UboxzLqa"
// khởi tạo đối tượng FirebaseData
FirebaseData firebaseData;
//------------------------------------------------------------------------------

// Khởi tạo đối tượng TinyGPSPlus để xử lý dữ liệu GPS
TinyGPSPlus gps;
// Khởi tạo cổng Serial để giao tiếp với module GPS
SoftwareSerial SerialGPS(D2, D1);

// Khai báo các biến để lưu trữ dữ liệu từ module GPS
float Latitude, Longitude;                                                // Kinh độ, Vĩ độ
int year, month, date, hour, minute, second;                              // Thời gian
String DateString, TimeString, LatitudeString, LongitudeString, Location; // Chuỗi lưu trữ

void setup()
{
  Serial.begin(115200); // Khởi tạo Serial Monitor

  // Kiểm tra khởi tạo MCP2515
  while (CAN_OK != CAN.begin(CAN_1000KBPS)) // Tốc độ truyền CAN phương tiện là: 1000kbps
  {
    Serial.println("Khởi tạo MCP2515 thất bại");
    delay(100);
  }
  Serial.println("Khởi tạo MCP2515 thành công");
  set_mask_filt(); // Hàm để thiết lập bộ lọc CAN_message cho MCP2515
  //------------------------------------------------------------------------------

  // Kiểm tra kết nối WiFi và cấu hình Firebase
  SerialGPS.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Đang kết nối Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(300);
    Serial.print(".");
  }
  Serial.println(" Kết nối WIFI thành công ");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // Kết nối đến Firebase
  Firebase.reconnectWiFi(true);                 // Tự động kết nối lại WiFi khi mất kết nối
}

void loop()
{
  Send_task();
  delay(1000); // Đợi 1 giây trước khi gửi yêu cầu tiếp theo
  taskCanRecv();

  // NEO-6M
  while (SerialGPS.available() > 0)
    if (gps.encode(SerialGPS.read()))
    {
      // location
      if (gps.location.isValid())
      {
        Latitude = gps.location.lat();
        LatitudeString = String(Latitude, 6);
        if (Firebase.setString(firebaseData, "/DATN/Xe 1/Latitude", LatitudeString))
          ;

        Longitude = gps.location.lng();
        LongitudeString = String(Longitude, 6);
        if (Firebase.setString(firebaseData, "/DATN/Xe 1/Longitude", LongitudeString))
          ;

        // Location = "@" + LatitudeString + "," + LongitudeString + ",18z" ;
        // if (Firebase.setString(firebaseData, "/Location", Location)) ;
      }
      // date
      if (gps.date.isValid())

      {
        DateString = "";
        date = gps.date.day();
        month = gps.date.month();
        year = gps.date.year();

        if (date < 10)
          DateString = '0';
        DateString += String(date);
        DateString += " / ";

        if (month < 10)
          DateString += '0';
        DateString += String(month);
        DateString += " / ";
        DateString += String(year);

        if (Firebase.setString(firebaseData, "/DATN/Xe 1/date", DateString))
          ;
      }
      // Tính toán thời gian
      if (gps.time.isValid())
      {
        TimeString = "";
        hour = gps.time.hour() + 7; // chênh lệch múi giờ
        minute = gps.time.minute();
        second = gps.time.second();

        if (hour < 10)
          TimeString = '0';
        TimeString += String(hour);
        TimeString += " : ";

        if (minute < 10)
          TimeString += '0';
        TimeString += String(minute);
        TimeString += " : ";

        if (second < 10)
          TimeString += '0';
        TimeString += String(second);

        if (Firebase.setString(firebaseData, "/DATN/Xe 1/time", TimeString))
          ;
      }
      break;
    }
}
