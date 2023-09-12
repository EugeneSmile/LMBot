#ifndef LMBOT_CONFIGWRAPPER_H_GUARD
#define LMBOT_CONFIGWRAPPER_H_GUARD

#include <string>
#include <vector>
#include <memory>
#include <concepts>
#include <type_traits>
#include <any>
#include <variant>
#include <unordered_map>
#include <fstream>

#include <toml++/toml.h>
#include <spdlog/spdlog.h>

#include "Bot.h"

namespace lmbot
{
    template <typename T, typename... U>
    concept IsAnyOf = (std::same_as<T, U> || ...);

    template <typename T>
    concept IsInteger = IsAnyOf<T, std::uint64_t, std::int64_t, std::uint32_t, std::int32_t, std::uint16_t, std::int16_t, std::uint8_t, std::int8_t>;

    template <typename T>
    concept IsFloat = IsAnyOf<T, double, float>;

    template <typename T>
    concept IsString = IsAnyOf<T, std::string, const std::string, char *, const char *>;

    template <typename T>
    concept IsBool = IsAnyOf<T, bool>;

    template <typename T>
    concept IsIntegerVector = IsAnyOf<T, std::vector<std::uint64_t>, std::vector<std::int64_t>, std::vector<std::uint32_t>, std::vector<std::int32_t>, std::vector<std::uint16_t>, std::vector<std::int16_t>, std::vector<std::uint8_t>, std::vector<std::int8_t>>;

    template <typename T>
    concept IsFloatVector = IsAnyOf<T, std::vector<double>, std::vector<float>>;

    template <typename T>
    concept IsStringVector = IsAnyOf<T, std::vector<std::string>, std::vector<const std::string>, std::vector<char *>, std::vector<const char *>>;

    template <typename T>
    concept IsBoolVector = IsAnyOf<T, std::vector<bool>>;

    template <typename T, std::enable_if_t<std::is_same_v<T, std::int64_t> || std::is_same_v<T, double> || std::is_same_v<T, std::string> || std::is_same_v<T, bool> || std::is_same_v<T, std::vector<int64_t>> || std::is_same_v<T, std::vector<double>> || std::is_same_v<T, std::vector<std::string>> || std::is_same_v<T, std::vector<bool>>, bool> = true>
    class ConfigParameter
    {
    private:
        std::shared_ptr<T> value;

    public:
        ConfigParameter(T value) : value(std::make_shared<T>(value)){};

        operator T() const { return *value; };
        operator std::shared_ptr<T>() const { return value; };
    };

    class ConfigWrapper : public bot::Bot
    {
    private:
        std::string filename;
        toml::table config;
        std::unordered_map<std::string, std::any> parameters;

        void parseParameters(toml::table *table, std::string_view prefix = "");

    public:
        explicit ConfigWrapper(const std::string &config_filename);
        ~ConfigWrapper();
        void stop();

        template <typename T>
            requires IsInteger<T>
        int64_t getParameter(const std::string &name, T default_value)
        {
            if (parameters.find(name) == parameters.end())
                parameters.insert_or_assign(name, ConfigParameter<int64_t>{default_value});
            return int64_t(std::any_cast<ConfigParameter<int64_t>>(parameters[name]));
        }
        template <typename T>
            requires IsFloat<T>
        double getParameter(const std::string &name, T default_value)
        {
            if (parameters.find(name) == parameters.end())
                parameters.insert_or_assign(name, ConfigParameter<double>{default_value});
            return double(std::any_cast<ConfigParameter<double>>(parameters[name]));
        }
        template <typename T>
            requires IsString<T>
        std::string getParameter(const std::string &name, T default_value)
        {
            if (parameters.find(name) == parameters.end())
                parameters.insert_or_assign(name, ConfigParameter<std::string>{default_value});
            return std::string(std::any_cast<ConfigParameter<std::string>>(parameters[name]));
        }
        template <typename T>
            requires IsBool<T>
        bool getParameter(const std::string &name, T default_value)
        {
            if (parameters.find(name) == parameters.end())
                parameters.insert_or_assign(name, ConfigParameter<bool>{default_value});
            return bool(std::any_cast<ConfigParameter<bool>>(parameters[name]));
        }
        template <typename T>
            requires IsIntegerVector<T>
        std::vector<int64_t> getParameter(const std::string &name, T default_value)
        {
            if (parameters.find(name) == parameters.end())
                parameters.insert_or_assign(name, ConfigParameter<std::vector<int64_t>>{default_value});
            return std::vector<int64_t>(std::any_cast<ConfigParameter<std::vector<int64_t>>>(parameters[name]));
        }
        template <typename T>
            requires IsFloatVector<T>
        std::vector<double> getParameter(const std::string &name, T default_value)
        {
            if (parameters.find(name) == parameters.end())
                parameters.insert_or_assign(name, ConfigParameter<std::vector<double>>{default_value});
            return std::vector<double>(std::any_cast<ConfigParameter<std::vector<double>>>(parameters[name]));
        }
        template <typename T>
            requires IsStringVector<T>
        std::vector<std::string> getParameter(const std::string &name, T default_value)
        {
            if (parameters.find(name) == parameters.end())
                parameters.insert_or_assign(name, ConfigParameter<std::vector<std::string>>{default_value});
            return std::vector<std::string>(std::any_cast<ConfigParameter<std::vector<std::string>>>(parameters[name]));
        }
        template <typename T>
            requires IsBoolVector<T>
        std::vector<bool> getParameter(const std::string &name, T default_value)
        {
            if (parameters.find(name) == parameters.end())
                parameters.insert_or_assign(name, ConfigParameter<std::vector<bool>>{default_value});
            return std::vector<bool>(std::any_cast<ConfigParameter<std::vector<bool>>>(parameters[name]));
        }

        template <typename T>
        void setParameter(const std::string &name, T value)
        {
            config.insert_or_assign(name, value);
        }
    };
} // namespace lmbot
#endif