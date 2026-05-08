 #include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4

typedef struct {
    int threadId;
    char **lines;
    int count;
    int cur_line;
} threadData;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int turn = 0;
int thr_compl = 0;

void *routine(void *_);
void clean(void *arg);

int main(void) {
    pthread_t tids[NUM_THREADS];
    threadData thread_data[NUM_THREADS];
    
    char *lines1[] = {
        "Don't you know I'm no good for you?",
        "I've learned to lose you, can't afford to",
        "Tore my shirt to stop you bleedin'"
    };

    char *lines2[] = {
        "Say it, spit it out, what is it exactly",
        "You're payin'? Is the amount cleanin' you out? Am I satisfactory?",
        "Today, I'm thinkin' about the things that are deadly"
    };

    char *lines3[] = {
        "Careful creature made friends with time",
        "He left her lonely with a diamond mind",
        "And those ocean eyes"
    };

    char *lines4[] = {
        "She's takin' pictures in the mirror",
        "Oh, my God, her skin's so clear",
        "Tell her, -Bring that over here"
    };

    thread_data[0] = (threadData){1, lines1, sizeof(lines1) / sizeof(lines1[0])};
    thread_data[1] = (threadData){2, lines2, sizeof(lines2) / sizeof(lines2[0])};
    thread_data[2] = (threadData){3, lines3, sizeof(lines3) / sizeof(lines3[0])};
    thread_data[3] = (threadData){4, lines4, sizeof(lines4) / sizeof(lines4[0])};

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&tids[i], NULL, routine, &thread_data[i]);
    }

    char *text[] = {
        "I want you to see",
        "How you look to me",
        "You wouldn't believe if I told ya"
    };
    int count = sizeof(text) / sizeof(text[0]);

    for (int i = 0; i < count; i++) {
        pthread_mutex_lock(&mutex);
    
    
        while (turn != 0) {
            pthread_cond_wait(&cond, &mutex);
        }

        printf("main text: %d = %s\n", i + 1, text[i]);
        turn = 1;
        thr_compl = 0;
        pthread_cond_broadcast(&cond);

        while (thr_compl < NUM_THREADS) {
            pthread_cond_wait(&cond, &mutex);
        }

        pthread_mutex_unlock(&mutex);
        sleep(1);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_cancel(tids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(tids[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}

void *routine(void *arg) {
    threadData *data = (threadData*)arg;
    pthread_cleanup_push(clean, data);
    for (int i = 0; i < data->count; i++) {
        pthread_mutex_lock(&mutex);

        while (turn != 1) {
            pthread_cond_wait(&cond, &mutex);
        }
        printf("thread %d: %d %s\n", data->threadId, i + 1, data->lines[i]);
        thr_compl++;
        if (thr_compl == NUM_THREADS) {
            turn = 0;
            pthread_cond_signal(&cond);
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    pthread_cleanup_pop(0);
    return NULL;
}

void clean(void *arg) {
    threadData *data = (threadData*)arg;
    printf("thread %d is died\n", data->threadId);
}
