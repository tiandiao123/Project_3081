#include <iostream>
#include <cmath>

// Due to a weird function undefined compilation error
// the static functions could not be moved to a CPP file
// :( I tried to figure it out for a long while then gave
// up
namespace KernelFilterAlgorithm
{
    class KernelFilter
    {
    public:
        KernelFilter();
        ~KernelFilter();
        int getSize();
        double **factoredFilter();
        void setBias(double bias);
        void updateZeroBrightnessFactor();
        void setSize(int size);
        void setSize(float size);
        int **getFilter();

    protected:
        int allocate(); // Allocates memory for KernelFilter
        void destroy(); // Frees memory for KernelFilter
        int **m_data;

    private:
        int m_size;
        double m_factor;
        double m_bias;
    };

    // A possible addition to speed up filter retreival time
    // instead of generating the filter each time
    //
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

    // Basic example
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

    static double** kernelSharpen(int &size, float *args) {
        std::cout << "kernelSharpen algorithm called";
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

    static double** kernelEdgeDetection(int &size, float *args) {
        std::cout << "kernelEdgeDetection algorithm called";
        KernelFilter kernel;
        kernel.setSize(5);
        size = kernel.getSize();
        kernel.getFilter()[0][2] = -1;
        kernel.getFilter()[1][2] = -1;
        kernel.getFilter()[2][2] = 2;
        return kernel.factoredFilter();
    }

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
