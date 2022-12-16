# ******************************************************************************************
# FileName     : SmartPot_Basic_python
# Description  : 스마트화분 코딩 키트 (기본)
# Author       : 손정인
# CopyRight    : (주)한국공학기술연구원(www.ketri.re.kr)
# Created Date : 2022.02.08
# Reference    :
# Modified     : 2022.11.23 : YSY : 소스 크린징, distance범위지정 
# Modified     :
# ******************************************************************************************

# import
import time                                        # 시간 관련 모듈
from machine import Pin, time_pulse_us             # 핀 및 시간 관련 모듈
from ETboard.lib.pin_define import *               # ETboard 핀 관련 모듈
from ETboard.lib.OLED_U8G2 import *                # ETboard OLED 관련 모듈

# global variable
oled = oled_u8g2()                                 # OLED
RESET = Pin(D6)                                    # 카운트 리셋핀 ( D6 = 빨강 버튼 )
TRIG = Pin(D9)                                     # 초음파 송신부
ECHO = Pin(D8)                                     # 초음파 수신부

count = 0                                          # 카운터용 변수
pre_time = 0                                       # 이전에 물건이 지나간 시간
temp_buffer = [0] * 255                            # OLED 표시용 버퍼

# setup
def setup():
    RESET.init(Pin.IN)                             # 리셋 버튼 입력 모드 설정하기
    TRIG.init(Pin.OUT)                             # 초음파 송신부 출력 모드 설정하기
    ECHO.init(Pin.IN)                              # 초음파 수신부 입력 모드 설정하기

# main loop
def loop():
    global pre_time, count                         # 이전 시각
    TRIG.value(LOW)                                # 초음파 거리 센싱 시작
    ECHO.value(LOW)
    time.sleep_ms(2)
    TRIG.value(HIGH)
    time.sleep_ms(10)
    TRIG.value(LOW)                                # 초음파 거리 센싱 종료

    duration = time_pulse_us(ECHO, HIGH)           # echoPin 이 HIGH 를 유지한 시간 저장
    distance = ((34 * duration) / 1000) / 2        # HIGH 였을 때 시간(초음파 송수신 시간)을 기준으로 거리를 계산

    print(f'{distance : .1f}', "cm")               # 출력
    time.sleep(0.1)                                # 0.1초 딜레이

    if( distance > 2 and distance < 10 ) :                          # 물체와의 거리가 10cm 이하면
        now_time = int(round(time.time() * 1000))  # 현재 시각(1/000초)
        if(now_time - pre_time > 500) :            # 0.5초 이상인 경우만, 중복 카운트 방지
            count += 1                             # 한 번 카운트
            print("count : ", count)               # 출력
            oled.clear()                           # OLED 화면 클리어
            oled_show(count)                       # OLED 화면에 카운트 표시
            time.sleep(1)                          # 1초 딜레이

            pre_time = now_time;                   # 이전 시각에 현재 시각 저장

    if(RESET.value() == LOW) :                     # 리셋 버튼을 누르면
        print("reset count")                       # 출력
        count = 0                                  # 카운트 초기화
        oled_show(count)                           # OLED 화면에 카운트 표시
        print                                      # 출력

# OLED에 카운트 표시
def oled_show(count) :
    global temp_buffer                             # 전역 변수 사용
    temp_buffer = "count : %d" %(count)            # 표시할 메시지 편집
    oled.setLine(1, "* Smart Factory *")           # OLED 1번째 줄 설정
    oled.setLine(2, temp_buffer)                   # OLED 2번째 줄 설정
    oled.setLine(3, "--------------")              # OLED 3번째 줄 설정
    oled.display()                                 # OLED 표시

if __name__ == "__main__":
    setup()
    while True:
        loop()

# ==========================================================================================
#
#  (주)한국공학기술연구원 http://et.ketri.re.kr
#
# ==========================================================================================