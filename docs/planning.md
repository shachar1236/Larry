# Goal
I want to have an engine that uses node system.
Each node will hove each own properties like we can have a node for static image, animation, text, ui, sound, collision, KinematicBody, empty node.
All the nodes can inherete from other nodes and the base node will be an empty node.
The empty node will only containe a position vector.
You can add to every node it's own script that will be in the language: lua

The nodes configuration will be stored in a yaml/toml file and when the engine starts it will read the file and add every node to the program.
I also want to have a config file so when the engine starts it will read the comnfig file and set the engine settings by it.

I will also want people to create their own shaders to use for preprocessing.

I want to have a particle system.
I want to add logging.

## Technologies
C++
OpenGL
