#ifndef VIEW_MODE_H
#define VIEW_MODE_H

#include <string>

// Inspector for development use.
enum class ViewMode {
    Inspector,
    World,
    Moisture,
    Sunlight,
    Elevation,
    CanopyCover
};

inline std::string ModeToString(ViewMode mode) {
    switch (mode) {
        case ViewMode::World:
            return "World";
        case ViewMode::Moisture:
            return "Moisture";
        case ViewMode::Sunlight:
            return "Sunlight";
        case ViewMode::Elevation:
            return "Elevation";
        case ViewMode::CanopyCover:
            return "Canopy";
        default:
            return "Unknown";
    }
}

#endif