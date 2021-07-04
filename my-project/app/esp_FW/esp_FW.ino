#include <ESP8266WiFi.h>                                                // Библиотека для создания Wi-Fi подключения (клиент или точка доступа)
#include <ESP8266WebServer.h>                                           // Библиотека для управления устройством по HTTP (например из браузера)
#include <FS.h>                                                         // Библиотека для работы с файловой системой
#include <ESP8266FtpServer.h>                                           // Библиотека для работы с SPIFFS по FTP

const byte relay = 2;                                                   // Пин подключения сигнального контакта реле
const char *ssid = "IRSA";                                             // Название генерируемой точки доступа
const char *password = "tech_irsa@"; 

ESP8266WebServer HTTP(80);                                              // Определяем объект и порт сервера для работы с HTTP
FtpServer ftpSrv;                                                       // Определяем объект для работы с модулем по FTP (для отладки HTML)

void setup() {
  pinMode(relay,OUTPUT);                                                // Определяем пин реле как исходящий
  Serial.begin(9600);                                                 // Инициализируем вывод данных на серийный порт со скоростью 9600 бод
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  
                                              
  
  SPIFFS.begin();                                                       // Инициализируем работу с файловой системой                          
  HTTP.begin();                                                         // Инициализируем Web-сервер
  ftpSrv.begin("CL","1");                                        // Поднимаем FTP-сервер для удобства отладки работы HTML 

  Serial.print("\nMy IP to connect via Web-Browser or FTP: ");          // Выводим на монитор серийного порта сообщение о том, что сейчас будем выводить локальный IP
  Serial.println(WiFi.localIP());                                      // Выводим локальный IP-адрес ESP8266
  Serial.println("\n");

#define KEY_PLS    0x01
#define KEY_MNS    0x02
#define KEY_HEAT   0x03
#define KEY_SET    0x04
#define KEY_PWR    0x0F
#define KEY_STP    0x0A
#define KEY_BR     0x05 
#define KEY_IDL    0x0B

// Обработка HTTP-запросов
  HTTP.on("/Pwr", [](){                                        // При HTTP запросе вида http://192.168.4.1/relay_switch
      HTTP.send(200, "text/plain", key_switch(KEY_PWR));                     // Отдаём клиенту код успешной обработки запроса, сообщаем, что формат ответа текстовый и возвращаем результат выполнения функции relay_switch 
  });
  HTTP.on("/up", [](){                                        // При HTTP запросе вида http://192.168.4.1/relay_status
      HTTP.send(200, "text/plain", key_switch(KEY_PLS));       // Отдаём клиенту код успешной обработки запроса, сообщаем, что формат ответа текстовый и возвращаем результат выполнения функции relay_status 
  });
  
  HTTP.on("/mode", [](){                                        // При HTTP запросе вида http://192.168.4.1/relay_status
      HTTP.send(200, "text/plain", key_switch(KEY_SET));                     // Отдаём клиенту код успешной обработки запроса, сообщаем, что формат ответа текстовый и возвращаем результат выполнения функции relay_status 
  });

  HTTP.on("/dwn", [](){                                        // При HTTP запросе вида http://192.168.4.1/relay_status
      HTTP.send(200, "text/plain", key_switch(KEY_MNS));                     // Отдаём клиенту код успешной обработки запроса, сообщаем, что формат ответа текстовый и возвращаем результат выполнения функции relay_status 
  });

  HTTP.on("/heat", [](){                                        // При HTTP запросе вида http://192.168.4.1/relay_status
      HTTP.send(200, "text/plain", key_switch(KEY_HEAT));                     // Отдаём клиенту код успешной обработки запроса, сообщаем, что формат ответа текстовый и возвращаем результат выполнения функции relay_status 
  });
  HTTP.onNotFound([](){                                                 // Описываем действия при событии "Не найдено"
  if(!handleFileRead(HTTP.uri()))                                       // Если функция handleFileRead (описана ниже) возвращает значение false в ответ на поиск файла в файловой системе
      HTTP.send(404, "text/plain", "Not Found");                        // возвращаем на запрос текстовое сообщение "File isn't found" с кодом 404 (не найдено)
  });
}

void loop() {
    HTTP.handleClient();                                                // Обработчик HTTP-событий (отлавливает HTTP-запросы к устройству и обрабатывает их в соответствии с выше описанным алгоритмом)
    ftpSrv.handleFTP();                                                 // Обработчик FTP-соединений  
}

String key_switch(byte k) {                                                 // Функция переключения реле 
  byte state;
  if (digitalRead(relay))                                               // Если на пине реле высокий уровень   
    state = 0;                                                          //  то запоминаем, что его надо поменять на низкий
  else                                                                  // иначе
    state = 1;                                                          //  запоминаем, что надо поменять на высокий
  digitalWrite(relay, state);                                           // меняем значение на пине подключения реле
    Serial.write(k);
  /* Serial.print("KEY=");
  Serial.print(String(k));                                                      // Выводим на монитор серийного порта сообщение о том, что сейчас будем выводить локальный IP
  Serial.println("\n");*/
  return String(state);                                                 // возвращаем результат, преобразовав число в строку
}



bool handleFileRead(String path){                                       // Функция работы с файловой системой
  if(path.endsWith("/")) path += "index.html";                          // Если устройство вызывается по корневому адресу, то должен вызываться файл index.html (добавляем его в конец адреса)
  String contentType = getContentType(path);                            // С помощью функции getContentType (описана ниже) определяем по типу файла (в адресе обращения) какой заголовок необходимо возвращать по его вызову
  if(SPIFFS.exists(path)){                                              // Если в файловой системе существует файл по адресу обращения
    File file = SPIFFS.open(path, "r");                                 //  Открываем файл для чтения
    size_t sent = HTTP.streamFile(file, contentType);                   //  Выводим содержимое файла по HTTP, указывая заголовок типа содержимого contentType
    file.close();                                                       //  Закрываем файл
    return true;                                                        //  Завершаем выполнение функции, возвращая результатом ее исполнения true (истина)
  }
  return false;                                                         // Завершаем выполнение функции, возвращая результатом ее исполнения false (если не обработалось предыдущее условие)
}

String getContentType(String filename){                                 // Функция, возвращающая необходимый заголовок типа содержимого в зависимости от расширения файла
  if (filename.endsWith(".html")) return "text/html";                   // Если файл заканчивается на ".html", то возвращаем заголовок "text/html" и завершаем выполнение функции
  else if (filename.endsWith(".css")) return "text/css";                // Если файл заканчивается на ".css", то возвращаем заголовок "text/css" и завершаем выполнение функции
  else if (filename.endsWith(".js")) return "application/javascript";   // Если файл заканчивается на ".js", то возвращаем заголовок "application/javascript" и завершаем выполнение функции
  else if (filename.endsWith(".png")) return "image/png";               // Если файл заканчивается на ".png", то возвращаем заголовок "image/png" и завершаем выполнение функции
  else if (filename.endsWith(".jpg")) return "image/jpeg";              // Если файл заканчивается на ".jpg", то возвращаем заголовок "image/jpg" и завершаем выполнение функции
  else if (filename.endsWith(".gif")) return "image/gif";               // Если файл заканчивается на ".gif", то возвращаем заголовок "image/gif" и завершаем выполнение функции
  else if (filename.endsWith(".ico")) return "image/x-icon";            // Если файл заканчивается на ".ico", то возвращаем заголовок "image/x-icon" и завершаем выполнение функции
  else if (filename.endsWith(".svg")) return "image/svg+xml";
  return "text/plain";                                                  // Если ни один из типов файла не совпал, то считаем что содержимое файла текстовое, отдаем соответствующий заголовок и завершаем выполнение функции
}
