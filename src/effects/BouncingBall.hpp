#pragma once

#include <LEDEffect.hpp>

CRGB defaultColor[5] = {
    CRGB::Red,
    CRGB::Green,
    CRGB::Blue,
    CRGB::White,
    CRGB::Yellow};

#define MAX_BALL_COUNT 10
// A rainbow that moves along the leds
class BoucingBall final : public BaseEffect
{
public:
    CRGB *colors;
    int colorSize;
    int ballCount;

    BoucingBall(const char *name, CRGB *colors = defaultColor, int colorSize = 5, int ballCount = 3) : 
        BaseEffect(name, 2 * JSON_NODE_SIZE), colors(colors), colorSize(colorSize), ballCount(ballCount)
    {
        for (int i = 0; i < ballCount; i++)
        {
            clockTimeSinceLastBounce[i] = millis();
            height[i] = StartHeight;
            position[i] = 0;
            impactVelocity[i] = impactVelocityStart;
            timeSinceLastBounce[i] = 0;
            dampening[i] = 0.90 - float(i) / pow(ballCount, 2);
        }
    };

    void deserialize(JsonObject &data) override
    {
    }

    void serialize(JsonObject &data) const override
    {
    }

    void loop() override
    {
        for (int i = 0; i < ballCount; i++)
        {
            timeSinceLastBounce[i] = millis() - clockTimeSinceLastBounce[i];
            height[i] = 0.5 * Gravity * pow(timeSinceLastBounce[i] / 1000, 2.0) + impactVelocity[i] * timeSinceLastBounce[i] / 1000;

            if (height[i] < 0)
            {
                height[i] = 0;
                impactVelocity[i] = dampening[i] * impactVelocity[i];
                clockTimeSinceLastBounce[i] = millis();

                if (impactVelocity[i] < 0.01)
                {
                    impactVelocity[i] = impactVelocityStart;
                }
            }
            position[i] = round(height[i] * (_controller->size() - 1) / StartHeight);
        }

        fill_solid(_controller->leds(), _controller->size(), CRGB::Black);
        //fadeToBlackBy(_controller->leds(), _controller->size(), 50);
        for (int i = 0; i < ballCount; i++)
        {
            _controller->leds()[position[i]] = colors[i];
        }

        
    }

protected:
    float _position = 0;
    float Gravity = -9.81;
    int StartHeight = 1;

    float height[MAX_BALL_COUNT];
    float impactVelocityStart = sqrt(-2 * Gravity * StartHeight);
    float impactVelocity[MAX_BALL_COUNT];
    float timeSinceLastBounce[MAX_BALL_COUNT];
    int position[MAX_BALL_COUNT];
    long clockTimeSinceLastBounce[MAX_BALL_COUNT];
    float dampening[MAX_BALL_COUNT];
};
