#ifndef VIEW_MODE_H
#define VIEW_MODE_H

#include <string>

// Inspector for development use.
enum class ViewMode { Inspector, World, Moisture, Sunlight };

inline std::string ModeToString(ViewMode mode) {
    switch (mode) {
        case ViewMode::World:
            return "World";
        case ViewMode::Moisture:
            return "Moisture";
        case ViewMode::Sunlight:
            return "Sunlight";
        default:
            return "Unknown";
    }
}

#endif