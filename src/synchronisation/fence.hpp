#pragma once

#include <vector>

#include "../destroyable.hpp"
#include "../device.hpp"
#include "volk.h"
namespace TTe {
class Fence : public Destroyable {
   public:
    // Constructors
    Fence() {};
    Fence(const Device *device, bool signaled = false);

    // Destructor
    ~Fence();

    // Copy and move constructors
    Fence(const Fence &other) = delete;
    Fence &operator=(const Fence &other) = delete;
    Fence(Fence &&other);
    Fence &operator=(Fence &&other);

    VkResult getFenceStatus() const;
    VkResult waitForFence();
    void resetFence();

    operator VkFence() const { return vk_Fence; }
    static VkResult waitForFences(
        const Device *vkDevice, const std::vector<Fence *> &fences, bool waitAllFence, size_t *firstFenceSignaled);

   private:
    VkFence vk_Fence = VK_NULL_HANDLE;

    const Device *device = nullptr;
};
}  // namespace TTe