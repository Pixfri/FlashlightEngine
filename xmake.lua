set_xmakever("2.9.3")

ProjectName = "FlashlightEngine"

set_project(ProjectName)

add_rules("mode.debug", "mode.release")

option("override_runtime", {description = "Override VS runtime to MD in release and MDd in debug.", default = true})
option("force_validation", {description = "Force Vulkan validation layers to be enabled.", default = false})
option("profiler", {description = "Enable the Tracy profiler.", default = false})

add_includedirs("Include")

add_rules("plugin.vsxmake.autoupdate")

if is_mode("release") then
  set_fpmodels("fast")
  set_optimize("fastest")
  set_symbols("hidden")
else
  add_defines("FL_DEBUG")
  set_symbols("debug")
end

set_encodings("utf-8")
set_exceptions("cxx")
set_languages("cxx23")
set_rundir("./bin/$(plat)_$(arch)_$(mode)")
set_targetdir("./bin/$(plat)_$(arch)_$(mode)")
set_warnings("allextra")
set_allowedplats("windows", "mingw")

if is_plat("windows") then
  if has_config("override_runtime") then
    set_runtimes(is_mode("debug") and "MDd" or "MD")
  end
end

if has_config("force_validation") then
  add_defines("FL_FORCE_DX_DEBUG_INTERFACE")
end

add_cxflags("-Wno-missing-field-initializers -Werror=vla", {tools = {"clang", "gcc"}})

add_requires("stb", "glfw 3.4", "spdlog v1.9.0")
add_requires("imgui", {configs = {dx11 = true, glfw = true}})
add_requires("directxshadercompiler", "directxmath")

if has_config("profiler") then
    add_defines("FL_PROFILER_ENABLED")
  add_requires("tracy")
end

target(ProjectName)
  set_kind("binary")

  add_files("Source/**.cpp")

  if is_plat("windows") then
    -- MSVC throws a bunch of warnings because of spdlog
    add_defines("_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING")
  end

  for _, ext in ipairs({".hpp", ".inl"}) do
    add_headerfiles("Include/**" .. ext)
  end

  add_packages("stb", "glfw", "spdlog", "imgui")
  add_packages("directxshadercompiler", "directxmath")
  if has_config("profiler") then
    add_packages("tracy")
  end

  add_syslinks("d3d11", "user32", "kernel32")

  add_rpathdirs("$ORIGIN")

includes("xmake/**.lua")