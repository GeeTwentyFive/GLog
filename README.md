# Usage:
1) `GLog__Init("<LOG_PATH>")`
2) `GLog__Log("[FORMAT]", ...)`

# Example:
```c
#include "GLog.h"

int main() {

        GLog__Init("TEST_LOG");

        GLog__Log("%s %d %s", "Logging this number: \"", 727, "\" 8)");

        GLog__Log("And another line");

        return 0;

}
```

### ^ Example output:
`TEST_LOG__2025-4-10_4-39-40`:
```
4:39:40 main.c:7: Logging this number: " 727 " 8)
4:39:40 main.c:9: And another line

```
