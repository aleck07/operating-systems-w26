#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "ppm.h"

struct thread_data {
	int thread_id;
	struct ppm *image;
	int start_y;
	int slice_height;
	long long sum_r;
	long long sum_g;
	long long sum_b;
};

static void *grayscale_slice(void *arg)
{
	struct thread_data *data = arg;
	int width = data->image->width;
	int end_y = data->start_y + data->slice_height;

	for (int y = data->start_y; y < end_y; y++) {
		for (int x = 0; x < width; x++) {
			int pixel = ppm_get_pixel(data->image, x, y);
			int r = PPM_PIXEL_R(pixel);
			int g = PPM_PIXEL_G(pixel);
			int b = PPM_PIXEL_B(pixel);

			data->sum_r += r;
			data->sum_g += g;
			data->sum_b += b;

			int gray = (299 * r + 587 * g + 114 * b) / 1000;
			int gray_pixel = PPM_PIXEL(gray, gray, gray);
			ppm_set_pixel(data->image, x, y, gray_pixel);
		}
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	// Check arguments
	if (argc != 4) {
		fprintf(stderr, "Usage: %s <threads> <input.ppm> <output.ppm>\n", argv[0]);
		return 1;
	}

	// Ensure positive thread count
	int thread_count = atoi(argv[1]);
	if (thread_count <= 0) {
		fprintf(stderr, "Error: thread count must be positive\n");
		return 1;
	}

	// Make sure we can load the image
	struct ppm *image = ppm_read(argv[2]);
	if (image == NULL) {
		fprintf(stderr, "Error: unable to read input file\n");
		return 1;
	}

	int height = image->height;
	int width = image->width;

	if (thread_count > height) {
		thread_count = height;
	}

	pthread_t *threads = calloc((size_t)thread_count, sizeof(*threads));
	struct thread_data *thread_data = calloc((size_t)thread_count, sizeof(*thread_data));
	if (threads == NULL || thread_data == NULL) {
		fprintf(stderr, "Error: unable to allocate thread data\n");
		ppm_free(image);
		free(threads);
		free(thread_data);
		return 1;
	}

	int base_height = height / thread_count;
	int remainder = height % thread_count;
	int current_y = 0;

	for (int i = 0; i < thread_count; i++) {
		int slice_height = base_height;
		if (i == thread_count - 1) {
			slice_height += remainder;
		}

		thread_data[i].thread_id = i;
		thread_data[i].image = image;
		thread_data[i].start_y = current_y;
		thread_data[i].slice_height = slice_height;
		thread_data[i].sum_r = 0;
		thread_data[i].sum_g = 0;
		thread_data[i].sum_b = 0;

		if (pthread_create(&threads[i], NULL, grayscale_slice, &thread_data[i]) != 0) {
			fprintf(stderr, "Error: unable to create thread %d\n", i);
			ppm_free(image);
			free(threads);
			free(thread_data);
			return 1;
		}

		printf("Thread %d: %d %d\n", i, current_y, slice_height);
		current_y += slice_height;
	}

	long long total_r = 0;
	long long total_g = 0;
	long long total_b = 0;

	for (int i = 0; i < thread_count; i++) {
		pthread_join(threads[i], NULL);
		total_r += thread_data[i].sum_r;
		total_g += thread_data[i].sum_g;
		total_b += thread_data[i].sum_b;
	}

	long long pixel_count = (long long)width * (long long)height;
	int avg_r = (int)(total_r / pixel_count);
	int avg_g = (int)(total_g / pixel_count);
	int avg_b = (int)(total_b / pixel_count);

	printf("Average R: %d\n", avg_r);
	printf("Average G: %d\n", avg_g);
	printf("Average B: %d\n", avg_b);

	if (ppm_write(image, argv[3]) != 0) {
		fprintf(stderr, "Error: unable to write output file\n");
		ppm_free(image);
		free(threads);
		free(thread_data);
		return 1;
	}

	ppm_free(image);
	free(threads);
	free(thread_data);
	return 0;
}

