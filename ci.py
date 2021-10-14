import subprocess
import time
import os
import matplotlib.pyplot as plt

build_dir = 'build/'

vers = 6
test_ver = 6

const_iters = 300
ns = [3, 7, 15, 51, 101]
ns_extra = [101, 201, 301, 401, 501, 601, 701, 801, 901]

const_n = 51
iters = [30, 100, 300, 500]

threads_test = range(2, 100, 2)

ns_results = []
iters_results = []
opt_results = []
threads_results = []

test = False
benchmark = True
plot = True

bench_cube_size = True
bench_cube_size_extra = True
bench_iterations = True
bench_threads = True
bench_opt = True

def Test():
  print('-' * 80)
  print('Testing: poision' + str(test_ver))
  print('-' * 80)

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
  if(bench_threads):
    print('-' * 80)
    print('Benchmarking number of threads')
    print('-' * 80)
    for t in threads_test:
      s = 0
      for i in range(0, 4):
        start_time = time.time()
        subprocess.run(['./' + build_dir + 'poisson6', '-n', str(const_n),
                        '-i', str(const_iters), '-t', str(t)], stdout=subprocess.DEVNULL)
        time_delta = (time.time() - start_time) * 1000
        s += time_delta
        if(i == 3):
          threads_results.append(s/4)
          print('Poisson6', ':', 'Threads', ':', str(t), 'Cube Size:', str(const_n), ',', 'Run Time Average:', s/4, 'ms')

  if(bench_cube_size):
    print('-' * 80)
    print('Benchmarking different cube sizes')
    print('-' * 80)
    for ver in range(1, vers):
      r = []
      for n in ns:
        start_time = time.time()
        subprocess.run(['./' + build_dir + 'poisson' + str(ver), '-n', str(n),
                        '-i', str(const_iters)], stdout=subprocess.DEVNULL)
        time_delta = (time.time() - start_time) * 1000
        r.append(time_delta)
        print('Poisson', ver, ':', 'Cube Size:', str(
            n), ',', 'Run Time:', time_delta, 'ms')
      ns_results.append(r)

  if(bench_cube_size_extra):
    r = []
    for n in ns_extra:
      start_time = time.time()
      subprocess.run(['./' + build_dir + 'poisson' + str(vers), '-n', str(n),
                      '-i', str(50)], stdout=subprocess.DEVNULL)
      time_delta = (time.time() - start_time) * 1000
      r.append(time_delta)
      print('Poisson', vers, ':', 'Cube Size:', str(
          n), ',', 'Run Time:', time_delta, 'ms')
    ns_results.append(r)


  if(bench_iterations):
    print('-' * 80)
    print('Benchmarking different iterations')
    print('-' * 80)
    for ver in range(1, vers + 1):
      r = []
      for iter in iters:
        start_time = time.time()
        subprocess.run(['./' + build_dir + 'poisson' + str(ver), '-n', str(const_n),
                        '-i', str(iter)], stdout=subprocess.DEVNULL)
        time_delta = (time.time() - start_time) * 1000
        r.append(time_delta)
        print('Poisson', ver, ':', 'Iterations:', str(
            iter), ',', 'Run Time:', time_delta, 'ms')
      iters_results.append(r)

  if(bench_opt):
    print('-' * 80)
    print('Benchmarking different optimizations')
    print('-' * 80)
    for ver in range(1, vers + 1):
      r = []
      for opts in range(0, 4):
        its = []
        for n in ns:
          start_time = time.time()
          if(opts == 0):
            subprocess.run(['./' + build_dir + 'poisson' + str(ver), '-n', str(n),
                            '-i', str(const_iters)], stdout=subprocess.DEVNULL)
          else:
            subprocess.run(['./' + build_dir + 'poisson' + str(ver) + 'O' + str(opts), '-n', str(n),
                            '-i', str(const_iters)], stdout=subprocess.DEVNULL)
          time_delta = (time.time() - start_time) * 1000
          print('Poisson', ver, ':', 'Opt:', str(opts),
                ',', 'n:', n, 'Run Time:', ',', time_delta, 'ms')
          its.append(time_delta)
        r.append(its)
      opt_results.append(r)


def Plot():
  if(bench_cube_size):
    fig1, ax1 = plt.subplots()
    for ver in range(1, vers + 1):
      ax1.plot(ns, ns_results[ver-1], label='Poisson' + str(ver))

    ax1.set_title(
        'Computation time of various Poisson solvers for different number cube sizes [n]')
    ax1.set_xlabel('Number of Sides [n]')
    ax1.set_ylabel('Time (ms)')
    ax1.legend(loc='upper left')

  if(bench_iterations):
    fig2, ax2 = plt.subplots()
    for ver in range(1, vers + 1):
      ax2.plot(iters, iters_results[ver-1], label='Poisson' + str(ver))

    ax2.set_title(
        'Computation time of various Poisson solvers for different number of iterations')
    ax2.set_xlabel('Number of Iterations')
    ax2.set_ylabel('Time (ms)')
    ax2.legend(loc='upper left')

  if(bench_opt):
    fig3, ax3 = plt.subplots()
    for ver in range(1, vers + 1):
      ax3.plot(opt_results[ver-1][:][3], label='Poisson' + str(ver))

    ax3.set_xticks([0, 1, 2, 3])
    ax3.set_title(
        'Computation time of various Poisson solvers with different compiler optimizations')
    ax3.set_xlabel('Optimization Level')
    ax3.set_ylabel('Time (ms)')
    #ax3.set_yscale('log')
    ax3.legend(loc='upper right')

    for ver in [1, 2, 5, 6]:
      plt.figure()
      for opt in range(0, 4):
          plt.plot(ns, opt_results[ver - 1][opt], label='Poisson' +
                   str(ver) + ' with Optimization O' + str(opt))
      plt.title('Computation time of Poisson ' + str(ver) +
                ' solver with different compiler optimizations')
      plt.legend(loc='upper left')
      plt.xlabel('Number of Sides [n]')
      plt.ylabel('Time (ms)')

  if(bench_threads):
    fig4, ax4 = plt.subplots()
    ax4.plot(threads_test ,threads_results)

    ax4.set_title('Computation time of multithreaded Poisson solvers for different number of threads')
    ax4.set_xlabel('Number of Threads')
    ax4.set_ylabel('Time (ms)')

  plt.show()

if __name__ == '__main__':
  if(test):
    Test()

  if(benchmark):
    Benchmark()

  if(plot):
    Plot()
