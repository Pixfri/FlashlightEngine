add_requires("catch2")

target("flashlightTest-dummy", function(target)
	set_kind("shared")
	set_languages("cxx20")
	add_files("../build/$(plat)_$(arch)_$(mode)/dummy.cpp")
	set_warnings("none")
	on_load(function(target)
		if is_host("windows") then
			io.writefile(
				"build/$(plat)_$(arch)_$(mode)/dummy.cpp",
				[[
            extern "C" {
                __declspec(dllexport) void __cdecl Dummy() {}
                __declspec(dllexport) int __cdecl DummyInt() { return 42;}
				__declspec(dllexport) int __cdecl Increment(int v) { return v + 1;}
            }
            ]]
			)
		elseif is_host("linux") or is_host("macos") then
			io.writefile(
				"build/$(plat)_$(arch)_$(mode)/dummy.cpp",
				[[
            extern "C" {
                 void Dummy() {}
                 int DummyInt() { return 42;}
			     int Increment(int v) { return v + 1;}
            }
            ]]
			)
		end
	end)
end)

target("UnitTests", function(target)
	add_deps("flashlightTest-dummy")

	add_files("Source/**.cpp")

	for _, ext in ipairs({ ".hpp", ".inl" }) do
		add_headerfiles("Include/**" .. ext .. ")")
	end

	add_includedirs("Include")

	add_deps("FlashlightEngine")

	add_packages("catch2")

	add_cxxflags("cl::/wd4251")

	if is_plat("linux") then
		add_syslinks("dl")
	end
end)
