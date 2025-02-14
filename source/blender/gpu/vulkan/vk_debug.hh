/* SPDX-FileCopyrightText: 2023 Blender Authors All rights reserved.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup gpu
 */
#pragma once

#include "BKE_global.hh"
#include "BLI_compiler_attrs.h"
#include "BLI_set.hh"
#include "BLI_string.h"

#include "vk_common.hh"

#include <typeindex>

namespace blender::gpu {
class VKContext;
class VKDevice;

namespace debug {
class VKDebuggingTools {
 public:
  void init(VkInstance vk_instance);
  void deinit(VkInstance vk_instance);
  void init_messenger(VkInstance vk_instance);
  void destroy_messenger(VkInstance vk_instance);
  void print_labels(const VkDebugUtilsMessengerCallbackDataEXT *callback_data);

 private:
  VkDebugUtilsMessengerEXT vk_debug_utils_messenger = nullptr;
};

void object_label(VkObjectType vk_object_type, uint64_t object_handle, const char *name);
template<typename T> void object_label(T vk_object_type, const StringRefNull name)
{
  if (!(G.debug & G_DEBUG_GPU)) {
    return;
  }
  const size_t label_size = 64;
  char label[label_size];
  memset(label, 0, label_size);
  static int stats = 0;
  SNPRINTF(label, "%s_%d", name.c_str(), stats++);
  object_label(to_vk_object_type(vk_object_type), (uint64_t)vk_object_type, (const char *)label);
};

/**
 * How to use:
 * \code{.cc}
 * debug::raise_message(0xB41ca2, VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
 *                      "This is a raise message. %llx", (uintptr_t)vk_object);
 * \endcode
 */
void raise_message(int32_t id_number,
                   VkDebugUtilsMessageSeverityFlagBitsEXT vk_severity_flag_bits,
                   const char *fmt,
                   ...) ATTR_PRINTF_FORMAT(3, 4);
}  // namespace debug
}  // namespace blender::gpu
