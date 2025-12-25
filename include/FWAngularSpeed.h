#pragma once

class FWAngularSpeed
{
private:
    float fl;
    float fr;
    float bl;
    float br;

public:
    FWAngularSpeed();

    /**
     * Set wheel speeds in rad/s.
     * @param fl Front left wheel speed in rad/s.
     * @param fr Front right wheel speed in rad/s
     * @param bl Rear left wheel speed in rad/s
     * @param br Rear right wheel speed in rad/s
     */
    void updateFrom(float fl, float fr, float bl, float br);

    /**
     * Get front left wheel speed in rad/s
     */
    float getFlWInRad() const;

    /**
     * Get front right wheel speed in rad/s
     */
    float getFrWInRad() const;

    /**
     * Get back left wheel speed in rad/s
     */
    float getBlWInRad() const;

    /**
     * Get back right wheel speed in rad/s
     */
    float getBrWInRad() const;
};