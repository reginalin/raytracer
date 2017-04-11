#include "ray.h"

<<<<<<< HEAD
ray::ray(vec4 origin, vec4 direction)
{
    this->origin = origin;
    this->direction = direction;
}

const ray ray::getTransformedCopy(mat4 transform) {
    return ray(this->origin * transform, this->direction * transform);
}
=======
//ray::ray()
//{

//}
>>>>>>> 3f354c3a70be2a8447dabbbb5bd72931adfba91d
