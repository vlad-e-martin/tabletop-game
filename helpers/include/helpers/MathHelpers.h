#ifndef MATH_HELPERS_H
#define MATH_HELPERS_H

#include <SFML/Graphics.hpp>

#define _USE_MATH_DEFINES // for C++
#include <cmath>

namespace GUI::MathHelpers {
    inline double deg2rad(const double& deg) {
        return deg * M_PI / 180.0;
    }

    inline double rad2deg(const double& rad) {
        return rad * 180.0 / M_PI;
    }

    double calcAngleBetweenVectors_deg(const sf::Vector2f& u, const sf::Vector2f& v);

    double calcVectorDotProduct(const sf::Vector2f& u, const sf::Vector2f& v);

    double calcVectorLength(const sf::Vector2f& u);
}

#endif // MATH_HELPERS_H