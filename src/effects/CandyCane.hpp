#pragma once

#include "ExtendedBaseEffect.hpp"

// A rainbow that moves along the leds
class CandyCane final : public ExtendedBaseEffect
{
public:
  uint8_t colorSize = 35;
  int delayBetweenMove = 150;

  CandyCane(const char *name) : ExtendedBaseEffect(name){};

  void deserialize(JsonObject &data) override
  {
    // deltaHue
    if (data.containsKey("colorSize"))
    {
      LEDEFFECT_DEBUG_PRINT(F("CandyCane: deltaHue to "));
      LEDEFFECT_DEBUG_PRINTLN(data["colorSize"].as<uint8_t>());
      colorSize = data["colorSize"].as<uint8_t>();
    }

    // rate
    if (data.containsKey("delayBetweenMove"))
    {
      LEDEFFECT_DEBUG_PRINT(F("CandyCane: delayBetweenMove to "));
      LEDEFFECT_DEBUG_PRINTLN(data["delayBetweenMove"].as<int8_t>());
      delayBetweenMove = data["delayBetweenMove"].as<int8_t>();
    }
  }

  void serialize(JsonObject &data) const override
  {
    data["colorSize"] = colorSize;
    data["delayBetweenMove"] = delayBetweenMove;
  }

  void loop() override
  {
    if (millis() < nextTimeToTick) {
      return;
    }
    nextTimeToTick = millis() + delayBetweenMove;
    shift++;
    if (shift >= 0) {
      shift = -2 * colorSize;
    }

    //fill leds
    for (uint8_t i = 0; i < colorSize; i++) {
      for (int j = 0; j < (_controller->size() + 3 * colorSize); j = j + 2 * colorSize) {
        int16_t id = i + j + shift;
        int16_t id2 = id + colorSize;
        if (id >= 0 && id < _controller->size()) {
          _controller->leds()[id] = colors[0];
        }
        if (id2 >= 0 && id2 < _controller->size()) {
          _controller->leds()[id2] = colors[1];
        }
      }
    }
  }

protected:
  unsigned long nextTimeToTick = 0;
  int8_t shift = -35;
  bool odd = true;
  CRGB colors[2] = {
      CRGB::Red,
      CRGB::White};
};
