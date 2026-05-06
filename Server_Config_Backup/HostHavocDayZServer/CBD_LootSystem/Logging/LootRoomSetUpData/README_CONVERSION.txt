CBD Loot Rooms door conversion from BS_KeyRoom room files
Generated from 57 old room JSONs.

Each folder follows CBD admin tool example: BuildingClass_[X_Y_Z]/DoorsToLock.json

Coordinate conversion used from BS_KeyRoom files:
LootRoomPosition[0] = round(building_x)
LootRoomPosition[1] = round(building_z) height
LootRoomPosition[2] = round(building_y) map north/south
LootRoomDoorIndex = first value from door_indices

This converts door locks only. Use manifest.json to map each converted folder back to old tier/key/loot-pool data.
