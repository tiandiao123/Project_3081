#include <iostream>
#include <cmath>

// Due to a weird function undefined compilation error
// the static functions could not be moved to a CPP file
// :( I tried to figure it out for a long while then gave

/// A container for algorithms passed into ConvolutionBasedFilter
namespace KernelFilterAlgorithm
{
    /// A helper class providing common support functions and array storage for convolution filter generating functions
    class KernelFilter
    {
    public:
        KernelFilter();
        ~KernelFilter();
        int getSize();
        double **factoredFilter(); ///< Returns the internal array converted into a double array, divided in by m_factor
        void setBias(double bias);
        void updateZeroBrightnessFactor(); ///< Calculates the factor that ensures the values of the filter are additive to 1 such that image brightness will not change on application.
        void setSize(int size); ///< Updates the size of the array ensuring it is odd width/height such that it has a center
        void setSize(float size); ///< Overload of setSize(int) rounding the float to an integer automatically
        int **getFilter(); ///< Allows direct access to the internal array

    protected:
        int allocate(); ///< Allocates memory for KernelFilter
        void destroy(); ///< Frees memory for KernelFilter
        int **m_data; ///< Internal array uses integers for speed and ease of editing

    private:
        int m_size;
        double m_factor;
        double m_bias;
    };

    /* A possible addition to speed up filter retreival time instead of generating the filter each time */
    // class KernelFilterCache
    // {
    // public:
    //     KernelFilterCache();
    //     ~KernelFilterCache();

    //     int hashFilter(int size, float *args, int arg_len, int kern_type);
    //     int saveFilter(KernelFilter kernel);
    //     KernelFilter getFilter(int hash);

    // private:
    //     KernelFilter[] cache;
    // };



    // All algorithms below builds a filter from the args provided
    // Modifies a byref size
    // and returns the filter as a 2D double array

    /// Basic example of the kernel filter generation using KernelFilter
    /** Returns the convolution filter as a 2D array of doubles\n
         [ 0 0 0 ]\n
         [ 0 1 0 ]\n
         [ 0 0 0 ]\n
     Returns the size of the filter by reference\n
     When applied, will return the original image */
    static double** kernelOriginal(int &size, float *args) {
        KernelFilter kernel;
        kernel.setSize(3);
        size = 3;
        int **filter = kernel.getFilter();
        for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            filter[i][j] = 0;
        }
        }
        filter[1][1] = 1;
        return kernel.factoredFilter();
    }

    /// Creates a kernel filter that blurs an image
    /** Filter has general cross shape form additive to 1\n
          0.0, 0.2,  0.0,\n
          0.2, 0.2,  0.2,\n
          0.0, 0.2,  0.0\n
     Larger filters apply more blur */
    static double** kernelBlur(int &size, float *args) {
        KernelFilter kernel;
        kernel.setSize(args[0]/2+2);
        size = kernel.getSize();
        int **filter = kernel.getFilter();
        for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            // Build filter edges...
            if ((i == 0 && j != size / 2) || (i == size - 1 && j != size / 2) ||
                (j == 0 && i != size / 2) || (j == size - 1 && i != size /2))
                filter[i][j] = 0;
            else
                filter[i][j] = 1;
        }
        }
        kernel.updateZeroBrightnessFactor();
        return kernel.factoredFilter();
    }

    /// Creates a kernel filter that sharpens an image
    /** Filter has generally high center value surrounded by negatives additive to 1\n
          -1, -1, -1,\n
          -1,  9, -1,\n
          -1, -1, -1\n
     Doesn't use KernelFilter to balance brightness, it maintains it on its own adding significant complexity to the code\n
     Larger filters apply more sharpening, but when too large, the application is slow, so the size is limited to 10 */
    static double** kernelSharpen(int &size, float *args) {
        KernelFilter kernel;
        int t_size = 60 / args[0];
        if (t_size > 10) t_size = 5;
        kernel.setSize(t_size);
        size = kernel.getSize();
        // We will be manually ensuring the brightness does not change
        // so pull the double version right away
        double **filter = kernel.factoredFilter();
        // Balance num b = perimeter of the matrix +1
        // so the total is is 1 when it is set
        int b = size * 4 - 3;
        // Remainder r = the amount of b remaining after movement from center
        double r = b;
        // f_val the amount to fill each extra square to
        double f_val = 7 / args[0];
        // Assign values
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j)
            {
                if (i == 0 || j == 0 || i == (size - 1) || j == (size - 1))
                    filter[i][j] = -1.0;  // negative edges
                else if (i == j && i == size/2)
                    continue; // skip center for later assignment
                else {
                    filter[i][j] = f_val;
                    r -= f_val;
                }
            }
        }
        filter[size/2][size/2] = r;
        return filter;
    }

    /// Creates a kernel filter that detects the edges of an image, brightening them and darkening the rest
    /** Returns a fixed filter\n
         0  -1  0\n
         0  -1  0\n
         0   2  0\n */
    static double** kernelEdgeDetection(int &size, float *args) {
        KernelFilter kernel;
        kernel.setSize(3);
        size = kernel.getSize();
        kernel.getFilter()[0][1] = -1;
        kernel.getFilter()[1][1] = -1;
        kernel.getFilter()[2][1] = 2;
        return kernel.factoredFilter();
    }

    /// Creates a kernel filter that blurs the an image in a directional manner
    /** Returns a filter with 1s from one end to the other, e.g. diagonal, vertical, horizontal */
    static double** kernelMotionBlur(int &size, float *args) {
        KernelFilter kernel;
        kernel.setSize(args[0]/2+2);
        size = kernel.getSize();
        int **filter = kernel.getFilter();

        for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            switch(int(args[1]))
            {
            case 0:   //vertically
                if (j == size/2) filter[i][j] = 1;
                break;
            case 1:    //horizontally
                if (i == size/2) filter[i][j]=1;
                break;
            case 2:    //northeast-southwest direction
                if (i + j == (size - 1)) filter[i][j] = 1;
                break;
            case 3:   //southeast-northwest direction
                if (i == j) filter[i][j] = 1;
                break;
             default :
                // Invalid motion blur direction
                break;
            }
        }
        }
        kernel.updateZeroBrightnessFactor();
        return kernel.factoredFilter();
    }

};
