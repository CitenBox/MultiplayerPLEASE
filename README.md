####ABOUT
##NOTE: this isn't anything serious it was a personal project for personal use so there is a lot of jank.
# Unreal Multiplayer Template

A high-performance Unreal Engine multiplayer template with steam sessions plugin focused on clean networking and extensibility.  
Includes session management, inventory, chat, and example gameplay systems using PushModel replication.

## Features

### Networking  
Optimized multiplayer setup using PushModel and basic client-side prediction.  

### Menus  
Main, settings, and host/join session menus.  
Maps are defined via data assets and automatically detected at design time.  

### Inventory  
Uses fast templated arrays (FTR) for efficient item handling.  
Supports stacking, moving, dropping, picking up, and equipping.  
Items are defined through data assets specifying class, icon, name, and stack size.  

### Gameplay  
Includes item equipping, example items, a partial gun implementation, and a Garry’s Mod–style physics gun.  
Also includes a simple in-game chat system.  

#### PREREQUISITES
Unreal Engine 5.4  
Visual Studio 2022
#unreal's official guide to properly setup Visual Studio so you get all the c++ tools you need:
https://dev.epicgames.com/documentation/en-us/unreal-engine/setting-up-visual-studio-development-environment-for-cplusplus-projects-in-unreal-engine

#### PACKAGING
When packaging in order for steam sessions to work you need to create a text file called steam_appid.txt and write in it 480.
the file has to be in the same directory as the executable that is inside the binary folder! not the one that is directly inside the packaged build. 

<img width="657" height="286" alt="image" src="https://github.com/user-attachments/assets/bf7c0caa-be93-4dab-8f78-6cae08d4b61a" />
<img width="650" height="143" alt="image" src="https://github.com/user-attachments/assets/f09c28d9-461a-485c-bee1-a8ed0562d872" />
<img width="504" height="120" alt="image" src="https://github.com/user-attachments/assets/182aa3f5-5b9a-48be-9286-3bfee1282fe0" />
<img width="737" height="540" alt="image" src="https://github.com/user-attachments/assets/a863e1ef-52a3-4ac1-93ae-88dfbce4e3e9" />
<img width="243" height="138" alt="image" src="https://github.com/user-attachments/assets/9958b1e2-af11-4950-8fcc-f560ee0e7666" />




