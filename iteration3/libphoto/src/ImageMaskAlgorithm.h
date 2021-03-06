#include <cmath>

/// A container for algorithms and helper functions used in ImageMaskBasedFilter
namespace ImageMaskAlgorithm
{
    /// Classic distance formula helper function
    static double dist(int x1, int y1, int x2, int y2)
    {
        return sqrt(pow((double) (x1 - x2), 2) + pow((double) (y1 - y2), 2));
    }

    /// Given a rect and a point get which corner of rect is farthest from the point.
    /// Adapted fromm http://stackoverflow.com/questions/22654770/creating-vignette-filter-in-opencv/
    static double getMaxDistFromCorners(int width, int height, int c_x,  int c_y)
    {
        int corners[4][2] = {{0, 0},
                             {width, 0},
                             {0, height},
                             {width, height}};

        double maxDis = 0;
        for (int i = 0; i < 4; ++i)
        {
            double dis = dist(corners[i][0], corners[i][1], c_x, c_y);
            if (maxDis < dis)
                maxDis = dis;
        }

        return maxDis;
    }

    /// Creates a mask in the shape of a vignette.
    /// Returns a 2D array of doubles that decays from a near 1 center value to a low value farther away\n
    /// Does *not* cleanup its mask, must be cleaned by calling function\n
    /// Adapted fromm http://stackoverflow.com/questions/22654770/creating-vignette-filter-in-opencv/
    static double** vignetteMask(int width, int height, float *args) {
        int c_x = width/2;
        int c_y = height/2;

        // allocate and initialize mask with 1's
        double** mask;
        mask = new double *[width];
        for (int i = 0; i < width; i++)
            mask[i] = new double[height];
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j)
                mask[i][j] = 1;
        }

        // Build a gradient
        double power = args[0];
        double maxImageRad = getMaxDistFromCorners(width, height, c_x, c_y);

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                double temp = (dist(c_x, c_y, i, j) / maxImageRad) * power;
                temp = pow(cos(temp), 4);
                mask[i][j] = temp;
            }
        }

        return mask;
    }


};
