# AssetLibraryPlugin

## Description

This plugin provides a simple asset library interface for retrieving assets information and thumbnail.

## How to use

### Default port `7788`
When the editor starts, a built-in server listening port.

#### There are three `GET` routers:
- /Info 
- /Thumbnail
- /Path

#### A `PUT` router：

- /PicToMaterial



## Example

### 1. Get asset Dependencies
Set `value` as `PackageName`.
>127.0.0.1:7788/Info?Name=/Game/AssetLibrary/Surface/Stone_Tiles_Facade_veqldaqo/MI_StoneTileFacade


![Info](./pic/Info.png)

### 2. Get asset Thumbnail
`key` can be set as `cache` or `render`:

`cache` - Default. Read static invariant files from .uasset

`render` - Rerender thumbnail from asset. There is a delay, and it may be necessary to open material editing to obtain the latest information

>127.0.0.1:7788/Thumbnail?cache=/Game/AssetLibrary/Surface/ClusterMaterial/M_ClusterGrid_Hex

![Thumbnail](./pic/Thumbnail.png)

### 3. Get Project Path

>127.0.0.1:7788/Path

![Path](./pic/Path.png)

### 4. Use in editor

![easy](./pic/tutorial2.png)

### 5. Create Material form URL

![CreateM](./pic/Giftutorial.gif)


`AssetName` is preferably equal to the folder name.

If using ARD, it is necessary to set `UseARD` to true and provide the ARD address, and `AO` \ `Height` \ `Roughness` is no longer required.

![Create](./pic/image.png)

At present, this method **ignores multi-level directories**, and each material occupies a folder in `ImportedMaterial`

![CreateMaterial](./pic/CreateMaterial.png)

now defalut formart is SRGBA, other format texture may led color error.
s

![Todo](./pic/UeWebServer.png)
