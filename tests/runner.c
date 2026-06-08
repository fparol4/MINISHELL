#include "tester.h"

#define TEST_SHARED_RUNNER
#include "shared.c"
#include "runner/expand.c"
#include "runner/command.c"
#include "runner/pipe.c"
#include "runner/redir.c"
#include "runner/status.c"
#include "runner/signal.c"

snow_main();
