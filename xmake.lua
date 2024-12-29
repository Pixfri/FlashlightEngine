set_xmakever("2.9.3")

ProjectName = "FlashlightEngine"

set_project(ProjectName)

add_rules("mode.debug", "mode.release")

option("override_runtime", {description = "Override VS runtime to MD in release and MDd in debug.", default = true})
option("profiler", {description = "Enable the Tracy profiler.", default = false})
option("force_vk_debug", {description = "Force Vulkan debug utilities to be enabled.", default = false})

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

add_cxflags("-Wno-missing-field-initializers -Werror=vla", {tools = {"clang", "gcc"}})

add_requires(
    "stb",
    "glfw",
    "spdlog",
    "slang",
    "volk"
)

if is_plat("windows") then
  if has_config("override_runtime") then
    set_runtimes(is_mode("debug") and "MDd" or "MD")
  end
end

if has_config("profiler") then
  add_defines("FL_PROFILER_ENABLED")
  add_requires("tracy")
end

if has_config("force_vk_debug") then
  add_defines("FL_FORCE_VULKAN_DEBUG")
end

rule("cp-resources")
  after_build(function(target)
    os.cp("Resources", "./bin/$(plat)_$(arch)_$(mode)/")
  end)

target(ProjectName)
  set_kind("binary")
  add_rules("cp-resources")

  add_files("Source/**.cpp")

  for _, ext in ipairs({".hpp", ".inl"}) do
    add_headerfiles("Include/**" .. ext)
  end

  add_packages(
    "stb",
    "glfw",
    "spdlog",
    "slang",
    "volk"
  )

  if has_config("profiler") then
    add_packages("tracy")
  end

  add_defines("VK_NO_PROTOTYPES", "GLFW_INCLUDE_VULKAN")

  add_rpathdirs("$ORIGIN")

includes("xmake/**.lua")