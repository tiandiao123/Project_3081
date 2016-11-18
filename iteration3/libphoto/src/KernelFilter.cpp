#include "KernelFilterAlgorithm.h"
#include <assert.h>
#include <stdlib.h>
#include <cmath>

KernelFilterAlgorithm::KernelFilter::KernelFilter() {
    m_size = 0;
    m_bias = 0;
    m_factor = 1.0;
}
KernelFilterAlgorithm::KernelFilter::~KernelFilter() {
    destroy();
}

int KernelFilterAlgorithm::KernelFilter::getSize() {
    return m_size;
}

double **KernelFilterAlgorithm::KernelFilter::factoredFilter() {
    // allocate
    double **filter;
    assert(m_size > 0);
    filter = new double *[m_size];
    for (int i = 0; i < m_size; i++)
        filter[i] = new double[m_size];
    // initialize
    for (int i = 0; i < m_size; ++i)
    {
        for (int j = 0; j < m_size; ++j)
        {
            filter[i][j] = 0;
        }
    }
    // assign
    for (int i = 0; i < m_size; ++i)
    {
        for (int j = 0; j < m_size; ++j)
        {
            filter[i][j] = m_data[i][j]*m_factor; // + m_bias?
        }
    }
    return filter;
}

int KernelFilterAlgorithm::KernelFilter::allocate() {
    assert(m_size > 0);
    m_data = new int *[m_size];
    for (int i = 0; i < m_size; i++)
        m_data[i] = new int[m_size];
    for (int i = 0; i < m_size; ++i)
    {
        for (int j = 0; j < m_size; ++j)
        {
            m_data[i][j] = 0;
        }
    }
    return 1;
}

int **KernelFilterAlgorithm::KernelFilter::getFilter() {
    return m_data;
}

void KernelFilterAlgorithm::KernelFilter::destroy() {
    for (int i = 0; i < m_size; i++)
        if (m_data[i] != NULL) delete [] m_data[i];
    delete [] m_data;
}

void KernelFilterAlgorithm::KernelFilter::updateZeroBrightnessFactor() {
    int sum = 0;
    for (int i = 0; i < m_size; ++i) {
    for (int j = 0; j < m_size; ++j) {
        sum += m_data[i][j];
    }
    }
    if (sum == 0)
        m_factor = 1.0;
    else
        m_factor = 1.0 / sum;
}

void KernelFilterAlgorithm::KernelFilter::setSize(int size) {
    if (m_size > 0) destroy();
    if ((size % 2) == 0) size += 1;
    m_size = size;
    allocate();
}

void KernelFilterAlgorithm::KernelFilter::setSize(float size) {
    setSize(int(round(size)));
}

void KernelFilterAlgorithm::KernelFilter::setBias(double bias) {
    m_bias = bias;
}