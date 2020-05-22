#include "lab3.h"
#include <iostream>
#include <windows.h>


#define THREADCOUNT 11

DWORD ThreadID; // номер потоков     //интервал времени
HANDLE fThread[THREADCOUNT]; // сихрон. доступ в критическую секцию   //дескриптор объекта
HANDLE fMutex;

HANDLE sem_F, sem_G, sem_H; // семафоры для сихрон.

DWORD WINAPI Thread_a(LPVOID);
DWORD WINAPI Thread_b(LPVOID);
DWORD WINAPI Thread_c(LPVOID);
DWORD WINAPI Thread_d(LPVOID);
DWORD WINAPI Thread_e(LPVOID);
DWORD WINAPI Thread_f(LPVOID);
DWORD WINAPI Thread_f_semaphore(LPVOID);
DWORD WINAPI Thread_g(LPVOID);
DWORD WINAPI Thread_h(LPVOID);
DWORD WINAPI Thread_h_semaphore(LPVOID);
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

    WaitForSingleObject(fThread[1], INFINITE); //b wait
    WaitForSingleObject(fThread[3], INFINITE); //d wait

    CloseHandle(fThread[1]); // закрываем дескриптор потока
    CloseHandle(fThread[3]); // закрываем дескриптор потока

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

    CloseHandle(fThread[2]);

    return 0;
}

DWORD WINAPI Thread_b(LPVOID lpParam) {
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

    for (int a = 0; a < 3; a++)
    {
        WaitForSingleObject(fMutex, INFINITE); // ждем пока поток  закончит работу
        std::cout << "c" << std::flush;
        computation();
        ReleaseMutex(fMutex);
    }

    WaitForSingleObject(fThread[1], INFINITE); //b wait
    WaitForSingleObject(fThread[3], INFINITE); //d wait

    CloseHandle(fThread[1]); // закрываем дескриптор потока
    CloseHandle(fThread[3]); // закрываем дескриптор потока

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

    CloseHandle(fThread[4]); //e wait
    return 0;
}

DWORD WINAPI Thread_e(LPVOID lpParam) {
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

    for (int a = 0; a < 3; a++)
    {
        WaitForSingleObject(fMutex, INFINITE); // ждем пока поток  закончит работу
        std::cout << "e" << std::flush;
        computation();
        ReleaseMutex(fMutex);
    }

    WaitForSingleObject(fThread[3], INFINITE); //d wait
    WaitForSingleObject(fThread[5], INFINITE); //f wait
    WaitForSingleObject(fThread[6], INFINITE); //h wait

    CloseHandle(fThread[3]); // закрываем дескриптор потока
    CloseHandle(fThread[5]); // закрываем дескриптор потока
    CloseHandle(fThread[6]); // закрываем дескриптор потока

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

    CloseHandle(fThread[7]); // закрываем дескриптор потока
    return 0;
}

DWORD WINAPI Thread_f(LPVOID lpParam)
{
    for (int a = 0; a < 3; a++)
    {
        WaitForSingleObject(fMutex, INFINITE); // ждем пока поток  закончит работу
        std::cout << "f" << std::flush;
        computation();
        ReleaseMutex(fMutex);
    }
    return 0;
}

DWORD WINAPI Thread_h(LPVOID lpParam)
{
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
    // THREAD F
    fThread[5] = CreateThread(NULL, 0, Thread_f_semaphore, NULL, 0, &ThreadID);
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
    fThread[6] = CreateThread(NULL, 0, Thread_h_semaphore, NULL, 0, &ThreadID);
    // (1)NULL - аттрибуты безопасности
    // (2)0 - размер стека
    // (4)NULL - нет аргумента для функ.- тела потока
    // (5)0 - флаг соз. потока
    if (fThread[6] == NULL)
    {
        std::cout << "ERROR - Create Thread: " << GetLastError();
        return 1;
    }

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
    WaitForSingleObject(fThread[5], INFINITE); //f_semaphore wait
    WaitForSingleObject(fThread[6], INFINITE); //h_semaphore wait

    CloseHandle(fThread[5]); // закрываем дескриптор потока
    CloseHandle(fThread[6]); // закрываем дескриптор потока


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

    CloseHandle(fThread[8]);                   // закрываем дескриптор потока

    return 0;
}

DWORD WINAPI Thread_f_semaphore(LPVOID lpParam)
{
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

DWORD WINAPI Thread_h_semaphore(LPVOID lpParam)
{
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
    return 0;
}

DWORD WINAPI Thread_i(LPVOID lpParam)
{
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

    WaitForSingleObject(fThread[6], INFINITE); //f wait
    WaitForSingleObject(fThread[9], INFINITE); //k wait

    CloseHandle(fThread[6]); // закрываем дескриптор потока
    CloseHandle(fThread[9]); // закрываем дескриптор потока

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

    CloseHandle(fThread[10]); // закрываем дескриптор потока

    return 0;
}

DWORD WINAPI Thread_k(LPVOID lpParam)
{
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
        std::cout << "m" << std::flush;
        computation();
        ReleaseMutex(fMutex);
    }

    WaitForSingleObject(fThread[9], INFINITE); //k wait

    CloseHandle(fThread[9]); // закрываем дескриптор потока
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

    //NULL аттрибуты безопасности по умолчанию
    //0 | 1   начальное значение счетчика
    //0 | 1   максимаьлное значение счетчика
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

    CloseHandle(fThread[0]); // закрываем дескриптор потока

    CloseHandle(fMutex);

    CloseHandle(sem_F);
    CloseHandle(sem_G);
    CloseHandle(sem_H);

    return 0;
}
