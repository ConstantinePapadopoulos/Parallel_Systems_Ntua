#include <stdio.h>
#include <stdlib.h>
#include "kmeans.h"
/*
 * TODO: include openmp header file
 */ 
#include "omp.h"
#define CACHE_LINE_SIZE 64  // Assuming 64 bytes cache line size
#define PAD(n) (((n) + CACHE_LINE_SIZE - 1) / CACHE_LINE_SIZE * CACHE_LINE_SIZE)

// square of Euclid distance between two multi-dimensional points
inline static double euclid_dist_2(int    numdims,  /* no. dimensions */
								 double * coord1,   /* [numdims] */
								 double * coord2)   /* [numdims] */
{
	int i;
	double ans = 0.0;

	for(i=0; i<numdims; i++)
		ans += (coord1[i]-coord2[i]) * (coord1[i]-coord2[i]);

	return ans;
}

inline static int find_nearest_cluster(int      numClusters, /* no. clusters */
									   int      numCoords,   /* no. coordinates */
									   double * object,      /* [numCoords] */
									   double * clusters)    /* [numClusters][numCoords] */
{
	int index, i;
	double dist, min_dist;

	// find the cluster id that has min distance to object 
	index = 0;
	min_dist = euclid_dist_2(numCoords, object, clusters);

	for(i=1; i<numClusters; i++) {
		dist = euclid_dist_2(numCoords, object, &clusters[i*numCoords]);
		// no need square root 
		if (dist < min_dist) { // find the min and its array index
			min_dist = dist;
			index    = i;
		}
	}
	return index;
}

void kmeans(double * objects,          /* in: [numObjs][numCoords] */
			int      numCoords,        /* no. coordinates */
			int      numObjs,          /* no. objects */
			int      numClusters,      /* no. clusters */
			double   threshold,        /* minimum fraction of objects that change membership */
			long     loop_threshold,   /* maximum number of iterations */
			int    * membership,       /* out: [numObjs] */
			double * clusters)         /* out: [numClusters][numCoords] */
{
	int i, j, k;
	int index, loop=0;
	double timing = 0;

	double delta;          // fraction of objects whose clusters change in each loop 
	int * newClusterSize; // [numClusters]: no. objects assigned in each new cluster 
	double * newClusters;  // [numClusters][numCoords] 
	int nthreads;         // no. threads 

	nthreads = omp_get_max_threads();
	printf("OpenMP Kmeans - Reduction\t(number of threads: %d)\n", nthreads);

	// initialize membership
	for (i=0; i<numObjs; i++)
		membership[i] = -1;

	// initialize newClusterSize and newClusters to all 0 
	newClusterSize = (typeof(newClusterSize)) calloc(numClusters, sizeof(*newClusterSize));
	newClusters = (typeof(newClusters))  calloc(numClusters * numCoords, sizeof(*newClusters));

	// Each thread calculates new centers using a private space. After that, thread 0 does an array reduction on them.
	int * local_newClusterSize[nthreads];  // [nthreads][numClusters] 
	double * local_newClusters[nthreads];   // [nthreads][numClusters][numCoords]

	/*
	 * Hint for false-sharing
	 * This is noticed when numCoords is low (and neighboring local_newClusters exist close to each other).
	 * Allocate local cluster data with a "first-touch" policy.
	 */


	// Initialize local (per-thread) arrays (and later collect result on global arrays)
	#pragma omp parallel private(k, i, j)
	{
    	int thread_id = omp_get_thread_num();
	    // Allocate and initialize each thread's local array
	    local_newClusterSize[thread_id] = (typeof(*local_newClusterSize)) malloc(numClusters * sizeof(**local_newClusterSize));
	    local_newClusters[thread_id] = (typeof(*local_newClusters)) malloc(numClusters *numCoords * sizeof(**local_newClusters));
	
        int c, coord;
    	//First-touch initialization
    	for (c = 0; c < numClusters; c++) {
        	local_newClusterSize[thread_id][c] = 0;
        	for (coord = 0; coord < numCoords; coord++) {
            	local_newClusters[thread_id][c * numCoords + coord] = 0.0;
        	}
    	}
	}

	timing = wtime();
	do {
		// before each loop, set cluster data to 0
		for (i=0; i<numClusters; i++) {
			for (j=0; j<numCoords; j++)
				newClusters[i*numCoords + j] = 0.0;
			newClusterSize[i] = 0;
		}
		delta = 0.0;

			#pragma omp parallel private(index, j, i)
			{
			    int thread_id = omp_get_thread_num();

			    // Initialize local_newClusterSize and local_newClusters to zero for each thread
			    int c, coord;
			    for (c = 0; c < numClusters; c++) {
			        local_newClusterSize[thread_id][c] = 0;
			        for (coord = 0; coord < numCoords; coord++) {
			            local_newClusters[thread_id][c * numCoords + coord] = 0.0;
			        }
			    }

			    #pragma omp for reduction(+:delta)
			    for (i = 0; i < numObjs; i++) {
			        // Find the array index of nearest cluster center
			        index = find_nearest_cluster(numClusters, numCoords, &objects[i * numCoords], clusters);

			        // If membership changes, increase delta by 1
			        if (membership[i] != index)
			            delta += 1.0;

			        // Assign the membership to object i
			        membership[i] = index;

			        // Update local cluster centers for each thread
			        local_newClusterSize[thread_id][index]++;
			        for (j = 0; j < numCoords; j++) {
			            local_newClusters[thread_id][index * numCoords + j] += objects[i * numCoords + j];
			        }
			    }
			}


		/*
		 * TODO: Reduction of cluster data from local arrays to shared.
		 *       This operation will be performed by one thread
		 */
			for (k=0; k<nthreads; k++){

		for(i=0; i<numClusters; i++){
				newClusterSize[i]+= local_newClusterSize[k][i];
				for(j=0; j<numCoords; j++){
					newClusters[i * numCoords + j] +=local_newClusters[k][i * numCoords + j];


				}
				//printf("Hi: %7.4f\t",newClusters[i * numCoords + j])
			}
		}


		// average the sum and replace old cluster centers with newClusters 
		for (i=0; i<numClusters; i++) {
			if (newClusterSize[i] > 0) {
				for (j=0; j<numCoords; j++) {
					clusters[i*numCoords + j] = newClusters[i*numCoords + j] / newClusterSize[i];
				}
			}
		}

		// Get fraction of objects whose membership changed during this loop. This is used as a convergence criterion.
		delta /= numObjs;

		loop++;
		printf("\r\tcompleted loop %d", loop);
		fflush(stdout);
	} while (delta > threshold && loop < loop_threshold);
	timing = wtime() - timing;
	printf("\n        nloops = %3d   (total = %7.4fs)  (per loop = %7.4fs)\n", loop, timing, timing/loop);

	for (k=0; k<nthreads; k++)
	{
		free(local_newClusterSize[k]);
		free(local_newClusters[k]);
	}
	free(newClusters);
	free(newClusterSize);
}

