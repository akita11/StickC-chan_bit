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
uint8_t fSpeaking = 0;

uint8_t face_fw = 16, face_fh = 8;
uint16_t face_ww = M5.Display.width(), face_wh = M5.Display.height();
uint8_t face_eye_x = 90, face_eye_y = 80, face_eye_r = 10;
uint8_t face_eye_close_width  = 6, face_eye_close_height = 40;
uint8_t face_mouth_x = 40, face_mouth_y = 120, face_mouth_width = 7, face_mouth_height = 50, face_mouth_open_width = 30;
uint8_t face_exclamation_x = 60, face_exclamation_y = 220, face_exclamation_width  = 30, face_exclamation_height = 10, face_exclamation_space  = 8;
uint8_t face_angry_x = 110, face_angry_y = 210, face_angry_width = 2, face_angry_length = 10, face_angry_gap = 10;

#define TIMER_INTERVAL 100 // [ms]

void _breath()
{
	breath_T = (breath_T + 1) % breath_cycle;
	dev = (int8_t)(breath_dev * sin(breath_T * 2.0 * 3.14 / breath_cycle));
}

void _eye_close(){
	M5.Display.drawCircle(face_eye_x + dev, face_eye_y, face_eye_r+10, TFT_BLACK);
	M5.Display.drawCircle(face_eye_x + dev, face_wh - face_eye_y, face_eye_r+10, TFT_BLACK);
	M5.Display.drawRect(
        face_eye_x - face_eye_close_width / 2 + dev,
        face_eye_y - face_eye_close_height / 2,
        face_eye_close_width, face_eye_close_height, TFT_WHITE);
	M5.Display.drawRect(
        face_eye_x - face_eye_close_width / 2 + dev,
        face_wh - face_eye_y - face_eye_close_height / 2,
        face_eye_close_width, face_eye_close_height,
        TFT_WHITE);
}

void _eye_open(){
	M5.Display.drawRect(
        face_eye_x - face_eye_close_width / 2 + dev - 10,
        face_eye_y - face_eye_close_height / 2,
        face_eye_close_width + 20, face_eye_close_height, TFT_BLACK);
	M5.Display.drawRect(
        face_eye_x - face_eye_close_width / 2 + dev - 10,
        face_wh - face_eye_y - face_eye_close_height / 2,
        face_eye_close_width + 20, face_eye_close_height,
        TFT_BLACK);
	M5.Display.drawCircle(face_eye_x + dev, face_eye_y, face_eye_r, TFT_WHITE);
	M5.Display.drawCircle(face_eye_x + dev, face_wh - face_eye_y, face_eye_r, TFT_WHITE);
}

void _blink()
{
	tm_blink = tm_blink + TIMER_INTERVAL;
	if (st_blink == 0){
		_eye_close();
		if (tm_blink >= tm_blink_open){
			st_blink = 1;
			tm_blink = 0;
		}
	}
	else{
  	_eye_open();
  	if (tm_blink >= tm_blink_open){
			tm_blink_open = random(2, 6) * 1000;
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
    face_mouth_y - face_mouth_height / 2,
    face_mouth_width + 20,
    face_mouth_height,
		TFT_BLACK);
	M5.Display.drawRect(
    face_mouth_x - face_mouth_width / 2,
    face_mouth_y - face_mouth_height / 2,
    face_mouth_width,
    face_mouth_height,
    TFT_BLACK);
}

void _lip_open(){
  M5.Display.drawRect(
    face_mouth_x - face_mouth_open_width / 2,
    face_mouth_y - face_mouth_height / 2,
    face_mouth_open_width,
    face_mouth_height,
    TFT_BLACK);
  M5.Display.drawRect(
    face_mouth_x - face_mouth_open_width / 2,
    face_mouth_y - face_mouth_height / 2,
    face_mouth_open_width,
    face_mouth_height,
    TFT_WHITE);
	st_mouth = 1;
}

void _lip_close(){
  M5.Display.drawRect(
    face_mouth_x - face_mouth_open_width / 2,
    face_mouth_y - face_mouth_height / 2,
    face_mouth_open_width,
    face_mouth_height,
    TFT_BLACK);
  M5.Display.drawRect(
    face_mouth_x - face_mouth_width / 2,
    face_mouth_y - face_mouth_height / 2,
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
	fSpeaking = 1;
	tm_speak = 0;
	_lipsync();
	while(tm_speak < tm_speak_period);
	tm_speak = 0;

	// ToDo

  _lip_close();
  fSpeaking = 0;
}

void _exclamation_color(uint32_t color){
	// ToDo
}

void _pale_color(uint32_t color){
	// ToDo
}

void _angry_color(uint32_t color){
	// ToDo
}


void IRAM_ATTR onTimer() {
  _breath();
  _blink();
  tm_speak = tm_speak + TIMER_INTERVAL;
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  Serial2.begin(115200, SERIAL_8N1, 26, 0); // RX=26, TX=0
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, TIMER_INTERVAL * 1000, true); // every 100ms
  timerAlarmEnable(timer);
	_lip_close();
}
void loop() {
}
