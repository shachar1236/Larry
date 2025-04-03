# Renderer

## Overview
This is what is responsible for rendering everithing in our world.
When the renderer initialize it takes a config and with that config it makes our window by the size that is in the config.
The config should contain the following things:
- window width and height
- viewport width and height
- is the window resizable
- fullscreen
- ...

When the renderer is destructed it should close the window and call glfwTerminate.

## Functions
- bool ShouldClose() - returns true if someone clicked on close
- void Background(Color) - Paints the background with the color provided
- void UpdateFrame() - Draw the frame to the screen
- void DrawLine(Vec2 a, Vec2 b) - Draws a line from a to b
- void DrawTriangle(Vec2 a, Vec2 b, Vec3 c) - Draws a triangle with the vertices provided
- void DrawSquare(Vec2 a, width, height) - Draws a square from a with the width and height provided 
- void DrawCircle(Vec2 a, radius) - Draws a circle from a with the radius provided.
- void DrawSprite(sprite, Vec2 a, width, height) - Draws a sprite from a with the width and height provided 
- void Text(text, Vec2 a, size, font, mode) - Draws text at a with the size and font provided. mode determined from where to draw the text: LEFT, CENTER, RIGHT.

Notes:
All the draw functions should also have a DrawMode parameter that tells if the its static or dynamic.

## Add in the future
I want to make it so the user will be able to pass a shader to create a preprocessing effect.
