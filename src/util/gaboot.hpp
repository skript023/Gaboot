#pragma once

namespace gaboot::util
{
	inline std::string get_env(std::string const& key)
	{
        std::ifstream env_file(".env");
        if (!env_file.is_open())
        {
            std::cerr << "Unable to open .env file" << std::endl;
            return {};
        }

        std::map<std::string, std::string> variables;

        for (std::string line; std::getline(env_file, line);)
        {
            size_t found = line.find('=');
            if (found != std::string::npos)
            {
                std::string key = line.substr(0, found);
                std::string value = line.substr(found + 1);
                variables[key] = value;
            }
        }

        env_file.close();

        return variables[key];
	}
}