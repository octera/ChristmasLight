#pragma once

#include <LEDEffect.hpp>

// A rainbow that moves along the leds
class RunningLights final : public BaseEffect
{
public:
  CRGB color;
  float rate = 0.1;       // rate of change of hue on each loop

  RunningLights(const char* name, CRGB color = CRGB::White ) : BaseEffect(name, 2 * JSON_NODE_SIZE), color(color) { };

  void deserialize(JsonObject& data) override {
    // deltaHue
    if (data.containsKey("color_rgb")) {
      LEDEFFECT_DEBUG_PRINT(F("SolidEffect: color_rgb to ["));
      LEDEFFECT_DEBUG_PRINT(data["color_rgb"][0].as<uint8_t>());
      LEDEFFECT_DEBUG_PRINT(F(", "));
      LEDEFFECT_DEBUG_PRINT(data["color_rgb"][1].as<uint8_t>());
      LEDEFFECT_DEBUG_PRINT(F(", "));
      LEDEFFECT_DEBUG_PRINT(data["color_rgb"][2].as<uint8_t>());
      LEDEFFECT_DEBUG_PRINTLN(F("]"));
      color = CRGB(data["color_rgb"][0], data["color_rgb"][1], data["color_rgb"][2]);
    // color hsv
    } else if (data.containsKey("color_hsv")) {
      LEDEFFECT_DEBUG_PRINT(F("SolidEffect: color_hsv to ["));
      LEDEFFECT_DEBUG_PRINT(data["color_hsv"][0].as<uint8_t>());
      LEDEFFECT_DEBUG_PRINT(F(", "));
      LEDEFFECT_DEBUG_PRINT(data["color_hsv"][1].as<uint8_t>());
      LEDEFFECT_DEBUG_PRINT(F(", "));
      LEDEFFECT_DEBUG_PRINT(data["color_hsv"][2].as<uint8_t>());
      LEDEFFECT_DEBUG_PRINTLN(F("]"));
      color = CHSV(data["color_hsv"][0], data["color_hsv"][1], data["color_hsv"][2]);
    }

    // rate
    if (data.containsKey("rate")) {
      LEDEFFECT_DEBUG_PRINT(F("RunningLights: rate to "));
      LEDEFFECT_DEBUG_PRINTLN(data["rate"].as<int8_t>());
      rate = data["rate"].as<int8_t>();
    }
  }

  void serialize(JsonObject& data) const override {
    JsonArray& color_rgb = data.createNestedArray("color_rgb");
    color_rgb.add(color.r);
    color_rgb.add(color.g);
    color_rgb.add(color.b);
    data["rate"] = rate;
  }

  void loop() override {
    _position += rate;
    for(int i=0; i<_controller->size(); i++) {
        float level = sin(i+_position) * 127 + 128;
        _controller->leds()[i] = color%level;
      }
  }

protected:
  float _position = 0;
};
