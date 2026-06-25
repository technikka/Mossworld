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