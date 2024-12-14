set_xmakever("2.9.3")

ProjectName = "FlashlightEngine"

set_project(ProjectName)

add_rules("mode.debug", "mode.release")

option("override_runtime", {description = "Override VS runtime to MD in release and MDd in debug.", default = true})

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
set_languages("cxx20")
set_rundir("./bin/$(plat)_$(arch)_$(mode)")
set_targetdir("./bin/$(plat)_$(arch)_$(mode)")
set_warnings("allextra")

if is_plat("windows") then
  if has_config("override_runtime") then
    set_runtimes(is_mode("debug") and "MDd" or "MD")
  end
end

add_cxflags("-Wno-missing-field-initializers -Werror=vla", {tools = {"clang", "gcc"}})

add_requires("volk", "vulkan-memory-allocator", "glfw", "spdlog v1.9.0")
add_requires("imgui", {configs = {glfw = true, vulkan = true}})
add_requires("optick", {configs = {vulkan = true}})

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

  add_packages("volk", "vulkan-memory-allocator", "glfw", "spdlog", "imgui", "optick")
  
  add_rpathdirs("$ORIGIN")

includes("xmake/**.lua")