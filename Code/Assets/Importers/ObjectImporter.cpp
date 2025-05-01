#include "ObjectImporter.hpp"

#include "Platform.hpp"

//	Windows fix
#ifdef TE_WINDOWS
# undef max
# undef min
#endif

namespace te
{
	ObjectImporter::ObjectImporter(): max(), min(), isRangeInit(false),
		meshes(), directory(""), path(""), bones(), nbBones(0), axis(Axis::Y_UP)
	{
	}

	ObjectImporter::~ObjectImporter()
	{
	}
}