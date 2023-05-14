@echo off
SET COPYCMD=/Y
MOVE /Y bwapi.ini ../sc/bwapi-data/
CD ../sc/
injectory_x86.exe --launch StarCraft.exe --inject bwapi-data\BWAPI.dll wmode.dll