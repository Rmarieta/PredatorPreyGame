#include "Utility/Vec2d.hpp"
#include <SFML/Graphics.hpp>
#include <Interface/Drawable.hpp>

#ifndef OBSTACLE_H
#define OBSTACLE_H

class Obstacle: public virtual Drawable {

	public:

virtual double getRadius() const=0;
virtual Vec2d getPosition() const=0;
		bool isColliding(Obstacle* other) const;
		bool isPointInside(const Vec2d& p) const;
		bool operator|(Obstacle*) const;
		bool operator>(const Vec2d& p) const;

	private:
		

};

#endif
