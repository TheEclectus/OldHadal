# (Old) Hadal
This project represents the results of my experience learning game engine principles and practices. SDL2 is leveraged as the graphics backend, and the incredible entt ECS framework, with as few other helper libraries as possible sitting on top of it.
Implemented:
- Runtime render target textures + composition (for lighting, occlusion, etc.)
- Procedural noise-based terrain geometry
- A completely custom UI (no helper libraries or reference implementations!) with dynamic sizing, virtual scrolling, bitmap fonts and more
- Pixel-perfect fast(tm) raycasting
- A suite of debugging tools to test in-game features and tweak lighting, test collisions, etc.
