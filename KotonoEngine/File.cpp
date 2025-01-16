#include "File.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

File::File(const std::filesystem::path& path) :
    _path(path)
{
    //_document.Parse(GetContent().c_str());
}

const std::filesystem::path& File::GetPath() const
{
    return _path;
}

const std::filesystem::path File::GetDirectory() const
{
    return _path.parent_path();
}

const std::string File::GetName() const
{
    return _path.filename().string();
}

const std::string File::GetExtension() const
{
    return _path.extension().string();
}

const std::string File::GetStem() const
{
    return _path.stem().string();
}

const std::string File::GetContent() const
{
    // Check if path exists.
    if (!std::filesystem::exists(_path))
    {
        std::cerr << "Failed to find a file at '" << _path << '\'' << std::endl;
        return "";
    }

    // Open file
    std::ifstream file(_path);
    
    if (!file.is_open())
    {
        std::cerr << "Failed to open the file at '" << _path << '\'' << std::endl;
        return "";
    }

    // Read file
    std::ostringstream fileContents;
    fileContents << file.rdbuf();
    return fileContents.str();
}

const std::unordered_map<std::string, const std::type_info&> extensionMap =
{
    {".txt", typeid(std::string)}/*,
    {".jpg", typeid(ImageTexture)},
    {".jpeg", typeid(ImageTexture)},
    {".png", typeid(ImageTexture)},
    {".bmp", typeid(ImageTexture)}*/
};

const std::type_info& File::GetType() const
{
    auto it = extensionMap.find(GetExtension());
    if (it != extensionMap.end())
    {
        return it->second; 
    }

    return typeid(void);  
}

const std::filesystem::path File::GetAssetPath() const
{
    return "";//return _document["assetPath"].GetString();
}

template<class T>
const bool File::IsOfType() const
{
    return typeid(T) == GetType();
}

template<class T>
T* File::Instantiate() const
{
    if (IsOfType<T>())
    {
        return new T();  
    }

    return nullptr; 
}