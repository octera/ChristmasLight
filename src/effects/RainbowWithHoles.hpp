#pragma once

#include "ExtendedBaseEffect.hpp"

// A rainbow that moves along the leds
class RainbowEffectWithHoles final : public ExtendedBaseEffect
{
public:
  uint8_t deltaHue = 2;  // hue difference between two leds
  int8_t rate = 1;       // rate of change of hue on each loop
  uint8_t holeSize = 4;
  uint8_t spaceBeetweenHole = 60;
  uint8_t delayBetweenMove = 50;

  RainbowEffectWithHoles(const char* name) : ExtendedBaseEffect(name) { };

  void deserialize(JsonObject& data) override {
    // deltaHue
    if (data.containsKey("delta_hue")) {
      LEDEFFECT_DEBUG_PRINT(F("RainbowEffect: deltaHue to "));
      LEDEFFECT_DEBUG_PRINTLN(data["delta_hue"].as<uint8_t>());
      deltaHue = data["delta_hue"].as<uint8_t>();
    }

    // rate
    if (data.containsKey("rate")) {
      LEDEFFECT_DEBUG_PRINT(F("RainbowEffect: rate to "));
      LEDEFFECT_DEBUG_PRINTLN(data["rate"].as<int8_t>());
      rate = data["rate"].as<int8_t>();
    }
  }

  void serialize(JsonObject& data) const override {
    data["delta_hue"] = deltaHue;
    data["rate"] = rate;
  }

  void loop() override {
    _hue += rate;
    fill_rainbow(_controller->leds(), _controller->size(), _hue, deltaHue);

    if (millis() > nextTimeToTick) {
        nextTimeToTick = millis() + delayBetweenMove;
        holeShift++;
        if (holeShift==spaceBeetweenHole) {
            holeShift=0;
        }
    }

    //fill hole
    for (uint8_t i = holeShift; i<_controller->size(); i=i+spaceBeetweenHole) {
        for (uint8_t j = 0; j<holeSize;j++) {
            if (j>=_controller->size()) {
                return; //Here we have reached end of leds for this loop
            }
            _controller->leds()[i+j] = CRGB::Black;
        }
    }
    
  }

protected:
  uint8_t _hue = 0;
  unsigned long nextTimeToTick = 0; 
  uint8_t holeShift = 0;
};
