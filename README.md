# (Old) Hadal
![https://github.com/TheEclectus/OldHadal/blob/main/screenshot.png](https://github.com/TheEclectus/OldHadal/blob/main/screenshot.png)

https://github.com/TheEclectus/OldHadal/assets/66447088/e6292475-c883-461e-98c9-8f53a4701ab7

This project represents the results of my experience learning game engine principles and practices. SDL2 is leveraged as the graphics backend, and the incredible entt ECS framework, with as few other helper libraries as possible sitting on top of it.
Implemented:
- Runtime render target textures + composition (for lighting, occlusion, etc.)
- Procedural noise-based terrain geometry
- A completely custom UI (no helper libraries or reference implementations!) with dynamic sizing, virtual scrolling, bitmap fonts and more
- Pixel-perfect fast(tm) raycasting
- A suite of debugging tools to test in-game features and tweak lighting, test collisions, etc.

Retrospective:
- Clear goals need better-defined; loose aims don't yield great code.
- Gitea/GitHub have a tremendous integrated issue tracker that makes organizing growing code bases _much_ easier to navigate.
- Having a virtual file structure (in the VS project) not reflected in the filesystem becomes quite unwieldy, and should be cleaned up before it gets this bad.
