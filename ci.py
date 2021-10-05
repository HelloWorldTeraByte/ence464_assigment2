import subprocess
import time

build_dir = 'build/'
n = 101
i = 300

start_time = time.time()
subprocess.run(['./' + build_dir + 'poisson1', '-n', str(n), '-i', str(i)], stdout=subprocess.DEVNULL)
print('1: ', time.time() - start_time)

start_time = time.time()
subprocess.run(['./' + build_dir + 'poisson2', '-n', str(n), '-i', str(i)], stdout=subprocess.DEVNULL)
print('2: ', time.time() - start_time)