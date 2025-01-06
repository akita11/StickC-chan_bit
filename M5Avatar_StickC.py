from m5stack import *
from m5ui import *
from uiflow import *


setScreenColor(0x222222)








# M5StickCAvatar BLOCK
# Based on M5StackAvatarPython : https://github.com/h-akanuma/M5StackAvatarPython
# v1.0 (220801)
# Copyright (c) 2022 @akita11, Released under the MIT license

import random
import math
setScreenColor(0x000000)
#lcd.font(lcd.FONT_Default, 90)

M5Avatar_fw, M5Avatar_fh = lcd.fontSize()
M5Avatar_ww, M5Avatar_wh = lcd.screensize()
M5Avatar_eye_x = 90
M5Avatar_eye_y = 80
M5Avatar_eye_r = 10
#M5Avatar_eye_close_x= 70
M5Avatar_eye_close_width  = 6
M5Avatar_eye_close_height = 40
M5Avatar_blink_term_ms    = 500
M5Avatar_mouth_x = 40
M5Avatar_mouth_y = 120
M5Avatar_mouth_width = 7
M5Avatar_mouth_height = 50
M5Avatar_mouth_close = True
M5Avatar_mouth_open_width = 30
M5Avatar_exclamation_x = 60
M5Avatar_exclamation_y = 220
M5Avatar_exclamation_width  = 30
M5Avatar_exclamation_height = 10
M5Avatar_exclamation_space  = 8
M5Avatar_angry_x = 110
M5Avatar_angry_y = 210
M5Avatar_angry_width = 2
M5Avatar_angry_length = 10
M5Avatar_angry_gap = 10
tm_blink = 0
st_blink = 0
tm_mouth = 0
st_mouth = 0
tm_speak = 0
timerAvatar_period = 100
tm_blink_open = random.randint(2, 6) * 1000
tm_mouth_period = 500
M5Avatar_spaces = ' '
fSpeaking = False

while lcd.textWidth(M5Avatar_spaces) < M5Avatar_ww:
  M5Avatar_spaces += ' '

@timerSch.event('timerAvatar')
def ttimerAvatar():
  global tm_speak
  _breath()
  _blink()
#  if fSpeaking == False:
#    _mouth()
  tm_speak = tm_speak + timerAvatar_period
  pass

dev = 0
breath_T = 0
breath_cycle = 30
breath_dev = 4

def _breath():
  global dev, breath_T
  breath_T = (breath_T + 1) % breath_cycle
  dev = int(breath_dev * math.sin(breath_T * 2 * math.pi / breath_cycle))

def _blink():
    global tm_blink, st_blink, tm_blink_open
    tm_blink = tm_blink + timerAvatar_period
    if st_blink == 0:
      _eye_close()
      if tm_blink >= M5Avatar_blink_term_ms:
        st_blink = 1
        tm_blink = 0
    else:
      _eye_open()
      if tm_blink >= tm_blink_open:
        tm_blink_open = random.randint(2, 6) * 1000
        st_blink = 0
        tm_blink = 0

def _eye_close():
    lcd.circle(M5Avatar_eye_x + dev, M5Avatar_eye_y, M5Avatar_eye_r+10, lcd.BLACK, lcd.BLACK)
    lcd.circle(M5Avatar_eye_x + dev, M5Avatar_wh - M5Avatar_eye_y, M5Avatar_eye_r+10, lcd.BLACK, lcd.BLACK)
    lcd.rect(
        M5Avatar_eye_x - M5Avatar_eye_close_width // 2 + dev,
        M5Avatar_eye_y - M5Avatar_eye_close_height // 2,
        M5Avatar_eye_close_width, M5Avatar_eye_close_height, lcd.WHITE, lcd.WHITE)
    lcd.rect(
        M5Avatar_eye_x - M5Avatar_eye_close_width // 2 + dev,
        M5Avatar_wh - M5Avatar_eye_y - M5Avatar_eye_close_height // 2,
        M5Avatar_eye_close_width, M5Avatar_eye_close_height,
        lcd.WHITE, lcd.WHITE
    )

def _eye_open():
    lcd.rect(
        M5Avatar_eye_x - M5Avatar_eye_close_width // 2 + dev - 10,
        M5Avatar_eye_y - M5Avatar_eye_close_height // 2,
        M5Avatar_eye_close_width + 20, M5Avatar_eye_close_height, lcd.BLACK, lcd.BLACK)
    lcd.rect(
        M5Avatar_eye_x - M5Avatar_eye_close_width // 2 + dev - 10,
        M5Avatar_wh - M5Avatar_eye_y - M5Avatar_eye_close_height // 2,
        M5Avatar_eye_close_width + 20, M5Avatar_eye_close_height,
        lcd.BLACK, lcd.BLACK
    )
    lcd.circle(M5Avatar_eye_x + dev, M5Avatar_eye_y, M5Avatar_eye_r, lcd.WHITE, lcd.WHITE)
    lcd.circle(M5Avatar_eye_x + dev, M5Avatar_wh - M5Avatar_eye_y, M5Avatar_eye_r, lcd.WHITE, lcd.WHITE)

def _mouth():
  global tm_mouth
  tm_mouth = tm_mouth + timerAvatar_period
  if tm_mouth >= tm_mouth_period:
    tm_mouth = 0
  lcd.rect(
#    M5Avatar_mouth_x - M5Avatar_mouth_width // 2 + dev - 10,
    M5Avatar_mouth_x - M5Avatar_mouth_width // 2,
    M5Avatar_mouth_y - M5Avatar_mouth_height // 2,
    M5Avatar_mouth_width + 20,
    M5Avatar_mouth_height,
    lcd.BLACK,
    lcd.BLACK
  )
  lcd.rect(
#    M5Avatar_mouth_x - M5Avatar_mouth_width // 2 + dev,
    M5Avatar_mouth_x - M5Avatar_mouth_width // 2,
    M5Avatar_mouth_y - M5Avatar_mouth_height // 2,
    M5Avatar_mouth_width,
    M5Avatar_mouth_height,
    lcd.WHITE,
    lcd.WHITE
  )

def _lipsync():
  if M5Avatar_mouth_close:
    _lip_open()
  else:
    _lip_close()

def _lip_close():
  global M5Avatar_mouth_close
  lcd.rect(
#    M5Avatar_mouth_x - M5Avatar_mouth_open_width // 2 + dev - 10,
    M5Avatar_mouth_x - M5Avatar_mouth_open_width // 2,
    M5Avatar_mouth_y - M5Avatar_mouth_height // 2,
    M5Avatar_mouth_open_width,
    M5Avatar_mouth_height,
    lcd.BLACK,
    lcd.BLACK
  )
  lcd.rect(
#    M5Avatar_mouth_x - M5Avatar_mouth_width // 2 + dev,
    M5Avatar_mouth_x - M5Avatar_mouth_width // 2,
    M5Avatar_mouth_y - M5Avatar_mouth_height // 2,
    M5Avatar_mouth_width,
    M5Avatar_mouth_height,
    lcd.WHITE,
    lcd.WHITE
  )
  M5Avatar_mouth_close = True

def _lip_open():
  global M5Avatar_mouth_close
  lcd.rect(
#    M5Avatar_mouth_x - M5Avatar_mouth_open_width // 2 + dev - 10,
    M5Avatar_mouth_x - M5Avatar_mouth_open_width // 2,
    M5Avatar_mouth_y - M5Avatar_mouth_height // 2,
    M5Avatar_mouth_open_width,
    M5Avatar_mouth_height,
    lcd.BLACK,
    lcd.BLACK
  )
  lcd.rect(
#    M5Avatar_mouth_x - M5Avatar_mouth_open_width // 2 + dev,
    M5Avatar_mouth_x - M5Avatar_mouth_open_width // 2,
    M5Avatar_mouth_y - M5Avatar_mouth_height // 2,
    M5Avatar_mouth_open_width,
    M5Avatar_mouth_height,
    lcd.WHITE,
    lcd.WHITE
  )
  M5Avatar_mouth_close = False

def _speak(text):
  global tm_speak
  global fSpeaking
  fSpeaking = True
#  lcd.setColor(lcd.BLACK, lcd.WHITE)
#  lcd.arc((M5Avatar_eye_x + M5Avatar_mouth_x) // 2, (M5Avatar_wh - M5Avatar_fh) - 5, 25, 25, 270, 360, lcd.WHITE, lcd.WHITE)
#  lcd.rect(0, (M5Avatar_wh - M5Avatar_fh) - 5, M5Avatar_ww + 5, M5Avatar_fh + 5, lcd.WHITE, lcd.WHITE)
#  lcd.textClear(0, (M5Avatar_wh - M5Avatar_fh) - 1, M5Avatar_spaces, lcd.WHITE)
#  lcd.print(text, 0, lcd.BOTTOM, lcd.BLACK)
#  lcd.rect(0, 0, 24, M5Avatar_wh, lcd.WHITE, lcd.WHITE)
  labelText = M5TextBox(24, 20, "", lcd.FONT_DejaVu24, 0xffffff, rotate=90)
  labelText.setText(text)
  _lipsync()
  while tm_speak < 500:
    pass
  tm_speak = 0
  while lcd.textWidth(text) > 0:
    text = text[1:]
#    lcd.textClear(0, (M5Avatar_wh - M5Avatar_fh) - 1, M5Avatar_spaces, lcd.WHITE)
#    lcd.rect(0, 0, 24, M5Avatar_wh, lcd.WHITE, lcd.WHITE)
    labelText.setText(text)
    _lipsync()
    while tm_speak < 500:
      pass
    tm_speak = 0
#  lcd.rect(0, (M5Avatar_wh - M5Avatar_fh) - 5, M5Avatar_ww, M5Avatar_fh + 5, lcd.BLACK, lcd.BLACK)
#  lcd.arc((M5Avatar_eye_x + M5Avatar_mouth_x) // 2, (M5Avatar_wh - M5Avatar_fh) - 5, 25, 25, 270, 360, lcd.BLACK, lcd.BLACK)
  labelText.setText("")
  _lip_close()
  fSpeaking = False

def _exclamation_color(color):
  lcd.rect(
    M5Avatar_exclamation_x,
    M5Avatar_exclamation_y,
    M5Avatar_exclamation_height,
    M5Avatar_exclamation_height,
    color,
    color
  )
  lcd.rect(M5Avatar_exclamation_x + M5Avatar_exclamation_height + M5Avatar_exclamation_space, M5Avatar_exclamation_y, M5Avatar_exclamation_width, M5Avatar_exclamation_height, color, color)

def _pale_color(color):
   lcd.rect(95, 220, 40, 5, color, color)
   lcd.rect(90, 210, 45, 5, color, color)
   lcd.rect(85, 200, 50, 5, color, color)
   lcd.rect(80, 190, 55, 5, color, color)
#  lcd.rect(40, 100, 5, 40, color, color)
#  lcd.rect(60, 103, 5, 35, color, color)
#  lcd.rect(80, 106, 5, 30, color, color)
#  lcd.rect(100, 109, 5, 25, color, color)

def _angry_color(color):
  lcd.rect( M5Avatar_angry_x - M5Avatar_angry_gap//2 - M5Avatar_angry_length, M5Avatar_angry_y - M5Avatar_angry_gap//2 - M5Avatar_angry_width,
    M5Avatar_angry_length, M5Avatar_angry_width, color)
  lcd.rect( M5Avatar_angry_x - M5Avatar_angry_gap//2 - M5Avatar_angry_width//2, M5Avatar_angry_y - M5Avatar_angry_gap//2 - M5Avatar_angry_length,
    M5Avatar_angry_width, M5Avatar_angry_length, color)
  lcd.rect( M5Avatar_angry_x + M5Avatar_angry_gap//2, M5Avatar_angry_y - M5Avatar_angry_gap//2 - M5Avatar_angry_width,
    M5Avatar_angry_length, M5Avatar_angry_width, color)
  lcd.rect( M5Avatar_angry_x + M5Avatar_angry_gap//2, M5Avatar_angry_y - M5Avatar_angry_gap//2 - M5Avatar_angry_width - M5Avatar_angry_length,
    M5Avatar_angry_width, M5Avatar_angry_length, color)
  lcd.rect( M5Avatar_angry_x - M5Avatar_angry_gap//2 - M5Avatar_angry_length, M5Avatar_angry_y + M5Avatar_angry_gap//2 - M5Avatar_angry_width,
    M5Avatar_angry_length, M5Avatar_angry_width, color)
  lcd.rect( M5Avatar_angry_x - M5Avatar_angry_gap//2 - M5Avatar_angry_width//2, M5Avatar_angry_y + M5Avatar_angry_gap//2,
    M5Avatar_angry_width, M5Avatar_angry_length, color)
  lcd.rect( M5Avatar_angry_x + M5Avatar_angry_gap//2, M5Avatar_angry_y + M5Avatar_angry_gap//2 - M5Avatar_angry_width,
    M5Avatar_angry_length, M5Avatar_angry_width, color)
  lcd.rect( M5Avatar_angry_x + M5Avatar_angry_gap//2, M5Avatar_angry_y + M5Avatar_angry_gap//2 - M5Avatar_angry_width,
    M5Avatar_angry_width, M5Avatar_angry_length, color)

_lip_close()
timerSch.setTimer('timerAvatar', timerAvatar_period, 0x00)
timerSch.run('timerAvatar', timerAvatar_period, 0x00)
tm_blink_open = random.randint(2, 6) * 1000

text =
_speak(text)
if  == 1:
  _exclamation_color(lcd.RED)
else:
  _exclamation_color(lcd.BLACK)

if  == 1:
  _pale_color(lcd.BLUE)
else:
  _pale_color(lcd.BLACK)

if  == 1:
  _angry_color(lcd.RED)
else:
  _angry_color(lcd.BLACK)
  
