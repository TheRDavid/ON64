# N64_DevBase
Built on top of libdragon, encapsulates functions to aid game development for the Nintendo64

- Progress:       http://funblerer.blogspot.de/
- libdragon repo: https://github.com/DragonMinded/libdrago
- libdragon info: https://dragonminded.com/n64dev/
- libdragon docs: https://dragonminded.com/n64dev/libdragon/doxygen/group__libdragon.html

To use merged sprites:
- Replace mksprite.c inside your libdragon/tools/mksprite folder with the one inside the root folder of this repo
- Recompile mksprite (use -16 as parameter for merging instead of 16, -32 not implemented yet because lazy)

To make your life easier:
- Get the ImageMerger: https://github.com/TheRDavid/ImageMerge to merge two image files into 1
- Get the Spritemapper (https://github.com/TheRDavid/Spritemapper) to create and merge sprite maps

Note: All .c files in the root dir except mksprite.c are test apps not a necessary part of the library. They are half-baked and may not work.
