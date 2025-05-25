#!/bin/bash

# Fast Inverse Square Root Benchmark Runner
# Compiles with different optimization levels and runs benchmarks

set -e  # Exit on any error

# Configuration
SOURCE_FILE="fast_isqrt_benchmark.c"
OUTPUT_DIR="benchmark_results"
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
RESULT_FILE="${OUTPUT_DIR}/benchmark_${TIMESTAMP}.txt"

# Optimization levels to test
OPT_LEVELS=("O0" "O1" "O2" "O3" "Ofast")

# Compiler configurations
declare -A COMPILERS=(
    ["gcc"]="gcc"
    ["clang"]="clang"
)

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_header() {
    echo -e "${BLUE}================================================${NC}"
    echo -e "${BLUE}    Fast Inverse Square Root Benchmark Suite${NC}"
    echo -e "${BLUE}================================================${NC}"
    echo "Timestamp: $(date)"
    echo "Host: $(hostname)"
    echo "CPU: $(grep 'model name' /proc/cpuinfo | head -1 | cut -d: -f2 | xargs)"
    echo "Cores: $(nproc)"
    echo ""
}

check_dependencies() {
    echo -e "${YELLOW}Checking dependencies...${NC}"
    
    if [ ! -f "$SOURCE_FILE" ]; then
        echo -e "${RED}Error: $SOURCE_FILE not found!${NC}"
        exit 1
    fi
    
    for compiler in "${!COMPILERS[@]}"; do
        if ! command -v "${COMPILERS[$compiler]}" &> /dev/null; then
            echo -e "${YELLOW}Warning: ${COMPILERS[$compiler]} not found, skipping${NC}"
            unset COMPILERS[$compiler]
        fi
    done
    
    if [ ${#COMPILERS[@]} -eq 0 ]; then
        echo -e "${RED}Error: No compilers available!${NC}"
        exit 1
    fi
    
    echo -e "${GREEN}Dependencies OK${NC}"
    echo ""
}

compile_and_run() {
    local compiler=$1
    local compiler_cmd=$2
    local opt_level=$3
    local binary_name="bench_${compiler}_${opt_level}"
    
    echo -e "${YELLOW}Compiling with $compiler -$opt_level...${NC}"
    
    # Compile
    if $compiler_cmd -$opt_level -o "$binary_name" "$SOURCE_FILE" -lm 2>/dev/null; then
        echo -e "${GREEN}✓ Compilation successful${NC}"
        
        # Get binary info
        local binary_size=$(stat -c%s "$binary_name")
        echo "Binary size: $binary_size bytes"
        
        echo -e "${BLUE}Running benchmark...${NC}"
        echo ""
        
        # Run benchmark and capture output
        {
            echo "========================================"
            echo "COMPILER: $compiler -$opt_level"
            echo "BINARY: $binary_name ($binary_size bytes)"
            echo "TIMESTAMP: $(date)"
            echo "========================================"
            ./"$binary_name"
            echo ""
        } | tee -a "$RESULT_FILE"
        
        # Cleanup binary
        rm -f "$binary_name"
        
    else
        echo -e "${RED}✗ Compilation failed${NC}"
        {
            echo "========================================"
            echo "COMPILER: $compiler -$opt_level"
            echo "STATUS: COMPILATION FAILED"
            echo "========================================"
            echo ""
        } >> "$RESULT_FILE"
    fi
    
    echo ""
}

generate_summary() {
    echo -e "${BLUE}Generating performance summary...${NC}"
    
    local summary_file="${OUTPUT_DIR}/summary_${TIMESTAMP}.txt"
    
    {
        echo "PERFORMANCE SUMMARY"
        echo "==================="
        echo "Generated: $(date)"
        echo ""
        
        echo "Standard 1/sqrt(x) Performance:"
        grep "Standard 1/sqrt(x)" "$RESULT_FILE" | while read -r line; do
            local compiler_line=$(grep -B5 "$line" "$RESULT_FILE" | grep "COMPILER:" | tail -1)
            echo "  $compiler_line -> $line"
        done
        echo ""
        
        echo "Fast InvSqrt 1iter Performance:"
        grep "Fast InvSqrt 1iter" "$RESULT_FILE" | while read -r line; do
            local compiler_line=$(grep -B5 "$line" "$RESULT_FILE" | grep "COMPILER:" | tail -1)
            echo "  $compiler_line -> $line"
        done
        echo ""
        
        echo "Speedup Analysis:"
        grep "Fast 1iter speedup:" "$RESULT_FILE" | while read -r line; do
            local compiler_line=$(grep -B10 "$line" "$RESULT_FILE" | grep "COMPILER:" | tail -1)
            echo "  $compiler_line -> $line"
        done
        
    } > "$summary_file"
    
    echo -e "${GREEN}Summary saved to: $summary_file${NC}"
}

main() {
    print_header
    check_dependencies
    
    # Create output directory
    mkdir -p "$OUTPUT_DIR"
    
    # Initialize result file
    {
        echo "Fast Inverse Square Root Benchmark Results"
        echo "=========================================="
        echo "Generated: $(date)"
        echo "Host: $(hostname)"
        echo "CPU: $(grep 'model name' /proc/cpuinfo | head -1 | cut -d: -f2 | xargs)"
        echo ""
    } > "$RESULT_FILE"
    
    # Run benchmarks for each compiler and optimization level
    for compiler in "${!COMPILERS[@]}"; do
        echo -e "${BLUE}Testing with $compiler...${NC}"
        
        for opt_level in "${OPT_LEVELS[@]}"; do
            compile_and_run "$compiler" "${COMPILERS[$compiler]}" "$opt_level"
        done
        
        echo -e "${GREEN}Completed $compiler tests${NC}"
        echo ""
    done
    
    generate_summary
    
    echo -e "${GREEN}All benchmarks completed!${NC}"
    echo -e "Results saved to: ${BLUE}$RESULT_FILE${NC}"
    echo ""
    
    # Show quick results preview
    echo -e "${YELLOW}Quick Preview (Fast 1iter speedups):${NC}"
    grep "Fast 1iter speedup:" "$RESULT_FILE" | while read -r line; do
        local compiler_line=$(grep -B10 "$line" "$RESULT_FILE" | grep "COMPILER:" | tail -1)
        echo "  $(echo $compiler_line | cut -d: -f2 | xargs) -> $line"
    done
}

# Cleanup function
cleanup() {
    echo -e "\n${YELLOW}Cleaning up temporary files...${NC}"
    rm -f bench_*_O*
}

# Set trap for cleanup
trap cleanup EXIT

# Run main function
main "$@"
