README:
|-------------------------------------|
|             CHANGELOG               |
|-------------------------------------|

01/04/2016 : 
 First commit with all the files as it comes from the download.
 Following commits with changes:
 - Project ported to Visual Studio 2015. 
 - Separated the main project in two projects: 
	- xixEngine: library project to embed the engine for the game
		- Moved all files from external/xix to this project
		- Moved previous external dir to deps dir inside this project
	- BlocksGame: the game using the engine 
  	- New relative configuration for the solution and its projects. 
	- Recompiled SDL2 for VS2015.
 - Moved ExampleGame from main.cpp to BlocksGame/MainGame.cpp and .h files.
 - Renamed Glyph to FontData in Font.h
 - Added InputManager class to handle all the input events
 - Moved functions related with the fonts to Font.h (FindFontChar, CalculateStringWidth)
 - Extracted the constants for: width, height, maxFrameTicks, textScale and made them variables so we can change them directly from the game creation ( MainGame game(800,640,300.0f,0.5f); )

02/04/2016:
- Draw Call Optimizations: the game uses GL_QUADS instead of GL_TRIANGLES, this is dangerous because it will be removed in the incoming versions of OpenGL. The SdlSurface class already uses a way to load images but it does not uses a batch to save drawcalls, so i started doing this and other things one by one. Based on my code from: https://github.com/juaxix/cpp_gamedev/tree/master/xixEngine
- Added a class to make a batching with textures, it renders a quad with triangles and uses a VertexData to store the info and make the batch to upload, per texture id to the GPU, all that info and reuse it. The class is named SpriteBatch. This class uses a struct Glyph (not the old one) to store each corner and uses a sort method with pointers (faster), the texture and the depth. Using this batched algorithm you can draw more sprites with a single drawcall thanks to the video and array buffers for vertices of the triangles for quad shapes.
- Added a class for a camera simulation: Camera2D
- The camera is used from the EngineImplementation structure, thanks to the new class GLSLProgram, this, uses a shader program stored in assets in two parts: vertex and fragment to pass to this program, the position of the camera, so we can move the camera and draw things beyond the box.
- Added a ID function to get the texture id loaded in a texture and assigned by opengl.
- Added colors to draw text (still using quads for rendering texts) and default colors as constants structs, now you can specify the color to the engine when you want to draw a text in a color different from white (default).
- Added the board draw and picking detection for each gem
- Added classes for Audio play using SDL_mixer from https://www.libsdl.org/projects/SDL_mixer/
- Extracted the texture cache method to be able to load textures from outside the engine.
- Moved textures to assets/textures, and added a sound in ogg format to play it: Milestone: texture caching and sound playing.!

03/04/2016
 - Removed some bugs in the memory deletion of sound chunks and music
 - Some vars renamed to follow the project naming convention
 - Added a music in assets/music/game.ogg to play itself once (1min)
 - Added a countdown clock in the box of 60 seconds
 - Added the library picopng to load PNG files data http://lodev.org/lodepng/picopng.cpp
 - Added classes to make the textures cache
 - Added a class to store blocks named Board
 - Bug fixing in SpriteBatching, also added the rotation to the glyph class with an angle and functions to compute it (applied to the selection-target-graphic when we pick a gem)
 - Added the selector swapping, when we touch a gem and draw it
 - Added the first two functions to find matches of gems of the same color
 
04/04/2016
 - Documented code with VS2015 XML format 
 - Optimizations in algorithm for match finding
 - Animations added for swapping gems : now detection of a valid move and return back with animation too
 - Code reorganized: TextureCache now with a .cpp 
 
05/04/2016
  - Solutions Projects updated and built for XCode 7.3 (recompiled glew and added SDL_mixer)
  - Board: Matched bricks are being removed after a scale animation
  - Milestone: matches are working 
  - Fixed bugs in text drawing scale
  
06/04/2016
  - Added the score, score texts, new sounds and textures for the fuse,score-background
  - Added score board with the score and little score texts when there are matches 
  - Added the fuse animation to move along the rope with an interpolation
  - Added the MENU Game State to the MainGame, so it will start in this mode after init and when the user click ,it will change to the PLAY mode
  - SDL_GL context set for OSX Core https://developer.apple.com/opengl/capabilities/ in the solutions for XCode. Same source,different solution projects, configured using the __APPLE__ macro.
  - Added the best score feature to be saved on disk.
  
07/04/2016
  - Memory optimizations
  - XCode project updated (still need to change the shaders version to be fully compatible with OSX)
  - Added sounds for transitions
  - Improve the textScore animations
  - Added commented code for BOMBS (Brick types)
  - Added a glsl animation with color and arcade style: updated the glsl shaders and functions in SpriteBatch
  - Time management improved
  - Added a new function in the engine to move the camera x,y coords and do transitions between scenes
  - Removed picopng lib and using SDL_image to load images into TextureCache class.