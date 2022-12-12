# AAA_Engine (UNreal Engine)
>⚠️ WARNING!!! ⚠️️ This is not (yet) a real engine... (https://github.com/markmeliaa/AAA_Engine).                                                                   
>Developed by Marc Alcón Meliá (https://github.com/markmeliaa).

---
## - **Required developed tasks**
1. The build is compiled in Release with only the necessary material needed for execution
(including two other sample geometries to load, _LinuxPenguin.fbx_ and _RedGuard.fbx_).
2. _Baker_house.fbx_ is automatically loaded at the start of the execution.
3. The own two models are in _.fbx_ format with one diffuse texture.
4. It accepts _"drag & drop"_ of any _.fbx_ files and textures (only to show the last dropped) from anywhere in the Hard Drive. The current geometry is removed if a new one is dropped.
5. To find a texture (every step is shown through the LOG):

     a. First, the path described in the FBX gets checked.
     
     b. Then, if that fails, it checks on the same folder from where the _.fbx_ was loaded.
     
     c. And last, it tries in the own _“Textures/”_ folder of the Engine.
6. It features Unity-like camera controls:

     a. While Right clicking, “W-A-S-D” fps-like movement and free look are enabled, also "Q-E" move the camera up and down.
     
     b. Mouse wheel zooms in and out.
     
     c. Left click drag rotates the camera around.
     
     d. Alt + Left click orbit the object.
     
     e. Pressing “F” focuses the camera around the current loaded geometry.
     
     f. Holding SHIFT duplicates any movement speed.
7. The Console window correctly LOGS the geometry loading process from ASSIMP.
8. There is a **Configuration** window containg the following:

     a. A graph for the frames per second, in the "Application" section.
     
     b. Configuration for the variables on each module (renderer, window, camera and textures in their corresponding sections of the window).
     
     c. There is information about memory consumption, hardware detection and software versions (SDL, OpenGL, GLSL, Assimp), it is located in the "Hardware" section of the **Properties** window (I decided to include this section in the **Properties** window rather than in the **Configuration** because it does not configure anything, it just shows the properties of the Hardware).
9. The camera adapts to the size of the geometry after being dropped to move far or close depending on the scale of the geometry.
10. There is a main menu at the top with option to quit (File section), visit the github page (GitHub section, with possibility to check the Wiki, check any new Releases and report a Bug), and get info about the engine (**About** window). It also shows the possiblity to turn **ON** closed windows in the Window section (in order to turn them **OFF**, just close each desired window with their own close (x) button).
11. A **Properties** window with three sections: Transformation, Geometry and Texture. All of them give read-only information about the current loaded meshes and texture (triangle count, texture size, texture name...).
12. If the _.fbx_ file contains multiple meshes, the system is able to load all of them without
applying any transformation (try loading _LinuxPenguin.fbx_), and only one texture at a time is required.
13. If the window changes its aspect ratio, the graphics are not deformed. The windows also get redistributed if the size of the window changes to adapt to the new size.
14. The window size of the editor is in relation to the desktop size (you can request the desktop screen size from SDL), at the beginning is the same size for all the computers, but it can be resized to the max size of the computer's desktop.

---
## - **Extra developed tasks**
There are a some small changes made to the basic tasks required, and they are stated above in their own line describing its funcionality or can be seen in the engine, and for some extra developed tasks:
1. The current loaded model can be moved, scaled and rotated around; the Transformation section from the **Properties** window is not read-only, but it can also be used to move the model around without altering the capacity to focus or orbit around it regarding to where or what size it is.
2. The current loaded texture is displayed as an image in the Texture section from the **Properties** window.
3. The system VRAM budget, usage, available and reserved is retrieved each frame and shown in the Hardware section of the **Properties** window.

---
## - **Additional comments for the teachers**
Something else that was also developed but is not really "extra work" is that the **style of the engine** was changed to be more original and personal, as one of the acceptance criteria for the engine is that it should be original. Also the windows are locked to the positions they are because I prefered for them to be fixed (kind of like in Unity) than to be moved around and in the middle of the rendering view.

Hope you like it! ~Mark
