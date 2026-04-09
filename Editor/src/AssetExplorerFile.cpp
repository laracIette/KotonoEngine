#include "AssetExplorerFile.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <stdexcept>
#endif

WAssetExplorerFile::WAssetExplorerFile(const UPath& path)
	: WAssetExplorerItem(path, 
        [](const UPath& path)
        {
            const auto string{ std::format("\"{0}\"", path.ToString()) };

#           ifdef _WIN32
                ShellExecute(0, 0, string.c_str(), 0, 0, SW_SHOW);
#           else
                throw std::exception("Open file not implemented for your OS.");
#           endif
        }
    )
{
}

#include "generated/AssetExplorerFile.generated.inl"
