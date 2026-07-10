#pragma once
#include <string>
#include <optional>



struct LaunchArgs
{
	std::optional<int> profileIndex = std::nullopt;
};


LaunchArgs parseArgs(int argc, char* argv[])
{
	LaunchArgs args;

	if (argc > 1)
	{
		for (int i = 1; i < argc; ++i)
		{
			std::string arg = argv[i];

			if (arg.starts_with("--profile="))
				args.profileIndex = std::stoi(arg.substr(std::string("--profile=").size()));
		}
	}

	return args;
}