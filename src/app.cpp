
#include "app.hpp"

#include <vulkan/vulkan_core.h>

#include <cmath>
#include <glm/fwd.hpp>

#include <glm/geometric.hpp>
#include <vector>


#include "device.hpp"
#include "dynamic_renderpass.hpp"
#include "scene/mesh.hpp"
#include "scene/object.hpp"
#include "scene/objects/animatic/BVH.h"
#include "struct.hpp"
#include "swapchain.hpp"
#include "utils.hpp"

namespace TTe {

void App::init(Device *device, SwapChain *swapchain) {
    this->swapchain = swapchain;
    this->device = device;

    BVH bvh;
    bvh.init("../data/Robot.bvh");

    renderPass =
        DynamicRenderPass(device, {1280, 720}, {}, swapchain->getswapChainImages().size(), depthAndStencil::DEPTH, swapchain, nullptr);

    std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, 0},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, 0},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, 0},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, 0},
    };

    std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0};
    Mesh m = Mesh(device, indices, vertices);

    Mesh m2 = Mesh(device, "../data/mesh/cubes.obj");

    scene = Scene(device);
    Object o = Object();
    o.meshId = 0;
    o.translation = {1, 1, 1};
    scene.objects.push_back(o);
    scene.meshes.push_back(m2);
    scene.camera.translation = {10, 10, 10};
    scene.camera.extent = {1280, 720};
    scene.materials.push_back( {"",{1, 0, 1, 1}, -1, -1});
    scene.materials.push_back( {"",{1, 1, 1, 1}, -1, -1});

    scene.materials.push_back( {"",{0, 0, 1, 1}, -1, -1});

    scene.materials.push_back( {"",{1, 0, 0, 1}, -1, -1});
    scene.materials.push_back( {"",{0, 0, 0, 1}, -1, -1});




    // scene.addBVH(bvh);

    // scene.textures.push_back(image);

    scene.createDescriptorSets();

    
    scene.updateBuffer();

    
}

void App::resize(int width, int height) { 
    renderPass.resize({static_cast<uint32_t>(width), static_cast<uint32_t>(height)}); 
    scene.camera.extent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
}
void App::update(float deltaTime, CommandBuffer &cmdBuffer, Window &windowObj) {
    time += deltaTime;

    //   float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    //   float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    //   float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    movementController.moveInPlaneXZ(&windowObj, deltaTime, scene.camera);
    renderPass.setClearColor({0.01, 0.01, 0.01});
    scene.camera.rotation = glm::normalize(glm::vec3(-1));
    // std::cout << time << std::endl;
    // std::cout << "camera rot : " << scene.camera.rotation.x << " " << scene.camera.rotation.y << " " << scene.camera.rotation.z << std::endl;
    scene.updateCameraBuffer();
}
void App::renderFrame(float deltatTime, CommandBuffer &cmdBuffer, uint32_t curentFrameIndex) {
    // copy renderedImage to swapchainImage
    // if (renderedImage) {

    // swapchain->getSwapChainImage(curentFrameIndex).transitionImageLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, &cmdBuffer);

    renderPass.beginRenderPass(cmdBuffer, curentFrameIndex);
    scene.render(cmdBuffer);
    renderPass.endRenderPass(cmdBuffer);

    // Image::blitImage(device, *renderedImage, (*swapchainImages)[curentFrameIndex], &cmdBuffer);
    // cmdBuffer.addRessourceToDestroy(renderedImage);
    // renderedImage = nullptr;
    // testMutex.unlock();

    // testMutex.lock();
    // cmdBuffer.addRessourceToDestroy(new Image(*renderedImage));
    // testMutex.unlock();

    // }
}
}  // namespace TTe