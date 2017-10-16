# ChromaKey
<h1> Project Overview </h1>
<p1> This is a C++ program that identifies the chroma key (green screen), and further replaces it with an alternative background of choice. </p1>

<p2>The images are 256x256 pixels, therfore the RGB value of each pixel is stored in a 256x256x3 array.</br>
Two algorithms are implemented:</br>
Method1 allows user to manually enter a specified threshold for the chroma mask(filter).</br>
Method2 automatically calculates the optimal threshold and creates a chroma mask accordingly.</p2>
