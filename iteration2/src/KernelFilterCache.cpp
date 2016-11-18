// A possible addition to speed up filter retreival time
// instead of generating the filter each time

// a unique hash is generated for each unique kernel
// based on their settings and type
// since the algorithms will always return the same
// thing given the settings

// KernelFilterCache::KernelFilterCache();
// KernelFilterCache::~KernelFilterCache();

// int KernelFilterCache::hashFilter(int size, float *args, int arg_len, int kern_type);
// int KernelFilterCache::saveFilter(KernelFilter kernel);
// KernelFilter KernelFilterCache::getFilter(int hash);
