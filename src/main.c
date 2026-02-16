#include "test.h"

int main(void)
{
#ifdef RUN_UNIT_TESTS
    lancer_tests(); // le reste du code ne sera pas executé
#endif
    return 0;
}
