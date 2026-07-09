## July 9, 2026

Mossworld's environment is now persistent. Moisture no longer resets each day, fertility changes over time, and the world's first ecological feedback loop has begun to emerge.

### What I Built

- Made tile moisture persistent across days.
- Added morning dew and evaporation as daily environmental processes.
- Introduced tile fertility as a persistent environmental property.
- Implemented fertility updates based on moisture conditions and nutrient activity.
- Refined mossling movement according to moisture preferences.
- Added moisture information to the Mossling observer HUD and made Mosslings visible in the moisture view.
- Refactored world initialization, environmental updates, and entity placement into clearer responsibilities.

### Why

- Persistent environmental state allows the world to develop a history rather than resetting each day.
- Fertility introduces the first ecological feedback loop, connecting environmental conditions to resource availability.
- The architectural refactors separate world generation from ongoing simulation, making future environmental systems easier to integrate.

### What I Learned

- Not every system in a simulation should be modeled as an entity. Some concepts, like moisture and fertility, are better represented as environmental processes.
- Configuration should describe the world, while true simulation constants belong in the code itself.
- As environmental systems begin interacting, architectural boundaries become increasingly important. Separating environment updates, entity behavior, and initialization keeps new systems composable rather than tightly coupled.

### Next

- Continue expanding the ecosystem through new environmental systems and creature behaviors.


---

## July 2, 2026

Mossworld now has its first environmental system: moisture. This update also introduces observer views and a major UI refresh.

### What I Built

- Added moisture as a property of each tile.
- Implemented moisture generation and spreading.
- Added a moisture observer view and support for switching between observer views.
- Refactored the simulation loop into `BeginDay()`, `Observe()`, and `RunCreatures()`.
- Updated the observer UI to better support multiple views.

### Why

- Moisture is the first environmental system in Mossworld and lays the foundation for future environmental simulation.
- Observer views make it possible to inspect different aspects of the world without changing the simulation itself.
- Separating the simulation loop makes each stage of a day more explicit and easier to expand with future systems.

### What I Learned

- std::clamp() provides a concise way to constrain values within a valid range.
- auto is most useful when it improves readability without hiding important type information.
- Environmental systems and observer tools are separate concerns and benefit from being designed independently.


### Next

- Give Mosslings an IdealMoisture attribute that influences their movement decisions.

---


## June 27, 2026

Mosslings have objectives and nutrient seeking behavior.

### What I built

- Introduced an objective system, allowing creatures to identify and move toward nearby nutrient clusters based on their level of need.
- Refactored creature energy state and added helpers for managing objectives.
- Added a NutrientNeed enum and updated narration to also use it.
- Simplified entity access with new helper methods like GetPosition() on Entity.


### Why 

- This lays the foundation for goal-oriented behavior, beginning with the most fundamental need: finding nourishment.
- The supporting refactors also improve encapsulation and simplify future behavior systems. 


### Learned

- Reinforced the difference between references and pointers, especially when working with `std::unique_ptr`. Dereferencing a smart pointer (`*ptr`) passes the object by reference, not by pointer.
- Learned to use references when an object is guaranteed to exist, and pointers when the absence of an object is meaningful.
- Began using scoped enums (`enum class`) to improve type safety and avoid namespace collisions.

---

## June 21, 2026

Completed a major architectural refactor of Mossworld's entity system.


### What I built

- Added Entity inheritance.
- Replaced position tracking with Tile* pointers.
- Changed tile occupancy from EntityType to Entity*.

### Why

- Tiles now reference entities directly.
- Eliminated repeated coordinate lookups.
- Established clear ownership and object relationships.
- Prevented pointer invalidation with std::unique_ptr.

### Learned

- Learned the distinction between raw pointers and smart pointers.
- Realized that pointers answer where an object is, while unique_ptr answers who owns the object.
- Discovered how std::vector reallocation can invalidate stored pointers.
- Learned when to use ->, .get(), dereferencing (*), and nullptr.

### Next

- Mossling's get individual behavior, additional entity types and emergent interactions between them.