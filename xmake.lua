set_xmakever("2.9.3")

ProjectName = "FlashlightEngine"

set_project(ProjectName)

add_rules("mode.debug", "mode.release")

option("override_runtime", {description = "Override VS runtime to MD in release and MDd in debug.", default = true})
option("force_d3d12_debug", {description = "Force D3D12 debug utilities to be enabled.", default = false})
option("use_preview_d3d12_sdk", {description = "Use the preview D3D12 SDK.", default = false})
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

add_cxflags("-Wno-missing-field-initializers -Werror=vla", {tools = {"clang", "gcc"}})

add_requires(
  "freeimage",
  "stb",
  "glfw",
  "spdlog",
  "directxmath",
  "directx-headers"
)

if is_plat("windows") then
  if has_config("override_runtime") then
    set_runtimes(is_mode("debug") and "MDd" or "MD")
  end
end

if has_config("force_d3d12_debug") then
  add_defines("FL_FORCE_D3D12_DEBUG")
end

if has_config("use_preview_d3d12_sdk") then
  add_defines("FL_USE_PREVIEW_D3D12_SDK")
end

if has_config("profiler") then
    add_defines("FL_PROFILER_ENABLED")
  add_requires("tracy")
end

rule("cp-resources")
  after_build(function(target)
    os.cp("Resources", "./bin/$(plat)_$(arch)_$(mode)/")
  end)

rule("cp-d3d12")
  after_build(function(target) 
    if (has_config("use_preview_d3d12_sdk")) then
      os.cp("Deps/D3D12-Preview", "./bin/$(plat)_$(arch)_$(mode)/")
    else
      os.cp("Deps/D3D12", "./bin/$(plat)_$(arch)_$(mode)/")
    end

    if is_mode("debug") or has_config("force_d3d12_debug") then
      os.cp("Deps/PIX/bin/WinPixEventRuntime.dll", "./bin/$(plat)_$(arch)_$(mode)/")
    end

    os.cp("Deps/D3DCompiler/d3dcompiler_47.dll", "./bin/$(plat)_$(arch)_$(mode)/")
  end)

target(ProjectName)
  set_kind("binary")
  add_rules("cp-resources", "cp-d3d12")

  add_files("Source/**.cpp")

  add_includedirs("Deps/PIX/include")
  add_linkdirs("Deps/PIX/lib")

  for _, ext in ipairs({".hpp", ".inl"}) do
    add_headerfiles("Include/**" .. ext)
  end

  add_packages(
    "freeimage",
    "stb",
    "glfw",
    "spdlog",
    "directxmath",
    "directx-headers"
  )

  if has_config("profiler") then
    add_packages("tracy")
  end

  add_syslinks(
    "d3d12", 
    "user32", 
    "kernel32",
    "dxgi", 
    "dxguid", 
    "WinMM",
    "d3dcompiler"
  )

  if is_mode("debug") or has_config("force_d3d12_debug") then
    add_syslinks("WinPixEventRuntime.lib")
    add_defines("USE_PIX")
  end

  add_rpathdirs("$ORIGIN")

includes("xmake/**.lua")