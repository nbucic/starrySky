#include "../config/stars/starsBLE.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>

void test_turn_on_light_white() {
    std::cout << "Testing turnOn(LIGHT, white=1.0)..." << std::endl;
    auto result = StarrySky::turnOn(StarrySky::FunctionType::LIGHT, -1, 0.0f, 0.0f, 0.0f, 1.0f);
    
    assert(result[StarrySky::Detail::ON_OFF_ELEMENT] == 0xFF);
    assert(result[StarrySky::Detail::FUNCTION_TYPE_ELEMENT] == static_cast<uint8_t>(StarrySky::FunctionType::LIGHT));
    assert(result[StarrySky::Detail::RED_ELEMENT] == 0x00);
    assert(result[StarrySky::Detail::GREEN_ELEMENT] == 0x00);
    assert(result[StarrySky::Detail::BLUE_ELEMENT] == 0x00);
    assert(result[StarrySky::Detail::WHITE_ELEMENT] == 0xFF);
    std::cout << "turnOn(LIGHT, white=1.0) tests passed!" << std::endl;
}

void test_turn_on_light_rgb() {
    std::cout << "Testing turnOn(LIGHT, red=0.5, green=0.2, blue=0.8, white=0)..." << std::endl;
    auto result = StarrySky::turnOn(StarrySky::FunctionType::LIGHT, -1, 0.5f, 0.2f, 0.8f, 0.0f);
    
    assert(result[StarrySky::Detail::ON_OFF_ELEMENT] == 0xFF);
    assert(result[StarrySky::Detail::FUNCTION_TYPE_ELEMENT] == static_cast<uint8_t>(StarrySky::FunctionType::LIGHT));
    assert(result[StarrySky::Detail::RED_ELEMENT] == static_cast<uint8_t>(0.5f * 255.0f));
    assert(result[StarrySky::Detail::GREEN_ELEMENT] == static_cast<uint8_t>(0.2f * 255.0f));
    assert(result[StarrySky::Detail::BLUE_ELEMENT] == static_cast<uint8_t>(0.8f * 255.0f));
    assert(result[StarrySky::Detail::WHITE_ELEMENT] == 0x00);
    std::cout << "turnOn(LIGHT, rgb) tests passed!" << std::endl;
}

void test_turn_on_effect() {
    std::cout << "Testing turnOnEffect(seven_color_fade)..." << std::endl;
    auto result = StarrySky::turnOnEffect(StarrySky::Effects::seven_color_fade);
    
    assert(result[StarrySky::Detail::ON_OFF_ELEMENT] == 0xFF);
    assert(result[StarrySky::Detail::FUNCTION_TYPE_ELEMENT] == static_cast<uint8_t>(StarrySky::FunctionType::EFFECT));
    assert(result[StarrySky::Detail::EFFECT_ELEMENT] == static_cast<uint8_t>(StarrySky::Effects::seven_color_fade));
    std::cout << "turnOnEffect() tests passed!" << std::endl;
}

void test_turn_on_effect_invalid() {
    std::cout << "Testing turnOn(EFFECT, effect=-1)..." << std::endl;
    auto result = StarrySky::turnOn(StarrySky::FunctionType::EFFECT, -1);
    
    assert(result[StarrySky::Detail::ON_OFF_ELEMENT] == 0x00); // Should be off if no effect provided
    std::cout << "turnOn(EFFECT, invalid) tests passed!" << std::endl;
}

void test_turn_off() {
    std::cout << "Testing turnOff()..." << std::endl;
    auto result = StarrySky::turnOff();
    
    assert(result[StarrySky::Detail::ON_OFF_ELEMENT] == 0x00);
    std::cout << "turnOff() tests passed!" << std::endl;
}

void test_motor_control() {
    std::cout << "Testing motor control in turnOn()..." << std::endl;
    // Motor off
    auto resultOff = StarrySky::turnOn(StarrySky::FunctionType::LIGHT, -1, 0, 0, 0, 1, 0, 128);
    assert(resultOff[StarrySky::Detail::MOTOR_ON_OFF_ELEMENT] == 0x00);
    assert(resultOff[StarrySky::Detail::MOTOR_SPEED_ELEMENT] == 0x00);

    // Motor on
    auto resultOn = StarrySky::turnOn(StarrySky::FunctionType::LIGHT, -1, 0, 0, 0, 1, 1, 128);
    assert(resultOn[StarrySky::Detail::MOTOR_ON_OFF_ELEMENT] == 0xFF);
    assert(resultOn[StarrySky::Detail::MOTOR_SPEED_ELEMENT] == 128);

    // Motor on, speed clamping
    auto resultClamped = StarrySky::turnOn(StarrySky::FunctionType::LIGHT, -1, 0, 0, 0, 1, 1, 300);
    assert(resultClamped[StarrySky::Detail::MOTOR_SPEED_ELEMENT] == 255);

    std::cout << "Motor control tests passed!" << std::endl;
}

void test_turn_on_light_wrapper() {
    std::cout << "Testing turnOnLight()..." << std::endl;
    auto result = StarrySky::turnOnLight(0.1f, 0.2f, 0.3f, 0.0f, 1, 100);
    assert(result[StarrySky::Detail::ON_OFF_ELEMENT] == 0xFF);
    assert(result[StarrySky::Detail::RED_ELEMENT] == static_cast<uint8_t>(0.1f * 255.0f));
    assert(result[StarrySky::Detail::MOTOR_SPEED_ELEMENT] == 100);
    std::cout << "turnOnLight() tests passed!" << std::endl;
}

int main() {
    try {
        test_turn_on_light_white();
        test_turn_on_light_rgb();
        test_turn_on_effect();
        test_turn_on_effect_invalid();
        test_turn_off();
        test_motor_control();
        test_turn_on_light_wrapper();
        
        std::cout << "\nAll starrySky-new tests passed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
