#include <stdio.h>

int main() {
    int n, m; // n = number of processes, m = number of resources
    int i, j, k;

    printf("Enter the number of processes: ");
    scanf("%d", &n);
    printf("Enter the number of resource types: ");
    scanf("%d", &m);

    int alloc[n][m], max[n][m], avail[m];
    int need[n][m], finish[n], safeSeq[n];
    
    printf("\nEnter the Allocation Matrix:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &alloc[i][j]);

    printf("\nEnter the Maximum Matrix:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &max[i][j]);

    printf("\nEnter the Available Resources:\n");
    for (j = 0; j < m; j++)
        scanf("%d", &avail[j]);

    // Calculate Need matrix
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];

    // Initialize Finish array
    for (i = 0; i < n; i++)
        finish[i] = 0;

    int count = 0; // Number of processes finished
    int found;

    // Main logic: Find a safe sequence
    while (count < n) {
        found = 0;
        for (i = 0; i < n; i++) {
            if (finish[i] == 0) {
                int canAllocate = 1;
                for (j = 0; j < m; j++) {
                    if (need[i][j] > avail[j]) {
                        canAllocate = 0;
                        break;
                    }
                }
                if (canAllocate) {
                    for (k = 0; k < m; k++)
                        avail[k] += alloc[i][k]; // Release resources

                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (!found) {
            printf("\nThe system is NOT in a safe state!\n");
            return 0;
        }
    }

    printf("\nThe system is in a SAFE state.\nSafe sequence is: ");
    for (i = 0; i < n; i++)
        printf("P%d ", safeSeq[i]);
    printf("\n");

    return 0;
}
