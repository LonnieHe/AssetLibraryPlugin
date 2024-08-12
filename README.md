# AssetLibraryPlugin

## Description

This plugin provides a simple asset library interface for retrieving assets information and thumbnail.

## How to use

### Default listening port `7788`

`HostName:7788/AssetLibrary`

When the editor starts, it has a built-in server listening port and uses `POST` for communication.



### Example

#### 1. Get asset Dependencies

```json
{
  "Type": "Info",
  "PackagePath": "/AssetLibrary/Surface/Stone_Tiles_Facade_veqldaqo/MI_StoneTileFacade.MI_StoneTileFacade"
}
```
![Info](./pic/Info.png)

#### 2. Get asset Thumbnail

```json
{
  "Type": "Thumbnail",
  "PackagePath": "/AssetLibrary/Surface/Stone_Tiles_Facade_veqldaqo/MI_StoneTileFacade.MI_StoneTileFacade"
}
```
![Thumbnail](./pic/Thumbnail.png)

#### 3. Get Project Path

```json
{
  "Type": "Path"
}
```
![Path](./pic/Path.png)

TODO
- More query methods

![Todo](./pic/UeWebServer.png)