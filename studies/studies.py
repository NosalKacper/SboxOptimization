import subprocess
from statistics import mean, stdev
import time
from concurrent.futures import ProcessPoolExecutor, as_completed
import os
import csv

def run_cpp_once(args):
    """Run the C++ program once"""
    # try:
    result = subprocess.run(
        args,
        capture_output=True,
        text=True,
        timeout=6000,          # Adjust if one run takes longer
        check=True
    )
    value = float(result.stdout.strip())
    return value
    # except Exception as e:
    #     return None


def run_multiple_parallel(runs=100, max_workers=8, iterations=10, populationSize=10, mutation_probability=10):
    # Your command split into list
    base_cmd = ["../build/SboxOptimization", "-p",str(populationSize), "-i", str(iterations), "-m", str(mutation_probability)]
    
    print(f"Running {runs} times with max {max_workers} parallel processes...\n")
    
    start_time = time.time()
    results = []
    successful = 0
    
    with ProcessPoolExecutor(max_workers=max_workers) as executor:
        # Submit all jobs
        future_to_run = {executor.submit(run_cpp_once, base_cmd): i for i in range(runs)}
        
        # Process as they complete
        for future in as_completed(future_to_run):
            run_number = future_to_run[future] + 1
            try:
                value = future.result()
                if value is not None:
                    results.append(value)
                    successful += 1
                    print(f"Run {run_number:3d}/{runs}: {value:.8f}")
                else:
                    print(f"Run {run_number:3d}/{runs}: Failed")
            except Exception as e:
                print(f"Run {run_number:3d}/{runs}: Error - {e}")
    
    total_time = time.time() - start_time
    
    if not results:
        print("\nNo successful runs.")
        return
    
    print("\n" + "="*65)
    print(f"FINAL RESULTS — {successful}/{runs} successful runs")
    print("="*65)
    print(f"Average        : {mean(results):.8f}")
    print(f"Std Deviation  : {stdev(results):.8f}" if len(results) > 1 else "Std Deviation  : N/A")
    print(f"Minimum        : {min(results):.8f}")
    print(f"Maximum        : {max(results):.8f}")
    print(f"Total time     : {total_time:.2f} seconds")
    print(f"Parallelism    : {max_workers} workers")
    print("="*65)

    return mean(results)
    
def run_mutation_experiment():
    results = []

    for mutation_probability in range(0, 100, 5):
        current_result = run_multiple_parallel(
            runs=100,
            max_workers=8,
            mutation_probability=mutation_probability,
            iterations=100,
            populationSize=100
        )
        results.append([mutation_probability, current_result])

    with open(filename, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["mutation_probability", "result"])
        writer.writerows(results)

if __name__ == "__main__":
    results = []
    for x in range(25,275,25):
        for y in range(25,275,25):
            result = run_multiple_parallel(runs=10,
                                            max_workers=8,
                                            mutation_probability=30,
                                            iterations=x,
                                            populationSize=y)
            results.append([x, y, result])

    with open("results.csv", "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["iterations", "populationSize", "result"])
        writer.writerows(results)

    