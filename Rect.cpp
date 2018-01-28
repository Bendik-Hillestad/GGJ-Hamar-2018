#include "Rect.h"

namespace GGJ
{
    bool Rect::Intersects(Rect const &rect) const noexcept
    {
        //Get the line from the center of one rectangle to the other
        auto d = this->center - rect.center;

        //Try to find a separating axis
        if ( d.x > this->halfSize.x + rect.halfSize.x) return false;
        if (-d.x > this->halfSize.x + rect.halfSize.x) return false;
        if ( d.y > this->halfSize.y + rect.halfSize.y) return false;
        if (-d.y > this->halfSize.y + rect.halfSize.y) return false;

        //They intersect
        return true;
    }

    bool Rect::Contains(Rect const &rect) const noexcept
    {
        if (this->center.x - this->halfSize.x > rect.center.x - rect.halfSize.x) return false;
        if (this->center.x + this->halfSize.x < rect.center.x + rect.halfSize.x) return false;
        if (this->center.y - this->halfSize.y > rect.center.y - rect.halfSize.y) return false;
        if (this->center.y + this->halfSize.y < rect.center.y + rect.halfSize.y) return false;

        //It's contained
        return true;
    }
};
