#include "ConfigWrapper.h"

#include <iostream>

#include "LMBot.h"

namespace lmbot
{
    ConfigWrapper::ConfigWrapper(const std::string &config_filename) : filename(config_filename)
    {
        logger = bot->logger->registerLogger("ConfigWrapper");
        try
        {
            config = toml::parse_file(config_filename);
            parseParameters(&config);
        }
        catch (const toml::parse_error &error)
        {
            logger->error("Config parse error: {} / {}", error.what(), error.description());
        }
    }

    ConfigWrapper::~ConfigWrapper()
    {
    }

    void ConfigWrapper::stop()
    {
    }

    void ConfigWrapper::parseParameters(toml::table *table, std::string_view prefix)
    {
        for (auto &&[key, node] : *table)
        {
            std::string full_key = std::string(prefix) + (prefix.empty() ? "" : ".") + std::string(key.str());
            switch (node.type())
            {
            case toml::node_type::table:
                parseParameters(node.as_table(), full_key);
                break;

            case toml::node_type::array:
                if (!node.as_array()->empty())
                {
                    if (node.as_array()->is_homogeneous())
                    {
                        switch (node.as_array()->begin()->type())
                        {
                        case toml::node_type::boolean:
                        {
                            std::vector<bool> vec(node.as_array()->size());
                            for (size_t i = 0; i < node.as_array()->size(); ++i)
                                vec.at(i) = **node.as_array()->at(i).as_boolean();
                            parameters.insert_or_assign(full_key, ConfigParameter<std::vector<bool>>(vec));
                            break;
                        }

                        case toml::node_type::integer:
                        {
                            std::vector<int64_t> vec(node.as_array()->size());
                            for (size_t i = 0; i < node.as_array()->size(); ++i)
                                vec.at(i) = **node.as_array()->at(i).as_integer();
                            parameters.insert_or_assign(full_key, ConfigParameter<std::vector<int64_t>>(vec));
                            break;
                        }

                        case toml::node_type::floating_point:
                        {
                            std::vector<double> vec(node.as_array()->size());
                            for (size_t i = 0; i < node.as_array()->size(); ++i)
                                vec.at(i) = **node.as_array()->at(i).as_floating_point();
                            parameters.insert_or_assign(full_key, ConfigParameter<std::vector<double>>(vec));
                            break;
                        }

                        case toml::node_type::string:
                        {
                            std::vector<std::string> vec(node.as_array()->size());
                            for (size_t i = 0; i < node.as_array()->size(); ++i)
                                vec.at(i) = **node.as_array()->at(i).as_string();
                            parameters.insert_or_assign(full_key, ConfigParameter<std::vector<std::string>>(vec));
                            break;
                        }

                        default:
                            break;
                        }
                    }
                    else
                    {
                        logger->debug("Key {} is non-homogeneous array", full_key);
                    }
                }
                else
                {
                    logger->debug("Key {} is EMPTY non-homogeneous array", full_key);
                }
                break;

            case toml::node_type::boolean:
                parameters.insert_or_assign(full_key, ConfigParameter<bool>{**node.as_boolean()});
                break;

            case toml::node_type::integer:
                parameters.insert_or_assign(full_key, ConfigParameter<int64_t>{**node.as_integer()});
                break;

            case toml::node_type::floating_point:
                parameters.insert_or_assign(full_key, ConfigParameter<double>{**node.as_floating_point()});
                break;

            case toml::node_type::string:
                parameters.insert_or_assign(full_key, ConfigParameter<std::string>{**node.as_string()});
                break;

            default:
                break;
            }
        }
    }
} // namespace lmbot