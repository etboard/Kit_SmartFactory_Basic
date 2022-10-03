/******************************************************************************************
 * FileName     : SmartFactory_Basic
 * Description  : 스마트팩토리 코딩 키트 (기본)
 * Author       : 위대원
 * CopyRight    : (주)한국공학기술연구원(www.ketri.re.kr)
 * Warning      : Arduino IDE에서 u8g2 라이브러리를 추가해서 컴파일 해야힘
 * Created Date : 2021.09.14 : WDW : 최초 작성
 * Modified     : 2022.01.12 : SCS : 소스 크린징
 * Modified     : 2022.10.03 : SCS : support arduino uno with ET-Upboard
******************************************************************************************/

//==========================================================================================
// Include & definition
//==========================================================================================
#include "pins_arduino.h"           // support arduino uno with ET-Upboard

// OLED 제어를 위한 라이브러리 불러오기
#include "oled_u8g2.h"
OLED_U8G2 oled;

int RESET_PIN = D7;                     // 카운트 리셋핀 (D7 = 파란 버튼)
int TRIG      = D9;                     // TRIG 핀 설정 (초음파 보내는 핀)
int ECHO      = D8;                     // ECHO 핀 설정 (초음파 받는 핀)

int count = 0;                          // 카운터용 변수
char temp_buffer[255] = {0, };          // 포멧팅용 임시 버퍼
int pre_time = 0;                       // 이전에 물건이 지나간 시간

//==========================================================================================
void oled_show(int count)
//==========================================================================================
{
  sprintf(temp_buffer, "count : %d", count);
  oled.setLine(1, "*Smart Factory");
  oled.setLine(2, temp_buffer);
  oled.setLine(3, "-------------");
  oled.display();
}

//==========================================================================================
void setup()
//==========================================================================================
{
  Serial.begin(115200);                 // 시리얼 통신 준비
  oled.setup();                         // OLED 셋업
  pinMode(TRIG, OUTPUT);                // 초음파 송신, 핀모드설정
  pinMode(ECHO, INPUT);                 // 초음파 수신, 핀모드설정
  pinMode(RESET_PIN, INPUT);            // 리셋버튼, 핀모드설정

  oled_show(count);
}

//==========================================================================================
void loop()
//==========================================================================================
{
  long duration, distance;
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn (ECHO, HIGH);    // 물체에 반사되어돌아온 초음파의 시간을 저장
  distance = duration * 17 / 1000;    // 측정된 값을 Cm 로 변환하기 위해 공식을 대입
  Serial.println(distance);

  if(distance < 30)
  {
    int now_time = millis();
    if(now_time - pre_time > 500)
    {
      count += 1;

      Serial.print("count : ");
      Serial.println(count);

      oled_show(count);
      delay(1000);

      pre_time = now_time;
    }
  }

  if(digitalRead(RESET_PIN) == LOW)
  {
    Serial.println("reset count");
    count = 0;
    oled_show(count);
  }
}


//==========================================================================================
//
// (주)한국공학기술연구원 http://et.ketri.re.kr
//
//==========================================================================================
