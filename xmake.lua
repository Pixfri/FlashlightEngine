set_xmakever("2.9.3")

-- Define a global for the project name so that other files can retrieve it.
ProjectName = "FlashlightEngine"

set_project(ProjectName)

add_rules("mode.debug", "mode.release")
add_rules("plugin.vsxmake.autoupdate")

if is_mode("release") then
  set_fpmodels("fast")
  set_optimize("fastest")
  set_symbols("hidden")
else
  add_defines("FL_DEBUG")
  set_symbols("debug")
end

add_includedirs("Include")
set_encodings("utf-8")
set_exceptions("cxx")
set_languages("cxx20")
set_rundir("./bin/$(plat)_$(arch)_$(mode)")
set_targetdir("./bin/$(plat)_$(arch)_$(mode)")
set_warnings("allextra")
add_cxflags("-Wno-missing-field-initializers -Werror=vla", {tools = {"clang", "gcc"}})

option("override_runtime", {description = "Override VS runtime to MD in release and MDd in debug.", default = true})
option("build_tests", {description = "Build the tests.", default = false})
option("build_examples", {description = "Build the examples.", default = false})

if is_plat("windows") then
  if has_config("override_runtime") then
    set_runtimes(is_mode("debug") and "MDd" or "MD")
  end
end

add_requires(
  "spdlog",
  "entt"
)

target(ProjectName)
  set_kind("static")
  
  add_files("Source/**.cpp")
  
  for _, ext in ipairs({".hpp", ".inl"}) do
    add_headerfiles("Include/**" .. ext)
  end

  add_defines("FL_BUILD")
  
  add_rpathdirs("$ORIGIN")

  add_packages(
    "spdlog",
    "entt",
    {public = true}
  )

includes("xmake/**.lua")

if has_config("build_tests") then
  -- includes("tests/xmake.lua")
end

if has_config("build_examples") then
  includes("examples/xmake.lua")
end