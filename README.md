# Flashlight Engine

Flashlight engine is a game engine made with C++.

I mainly maintain this project for educational purposes, since I want to learn how game engines work
and how to make one myself.

## Building the engine

For now there is no xmake package for the engine.

To build the tests and examples, or just the static library, here are the instructions :

### Getting the build system

This project uses [xmake](https://xmake.io) for its build system, so you will need to install it.

> [!TIP]
> XMake overrides the already set options when you use the `xmake f <something>` command, so you may want to add 
> multiples options to enable multiple features at once.
> Another option is to use [this plugin](https://github.com/SirLynix/xmake-plugins)
> (by [SirLynix](https://github.com/SirLynix)) that lets you use the command `xmake cu <options>` to add options without
> overwriting the already set ones. 

### Enabling the tests

To enable the tests, use the command `xmake f --build_tests=y` in the project's directory.

### Enabling the examples

> [!NOTE]  
> There are no examples for now, since the engine isn't ready yet to create some.

To enable the examples, use the command `xmake f --build_examples=y` in the project's directory.

### Enabling the Tracy profiler.

Flashlight Engine supports [Tracy](https://github.com/wolfpld/tracy) as the main way of profiling the engine. To enable
it, simple use the `xmake f --tracy_profiler=y` command in the project's directory.

### Building the engine

To build all the enabled target, use the command `xmake build` (it can be shortened to `xmake b` or simply `xmake`).  
To build a specific target, use the command `xmake build <TargetName>` (or `xmake b <TargetName>`).   
For example : to build the main engine target, use the command `xmake build FlashlightEngine` (or `xmake b FlashlightEngine`).  
XMake will also build all the targets the one you are trying to build depends on, for example, `FlTests` (the engine's test suite)
depends on the main engine target, so when you will try to build the `FlTests` target, it will also compile the
`FlashlightEngine` target.

## Roadmap

- [X] Logger (Using [spdlog](https://github.com/gabime/spdlog))
- [X] ECS (Custom made)
- [X] Profiler integration (Using the [Tracy profiler](https://github.com/wolfpld/tracy))
- [ ] WebGPU Renderer
  - [ ] PBR
  - [ ] Deferred rendering
  - [ ] Post-processing effects
  - [ ] Camera
  - [ ] Lights
  - [ ] Windowing
  - [ ] UI overlay
  - [ ] Cubemap
  - [ ] Normal mapping
- [ ] Custom math library
  - [ ] Vector class
  - [ ] Matrix class
  - [ ] Quaternion class
  - [ ] Transform entity component
  - [ ] Angle units classes
- [ ] Physics
- [ ] Animations
- [ ] Model loading
- [ ] Scripting (with Lua)
- [ ] Audio