Import('RTT_ROOT')
Import('rtconfig')
from building import *

#cwd     = os.path.join(str(Dir('#')), 'applications')
cwd  = GetCurrentDir()

include_path = Split("""
lib/include
test
lib/include/metal/processor
""")

src = Split("""
lib/device.c
lib/log.c
lib/init.c
lib/system/rtthread/io.c
lib/system/rtthread/device.c
lib/system/rtthread/init.c
lib/system/rtthread/riscv/sys.c
""")

src += ['test/system/rtthread/atomic.c']
src += ['test/system/rtthread/mutex.c']
src += ['examples/system/rtthread/shmem_demod.c']

src += Split("""
test/metal-test.c
test/system/rtthread/main.c
test/system/rtthread/threads.c
""")

src += ['lib/io.c']
src += ['lib/shmem.c']

CPPDEFINES = []

group = DefineGroup('Applications', src, depend = [''], CPPPATH = include_path, CPPDEFINES = CPPDEFINES)

Return('group')
