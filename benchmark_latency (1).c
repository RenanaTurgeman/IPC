#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>

#define ITERS 1000000
pthread_mutex_t mutex;
pthread_cond_t cond;
int flag = 1;


void *thread_func(void *arg) {
  int fd = *((int*) arg);
  struct timespec start, end;
  struct flock lock;
  lock.l_type = F_WRLCK;
  lock.l_whence = SEEK_SET;
  lock.l_start = 0;
  lock.l_len = 0;

  clock_gettime(CLOCK_REALTIME, &start);
  for (int i = 0; i < ITERS; i++) {
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
      perror("fcntl");
      exit(EXIT_FAILURE);
    }
    if (fcntl(fd, F_UNLCK, &lock) == -1) {
      perror("fcntl");
      exit(EXIT_FAILURE);
    }
  }
  clock_gettime(CLOCK_REALTIME, &end);

  long long elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
  double double_time = elapsed_ns / (double) 1000000000; 
  printf("process by fcntl- wake a task using lock: %f seconds\n\n", double_time);
  return NULL;
} 

void fcntl_process(){
    pthread_t tid;
    int fd;

    // open a temporary file
    if ((fd = open("/tmp/lock_file", O_CREAT | O_RDWR | O_TRUNC, 0666)) == -1) {
      perror("open");
      exit(EXIT_FAILURE);
    }

    // create the thread
    if (pthread_create(&tid, NULL, thread_func, (void*) &fd) != 0) {
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }

    // join the thread and clean up
    pthread_join(tid, NULL);
    close(fd);
    unlink("/tmp/lock_file");
  }


void *thread_func2(void *arg) {
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < ITERS; i++) {
        pthread_mutex_lock(&mutex);
        pthread_mutex_unlock(&mutex);
    }
    clock_gettime(CLOCK_REALTIME, &end);

    long long elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    double double_time = elapsed_ns / (double) 1000000000; 
    printf("thread by mutex- wake a task using lock: %f seconds\n\n", double_time);
    return NULL;
}

void mutex_thread(){
    pthread_t tid;

    // initialize mutex
    pthread_mutex_init(&mutex, NULL);

    // // create the thread
    if (pthread_create(&tid, NULL, thread_func2, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    // join the thread and clean up
    pthread_join(tid, NULL);
    pthread_mutex_destroy(&mutex);
}

void *thread_func3(void *arg) {
  for (int i = 0; i < ITERS; i++) {
    pthread_mutex_lock(&mutex);
    if (flag == 1)
    {
      flag = 2;
      pthread_cond_wait(&cond, &mutex);
    }
    else {
      flag = 1;
      pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

void cond_thread(){
  pthread_t tid1, tid2;

    // initialize mutex and condition variable
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    // create the first thread
    if (pthread_create(&tid1, NULL, thread_func3, NULL) != 0) {
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }

    // create the second thread
    if (pthread_create(&tid2, NULL, thread_func3, NULL) != 0) {
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }

    // join the thread and clean up
    pthread_join(tid2, NULL);
    clock_gettime(CLOCK_REALTIME, &end);
    long long elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    double double_time = elapsed_ns / (double) 1000000000; 
    printf("thread by cond- wake a task using signal: %f seconds\n\n", double_time);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

void pipe_process(){
  struct timespec start, end;
  int pipefd[2];
  char c;

  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  clock_gettime(CLOCK_REALTIME, &start);
  pid_t pid = fork();
  if (pid == 0) {
    // child process
    close(pipefd[1]); // close write end
    for (int i = 0; i < ITERS/2; i++) {
      read(pipefd[0], &c, 1);
      write(pipefd[0], &c, 1);
    }
  } else {
    // parent process
    close(pipefd[0]); // close read end 
    for (int i = 0; i < ITERS/2; i++) {
      write(pipefd[1], "x", 1);
      read(pipefd[1], &c, 1);
    }
    close(pipefd[1]); // close write end
    clock_gettime(CLOCK_REALTIME, &end);
    long long elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    double double_time = elapsed_ns / (double) 1000000000; 
    printf("process by pipe- wake a task using signal: %f seconds\n\n", double_time);
  }
}


int main(int argc, char *argv[]) {
  
  printf("\n");
  fcntl_process();
  mutex_thread();
  cond_thread();
  pipe_process();




  return 0;
}