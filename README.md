
ON64 - Open Nintendo 64 Library
===================

ON64 is a small programming library built ontop of the hardwareinterface [Libdragon](https://github.com/DragonMinded/libdragon).
The aim of this project is to develop a simple library that provides tools to create Nintendo 64 compatible games. Granted, that alone can be done with Libdragon itself, ON64 simply adds some graphics-functionality, a few development tools and a simple application structure.

How to use it
-------------
> 1. Build and install the Libdragon-Library if you haven't already.
> (or use a pre-built [Dev-ISO](https://www.neoflash.com/forum/index.php?topic=7444.0) )
> 2. Download ON64 (no need to install, you'll build the source along with your game)
> 3. Create a new folder for your game - it should contain:
>> - <i class="icon-file"></i> game.c *(your main source file)*
>> - <i class="icon-file"></i> Makefile
>> - <i class="icon-file"></i> clean
>> - <i class="icon-folder-open"></i> lib *(contains the ON64 source)*
>> - <i class="icon-folder-open"></i> mips64-elf

> 4.  Compile using the <i class="icon-cog"></i>**clean** shellscript (```sh clean game```)

Have a look at the demo-apps to get started.

---

Things you should read:
> - [Libdragon Info](https://dragonminded.com/n64dev/)
> - [Libdragon Documentation](https://dragonminded.com/n64dev/libdragon/doxygen/group__libdragon.html)
> - [ON64 Documentation](http://does-not-exist-yet)

Additional Tools:
> - [ImageMerger](https://github.com/TheRDavid/ImageMerge) merges to images into one (by sacrificing color-depth) by using an additional color-scheme implemented in ON64
> - [SpriteMapper](https://github.com/TheRDavid/Spritemapper) is a tool to create, arrange and merge spritemap animations

To use these tools you'll need to replace <i class="icon-file"></i>**mksprite.c** inside your <i class="icon-folder-open"></i>**libdragon/tools/mksprite** - folder with the one in the root-directory of this project and recompile it. To take advantage the merged color-scheme, use *-16* as parameter instead of 16/32.

**Note:** All .c files in the root dir except mksprite.c are test apps not a necessary part of the library. They are half-baked and may not work.


Features
-----

1. Simplistic Game-Infrastructure
2. Graphics
 -  Flipping
3. Dev-Tools
⋅⋅1. Ordered sub-list
4. Desktop-Tools

⋅⋅⋅You can have properly indented paragraphs within list items. Notice the blank line above, and the leading spaces (at least one, but we'll use three here to also align the raw Markdown).

⋅⋅⋅To have a line break without a paragraph, you will need to use two trailing spaces.⋅⋅
⋅⋅⋅Note that this line is separate, but within the same paragraph.⋅⋅
⋅⋅⋅(This is contrary to the typical GFM line break behaviour, where trailing spaces are not required.)

* Unordered list can use asterisks
- Or minuses
+ Or pluses
+ 
> - Simplistic Game-Infrastructure

---
> - **Graphics**
>  Flipping
>  Nearest-Neighbour Scaling
>  Bilinear Scaling
>  Rotation
>  Fading
>  Basic Geometries (Circle, Rectangle, Triangle)
>  4-Corner Distortion
>  Whirl-Effect
>  Additinal 8-Bit color-scheme

---
> - **Dev-Tools**
>  - FPS-Display
>  - Memory-Monitor
>  - Debug-Output

---
> - **Desktop-Tools**
>  - Image Merger
>  - Spritemap Creator
