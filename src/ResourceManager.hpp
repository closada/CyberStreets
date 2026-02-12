#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>


template <typename Resource, typename ID>
class ResourceManager
{
public:
    void load(ID id, const std::string& filename)
    {
        Resource resource;
        if (!resource.loadFromFile(filename))
            throw std::runtime_error("No se pudo cargar recurso");

        resources[id] = std::move(resource);
    }

    Resource& get(ID id)
    {
        return resources.at(id);
    }

private:
    std::unordered_map<ID, Resource> resources;
};

