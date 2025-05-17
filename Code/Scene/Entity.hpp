#ifndef _ENTITY_HPP_
#define _ENTITY_HPP_

#include <cstdint>

namespace te
{
	struct Entity
	{
		Entity()
		{
			id = generateId();
		}

		uint64_t id;

		uint64_t generateId()
		{
			return count++;
		}

		uint64_t getId() const
		{
			return id;
		}

	protected:
		static uint64_t count;
	};

	struct EntityRef
	{
		EntityRef(uint64_t id) : id{id}
		{
		}
		uint64_t id{};

		uint64_t getId() const { return id; }
	};
} // namespace te`


#endif // _ENTITY_HPP_