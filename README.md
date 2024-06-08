# WatershedSegmentation
Learn the watershed segmentation technique hopefully
What I have done is:
1. read a RGB image from disk
2. greyscale the image, so the image is turned to a 2D matrix
3. start traverse from each of the pixel, and label them

The traverse routine:

I use DFS (Depth First Search), starting from pixel(i, j):

    * If there are pixels that have smaller value than it, choose the smallest one, then depth + 1

    * If there's no pixels have smaller value than it, which means it's a `minimum point`, return

    * label every pixels on the DFS path with the id of the `minimum point` it can reach to