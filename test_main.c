#include <assert.h>
#include "mutf8.h"

int main(void)
{
    int test = sample(1);
    assert(test == 2);
    return 0;
}