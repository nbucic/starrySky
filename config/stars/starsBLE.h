#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace StarrySky {

enum class Effects : uint8_t {
  seven_color_fade = 0x00,
  rgb_fade = 0x01,
  seven_color_breathe = 0x02,
  rgb_breath = 0x03,
  red_green_fade = 0x04,
  red_blue_fade = 0x05,
  green_blue_fade = 0x06,
  seven_color_jump = 0x07,
  rgb_jump = 0x08,
  seven_color_flash = 0x09,
  rgb_flash = 0x0A,
  red_flash = 0x0B,
  green_flash = 0x0C,
  blue_flash = 0x0D,
  yellow_flash = 0x0E,
  cyan_flash = 0x10,
  white_flash = 0x11,
  yellow_purple_flash = 0x12,
  purple_cyan_flash = 0x13,
};

enum class FunctionType : uint8_t {
  EFFECT = 0x00,
  LIGHT = 0x01,
};

enum class SpeedType : uint8_t {
  STOP = 0,
  SLOW = 1,
  NORMAL = 2,
  FAST = 3,
  ULTRA_FAST = 4,
};

namespace Detail {
static constexpr std::array<uint8_t, 20> BASE_COMMAND = {
    0xA5, 0x00, 0x01, 0x0C, 0x05, 0x00, 0x00,
    0x00, 0x00, 0x64, 0x00, 0x12, 0x05, 0xFF,
    0x04, 0xFF, 0x01, 0x05, 0x00, 0xAA};

static constexpr std::size_t ON_OFF_ELEMENT = 1;
static constexpr std::size_t FUNCTION_TYPE_ELEMENT = 2;
static constexpr std::size_t EFFECT_ELEMENT = 3;
static constexpr std::size_t RED_ELEMENT = 5;
static constexpr std::size_t GREEN_ELEMENT = 6;
static constexpr std::size_t BLUE_ELEMENT = 7;
static constexpr std::size_t WHITE_ELEMENT = 8;
static constexpr std::size_t MOTOR_ON_OFF_ELEMENT = 13;
static constexpr std::size_t MOTOR_SPEED_ELEMENT = 14;

inline uint8_t clamp_to_byte(float value) {
  const float clamped = std::clamp(value, 0.0f, 1.0f);
  return static_cast<uint8_t>(clamped * 255.0f);
}

inline std::vector<uint8_t> make_base_command() {
  return std::vector<uint8_t>(BASE_COMMAND.begin(), BASE_COMMAND.end());
}

inline void apply_motor(std::vector<uint8_t> &command, bool motor_on,
                        int motor_speed) {
  if (!motor_on) {
    command[MOTOR_ON_OFF_ELEMENT] = 0x00;
    command[MOTOR_SPEED_ELEMENT] = 0x00;
    return;
  }

  command[MOTOR_ON_OFF_ELEMENT] = 0xFF;
  command[MOTOR_SPEED_ELEMENT] =
      static_cast<uint8_t>(std::clamp(motor_speed, 0, 255));
}

inline void apply_light(std::vector<uint8_t> &command, float red, float green,
                        float blue, float white) {
  command[FUNCTION_TYPE_ELEMENT] = static_cast<uint8_t>(FunctionType::LIGHT);

  if (white > 0.0f) {
    command[RED_ELEMENT] = 0x00;
    command[GREEN_ELEMENT] = 0x00;
    command[BLUE_ELEMENT] = 0x00;
    command[WHITE_ELEMENT] = 0xFF;
    return;
  }

  command[RED_ELEMENT] = clamp_to_byte(red);
  command[GREEN_ELEMENT] = clamp_to_byte(green);
  command[BLUE_ELEMENT] = clamp_to_byte(blue);
  command[WHITE_ELEMENT] = 0x00;
}

inline bool apply_effect(std::vector<uint8_t> &command, int effect) {
  if (effect < 0 || effect > 255) {
    command[ON_OFF_ELEMENT] = 0x00;
    return false;
  }

  command[FUNCTION_TYPE_ELEMENT] = static_cast<uint8_t>(FunctionType::EFFECT);
  command[EFFECT_ELEMENT] = static_cast<uint8_t>(effect);
  return true;
}
}  // namespace Detail

inline std::vector<uint8_t> turnOn(
    FunctionType function = FunctionType::LIGHT,
    int effect = -1,
    float red = 0.0f,
    float green = 0.0f,
    float blue = 0.0f,
    float white = 1.0f,
    int motorOnOff = 0,
    int motorSpeed = -1) {
  auto command = Detail::make_base_command();
  command[Detail::ON_OFF_ELEMENT] = 0xFF;

  if (function == FunctionType::LIGHT) {
    Detail::apply_light(command, red, green, blue, white);
  } else {
    if (!Detail::apply_effect(command, effect)) {
      return command;
    }
  }

  Detail::apply_motor(command, motorOnOff != 0, std::max(motorSpeed, 0));
  return command;
}

inline std::vector<uint8_t> turnOnLight(float red = 0.0f, float green = 0.0f,
                                        float blue = 0.0f, float white = 1.0f,
                                        int motorOnOff = 0,
                                        int motorSpeed = -1) {
  return turnOn(FunctionType::LIGHT, -1, red, green, blue, white, motorOnOff,
                motorSpeed);
}

inline std::vector<uint8_t> turnOnEffect(Effects effect, int motorOnOff = 0,
                                         int motorSpeed = -1) {
  return turnOn(FunctionType::EFFECT, static_cast<int>(effect), 0.0f, 0.0f,
                0.0f, 0.0f, motorOnOff, motorSpeed);
}

inline std::vector<uint8_t> turnOff() {
  auto command = Detail::make_base_command();
  command[Detail::ON_OFF_ELEMENT] = 0x00;
  return command;
}

inline void printHexCommand(const std::string &text,
                            const std::vector<uint8_t> &command) {
  std::cout << text << '\n';
  for (const auto byte : command) {
    std::cout << std::hex << std::setfill('0') << std::setw(2)
              << static_cast<int>(byte) << ' ';
  }
  std::cout << std::dec << '\n';
}

}  // namespace StarrySky