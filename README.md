# ChromaKey
This is a C++ program that identifies the chroma key (green screen) and further replaces it with an alternative background of choice.
The images are 256x256 pixels, therfore the RGB value of each pixel is stored in a 256x256x3 array.
Two algorithms are implemented:
Method1 allows user to manually enter a specified threshold for the chroma mask(filter).
Method2 automatically calculates the optimal threshold and creates a chroma mask accordingly.
