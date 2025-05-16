#ifndef __PARTICLE_HPP__
#define __PARTICLE_HPP__

#include "Maths/Vector.hpp"

#define MAX_PARTICLES 1024

namespace te
{
	struct Particle
	{
		alignas(16) sml::vec3 pos;
		alignas(16) sml::vec3 velocity;
		alignas(16) sml::vec4 color;
	};
} // namespace te

#endif // __PARTICLE_HPP__