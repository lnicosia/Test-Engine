
#include "Rendering/SoftwareRendering/SoftwareRenderer.hpp"
#include "Rendering/Vulkan/VulkanRenderer.hpp"
#include "Rendering/OpenGL/GLRenderer.hpp"
#include "Rendering/D3D/D3DRenderer.hpp"
#include "Rendering/SoftwareRendering/Raycaster.hpp"
#include "Rendering/MeshInternal.hpp"
#include "Scene/Actor.hpp"
#include "Debug/Log.hpp"

#include "Platform.hpp"

#if !defined(NDEBUG) && defined(TE_WINDOWS)
#define __CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <memory>

int main()
{
	/*try
	{
		te::SoftwareRenderer renderer;
		te::Raycaster	raycaster(&renderer);
		raycaster.render();
	}*/

	std::vector<te::Vertex> vertices =
	{
		{{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},

		{{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
		{{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
		{{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
		{{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}
	};
	std::vector<uint32_t> indices =
	{
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4
	};
	try
	{
		std::unique_ptr<te::Renderer> renderer = std::make_unique<te::VulkanRenderer>();

		te::MeshGeometry geometry(std::move(vertices), std::move(indices));
		std::shared_ptr<te::MeshInternal> squareMesh = renderer->createSolidMesh(geometry);
		std::shared_ptr<te::MeshInternal> squareMesh2 = renderer->createSolidMesh(geometry);
		std::shared_ptr<te::MeshInternal> loadedMesh =
			renderer->loadMesh(te::Logger::getRootDirPath() + "Resources/Objects/viking_room/viking_room.obj");

		te::Actor* actor = renderer->scene.createActor();
		te::Component* component = renderer->scene.createComponent();
		te::Component* component2 = renderer->scene.createComponent();

		std::shared_ptr<te::Texture> newTex = renderer->loadTexture(
			te::Logger::getRootDirPath() + "Resources/Objects/viking_room/viking_room.png"
		);

		std::shared_ptr<te::Material> mat = std::make_shared<te::Material>(newTex);

		loadedMesh->setMaterial(mat);
		component->setMeshInternal(loadedMesh);
		component2->setMeshInternal(squareMesh);
		actor->addComponent(component);
		actor->addComponent(component2);
		renderer->scene.addActor(actor->getId());
		renderer->render();
		newTex->cleanUp();
	}
	catch (std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
#ifdef __CRTDBG_MAP_ALLOC
	printf("Dumping leaks...\n");
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}