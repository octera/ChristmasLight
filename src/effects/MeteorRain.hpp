#pragma once

#include "ExtendedBaseEffect.hpp"

// A rainbow that moves along the leds
class MeteorRain final : public ExtendedBaseEffect
{
public:
  CRGB color;
  int meteorSize;
  int meteorTrailDecay;
  bool meteorRandomDecay;
  int speedDelay;

  MeteorRain(const char *name, CRGB color = CRGB::Purple, uint8_t meteorSize = 10, uint8_t meteorTrailDecay = 40, bool meteorRandomDecay = true, uint8_t speedDelay = 30) : 
  ExtendedBaseEffect(name, 2 * JSON_NODE_SIZE), color(color), meteorSize(meteorSize), meteorTrailDecay(meteorTrailDecay), meteorRandomDecay(meteorRandomDecay), speedDelay(speedDelay){};

  void deserialize(JsonObject &data) override
  {
    deserializeJsonColor(data, &color);

    // meteorSize
    if (data.containsKey("meteorSize")) {
      LEDEFFECT_DEBUG_PRINT(F("MeteorRain: meteorSize to "));
      LEDEFFECT_DEBUG_PRINTLN(data["meteorSize"].as<uint8_t>());
      meteorSize = data["meteorSize"].as<uint8_t>();
    }

        // meteorTrailDecay
    if (data.containsKey("meteorTrailDecay")) {
      LEDEFFECT_DEBUG_PRINT(F("MeteorRain: meteorTrailDecay to "));
      LEDEFFECT_DEBUG_PRINTLN(data["meteorTrailDecay"].as<uint8_t>());
      meteorTrailDecay = data["meteorTrailDecay"].as<uint8_t>();
    }

        // meteorRandomDecay
    if (data.containsKey("meteorRandomDecay")) {
      LEDEFFECT_DEBUG_PRINT(F("MeteorRain: meteorRandomDecay to "));
      LEDEFFECT_DEBUG_PRINTLN(data["meteorRandomDecay"].as<bool>());
      meteorRandomDecay = data["meteorRandomDecay"].as<bool>();
    }

        // speedDelay
    if (data.containsKey("speedDelay")) {
      LEDEFFECT_DEBUG_PRINT(F("MeteorRain: speedDelay to "));
      LEDEFFECT_DEBUG_PRINTLN(data["speedDelay"].as<uint8_t>());
      speedDelay = data["speedDelay"].as<uint8_t>();
    }
  }

  void serialize(JsonObject &data) const override
  {
    serializeJsonColor(data, color);
    data["meteorSize"] = meteorSize;
    data["meteorTrailDecay"] = meteorTrailDecay;
    data["meteorRandomDecay"] = meteorRandomDecay;
    data["speedDelay"] = speedDelay;
  }

  void loop() override
  {
    if (millis() < nextTimeToTick)
      return; //not time to do anything
    nextTimeToTick = millis() + speedDelay;
    // fade brightness all LEDs one step
    for (int j = 0; j < _controller->size(); j++)
    {
      if ((!meteorRandomDecay) || (random(10) > 5))
      {
        _controller->leds()[j].fadeToBlackBy(meteorTrailDecay);
      }
    }

    // draw meteor
    for (int j = 0; j < meteorSize; j++)
    {
      if ((step - j < _controller->size()) && (step - j >= 0))
      {
        int ledId = _controller->size() - step + j - 1;
        _controller->leds()[ledId] = color;
      }
    }

    step++;
    if (step == _controller->size())
    {
      step = 0;
    }
  }

protected:
  int step = 0;
  unsigned long nextTimeToTick = 0;
};
