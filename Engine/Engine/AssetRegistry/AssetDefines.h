#pragma once

/*
 * Internal asset: An asset that is made in engine and modified in engine. No external files are managed for this asset type.
 * 
 * External asset: These are assets that are imported to the engine. Most of the time they are converted to a binary format the 
 * engine can read faster but it also stores the last modification time so if the asset gets modified outside the engine, we will automatically reimport it next time.
 */

// This is the path to the internal asset on disk, or cached version of external asset.
using CachePath = std::filesystem::path;

// This would be the path to the external asset if it is such, otherwise it is just a unique identified to the internal asset, usually just the name of the asset.
using SourcePath = std::filesystem::path;