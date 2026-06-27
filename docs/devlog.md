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