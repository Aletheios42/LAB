#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <float.h>
#include <limits.h>

// Test parameters
#define NUM_SAMPLES 50
#define ITERATIONS_PER_SAMPLE 1000000
#define MIN_VALUE 1.0f
#define MAX_VALUE ((float)INT_MAX)

// Function implementations
float standard_inv_sqrt(float x) {
    return 1.0f / sqrtf(x);
}

float fast_inv_sqrt_1iter(float number) {
    long i;
    float x2, y;
    const float threehalfs = 1.5f;
    
    x2 = number * 0.5f;
    y = number;
    i = *(long*)&y;                         // evil floating point bit hack
    i = 0x5f3759df - (i >> 1);             // what the fuck?
    y = *(float*)&i;
    y = y * (threehalfs - (x2 * y * y));   // 1st iteration
    
    return y;
}

float fast_inv_sqrt_2iter(float number) {
    long i;
    float x2, y;
    const float threehalfs = 1.5f;
    
    x2 = number * 0.5f;
    y = number;
    i = *(long*)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float*)&i;
    y = y * (threehalfs - (x2 * y * y));   // 1st iteration
    y = y * (threehalfs - (x2 * y * y));   // 2nd iteration
    
    return y;
}

// Timing utilities
double get_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1e9 + ts.tv_nsec;
}

// Test data generation
void generate_test_values(float* values, int count) {
    for (int i = 0; i < count; i++) {
        // Logarithmic distribution from MIN_VALUE to MAX_VALUE
        float log_min = logf(MIN_VALUE);
        float log_max = logf(MAX_VALUE);
        float log_val = log_min + (log_max - log_min) * i / (count - 1);
        values[i] = expf(log_val);
    }
}

// Benchmark function
typedef struct {
    double avg_time_ns;
    double max_abs_error;
    double max_rel_error;
    double avg_abs_error;
} benchmark_result_t;

benchmark_result_t benchmark_function(float (*func)(float), float* test_values, 
                                    float* reference_values, int num_samples) {
    benchmark_result_t result = {0};
    double total_time = 0;
    double total_abs_error = 0;
    
    for (int sample = 0; sample < num_samples; sample++) {
        float x = test_values[sample];
        float reference = reference_values[sample];
        
        // Timing test
        double start_time = get_time_ns();
        volatile float result_val;
        for (int i = 0; i < ITERATIONS_PER_SAMPLE; i++) {
            result_val = func(x);
        }
        double end_time = get_time_ns();
        
        total_time += (end_time - start_time) / ITERATIONS_PER_SAMPLE;
        
        // Error analysis
        float computed = func(x);
        float abs_error = fabsf(computed - reference);
        float rel_error = abs_error / reference;
        
        total_abs_error += abs_error;
        
        if (abs_error > result.max_abs_error) {
            result.max_abs_error = abs_error;
        }
        
        if (rel_error > result.max_rel_error) {
            result.max_rel_error = rel_error;
        }
    }
    
    result.avg_time_ns = total_time / num_samples;
    result.avg_abs_error = total_abs_error / num_samples;
    
    return result;
}

void print_results(const char* name, benchmark_result_t result) {
    printf("%-20s | %8.2f ns | %12.2e | %12.2e | %12.2e\n",
           name, result.avg_time_ns, result.max_abs_error, 
           result.max_rel_error, result.avg_abs_error);
}

int main() {
    printf("Fast Inverse Square Root Benchmark\n");
    printf("==================================\n");
    printf("Samples: %d, Iterations per sample: %d\n", NUM_SAMPLES, ITERATIONS_PER_SAMPLE);
    printf("Range: [%.1f, %.0e]\n\n", MIN_VALUE, MAX_VALUE);
    
    // Generate test data
    float* test_values = malloc(NUM_SAMPLES * sizeof(float));
    float* reference_values = malloc(NUM_SAMPLES * sizeof(float));
    
    generate_test_values(test_values, NUM_SAMPLES);
    
    // Calculate reference values using standard function
    for (int i = 0; i < NUM_SAMPLES; i++) {
        reference_values[i] = standard_inv_sqrt(test_values[i]);
    }
    
    // Print header
    printf("%-20s | %8s | %12s | %12s | %12s\n",
           "Algorithm", "Avg Time", "Max Abs Err", "Max Rel Err", "Avg Abs Err");
    printf("%-20s-+-%8s-+-%12s-+-%12s-+-%12s\n",
           "--------------------", "--------", "------------", 
           "------------", "------------");
    
    // Benchmark all functions
    benchmark_result_t std_result = benchmark_function(standard_inv_sqrt, test_values, reference_values, NUM_SAMPLES);
    benchmark_result_t fast1_result = benchmark_function(fast_inv_sqrt_1iter, test_values, reference_values, NUM_SAMPLES);
    benchmark_result_t fast2_result = benchmark_function(fast_inv_sqrt_2iter, test_values, reference_values, NUM_SAMPLES);
    
    print_results("Standard 1/sqrt(x)", std_result);
    print_results("Fast InvSqrt 1iter", fast1_result);
    print_results("Fast InvSqrt 2iter", fast2_result);
    
    // Performance ratios
    printf("\nPerformance Analysis:\n");
    printf("Fast 1iter speedup: %.2fx\n", std_result.avg_time_ns / fast1_result.avg_time_ns);
    printf("Fast 2iter speedup: %.2fx\n", std_result.avg_time_ns / fast2_result.avg_time_ns);
    printf("2iter vs 1iter: %.2fx slower\n", fast2_result.avg_time_ns / fast1_result.avg_time_ns);
    
    // Accuracy comparison
    printf("\nAccuracy Analysis:\n");
    printf("1iter precision loss: %.2e (%.4f%%)\n", 
           fast1_result.max_rel_error, fast1_result.max_rel_error * 100);
    printf("2iter precision loss: %.2e (%.4f%%)\n", 
           fast2_result.max_rel_error, fast2_result.max_rel_error * 100);
    
    // Test specific problematic values
    printf("\nEdge Case Analysis:\n");
    float edge_cases[] = {1.0f, 2.0f, 4.0f, 0.25f, 16.0f, 1024.0f, 1048576.0f};
    int num_edges = sizeof(edge_cases) / sizeof(edge_cases[0]);
    
    printf("%-10s | %-12s | %-12s | %-12s\n", "Input", "Standard", "Fast 1iter", "Fast 2iter");
    printf("%-10s-+-%12s-+-%12s-+-%12s\n", "----------", "------------", "------------", "------------");
    
    for (int i = 0; i < num_edges; i++) {
        float x = edge_cases[i];
        float std_val = standard_inv_sqrt(x);
        float fast1_val = fast_inv_sqrt_1iter(x);
        float fast2_val = fast_inv_sqrt_2iter(x);
        
        printf("%-10.0f | %12.8f | %12.8f | %12.8f\n", x, std_val, fast1_val, fast2_val);
    }
    
    free(test_values);
    free(reference_values);
    
    return 0;
}

// Compile with different optimization levels:
// gcc -O0 -o bench_O0 fast_isqrt_benchmark.c -lm
// gcc -O1 -o bench_O1 fast_isqrt_benchmark.c -lm  
// gcc -O2 -o bench_O2 fast_isqrt_benchmark.c -lm
// gcc -O3 -o bench_O3 fast_isqrt_benchmark.c -lm
