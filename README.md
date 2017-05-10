<h1 id="on64-open-nintendo-64-library">ON64 - Open Nintendo 64 Library</h1>

<p>ON64 is a small programming library built ontop of the hardwareinterface <a href="https://github.com/DragonMinded/libdragon">Libdragon</a>. <br>
The aim of this project is to develop a simple library that provides tools to create Nintendo 64 compatible games. Granted, that alone can be done with Libdragon itself, ON64 simply adds some graphics-functionality, a few development tools and a simple application structure.</p>

<h2 id="how-to-use-it">How to use it</h2>

<blockquote>
  <ol>
  <li>Build and install the Libdragon-Library if you haven’t already <br>
  (or use a pre-built <a href="https://www.neoflash.com/forum/index.php?topic=7444.0">Dev-ISO</a> )</li>
  <li>Download ON64 (no need to install, you’ll build the source along with your game)</li>
  <li>Create a new folder for your game - it should contain: <br>
  <ul><li>game.c <em>(your main source file)</em></li>
  <li>Makefile </li>
  <li>clean </li>
  <li>lib <em>(contains the ON64 source)</em></li>
  <li>mips64-elf</li></ul></li>
  <li>Compile using the <i class="icon-cog"></i><strong>clean</strong> shellscript (<code>sh clean game</code>)</li>
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

<blockquote>
  <ol>
  <li>Simplistic Game-Infrastructure</li>
  <li>Graphics <br>
  <ul><li>Flipping</li>
  <li>Nearest-Neighbour Scaling</li>
  <li>Bilinear Scaling</li>
  <li>Rotation</li>
  <li>Fading</li>
  <li>Basic Geometries (Circle, Rectangle, Triangle)</li>
  <li>4-Corner Distortion</li>
  <li>Whirl-Effect</li>
  <li>Additinal 8-Bit color-scheme</li></ul></li>
  <li>Dev-Tools <br>
  <ul><li>FPS-Display</li>
  <li>Memory-Monitor</li>
  <li>Debug-Output</li></ul></li>
  <li>Desktop-Tools <br>
  <ul><li>Image Merger</li>
  <li>Spritemap Creator</li></ul></li>
  </ol>
</blockquote>