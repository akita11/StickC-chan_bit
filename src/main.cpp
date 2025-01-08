#include <Arduino.h>
#include <M5Unified.h>

// 115200bps, term=CR+LF
// Txxxx : 吹き出しに”xxxx”と表示し、それにあわせて口が動く
// En : 表情を設定。n=0〜5 (0=Angry, 1=Sleepy, 2=Happy, 3=Sat, 4=Doubt, 5=Neutral)

hw_timer_t * timer = NULL;
uint16_t tm_blink = 0, tm_mouth = 0, tm_speak = 0, tm_blink_open = 500, tm_mouth_period = 500, tm_speak_period = 500;
uint8_t breath_T = 0, breath_cycle = 30, breath_dev = 4;
int8_t dev = 0;
uint8_t st_blink = 0, st_mouth = 0;

uint8_t face_fw = 16, face_fh = 8;
uint16_t face_ww, face_wh;
uint8_t face_eye_x = 80, face_eye_y = 90, face_eye_r = 10;
uint8_t face_eye_close_width  = 40, face_eye_close_height = 6;
uint8_t face_mouth_x = 120, face_mouth_y = 40, face_mouth_width = 50, face_mouth_height = 7, face_mouth_open_height = 30;
uint8_t face_exclamation_x = 220, face_exclamation_y = 60, face_exclamation_width = 10, face_exclamation_height = 30, face_exclamation_space  = 8;
uint8_t face_angry_x = 210, face_angry_y = 110, face_angry_length = 10, face_angry_width = 2, face_angry_gap = 10;

#define TIMER_INTERVAL 100 // [ms]

void _breath()
{
	breath_T = (breath_T + 1) % breath_cycle;
	dev = (int8_t)(breath_dev * sin(breath_T * 2.0 * 3.14 / breath_cycle));
}

void _eye_close(){
	M5.Display.fillCircle(
		face_eye_x, 
		face_wh - (face_eye_y + dev), 
		face_eye_r+10, 
		TFT_BLACK);
	M5.Display.fillCircle(
		face_ww - face_eye_x, 
		face_wh - (face_eye_y + dev), 
		face_eye_r+10, TFT_BLACK);
	M5.Display.fillRect(
		face_eye_x - face_eye_close_width / 2,
		face_wh - (face_eye_y + dev) - face_eye_close_height / 2 ,
		face_eye_close_width, 
		face_eye_close_height, 
		TFT_WHITE);
	M5.Display.fillRect(
		face_ww - face_eye_x - face_eye_close_width / 2,
		face_wh - (face_eye_y + dev) - face_eye_close_height / 2,
		face_eye_close_width, 
		face_eye_close_height, 
		TFT_WHITE);
}

void _eye_open(){
	M5.Display.fillRect(
		face_eye_x - face_eye_close_width / 2,
		face_wh - (face_eye_y + dev) - face_eye_close_height / 2 - 10,
		face_eye_close_width,
		face_eye_close_height + 20, 
		TFT_BLACK);
	M5.Display.fillRect(
		face_ww - face_eye_x - face_eye_close_width / 2,
		face_wh - (face_eye_y + dev) - face_eye_close_height / 2 - 10,
		face_eye_close_width, 
		face_eye_close_height + 20, 
		TFT_BLACK);
	M5.Display.fillCircle(
		face_eye_x, 
		face_wh - (face_eye_y + dev), 
		face_eye_r, 
		TFT_WHITE);
	M5.Display.fillCircle(
		face_ww - face_eye_x, 
		face_wh - (face_eye_y + dev), 
		face_eye_r, 
		TFT_WHITE);
}

void _blink()
{
	tm_blink = tm_blink + TIMER_INTERVAL;
	if (st_blink == 0){
		_eye_close();
		if (tm_blink >= tm_blink_open){
			tm_blink_open = random(4, 10) * 500;
			st_blink = 1;
			tm_blink = 0;
		}
	}
	else{
  	_eye_open();
  	if (tm_blink >= tm_blink_open){
      tm_blink_open = 500;
			st_blink = 0;
			tm_blink = 0;
		}
	}
}

void _mouth(){
	tm_mouth = tm_mouth + TIMER_INTERVAL;
  if (tm_mouth >= tm_mouth_period){
		tm_mouth = 0;
	}
  M5.Display.drawRect(
		face_mouth_x - face_mouth_width / 2,
    face_wh - face_mouth_y - face_mouth_height / 2,
    face_mouth_width,
    face_mouth_height,
		TFT_BLACK);
	M5.Display.drawRect(
    face_mouth_x - face_mouth_width / 2,
    face_wh - face_mouth_y - face_mouth_height / 2,
    face_mouth_width,
    face_mouth_height,
    TFT_WHITE);
}

void _lip_open(){
  M5.Display.fillRect(
    face_mouth_x - face_mouth_width / 2,
    face_wh - face_mouth_y - face_mouth_height / 2,
    face_mouth_width,
    face_mouth_height,
    TFT_BLACK);
  M5.Display.fillRect(
    face_mouth_x - face_mouth_width / 2,
    face_wh - face_mouth_y - face_mouth_open_height / 2,
    face_mouth_width,
    face_mouth_open_height,
    TFT_WHITE);
	st_mouth = 1;
}

void _lip_close(){
  M5.Display.fillRect(
    face_mouth_x - face_mouth_width / 2,
    face_wh - face_mouth_y - face_mouth_height / 2,
    face_mouth_width,
    face_mouth_open_height,
    TFT_BLACK);
  M5.Display.fillRect(
    face_mouth_x - face_mouth_width / 2,
    face_wh - face_mouth_y - face_mouth_height / 2,
    face_mouth_width,
    face_mouth_height,
    TFT_WHITE);
	st_mouth = 0;
}

void _lipsync(){
	if (st_mouth == 0) _lip_open();
	else _lip_close();
}

void _speak(String str){
  M5.Display.setCursor(20, face_wh - 24); M5.Display.print(str);
	_lipsync();
	tm_speak = 0;
  delay(tm_speak_period);
  while(str.length() > 0){
		str = str.substring(1, str.length());
		M5.Display.clear();
		M5.Display.setCursor(20, face_wh - 24); M5.Display.print(str);
		_lipsync();
		delay(tm_speak_period);
  }
	M5.Display.clearDisplay();
  _lip_close();
}

void _exclamation_color(uint16_t color){
  M5.Display.fillRect(
    face_exclamation_x,
    face_wh - face_exclamation_y,
    face_exclamation_width,
    face_exclamation_width,
    color
  );
  M5.Display.fillRect(
		face_exclamation_x, 
		face_wh - (face_exclamation_y + face_exclamation_height + face_exclamation_space), 
		face_exclamation_width, 
		face_exclamation_height, 
		color);
}

void _pale_color(uint16_t color){
   M5.Display.fillRect(220, face_wh - 95, 5, 40, color);
   M5.Display.fillRect(210, face_wh - 90, 5, 45, color);
   M5.Display.fillRect(200, face_wh - 85, 5, 50, color);
   M5.Display.fillRect(190, face_wh - 80, 5, 55, color);
}

void _angry_color(uint16_t color){
	M5.Display.fillRect(
		face_angry_x - face_angry_gap/2 - face_angry_length, 
		face_wh - (face_angry_y - face_angry_gap/2) - face_angry_width/2,
		face_angry_length, 
		face_angry_width, 
		color);
  M5.Display.fillRect(
		face_angry_x - face_angry_gap/2 - face_angry_width/2, 
		face_wh - (face_angry_y - face_angry_gap/2),
  	face_angry_width, 
		face_angry_length, 
		color);
  M5.Display.fillRect(
		face_angry_x + face_angry_gap/2, 
		face_wh - (face_angry_y - face_angry_gap/2) - face_angry_width/2,
    face_angry_length, 
		face_angry_width, 
		color);
  M5.Display.fillRect(
		face_angry_x + face_angry_gap/2 - face_angry_width/2, 
		face_wh - (face_angry_y - face_angry_gap/2),
    face_angry_width, 
		face_angry_length, 
		color);
  M5.Display.fillRect( 
		face_angry_x - face_angry_gap/2 - face_angry_length, 
		face_wh - (face_angry_y + face_angry_gap/2) - face_angry_width,
    face_angry_length, 
		face_angry_width, 
		color);
  M5.Display.fillRect( 
		face_angry_x - face_angry_gap/2 - face_angry_width/2, 
		face_wh - (face_angry_y + face_angry_gap/2) - face_angry_length,
    face_angry_width, 
		face_angry_length, 
		color);
  M5.Display.fillRect( 
		face_angry_x + face_angry_gap/2, 
		face_wh - (face_angry_y + face_angry_gap/2) - face_angry_width,
    face_angry_length, 
		face_angry_width, 
		color);
  M5.Display.fillRect( 
		face_angry_x + face_angry_gap/2 - face_angry_width/2, 
		face_wh - (face_angry_y + face_angry_gap/2) - face_angry_length,
    face_angry_width, 
		face_angry_length, 
		color);
}

void IRAM_ATTR onTimer() {
  _breath();
  _blink();
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  // https://lang-ship.com/blog/work/m5stack-m5stickc-plus2-1/
  M5.Display.setRotation(1); 
  Serial2.begin(115200, SERIAL_8N1, 26, 0); // RX=26, TX=0
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, TIMER_INTERVAL * 1000, true); // every 100ms
  timerAlarmEnable(timer);
	face_ww = M5.Display.width(); face_wh = M5.Display.height();
	_lip_close();
	M5.Display.setFont(&lgfxJapanGothic_24);
}

char buf[1024];
uint16_t pBuf = 0;

// Txxxx : 吹き出しに”xxxx”と表示し、それにあわせて口が動く
// En : 表情を設定。n=0〜5 (0=Angry, 1=Sleepy, 2=Happy, 3=Sat, 4=Doubt, 5=Neutral)
void loop() {
	char c;
	while(Serial2.available()){
		c = Serial2.read();
		if (c == '\n' || c == '\r'){
			buf[pBuf] = '\0';
			pBuf = 0;
			if (buf[0] == 'T') _speak(buf+1);
			else if (buf[0] == 'E'){
				if (buf[1] == '0') _angry_color(TFT_RED);
				else if (buf[1] == '1') _angry_color(TFT_BLACK);
				else if (buf[1] == '2') _pale_color(TFT_BLUE);
				else if (buf[1] == '3') _pale_color(TFT_BLACK);
				else if (buf[1] == '4') _exclamation_color(TFT_YELLOW);
				else if (buf[1] == '5') _exclamation_color(TFT_BLACK);
			}
		}
		else buf[pBuf++] = c;
		if (pBuf >= 1024) pBuf = 0;
	}
  _speak("ほげほげ");
	delay(1000);
  _angry_color(TFT_RED);
  delay(1000);
  _angry_color(TFT_BLACK);
  delay(1000);
  _pale_color(TFT_BLUE);
  delay(1000);
  _pale_color(TFT_BLACK);
  delay(1000);
  _exclamation_color(TFT_YELLOW);
  delay(1000);
  _exclamation_color(TFT_BLACK);
  delay(1000);
}
