
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

void initBasicBindings(te::Renderer* renderer)
{
	te::Binding forward("forward", SDLK_UP, SDLK_W, true);
	forward.whenPressed = std::make_shared<te::Action<void>>([renderer]()
	{
		renderer->camera.moveForward(renderer->getDeltaTime());
	});
	renderer->addBinding(forward);

	te::Binding backward("backward", SDLK_DOWN, SDLK_S, true);
	backward.whenPressed = std::make_shared<te::Action<void>>([renderer]()
	{
		renderer->camera.moveBackward(renderer->getDeltaTime());
	});
	renderer->addBinding(backward);

	te::Binding left("left", SDLK_LEFT, SDLK_A, true);
	left.whenPressed = std::make_shared<te::Action<void>>([renderer]()
	{
		renderer->camera.moveLeft(renderer->getDeltaTime());
	});
	renderer->addBinding(left);

	te::Binding right("right", SDLK_RIGHT, SDLK_D, true);
	right.whenPressed = std::make_shared<te::Action<void>>([renderer]()
	{
		renderer->camera.moveRight(renderer->getDeltaTime());
	});
	renderer->addBinding(right);

	te::MouseBinding mouseMove("mouseMove", SDL_BUTTON_LEFT, 0, true);
	mouseMove.onPress= std::make_shared<te::Action<void>>([renderer]()
	{
		renderer->getWindow()->hideCursor();
		renderer->getWindow()->startRecordingMouse();
	});
	mouseMove.whenPressed = std::make_shared<te::Action<void>>([renderer]()
	{
		renderer->camera.setYaw(
			renderer->camera.getYaw() +
			-(renderer->getWindow()->events->mouseRelativePos.x
			) * renderer->camera.rotationSpeed * renderer->getDeltaTime());
		renderer->camera.setPitch(
			renderer->camera.getPitch() +
			-(renderer->getWindow()->events->mouseRelativePos.y
			) * renderer->camera.rotationSpeed * renderer->getDeltaTime());
		renderer->getWindow()->startRecordingMouse();
	});
	mouseMove.onRelease = std::make_shared<te::Action<void>>([renderer]()
	{
		renderer->getWindow()->showCursor();
	});
	renderer->addMouseBinding(mouseMove);
}

int main()
{
	/*try
	{
		te::SoftwareRenderer renderer;
		te::Raycaster	raycaster(&renderer);
		raycaster.render();
	}*/

	try
	{
		std::unique_ptr<te::Renderer> renderer = std::make_unique<te::VulkanRenderer>();
		initBasicBindings(renderer.get());

		std::shared_ptr<te::MeshInternal> loadedMesh =
			renderer->loadMesh(te::Logger::getRootDirPath() + "Resources/Objects/viking_room/viking_room.obj");
			//renderer->loadMesh(te::Logger::getRootDirPath() + "..\\3D objects\\Pulse pilot\\scene.gltf");

		te::Actor* actor = renderer->scene.createActor();
		te::Component* component = renderer->scene.createComponent();

		std::shared_ptr<te::Texture> newTex = renderer->loadTexture(
			te::Logger::getRootDirPath() + "Resources/Objects/viking_room/viking_room.png"
		);

		std::shared_ptr<te::Material> mat = std::make_shared<te::Material>(newTex);

		loadedMesh->setMaterial(mat);
		component->setMeshInternal(loadedMesh);
		actor->addComponent(component);
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