#include "ArenaTest.hpp"
#include "Rendering/Vulkan/VulkanRenderer.hpp"

int main(int ac, char** av)
{
	te::VulkanRenderer renderer;
	testArena();
	return 0;
}