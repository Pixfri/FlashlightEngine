local headerTemplate, inlineTemplate, sourceTemplate

task("create-class")

set_menu({
	usage = "xmake create-class [options] name",
	description = "Task to help class creation.",
	options = {
		{ nil, "nocpp", "k", nil, "Set this to create a header-only class." },
		{ nil, "name", "v", nil, "Class name" },
	},
})

local project = ProjectName

on_run(function()
	import("core.base.option")

	local classPath = option.get("name")
	if not classPath then
		os.raise("Missing class name")
	end

	local className = path.basename(classPath)

	local files = {
		{ TargetPath = path.join("Include", project, classPath) .. ".hpp", Template = headerTemplate },
		{ TargetPath = path.join("Include", project, classPath) .. ".inl", Template = inlineTemplate },
	}

	if not option.get("nocpp") then
		table.insert(
			files,
			{ TargetPath = path.join("Source", project, classPath) .. ".cpp", Template = sourceTemplate }
		)
	end

	local replacements = {
		CLASS_NAME = className,
		CLASS_PATH = classPath,
		COPYRIGHT = os.date("%Y") .. [[ Jean "Pixfri" Letessier ]],
		HEADER_GUARD = "FL_" .. classPath:gsub("[/\\]", "_"):upper() .. "_HPP",
		PROJECT = project,
	}

	for _, file in pairs(files) do
		local content = file.Template:gsub("%%([%u_]+)%%", function(kw)
			local r = replacements[kw]
			if not r then
				os.raise("Missing replacement for " .. kw)
			end

			return r
		end)

		io.writefile(file.TargetPath, content)
	end
end)

headerTemplate = [[
// Copyright (C) %COPYRIGHT%
// This file is part of %PROJECT%.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#ifndef %HEADER_GUARD%
#define %HEADER_GUARD%

#include <FlashlightEngine/Prerequisites.hpp>
#include <FlashlightEngine/Core/BaseObject.hpp>

namespace Fl {
    class FL_API %CLASS_NAME% : public BaseObject {
    public:
        %CLASS_NAME%() = default;
        ~%CLASS_NAME%() = default;

        %CLASS_NAME%(const %CLASS_NAME%&) = delete;
        %CLASS_NAME%(%CLASS_NAME%&&) = delete;

        %CLASS_NAME%& operator=(const %CLASS_NAME%&) = delete;
        %CLASS_NAME%& operator=(%CLASS_NAME%&&) = delete;
    
    private:
    };
}

#include <%PROJECT%/%CLASS_PATH%.inl>

#endif // %HEADER_GUARD%
]]

inlineTemplate = [[
// Copyright (C) %COPYRIGHT%
// This file is part of %PROJECT%.
// For conditions of distribution and use, see copyright notice in LICENSE.

#pragma once

#include <%PROJECT%/%CLASS_PATH%.hpp>

namespace Fl {
    
}
]]

sourceTemplate = [[
// Copyright (C) %COPYRIGHT%
// This file is part of %PROJECT%.
// For conditions of distribution and use, see copyright notice in LICENSE.

#include <%PROJECT%/%CLASS_PATH%.hpp>

namespace Fl {

}
]]