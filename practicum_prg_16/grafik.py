import matplotlib.pyplot as plt
import numpy as np

data = []
with open('data.txt', 'r') as f:
    for line in f:
        N, threads, time = map(int, line.split())
        data.append((N, threads, time))

threads_values = sorted(set([t for _, t, _ in data]))

plt.figure(figsize=(10, 6))

for threads in threads_values:
    sizes = []
    times = []
    for N, t, time in data:
        if t == threads:
            sizes.append(N)
            times.append(time)
    plt.plot(sizes, times, 'o-', label=f'{threads} threads', linewidth=2)

plt.xlabel('Matrix Size (N)', fontsize=12)
plt.ylabel('Time (ms)', fontsize=12)
plt.title('Matrix Multiplication Time vs Size', fontsize=14)
plt.legend()
plt.grid(True, alpha=0.3)
plt.savefig('graph.png', dpi=100)
plt.show()