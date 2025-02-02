#include <iomanip>
#include <iostream>
#include <vector>

namespace StarrySky {
std::vector<u_int8_t> API_DATA = {0xa5, 0x00, 0x01, 0x0c, 0x05, 0x00, 0x00,
                                  0x00, 0x00, 0x64, 0x00, 0x12, 0x05, 0xff,
                                  0x04, 0xff, 0x01, 0x05, 0x00, 0xaa};

enum Effects {
  seven_color_fade = 0x00,
  rgb_fade = 0x01,
  seven_color_breathe = 0x02,
  rgb_breath = 0x03,
  red_green_fae = 0x04,
  red_blue_fade = 0x05,
  green_blue_fade = 0x06,
  seven_color_jump = 0x07,
  rgb_jump = 0x08,
  seven_color_flash = 0x09,
  rgb_flash = 0x0a,
  red_flash = 0x0b,
  green_flash = 0x0c,
  blue_flash = 0x0d,
  yellow_flash = 0x0e,
  cyan_flash = 0x10,
  white_flash = 0x11,
  yellow_purple_flash = 0x12,
  purple_cyan_flash = 0x13,
};

enum FunctionType {
  EFFECT = 0,
  LIGHT = 1,
};

enum SpeedType {
  STOP = 0,
  SLOW = 1,
  NORMAL = 2,
  FAST = 3,
  ULTRA_FAST = 4
};

int function = FunctionType::LIGHT;
int speed = SpeedType::STOP;
float red = 0;
float green = 0;
float blue = 0;
float white = 0xFF;
float brightness = 1;

static constexpr int onOffElement = 1;
static constexpr int functionTypeElement = 2;
static constexpr int effectElement = 3;
// static constexpr int speedElement = 4;
static constexpr int redElement = 5;
static constexpr int greenElement = 6;
static constexpr int blueElement = 7;
static constexpr int whiteElement = 8;
// static constexpr int brightnessElement = 9;
static constexpr int motorOnOffElement = 13;
static constexpr int motorSpeedElement = 14;

void updateRed(float value) {
  red = std::min(value, static_cast<float>(1)) * 0xFF;
}
void updateGreen(float value) {
  green = std::min(value, static_cast<float>(1)) * 0xFF;
}
void updateBlue(float value) {
  blue = std::min(value, static_cast<float>(1)) * 0xFF;
}
void updateWhite(float value) {
  white = std::min(value, static_cast<float>(1)) * 0xFF;
}
void updateBrightness(float value) {
  brightness = std::min(value, static_cast<float>(1)) * 0xFF;
}

std::vector<u_int8_t> turnOn(int function = FunctionType::LIGHT,
                             int effect = -1, float red = 0, float green = 0,
                             float blue = 0, float white = 1, int motorOnOff = 0, int motorSpeed = -1) {
  API_DATA[onOffElement] = 0xFF;

  if (function == FunctionType::LIGHT) {
    API_DATA[functionTypeElement] = 0x01;
    if (white > 0) {
      API_DATA[redElement] = 0x00;
      API_DATA[greenElement] = 0x00;
      API_DATA[blueElement] = 0x00;
      API_DATA[whiteElement] = 0xFF;
    } else {
      API_DATA[redElement] = red * 0xFF;
      API_DATA[greenElement] = green * 0xFF;
      API_DATA[blueElement] = blue * 0xFF;
      API_DATA[whiteElement] = 0x00;
    }
  } else {
    API_DATA[functionTypeElement] = 0x00;
    if (effect >= 0) {
      API_DATA[effectElement] = static_cast<u_int8_t>(effect);
    } else {
      API_DATA[onOffElement] = 0x00;
      return API_DATA;
    }
  }

  if (motorOnOff == 0) {
    API_DATA[motorOnOffElement] = 0x00;
    API_DATA[motorSpeedElement] = 0x00;
  } else {
    API_DATA[motorOnOffElement] = 0xFF;
    API_DATA[motorSpeedElement] = motorSpeed >= 0 ? motorSpeed : 0;
  }
  return API_DATA;
}

std::vector<u_int8_t> turnOff() {
  API_DATA[onOffElement] = 0x00;
  return API_DATA;
}

std::vector<u_int8_t> getStarrySkyArray() {
  std::vector<u_int8_t> starrySkyArray = API_DATA;

  if (white != 0x00) {
    starrySkyArray[redElement] = red;
    starrySkyArray[greenElement] = green;
    starrySkyArray[blueElement] = blue;
    starrySkyArray[whiteElement] = 0xFF;
  } else {
    starrySkyArray[onOffElement] = 0xFF;
    starrySkyArray[redElement] = red;
    starrySkyArray[greenElement] = green;
    starrySkyArray[blueElement] = blue;
    starrySkyArray[whiteElement] = 0x00;
  }

  return starrySkyArray;
}

void printHexCommand(const std::string text, std::vector<u_int8_t> command) {
  std::cout << text << std::endl;
  for (auto byte : command) {
    std::cout << std::hex << std::setfill('0') << std::setw(2)
              << static_cast<int>(byte) << " ";
  }
  std::cout << std::endl;
}
} // namespace StarrySky
