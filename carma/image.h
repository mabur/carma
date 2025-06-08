#pragma once

#define FLIP_IMAGE_X(image) do { \
    auto width = (image).width; \
    auto height = (image).height; \
    for (typeof(height) y = 0; y < height; ++y) \
        for (typeof(width) x = 0; x < width / 2; ++x) \
            SWAP((image).data[y * width + width - 1 - x], (image).data[y * width + x]); \
} while(0)

#define FLIP_IMAGE_Y(image) do { \
    auto width = (image).width; \
    auto height = (image).height; \
    for (typeof(height) y = 0; y < height / 2; ++y) \
        for (typeof(width) x = 0; x < width; ++x) \
            SWAP((image).data[(height - 1 - y) * width + x], (image).data[y * width + x]); \
} while(0)
