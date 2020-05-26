#include "lab3.h"
#include <iostream>
#include <windows.h>


#define THREADCOUNT 11

DWORD ThreadID; // номер потоков     //интервал времени
HANDLE fThread[THREADCOUNT]; // сихрон. доступ в критическую секцию   //дескриптор объекта
HANDLE fMutex;

HANDLE sem_F, sem_G, sem_H; // семафоры для сихрон.
HANDLE sem_pause, sem_next;

DWORD WINAPI Thread_a(LPVOID);
DWORD WINAPI Thread_b(LPVOID);
DWORD WINAPI Thread_c(LPVOID);
DWORD WINAPI Thread_d(LPVOID);
DWORD WINAPI Thread_e(LPVOID);
DWORD WINAPI Thread_f(LPVOID);
DWORD WINAPI Thread_g(LPVOID);
DWORD WINAPI Thread_h(LPVOID);
DWORD WINAPI Thread_i(LPVOID);
DWORD WINAPI Thread_k(LPVOID);
DWORD WINAPI Thread_m(LPVOID);

unsigned int lab3_thread_graph_id()
{
    return 17;
}

const char* lab3_unsynchronized_threads()
{
    return "bcd";
}

const char* lab3_sequential_threads()
{
    return "fgh";
}

DWORD WINAPI Thread_a(LPVOID lpParam) {

    UNREFERENCED_PARAMETER(lpParam);
    //////////////////1///////////////////////
    // THREAD B
    fThread[1] = CreateThread(NULL, 0, Thread_b, NULL, 0, &ThreadID);
    // (1)NULL - аттрибуты безопасности
    // (2)0 - размер стека 
    // (4)NULL - нет аргумента для функ.- тела потока
    // (5)0 - флаг соз. потока
    if (fThread[1] == NULL)
    {
        std::cout << "ERROR - Create Thread: " << GetLastError();
        return 1;
    }

    // THREAD D 
    fThread[3] = CreateThread(NULL, 0, Thread_d, NULL, 0, &ThreadID);
    // (1)NULL - аттрибуты безопасности
    // (2)0 - размер стека
    // (4)NULL - нет аргумента для функ.- тела потока
    // (5)0 - флаг соз. потока
    if (fThread[3] == NULL)
    {
        std::cout << "ERROR - Create Thread: " << GetLastError();
        return 1;
    }

    for (int a = 0; a < 3; a++) {
        WaitForSingleObject(fMutex, INFINITE); // ждем пока поток  закончит работу
        std::cout << "a" << std::flush;
        computation();
        ReleaseMutex(fMutex);
    }

    WaitForSingleObject(sem_pause, INFINITE); // b
    WaitForSingleObject(sem_pause, INFINITE); // d

    // THREAD C
    fThread[2] = CreateThread(NULL, 0, Thread_c, NULL, 0, &ThreadID);
    // (1)NULL - аттрибуты безопасности
    // (2)0 - размер стека
    // (4)NULL - нет аргумента для функ.- тела потока
    // (5)0 - флаг соз. потока
    if (fThread[2] == NULL)
    {
        std::cout << "ERROR - Create Thread: " << GetLastError();
        return 1;
    }

    WaitForSingleObject(fThread[2], INFINITE); //c wait

    return 0;
}

DWORD WINAPI Thread_b(LPVOID lpParam) {

    UNREFERENCED_PARAMETER(lpParam);
    ////////////////1/////////////////////
    for (int a = 0; a < 3; a++)
    {
        WaitForSingleObject(fMutex, INFINITE); // ждем пока поток закончит работу
        std::cout << "b" << std::flush;
        computation();
        ReleaseMutex(fMutex);
    }

    ReleaseSemaphore(sem_pause, 1, NULL);
    WaitForSingleObject(sem_next, INFINITE);

    ///////////////////2///////////////////////
    for (int a = 0; a < 3; a++)
    {
        WaitForSingleObject(fMutex, INFINITE); // ждем пока поток закончит работу
        std::cout << "b" << std::flush;
        computation();
        ReleaseMutex(fMutex);
    }

    return 0;
}

DWORD WINAPI Thread_d(LPVOID lpParam) {
    UNREFERENCED_PARAMETER(lpParam);
    ////////////////1///////////////////////
    for (int a = 0; a < 3; a++)
    {
        WaitForSingleObject(fMutex, INFINITE); // ждем пока поток  закончит работу
        std::cout << "d" << std::flush;
        computation();
        ReleaseMutex(fMutex);
    }

    ReleaseSemaphore(sem_pause, 1, NULL);
    WaitForSingleObject(sem_next, INFINITE);

    ///////////////2///////////////////////
    for (int a = 0; a < 3; a++)
    {
        WaitForSingleObject(fMutex, INFINITE); // ждем пока поток  закончит работу
        std::cout << "d" << std::flush;
        computation();
        ReleaseMutex(fMutex);
    }

    ReleaseSemaphore(sem_pause, 1, NULL);
    WaitForSingleObject(sem_next, INFINITE);

    /////////////////3//////////////////////
    for (int a = 0; a < 3; a++)
    {
        WaitForSingleObject(fMutex, INFINITE); // ждем пока поток  закончит работу
        std::cout << "d" << std::flush;
        computation();
        ReleaseMutex(fMutex);
    }
    return 0;
}

DWORD WINAPI Thread_c(LPVOID lpParam) {
    UNREFERENCED_PARAMETER(lpParam);
    /////////////////2//////////////////////////////
    ReleaseSemaphore(sem_next, 1, NULL); // b
    ReleaseSemaphore(sem_next, 1, NULL); // d

    for (int a = 0; a < 3; a++)
    {
        WaitForSingleObject(fMutex, INFINITE); // ждем пока поток  закончит работу
        std::cout << "c" << std::flush;
        computation();
        ReleaseMutex(fMutex);
    }

    WaitForSingleObject(fThread[1], INFINITE); //b wait
    WaitForSingleObject(sem_pause, INFINITE); // d


    // THREAD E
    fThread[4] = CreateThread(NULL, 0, Thread_e, NULL, 0, &ThreadID);
    // (1)NULL - аттрибуты безопасности
    // (2)0 - размер стека
    // (4)NULL - нет аргумента для функ.- тела потока
    // (5)0 - флаг соз. потока
    if (fThread[4] == NULL)
    {
        std::cout << "ERROR - Create Thread: " << GetLastError();
        return 1;
    }

    WaitForSingleObject(fThread[4], INFINITE); //e wait

    return 0;
}

DWORD WINAPI Thread_e(LPVOID lpParam) {
    UNREFERENCED_PARAMETER(lpParam);
    ////////////////3/////////////////////
    // THREAD F
    fThread[5] = CreateThread(NULL, 0, Thread_f, NULL, 0, &ThreadID);
    // (1)NULL - аттрибуты безопасности
    // (2)0 - размер стека
    // (4)NULL - нет аргумента для функ.- тела потока
    // (5)0 - флаг соз. потока
    if (fThread[5] == NULL)
    {
        std::cout << "ERROR - Create Thread: " << GetLastError();
        return 1;
    }

    // THREAD H
    fThread[6] = CreateThread(NULL, 0, Thread_h, NULL, 0, &ThreadID);
    // (1)NULL - аттрибуты безопасности
    // (2)0 - размер стека
    // (4)NULL - нет аргумента для функ.- тела потока
    // (5)0 - флаг соз. потока
    if (fThread[6] == NULL)
    {
        std::cout << "ERROR - Create Thread: " << GetLastError();
        return 1;
    }

    ReleaseSemaphore(sem_next, 1, NULL); // d

    for (int a = 0; a < 3; a++)
    {
        WaitForSingleObject(fMutex, INFINITE); // ждем пока поток  закончит работу
        std::cout << "e" << std::flush;
        computation();
        ReleaseMutex(fMutex);
    }

    WaitForSingleObject(fThread[3], INFINITE); //d wait


    // THREAD G
    fThread[7] = CreateThread(NULL, 0, Thread_g, NULL, 0, &ThreadID);
    // (1)NULL - аттрибуты безопасности
    // (2)0 - размер стека
    // (4)NULL - нет аргумента для функ.- тела потока
    // (5)0 - флаг соз. потока
    if (fThread[4] == NULL)
    {
        std::cout << "ERROR - Create Thread: " << GetLastError();
        return 1;
    }

    WaitForSingleObject(fThread[7], INFINITE); //g wait

    return 0;
}

DWORD WINAPI Thread_f(LPVOID lpParam)
{
    UNREFERENCED_PARAMETER(lpParam);
    ///////////////3/////////////////
    for (int a = 0; a < 3; a++)
    {
        WaitForSingleObject(fMutex, INFINITE); // ждем пока поток  закончит работу
        std::cout << "f" << std::flush;
        computation();
        ReleaseMutex(fMutex);
    }

    ReleaseSemaphore(sem_pause, 1, NULL);
    WaitForSingleObject(sem_next, INFINITE);
    //////////////4//////////////////
    for (int a = 0; a < 3; a++)
    {
        WaitForSingleObject(sem_F, INFINITY);
        // sem_* дескриптор семафора
        // INFINITY время ожидания
        WaitForSingleObject(fMutex, INFINITE); // ждем пока поток  закончит работу
        std::cout << "f" << std::flush;
        computation();
        ReleaseMutex(fMutex);
        ReleaseSemaphore(sem_G, 1, NULL);
        //sem_*  // дескриптор семафора
        // 1           // увеличиваем значение счетчика на единицу
        // NULL      // игнорируем предыдущее значение счетчика
    }

    return 0;
}

DWORD WINAPI Thread_h(LPVOID lpParam)
{
    UNREFERENCED_PARAMETER(lpParam);
    /////////////3/////////////////////
    for (int a = 0; a < 3; a++)
    {
        WaitForSingleObject(fMutex, INFINITE); // ждем пока поток  закончит работу
        std::cout << "h" << std::flush;
        computation();
        ReleaseMutex(fMutex);
    }

    ReleaseSemaphore(sem_pause, 1, NULL);
    WaitForSingleObject(sem_next, INFINITE);
    ////////////////4//////////////////////
    for (int a = 0; a < 3; a++)
    {
        WaitForSingleObject(sem_H, INFINITY);
        // sem_* дескриптор семафора
        // INFINITY время ожидания
        WaitForSingleObject(fMutex, INFINITE); // ждем пока поток  закончит работу
        std::cout << "h" << std::flush;
        computation();
        ReleaseMutex(fMutex);
        ReleaseSemaphore(sem_F, 1, NULL);
        //sem_*  // дескриптор семафора
        // 1           // увеличиваем значение счетчика на единицу
        // NULL      // игнорируем предыдущее значение счетчика
    }

    ReleaseSemaphore(sem_pause, 1, NULL);
    WaitForSingleObject(sem_next, INFINITE);
    ///////////////5///////////////////
    for (int a = 0; a < 3; a++)
    {
        WaitForSingleObject(fMutex, INFINITE); // ждем пока поток  закончит работу
        std::cout << "h" << std::flush;
        computation();
        ReleaseMutex(fMutex);
    }
    return 0;
}

DWORD WINAPI Thread_g(LPVOID lpParam)
{
    UNREFERENCED_PARAMETER(lpParam);
    ///////////////4//////////////
    ReleaseSemaphore(sem_next, 1, NULL); // h
    ReleaseSemaphore(sem_next, 1, NULL); // f

    for (int a = 0; a < 3; a++)
    {
        WaitForSingleObject(sem_G, INFINITY);
        // sem_* дескриптор семафора
        // INFINITY время ожидания
        WaitForSingleObject(fMutex, INFINITE); // ждем пока поток  закончит работу
        std::cout << "g" << std::flush;
        computation();
        ReleaseMutex(fMutex);
        ReleaseSemaphore(sem_H, 1, NULL);
        //sem_*  // дескриптор семафора
        // 1           // увеличиваем значение счетчика на единицу
        // NULL      // игнорируем предыдущее значение счетчика
    }

    WaitForSingleObject(fThread[5], INFINITE); //f



    // THREAD I
    fThread[8] = CreateThread(NULL, 0, Thread_i, NULL, 0, &ThreadID);
    // (1)NULL - аттрибуты безопасности
    // (2)0 - размер стека
    // (4)NULL - нет аргумента для функ.- тела потока
    // (5)0 - флаг соз. потока
    if (fThread[8] == NULL)
    {
        std::cout << "ERROR - Create Thread: " << GetLastError();
        return 1;
    }

    WaitForSingleObject(fThread[8], INFINITE); //i wait

    return 0;
}


DWORD WINAPI Thread_i(LPVOID lpParam)
{
    UNREFERENCED_PARAMETER(lpParam);
    //////////////////5///////////////////
    ReleaseSemaphore(sem_next, 1, NULL); // h
    // THREAD K
    fThread[9] = CreateThread(NULL, 0, Thread_k, NULL, 0, &ThreadID);
    // (1)NULL - аттрибуты безопасности
    // (2)0 - размер стека
    // (4)NULL - нет аргумента для функ.- тела потока
    // (5)0 - флаг соз. потока
    if (fThread[9] == NULL)
    {
        std::cout << "ERROR - Create Thread: " << GetLastError();
        return 1;
    }

    for (int a = 0; a < 3; a++)
    {
        WaitForSingleObject(fMutex, INFINITE); // ждем пока поток  закончит работу
        std::cout << "i" << std::flush;
        computation();
        ReleaseMutex(fMutex);
    }

    WaitForSingleObject(fThread[6], INFINITE); //h wait

    // THREAD M
    fThread[10] = CreateThread(NULL, 0, Thread_m, NULL, 0, &ThreadID);
    // (1)NULL - аттрибуты безопасности
    // (2)0 - размер стека
    // (4)NULL - нет аргумента для функ.- тела потока
    // (5)0 - флаг соз. потока
    if (fThread[10] == NULL)
    {
        std::cout << "ERROR - Create Thread: " << GetLastError();
        return 1;
    }

    WaitForSingleObject(fThread[10], INFINITE); //m wait

    return 0;
}

DWORD WINAPI Thread_k(LPVOID lpParam)
{
    UNREFERENCED_PARAMETER(lpParam);
    ///////////////////5////////////////
    for (int a = 0; a < 3; a++)
    {
        WaitForSingleObject(fMutex, INFINITE); // ждем пока поток  закончит работу
        std::cout << "k" << std::flush;
        computation();
        ReleaseMutex(fMutex);
    }

    ReleaseSemaphore(sem_pause, 1, NULL);
    WaitForSingleObject(sem_next, INFINITE);
    //////////////////6///////////////////
    for (int a = 0; a < 3; a++)
    {
        WaitForSingleObject(fMutex, INFINITE); // ждем пока поток  закончит работу
        std::cout << "k" << std::flush;
        computation();
        ReleaseMutex(fMutex);
    }
    return 0;
}

DWORD WINAPI Thread_m(LPVOID lpParam)
{
    UNREFERENCED_PARAMETER(lpParam);
    /////////////////6///////////////////
    ReleaseSemaphore(sem_next, 1, NULL); // k

    for (int a = 0; a < 3; a++)
    {
        WaitForSingleObject(fMutex, INFINITE); // ждем пока поток  закончит работу
        std::cout << "m" << std::flush;
        computation();
        ReleaseMutex(fMutex);
    }

    WaitForSingleObject(fThread[9], INFINITE); //k wait

    return 0;
}

int lab3_init()
{
    fMutex = CreateMutex(NULL, 0, NULL);

    if (fMutex == NULL)
    {
        std::cout << "ERROR - Create Mutex: " << GetLastError();
        return 1;
    }

    sem_F = CreateSemaphore(NULL, 1, 1, NULL);
    if (sem_F == NULL)
    {
        std::cout << "ERROR - Create Semaphore: " << GetLastError();
        return 1;
    }
    sem_G = CreateSemaphore(NULL, 0, 1, NULL);
    if (sem_G == NULL)
    {
        std::cout << "ERROR - Create Semaphore: " << GetLastError();
        return 1;
    }
    sem_H = CreateSemaphore(NULL, 0, 1, NULL);
    if (sem_H == NULL)
    {
        std::cout << "ERROR - Create Semaphore: " << GetLastError();
        return 1;
    }

    sem_pause = CreateSemaphore(NULL, 0, 3, NULL);
    if (sem_pause == NULL)
    {
        std::cout << "ERROR - Create Semaphore: " << GetLastError();
        return 1;
    }

    sem_next = CreateSemaphore(NULL, 0, 3, NULL);
    if (sem_next == NULL)
    {
        std::cout << "ERROR - Create Semaphore: " << GetLastError();
        return 1;
    }

    //NULL аттрибуты безопасности по умолчанию
    //0 | 1 ...   начальное значение счетчика
    //0 | 1 ...   максимаьлное значение счетчика
    //NULL безымянный семафор

    //THREAD A
    fThread[0] = CreateThread(NULL, 0, Thread_a, NULL, 0, &ThreadID);
    // (1)NULL - аттрибуты безопасности
    // (2)0 - размер стека
    // (4)NULL - нет аргумента для функ.- тела потока
    // (5)0 - флаг соз. потока
    if (fThread[0] == NULL)
    {
        std::cout << "ERROR - Create Thread: " << GetLastError();
        return 1;
    }

    WaitForSingleObject(fThread[0], INFINITE); //a wait

    CloseHandle(fMutex);

    CloseHandle(sem_F);
    CloseHandle(sem_G);
    CloseHandle(sem_H);
    CloseHandle(sem_pause);
    CloseHandle(sem_next);

    return 0;
}
