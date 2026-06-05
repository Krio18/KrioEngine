#pragma once

#include <string>
#include <cstdint>

namespace Voxel {
    struct Tag {
        Tag();

        std::string entityName;
        bool enableEntity;
        uint32_t layer;
        std::string tag;
    };
}
