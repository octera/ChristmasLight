#pragma once

#include <LEDEffect.hpp>

class ExtendedBaseEffect : public BaseEffect
{
public:
    ExtendedBaseEffect(const char *name, const size_t jsonBufferSize = 2 * JSON_NODE_SIZE) : BaseEffect(name, jsonBufferSize){};

protected:
    void serializeJsonColor(JsonObject &data, CRGB color) const
    {
        JsonArray &color_rgb = data.createNestedArray("color_rgb");
        color_rgb.add(color.r);
        color_rgb.add(color.g);
        color_rgb.add(color.b);
    }

    void deserializeJsonColor(JsonObject &data, CRGB *color)
    { // deltaHue
        if (data.containsKey("color_rgb"))
        {
            LEDEFFECT_DEBUG_PRINT(F("ExtendedBaseEffect: color_rgb to ["));
            LEDEFFECT_DEBUG_PRINT(data["color_rgb"][0].as<uint8_t>());
            LEDEFFECT_DEBUG_PRINT(F(", "));
            LEDEFFECT_DEBUG_PRINT(data["color_rgb"][1].as<uint8_t>());
            LEDEFFECT_DEBUG_PRINT(F(", "));
            LEDEFFECT_DEBUG_PRINT(data["color_rgb"][2].as<uint8_t>());
            LEDEFFECT_DEBUG_PRINTLN(F("]"));
            color->r = data["color_rgb"][0];
            color->g = data["color_rgb"][1];
            color->b = data["color_rgb"][2];
            // color hsv
        }
        else if (data.containsKey("color_hsv"))
        {
            LEDEFFECT_DEBUG_PRINT(F("ExtendedBaseEffect: color_hsv to ["));
            LEDEFFECT_DEBUG_PRINT(data["color_hsv"][0].as<uint8_t>());
            LEDEFFECT_DEBUG_PRINT(F(", "));
            LEDEFFECT_DEBUG_PRINT(data["color_hsv"][1].as<uint8_t>());
            LEDEFFECT_DEBUG_PRINT(F(", "));
            LEDEFFECT_DEBUG_PRINT(data["color_hsv"][2].as<uint8_t>());
            LEDEFFECT_DEBUG_PRINTLN(F("]"));
            CRGB curColor = CHSV(data["color_hsv"][0], data["color_hsv"][1], data["color_hsv"][2]);
            color->r = curColor.r;
            color->g = curColor.g;
            color->b = curColor.b;
        }
    }
};