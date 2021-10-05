import subprocess
import time
import os

build_dir = 'build/'

vers = 4
test_ver = 4
n = 51
iters = 300

test = True
benchmark = False
#benchmark = True

def Test():
  print('-' * 50)
  print('Testing: poision' + str(test_ver))
  print('-' * 50)

  r = os.system('./' + build_dir + 'poisson' + str(test_ver) + ' -n 7' + 
                 ' -i 300 ' + ' | cmp reference/7.txt > /dev/null')
  if(r):
    print('n=7 i=300 Failed!')
  else:
    print('n=7 i=300 Passed!')

  r = os.system('./' + build_dir + 'poisson' + str(test_ver) + ' -n 15' + 
                 ' -i 300 ' + ' | cmp reference/15.txt > /dev/null')

  if(r):
    print('n=15 i=300 Failed!')
  else:
    print('n=15 i=300 Passed!')

  r = os.system('./' + build_dir + 'poisson' + str(test_ver) + ' -n 51' + 
                 ' -i 300 ' + ' | cmp reference/51.txt > /dev/null')

  if(r):
    print('n=51 i=300 Failed!')
  else:
    print('n=51 i=300 Passed!')

def Benchmark():
  for i in range(1, vers + 1):
    start_time = time.time()
    subprocess.run(['./' + build_dir + 'poisson' + str(i), '-n', str(n),
                 '-i', str(iters)], stdout=subprocess.DEVNULL)
    print(i, ': ', time.time() - start_time)

if __name__ == '__main__':
  if(test):
    Test()

  if(benchmark):
    Benchmark()
