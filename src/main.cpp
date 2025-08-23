#include <Gps.h>         // Khai báo các hàm nhận và xử lý dữ liệu từ module GPS Neo-6M
#include <Obd2.h>        // Khai báo các hàm để yêu cầu và giải mã dữ liệu CAN thông qua chuẩn OBD-II
#include <ESP8266WiFi.h> // Thư viện để kết nối WiFi
#include <Firebase.h>    // Thư viện để làm việc với Firebase

// Khai báo thông tin kết nối WiFi và Firebase
#define WIFI_SSID "phantinh_ne"
#define WIFI_PASSWORD "12345678"
#define API_KEY "datn-car-supervisor-default-rtdb.firebaseio.com"
#define DATABASE_URL "BOV1iatWLkigeV6rPQrqo0fDDoSHKOw8UboxzLqa"

// Khởi tạo đối tượng FirebaseData
FirebaseData firebaseData;

// Khai báo các đối tượng cấu hình Firebase
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

// Khởi tạo đối tượng TinyGPSPlus để xử lý dữ liệu GPS
TinyGPSPlus gps;
// Khởi tạo cổng Serial để giao tiếp với module GPS
SoftwareSerial SerialGPS(D2, D1);

const int SPI_CS_PIN = 15; // Chân GPIO 15 của ESP8266 nối chân Chip select của MCP2515
MCP_CAN CAN(SPI_CS_PIN);   // Khởi tạo đối tượng CAN để giao tiếp với MCP2515

void GPS_task()
{
  while (SerialGPS.available() > 0) // Kiểm tra dữ liệu từ module GPS (một lần)
  {
    if (gps.encode(SerialGPS.read())) // Đọc và xử lý một byte duy nhất
    {
      // Khai báo các chuỗi để lưu trữ dữ liệu từ module GPS
      String DateString, TimeString, LocationString;

      // Xử lý dữ liệu vị trí, ngày tháng và thời gian
      LocationProcess(gps, LocationString);
      DateProcess(gps, DateString);
      TimeProcess(gps, TimeString);

      // Gửi dữ liệu lên Firebase
      if (Firebase.ready())
      {
        Firebase.setString(firebaseData, "/DATN/Xe 1/location", LocationString);
        Firebase.setString(firebaseData, "/DATN/Xe 1/date", DateString);
        Firebase.setString(firebaseData, "/DATN/Xe 1/time", TimeString);
      }
    }
  }
}

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

  // Thiết lập bộ lọc CAN_message cho MCP2515
  CAN.init_Mask(0, 0, 0x7FC);
  CAN.init_Mask(1, 0, 0x7FC);
  CAN.init_Filt(0, 0, 0x7E8);
  CAN.init_Filt(1, 0, 0x7E8);
  CAN.init_Filt(2, 0, 0x7E8);
  CAN.init_Filt(3, 0, 0x7E8);
  CAN.init_Filt(4, 0, 0x7E8);
  CAN.init_Filt(5, 0, 0x7E8);

  //------------------------------------------------------------------------------
  SerialGPS.begin(9600);

  // Kiểm tra kết nối WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Đang kết nối Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(300);
    Serial.print(".");
  }
  Serial.println(" Kết nối WIFI thành công ");
  //------------------------------------------------------------------------------

  // Gán thông tin cấu hình Firebase
  firebaseConfig.api_key = API_KEY;
  firebaseConfig.database_url = DATABASE_URL;

  Firebase.begin(&firebaseConfig, &firebaseAuth); // Kết nối đến Firebase
  Firebase.reconnectWiFi(true);                   // Tự động kết nối lại WiFi khi mất kết nối
}

void loop()
{
  Send_task(CAN);
  Receive_task(firebaseData, CAN);
  GPS_task();
}
