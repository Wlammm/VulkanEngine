#pragma once

#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b

#undef UNIQUE_NAME
#define UNIQUE_NAME(base) CONCAT(base, __COUNTER__)

#define ON_SCOPE_EXIT(lambda) OnScopeExit UNIQUE_NAME(scopeExitObject)(lambda) 