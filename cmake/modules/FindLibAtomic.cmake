# FindLibAtomic
# --------
#
# Find LibAtomic
#
# Find the native LibAtomic includes and library This module defines
#
# ::
#
#   LIBATOMIC_LIBRARIES, the libraries needed to use LibAtomic.
#   LIBATOMIC_FOUND, If false, do not try to use LibAtomic.
#
# also defined, but not for general use are
#
# ::
#
#   LIBATOMIC_LIBRARY, where to find the LibAtomic library.

set (LIBATOMIC_NAMES ${LIBATOMIC_NAMES} atomic)
find_library (LIBATOMIC_LIBRARY NAMES ${LIBATOMIC_NAMES})

# handle the QUIETLY and REQUIRED arguments and set LIBATOMIC_FOUND to TRUE if
# all listed variables are TRUE
include (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (LibAtomic DEFAULT_MSG LIBATOMIC_LIBRARY)

if (LIBATOMIC_FOUND)
  set (LIBATOMIC_LIBRARIES ${LIBATOMIC_LIBRARY})
endif (LIBATOMIC_FOUND)

mark_as_advanced (LIBATOMIC_LIBRARY)
