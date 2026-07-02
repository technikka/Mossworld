#ifndef VIEW_MODE_H
#define VIEW_MODE_H

#include <string>

enum class ViewMode { World, Moisture };

inline std::string ModeToString(ViewMode mode) {
    switch (mode) {
        case ViewMode::World:
            return "World";
        case ViewMode::Moisture:
            return "Moisture";
        default:
            return "Unknown";
    }
}

#endif