# AssetLibraryPlugin

## Description

This plugin provides a simple asset library interface for retrieving assets information and thumbnail.

## How to use

### Default listening port `7788`

When the editor starts, it has a built-in server listening port and uses `GET` for communication.

There are three routers:
- /Info 
- /Thumbnail
- /Path



## Example
Set `value` as `PackageName`.
### 1. Get asset Dependencies

>127.0.0.1:7788/Info?Name=/AssetLibrary/Surface/Stone_Tiles_Facade_veqldaqo/MI_StoneTileFacade


![Info](./pic/Info.png)

### 2. Get asset Thumbnail
`key` can be set 

as `cache` (Default. Read static invariant files from .uasset)

or `render`(Rerender thumbnail from asset. There is a delay, and it may be necessary to open material editing to obtain the latest information)

>127.0.0.1:7788/Thumbnail?cache=/Game/AssetLibrary/Surface/ClusterMaterial/M_ClusterGrid_Hex

![Thumbnail](./pic/Thumbnail.png)

### 3. Get Project Path

>127.0.0.1:7788/Path

![Path](./pic/Path.png)

TODO
- More query methods

![Todo](./pic/UeWebServer.png)