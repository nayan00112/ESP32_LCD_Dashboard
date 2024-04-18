#include <WiFi.h>
#include <WebServer.h>

// For I2C LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define the I2C address for your LCD (use an I2C scanner if you're not sure)
#define I2C_ADDRESS 0x27

// Define the LCD dimensions (usually 16 columns and 2 rows)
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// Create an instance of the LiquidCrystal_I2C class
LiquidCrystal_I2C lcd(I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS);

int red_led = 5;
int green_led = 19;

/*Put your SSID & Password*/
const char* ssid = "abcd";  // Enter SSID here
const char* password = "password";  //Enter Password here

WebServer server(80);

void setup() {
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  delay(100);

  server.on("/", handle_OnConnect);
  server.on("/utlpost", HTTP_POST, handle_OnPostRequest);
  server.on("/utlget", HTTP_GET, handle_OnGetRequest);
  server.on("/ledIndicater", HTTP_GET, handle_OnGetRequest_ledIndicater);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hello All!");
  delay(1000);
}
void loop() {
  server.handleClient();
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML());
}

void handle_OnPostRequest() {
  String text = server.arg("text");
  textDisp(text, 0, 0);
  server.send(200, "text/html", SendHTML());
}

void handle_OnGetRequest() {
  if (server.hasArg("text")) {
    String text = server.arg("text");
    textDisp(text, 0, 0);

  } else {
    textDisp("Rady for Recive", 0, 0);
  }
  server.send(200, "text/html", SendHTML());
}

void handle_OnGetRequest_ledIndicater() {
  if (server.hasArg("status") ){

    String led_status = server.arg("status");

    if (led_status != "0") {
      ledControl(red_led);
      textDisp("Error", 0, 0);
    } else {
      ledControl(green_led);
      textDisp("Successfull", 0, 0);
    }

  } else {
    textDisp("Rady for Recive", 0, 0);
  }
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

void textDisp(String s, int u, int v) {
  lcd.clear();
  lcd.setCursor(u, v);

  // Set the delay between scrolling steps (adjust as needed)
  int scrollDelay = 500;

  // If the text length is greater than the LCD width
  if (s.length() > 16) {
    for (int i = 0; i <= s.length() - 16; i++) {
      lcd.clear();
      lcd.setCursor(u, v);
      lcd.print(s.substring(i, i + 16));
      delay(scrollDelay);
    }
  } else {
    // If the text fits on the LCD without scrolling
    if (s != "") {
      lcd.print(s);
    } else {
      lcd.print("Empty Value");
    }
    delay(1000);  // Adjust delay as needed
  }
}

void ledControl(int p) {
  int t = 6;
  while (t > 0) {
    digitalWrite(p, HIGH);
    delay(120);
    digitalWrite(p, LOW);
    delay(120);
    t -= 1;
  }
}

String SendHTML() {
  String ptr = "<!DOCTYPE html>\n";
  ptr += "<html lang='en'>\n";
  ptr += "<head>\n";
  ptr += "    <meta charset='UTF-8'>\n";
  ptr += "    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n";
  ptr += "    <title>ESP32 Display</title>\n";
  ptr += "    <style>\n";
  ptr += "        body {\n";
  ptr += "            font-family: 'Arial', sans-serif;\n";
  ptr += "            background-color: #f4f4f4;\n";
  ptr += "            color: #333;\n";
  ptr += "            margin: 0;\n";
  ptr += "            padding: 0;\n";
  ptr += "        }\n";
  ptr += "        div {\n";
  ptr += "            max-width: 600px;\n";
  ptr += "            margin: 50px auto;\n";
  ptr += "            padding: 20px;\n";
  ptr += "            background-color: #fff;\n";
  ptr += "            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);\n";
  ptr += "            border-radius: 8px;\n";
  ptr += "        }\n";
  ptr += "        h1 {\n";
  ptr += "            color: #007BFF;\n";
  ptr += "        }\n";
  ptr += "        p {\n";
  ptr += "            margin-bottom: 20px;\n";
  ptr += "        }\n";
  ptr += "        form {\n";
  ptr += "            display: flex;\n";
  ptr += "            flex-direction: column;\n";
  ptr += "        }\n";
  ptr += "        label {\n";
  ptr += "            margin-bottom: 8px;\n";
  ptr += "        }\n";
  ptr += "        input[type='text'] {\n";
  ptr += "            padding: 10px;\n";
  ptr += "            margin-bottom: 10px;\n";
  ptr += "            border: 1px solid #ccc;\n";
  ptr += "            border-radius: 4px;\n";
  ptr += "        }\n";
  ptr += "        input[type='submit'] {\n";
  ptr += "            background-color: #007BFF;\n";
  ptr += "            color: #fff;\n";
  ptr += "            padding: 10px;\n";
  ptr += "            border: none;\n";
  ptr += "            border-radius: 4px;\n";
  ptr += "            cursor: pointer;\n";
  ptr += "        }\n";
  ptr += "        input[type='submit']:hover {\n";
  ptr += "            background-color: #0056b3;\n";
  ptr += "        }\n";
  ptr += "        span {\n";
  ptr += "            display: block;\n";
  ptr += "            text-align: center;\n";
  ptr += "            padding: 10px 0px;\n";
  ptr += "            color: #5d5d5d;\n";
  ptr += "            font-size: 0.9rem;\n";
  ptr += "        }\n";
  ptr += "    </style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "    <div>\n";
  ptr += "        <h1>\n";
  ptr += "            Welcome to ESP32\n";
  ptr += "        </h1>\n";
  ptr += "        <p>\n";
  ptr += "            Write and submit text you want to display..\n";
  ptr += "        </p>\n";
  ptr += "        <form action='utlpost' method='post'>\n";
  ptr += "            <label for='text'>Text</label>\n";
  ptr += "            <input type='text' name='text' id='text'>\n";
  ptr += "            <input type='submit' value='Display' id='but'>\n";
  ptr += "        </form>\n";
  ptr += "        <footer>\n";
  ptr += "            <span>By Nayankumar 2024</span>\n";
  ptr += "        </footer>\n";
  ptr += "    </div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}