/* *****************************************************************************
facil.io implementation translation unit.

This is the only file that compiles the facil.io definitions
(`FIO_EXTERN_COMPLETE`). Every other translation unit includes
`fio-stl.h` with `FIO_EXTERN` only and sees declarations.

Do not add application code here. Keep this file to a single `#include` so the
split stays obvious and the 136k-line parse happens exactly once.
`FIO_EXTERN` alone would only emit declarations. Defining both is NOT
redundant: `FIO_EXTERN` makes `SFUNC` expand to nothing (external linkage)
while `FIO_EXTERN_COMPLETE` selects the definition blocks. Without
`FIO_EXTERN` every API function silently becomes `static` and the archive
ends up empty of the public API.
***************************************************************************** */
#define FIO_EVERYTHING
#define FIO_EXTERN
#define FIO_EXTERN_COMPLETE
#include "fio-stl.h"
