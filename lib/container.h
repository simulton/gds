#pragma once

#include <cassert>
#include <type_traits>
#include <variant>
#include <list>
#include <map>
#include <vector>

namespace Gds
{

    class Container
    {
    public:
        enum Type {
            BoolType,
            IntType,
            DoubleType,
            StringType,
            ContainerType
        };

        using Value     = std::variant<bool, int, double, std::string, Container>;
        using Entry     = std::pair< Type, Value >;
        using Entries   = std::multimap< std::string, Entry >;
        using Argument  = std::pair< std::string, std::string >;
        using Arguments = std::list< Argument >;

        // Helper function to retrieve the index of the type T in the the VariantType
        template<class VariantType, class T, std::size_t index = 0>
        static constexpr std::size_t variant_index()
        {
            if constexpr (index == std::variant_size_v<VariantType>) {
                return index;
            }

            else if constexpr (std::is_same_v<std::variant_alternative_t<index, VariantType>, T>) {
                return index;
            }

            else {
                return variant_index<VariantType, T, index + 1>();
            }
        }

        Container(const std::string& name = std::string()) :
            name(name)
        {
        }

        void addEntry(const std::string& name, Type type, const Value& value)
        {
            entries.emplace(name, std::make_pair(type, value));
        }

        void addEntry(const std::string& name, bool value)
        {
            addEntry(name, BoolType, value);
        }

        void addEntry(const std::string& name, int value)
        {
            addEntry(name, IntType, value);
        }

        void addEntry(const std::string& name, double value)
        {
            addEntry(name, DoubleType, value);
        }

        void addEntry(const std::string& name, const std::string& value)
        {
            addEntry(name, StringType, value);
        }

        void addEntry(const std::string& name, const Container& value)
        {
            this->name = name;
            addEntry(name, ContainerType, value);
        }

        template<class T>
        T getEntry(const std::string& name) const
        {
            auto it = entries.find( name )->second.second;
            const std::size_t& typeIndexVariant = it.index();
            std::size_t typeIndexTemplate = static_cast<std::size_t>( variant_index<Value, T>() );

            // Ensure that T and the variant type are the same
            if ( typeIndexVariant != typeIndexTemplate ) {
                return T();
            }

            return std::get<T>( it );
        }

        template<class T>
        std::vector<T> getEntries(const std::string& name) const
        {
            const auto& range = entries.equal_range( name );
            std::vector<T> entries( std::distance( range.first, range.second ) );
            for (auto it = range.first; it != range.second; it++) {
                entries[ std::distance( range.first, it ) ] = std::get<T>(it->second.second);
            }

            return entries;
        }

        void addArgument(const std::string& key, const std::string& value)
        {
            arguments.emplace_back(key, value);
        }

        bool isList() const
        {
            for (const auto& it : entries ) {
                if (it.second.first != ContainerType) {
                    return false;
                }
            }

            return true;
        }

        std::string name;
        Arguments arguments;
        Entries entries;
    };

}