#include "AssetExplorerFile.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <stdexcept>
#endif

WAssetExplorerFile::WAssetExplorerFile(UPtr<WAssetExplorer> const& assetExplorer, UPath const& path)
	: Base(assetExplorer, path
        , [](UPath const& path)
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
