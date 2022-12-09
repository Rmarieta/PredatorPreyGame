#include "Obstacle.hpp"
#include <iostream>
#include <cmath>
#include "Utility/Vec2d.hpp"


bool Obstacle::isColliding(Obstacle* other) const {
	return (getPosition() - other->getPosition()).length() <= (other->getRadius() + getRadius());	
}

bool Obstacle::isPointInside (const Vec2d& p) const {
	return ((getPosition() - p).length() <= getRadius());
}

bool Obstacle::operator|(Obstacle* body) const {
	return isColliding(body);
}

bool Obstacle::operator>(const Vec2d& p) const {
	return isPointInside(p);
}
 



