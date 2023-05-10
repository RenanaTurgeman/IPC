    struct timeval start, end;
    gettimeofday(&start, NULL); // get start time before start to send
    gettimeofday(&end, NULL); // get end time after finish to send
long seconds = end.tv_sec - start.tv_sec;
    long useconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds * 1000.0 + useconds / 1000.0;
    printf("Time taken in funnction: %.2f ms\n", elapsed);
    return 0;