#include <iostream>
#include <spatialindex/capi/sidx_api.h>
using namespace std;
using namespace SpatialIndex;

int main(int argc, char* argv[])
{
    char* pszVersion = SIDX_Version();
    fprintf(stdout, "libspatialindex version: %s\n", pszVersion);
    fflush(stdout);
    free(pszVersion);
}	
