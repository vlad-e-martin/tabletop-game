#include <helpers/MathHelpers.h>

double GUI::MathHelpers::calcAngleBetweenVectors_deg(const sf::Vector2f& u, const sf::Vector2f& v) {
    const double angle_rad = std::acos(calcVectorDotProduct(u, v) / (calcVectorLength(u) * calcVectorLength(v)));
    if (u.x * v.y - u.y * v.x < 0.0) {
        return -rad2deg(angle_rad); 
    }
    return rad2deg(angle_rad);
}

double GUI::MathHelpers::calcVectorDotProduct(const sf::Vector2f& u, const sf::Vector2f& v) {
    return u.x * v.x + u.y * v.y;
}

double GUI::MathHelpers::calcVectorLength(const sf::Vector2f& u) {
    return std::sqrt(u.x * u.x + u.y * u.y);
}