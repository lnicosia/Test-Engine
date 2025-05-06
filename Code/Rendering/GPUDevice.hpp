#ifndef _GPU_DEVICE_HPP_
#define _GPU_DEVICE_HPP_

#include "Rendering/Vertex.hpp"
#include "Rendering/MeshInternal.hpp"
#include "Rendering/FrameStats.hpp"
#include "Rendering/Camera.hpp"
#include "Assets/Textures/Texture.hpp"
#include "Scene/Scene.hpp"

#include <vector>
#include <memory>

#define MAX_FRAMES_IN_FLIGHT 2

namespace te
{
	/** Base GPU Device responsible for every GPU allocation, free, etc */
	class GPUDevice
	{
	public:
		GPUDevice();
		virtual ~GPUDevice();

		virtual void createVertexBuffer(const std::vector<Vertex>& vertices,
			const std::vector<uint32_t>& indices, void* outBuffer, void* outBufferMemory) = 0;

		virtual void drawFrame(const Camera& camera) = 0;
		virtual void updateDrawContext(const Scene& scene) = 0;
		virtual void updateCameraContext(const Camera& camera) = 0;
		
	public:

		FrameStats frameStats;

	protected:
		std::vector<std::shared_ptr<Texture>> textures{};
		//std::vector<std::shared_ptr<MeshInternal>> meshes{};
	};
} // namespace te

#endif // _GPU_DEVICE_HPP_