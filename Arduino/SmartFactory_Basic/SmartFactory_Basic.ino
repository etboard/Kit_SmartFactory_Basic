 /*****************************************************************************************
 * FileName     : SmartFactory_Basic
 * Description  : 스마트 팩토리 코딩 키트 (기본)
 * Author       : 위대원
 * CopyRight    : (주)한국공학기술연구원(www.ketri.re.kr)
 * Warning      : Arduino IDE에서 u8g2 라이브러리를 추가해서 컴파일 해야함
 * Created Date : 2021.09.14 : WDW : 최초 작성
 * Modified     : 2022.01.12 : SCS : 소스 크린징
 * Modified     : 2022.10.03 : SCS : support arduino uno with ET-Upboard
 * Modified     : 2022.11.23 : YSY : 소스 크린징, distance 값 수정 및 범위 지정, delay 수정
 * Modified     : 2022.12.19 : YSY : reset_pin_PIN -> reset_pin, 주석
 * Modified     : 2022.12.21 : YSY : 변수 명명법 통일
 * Modified     : 2022.12.21 : YSY : 2 < distance < 8
 * Modified     : 2023.03.13 : PEJ : 주석 및 코드 길이, 헤더 주석 수정
 * Modified     : 2024.02.02 : PEJ : 서보모터 배출 제어 로직 추가
 * Modified     : 2024.02.07 : PEJ : 주석 추가, 초음파 측정 시간 조절
 * Modified     : 2024.12.24 : 박은정 : 서보모터 라이브러리 변경
 *****************************************************************************************/


// OLED 제어를 위한 라이브러리 불러오기
#include "oled_u8g2.h"
OLED_U8G2 oled;

// Servo 제어를 위한 라이브러리 불러오기
#include <Servo.h>
Servo servo;

//-----------------------------------------------------------------------------------------
// ETBoard 핀번호 설정
//-----------------------------------------------------------------------------------------
#include "pins_arduino.h"                       // support arduino uno with ET-Upboard

int reset_pin = D7;                             // 카운트 리셋핀 (D7 = 파랑 버튼)

int servoPin = D6;                              // 서보모터 핀

int echo_pin  = D8;                             // 초음파 센서 수신부
int trig_pin  = D9;                             // 초음파 센서 송신부

int count = 0;                                  // 카운터용 변수
int pre_time = 0;                               // 이전에 물건이 지나간 시간


//=========================================================================================
void setup() {
//=========================================================================================
  Serial.begin(115200);                         // 시리얼 통신 준비
  oled.setup();                                 // OLED 셋업

  servo.attach(servoPin);                       // 서보모터 핀 설정

  pinMode(reset_pin, INPUT);                    // 리셋버튼 입력 모드 설정

  pinMode(trig_pin, OUTPUT);                    // 초음파 센서 송신부 출력 모드 설정
  pinMode(echo_pin, INPUT);                     // 초음파 센서 수신부 입력 모드 설정
}


//=========================================================================================
void loop() {
//=========================================================================================
  //-------------------------------------------------------------------------------------
  // 물체가 초음파 센서를 지나면 카운트 하기
  //-------------------------------------------------------------------------------------
  long duration, distance;
  digitalWrite(trig_pin, LOW);                  // 초음파 센서 거리 센싱 시작
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);                  // 초음파 센서 거리 센싱 종료

  duration = pulseIn (echo_pin, HIGH);          // 반사되어 돌아온 초음파의 시간을 저장
  distance = ((34 * duration) / 1000) / 2;      // 측정된 값을 cm로 변환하는 공식
  Serial.print("distance : ");                  // 측정된 거리를 시리얼 모니터에 출력
  Serial.print(distance);
  Serial.println("cm");

  if(distance > 2 && distance < 8) {            // 물체와의 거리가 2cm 초과 8cm 미만이면
    int now_time = millis();                    // 현재 시간 저장
    if(now_time - pre_time > 500) {             // 중복 카운트를 방지하기 위해 0.5초 초과면
      count += 1;                               // 카운트 1 증가
      delay(500);

      //---------------------------------------------------------------------------------
      // 서보모터 배출 제어
      //---------------------------------------------------------------------------------
      servo.write(75);
      delay(1000);
      servo.write(0);
      delay(1000);

      pre_time = now_time;                      // 이전 시각에 현재 시각 저장
    }
  }

  //-------------------------------------------------------------------------------------
  // 리셋 버튼을 누르면 카운트 초기화 하기
  //-------------------------------------------------------------------------------------
  if(digitalRead(reset_pin) == LOW) {           // 리셋 버튼을 누르면
    Serial.println("count reset");                
    count = 0;                                  // 카운트 초기화
  }

  Serial.print("count : ");             
  Serial.println(count);
  Serial.println("---------------------");

  //-------------------------------------------------------------------------------------
  // OLED 텍스트 표시
  //-------------------------------------------------------------------------------------
  char text1[32] = "count : ";                  // text1 count 값 표시
  char value1[32];
  String str1 = String(count, DEC);
  str1.toCharArray(value1, 6);
  strcat(text1, value1);

  oled.setLine(1, "*Smart Factory");            // OLED 첫 번째 줄 : 시스템 이름
  oled.setLine(2, text1);                       // OLED 두 번째 줄 : count 값
  oled.setLine(3, "---------------------");     // OLED 세 번째 줄 
  oled.display();

  delay(70);                                    // 초음파 측정 시 최소 시간 간격 0.07초
}


//=========================================================================================
//
// (주)한국공학기술연구원 http://et.ketri.re.kr
//
//=========================================================================================
