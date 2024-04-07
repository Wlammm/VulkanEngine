#pragma once

class AssetObserver
{
public:
	virtual ~AssetObserver() = default;
	virtual void OnAssetUpdated() = 0;
};