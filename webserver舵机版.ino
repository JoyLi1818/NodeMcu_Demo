/**********************************************************************
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : 3_2_2_Turning_on_and_off_an_LED
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : CYNO朔
日期/Date（YYYYMMDD）     : 20191108
程序目的/Purpose          : 使用NodeMCU建立基本服务器。用户可通过浏览器使用8266的IP地址
                           访问8266所建立的基本网页并通过该页面点亮/熄灭NodeMCU的内置LED
-----------------------------------------------------------------------
修订历史/Revision History  
日期/Date    作者/Author      参考号/Ref    修订说明/Revision Description
 
***********************************************************************/
#include <ESP8266WiFi.h>        // 本程序使用 ESP8266WiFi库
#include <ESP8266WiFiMulti.h>   //  ESP8266WiFiMulti库
#include <ESP8266WebServer.h>   //  ESP8266WebServer库
#include "webhtml.h"
#include <Servo.h>

#define ANGLE_ON  0    //开灯时的舵机角度
#define ANGLE_OFF 180   //关灯时的舵机角度
#define ANGLE_NONE 90  //置空时的舵机角度

#define buttonPin D3            // 按键D2引脚
#define buttonPin D4            // 按键D2引脚

IPAddress local_IP(192, 168, 31, 123);//想要设定的静态ip
IPAddress gateway(192, 168, 31, 1);//当前网关
IPAddress subnet(255, 255, 255, 0);//当前子网掩码
 
ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是 'wifiMulti'
 
ESP8266WebServer esp8266_server(80);// 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）

Servo left;
Servo right;
int angle=ANGLE_NONE;//舵机角度
 
void setup(void){
  Serial.begin(9600);   // 启动串口通讯
  left.attach(D3);
  right.attach(D4);
  pinMode(LED_BUILTIN, OUTPUT); //设置内置LED引脚为输出模式以便控制LED
  
  wifiMulti.addAP("417", "chengda417417"); // 将需要连接的一系列WiFi ID和密码输入这里
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2"); // ESP8266-NodeMCU再启动后会扫描当前网络
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3"); // 环境查找是否有这里列出的WiFi ID。如果有
  Serial.println("Connecting ...");                            // 则尝试使用此处存储的密码进行连接。
  
  WiFi.config(local_IP, gateway, subnet);//设置静态IP
  WiFi.mode(WIFI_STA);//设置为接收WiFi模式

  int i = 0;                                 
  while (wifiMulti.run() != WL_CONNECTED) {  // 此处的wifiMulti.run()是重点。通过wifiMulti.run()，NodeMCU将会在当前
    delay(1000);                             // 环境中搜索addAP函数所存储的WiFi。如果搜到多个存储的WiFi那么NodeMCU
    Serial.print(i++); Serial.print(' ');    // 将会连接信号最强的那一个WiFi信号。
  }                                          // 一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。这也是
                                             // 此处while循环判断是否跳出循环的条件。
  
  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // 通过串口监视器输出连接的WiFi名称
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // 通过串口监视器输出ESP8266-NodeMCU的IP
 
  esp8266_server.begin();                           // 启动网站服务
  esp8266_server.on("/", HTTP_GET, handleRoot);     // 设置服务器根目录即'/'的函数'handleRoot'
  esp8266_server.on("/ON_light", HTTP_GET, ON_light);  // 设置处理LED控制请求的函数'handleLED'
  esp8266_server.on("/OFF_light", HTTP_GET, OFF_light);  // 设置处理LED控制请求的函数'handleLED'
  esp8266_server.onNotFound(handleNotFound);        // 设置处理404情况的函数'handleNotFound'
 
  Serial.println("HTTP esp8266_server started");//  告知用户ESP8266网络服务功能已经启动
}
 
void loop(void){
  esp8266_server.handleClient();                     // 检查http服务器访问
}
 
/*设置服务器根目录即'/'的函数'handleRoot'
  该函数的作用是每当有客户端访问NodeMCU服务器根目录时，
  NodeMCU都会向访问设备发送 HTTP 状态 200 (Ok) 这是send函数的第一个参数。
  同时NodeMCU还会向浏览器发送HTML代码，以下示例中send函数中第三个参数，
  也就是双引号中的内容就是NodeMCU发送的HTML代码。该代码可在网页中产生LED控制按钮。 
  当用户按下按钮时，浏览器将会向NodeMCU的/LED页面发送HTTP请求，请求方式为POST。
  NodeMCU接收到此请求后将会执行handleLED函数内容*/
void handleRoot() {       
  esp8266_server.send(200, "text/html", String(HTML));
}
 
//开灯
void ON_light() {                         

  angle = ANGLE_OFF;
  left.write(angle);// 0~180°
  delay(500);
  angle = ANGLE_ON;
  right.write(angle);// 0~180°
  delay(500);
  right.write(ANGLE_NONE);//将舵机重新置空，以免一直受力损坏
  delay(500);
  left.write(ANGLE_NONE);//将舵机重新置空，以免一直受力损坏
  delay(500);
  digitalWrite(LED_BUILTIN,HIGH);// 改变LED的点亮或者熄灭状态
  Serial.println("开灯");

  esp8266_server.sendHeader("Location","/");          // 跳转回页面根目录
  esp8266_server.send(303);                           // 发送Http相应代码303 跳转  
}

//关灯
void OFF_light() {    

  angle = ANGLE_ON;
  left.write(angle);// 0~180°
  delay(500);
  angle = ANGLE_OFF;
  right.write(angle);// 0~180°
  delay(500);
  right.write(ANGLE_NONE);//将舵机重新置空，以免一直受力损坏
  delay(500);
  left.write(ANGLE_NONE);//将舵机重新置空，以免一直受力损坏
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);// 改变LED的点亮或者熄灭状态
  Serial.println("关灯");

  esp8266_server.sendHeader("Location","/");          // 跳转回页面根目录
  esp8266_server.send(303);                           // 发送Http相应代码303 跳转  
}
 
// 设置处理404情况的函数'handleNotFound'
void handleNotFound(){
  esp8266_server.send(404, "text/plain", "你干嘛？哎哟！"); // 发送 HTTP 状态 404 (未找到页面) 并向浏览器发送文字 "404: Not found"
}