<h1 id="on64-open-nintendo-64-library">ON64 - Open Nintendo 64 Library</h1>

<p>ON64 is a small programming library built ontop of the hardwareinterface <a href="https://github.com/DragonMinded/libdragon">Libdragon</a>. <br>
The aim of this project is to develop a simple library that provides tools to create Nintendo 64 compatible games. Granted, that alone can be done with Libdragon itself, ON64 simply adds some graphics-functionality, a few development tools and a simple application structure.</p>

<h2 id="how-to-use-it">How to use it</h2>

<blockquote>
  <ol>
  <li>Build and install the Libdragon-Library if you haven’t already. <br>
  (or use a pre-built <a href="https://www.neoflash.com/forum/index.php?topic=7444.0">Dev-ISO</a> )</li>
  <li>Download ON64 (no need to install, you’ll build the source along with your game)</li>
  <li><p>Create a new folder for your game - it should contain:</p>
  
  <blockquote>
    <ul><li><i class="icon-file"></i> game.c <em>(your main source file)</em></li>
    <li><i class="icon-file"></i> Makefile</li>
    <li><i class="icon-file"></i> clean</li>
    <li><i class="icon-folder-open"></i> lib <em>(contains the ON64 source)</em></li>
    <li><i class="icon-folder-open"></i> mips64-elf</li></ul>
  </blockquote></li>
  <li><p>Compile using the <i class="icon-cog"></i><strong>clean</strong> shellscript (<code>sh clean game</code>)</p></li>
  </ol>
</blockquote>

<p>Have a look at the demo-apps to get started.</p>

<hr>

<p>Things you should read:</p>

<blockquote>
  <ul>
  <li><a href="https://dragonminded.com/n64dev/">Libdragon Info</a></li>
  <li><a href="https://dragonminded.com/n64dev/libdragon/doxygen/group__libdragon.html">Libdragon Documentation</a></li>
  <li><a href="http://does-not-exist-yet">ON64 Documentation</a></li>
  </ul>
</blockquote>

<p>Additional Tools:</p>

<blockquote>
  <ul>
  <li><a href="https://github.com/TheRDavid/ImageMerge">ImageMerger</a> merges to images into one (by sacrificing color-depth) by using an additional color-scheme implemented in ON64</li>
  <li><a href="https://github.com/TheRDavid/Spritemapper">SpriteMapper</a> is a tool to create, arrange and merge spritemap animations</li>
  </ul>
</blockquote>

<p>To use these tools you’ll need to replace <i class="icon-file"></i><strong>mksprite.c</strong> inside your <i class="icon-folder-open"></i><strong>libdragon/tools/mksprite</strong> - folder with the one in the root-directory of this project and recompile it. To take advantage the merged color-scheme, use <em>-16</em> as parameter instead of 16/32.</p>

<p><strong>Note:</strong> All .c files in the root dir except mksprite.c are test apps not a necessary part of the library. They are half-baked and may not work.</p>

<h2 id="features">Features</h2>

<ol>
<li>Simplistic Game-Infrastructure</li>
<li>Graphics <br>
<ul><li>Flipping</li></ul></li>
<li>Dev-Tools <br>
⋅⋅1. Ordered sub-list</li>
<li>Desktop-Tools</li>
</ol>

<p>⋅⋅⋅You can have properly indented paragraphs within list items. Notice the blank line above, and the leading spaces (at least one, but we’ll use three here to also align the raw Markdown).</p>

<p>⋅⋅⋅To have a line break without a paragraph, you will need to use two trailing spaces.⋅⋅ <br>
⋅⋅⋅Note that this line is separate, but within the same paragraph.⋅⋅ <br>
⋅⋅⋅(This is contrary to the typical GFM line break behaviour, where trailing spaces are not required.)</p>

<p></p><ul> <br>
<li>Unordered list can use asterisks</li>
<li>Or minuses</li>
<li>Or pluses</li>
<li> <br></li></ul><p></p>

<blockquote>
  <ul>
  <li><p>Simplistic Game-Infrastructure</p></li>
  </ul>
  
  <hr>
  <li><p><strong>Graphics</strong> <br>
  Flipping <br>
  Nearest-Neighbour Scaling <br>
  Bilinear Scaling <br>
  Rotation <br>
  Fading <br>
  Basic Geometries (Circle, Rectangle, Triangle) <br>
  4-Corner Distortion <br>
  Whirl-Effect <br>
  Additinal 8-Bit color-scheme</p></li>
  
</blockquote>

<hr>

<blockquote>
  <ul>
  <li><strong>Dev-Tools</strong> <br>
  <ul><li>FPS-Display</li>
  <li>Memory-Monitor</li>
  <li>Debug-Output</li></ul></li>
  </ul>
</blockquote>

<hr>

<blockquote>
  <ul>
  <li><strong>Desktop-Tools</strong> <br>
  <ul><li>Image Merger</li>
  <li>Spritemap Creator</li></ul></li>
  </ul>
</blockquote>