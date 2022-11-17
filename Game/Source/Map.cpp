#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    mapFileName = config.child("mapfile").attribute("path").as_string();
    mapFolder = config.child("mapfolder").attribute("path").as_string();

    //Initialize the path
    frontier.Push(iPoint(19, 4), 0);
    visited.Add(iPoint(19, 4));
    breadcrumbs.Add(iPoint(19, 4));

    // L09 DONE 4: Initialize destination point
    destination = iPoint(0, 20);

    return ret;
}

void Map::PropagateDijkstra()
{
    // L10: TODO 3: Taking BFS as a reference, implement the Dijkstra algorithm
    // use the 2 dimensional array "costSoFar" to track the accumulated costs
    // on each cell (is already reset to 0 automatically)


}

void Map::Draw()
{
    if (mapLoaded == false)
        return;

    /*
    // L04: DONE 6: Iterate all tilesets and draw all their
    // images in 0,0 (you should have only one tileset for now)

    ListItem<TileSet*>* tileset;
    tileset = mapData.tilesets.start;

    while (tileset != NULL) {
        app->render->DrawTexture(tileset->data->texture,0,0);
        tileset = tileset->next;
    }
    */

    // L05: DONE 5: Prepare the loop to draw all tiles in a layer + DrawTexture()

    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;

    while (mapLayerItem != NULL) {

        //L06: DONE 7: use GetProperty method to ask each layer if your “Draw” property is true.
        if (mapLayerItem->data->properties.GetProperty("Draw") != NULL && mapLayerItem->data->properties.GetProperty("Draw")->value) {

            for (int x = 0; x < mapLayerItem->data->width; x++)
            {
                for (int y = 0; y < mapLayerItem->data->height; y++)
                {
                    // L05: DONE 9: Complete the draw function
                    int gid = mapLayerItem->data->Get(x, y);

                    //L06: DONE 3: Obtain the tile set using GetTilesetFromTileId
                    TileSet* tileset = GetTilesetFromTileId(gid);

                    SDL_Rect r = tileset->GetTileRect(gid);
                    iPoint pos = MapToWorld(x, y);

                    app->render->DrawTexture(tileset->texture,
                        pos.x,
                        pos.y,
                        &r);
                }
            }
        }
        mapLayerItem = mapLayerItem->next;

    }
}



// L05: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
    iPoint ret;

    ret.x = x * mapData.tileWidth;
    ret.y = y * mapData.tileHeight;

    return ret;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int gid) const
{
    SDL_Rect rect = { 0 };
    int relativeIndex = gid - firstgid;

    // L05: DONE 7: Get relative Tile rectangle
    rect.w = tileWidth;
    rect.h = tileHeight;
    rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
    rect.y = margin + (tileWidth + spacing) * (relativeIndex / columns);

    return rect;
}


// L06: DONE 2: Pick the right Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int gid) const
{
    ListItem<TileSet*>* item = mapData.tilesets.start;
    TileSet* set = NULL;

    while (item)
    {
        set = item->data;
        if (gid < (item->data->firstgid + item->data->tilecount))
        {
            break;
        }
        item = item->next;
    }

    return set;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // L04: DONE 2: Make sure you clean up any memory allocated from tilesets/map
    ListItem<TileSet*>* item;
    item = mapData.tilesets.start;

    while (item != NULL)
    {
        RELEASE(item->data);
        item = item->next;
    }
    mapData.tilesets.Clear();

    // L05: DONE 2: clean up all layer data
    // Remove all layers
    ListItem<MapLayer*>* layerItem;
    layerItem = mapData.maplayers.start;

    while (layerItem != NULL)
    {
        RELEASE(layerItem->data);
        layerItem = layerItem->next;
    }

    return true;
}

// Load new map
bool Map::Load()
{
    bool ret = true;

    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(mapFileName.GetString());

    if (result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", mapFileName, result.description());
        ret = false;
    }

    if (ret == true)
    {
        ret = LoadMap(mapFileXML);
    }

    if (ret == true)
    {
        ret = LoadTileSet(mapFileXML);
    }

    // L05: DONE 4: Iterate all layers and load each of them
    if (ret == true)
    {
        ret = LoadAllLayers(mapFileXML.child("map"));
    }

    // L07 DONE 3: Create colliders
    // Later you can create a function here to load and create the colliders from the map

    // Els dos últims són les seves mesures! El primer és lo llarg que és i l'últim l'amplada!
    // El primer són les x quan més gran més cap a la dreta, el segon són segurament les y. Quan més baixes siguin les y més cap adalt. 
    // -33 one block upwards/to the left



    //Ground
    PhysBody* c1 = app->physics->CreateRectangle(500, 704 + 32, 1000, 64, STATIC);
    // L07 DONE 7: Assign collider type
    c1->ctype = ColliderType::PLATFORM;

    //WallL
    PhysBody* c2 = app->physics->CreateRectangle(25, 400, 75, 800, STATIC);
    // L07 DONE 7: Assign collider type
    c2->ctype = ColliderType::PLATFORM;

    //Ceiling
    PhysBody* c3 = app->physics->CreateRectangle(550, 30, 1000, 64, STATIC);
    // L07 DONE 7: Assign collider type
    c3->ctype = ColliderType::PLATFORM;

    //Ladder1

    PhysBody* c4 = app->physics->CreateRectangle(314 + 16, 687, 50+30, 30, STATIC);
    // L07 DONE 7: Assign collider type
    c4->ctype = ColliderType::PLATFORM;

   // Ladder2

    PhysBody* c5 = app->physics->CreateRectangle(345+17, 655, 50+28, 30, STATIC);
    // L07 DONE 7: Assign collider type
    c5->ctype = ColliderType::PLATFORM;

    //Ladder3

    PhysBody* c6 = app->physics->CreateRectangle(384, 625, 60, 30, STATIC);
    // L07 DONE 7: Assign collider type
    c6->ctype = ColliderType::PLATFORM;

    //Platform 1

    PhysBody* c7 = app->physics->CreateRectangle(255, 591, 130, 32, STATIC);
    // L07 DONE 7: Assign collider type
    c7->ctype = ColliderType::PLATFORM;

    //Platform 2

    PhysBody* c8 = app->physics->CreateRectangle(255-126, 591-30, 130-66, 32, STATIC);
    // L07 DONE 7: Assign collider type
    c8->ctype = ColliderType::PLATFORM;


    //Platform 3

    PhysBody* c9 = app->physics->CreateRectangle(255-33, 591 - 63, 130 - 66, 32, STATIC);
    // L07 DONE 7: Assign collider type
    c9->ctype = ColliderType::PLATFORM;

    //Platform 4

    PhysBody* c10 = app->physics->CreateRectangle(255 + 33, 591 - 95, 130 - 66, 32, STATIC);
    // L07 DONE 7: Assign collider type
    c10->ctype = ColliderType::PLATFORM;


    //Platform 5

    PhysBody* c11 = app->physics->CreateRectangle(255 + 95, 591 - 125, 130 - 66, 32, STATIC);
    // L07 DONE 7: Assign collider type
    c11->ctype = ColliderType::PLATFORM;

    //Platform 6

    PhysBody* c12 = app->physics->CreateRectangle(255, 591 - 157, 130 - 66, 32, STATIC);
    // L07 DONE 7: Assign collider type
    c12->ctype = ColliderType::PLATFORM;

    //Platform 7

    PhysBody* c13 = app->physics->CreateRectangle(255 - 63, 591 - 190, 130 - 66, 32, STATIC);
    // L07 DONE 7: Assign collider type
    c13->ctype = ColliderType::PLATFORM;

    //Platform 8

    PhysBody* c14 = app->physics->CreateRectangle(255 - 126, 591 - 223, 130 - 66, 32, STATIC);
    // L07 DONE 7: Assign collider type
    c14->ctype = ColliderType::PLATFORM;

    //Platform 9

    PhysBody* c15 = app->physics->CreateRectangle(255 - 15, 591 - 256, 130 - 35, 32, STATIC);
    // L07 DONE 7: Assign collider type
    c15->ctype = ColliderType::PLATFORM;


    //Platform 10

    PhysBody* c16 = app->physics->CreateRectangle(255 + 63, 591 - 289, 130 - 66, 32, STATIC);
    // L07 DONE 7: Assign collider type
    c16->ctype = ColliderType::PLATFORM;

    //Platform 11

    PhysBody* c17 = app->physics->CreateRectangle(255 + 96, 591 - 319, 130 - 66, 32, STATIC);
    // L07 DONE 7: Assign collider type
    c17->ctype = ColliderType::PLATFORM;


    //Platform 12

    PhysBody* c18 = app->physics->CreateRectangle(255 + 210, 591 - 350, 130 + 31, 32, STATIC);
    // L07 DONE 7: Assign collider type
    c18->ctype = ColliderType::PLATFORM;

    //Platform 12 (just in case)

    PhysBody* c19 = app->physics->CreateRectangle(255 + 210, 591 - 317, 130 + 31, 32, STATIC);
    // L07 DONE 7: Assign collider type
    c19->ctype = ColliderType::PLATFORM;

    //WallR
    PhysBody* c20 = app->physics->CreateRectangle(25 + 1000, 400, 75, 800, STATIC);
    // L07 DONE 7: Assign collider type
    c20->ctype = ColliderType::PLATFORM;


    if (ret == true)
    {
        // L04: DONE 5: LOG all the data loaded iterate all tilesets and LOG everything

        LOG("Successfully parsed map XML file :%s", mapFileName.GetString());
        LOG("width : %d height : %d", mapData.width, mapData.height);
        LOG("tile_width : %d tile_height : %d", mapData.tileWidth, mapData.tileHeight);

        LOG("Tilesets----");

        ListItem<TileSet*>* tileset;
        tileset = mapData.tilesets.start;

        while (tileset != NULL) {
            LOG("name : %s firstgid : %d", tileset->data->name.GetString(), tileset->data->firstgid);
            LOG("tile width : %d tile height : %d", tileset->data->tileWidth, tileset->data->tileHeight);
            LOG("spacing : %d margin : %d", tileset->data->spacing, tileset->data->margin);
            tileset = tileset->next;
        }

        // L05: DONE 4: LOG the info for each loaded layer
        ListItem<MapLayer*>* mapLayer;
        mapLayer = mapData.maplayers.start;

        while (mapLayer != NULL) {
            LOG("id : %d name : %s", mapLayer->data->id, mapLayer->data->name.GetString());
            LOG("Layer width : %d Layer height : %d", mapLayer->data->width, mapLayer->data->height);
            mapLayer = mapLayer->next;
        }
    }

    if (mapFileXML) mapFileXML.reset();

    mapLoaded = ret;

    return ret;
}

// L04: DONE 3: Implement LoadMap to load the map properties
bool Map::LoadMap(pugi::xml_node mapFile)
{
    bool ret = true;
    pugi::xml_node map = mapFile.child("map");

    if (map == NULL)
    {
        LOG("Error parsing map xml file: Cannot find 'map' tag.");
        ret = false;
    }
    else
    {
        //Load map general properties
        mapData.height = map.attribute("height").as_int();
        mapData.width = map.attribute("width").as_int();
        mapData.tileHeight = map.attribute("tileheight").as_int();
        mapData.tileWidth = map.attribute("tilewidth").as_int();
    }

    return ret;
}

// L04: DONE 4: Implement the LoadTileSet function to load the tileset properties
bool Map::LoadTileSet(pugi::xml_node mapFile) {

    bool ret = true;

    pugi::xml_node tileset;
    for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
    {
        TileSet* set = new TileSet();

        // L04: DONE 4: Load Tileset attributes
        set->name = tileset.attribute("name").as_string();
        set->firstgid = tileset.attribute("firstgid").as_int();
        set->margin = tileset.attribute("margin").as_int();
        set->spacing = tileset.attribute("spacing").as_int();
        set->tileWidth = tileset.attribute("tilewidth").as_int();
        set->tileHeight = tileset.attribute("tileheight").as_int();
        set->columns = tileset.attribute("columns").as_int();
        set->tilecount = tileset.attribute("tilecount").as_int();

        // L04: DONE 4: Load Tileset image
        SString tmp("%s%s", mapFolder.GetString(), tileset.child("image").attribute("source").as_string());
        set->texture = app->tex->Load(tmp.GetString());

        mapData.tilesets.Add(set);
    }

    return ret;
}

// L05: DONE 3: Implement a function that loads a single layer layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
    bool ret = true;

    //Load the attributes
    layer->id = node.attribute("id").as_int();
    layer->name = node.attribute("name").as_string();
    layer->width = node.attribute("width").as_int();
    layer->height = node.attribute("height").as_int();

    //L06: DONE 6 Call Load Propoerties
    LoadProperties(node, layer->properties);

    //Reserve the memory for the data 
    layer->data = new uint[layer->width * layer->height];
    memset(layer->data, 0, layer->width * layer->height);

    //Iterate over all the tiles and assign the values
    pugi::xml_node tile;
    int i = 0;
    for (tile = node.child("data").child("tile"); tile && ret; tile = tile.next_sibling("tile"))
    {
        layer->data[i] = tile.attribute("gid").as_int();
        i++;
    }

    return ret;
}

// L05: DONE 4: Iterate all layers and load each of them
bool Map::LoadAllLayers(pugi::xml_node mapNode) {
    bool ret = true;

    for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode && ret; layerNode = layerNode.next_sibling("layer"))
    {
        //Load the layer
        MapLayer* mapLayer = new MapLayer();
        ret = LoadLayer(layerNode, mapLayer);

        //add the layer to the map
        mapData.maplayers.Add(mapLayer);
    }

    return ret;
}

// L06: DONE 6: Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
    bool ret = false;

    for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
    {
        Properties::Property* p = new Properties::Property();
        p->name = propertieNode.attribute("name").as_string();
        p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

        properties.list.Add(p);
    }

    return ret;
}


// L06: DONE 7: Ask for the value of a custom property
Properties::Property* Properties::GetProperty(const char* name)
{
    ListItem<Property*>* item = list.start;
    Property* p = NULL;

    while (item)
    {
        if (item->data->name == name) {
            p = item->data;
            break;
        }
        item = item->next;
    }

    return p;
}


bool Map::IsWalkable(int x, int y) const
{
    bool isWalkable = false;

    // L09: DONE 3: return true only if x and y are within map limits
    // and the tile is walkable (tile id 0 in the navigation layer)

    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;
    MapLayer* navigationLayer = mapLayerItem->data;

    //Search the layer in the map that contains information for navigation
    while (mapLayerItem != NULL) {

        if (mapLayerItem->data->properties.GetProperty("Navigation") != NULL && mapLayerItem->data->properties.GetProperty("Navigation")->value) {
            navigationLayer = mapLayerItem->data;
        }

        mapLayerItem = mapLayerItem->next;
    }

    //Set isWalkable to true if the position is inside map limits and is a position that is not blocked in the navigation layer
    if (x >= 0 && y >= 0 && x < mapData.width && y < mapData.height && navigationLayer->Get(x, y) != 26) {
        isWalkable = true;
    }

    return isWalkable;
}

void Map::PropagateBFS()
{
    // L09: DONE 1: If frontier queue contains elements
    // pop the last one and calculate its 4 neighbors
    iPoint currentTile;
    bool foundDestination = false;

    // L09 DONE 4: Check if we have reach a destination
    if (frontier.Count() > 0) {
        iPoint frontierPoint = *(frontier.Peek(0));
        if (frontierPoint == destination) {
            foundDestination = true;
            ComputePath(destination.x, destination.y);
        }
    }

    if (!foundDestination && frontier.Pop(currentTile))
    {

        List<iPoint> neighbors;
        if (IsWalkable(currentTile.x + 1, currentTile.y)) {
            iPoint p;
            neighbors.Add(p.Create(currentTile.x + 1, currentTile.y));
        }
        if (IsWalkable(currentTile.x, currentTile.y + 1)) {
            iPoint p;
            neighbors.Add(p.Create(currentTile.x, currentTile.y + 1));
        }
        if (IsWalkable(currentTile.x - 1, currentTile.y)) {
            iPoint p;
            neighbors.Add(p.Create(currentTile.x - 1, currentTile.y));
        }
        if (IsWalkable(currentTile.x, currentTile.y - 1)) {
            iPoint p;
            neighbors.Add(p.Create(currentTile.x, currentTile.y - 1));
        }

        // L09: DONE 2: For each neighbor, if not visited, add it
        // to the frontier queue and visited list
        ListItem<iPoint>* item = neighbors.start;

        while (item != NULL)
        {
            if (visited.Find(item->data) == -1)
            {
                frontier.Push(item->data, 0);
                visited.Add(item->data);

                // L10: TODO 1: Record the direction to the previous node 
                // with the new list "breadcrumps"

                breadcrumbs.Add(currentTile);
            }
            item = item->next;
        }

    }
}

void Map::ComputePath(int x, int y)
{
    path.Clear();
    iPoint goal = iPoint(x, y);

    // L10: TODO 2: Follow the breadcrumps to goal back to the origin
    // add each step into "path" dyn array (it will then draw automatically)

    path.PushBack(goal);

    while (visited.Find(goal) != -1 && goal != visited.start->data)
    {
        int index = visited.Find(goal);
        iPoint nodeIcomeFrom = breadcrumbs[index];
        path.PushBack(nodeIcomeFrom);
        goal = nodeIcomeFrom;
    }

}

void Map::PropagateDijkstra()
{
    // L10: TODO 3: Taking BFS as a reference, implement the Dijkstra algorithm
    // use the 2 dimensional array "costSoFar" to track the accumulated costs
    // on each cell (is already reset to 0 automatically)


}



