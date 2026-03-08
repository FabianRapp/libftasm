#include "libasm.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef NS_PER_SEC
# define NS_PER_SEC 1000000000ULL
#endif

#define WARMUP_ROUNDS 2000
#define BENCH_ROUNDS 20000

typedef size_t (*strlen_fn)(const char *);

typedef struct s_case {
	const char *label;
	size_t *lengths;
	size_t count;
	size_t samples_per_length;
} t_case;

volatile size_t g_sink = 0;

uint64_t now_ns(void) {
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ((uint64_t)ts.tv_sec * NS_PER_SEC + (uint64_t)ts.tv_nsec);
}

char *make_random_string(size_t len) {
	char *s = malloc(len + 1);
	if (!s) {
		assert(0);
	}
	for (size_t i = 0; i < len; i++) {
		s[i] = (char)(rand() % 256);
	}
	s[len] = '\0';
	return s;
}

char **build_samples(const size_t *lengths, size_t count, size_t samples_per_length, size_t *out_total) {
	size_t total = count * samples_per_length;
	char **samples = malloc(sizeof(char *) * total);
	if (!samples) {
		assert(0);
	}
	size_t k = 0;
	for (size_t i = 0; i < count; i++) {
		for (size_t j = 0; j < samples_per_length; j++) {
			samples[k++] = make_random_string(lengths[i]);
		}
	}
	*out_total = total;
	return samples;
}

void shuffle_samples(char **samples, size_t total) {
	if (total < 2) {
		return ;
	}
	for (size_t i = total - 1; i > 0; i--) {
		size_t j = (size_t)(rand() % (i + 1));
		char *tmp = samples[i];
		samples[i] = samples[j];
		samples[j] = tmp;
	}
}

void free_samples(char **samples, size_t total) {
	for (size_t i = 0; i < total; i++) {
		free(samples[i]);
	}
	free(samples);
}

uint64_t run_bench(strlen_fn fn, char **samples, size_t total, size_t rounds) {
	uint64_t start = now_ns();
	for (size_t r = 0; r < rounds; r++) {
		for (size_t i = 0; i < total; i++) {
			g_sink += fn(samples[i]);
		}
	}
	uint64_t end = now_ns();
	return end - start;
}

void benchmark_group(const char *label, const size_t *lengths, size_t count, size_t samples_per_length) {
	size_t total;
	char **samples = build_samples(lengths, count, samples_per_length, &total);
	shuffle_samples(samples, total);

	for (size_t i = 0; i < total; i++) {
		assert(ft_strlen(samples[i]) == strlen(samples[i]));
	}

	for (size_t i = 0; i < WARMUP_ROUNDS; i++) {
		size_t idx = (size_t)(rand() % total);
		if (i & 1) {
			g_sink += ft_strlen(samples[idx]);
			g_sink += strlen(samples[idx]);
		} else {
			g_sink += strlen(samples[idx]);
			g_sink += ft_strlen(samples[idx]);
		}
	}

	uint64_t my_accum = 0;
	uint64_t std_accum = 0;
	for (size_t i = 0; i < 6; i++) {
		shuffle_samples(samples, total);
		uint64_t my_time;
		uint64_t std_time;
		if (i & 1) {
			std_time = run_bench(strlen, samples, total, BENCH_ROUNDS);
			my_time = run_bench(ft_strlen, samples, total, BENCH_ROUNDS);
		} else {
			my_time = run_bench(ft_strlen, samples, total, BENCH_ROUNDS);
			std_time = run_bench(strlen, samples, total, BENCH_ROUNDS);
		}
		my_accum += my_time;
		std_accum += std_time;
	}

	printf("== %s ==\n", label);
	printf("samples: %zu, rounds: %d\n", total, BENCH_ROUNDS);
	printf("ft_strlen : %.3f ms\n", (double)my_accum / 6.0 / 1000000.0);
	printf("strlen    : %.3f ms\n", (double)std_accum / 6.0 / 1000000.0);
	if (std_accum != 0) {
		printf("ratio     : %.3f x\n\n", (double)my_accum / (double)std_accum);
	} else {
		printf("ratio     : n/a\n\n");
	}

	free_samples(samples, total);
}

void bench_strlen(void) {
	size_t empty_lengths[] = {0};
	size_t small_lengths[] = {1, 2, 3, 7, 15, 31};
	size_t medium_lengths[] = {32, 64, 128, 256, 512};
	size_t large_lengths[] = {1024, 4096, 16384, 65536};
	size_t huge_lengths[] = {262144, 1048576};

	printf("Benchmarking ft_strlen vs strlen\n");
	printf("(volatile sink = %zu before bench)\n\n", g_sink);

	benchmark_group("empty strings", empty_lengths,
		sizeof(empty_lengths) / sizeof(empty_lengths[0]), 128);
	benchmark_group("small strings", small_lengths,
		sizeof(small_lengths) / sizeof(small_lengths[0]), 256);
	benchmark_group("medium strings", medium_lengths,
		sizeof(medium_lengths) / sizeof(medium_lengths[0]), 128);
	benchmark_group("large strings", large_lengths,
		sizeof(large_lengths) / sizeof(large_lengths[0]), 32);
	benchmark_group("huge strings", huge_lengths,
		sizeof(huge_lengths) / sizeof(huge_lengths[0]), 8);

	printf("ignore this: %zu\n", g_sink);
}

int main(void) {
	srand(time(NULL));
	bench_strlen();
	return 0;
}
