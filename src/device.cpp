
#include "device.hpp"
#include "VkBootstrap.h"
#include "structs_vk.hpp"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

#include <volk.h>
namespace TTe {

Device::Device(Window &window) {
    createInstance();
    selectPhysicalDevice(window);
    createLogicialDevice();
    initVMA();
}

void Device::createInstance() {
    vkb::InstanceBuilder instance_builder;
    instance_builder.set_app_name("TTEngine 2.0").set_engine_name("TTEngine 2.0").require_api_version(1, 3, 0);

    if (enableValidationLayers) {
        instance_builder.request_validation_layers()
            .use_default_debug_messenger();  //.add_validation_feature_enable(VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT);
    }
    auto instance_builder_return = instance_builder.build();

    if (!instance_builder_return) {
        throw std::runtime_error(instance_builder_return.error().message());
    }
    vkbInstance = std::move(instance_builder_return.value());
    volkInitialize();
    volkLoadInstance(vkbInstance.instance);
}

void Device::selectPhysicalDevice(Window &window) {
    this->surface = window.getSurface(vkbInstance);
    vkb::PhysicalDeviceSelector phys_device_selector(vkbInstance);
    phys_device_selector.set_surface(surface);
    setRequiredExtensions(phys_device_selector);
    setRequiredFeatures(phys_device_selector);
    auto physical_device_selector_return = phys_device_selector.select();
    if (!physical_device_selector_return) {
        throw std::runtime_error(physical_device_selector_return.error().message());
    }
    vkbPhysicalDevice = std::move(physical_device_selector_return.value());
}

void Device::createLogicialDevice() {
    vkb::DeviceBuilder device_builder{vkbPhysicalDevice};
    auto dev_ret = device_builder.build();
    if (!dev_ret) {
        throw std::runtime_error(dev_ret.error().message());
    }
    vkbDevice = std::move(dev_ret.value());

    renderQueueFamilyIndex = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();
    computeQueueFamilyIndex = vkbDevice.get_queue_index(vkb::QueueType::compute).value();
    transferQueueFamilyIndex = vkbDevice.get_queue_index(vkb::QueueType::transfer).value();
    vkGetDeviceQueue(vkbDevice.device, renderQueueFamilyIndex, 0, &renderQueue);
    vkGetDeviceQueue(vkbDevice.device, computeQueueFamilyIndex, 0, &computeQueue);
    vkGetDeviceQueue(vkbDevice.device, transferQueueFamilyIndex, 0, &transferQueue);

    vk_device = vkbDevice.device;
}

void Device::initVMA() {
    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.physicalDevice = vkbPhysicalDevice.physical_device;
    allocatorInfo.device = *this;
    allocatorInfo.instance = vkbInstance.instance;
    allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_3;

    
    vma_vulkan_func.vkGetDeviceProcAddr = vkGetDeviceProcAddr;
    vma_vulkan_func.vkGetInstanceProcAddr = vkGetInstanceProcAddr;

    vma_vulkan_func.vkAllocateMemory = vkAllocateMemory;
    vma_vulkan_func.vkBindBufferMemory = vkBindBufferMemory;
    vma_vulkan_func.vkBindImageMemory = vkBindImageMemory;
    vma_vulkan_func.vkCreateBuffer = vkCreateBuffer;
    vma_vulkan_func.vkCreateImage = vkCreateImage;
    vma_vulkan_func.vkDestroyBuffer = vkDestroyBuffer;
    vma_vulkan_func.vkDestroyImage = vkDestroyImage;
    vma_vulkan_func.vkFlushMappedMemoryRanges = vkFlushMappedMemoryRanges;
    vma_vulkan_func.vkFreeMemory = vkFreeMemory;
    vma_vulkan_func.vkGetBufferMemoryRequirements = vkGetBufferMemoryRequirements;
    vma_vulkan_func.vkGetImageMemoryRequirements = vkGetImageMemoryRequirements;
    vma_vulkan_func.vkGetPhysicalDeviceMemoryProperties = vkGetPhysicalDeviceMemoryProperties;
    vma_vulkan_func.vkGetPhysicalDeviceProperties = vkGetPhysicalDeviceProperties;
    vma_vulkan_func.vkInvalidateMappedMemoryRanges = vkInvalidateMappedMemoryRanges;
    vma_vulkan_func.vkMapMemory = vkMapMemory;
    vma_vulkan_func.vkUnmapMemory = vkUnmapMemory;
    vma_vulkan_func.vkCmdCopyBuffer = vkCmdCopyBuffer;

    allocatorInfo.pVulkanFunctions = &vma_vulkan_func;
    vmaCreateAllocator(&allocatorInfo, &_allocator);
}

void setRequiredExtensionsFeatures(vkb::PhysicalDeviceSelector &phys_device_selector) {
    VkPhysicalDeviceShaderObjectFeaturesEXT shaderObjFeature{};
    shaderObjFeature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_OBJECT_FEATURES_EXT;
    shaderObjFeature.shaderObject = true;

    phys_device_selector.add_required_extension_features(shaderObjFeature);
}

// Features paramater

void setRequiredFeatures10(vkb::PhysicalDeviceSelector &phys_device_selector) {
    auto required_features = make<VkPhysicalDeviceFeatures>();
    required_features.samplerAnisotropy = true;
    phys_device_selector.set_required_features(required_features);
}

void setRequiredFeatures11(vkb::PhysicalDeviceSelector &phys_device_selector) {
    auto required_features = make<VkPhysicalDeviceVulkan11Features>();
    phys_device_selector.set_required_features_11(required_features);
}

void setRequiredFeatures12(vkb::PhysicalDeviceSelector &phys_device_selector) {
    auto required_features12 = make<VkPhysicalDeviceVulkan12Features>();
    required_features12.descriptorBindingPartiallyBound = true;
    // required_features12.runtimeDescriptorArray = true;
    //  required_features12.descriptorBindingVariableDescriptorCount = true;
    // required_features12.drawIndirectCount = true;
    // required_features12.samplerFilterMinmax = true;
    required_features12.bufferDeviceAddress = true;
    required_features12.descriptorIndexing = true;
    required_features12.scalarBlockLayout = true;

    phys_device_selector.set_required_features_12(required_features12);
}

void setRequiredFeatures13(vkb::PhysicalDeviceSelector &phys_device_selector) {
    auto required_features13 = make<VkPhysicalDeviceVulkan13Features>();
    required_features13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    required_features13.dynamicRendering = true;
    required_features13.synchronization2 = true;

    phys_device_selector.set_required_features_13(required_features13);
}

void Device::setRequiredFeatures(vkb::PhysicalDeviceSelector &phys_device_selector) {
    setRequiredExtensionsFeatures(phys_device_selector);
    setRequiredFeatures10(phys_device_selector);
    setRequiredFeatures12(phys_device_selector);
    setRequiredFeatures13(phys_device_selector);
}

void Device::setRequiredExtensions(vkb::PhysicalDeviceSelector &phys_device_selector) {
    phys_device_selector.add_required_extension("VK_EXT_shader_object");
}

Device::~Device() {
    vmaDestroyAllocator(_allocator);
    vkb::destroy_device(vkbDevice);
    vkb::destroy_surface(vkbInstance, surface);
    vkb::destroy_instance(vkbInstance);
}

}  // namespace TTe