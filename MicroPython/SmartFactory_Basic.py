# ******************************************************************************************
# FileName     : SmartPot_Basic_python
# Description  : 스마트화분 코딩 키트 (기본)
# Author       : 손정인
# CopyRight    : (주)한국공학기술연구원(www.ketri.re.kr)
# Created Date : 2022.02.08
# Reference    :
# Modified     :
# ******************************************************************************************

# import
import time
from machine import Pin, time_pulse_us
from ETboard.lib.pin_define import *
from ETboard.lib.OLED_U8G2 import *

# global variable
oled = oled_u8g2()
RESET = Pin(D7)                                    # 카운트 리셋핀 ( D7 = 파란 버튼 )       
TRIG = Pin(D9)                                     # 초음파 송신부                
ECHO = Pin(D8)                                     # 초음파 수신부

count = 0                                          # 카운터용 변수
pre_time = 0                                       # 이전에 물건이 지나간 시간   
temp_buffer = [0] * 255                                     

# setup
    def setup():
    RESET.init(Pin.IN)                             # 리셋 버튼 입력 모드 설정하기    
    TRIG.init(Pin.OUT)                             # 초음파 송신부 출력 모드 설정하기                     
    ECHO.init(Pin.IN)                              # 초음파 수신부 입력 모드 설정하기

# main loop
def loop():
    global pre_time, count
    TRIG.value(LOW)
    ECHO.value(LOW)
    time.sleep_ms(2)
    TRIG.value(HIGH)
    time.sleep_ms(10)
    TRIG.value(LOW)
    
    duration = time_pulse_us(ECHO, HIGH)           # echoPin 이 HIGH 를 유지한 시간 저장
    distance = ((34 * duration) / 1000) / 2        # HIGH 였을 때 시간(초음파 송수신 시간)을 기준으로 거리를 계산
    
    print(f'{distance : .1f}', "cm")
    time.sleep(0.1)
    
    if( distance < 10 ) :                          # 물체와의 거리가 10cm 이하면
        now_time = int(round(time.time() * 1000))
        if(now_time - pre_time > 500) :
            count += 1                             # 한 번 카운트
            print("count : ", count)
            oled.clear()
            oled_show(count)                        
            time.sleep(1)
            
            pre_time = now_time;
            
    if(RESET.value() == LOW) :                     # 리셋 버튼을 누르면
        print("reset count")
        count = 0                                  # 카운트 초기화
        oled_show(count)
        print

def oled_show(count) :
    global temp_buffer
    temp_buffer = "count : %d" %(count)
    oled.setLine(1, "* Smart Factory *")
    oled.setLine(2, temp_buffer) 
    oled.setLine(3, "--------------")
    oled.display()
  
if __name__ == "__main__":
    setup()
    while True:
        loop()

# ==========================================================================================
#
#  (주)한국공학기술연구원 http://et.ketri.re.kr
#
# ==========================================================================================
